// Roof.cpp : 
//

#include "Roof.h"
#include "..\..\engine\gfx\Containers.h"
#include "..\..\platform\d3drm.h"
#include "..\..\engine\core\Maths.h"

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @00553180>
LegoRR::Roof_Globs & LegoRR::roofGlobs = *(LegoRR::Roof_Globs*)0x00553180;

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @00463060>
bool32 __cdecl LegoRR::Roof_Initialise(Gods98::Container* contRoot, sint32 width, sint32 height)
{
	// Make mesh from base container, check if its successful afterwards
	roofGlobs.contMesh = Gods98::Container_MakeMesh2(contRoot, Gods98::Container_MeshType::Immediate);

	if (roofGlobs.contMesh != nullptr) {
		// clear textures and set up counter for looping through grid
		roofGlobs.texture = nullptr;
		uint32 count = height * width;
		roofGlobs.hidden = 0;
		roofGlobs.grid = (RoofBlock*)Gods98::Mem_Alloc(count * sizeof(RoofBlock));
		// if we were able to create the grid start looping through it
		if (roofGlobs.grid != nullptr) {
			if (count != 0) {
				for (uint32_t i = 0; i < count; i++){
					// group index is set to 0 and set the roof to hidden
					roofGlobs.grid[i].groupID = D3DRMGROUP_0; // Had to create D3DRMGROUP_0 in d3drmdef.h -FC
					roofGlobs.grid[i].flags = ROOF_FLAG_HIDDEN;
				}
			}
			// Setup roof object
			roofGlobs.needsUpdate = TRUE;
			roofGlobs.dimensions.width = width;
			roofGlobs.dimensions.height = height;
			roofGlobs.visibleCount = 0;
			return TRUE;
		}
		// Clean up obj
		Gods98::Container_Remove(roofGlobs.contMesh);
	}
	return FALSE;
}

// <LegoRR.exe @00463120>
bool32 __cdecl LegoRR::Roof_SetTexture(const char* filename)
{
	// Clear existing texture if it exists
	if (roofGlobs.texture != nullptr) {
		Container_FreeTexture(roofGlobs.texture);
	}

	// Attempt to load in texture from file
	roofGlobs.texture = Gods98::Container_LoadTexture2(filename, TRUE, nullptr, nullptr);
	if (roofGlobs.texture != nullptr) {
		// Grab container group and set textures
		D3DRMGROUPINDEX count = Gods98::Container_Mesh_GetGroupCount(roofGlobs.contMesh);
		D3DRMGROUPINDEX groupID = D3DRMGROUP_0;
		if (count != D3DRMGROUP_0) {
			do {
				Container_Mesh_SetTexture(roofGlobs.contMesh, groupID,roofGlobs.texture);
				groupID++;
			} while (groupID < count);
		}
		return TRUE;
	}
	return FALSE;
}

// <LegoRR.exe @00463190>
void __cdecl LegoRR::Roof_Shutdown(void)
{
	// Free up memory, containers, and textures
	if (roofGlobs.grid != nullptr) {
		Gods98::Mem_Free(roofGlobs.grid);
	}
	roofGlobs.grid = nullptr;
	if (roofGlobs.contMesh != nullptr) {
		Gods98::Container_Remove(roofGlobs.contMesh);
	}
	roofGlobs.contMesh = nullptr;
	Gods98::Container_FreeTexture(roofGlobs.texture);
}

