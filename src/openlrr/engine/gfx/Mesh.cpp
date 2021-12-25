// Mesh.cpp : 
//

#include "../../platform/d3drm.h"

#include "Containers.h"
#include "Lwt.h"
#include "Viewports.h"
#include "../core/Errors.h"
#include "../core/Files.h"
#include "../core/Maths.h"
#include "../core/Memory.h"
#include "../core/Utils.h"
#include "../Graphics.h"

#include "Mesh.h"


#define RELEASE(x) { if (x != nullptr) {x->Release(); x = nullptr;} }


/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @005353c0>
Gods98::Mesh_Globs & Gods98::meshGlobs = *(Gods98::Mesh_Globs*)0x005353c0; // = { nullptr };

Gods98::Mesh_ListSet Gods98::meshListSet = Gods98::Mesh_ListSet(Gods98::meshGlobs);

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @00480870>
bool32 __cdecl Gods98::Mesh_Initialise(const char* sharedTextureDir)
{
	// These two lines conflict with each other.
	// Mesh_ClearPostRenderList only does something if postRenderMeshList is non-null.
	meshGlobs.postRenderMeshList = nullptr;
	Mesh_ClearPostRenderList();


	if (sharedTextureDir) {
		uint32 len = std::strlen(sharedTextureDir);
		if (sharedTextureDir[len-1] != '\\') len++; // Add room for trailing backslash
		meshGlobs.sharedTextureDir = (char*)Mem_Alloc(len+1); // +1 for null terminator
		std::strcpy(meshGlobs.sharedTextureDir, sharedTextureDir);
		meshGlobs.sharedTextureDir[len-1] = '\\';
		meshGlobs.sharedTextureDir[len] = '\0';
	}

	meshListSet.Initialise();

	Mesh_CreateGlobalMaterial();

	return true;
}

// <LegoRR.exe @00480910>
bool32 __cdecl Gods98::Mesh_CreateGlobalMaterial(void)
{
	D3DMATERIAL material = {
		sizeof(D3DMATERIAL),
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		/// OLD GODS98: Ambient set to 0.0f for RGBA
		{ 0.0f, 0.0f, 0.0f, 0.0f },
		//{ 1.0f, 1.0f, 1.0f, 1.0f },
		{ 0.0f, 0.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f, 0.0f },
		0.0f,
		0,
		0,
	};


	Mesh_Debug_CheckIMDevice_Int();

	IDirect3D3* imd3d;
	lpIMDevice()->GetDirect3D(&imd3d);

	if (imd3d->CreateMaterial(&meshGlobs.imMat, nullptr) != D3D_OK) {
		Error_Warn(true, "Cannot 'CreateMaterial'.");
		RELEASE(imd3d);

		return false;
	}

	if (meshGlobs.imMat->GetHandle(lpIMDevice(), &meshGlobs.matHandle) != D3D_OK) {
		Error_Warn(true, "Cannot 'GetHandle' for material.");
		RELEASE(imd3d);

		return false;
	}

	//SET UP ANY MATERIAL AS DEFAULT
	Mesh_SetMaterial(&material);

	RELEASE(imd3d);

	return true;
}

// <LegoRR.exe @00480a40>
bool32 __cdecl Gods98::Mesh_SetMaterial(D3DMATERIAL* newMaterial)
{
	Mesh_Debug_CheckIMDevice_Int();

	if (meshGlobs.imMat->SetMaterial(newMaterial) != D3D_OK) {
		Error_Warn(true, "Cannot 'SetMaterial'.");

		return false;
	}

	return true;
}

// <LegoRR.exe @00480a60>
Gods98::Mesh* __cdecl Gods98::Mesh_ObtainFromList(void)
{
	Mesh* newMesh = meshListSet.Add();
	ListSet::MemZero(newMesh); // Clear memory before returning.

	return newMesh;
}

// <LegoRR.exe @00480a90>
void __cdecl Gods98::Mesh_ReturnToList(Mesh* dead)
{
	meshListSet.Remove(dead);
}

// <LegoRR.exe @00480ab0>
void __cdecl Gods98::Mesh_AddList(void)
{
	// NOTE: This function is no longer called, meshListSet.Add already handles this.
	meshListSet.AddList();
}

// <LegoRR.exe @00480b30>
Gods98::Mesh* __cdecl Gods98::Mesh_CreateOnFrame(IDirect3DRMFrame3* frame, MeshRenderCallback renderFunc,
												 Mesh_RenderFlags renderFlags, void* data, Mesh_Type type)
{
	Mesh* mesh = Mesh_ObtainFromList();
	Container* rootCont = Container_GetRoot();


	Mesh_Debug_CheckIMDevice_Ptr(Mesh*);

	lpD3DRM()->CreateUserVisual(Mesh_RenderCallback, mesh, &mesh->uv);
	Container_NoteCreation(mesh->uv);
	frame->AddVisual((IUnknown*)mesh->uv);
	mesh->uv->SetAppData((DWORD)mesh);
	mesh->frameCreatedOn = frame;

	mesh->renderDesc.renderCallback = renderFunc;
	mesh->renderDesc.renderCallbackData = data;
	mesh->renderDesc.renderFlags = renderFlags;

	if (type == Mesh_Type::PostEffect)
		mesh->flags |= MeshFlags::MESH_FLAG_POSTEFFECT;
	else if (type == Mesh_Type::LightWaveObject)
		mesh->flags |= MeshFlags::MESH_FLAG_LWO;

	mesh->numOfRefs = 1;

	/// NEW GODS98: Not in LegoRR
	//mesh->textureRenderCallback = nullptr;

	return mesh;
}

// <LegoRR.exe @00480bc0>
Gods98::Mesh* __cdecl Gods98::Mesh_Clone(Gods98::Mesh* mesh, IDirect3DRMFrame3* frame)
{
	Mesh* newMesh = Mesh_ObtainFromList();
	HRESULT r;

	if (mesh->clonedFrom) mesh = mesh->clonedFrom;

	/// FIX APPLY: Copy mesh without overwriting nextFree field.
	ListSet::MemCopy(newMesh, mesh);
	newMesh->clonedFrom = mesh;


	newMesh->listSize = newMesh->groupCount;
	newMesh->groupList = (Mesh_Group*)Mem_Alloc(sizeof(Mesh_Group) * newMesh->groupCount);
	std::memcpy(newMesh->groupList, mesh->groupList, sizeof(Mesh_Group) * newMesh->groupCount);

	for (uint32 loop = 0; loop < newMesh->groupCount; loop++) {
		Mesh_CreateGroupMaterial(newMesh, loop);
		Mesh_SetGroupMaterial(newMesh, loop, &mesh->groupList[loop].material);
	}

	r = lpD3DRM()->CreateUserVisual(Mesh_RenderCallback, newMesh, &newMesh->uv);
	Container_NoteCreation(newMesh->uv);
	frame->AddVisual((IUnknown*)newMesh->uv);
	newMesh->uv->SetAppData((DWORD)newMesh);
	newMesh->frameCreatedOn = frame;

	mesh->numOfRefs++;
	mesh->flags |= MeshFlags::MESH_FLAG_HASBEENCLONED;

	newMesh->numOfRefs = 0;

	return newMesh;
}

// <LegoRR.exe @00480ca0>
Gods98::Mesh* __cdecl Gods98::Mesh_Load(const char* fname, IDirect3DRMFrame3* frame, bool32 noTextures)
{
	char path[FILE_MAXPATH];

	std::strcpy(path, fname);

	char* t;
	for (char* s = t = path; *s != '\0'; s++) {

		if (*s == '\\') t = s;
	}
	t[(t!=path ? 1 : 0)] = '\0'; // Remove final backslash to get directory name.

	APPOBJ* lightWaveObject;
	if (LoadAppObj(fname, &lightWaveObject, false)) {
		Mesh* mesh = Mesh_CreateOnFrame(frame, nullptr, Mesh_RenderFlags::MESH_RENDERFLAGS_LWOALPHA, nullptr, Mesh_Type::LightWaveObject);
		Mesh_ParseLWO(path, mesh, lightWaveObject, noTextures);
		FreeLWOB(lightWaveObject);

		return mesh;
	}

	return nullptr;
}

// <LegoRR.exe @00480d80>
bool32 __cdecl Gods98::Mesh_ParseLWO(const char* basePath, Mesh* mesh, APPOBJ* lightWaveObject, bool32 noTextures)
{
	const uint32 faceDataQuad[] = { 0, 1, 2, 0, 2, 3 };

	// UNUSED:
	//real32 textCoords[][2] = {
	//	{ 0, 1 }, { 0, 0 }, { 1, 0 }, { 1, 1 }
	//};

	Vector3F* surfVertices;
	Vector3F** surfVerticesNorm;
	uint32* surfVertexCount;
	uint32* surfVertexRef;
	uint32* surfFaceData;
	uint32* surfFaceDataCount;
	Vector3F* faceNorm;
	Vector3F* vertexNorm;

	Point2F* vertexUVs = nullptr;
	Point2F* surfTextCoords;
	uint32 size = (lightWaveObject->aoSize.lwVertCount * 2);
	uint32 fDSize = (lightWaveObject->aoSize.lwPolyCount * 6);
//	bool32 first = true;
	uint32 surfArraySize, surfFDArraySize;


	Error_Fatal(!lightWaveObject->aoSize.lwSurfaceCount, "LightWave object must contain at least one surface.");

#define MESH_MAXPOLYSPERVETEX		20

	//GET TEXTURE AND TEXTURE COORDINATES FOR VERTICES IF UVIEW FILE EXISTS
	//NEED TO CALL THIS FIRST BECAUSE THE LIGHTWAVE STRUCTURE MAY BE ALTERED
	if (lightWaveObject->aoFileUV) {
		vertexUVs = (Point2F*)Mem_Alloc(sizeof(Point2F) * size * MESH_MAXPOLYSPERVETEX);
		Mesh_UViewMesh(lightWaveObject, vertexUVs);
		Error_Fatal(size * MESH_MAXPOLYSPERVETEX < lightWaveObject->aoSize.lwVertCount, "MESH_MAXPOLYSPERVETEX too small");

		size = (lightWaveObject->aoSize.lwVertCount * 2);
		surfTextCoords = (Point2F*)Mem_Alloc(sizeof(Point2F) * lightWaveObject->aoSize.lwSurfaceCount * size);
	}
	else {
		surfTextCoords = (Point2F*)Mem_Alloc(sizeof(Point2F) * size);
	}


	//ALLOCATE MEMORY FOR OBJECT
	surfArraySize = (lightWaveObject->aoSize.lwSurfaceCount * size);

	surfVertices      =  (Vector3F*)Mem_Alloc(sizeof(Vector3F) * surfArraySize);
	surfVerticesNorm  = (Vector3F**)Mem_Alloc(sizeof(Vector3F*) * surfArraySize); // yes, sizeof(pointer), not sizeof(Vector3F)
	surfVertexCount   =    (uint32*)Mem_Alloc(sizeof(uint32) * lightWaveObject->aoSize.lwSurfaceCount);
	surfVertexRef     =    (uint32*)Mem_Alloc(sizeof(uint32) * surfArraySize);
	surfFaceData      =    (uint32*)Mem_Alloc(sizeof(uint32) * (lightWaveObject->aoSize.lwSurfaceCount * fDSize));
	surfFaceDataCount =    (uint32*)Mem_Alloc(sizeof(uint32) * lightWaveObject->aoSize.lwSurfaceCount);
	faceNorm          =  (Vector3F*)Mem_Alloc(sizeof(Vector3F) * lightWaveObject->aoSize.lwPolyCount);
	vertexNorm        =  (Vector3F*)Mem_Alloc(sizeof(Vector3F) * size);

	std::memset(surfVertexCount,   0, sizeof(uint32) * lightWaveObject->aoSize.lwSurfaceCount);
	// Although memset is assigning -1 as a byte (0xff), it still creates an identical uint32 of -1 (0xffffffff)
	std::memset(surfVertexRef,    -1, sizeof(uint32) * surfArraySize);
	std::memset(surfFaceDataCount, 0, sizeof(uint32) * lightWaveObject->aoSize.lwSurfaceCount);
	std::memset(vertexNorm,        0, sizeof(Vector3F) * size);


	mesh->lightWaveSurf = (Mesh_LightWave_Surface*)Mem_Alloc(sizeof(Mesh_LightWave_Surface) * lightWaveObject->aoSize.lwSurfaceCount);
	std::memset(mesh->lightWaveSurf, 0, sizeof(Mesh_LightWave_Surface) * lightWaveObject->aoSize.lwSurfaceCount);


	//STORE FACE NORMALS FOR GOURAUD SHADING - ASSUMES ALL POLYS ARE PLANAR
	for (uint32 face = 0; face < lightWaveObject->aoSize.lwPolyCount; face++) {

		Vector3F pos[3];
		for (uint32 vertex = 0; vertex < 3; vertex++) {
			uint32 vertexIndex = lightWaveObject->aoPoly[face].plyData[vertex] * 3;

			pos[vertex].x = lightWaveObject->aoVerts[vertexIndex];
			pos[vertex].y = lightWaveObject->aoVerts[vertexIndex + 1];
			pos[vertex].z = lightWaveObject->aoVerts[vertexIndex + 2];

		}
		Maths_PlaneNormal(&faceNorm[face], &pos[0], &pos[1], &pos[2]);
	}


//	//GET TEXTURE AND TEXTURE COORDINATES FOR VERTICES IF UVIEW FILE EXISTS
//	if (lightWaveObject->aoFileUV) {
//		vertexUVs      = (Point2F*)Mem_Alloc(sizeof(Point2F) * size);
//		surfTextCoords = (Point2F*)Mem_Alloc(sizeof(Point2F) * (lightWaveObject->aoSize.lwSurfaceCount * size));
//
//		Mesh_UViewMesh(lightWaveObject, vertexUVs);
//	}
//	else {
//		surfTextCoords = (Point2F*)Mem_Alloc(sizeof(Point2F) * size);
//	}

	//READ SURFACE INFORMATION
	Mesh_GetSurfInfo(basePath, lightWaveObject, mesh->lightWaveSurf, noTextures);


	//READ VERTEX INFORMATION
	for (uint32 face = 0; face < lightWaveObject->aoSize.lwPolyCount; face++) {

		surfArraySize = lightWaveObject->aoPoly[face].plySurface * size;
		surfFDArraySize = lightWaveObject->aoPoly[face].plySurface * fDSize;

		if (lightWaveObject->aoPoly[face].plyCount == 3) {
			for (uint32 vertex = 0; vertex < 3; vertex++) {
				uint32 vertexIndex = lightWaveObject->aoPoly[face].plyData[vertex] * 3;

				if (surfVertexRef[surfArraySize + lightWaveObject->aoPoly[face].plyData[vertex]] == -1) {

					//STORE NEW VERTEX IN SURFACE LIST
					surfVertices[surfArraySize + surfVertexCount[lightWaveObject->aoPoly[face].plySurface]].x
						= lightWaveObject->aoVerts[vertexIndex];
					surfVertices[surfArraySize + surfVertexCount[lightWaveObject->aoPoly[face].plySurface]].y
						= lightWaveObject->aoVerts[vertexIndex + 1];
					surfVertices[surfArraySize + surfVertexCount[lightWaveObject->aoPoly[face].plySurface]].z
						= lightWaveObject->aoVerts[vertexIndex + 2];

					if (lightWaveObject->aoFileUV) {
						surfTextCoords[surfArraySize + surfVertexCount[lightWaveObject->aoPoly[face].plySurface]]
							= vertexUVs[lightWaveObject->aoPoly[face].plyData[vertex]];
					}

//					if (first) {
//						mesh->boundingBox.min = surfVertices[ (surfArraySize) + surfVertexCount[lightWaveObject->aoPoly[face].plySurface] ];
//						mesh->boundingBox.max = surfVertices[ (surfArraySize) + surfVertexCount[lightWaveObject->aoPoly[face].plySurface] ];
//						first = false;
//					}
//					else {
//						Mesh_UpdateBoundingBox(&mesh->boundingBox, &surfVertices[ (surfArraySize) + surfVertexCount[lightWaveObject->aoPoly[face].plySurface] ]);
//					}

					//ADD VERTEX FACE NORMAL TO CURRENT VERTEX NORMAL
					Maths_Vector3DAdd(&vertexNorm[lightWaveObject->aoPoly[face].plyData[vertex]],
									  &vertexNorm[lightWaveObject->aoPoly[face].plyData[vertex]],
									  &faceNorm[face]);
					Maths_Vector3DNormalise(&vertexNorm[lightWaveObject->aoPoly[face].plyData[vertex]]);
					surfVerticesNorm[surfArraySize + surfVertexCount[lightWaveObject->aoPoly[face].plySurface]]
						= &vertexNorm[lightWaveObject->aoPoly[face].plyData[vertex]];

					//STORE NEW VERTEX IN FACE DATA
					surfFaceData[surfFDArraySize + surfFaceDataCount[lightWaveObject->aoPoly[face].plySurface]++]
						= surfVertexCount[lightWaveObject->aoPoly[face].plySurface];

					//STORE NEW VERTEX REFERENCE IN VERTEX LIST
					surfVertexRef[surfArraySize + lightWaveObject->aoPoly[face].plyData[vertex]]
						= surfVertexCount[lightWaveObject->aoPoly[face].plySurface]++;

				}
				else {
					//ADD VERTEX FACE NORMAL TO CURRENT VERTEX NORMAL
					Maths_Vector3DAdd(&vertexNorm[lightWaveObject->aoPoly[face].plyData[vertex]],
									  &vertexNorm[lightWaveObject->aoPoly[face].plyData[vertex]],
									  &faceNorm[face]);
					Maths_Vector3DNormalise(&vertexNorm[lightWaveObject->aoPoly[face].plyData[vertex]]);

					//GET VERTEX REFERENCE AND COPY TO FACE DATA
					surfFaceData[surfFDArraySize + surfFaceDataCount[lightWaveObject->aoPoly[face].plySurface]++]
						= surfVertexRef[surfArraySize + lightWaveObject->aoPoly[face].plyData[vertex]];

				}

			}

		}
//		else if (lightWaveObject->aoPoly[face].plyCount == 4) {
		else {
			Error_Warn(((lightWaveObject->aoPoly[face].plyCount != 4) && (lightWaveObject->aoPoly[face].plyCount != 3)),
					   "Mesh contains polys which are not triples or quads.");

			surfArraySize = lightWaveObject->aoPoly[face].plySurface * size;

			for (uint32 vertex = 0; vertex < 6; vertex++) {

				if (surfVertexRef[surfArraySize + lightWaveObject->aoPoly[face].plyData[faceDataQuad[vertex]]] == -1) {

					uint32 vertexIndex = lightWaveObject->aoPoly[face].plyData[faceDataQuad[vertex]] * 3;

					//STORE NEW VERTEX IN SURFACE LIST
					surfVertices[surfArraySize + surfVertexCount[lightWaveObject->aoPoly[face].plySurface]].x
						= lightWaveObject->aoVerts[vertexIndex];
					surfVertices[surfArraySize + surfVertexCount[lightWaveObject->aoPoly[face].plySurface]].y
						= lightWaveObject->aoVerts[vertexIndex + 1];
					surfVertices[surfArraySize + surfVertexCount[lightWaveObject->aoPoly[face].plySurface]].z
						= lightWaveObject->aoVerts[vertexIndex + 2];

					if (lightWaveObject->aoFileUV) {
						surfTextCoords[surfArraySize + surfVertexCount[lightWaveObject->aoPoly[face].plySurface]] = vertexUVs[lightWaveObject->aoPoly[face].plyData[faceDataQuad[vertex]]];
					}

//					if (first) {
//						mesh->boundingBox.min = surfVertices[ (surfArraySize) + surfVertexCount[lightWaveObject->aoPoly[face].plySurface] ];
//						mesh->boundingBox.max = surfVertices[ (surfArraySize) + surfVertexCount[lightWaveObject->aoPoly[face].plySurface] ];
//						first = false;
//					}
//					else {
//						Mesh_UpdateBoundingBox( &mesh->boundingBox, &surfVertices[ (surfArraySize) + surfVertexCount[lightWaveObject->aoPoly[face].plySurface] ] );
//					}

					//ADD VERTEX FACE NORMAL TO CURRENT VERTEX NORMAL
					Maths_Vector3DAdd(&vertexNorm[lightWaveObject->aoPoly[face].plyData[faceDataQuad[vertex]]],
									  &vertexNorm[lightWaveObject->aoPoly[face].plyData[faceDataQuad[vertex]]],
									  &faceNorm[face]);
					Maths_Vector3DNormalise(&vertexNorm[lightWaveObject->aoPoly[face].plyData[faceDataQuad[vertex]]]);
					surfVerticesNorm[surfArraySize + surfVertexCount[lightWaveObject->aoPoly[face].plySurface]]
						= &vertexNorm[lightWaveObject->aoPoly[face].plyData[faceDataQuad[vertex]]];

					//STORE NEW VERTEX IN FACE DATA
					surfFaceData[surfFDArraySize + surfFaceDataCount[lightWaveObject->aoPoly[face].plySurface]++]
						= surfVertexCount[lightWaveObject->aoPoly[face].plySurface];

					//STORE NEW VERTEX REFERENCE IN VERTEX LIST
					surfVertexRef[surfArraySize + lightWaveObject->aoPoly[face].plyData[faceDataQuad[vertex]]]
						= surfVertexCount[lightWaveObject->aoPoly[face].plySurface]++;

				}
				else {
					//ADD VERTEX FACE NORMAL TO CURRENT VERTEX NORMAL
					Maths_Vector3DAdd(&vertexNorm[lightWaveObject->aoPoly[face].plyData[faceDataQuad[vertex]]],
									  &vertexNorm[lightWaveObject->aoPoly[face].plyData[faceDataQuad[vertex]]],
									  &faceNorm[face]);
					Maths_Vector3DNormalise(&vertexNorm[lightWaveObject->aoPoly[face].plyData[faceDataQuad[vertex]]]);

					//GET VERTEX REFERENCE AND COPY TO FACE DATA
					surfFaceData[surfFDArraySize + surfFaceDataCount[lightWaveObject->aoPoly[face].plySurface]++]
						= surfVertexRef[surfArraySize + lightWaveObject->aoPoly[face].plyData[faceDataQuad[vertex]]];

				}

			}

		}
//		else Error_Fatal( true, "LightWave object contains corrupt data. Use only triples or quads." );

	}


	//ADD A GROUP FOR EACH SURFACE IN OBJECT
	for (uint32 loop = 0; loop < lightWaveObject->aoSize.lwSurfaceCount; loop++) {

		surfArraySize = loop * size;
		surfFDArraySize = loop * fDSize;

		uint32 group = Mesh_AddGroup(mesh, surfFaceDataCount[loop], surfFaceDataCount[loop]/3, 3, &surfFaceData[surfFDArraySize]);

		//SET NORMALS
		if (!lightWaveObject->aoFileUV) {
			Mesh_GetTextureUVsWrap(surfVertexCount[loop], &surfVertices[surfArraySize], surfTextCoords,
								   lightWaveObject->aoSurface[loop].srfTexSize.tdX, lightWaveObject->aoSurface[loop].srfTexSize.tdY, lightWaveObject->aoSurface[loop].srfTexSize.tdZ,
								   lightWaveObject->aoSurface[loop].srfTexCentre.tdX, lightWaveObject->aoSurface[loop].srfTexCentre.tdY, lightWaveObject->aoSurface[loop].srfTexCentre.tdZ,
								   lightWaveObject->aoSurface[loop].srfTexFlags);

			Mesh_SetVertices_VNT(mesh, group, 0, surfVertexCount[loop], &surfVertices[surfArraySize], &surfVerticesNorm[surfArraySize], surfTextCoords);
		}
		else {
			Mesh_SetVertices_VNT(mesh, group, 0, surfVertexCount[loop], &surfVertices[surfArraySize], &surfVerticesNorm[surfArraySize], &surfTextCoords[surfArraySize]);
		}

		//ONLY ALTER RENDERING FLAGS FOR GROUP IF NEEDED
		Mesh_RenderFlags newFlags = Mesh_RenderFlags::MESH_RENDERFLAGS_LWONORM;

		if (mesh->lightWaveSurf[loop].flags & LightWave_SurfFlags::SFM_ADDITIVE) {
			newFlags |= Mesh_RenderFlags::MESH_RENDER_FLAG_ALPHASA1;
			Mesh_AlterGroupRenderFlags(mesh, group, newFlags);
		}
		else {
			newFlags |= Mesh_RenderFlags::MESH_RENDER_FLAG_ALPHATRANS;
		}

		if (mesh->lightWaveSurf[loop].flags & LightWave_SurfFlags::SFM_DOUBLESIDED) {
			newFlags |= Mesh_RenderFlags::MESH_RENDER_FLAG_DOUBLESIDED;
			Mesh_AlterGroupRenderFlags(mesh, group, newFlags);
		}

		/// OLD GODS98: This check (and applying of newFlags was commented out in Gods98 source,
		///             while the call to Mesh_AlterGroupRenderFlags was not.
		if (!(mesh->lightWaveSurf[loop].texFlags & LightWave_TexFlags::TFM_PIXEL_BLENDING)) {
			newFlags |= Mesh_RenderFlags::MESH_RENDER_FLAG_FILTERNEAREST;
			Mesh_AlterGroupRenderFlags(mesh, group, newFlags);
		}

		/// TODO: Are flags intended to always be modified on not SFM_ADDITIVE?
		///       Or only if one of the remaining conditions are met?
		//if (newFlags != Mesh_RenderFlags::MESH_RENDERFLAGS_LWONORM) {
		//	Mesh_AlterGroupRenderFlags(mesh, group, newFlags);
		//}

		//SET TEXTURE
		if ((lightWaveObject->aoSurface[loop].srfTexFlags & LightWave_TexFlags::TFM_SEQUENCE)) {

			if (mesh->lightWaveSurf[loop].textureSeq)
				Mesh_SetGroupTexture(mesh, group, mesh->lightWaveSurf[loop].textureSeq[0]);
		}
		else {
			if (mesh->lightWaveSurf[loop].texture)
				Mesh_SetGroupTexture(mesh, group, mesh->lightWaveSurf[loop].texture);
		}

		//SET COLOUR
		Mesh_SetGroupDiffuse(mesh, group,
							 mesh->lightWaveSurf[loop].colour.red   * mesh->lightWaveSurf[loop].diffuse,
							 mesh->lightWaveSurf[loop].colour.green * mesh->lightWaveSurf[loop].diffuse,
							 mesh->lightWaveSurf[loop].colour.blue  * mesh->lightWaveSurf[loop].diffuse);

		Mesh_SetGroupEmissive(mesh, group,
							  mesh->lightWaveSurf[loop].colour.red   * mesh->lightWaveSurf[loop].emissive,
							  mesh->lightWaveSurf[loop].colour.green * mesh->lightWaveSurf[loop].emissive,
							  mesh->lightWaveSurf[loop].colour.blue  * mesh->lightWaveSurf[loop].emissive);

		if (mesh->lightWaveSurf[loop].flags & LightWave_SurfFlags::SFM_COLORHIGHLIGHTS) {
			Mesh_SetGroupSpecular(mesh, group,
								  mesh->lightWaveSurf[loop].colour.red   * mesh->lightWaveSurf[loop].specular,
								  mesh->lightWaveSurf[loop].colour.green * mesh->lightWaveSurf[loop].specular,
								  mesh->lightWaveSurf[loop].colour.blue  * mesh->lightWaveSurf[loop].specular);
		}
		else {
			Mesh_SetGroupSpecular(mesh, group,
								  mesh->lightWaveSurf[loop].specular,
								  mesh->lightWaveSurf[loop].specular,
								  mesh->lightWaveSurf[loop].specular);
		}
		Mesh_SetGroupPower(mesh, group, mesh->lightWaveSurf[loop].power * 10.0f);
		Mesh_SetGroupAlpha(mesh, group, 1.0f - mesh->lightWaveSurf[loop].transparency);

		//STORE SURFACE INFORMATION FOR GROUP
		mesh->groupList[group].lightWaveSurfaceInfo = &mesh->lightWaveSurf[loop];

	}

	//FREE ALLOCATED MEMORY
	Mem_Free(surfVertices);
	Mem_Free(surfVerticesNorm);
	Mem_Free(surfTextCoords);
	Mem_Free(surfVertexCount);
	Mem_Free(surfVertexRef);
	Mem_Free(surfFaceData);
	Mem_Free(surfFaceDataCount);
	Mem_Free(faceNorm);
	Mem_Free(vertexNorm);
	if (vertexUVs) Mem_Free(vertexUVs);

	return true;
}