// <LegoRR.exe @004631e0>
void __cdecl LegoRR::Roof_SetBlockRoofVertices(uint32 bx, uint32 by, const Vector3F* vertPos0, const Vector3F* vertPos1, const Vector3F* vertPos2, const Vector3F* vertPos3)
{
	RoofBlock* locRoofBlock;

	locRoofBlock = roofGlobs.grid + by * roofGlobs.dimensions.width + bx;
	/* I'm not sure how this is supposed to function for already-visible roof tiles
	   (maybe it's handled separately?)
	   if (roof->flags & ROOF_HIDDEN) // (0x1) has roof never been visible? */
	if ((locRoofBlock->flags & ROOF_FLAG_HIDDEN) != 0) {
		uint32 faceData[6] = { 0,1,3,1,2,3 };
		// Grab group id to check if we need to create roof verts
		D3DRMGROUPINDEX groupID = Gods98::Container_Mesh_AddGroup(roofGlobs.contMesh, 4, 2, 3, faceData);
		locRoofBlock->groupID = groupID;
		if (groupID != D3DRMGROUP_ALLGROUPS) {
			Gods98::Container_Mesh_SetTexture(roofGlobs.contMesh, groupID, roofGlobs.texture);
			Gods98::Container_Mesh_SetPerspectiveCorrection(roofGlobs.contMesh, locRoofBlock->groupID, TRUE);
			Gods98::Container_Mesh_HideGroup(roofGlobs.contMesh, locRoofBlock->groupID, TRUE);
			/* &= ~ROOF_HIDDEN; // (~0x1) roof mesh has been created, unset flag */
			locRoofBlock->flags &= ~ROOF_FLAG_HIDDEN;
		}
	}
	/* At least one block has been added that needs processing by RoofMesh_Update */
	roofGlobs.needsUpdate = TRUE;
	locRoofBlock->flags = locRoofBlock->flags | ROOF_FLAG_NEEDUPDATE;
	// Check bounds to see if we need to shift vertex pos
	// Setup local storage vars
	bool bShiftVertices = true;
	Vertex vertices[4];
	if ((vertPos2->z <= vertPos0->z) && (vertPos2->z <= vertPos1->z) && (vertPos2->z <= vertPos3->z) && (vertPos0->z <= vertPos1->z) && (vertPos0->z <= vertPos3->z))
	{
		bShiftVertices = false;
	}
	// Pretty wordy just for position swapping, may have been macro -FC
	if (bShiftVertices) {
		/* Assign vertex positions in order: [1->3], [2->2], [3->1], [0->0] */
		vertices[3].position.x = vertPos1->x;
		vertices[3].position.y = vertPos1->y;
		vertices[3].position.z = vertPos1->z;
		vertices[2].position.x = vertPos2->x;
		vertices[2].position.y = vertPos2->y;
		vertices[2].position.z = vertPos2->z;
		vertices[1].position.x = vertPos3->x;
		vertices[1].position.y = vertPos3->y;
		vertices[1].position.z = vertPos3->z;
		vertices[0].position.x = vertPos0->x;
		vertices[0].position.y = vertPos0->y;
		vertices[0].position.z = vertPos0->z;
		/* |= ROOF_SHIFTVERTICES; // (0x4) */
		locRoofBlock->flags = locRoofBlock->flags | ROOF_FLAG_SHIFTVERTICES;
	}
	else {
		/* Assign vertex positions in order: [0->3], [1->2], [2->1], [3->0] */
		vertices[3].position.x = vertPos0->x;
		vertices[3].position.y = vertPos0->y;
		vertices[3].position.z = vertPos0->z;
		vertices[2].position.x = vertPos1->x;
		vertices[2].position.y = vertPos1->y;
		vertices[2].position.z = vertPos1->z;
		vertices[1].position.x = vertPos2->x;
		vertices[1].position.y = vertPos2->y;
		vertices[1].position.z = vertPos2->z;
		vertices[0].position.x = vertPos3->x;
		vertices[0].position.y = vertPos3->y;
		vertices[0].position.z = vertPos3->z;
		/* &= ~ROOF_SHIFTVERTICES; // (~0x4) */
		locRoofBlock->flags &= ~ROOF_FLAG_SHIFTVERTICES;
	}
	// Finish clearing normals
	vertices[3].normal.x = 0.0;
	vertices[3].normal.z = 0.0;
	vertices[2].normal.x = 0.0;
	vertices[2].normal.z = 0.0;
	vertices[1].normal.x = 0.0;
	vertices[1].normal.z = 0.0;
	vertices[0].normal.x = 0.0;
	vertices[0].normal.z = 0.0;
	vertices[3].normal.y = 0.0;
	vertices[2].normal.y = 0.0;
	vertices[1].normal.y = 0.0;
	vertices[0].normal.y = 0.0;
	/* white (no coloration) */
	vertices[3].colour = 0xffffffff;
	vertices[2].colour = 0xffffffff;
	vertices[1].colour = 0xffffffff;
	vertices[0].colour = 0xffffffff;
	vertices[3].tu = 0.0;
	vertices[3].tv = 1.0;
	vertices[2].tu = 1.0;
	vertices[2].tv = 1.0;
	vertices[1].tu = 1.0;
	vertices[1].tv = 0.0;
	vertices[0].tu = 0.0;
	vertices[0].tv = 0.0;
	Gods98::Container_Mesh_SetVertices(roofGlobs.contMesh, locRoofBlock->groupID, 0, 4, vertices);
}