// <LegoRR.exe @00481ae0>
void __cdecl Gods98::Mesh_GetSurfInfo(const char* basePath, APPOBJ* lightWaveObject, Mesh_LightWave_Surface* lightWaveSurf/*[]*/, bool32 noTextures)
{
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	char path[_MAX_PATH];


	for (uint32 loopSurf = 0; loopSurf < lightWaveObject->aoSize.lwSurfaceCount; loopSurf++) {
		if (lightWaveObject->aoSurface[loopSurf].srfPath && !noTextures) {

			::_splitpath(lightWaveObject->aoSurface[loopSurf].srfPath, drive, dir, fname, ext);

			if (lightWaveObject->aoSurface[loopSurf].srfTexFlags & LightWave_TexFlags::TFM_SEQUENCE) {

				char baseName[_MAX_FNAME], textName[_MAX_FNAME];
				uint32 textNum, numOfDigits;

				//FIND NUM OF TEXTURES IN SEQUENCE
				if (Mesh_GetTextureSeqInfo(fname, baseName, &textNum, &numOfDigits)) {
					uint32 tempNum = textNum;
					uint32 numInTexSeq = 0;

					/*do {
						Mesh_GetNextInSequence(baseName, textName, &textNum, numOfDigits);
						std::sprintf(path, "%s%s%s", basePath, textName, ext);

						numInTexSeq++;
					} while(File_Exists(path));

					if (--numInTexSeq) {
					
						//LOAD TEXTURE SEQUENCE
						lightWaveSurf[loopSurf].textureSeq = (Mesh_Texture**)Mem_Alloc(sizeof(Mesh_Texture*) * (numInTexSeq));

						for (uint32 frame = 0; frame < numInTexSeq; frame++) {
							Mesh_GetNextInSequence(baseName, textName, &tempNum, numOfDigits);
							std::sprintf(path, "%s%s", textName, ext);
							lightWaveSurf[loopSurf].textureSeq[frame] = Mesh_LoadTexture(basePath, path, nullptr, nullptr);
						}

					}
					else {
						lightWaveSurf[loopSurf].textureSeq = nullptr;
						Error_Fatal(true, Error_Format("No textures found in sequence (%s)", path));
					}*/

					lightWaveSurf[loopSurf].textureSeq = (Mesh_Texture**)Mem_Alloc(sizeof(Mesh_Texture*) * MESH_MAXTEXTURESEQENCE);
					for (numInTexSeq = 0; numInTexSeq < MESH_MAXTEXTURESEQENCE; numInTexSeq++) {

						Mesh_GetNextInSequence(baseName, textName, &textNum, numOfDigits);
						std::sprintf(path, "%s%s", textName, ext);
						if ((lightWaveSurf[loopSurf].textureSeq[numInTexSeq] = Mesh_LoadTexture(basePath, path, nullptr, nullptr)) == nullptr) {
							break;
						}
					}

					// Mem_Realloc() will set 'textureSeq' to NULL if 'numInTexSeq' is zero...
					lightWaveSurf[loopSurf].numInTexSeq = numInTexSeq;
					lightWaveSurf[loopSurf].textureSeq = (Mesh_Texture**)Mem_ReAlloc(lightWaveSurf[loopSurf].textureSeq, sizeof(Mesh_Texture*) * numInTexSeq);
				}
				else {
					Error_Fatal(true, Error_Format("Error loading texture sequence \"%s\".", fname));
				}
			}
			else {
				//LOAD SINGLE TEXTURE
				std::sprintf(path, "%s%s", fname, ext);

				lightWaveSurf[loopSurf].texture = Mesh_LoadTexture(basePath, path, nullptr, nullptr);
			}

			if (lightWaveObject->aoSurface[loopSurf].srfFlags & LightWave_SurfFlags::SFM_SHARPTERMINATOR) {
				//IF SHARP TERMINATOR FLAG IS SET THEN MIX TEXTURE COLOUR WITH DIFFUSE
				lightWaveSurf[loopSurf].colour.red   = lightWaveObject->aoSurface[loopSurf].srfCol.colRed   / 256.0f;
				lightWaveSurf[loopSurf].colour.green = lightWaveObject->aoSurface[loopSurf].srfCol.colGreen / 256.0f;
				lightWaveSurf[loopSurf].colour.blue  = lightWaveObject->aoSurface[loopSurf].srfCol.colBlue  / 256.0f;
				//lightWaveSurf[loopSurf].colour.alpha = lightWaveObject->aoSurface[loopSurf].srfCol.colAlpha / 256.0f;
			}
			else {
				lightWaveSurf[loopSurf].colour.red   = 1.0f;
				lightWaveSurf[loopSurf].colour.green = 1.0f;
				lightWaveSurf[loopSurf].colour.blue  = 1.0f;
				//lightWaveSurf[loopSurf].colour.alpha = 1.0f;
			}
		}
		else {
			lightWaveSurf[loopSurf].colour.red   = lightWaveObject->aoSurface[loopSurf].srfCol.colRed / 256.0f;
			lightWaveSurf[loopSurf].colour.green = lightWaveObject->aoSurface[loopSurf].srfCol.colGreen / 256.0f;
			lightWaveSurf[loopSurf].colour.blue  = lightWaveObject->aoSurface[loopSurf].srfCol.colBlue / 256.0f;
			//lightWaveSurf[loopSurf].colour.alpha = lightWaveObject->aoSurface[loopSurf].srfCol.colAlpha / 256.0f;
		}

		lightWaveSurf[loopSurf].emissive = lightWaveObject->aoSurface[loopSurf].srfLuminous;
		lightWaveSurf[loopSurf].transparency = lightWaveObject->aoSurface[loopSurf].srfTransparent;
		lightWaveSurf[loopSurf].diffuse  = lightWaveObject->aoSurface[loopSurf].srfDiffuse;
		lightWaveSurf[loopSurf].specular = lightWaveObject->aoSurface[loopSurf].srfSpecular;
		lightWaveSurf[loopSurf].power    = lightWaveObject->aoSurface[loopSurf].srfSpecPower;
		lightWaveSurf[loopSurf].flags    = lightWaveObject->aoSurface[loopSurf].srfFlags;
		lightWaveSurf[loopSurf].texFlags = lightWaveObject->aoSurface[loopSurf].srfTexFlags;
	}
}

// <LegoRR.exe @00481d80>
bool32 __cdecl Gods98::Mesh_GetTextureSeqInfo(const char* tname, OUT char* tfname, OUT uint32* tstart, OUT uint32* tnumlen)
{
	uint32 len = std::strlen(tname);

	// Parse a decimal number
	
	char c = tname[len - 1];
	if (c < '0' || c > '9')
		return false; // Does not end with decimal digit

	uint32 n, indx = 1, val = 0;

	for (n = len - 1; n > 0; n--) {
		c = tname[n];
		if (c < '0' || c > '9')
			break;
		else
			val += (c - '0') * indx;

		indx *= 10;
	}

	if (n == 0)
		return false; // Does not start with decimal digit

	std::strcpy(tfname, tname);
	tfname[n + 1] = '\0';
	*tnumlen = (len - n - 1);
	*tstart = val;
	return true;
}

// <LegoRR.exe @00481e40>
void __cdecl Gods98::Mesh_GetNextInSequence(const char* baseName, OUT char* nextTextName, OUT uint32* texNum, uint32 tnumlen)
{
	char numBuff[16];

	std::strcpy(nextTextName, baseName);

	std::sprintf(numBuff, "%d", (*texNum));

	// Perform zero-padding before concatenating the formatted number
	for (uint32 k = (tnumlen - std::strlen(numBuff)); k >= 1; k--) {
		std::strcat(nextTextName, "0");
	}

	(*texNum)++;

	std::strcat(nextTextName, numBuff);
}

// <LegoRR.exe @00481f10>
void __cdecl Gods98::Mesh_UViewMesh(APPOBJ* lightWaveObject, OUT Point2F* textCoords)
{
	char line[1024];
	char lineSplit[1024];
	uint32 groupID;
	char* argv[5];
	sint32 argc, lineNum = 0;
	sint32 polyRead = -2;
	sint32 surfRead = -1;
	sint32 vertexRead = -1;
	uint32 addedCount = 0;
	bool32 notSame;
	real32 uvX, uvY;
	uint32 vertexIndex, newIndex;

	File* fileUV = lightWaveObject->aoFileUV;

	/// NEW GODS98: New functionality not present in LegoRR
	/*if (File_GetLine(line, sizeof(line), fileUV)) {
		if (File_GetLine(line, sizeof(line), fileUV)) {
			File_Seek(fileUV, 0, SeekOrigin::Set);
			/// NOTE: case sensitive string comparison
			if (std::strcmp("VERSION 4", line) == 0) {
				Mesh_UViewMeshV4(lightWaveObject, textCoords);
				return;
			}
		}
	}*/

	bool32* uvSet = (bool32*)Mem_Alloc(sizeof(bool32) * lightWaveObject->aoSize.lwVertCount * 2);
	std::memset(uvSet, 0, sizeof(bool32) * lightWaveObject->aoSize.lwVertCount * 2);

	while (File_GetLine(line, sizeof(line), fileUV)) {
		std::strcpy(lineSplit, line);

		if (argc = Util_WSTokenise(lineSplit, argv)) {
			//MAYBE CHECK VERSION NUMBER ON FIRST LINE - SHOULD BE 2

			//CHECK SURFACE COUNT IS SAME AS OBJECT
			Error_Fatal((lineNum == 1) && (lightWaveObject->aoSize.lwSurfaceCount != (uint32)std::atoi(argv[0])), "UView file corrupt.");

			if (lineNum == 2)
				surfRead = 0;

			//READ SURFACE INFORMATION
			if (surfRead != -1) {
				if (surfRead >= (sint32)(lightWaveObject->aoSize.lwSurfaceCount * 2)) {
					polyRead = -1;
					surfRead = -1;
				}
				else if (surfRead >= (sint32)lightWaveObject->aoSize.lwSurfaceCount) {
					//OVERWRITE EXISTING IMAGE FILE NAME ON SURFACE
					lightWaveObject->aoSurface[surfRead - lightWaveObject->aoSize.lwSurfaceCount].srfPath = Util_StrCpy(argv[0]);

					surfRead++;
				}
				else {
					surfRead++;
				}

			}

			//RUN THROUGH POLYGONS
			if (polyRead != -2) {
				if (polyRead == -1) {
					//CHECK POLY COUNT IS THE SAME AS OBJECT
					Error_Fatal((lightWaveObject->aoSize.lwPolyCount != (uint32)std::atoi(argv[0])), "UView file corrupt.");
					polyRead = 0;
				}
				else if (polyRead < (sint32)lightWaveObject->aoSize.lwPolyCount) {

					if ((vertexRead != -1) && (vertexRead < (sint32)lightWaveObject->aoPoly[groupID].plyCount)) {
						notSame = false;

						//READ UV INFORMATION FOR EACH VERTEX
						uvX = (real32)std::atof(argv[0]);
						uvY = -(real32)std::atof(argv[1]);

						if ((textCoords[lightWaveObject->aoPoly[groupID].plyData[vertexRead]].x != uvX) ||
							(textCoords[lightWaveObject->aoPoly[groupID].plyData[vertexRead]].y != uvY))
						{
							notSame = true;
						}

						if (!uvSet[lightWaveObject->aoPoly[groupID].plyData[vertexRead]]) {

							textCoords[lightWaveObject->aoPoly[groupID].plyData[vertexRead]].x = uvX;
							textCoords[lightWaveObject->aoPoly[groupID].plyData[vertexRead]].y = uvY;

							uvSet[lightWaveObject->aoPoly[groupID].plyData[vertexRead]] = true;
						}
						else if (notSame) {
							//THE SHARED VERTEX HAS MORE THEN ONE TEXTURE COORDINATE
							//COPY THE VERTEX AND ALTER THE FACE DATA FOR THE GROUP
							//ADD THE NEW UV INFORMATION TO THE NEW VERTEX

							//THIS ALTERS THE STRUCTURE OF 'lightWaveObject'
							//REFERENCES BEFORE THIS FUNCTION WILL BE DIFFERENT TO THOSE AFTER

							if (addedCount == 0) {
								lightWaveObject->aoVerts = (real32*)Mem_ReAlloc(lightWaveObject->aoVerts, (sizeof(real32) * (lightWaveObject->aoSize.lwVertCount + MESH_UVREALLOCSIZE + 1) * 3));
							}

							vertexIndex = lightWaveObject->aoPoly[groupID].plyData[vertexRead] * 3;
							newIndex = lightWaveObject->aoSize.lwVertCount * 3;

							//ADD A COPY OF VERTEX TO LIST
							lightWaveObject->aoVerts[newIndex] = lightWaveObject->aoVerts[vertexIndex];
							lightWaveObject->aoVerts[newIndex + 1] = lightWaveObject->aoVerts[vertexIndex + 1];
							lightWaveObject->aoVerts[newIndex + 2] = lightWaveObject->aoVerts[vertexIndex + 2];

							//POINT THE FACE DATA TO POINT TO THE NEW VERTEX
							lightWaveObject->aoPoly[groupID].plyData[vertexRead] = (uint16)lightWaveObject->aoSize.lwVertCount;

							//STORE THE NEW UV INFORMATION
							textCoords[lightWaveObject->aoPoly[groupID].plyData[vertexRead]].x = uvX;
							textCoords[lightWaveObject->aoPoly[groupID].plyData[vertexRead]].y = uvY;

							lightWaveObject->aoSize.lwVertCount++;

							if (++addedCount >= MESH_UVREALLOCSIZE)
								addedCount = 0;
						}

						vertexRead++;
					}
					else if (polyRead < (sint32)lightWaveObject->aoSize.lwPolyCount - 1) {
						//CHECK POLY VERTEX COUNT IS SAME AS OBJECT
						groupID = std::atoi(argv[0]);
						Error_Fatal((lightWaveObject->aoPoly[groupID].plyCount != (uint32)std::atoi(argv[1])), "UView file corrupt.");

						if (vertexRead != -1)
							polyRead++;

						vertexRead = 0;
					}

				}
				else {
					polyRead = -2;

					//RETURN HERE BECAUSE REST OF FILE IS UNUSED

					// FIX APPLY: OH NO YOU DON'T! GO CLEAN UP YOUR MEMORY FIRST!
					Mem_Free(uvSet);
					return;
				}

			}

			lineNum++;

		}

	}

	Mem_Free(uvSet);
}

// flags = lwt.h srfTexFlags bit enumeration `TFM_*`
// <LegoRR.exe @00482260>
void __cdecl Gods98::Mesh_GetTextureUVsWrap(uint32 vertexCount, OUT Vector3F* vertices, OUT Point2F* coords,
											real32 sx, real32 sy, real32 sz, real32 px, real32 py, real32 pz, LightWave_TexFlags flags)
{
	for (uint32 vertex = 0; vertex < vertexCount; vertex++) {
		real32 x = vertices[vertex].x - px;
		real32 y = vertices[vertex].y - py;
		real32 z = vertices[vertex].z - pz;

		coords[vertex].x = ((flags & LightWave_TexFlags::TFM_AXIS_X) ? (z/sz) : (x/sx)) + 0.5f;
		coords[vertex].y = ((flags & LightWave_TexFlags::TFM_AXIS_Y) ? (z/sz) : (y/sy)) + 0.5f;

	}
}


/// NEW GODS98: API that includes mesh->textureRenderCallback
// Only calls Mesh_SetTextureTime2 if mesh->textureRenderCallback == null.
// This function serves no purpose for this version of the engine,
//  but is included regardless.
// <inlined>
bool32 __cdecl Gods98::Mesh_SetTextureTime(Mesh* mesh, real32 frame)
{
	/*if (mesh->textureRenderCallback == nullptr)*/ return Mesh_SetTextureTime2(mesh, frame);
	//return false;
}


// <LegoRR.exe @00482300>
bool32 __cdecl Gods98::Mesh_SetTextureTime2(Mesh* mesh, real32 frame)
{
	/// NOTE: Decompiled source here passes a float: fmod(frame, 1.0f)
	///       But std function changes to template argument of double,int
	uint32 texNum = (uint32)(frame - (real32)std::fmod(frame, 1.0f));
	//uint32 texNum = (uint32)(frame - (real32)std::fmod(frame, 1));

	if (mesh->flags & MeshFlags::MESH_FLAG_LWO) {

		for (uint32 groupNum = 0; groupNum < mesh->groupCount; groupNum++) {
			Mesh_Group* group = &mesh->groupList[groupNum];

			if ((group->lightWaveSurfaceInfo->texFlags & LightWave_TexFlags::TFM_SEQUENCE) && (group->lightWaveSurfaceInfo->numInTexSeq)) {

				sint32 groupTexNum = (group->lightWaveSurfaceInfo->texSeqOffset + (sint32)texNum) % (sint32)group->lightWaveSurfaceInfo->numInTexSeq;

				if (groupTexNum < 0)
					groupTexNum = 0;

				Mesh_SetGroupTexture(mesh, groupNum, group->lightWaveSurfaceInfo->textureSeq[groupTexNum]);
			}

		}

		return true;
	}
	else {
		Error_Warn(true, "Cannot set texture time on non LightWave mesh.");

		return false;
	}
}

// <LegoRR.exe @00482390>
void __cdecl Gods98::Mesh_Remove(Mesh* mesh, IDirect3DRMFrame3* frame)
{
	Mesh_Debug_CheckIMDevice_Void();

	frame->DeleteVisual((IUnknown*)mesh->uv);

	if (mesh->clonedFrom) {
		Mesh* clonedFrom = mesh->clonedFrom;

		RELEASE(mesh->uv);
		for (uint32 loop = 0; loop < mesh->groupCount; loop++) {
			Mesh_Group* group = &mesh->groupList[loop];
			//RELEASE(group->imMat);
		}
		Mem_Free(mesh->groupList);
		Mesh_ReturnToList(mesh);

		mesh = clonedFrom;
	}

	mesh->numOfRefs--;

	if (mesh->numOfRefs == 0) {
		RELEASE(mesh->uv);

		for (uint32 loop = 0; loop < mesh->groupCount; loop++) {
			Mesh_Group* group = &mesh->groupList[loop];

//			RELEASE(group->imMat);
//			RELEASE(group->imText);
			Mesh_RemoveGroupTexture(mesh, loop);
			Mem_Free(group->faceData);
			Mem_Free(group->vertices);
//			Mem_Free(group);

		}

		if (mesh->lightWaveSurf) Mem_Free(mesh->lightWaveSurf);
		Mem_Free(mesh->groupList);
		Mesh_ReturnToList(mesh);

	}
}

// <LegoRR.exe @00482460>
void __cdecl Gods98::Mesh_GetGroup(const Mesh* mesh, uint32 groupID, OUT uint32* vertexCount,
								   OUT uint32* faceCount, OUT uint32* vPerFace,
								   OUT uint32* faceDataSize, OUT uint32* faceData)
{
	if (groupID < mesh->groupCount) {
		const Mesh_Group* group = &mesh->groupList[groupID];

		if (vertexCount) *vertexCount = group->vertexCount;
		if (vPerFace) *vPerFace = 3;
		if (faceDataSize) *faceDataSize = group->faceDataSize;

		if (faceData) {
			for (uint32 loop = 0; loop < group->faceDataSize; loop++) {
				faceData[loop] = group->faceData[loop];
			}
		}
	}
}

// <LegoRR.exe @004824d0>
uint32 __cdecl Gods98::Mesh_GetGroupCount(const Mesh* mesh)
{
	return mesh->groupCount;
}

// <LegoRR.exe @004824e0>
sint32 __cdecl Gods98::Mesh_AddGroup(Mesh* mesh, uint32 vertexCount, uint32 faceCount,
									 uint32 vPerFace, const uint32* faceData)
{
	Mesh_Debug_CheckIMDevice_Int();
	Error_Fatal((vPerFace != 3), "Only triangles supported so far.");
	Error_Fatal((mesh->clonedFrom!=nullptr)||(mesh->flags&MeshFlags::MESH_FLAG_HASBEENCLONED), "Cannot AddGroup() to a cloned mesh");


	if (mesh->groupList == nullptr) {
		mesh->listSize = MESH_DEFLISTSIZE;
		mesh->groupCount = 1;
		mesh->groupList = (Mesh_Group*)Mem_Alloc(mesh->listSize * sizeof(Mesh_Group));

	}
	else if (++mesh->groupCount == mesh->listSize) {
		Mesh_Group* newList;
		uint32 newSize = (mesh->listSize * MESH_LISTINCREASE) / 100;

		if (newList = (Mesh_Group*)Mem_ReAlloc(mesh->groupList, newSize * sizeof(Mesh_Group))) {
			mesh->groupList = newList;
			mesh->listSize = newSize;

		}
		else {
			Error_Fatal(true, "Memory allocation error");

			return -1;
		}
	}
	Mesh_Group* group = &mesh->groupList[mesh->groupCount-1];

	/// NEW GODS98: Different method to achieve the same result as above(?)
	///             It seems the older method allocated more room if *this* was the last group,
	///             rather than if we needed room for another group.
	/*if (mesh->groupCount == mesh->listSize) {
		mesh->listSize = std::max((uint32)MESH_DEFLISTSIZE, (mesh->listSize * MESH_LISTINCREASE) / 100);
		mesh->groupList = (Mesh_Group*)Mem_ReAlloc(mesh->groupList, sizeof(mesh->groupList[0]) * mesh->listSize);
	}
	group = &mesh->groupList[mesh->groupCount++];*/

	std::memset(group, 0, sizeof(*group));

//	group = (Mesh_Group*)Mem_Alloc( sizeof(Mesh_Group) );
//	std::memset( group, 0, sizeof(Mesh_Group) );

	uint32 size = sizeof(uint16) * faceCount * vPerFace;
	group->faceData = (uint16*)Mem_Alloc(size);
	group->faceDataSize = faceCount * vPerFace;
	for (uint32 loop = 0; loop < group->faceDataSize; loop++) {
		group->faceData[loop] = (uint16)faceData[loop];
	}

	size = sizeof(Mesh_Vertex) * vertexCount;
	group->vertices = (Mesh_Vertex*)Mem_Alloc(size);
	std::memset(group->vertices, 0, size);

	group->vertexCount = vertexCount;
	group->flags = MeshFlags::MESH_FLAG_NONE;

//	mesh->groupList[mesh->groupCount-1] = group;

	Mesh_CreateGroupMaterial(mesh, (mesh->groupCount - 1));


	return (mesh->groupCount - 1);
}

// <LegoRR.exe @00482610>
void __cdecl Gods98::Mesh_AlterGroupRenderFlags(Mesh* mesh, uint32 groupID, Mesh_RenderFlags newFlags)
{
	/// NEW GODS98: Safety check is not present in LegoRR
	if (groupID < mesh->groupCount) {
		mesh->groupList[groupID].renderFlags = newFlags;
	}
}

// <LegoRR.exe @00482630>
void __cdecl Gods98::Mesh_Scale(Mesh* mesh, real32 x, real32 y, real32 z)
{
	Mesh_Debug_CheckIMDevice_Void();

	for (uint32 loop = 0; loop < mesh->groupCount; loop++) {
		Mesh_Group* group = &mesh->groupList[loop];

		for (uint32 sub = 0; sub < group->vertexCount; sub++) {
			group->vertices[sub].position.x *= x;
			group->vertices[sub].position.y *= y;
			group->vertices[sub].position.z *= z;
		}
	}
}