// <LegoRR.exe @004634f0>
void __cdecl LegoRR::Roof_Update(void)
{
	RoofBlock* locRoofBlock;
	float fVar2;
	uint32 dirIndex;
	uint32 width;
	Point2I* pDir;
	Vector3F worldPos;
	Vertex vertices[4];

	// Check if the roof needs an update, then go through loop, updating object vectors
	if (roofGlobs.needsUpdate != 0) {
		roofGlobs.needsUpdate = 0;
		width = roofGlobs.dimensions.width;
		// Sanity check then iterate through heigh and with
		if (roofGlobs.dimensions.height != 0) {
			for (uint32 y = 0; y < roofGlobs.dimensions.height; y++) {
				if (width != 0) {
					for(uint32 x = 0; x < width; x++) {
						/* Roof vertex has been updated by RoofMesh_LowerRoofVertices.
							Update it here, and unset the flag. */
						locRoofBlock = roofGlobs.grid + (y * width + x);
						if ((roofGlobs.grid[(y * width + x)].flags & ROOF_FLAG_NEEDUPDATE) != 0) {
							// Setup all directions with appropritate vectors		
							Point2I DIRECTIONS_4X4[16] = { {0,0},{-1,0},{0,1},{-1,1},{0,0},{1,0},{1,1},{0,1},{0,0},{0,-1},{1,-1},{1,0},{0,0},{-1,-1},{0,-1},{-1,0}};
							// Grab verticies to manipulate below
							Gods98::Container_Mesh_GetVertices(roofGlobs.contMesh, locRoofBlock->groupID, 0, 4, vertices);							
							/* loop for DIRECTIONS_4X4[i...][] */
							for(uint32 i = 0; i < 4; i++) {
								/* Some counter-clockwise flag maybe? */
								dirIndex = i;
								/* GHIDRA FAIL: setting 0x4 flag equate causes Ghidra to hang
								   if (roof->flags & ROOF_SHIFTVERTICES) // (0x4) */
								if ((locRoofBlock->flags & ROOF_FLAG_SHIFTVERTICES) != 0) {
									dirIndex = i - 1 & 3;
								}
								/* loop for DIRECTIONS_4X4[dirIndex][j...] */
								for(int j =4; j != 0; j--){
									if (Roof_GetBlockPlaneNormal(x + DIRECTIONS_4X4[dirIndex * 4].x, DIRECTIONS_4X4[dirIndex * 4].y + y, &worldPos) != 0) {
										vertices[i].normal.x = worldPos.x + vertices[i].normal.x;
										vertices[i].normal.y = worldPos.y + vertices[i].normal.y;
										vertices[i].normal.z = worldPos.z + vertices[i].normal.z;
									}
								}
								// Calc inverse then apply to normals
								float InvTemp = 1.0f / sqrtf((vertices[i].normal.x * vertices[i].normal.x) + (vertices[i].normal.z * vertices[i].normal.z) + (vertices[i].normal.y * vertices[i].normal.y));
								vertices[i].normal.z = InvTemp * vertices[i].normal.z;
								vertices[i].normal.x = InvTemp * vertices[i].normal.x;
								vertices[i].normal.y = InvTemp * vertices[i].normal.y;
							}
							Gods98::Container_Mesh_SetVertices(roofGlobs.contMesh, locRoofBlock->groupID, 0, 4, vertices);
							/* &= ~ROOF_NEEDUPDATE; // (~0x2) updated, so clear flag */
							locRoofBlock->flags &= ~ROOF_FLAG_NEEDUPDATE;
							width = roofGlobs.dimensions.width;
						}
					}
				}
			}
		}
	}
}