// <LegoRR.exe @004826a0>
void __cdecl Gods98::Mesh_SetVertices(Mesh* mesh, uint32 groupID, uint32 index,
									  uint32 count, const Vertex* vertices)
{
	if (groupID < mesh->groupCount) {

		Mesh_Debug_CheckIMDevice_Void();
		Mesh_Group* group = &mesh->groupList[groupID];

		for (uint32 loop = 0; loop < count; loop++) {
			group->vertices[loop+index].position = vertices[loop].position;
			group->vertices[loop+index].normal = vertices[loop].normal;
			group->vertices[loop+index].tu = vertices[loop].tu;
			group->vertices[loop+index].tv = vertices[loop].tv;
//			group->vertices[loop+index].colour = vertices[loop].colour;
//			group->vertices[loop+index].specular = 0;
		}
	}
}

// <LegoRR.exe @00482730>
void __cdecl Gods98::Mesh_GetVertices(const Mesh* mesh, uint32 groupID, uint32 index,
									  uint32 count, OUT Vertex* vertices)
{
	if (groupID < mesh->groupCount) {

		Mesh_Debug_CheckIMDevice_Void();
		const Mesh_Group* group = &mesh->groupList[groupID];

		for (uint32 loop = 0; loop < count; loop++) {
			vertices[loop].position = group->vertices[loop+index].position;
			vertices[loop].normal = group->vertices[loop+index].normal;
			vertices[loop].tu = group->vertices[loop+index].tu;
			vertices[loop].tv = group->vertices[loop+index].tv;

		}
	}
}

// <LegoRR.exe @004827c0>
void __cdecl Gods98::Mesh_SetVertices_PointNormalAt(Mesh* mesh, uint32 groupID, uint32 index,
													uint32 count, const Vector3F* vertices, const Vector3F* position, real32(*textCoords)[2])
{
	if (groupID < mesh->groupCount) {

		Mesh_Debug_CheckIMDevice_Void();
		Mesh_Group* group = &mesh->groupList[groupID];

		for (uint32 loop = 0; loop < count; loop++) {
			group->vertices[loop+index].position.x = vertices[loop].x;
			group->vertices[loop+index].position.y = vertices[loop].y;
			group->vertices[loop+index].position.z = vertices[loop].z;
			Maths_Vector3DSubtract(&group->vertices[loop+index].normal, position, &group->vertices[loop+index].position);
			Maths_Vector3DNormalise(&group->vertices[loop+index].normal);
			group->vertices[loop+index].tu = textCoords[loop][0];
			group->vertices[loop+index].tv = textCoords[loop][1];
		}
	}
}

// <LegoRR.exe @004828e0>
void __cdecl Gods98::Mesh_SetVertices_SameNormal(Mesh* mesh, uint32 groupID, uint32 index,
												 uint32 count, const Vector3F* vertices, const Vector3F* normal, real32(*textCoords)[2])
{
	if (groupID < mesh->groupCount) {

		Mesh_Debug_CheckIMDevice_Void();
		Mesh_Group* group = &mesh->groupList[groupID];

		for (uint32 loop = 0; loop < count; loop++) {
			group->vertices[loop+index].position.x = vertices[loop].x;
			group->vertices[loop+index].position.y = vertices[loop].y;
			group->vertices[loop+index].position.z = vertices[loop].z;
			group->vertices[loop+index].normal = (*normal);
			group->vertices[loop+index].tu = textCoords[loop][0];
			group->vertices[loop+index].tv = textCoords[loop][1];
		}
	}
}

// <LegoRR.exe @00482980>
void __cdecl Gods98::Mesh_SetVertices_VNT(Mesh* mesh, uint32 groupID, uint32 index, uint32 count,
										  const Vector3F* vertices, const Vector3F* const* normal, const Point2F* textCoords)
{
	if (groupID < mesh->groupCount) {

		Mesh_Debug_CheckIMDevice_Void();

		Mesh_Group* group = &mesh->groupList[groupID];

		for (uint32 loop = 0; loop < count; loop++) {
			group->vertices[loop+index].position.x = vertices[loop].x;
			group->vertices[loop+index].position.y = vertices[loop].y;
			group->vertices[loop+index].position.z = vertices[loop].z;
			group->vertices[loop+index].normal.x = normal[loop]->x;
			group->vertices[loop+index].normal.y = normal[loop]->y;
			group->vertices[loop+index].normal.z = normal[loop]->z;
			group->vertices[loop+index].tu = textCoords[loop].x;
			group->vertices[loop+index].tv = textCoords[loop].y;

		}
	}
}

// <LegoRR.exe @00482a40>
bool32 __cdecl Gods98::Mesh_IsGroupHidden(const Mesh* mesh, uint32 groupID)
{
	/// NEW GODS98: Safety check is not present in LegoRR
	if (groupID < mesh->groupCount) {

		Mesh_Debug_CheckIMDevice_Int();
		const Mesh_Group* group = &mesh->groupList[groupID];

		return (group->flags & MeshFlags::MESH_FLAG_HIDDEN);
	}

	return true;
}

// <LegoRR.exe @00482a60>
void __cdecl Gods98::Mesh_HideGroup(Mesh* mesh, uint32 groupID, bool32 hide)
{
	/// NEW GODS98: Safety check is not present in LegoRR
	if (groupID < mesh->groupCount) {

		Mesh_Debug_CheckIMDevice_Void();
		Mesh_Group* group = &mesh->groupList[groupID];

		if (hide) group->flags |= MeshFlags::MESH_FLAG_HIDDEN;
		else      group->flags &= ~MeshFlags::MESH_FLAG_HIDDEN;
	}
}

// <LegoRR.exe @00482a90>
void __cdecl Gods98::Mesh_Hide(Mesh* mesh, bool32 hide)
{
	Mesh_Debug_CheckIMDevice_Void();

	if (hide) mesh->flags |= MeshFlags::MESH_FLAG_HIDDEN;
	else      mesh->flags &= ~MeshFlags::MESH_FLAG_HIDDEN;
}

// <LegoRR.exe @00482ab0>
BOOL __cdecl Gods98::Mesh_RenderCallback(LPDIRECT3DRMUSERVISUAL lpD3DRMUV, LPVOID lpArg, D3DRMUSERVISUALREASON lpD3DRMUVreason, LPDIRECT3DRMDEVICE lpD3DRMDev, LPDIRECT3DRMVIEWPORT lpD3DRMview)
{
	Mesh* mesh = (Mesh*)lpArg;


	if (lpD3DRMUVreason == D3DRMUSERVISUAL_CANSEE) {

		if (lpIMDevice()) {

			if (mesh->flags & MeshFlags::MESH_FLAG_HIDDEN) {
				return false;
			}


			Container* cont; // Only used for the the next 2 lines
			if (cont = Container_SearchOwner(mesh->frameCreatedOn)) {
				if (cont->flags & ContainerFlags::CONTAINER_FLAG_HIDDEN2) {
					return false;
				}
			}

			if ((mesh->flags & MeshFlags::MESH_FLAG_FACECAMERA) && !(mesh->flags & MeshFlags::MESH_FLAG_FACECAMERADONE)) {
				IDirect3DRMFrame3* camera;
				IDirect3DRMFrame3* scene;
				Viewport* vp = (Viewport*)lpD3DRMview->GetAppData();
				if (vp->rendering) {
					mesh->frameCreatedOn->GetScene(&scene);
					vp->lpVP->GetCamera(&camera);
					mesh->frameCreatedOn->LookAt(camera, scene, D3DRMFRAMECONSTRAINT::D3DRMCONSTRAIN_Z);
//					mesh->frameCreatedOn->SetOrientation(camera, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f);
					mesh->flags |= MeshFlags::MESH_FLAG_FACECAMERADONE;
				}
			}

			return true;
		}
		else {
			return false;
		}
	}
	else if (lpD3DRMUVreason == D3DRMUSERVISUAL_RENDER) {

		if (lpIMDevice()) {

			Matrix4F matWorld;
			IDirect3DRMViewport2* view;
			Mesh_Group* group;
			bool32 postRender = false;
			bool32 renderStateSet = false;


			mesh->flags &= ~MeshFlags::MESH_FLAG_FACECAMERADONE;

			//GET THE CURRENT RM WORLD MATRIX FOR POST RENDER
			lpIMDevice()->GetTransform(D3DTRANSFORMSTATE_WORLD, reinterpret_cast<D3DMATRIX*>(&matWorld));

			Mesh_SetCurrentViewport(lpD3DRMview);
			lpD3DRMview->QueryInterface(Idl::IID_IDirect3DRMViewport2, (void**)&view);

			Viewport* vp = (Viewport*)view->GetAppData();

			if (mesh->flags & MeshFlags::MESH_FLAG_POSTEFFECT) {

				//POST RENDER MESH
				Mesh_AddToPostRenderList(mesh, &matWorld); // (optional parameter)
			}
			else if (mesh->renderDesc.renderFlags & Mesh_RenderFlags::MESH_RENDER_FLAG_ALLALPHA) {

				//RENDER ALL GROUPS WITHOUT FLAGS CHANGED FISRT
				for (uint32 loop = 0; loop < mesh->groupCount; loop++) {
					group = &mesh->groupList[loop];

					if (Mesh_CanRenderGroup(group)) {
						if (group->flags & MeshFlags::MESH_FLAG_ALPHAENABLE) {
							//POST RENDER MESH
							//DO NOT RENDER GROUP NOW
							postRender = true;
							continue;
						}

						// If no render flags
						if (!group->renderFlags) {
							//GROUP OPAQUE SO RENDER NOW
							if (!renderStateSet) {
								Mesh_StoreTextureAndMat();
								Mesh_SetMeshRenderDesc(mesh, vp, &matWorld, false);
								renderStateSet = true;
							}

							Mesh_RenderGroup(mesh, group, &matWorld, false);
						}
					}
				}

				//RENDER ALL GROUPS WITH FLAGS CHANGED FISRT
				for (uint32 loop = 0; loop < mesh->groupCount; loop++) {
					group = &mesh->groupList[loop];

					if (Mesh_CanRenderGroup(group)) {
						if (group->flags & MeshFlags::MESH_FLAG_ALPHAENABLE) {
							//POST RENDER MESH
							//DO NOT RENDER GROUP NOW
							postRender = true;
							continue;
						}

						if (group->renderFlags) {
							//GROUP OPAQUE SO RENDER NOW
							if (!renderStateSet) {
								Mesh_StoreTextureAndMat();
								Mesh_SetMeshRenderDesc(mesh, vp, &matWorld, false);
								renderStateSet = true;
							}

							Mesh_RenderGroup(mesh, group, &matWorld, false);
						}
					}
				}

				if (postRender)
					Mesh_AddToPostRenderList(mesh, &matWorld); // (optional parameter)
			}
			else {
				//NOTE IS SOME OF THE GROUPS IN THIS MESH USE ALPHA BLENDING THEY WILL NOT APPEAR
				//IF YOU SUSPECT THAT ONE OR MORE GROUPS WILL USE ALPHA BLENDING SET 'MESH_RENDER_FLAG_ALPHATRANS' ON THE MESH RENDER DESC.
				Mesh_StoreTextureAndMat();
				Mesh_SetMeshRenderDesc(mesh, vp, &matWorld, false);
				renderStateSet = true;

				Mesh_RenderMesh(mesh, &matWorld, false);
			}

			if (renderStateSet) {
				//RESTORE STATES
				/// UNKNOWN: this is false, which would cause the function to do nothing
				///           in new Gods98 engine source. (in LegoRR it's no argument)
				Graphics_RestoreStates(/*false*/);
				//Mesh_RestoreTextureStageStates();
				Mesh_RestoreTextureAndMat();
			}

			RELEASE(view);
		}

		return true;
	}

	return false;
}

// <LegoRR.exe @00482d80>
void __cdecl Gods98::Mesh_SetMeshRenderDesc(Mesh* mesh, Viewport* vp, const Matrix4F* matWorld, bool32 alphaBlend)
{
	//CALL MESH SPECIFIC RENDER FUNCTION
	if (mesh->renderDesc.renderCallback)
		mesh->renderDesc.renderCallback(mesh, mesh->renderDesc.renderCallbackData, vp);

	//ADD DEFAULT STATES HERE
	Graphics_ChangeRenderState(D3DRENDERSTATE_SPECULARENABLE, false);

	/// CUSTOM: Add control over rendering quality using settings passed to Main_Setup3D
	// D3DRENDERSTATE_LIGHTING is not supported by Direct3DDevice3, which is
	//  why LegoRR doesn't support parsing the UnlitFlat quality in Lego.cfg.
	switch (graphicsGlobs.renderQuality) {
	case Graphics_Quality::Wireframe:
		Graphics_ChangeRenderState(D3DRENDERSTATE_SHADEMODE, D3DSHADE_FLAT);
		//Graphics_ChangeRenderState(D3DRENDERSTATE_LIGHTING,  false);
		Graphics_ChangeRenderState(D3DRENDERSTATE_FILLMODE,  D3DFILL_WIREFRAME);
		break;
	case Graphics_Quality::UnlitFlat:
		// Due to the lack of lighting support, this is identical to FLATSHADE
		Graphics_ChangeRenderState(D3DRENDERSTATE_SHADEMODE, D3DSHADE_FLAT);
		//Graphics_ChangeRenderState(D3DRENDERSTATE_LIGHTING,  false);
		Graphics_ChangeRenderState(D3DRENDERSTATE_FILLMODE,  D3DFILL_SOLID);
		break;
	case Graphics_Quality::Flat:
		Graphics_ChangeRenderState(D3DRENDERSTATE_SHADEMODE, D3DSHADE_FLAT);
		//Graphics_ChangeRenderState(D3DRENDERSTATE_LIGHTING,  true);
		Graphics_ChangeRenderState(D3DRENDERSTATE_FILLMODE,  D3DFILL_SOLID);
		break;
	case Graphics_Quality::Gouraud:
		Graphics_ChangeRenderState(D3DRENDERSTATE_SHADEMODE, D3DSHADE_GOURAUD);
		//Graphics_ChangeRenderState(D3DRENDERSTATE_LIGHTING,  true);
		Graphics_ChangeRenderState(D3DRENDERSTATE_FILLMODE,  D3DFILL_SOLID);
		break;
	case Graphics_Quality::Phong:
		Graphics_ChangeRenderState(D3DRENDERSTATE_SHADEMODE, D3DSHADE_PHONG);
		//Graphics_ChangeRenderState(D3DRENDERSTATE_LIGHTING,  true);
		Graphics_ChangeRenderState(D3DRENDERSTATE_FILLMODE,  D3DFILL_SOLID);
		break;
	}
	Graphics_ChangeRenderState(D3DRENDERSTATE_DITHERENABLE, graphicsGlobs.dither);

	/// ORIGINAL FUNCTIONALITY:
	//Graphics_ChangeRenderState(D3DRENDERSTATE_SHADEMODE, D3DSHADE_GOURAUD);
	//Graphics_ChangeRenderState(D3DRENDERSTATE_DITHERENABLE, true);

	if (!(mainGlobs.flags & MainFlags::MAIN_FLAG_DONTMANAGETEXTURES)) {
		Mesh_ChangeTextureStageState(D3DTSS_COLORARG1, D3DTA_TEXTURE);
		Mesh_ChangeTextureStageState(D3DTSS_COLORARG2, D3DTA_DIFFUSE);
		Mesh_ChangeTextureStageState(D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		Mesh_ChangeTextureStageState(D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	}

	Mesh_SetRenderDesc(mesh->renderDesc.renderFlags, matWorld, alphaBlend);
}

// <LegoRR.exe @00482e10>
void __cdecl Gods98::Mesh_SetRenderDesc(Mesh_RenderFlags flags, const Matrix4F* matWorld, bool32 alphaBlend)
{


	//CHECK MESH IS ALPHA BLENDED
	if ((flags & Mesh_RenderFlags::MESH_RENDER_FLAG_ALLALPHA) && alphaBlend) {
		Graphics_ChangeRenderState(D3DRENDERSTATE_FOGENABLE, false); // Don't fog alpha efects...
	}
	/*else {
		Graphics_ChangeRenderState(D3DRENDERSTATE_FOGENABLE, true);
		Container_EnableFog(true);
		Container_SetFogMode(D3DFOG_LINEAR);
		Container_SetFogParams(100, 1000, 0.5f);
	}*/

	//ALPHA STATES
	if ((flags & Mesh_RenderFlags::MESH_RENDER_FLAG_ALLALPHA) && !alphaBlend)
		Graphics_ChangeRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, false);
	else if (flags & Mesh_RenderFlags::MESH_RENDER_FLAG_ALPHA11)
		Mesh_SetAlphaRender(D3DBLEND_ONE, D3DBLEND_ONE);
	else if (flags & Mesh_RenderFlags::MESH_RENDER_FLAG_ALPHASA1)
		Mesh_SetAlphaRender(D3DBLEND_SRCALPHA, D3DBLEND_ONE);
	else if (flags & Mesh_RenderFlags::MESH_RENDER_FLAG_ALPHATRANS)
		Mesh_SetAlphaRender(D3DBLEND_SRCALPHA, D3DBLEND_INVSRCALPHA);
	else if (flags & Mesh_RenderFlags::MESH_RENDER_FLAG_ALPHASA0)
		Mesh_SetAlphaRender(D3DBLEND_ZERO, D3DBLEND_INVSRCCOLOR);
	else
		Graphics_ChangeRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, false);

	if (flags & Mesh_RenderFlags::MESH_RENDER_FLAG_DOUBLESIDED) Graphics_ChangeRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_NONE);
	else Graphics_ChangeRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_CCW);

	if (!(mainGlobs.flags & MainFlags::MAIN_FLAG_DONTMANAGETEXTURES)) {
		//ALPHA CHANNEL
		if (flags & Mesh_RenderFlags::MESH_RENDER_FLAG_ALPHATEX)
			Mesh_ChangeTextureStageState(D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
		else if (flags & Mesh_RenderFlags::MESH_RENDER_FLAG_ALPHADIFFUSE)
			Mesh_ChangeTextureStageState(D3DTSS_ALPHAOP, D3DTOP_SELECTARG2);
		else
			Mesh_ChangeTextureStageState(D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	}

	//Z BUFFER CHECK
	Graphics_ChangeRenderState(D3DRENDERSTATE_ZENABLE, true);

	//Z BUFFER WRITE
	if ((flags & Mesh_RenderFlags::MESH_RENDER_FLAG_ALLALPHA) && alphaBlend)
		Graphics_ChangeRenderState(D3DRENDERSTATE_ZWRITEENABLE, false);
	else
		Graphics_ChangeRenderState(D3DRENDERSTATE_ZWRITEENABLE, true);

//	if (flags & Mesh_RenderFlags::MESH_RENDER_FLAG_HIGHZBIAS) Graphics_ChangeRenderState(D3DRENDERSTATE_ZBIAS, 1);
//	else Graphics_ChangeRenderState(D3DRENDERSTATE_ZBIAS, 0);

	//WORLD TRANSORMATION
	if (flags & Mesh_RenderFlags::MESH_TRANSFORM_FLAG_PARENTPOS) {
		Mesh_SetTransform(D3DTRANSFORMSTATE_WORLD, matWorld);
	}
	else if (flags & Mesh_RenderFlags::MESH_TRANSFORM_FLAG_IDENTITY) {
		Matrix4F identity;
		Matrix_Identity(&identity);
		Mesh_SetTransform(D3DTRANSFORMSTATE_WORLD, &identity);

	}

	if (flags & Mesh_RenderFlags::MESH_RENDER_FLAG_FILTERNEAREST) {
		Graphics_ChangeRenderState(D3DRENDERSTATE_TEXTUREMAG, D3DFILTER_NEAREST);
		Graphics_ChangeRenderState(D3DRENDERSTATE_TEXTUREMIN, D3DFILTER_NEAREST);
	}
	else {
		Graphics_ChangeRenderState(D3DRENDERSTATE_TEXTUREMAG, D3DFILTER_LINEAR);

		/// CUSTOM: Add control over rendering filtering using settings passed to Main_Setup3D
		if (graphicsGlobs.mipMap) {
			// At the moment, this condition will be disabled until the
			// effects and any side effects are fully understood.
			// By default, Lego.cfg specifies `LinearMipMap TRUE`, but
			// this function would normally pass D3DFILTER_MIPLINEAR.
			//if (mainGlobs2.mipMapLinear) {
			//	Graphics_ChangeRenderState(D3DRENDERSTATE_TEXTUREMIN, D3DFILTER_LINEARMIPLINEAR);
			//}
			//else {
				// what LegoRR normally uses
				Graphics_ChangeRenderState(D3DRENDERSTATE_TEXTUREMIN, D3DFILTER_MIPLINEAR);
			//}
		}
		else {
			Graphics_ChangeRenderState(D3DRENDERSTATE_TEXTUREMIN, D3DFILTER_LINEAR);
		}
		/// OLD GODS98: MIPLINEAR used by LegoRR but LINEAR used by newer Gods98 source.
		//Graphics_ChangeRenderState(D3DRENDERSTATE_TEXTUREMIN, D3DFILTER_MIPLINEAR);
		//Graphics_ChangeRenderState(D3DRENDERSTATE_TEXTUREMIN, D3DFILTER_LINEAR);
	}
}

// <LegoRR.exe @00482f70>
void __cdecl Gods98::Mesh_SetAlphaRender(D3DBLEND src, D3DBLEND dest)
{
	Graphics_ChangeRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, true);
	Graphics_ChangeRenderState(D3DRENDERSTATE_SRCBLEND, src);
	Graphics_ChangeRenderState(D3DRENDERSTATE_DESTBLEND, dest);
}

// <LegoRR.exe @00482fa0>
void __cdecl Gods98::Mesh_AddToPostRenderList(Mesh* mesh, OPTIONAL const Matrix4F* matWorld)
{
	Mesh_PostRenderInfo* info = (Mesh_PostRenderInfo*)Mem_Alloc(sizeof(Mesh_PostRenderInfo));
	std::memset(info, 0, sizeof(Mesh_PostRenderInfo));

	info->mesh = mesh;
	if (matWorld) info->matWorld = (*matWorld);

	//ADD TO LINKED LIST
	info->next = meshGlobs.postRenderMeshList;
	meshGlobs.postRenderMeshList = info;
}

// <LegoRR.exe @00482ff0>
void __cdecl Gods98::Mesh_ClearPostRenderList(void)
{
	Mesh_PostRenderInfo* info = meshGlobs.postRenderMeshList;

	while (info) {
		Mesh_PostRenderInfo* infoNext = info->next;

		Mem_Free(info);

		info = infoNext;
	}

	meshGlobs.postRenderMeshList = nullptr;
}

// <LegoRR.exe @00483020>
void __cdecl Gods98::Mesh_PostRenderAll(Viewport* vp)
{
	Mesh_PostRenderInfo* info = meshGlobs.postRenderMeshList;

	Mesh_Debug_CheckIMDevice_Void();

	if (info) {
		Mesh_SetCurrentGODSViewport(vp);

		Mesh_StoreTextureAndMat();

		while (info) {
			if (!(info->mesh->flags & MeshFlags::MESH_FLAG_HIDDEN)) {

				Mesh_SetMeshRenderDesc(info->mesh, vp, &info->matWorld, true);

				if (info->mesh->flags & MeshFlags::MESH_FLAG_POSTEFFECT) {
					Mesh_RenderMesh(info->mesh, &info->matWorld, true);
				}
				else {
					//RUN THROUGH GROUPS AND ONLY RENDER TRANSPARENT GROUPS

					//RENDER ALL GROUPS WITHOUT FLAGS CHANGED FIRST
					for (uint32 loop = 0; loop < info->mesh->groupCount; loop++) {
						Mesh_Group* group = &info->mesh->groupList[loop];

						if (Mesh_CanRenderGroup(group)) {

							if ((group->flags & MeshFlags::MESH_FLAG_ALPHAENABLE) && !group->renderFlags) {
								Mesh_RenderGroup(info->mesh, group, &info->matWorld, true);
							}
						}
					}

					//RENDER ALL GROUPS WITH FLAGS CHANGED
					for (uint32 loop = 0; loop < info->mesh->groupCount; loop++) {
						Mesh_Group* group = &info->mesh->groupList[loop];

						if (Mesh_CanRenderGroup(group)) {

							if ((group->flags & MeshFlags::MESH_FLAG_ALPHAENABLE) && group->renderFlags) {
								Mesh_RenderGroup(info->mesh, group, &info->matWorld, true);
							}
						}
					}
				}
			}

			info = info->next;
		}

		Mesh_ClearPostRenderList();

		//RESTORE STATES
		/// NOTE: In new Gods98 engine, false would cause no action, in LegoRR
		///        it's always called.
		Graphics_RestoreStates(/*false*/);
		//Mesh_RestoreTextureStageStates();
		Mesh_RestoreTextureAndMat();
	}
}

// <LegoRR.exe @00483130>
Gods98::Mesh_Texture* __cdecl Gods98::Mesh_LoadTexture(const char* baseDir, const char* name, OUT uint32* width, OUT uint32* height)
{
	char path[FILE_MAXPATH];
	Mesh_Texture* texture = (Mesh_Texture*)Mem_Alloc(sizeof(Container_Texture));
	IDirectDrawSurface4* surface = nullptr;
	Mesh_TextureReference* ref;
	bool32 trans;

	Mesh_Debug_CheckIMDevice_Ptr(Mesh_Texture*);

	if (baseDir) std::sprintf(path, "%s%s", baseDir, name);
	else std::strcpy(path, name);

	if (ref = Mesh_SearchTexturePathList(meshGlobs.textureList, meshGlobs.textureCount, path)) {
		if (surface = ref->surface)
			surface->AddRef();
		trans = ref->trans;

	}
	else if (surface = Container_LoadTextureSurface(path, true, width, height, &trans)) {
		Mesh_AddTexturePathEntry(meshGlobs.textureList, &meshGlobs.textureCount, path, surface, trans);

	}
	else if (meshGlobs.sharedTextureDir) {

		std::sprintf(path, "%s%s", meshGlobs.sharedTextureDir, name);

		if (ref = Mesh_SearchTexturePathList(meshGlobs.textureListShared, meshGlobs.textureCountShared, path)) {
			if (surface = ref->surface)
				surface->AddRef();
			trans = ref->trans;

		}
		else if (surface = Container_LoadTextureSurface(path, true, width, height, &trans)) {
			Mesh_AddTexturePathEntry(meshGlobs.textureListShared, &meshGlobs.textureCountShared, path, surface, trans);
		}
	}

	if (surface) {
		texture->texture = nullptr;
		texture->surface = surface;
		texture->colourKey = trans;
	}
	else {
		Mem_Free(texture);
		texture = nullptr;
//		Error_Fatal(true, Error_Format("Cannot find or load texture >(%s\\)%s<", baseDir, name));
	}

	return texture;
}