// <LegoRR.exe @00463770>
void __cdecl LegoRR::Roof_Hide(bool32 hide)
{
	// Check if roof is already hidden, if not, hide it
	if (roofGlobs.hidden != hide) {
		roofGlobs.hidden = hide;
		if (hide != 0) {
			Gods98::Container_Hide(roofGlobs.contMesh, TRUE);
			return;
		}
		Container_Hide(roofGlobs.contMesh, FALSE);
	}
}

// <LegoRR.exe @004637b0>
void __cdecl LegoRR::Roof_AddVisibleBlock(uint32 bx, uint32 by)
{
	int idx;
	
	idx = (by * roofGlobs.dimensions.width) + bx;
	Gods98::Container_Mesh_HideGroup(roofGlobs.contMesh, roofGlobs.grid[idx].groupID, FALSE);
	roofGlobs.visibleTable[roofGlobs.visibleCount] = roofGlobs.grid + idx;
	roofGlobs.visibleCount++;
}

// <LegoRR.exe @00463800>
void __cdecl LegoRR::Roof_HideAllVisibleBlocks(void)
{
	// If any blocks visable clear them out
	if (roofGlobs.visibleCount != 0) {
		for(uint32 i = 0; i < roofGlobs.visibleCount; i++) {
			Gods98::Container_Mesh_HideGroup(roofGlobs.contMesh, roofGlobs.visibleTable[i]->groupID, TRUE);
		}
	}
	roofGlobs.visibleCount = 0;
}

// <LegoRR.exe @00463850>
bool32 __cdecl LegoRR::Roof_GetBlockPlaneNormal(uint32 bx, uint32 by, OUT Vector3F* normal)
{
	float fVar1;
	float fVar2;
	float fVar3;
	float fVar4;
	Vector3F normal1;
	Vector3F normal2;
	Vertex vertices[4];

	// Check if block is within bounds, calc normals, then assign to caller object
	if (bx < (uint32)roofGlobs.dimensions.width && by < (uint32)roofGlobs.dimensions.height && (roofGlobs.grid[by * roofGlobs.dimensions.width + bx].flags & ROOF_FLAG_HIDDEN) == 0) {
		Gods98::Container_Mesh_GetVertices(roofGlobs.contMesh,roofGlobs.grid[by * roofGlobs.dimensions.width + bx].groupID, 0, 4, vertices);
		Gods98::Maths_PlaneNormal(&normal1, &vertices[0].position, &vertices[1].position, &vertices[2].position);
		Gods98::Maths_PlaneNormal(&normal2, &vertices[2].position, &vertices[3].position, &vertices[0].position);
		fVar3 = normal2.y + normal1.y;
		fVar1 = normal2.z + normal1.z;
		fVar2 = normal2.x + normal1.x;
		fVar4 = 1.0 / sqrtf(fVar2 * fVar2 + fVar1 * fVar1 + fVar3 * fVar3);
		normal->x = fVar4 * fVar2;
		normal->y = fVar4 * fVar3;
		normal->z = fVar4 * fVar1;
		return TRUE;
	}
	else {
		return FALSE;
	}
}

#pragma endregion