// <LegoRR.exe @004832f0>
Gods98::Mesh_TextureReference* __cdecl Gods98::Mesh_SearchTexturePathList(Mesh_TextureReference* list, uint32 count, const char* path)
{
	for (uint32 loop = 0; loop < count; loop++) {
		if (::_stricmp(path, list[loop].path) == 0)
			return &list[loop];
	}
	return nullptr;
}

// <LegoRR.exe @00483340>
void __cdecl Gods98::Mesh_AddTexturePathEntry(Mesh_TextureReference* list, IN OUT uint32* count, const char* path, IDirectDrawSurface4* surface, bool32 trans)
{
	Error_Fatal(*count==MESH_TEXTURELISTSIZE, "MESH_TEXTURELISTSIZE too small");

	list[*count].path = Util_StrCpy(path);
	list[*count].surface = surface;
	list[*count].trans = trans;
	(*count)++;
}

// <LegoRR.exe @00483380>
void __cdecl Gods98::Mesh_SetGroupTexture(Mesh* mesh, uint32 groupID, Mesh_Texture* mt)
{
	/// NEW GODS98: Safety check is not present in LegoRR
	if (groupID < mesh->groupCount) {
		Mesh_Debug_CheckIMDevice_Void();

	//	Mesh_RemoveGroupTexture(mesh, groupID);

		if (mt) {
			if (mt->surface) mt->surface->QueryInterface(IID_IDirect3DTexture2, (void**)&mesh->groupList[groupID].imText);

			if (mt->colourKey) mesh->groupList[groupID].flags |= MeshFlags::MESH_FLAG_TRANSTEXTURE;
			else               mesh->groupList[groupID].flags &= ~MeshFlags::MESH_FLAG_TRANSTEXTURE;
		}
		//else mesh->groupList[groupID].imText = nullptr;
	}
}

// <LegoRR.exe @00483400>
void __cdecl Gods98::Mesh_RemoveGroupTexture(Mesh* mesh, uint32 groupID)
{
	/// NEW GODS98: Safety check is not present in LegoRR
	if (groupID < mesh->groupCount) {

		Mesh_Debug_CheckIMDevice_Void();
		Mesh_Group* group = &mesh->groupList[groupID];

		RELEASE(group->imText);
	}
}

// <LegoRR.exe @00483430>
bool32 __cdecl Gods98::Mesh_CreateGroupMaterial(Mesh* mesh, uint32 groupID)
{
	/// NEW GODS98: Safety check is not present in LegoRR
	if (groupID < mesh->groupCount) {
		Mesh_Group* group = &mesh->groupList[groupID];
		D3DMATERIAL material = {
			sizeof(D3DMATERIAL),
			{ 1.0f, 1.0f, 1.0f, 1.0f },
			/// OLD GODS98: Ambient set to 0.0f for RGBA
			{ 0.0f, 0.0f, 0.0f, 0.0f },
			//{ 1.0f, 1.0f, 1.0f, 1.0f },
			{ 0.0f, 0.0f, 0.0f, 0.0f },
			{ 0.0f, 0.0f, 0.0f, 0.0f },
			0.0f,
			0,
			0,
		};


		Mesh_Debug_CheckIMDevice_Int();

		group->material = material;

		return true;
	}
	return false;
}

// <LegoRR.exe @00483500>
bool32 __cdecl Gods98::Mesh_SetGroupMaterial(Mesh* mesh, uint32 groupID, const D3DMATERIAL* mat)
{
	/// NEW GODS98: Safety check is not present in LegoRR
	if (groupID < mesh->groupCount) {
		Mesh_Debug_CheckIMDevice_Int();

		mesh->groupList[groupID].material = (*mat);

		return true;
	}
	return false;
}

// <LegoRR.exe @00483530>
bool32 __cdecl Gods98::Mesh_SetGroupColour(Mesh* mesh, uint32 groupID, real32 r, real32 g, real32 b, Mesh_Colour type)
{
	/// NEW GODS98: Safety check is not present in LegoRR
	if (groupID < mesh->groupCount) {
		Mesh_Group* group = &mesh->groupList[groupID];
		D3DMATERIAL* material;


		Mesh_Debug_CheckIMDevice_Int();

		material = Mesh_GetGroupMaterial(mesh, groupID);

		if (r < 0.0f) r = 0.0f;
		if (g < 0.0f) g = 0.0f;
		if (b < 0.0f) b = 0.0f;
		if (r > 1.0f) r = 1.0f;
		if (g > 1.0f) g = 1.0f;
		if (b > 1.0f) b = 1.0f;

		if (type == Mesh_Colour::Diffuse) {
			material->diffuse.r = r;
			material->diffuse.g = g;
			material->diffuse.b = b;

			if ((r == 1.0f) && (g == 1.0f) && (b == 1.0f))
				group->flags |= MeshFlags::MESH_FLAG_TEXTURECOLOURONLY;
			else
				group->flags &= ~MeshFlags::MESH_FLAG_TEXTURECOLOURONLY;

			//Mesh_HideGroup(mesh, groupID, false);
			//if ((r == 0.0f) && (g == 0.0f) && (b == 0.0f)) {
			//	if (group->renderFlags) {
			//		if (group->renderFlags & Mesh_RenderFlags::MESH_RENDER_FLAG_ALLALPHA);
			//			Mesh_HideGroup(mesh, groupID, true);
			//	}
			//	else if (mesh->renderDesc.renderFlags & Mesh_RenderFlags::MESH_RENDER_FLAG_ALLALPHA) {
			//		Mesh_HideGroup(mesh, groupID, true);
			//	}
			//}
		}
		else if (type == Mesh_Colour::Ambient) {
			material->ambient.r = r;
			material->ambient.g = g;
			material->ambient.b = b;
		}
		else if (type == Mesh_Colour::Emissive) {
			material->emissive.r = r;
			material->emissive.g = g;
			material->emissive.b = b;
		}
		else if (type == Mesh_Colour::Specular) {
			material->specular.r = r;
			material->specular.g = g;
			material->specular.b = b;
		}

		return true;
	}

	return false;
}

// <LegoRR.exe @004836c0>
D3DMATERIAL* __cdecl Gods98::Mesh_GetGroupMaterial(Mesh* mesh, uint32 groupID)
{
	/// NEW GODS98: Safety check is not present in LegoRR
	if (groupID < mesh->groupCount) {
		Mesh_Debug_CheckIMDevice_Ptr(D3DMATERIAL*);

		return &mesh->groupList[groupID].material;
	}

	return nullptr; // original Gods98 source used `false` by mistake.
}

// <LegoRR.exe @004836e0>
bool32 __cdecl Gods98::Mesh_SetGroupMaterialValues(Mesh* mesh, uint32 groupID, real32 value, Mesh_Colour type)
{
	/// NEW GODS98: Safety check is not present in LegoRR
	if (groupID < mesh->groupCount) {

		Mesh_Debug_CheckIMDevice_Int();
		Mesh_Group* group = &mesh->groupList[groupID];

		D3DMATERIAL* material = Mesh_GetGroupMaterial(mesh, groupID);

		if (value < 0.0f) value = 0.0f;
		if (value > 1.0f) value = 1.0f;

		if (type == Mesh_Colour::Alpha) {
			group->flags &= ~MeshFlags::MESH_FLAG_ALPHAHIDDEN;
			group->flags &= ~MeshFlags::MESH_FLAG_ALPHAENABLE;

			if (value == 0.0f) {

				if (group->renderFlags) {
					/// FIX APPLY: Semicolon after if statement, but before condition block
					if (group->renderFlags & Mesh_RenderFlags::MESH_RENDER_FLAG_ALLALPHA)
						group->flags |= MeshFlags::MESH_FLAG_ALPHAHIDDEN;
				}
				else if (mesh->renderDesc.renderFlags & Mesh_RenderFlags::MESH_RENDER_FLAG_ALLALPHA) {
					group->flags |= MeshFlags::MESH_FLAG_ALPHAHIDDEN;
				}
			}
			else if (value != 1.0f) {

				if (group->renderFlags) {
					/// FIX APPLY: Semicolon after if statement, but before condition block
					if (group->renderFlags & Mesh_RenderFlags::MESH_RENDER_FLAG_ALLALPHA)
						group->flags |= MeshFlags::MESH_FLAG_ALPHAENABLE;
				}
				else if (mesh->renderDesc.renderFlags & Mesh_RenderFlags::MESH_RENDER_FLAG_ALLALPHA) {
					group->flags |= MeshFlags::MESH_FLAG_ALPHAENABLE;
				}
			}

			//COLOUR ADDITION CAN OCCUR EVEN IF ALPHA VALUE IS 1.0f
			if (group->renderFlags) {

				if ((group->renderFlags & Mesh_RenderFlags::MESH_RENDER_FLAG_ALPHA11) ||
					(group->renderFlags & Mesh_RenderFlags::MESH_RENDER_FLAG_ALPHASA1) ||
					(group->renderFlags & Mesh_RenderFlags::MESH_RENDER_FLAG_ALPHASA0))
				{
					//group->flags &= ~MeshFlags::MESH_FLAG_ALPHAHIDDEN;
					group->flags |= MeshFlags::MESH_FLAG_ALPHAENABLE;
				}
			}
			else if ((mesh->renderDesc.renderFlags & Mesh_RenderFlags::MESH_RENDER_FLAG_ALPHA11) ||
					 (mesh->renderDesc.renderFlags & Mesh_RenderFlags::MESH_RENDER_FLAG_ALPHASA1) ||
					 (mesh->renderDesc.renderFlags & Mesh_RenderFlags::MESH_RENDER_FLAG_ALPHASA0))
			{
				//group->flags &= ~MeshFlags::MESH_FLAG_ALPHAHIDDEN;
				group->flags |= MeshFlags::MESH_FLAG_ALPHAENABLE;
			}

			material->diffuse.a = value;
		}
		else if (type == Mesh_Colour::Power) {
			material->power = value;
		}

		return true;
	}

	return false;
}

// <LegoRR.exe @00483800>
void __cdecl Gods98::Mesh_SetIdentityMatrix(OUT Matrix4F* m)
{
	Matrix_Identity(m);
}

// <LegoRR.exe @00483840>
bool32 __cdecl Gods98::Mesh_SetCurrentViewport(IDirect3DRMViewport* view)
{
	IDirect3DViewport* imview1;
	IDirect3DViewport3* imview;


	if (view->GetDirect3DViewport(&imview1) != D3DRM_OK) {
		Error_Warn(true, "Cannot 'GetDirect3DViewport'.");
		return false;
	}

	if (imview1->QueryInterface(IID_IDirect3DViewport3, (void**)&imview) != D3DRM_OK) {
		Error_Warn(true, "Cannot 'QueryInterface' 'IID_IDirect3DViewport3'.");
		RELEASE(imview1);
		return false;
	}

	RELEASE(imview1);

	if (lpIMDevice()->SetCurrentViewport(imview) != D3D_OK) {
		Error_Warn(true, "Cannot 'SetCurrentViewport'.");
		RELEASE(imview);
		return false;
	}

	RELEASE(imview);

	return true;
}

// <LegoRR.exe @004838c0>
bool32 __cdecl Gods98::Mesh_SetCurrentGODSViewport(Viewport* vp)
{
	IDirect3DViewport* imview1;
	IDirect3DViewport3* imview;


	if (vp->lpVP->GetDirect3DViewport(&imview1) != D3DRM_OK) {
		Error_Warn(true, "Cannot 'GetDirect3DViewport'.");
		return false;
	}

	if (imview1->QueryInterface(IID_IDirect3DViewport3, (void**)&imview) != D3DRM_OK) {
		Error_Warn(true, "Cannot 'QueryInterface' 'IID_IDirect3DViewport3'.");
		RELEASE(imview1);
		return false;
	}

	RELEASE(imview1);

	if (lpIMDevice()->SetCurrentViewport(imview) != D3D_OK) {
		Error_Warn(true, "Cannot 'SetCurrentViewport'.");
		RELEASE(imview);
		return false;
	}

	RELEASE(imview);

	return true;
}

// <LegoRR.exe @00483950>
bool32 __cdecl Gods98::Mesh_SetTransform(D3DTRANSFORMSTATETYPE state, const Matrix4F* matrix)
{
	assert_sizeof(D3DMATRIX, sizeof(Matrix4F));

	Matrix4F oldMatrix;

	//DIRECTX DOCUMENTATION STATES CALLS TO 'SetTransform' SHOULD BE MINIMISED
	if (lpIMDevice()->GetTransform(state, reinterpret_cast<D3DMATRIX*>(&oldMatrix)) != D3D_OK) {
		Error_Warn(true, "Cannot 'GetTransform'.");
	}

	if (!Matrix_Equals(&oldMatrix, matrix)) {
		if (lpIMDevice()->SetTransform(state, const_cast<D3DMATRIX*>(reinterpret_cast<const D3DMATRIX*>(matrix))) != D3D_OK) {
			Error_Warn(true, "Cannot 'SetTransform'.");

			return false;
		}
	}

	return true;
}

// <LegoRR.exe @00483ad0>
bool32 __cdecl Gods98::Mesh_ChangeTextureStageState(D3DTEXTURESTAGESTATETYPE dwRenderStateType, uint32 dwRenderState)
{
	Error_Fatal(dwRenderStateType>=MESH_MAXTEXTURESTAGESTATES, "TextureStageState type is out of range");

	DWORD currValue;
	if (lpIMDevice()->GetTextureStageState(0, dwRenderStateType, &currValue) != D3D_OK) {
		Error_Warn(true, "Cannot 'GetTextureStageState'.");

		currValue = -1;
		//return false;
	}

	if (currValue != dwRenderState) {

		if (lpIMDevice()->SetTextureStageState(0, dwRenderStateType, dwRenderState) != D3D_OK) {
			Error_Warn(true, "Cannot 'SetTextureStageState'.");

			return false;
		}
		else {
			Mesh_TextureStateChangeData* data = &meshGlobs.stateData[dwRenderStateType];
			if (data->changed) {
				if (data->origValue == currValue) data->changed = false;
			}
			else {
				data->origValue = currValue;
				data->changed = true;
			}
		}

	}

	return true;
}

// <LegoRR.exe @00483b70>
void __cdecl Gods98::Mesh_StoreTextureAndMat(void)
{
	bool32 ok = true;


	//GET OLD MATERIAL
	if (lpIMDevice()->GetLightState(D3DLIGHTSTATE_MATERIAL, &meshGlobs.oldMatIM) != D3D_OK) {
		Error_Warn(true, "Cannot 'GetLightState' for current D3DIM material.");

		ok = false;
	}
	meshGlobs.currMatIM = meshGlobs.oldMatIM;

//#ifndef _GODS98_VIDEOMEMTEXTURES
	if (!(mainGlobs.flags & MainFlags::MAIN_FLAG_DONTMANAGETEXTURES)) {
		//GET OLD TEXTURE
		if (lpIMDevice()->GetTexture(0, &meshGlobs.oldTextureIM) != D3D_OK) {
			Error_Warn(true, "Cannot 'GetTexture' for current D3DIM texture.");

			ok = false;
		}
		meshGlobs.currTextureIM = meshGlobs.oldTextureIM;

		//GET OLD RM TEXTURE
		lpIMDevice()->GetRenderState(D3DRENDERSTATE_TEXTUREHANDLE, &meshGlobs.oldTextureRM);
		//CLEAR RETAINED MODE TEXTURE HANDLE
		if (meshGlobs.oldTextureRM != 0) {
			lpIMDevice()->SetRenderState(D3DRENDERSTATE_TEXTUREHANDLE, 0);
		}
	}
	else {
//#else //_GODS98_VIDEOMEMTEXTURES
		lpIMDevice()->GetRenderState(D3DRENDERSTATE_TEXTUREHANDLE, &meshGlobs.oldTextureRM);
		meshGlobs.currTextureRM = meshGlobs.oldTextureRM;
//#endif //_GODS98_VIDEOMEMTEXTURES
	}

}

// <LegoRR.exe @00483c00>
void __cdecl Gods98::Mesh_RestoreTextureAndMat(void)
{
	bool32 ok = true; // Assigned, but never used or returned.


/*	//GET OLD MATERIAL
	D3DMATERIALHANDLE oldMatHandle;
	if (lpIMDevice()->lpVtbl->GetLightState(D3DLIGHTSTATE_MATERIAL, &oldMatHandle) != D3D_OK) {
		Error_Warn(true, "Cannot 'GetLightState' for current D3DIM material.");

		ok = false;
	}
	//GET OLD TEXTURE
	IDirect3DTexture2* oldTexture;
	if (lpIMDevice()->GetTexture(0, &oldTexture) != D3D_OK) {
		Error_Warn(true, "Cannot 'GetTexture' for current D3DIM texture.");

		ok = false;
	}
*/

	if (meshGlobs.currMatIM != meshGlobs.oldMatIM) {
		//SET OLD MATERIAL
		if (lpIMDevice()->SetLightState(D3DLIGHTSTATE_MATERIAL, meshGlobs.oldMatIM) != D3D_OK) {
			Error_Warn(true, "Cannot 'SetLightState' for old D3DIM material.");

			ok = false;
		}
	}

//#ifndef _GODS98_VIDEOMEMTEXTURES
	if (!(mainGlobs.flags & MainFlags::MAIN_FLAG_DONTMANAGETEXTURES)) {

		if (meshGlobs.currTextureIM != meshGlobs.oldTextureIM) {
			//SET OLD TEXTURE
			if (lpIMDevice()->SetTexture(0, meshGlobs.oldTextureIM) != D3D_OK) {
				Error_Warn(true, "Cannot 'SetTexture' for old D3DIM texture.");

				ok = false;
			}
		}

		//SET OLD RETAINED MODE TEXTURE
		if (meshGlobs.oldTextureRM != 0) {
			lpIMDevice()->SetRenderState(D3DRENDERSTATE_TEXTUREHANDLE, meshGlobs.oldTextureRM);
		}
	}
	else {
//#else //_GODS98_VIDEOMEMTEXTURES
		lpIMDevice()->SetRenderState(D3DRENDERSTATE_TEXTUREHANDLE, meshGlobs.oldTextureRM);
//#endif //_GODS98_VIDEOMEMTEXTURES
	}
}

// <LegoRR.exe @00483c80>
bool32 __cdecl Gods98::Mesh_RenderMesh(Mesh* mesh, const Matrix4F* matWorld, bool32 alphaBlend)
{
	bool32 ok = true;

	//RENDER ALL GROUPS WITHOUT FLAGS CHANGED FISRT
	for (uint32 loop = 0; loop < mesh->groupCount; loop++) {
		Mesh_Group* group = &mesh->groupList[loop];

		if (Mesh_CanRenderGroup(group)) {

			if (!group->renderFlags) {
				if (!Mesh_RenderGroup(mesh, group, matWorld, alphaBlend)) {
					ok = false;
				}
			}
		}
	}

	//RENDER ALL GROUPS WITH FLAGS CHANGED
	for (uint32 loop = 0; loop < mesh->groupCount; loop++) {
		Mesh_Group* group = &mesh->groupList[loop];

		if (Mesh_CanRenderGroup(group)) {

			if (group->renderFlags) {
				if (!Mesh_RenderGroup(mesh, group, matWorld, alphaBlend)) {
					ok = false;
				}
			}
		}
	}

	return ok;
}

// <LegoRR.exe @00483d30>
bool32 __cdecl Gods98::Mesh_CanRenderGroup(const Mesh_Group* group)
{
	return (!(group->flags & MeshFlags::MESH_FLAG_HIDDEN) && !(group->flags & MeshFlags::MESH_FLAG_ALPHAHIDDEN));
}

// <LegoRR.exe @00483d50>
bool32 __cdecl Gods98::Mesh_RenderGroup(Mesh* mesh, Mesh_Group* group, const Matrix4F* matWorld, bool32 alphaBlend)
{
	bool32 ok = true;

	//COULD RUN THROUGH ALL GROUPS WITHOUT INDIVIDUAL RENDERING FLAGS
	//THEN RENDER GROUPS WITH RENDERING FLAGS ALTERED

	//bool32 flagsChanged = Mesh_SetGroupRenderDesc(mesh, group, matWorld, alphaBlend);

	Mesh_SetGroupRenderDesc(mesh, group, matWorld, alphaBlend);

	//if (!Mesh_RenderTriangleList(group->matHandle, group->imText, Mesh_RenderFlags::MESH_RENDERFLAGS_DEFAULT,
	//							 group->vertices, group->vertexCount, group->faceData, group->faceDataSize))
	//{
	//	ok = false;
	//}

	//SET THE MATERIAL FOR RENDER - SEE END OF FILE
	if (!Mesh_SetMaterial(&group->material)) {
		ok = false;
	}


	/// NEW GODS98: new functionality not present in LegoRR
	/*if (mesh->textureRenderCallback) {
		sint32 frame = (mesh->textureRenderCallback(mesh->textureRenderCallbackData));
		if (frame != -1) Mesh_SetTextureTime2(mesh, (real32)frame);

	}*/


	if (!Mesh_RenderTriangleList(meshGlobs.matHandle, group->imText, Mesh_RenderFlags::MESH_RENDERFLAGS_DEFAULT,
								 group->vertices, group->vertexCount, group->faceData, group->faceDataSize))
	{
		ok = false;
	}

	//if (flagsChanged) { //RESTORE PARENT MESH STATES
	//	Mesh_SetRenderDesc(mesh->renderDesc.renderFlags, matWorld, alphaBlend);
	//}

	return ok;
}

// <LegoRR.exe @00483dc0>
bool32 __cdecl Gods98::Mesh_SetGroupRenderDesc(Mesh* mesh, Mesh_Group* group, const Matrix4F* matWorld, bool32 alphaBlend)
{
	if (!(mainGlobs.flags & MainFlags::MAIN_FLAG_DONTMANAGETEXTURES)) {

		if ((group->flags & MeshFlags::MESH_FLAG_TEXTURECOLOURONLY) && group->imText)
			Mesh_ChangeTextureStageState(D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		else
			Mesh_ChangeTextureStageState(D3DTSS_COLOROP, D3DTOP_MODULATE);
	}

	if (group->flags & MeshFlags::MESH_FLAG_TRANSTEXTURE)
		Graphics_ChangeRenderState(D3DRENDERSTATE_COLORKEYENABLE, true);
	else
		Graphics_ChangeRenderState(D3DRENDERSTATE_COLORKEYENABLE, false);


	if (group->renderFlags) {
		Mesh_SetRenderDesc(group->renderFlags, matWorld, alphaBlend);

		return true;
	}

	return false;
}

// <LegoRR.exe @00483e30>
bool32 __cdecl Gods98::Mesh_RenderTriangleList(D3DMATERIALHANDLE matHandle, IDirect3DTexture2* texture, Mesh_RenderFlags renderFlags,
											   Mesh_Vertex* vertices, uint32 vertexCount, uint16* faceData, uint32 indexCount)
{
	bool32 ok = true;

	//TEXTURES
	if (!(mainGlobs.flags & MainFlags::MAIN_FLAG_DONTMANAGETEXTURES)) {
		if (texture != meshGlobs.currTextureIM) {
			//SET NEW TEXTURE
			if (lpIMDevice()->SetTexture(0, texture) != D3D_OK) {
				Error_Warn(true, "Cannot 'SetTexture' for new D3DIM texture.");
				ok = false;
			}
			else {
				meshGlobs.currTextureIM = texture;
			}
		}
	}
	else {
		D3DTEXTUREHANDLE textureHandle = 0; // just a DWORD

		if (texture) {
			//QUERY INTERFACE FOR OLD DEVICE AND GET HANDLE
			IDirect3DDevice2* device2;
			lpIMDevice()->QueryInterface(IID_IDirect3DDevice2, (void**)&device2);
			texture->GetHandle(device2, &textureHandle);
			RELEASE(device2);
		}

		if (textureHandle != meshGlobs.currTextureRM) {
			//SET NEW TEXTURE
			Graphics_ChangeRenderState(D3DRENDERSTATE_TEXTUREHANDLE, textureHandle);
			meshGlobs.currTextureRM = textureHandle;
		}
	}

	//MATERIAL
	if (matHandle != meshGlobs.currMatIM) {
		//SET NEW MATERIAL
		if (lpIMDevice()->SetLightState(D3DLIGHTSTATE_MATERIAL, matHandle) != D3D_OK) {
			Error_Warn(true, "Cannot 'SetLightState' for new D3DIM material.");
			ok = false;
		}
		else {
			meshGlobs.currMatIM = matHandle;
		}
	}

	/// NEW GODS98: New functionality not present in LegoRR
	//uint32 oldAmbientLightState;
	//lpIMDevice()->GetLightState(D3DLIGHTSTATE_AMBIENT, &oldAmbientLightState);
	//lpIMDevice()->SetLightState(D3DLIGHTSTATE_AMBIENT, meshGlobs.ambientLight);

	//RENDER PRIMITIVE
	if (lpIMDevice()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, renderFlags,
										   vertices, vertexCount, faceData, indexCount, 0) != D3D_OK)
	{
		Error_Warn(true, "Cannot 'DrawIndexedPrimitive'.");
		ok = false;
	}

	/// NEW GODS98: New functionality not present in LegoRR (see above)
	//lpIMDevice()->SetLightState(D3DLIGHTSTATE_AMBIENT, oldAmbientLightState);

	return ok;
}


/*
/// NEW GODS98: Feature not present in LegoRR
// <unused>
void __cdecl Gods98::Mesh_SetAmbientLight(real32 r, real32 g, real32 b)
{
	if (r > 1.0f) r = 1.0f;
	if (g > 1.0f) g = 1.0f;
	if (b > 1.0f) b = 1.0f;
	if (r < 0.0f) r = 0.0f;
	if (g < 0.0f) g = 0.0f;
	if (b < 0.0f) b = 0.0f;

	meshGlobs.ambientLight = ((uint32)(r * 255.0f) << 16) | ((uint32)(g * 255.0f) << 8) | ((uint32)(b * 255.0f));
}
*/

#pragma endregion
