// AnimClone.cpp : 
//

#include "../../platform/d3drm.h"

#include "../core/Errors.h"
#include "../core/Memory.h"
#include "../Graphics.h"	// accessor: lpD3DRM()
#include "Containers.h"		// debug macro: Container_NoteCreation(o)
#include "Lws.h"
#include "Mesh.h"

#include "AnimClone.h"


/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @004897e0>
Gods98::AnimClone* __cdecl Gods98::AnimClone_Register(IDirect3DRMAnimationSet2* animSet, IDirect3DRMFrame3* root, uint32 frameCount)
{
	AnimClone data;
	AnimClone* orig = (AnimClone *)Mem_Alloc(sizeof(AnimClone));
	HRESULT debugr;

	orig->clonedFrom = nullptr;
	orig->animSet = animSet;
	orig->scene = nullptr;
	orig->lws = false;

	orig->root = root;
	debugr = orig->root->AddRef();
	Container_NoteAddRef(orig->root, debugr);
	orig->partCount = 0;
	orig->frameCount = frameCount;
	
	AnimClone_WalkTree(root, 0, AnimClone_FrameCountCallback, &orig->partCount);
	orig->partArray = (IDirect3DRMFrame3**)Mem_Alloc(sizeof(IDirect3DRMFrame3*) * orig->partCount);

	data.partArray = orig->partArray;
	data.partCount = 0;
	AnimClone_WalkTree(root, 0, AnimClone_SetupFrameArrayCallback, &data);

	return orig;
}

// <LegoRR.exe @00489880>
Gods98::AnimClone* __cdecl Gods98::AnimClone_RegisterLws(Lws_Info* scene, IDirect3DRMFrame3* root, uint32 frameCount)
{
	AnimClone data;
	AnimClone* orig = (AnimClone*)Mem_Alloc(sizeof(AnimClone));
	HRESULT debugr;

	orig->clonedFrom = nullptr;
	orig->animSet = nullptr;
	orig->scene = scene;
	orig->lws = true;

	orig->root = root;
	debugr = orig->root->AddRef();
	Container_NoteAddRef(orig->root, debugr);
	orig->partCount = 0;
	orig->frameCount = frameCount;
	
	AnimClone_WalkTree(root, 0, AnimClone_FrameCountCallback, &orig->partCount);
	orig->partArray = (IDirect3DRMFrame3**)Mem_Alloc(sizeof(IDirect3DRMFrame3*) * orig->partCount);

	data.partArray = orig->partArray;
	data.partCount = 0;
	AnimClone_WalkTree(root, 0, AnimClone_SetupFrameArrayCallback, &data);

	return orig;
}

// <LegoRR.exe @00489920>
Gods98::AnimClone* __cdecl Gods98::AnimClone_Make(AnimClone* orig, IDirect3DRMFrame3* parent, OUT uint32* frameCount)
{
	AnimClone* clone = (AnimClone*)Mem_Alloc(sizeof(AnimClone));
	AnimClone data;

	// Setup from the first frame...
	if (orig->lws){

//		Lws_SetTime(orig->scene, 0.0f);
		*clone = *orig;
		clone->scene = Lws_Clone(orig->scene, parent);
		clone->clonedFrom = nullptr;
		if (frameCount) *frameCount = orig->frameCount;

	} else {
		
		std::memset(clone, 0, sizeof(AnimClone));
		orig->animSet->SetTime(0.0f);
		clone->animSet = nullptr;

		clone->clonedFrom = orig;
		lpD3DRM()->CreateFrame(parent, &clone->root);
		Container_NoteCreation(clone->root);
		clone->partArray = (IDirect3DRMFrame3**)Mem_Alloc(sizeof(LPDIRECT3DRMFRAME3) * orig->partCount);

		AnimClone_CreateCopy(orig->root, clone->root, orig->lws);
		clone->partCount = orig->partCount;
		if (frameCount) *frameCount = orig->frameCount;

		data.partArray = clone->partArray;
		data.partCount = 0;
		AnimClone_WalkTree(clone->root, 0, AnimClone_SetupFrameArrayCallback, &data);

	}

	return clone;
}

// <LegoRR.exe @00489a10>
void __cdecl Gods98::AnimClone_Remove(AnimClone* dead)
{
	HRESULT debugr;
	if (dead) {
		if (NULL == dead->clonedFrom) {
			if (dead->lws) {
				Lws_Free(dead->scene);
			}
			else {
				debugr = dead->animSet->Release();
				Container_NoteRelease(dead->animSet, debugr);
				debugr = dead->root->Release();
				Container_NoteRelease(dead->root, debugr);
			}
		}

		if (!dead->lws) {
			for (uint32 loop = 0; loop < dead->partCount; loop++) {
				debugr = dead->partArray[loop]->Release();
				Container_NoteRelease(dead->partArray[loop], debugr);
			}
			Mem_Free(dead->partArray);
		}

		Mem_Free(dead);
	}
}


// This function performs the same accessor,
// shared between 3 different structure types.
//  uint32 __cdecl Flic_GetWidth(Flic* fsp);
//  bool32 __cdecl AnimClone_IsLws(AnimClone* clone);
//  uint32 __cdecl Flocks_GetNumSubdata(Flocks* flocksData);
// (shared) "AnimClone_IsLws__Flic_GetWidth"
// THIS FUNCTION MUST BE HOOKED ON AN INDIVIDUAL BASIS
// There are 5 calls made to this:
//  type:Flic (Flic_GetWidth)  -> FUN_004120e0  <@004120f7>
//                                      Panel_FUN_0045a9f0  <@0045ab17>
//                                      Pointer_DrawPointer  <@0045cfc8>
//  type:FlocksData (Flocks_GetNumSubdata) -> LegoObject_Flocks_FUN_0044bef0  <@0044bfc3>
//  type:AnimClone (AnimClone_IsLws) -> Container_FormatPartName  <@00473f60>
// 
// Only called by Container_FormatPartName
// <called @00473f60>
// <LegoRR.exe @00489a90>
bool32 __cdecl Gods98::AnimClone_IsLws(AnimClone* clone)
{
	return clone->lws;
}


// <LegoRR.exe @00489aa0>
void __cdecl Gods98::AnimClone_SetTime(AnimClone* clone, real32 time, OUT real32* oldTime)
{
	AnimClone* orig = clone->clonedFrom;
	HRESULT debugr;
	if (orig) {

		IDirect3DRMFrame3* parent;
		D3DRMMATRIX4D mat; // same structure as Matrix4F

		// Set time on the original animation then copy the all of the transformations to the clone...
		if (orig->lws) Lws_SetTime(orig->scene, time);
		else orig->animSet->SetTime(time);

		for (uint32 loop = 0; loop < clone->partCount; loop++) {
			orig->partArray[loop]->GetParent(&parent);
			Container_NoteAddRef(parent);
			orig->partArray[loop]->GetTransform(parent, mat);
			debugr = parent->Release();
			Container_NoteRelease(parent, debugr);
			clone->partArray[loop]->AddTransform(D3DRMCOMBINE_REPLACE, mat);
		}

		// Restore the original animations' time if required...
		if (oldTime) {
			if (orig->lws) Lws_SetTime(orig->scene, *oldTime);
			else orig->animSet->SetTime(*oldTime);
		}

	}
	else {

		// 'clone' is actually the original...
		if (clone->lws) Lws_SetTime(clone->scene, time);
		else clone->animSet->SetTime(time);

	}
}

// <LegoRR.exe @00489ba0>
bool32 __cdecl Gods98::AnimClone_FrameCountCallback(IDirect3DRMFrame3* frame, void* data)
{
	// Count the total number of frames within a given hierachy...

	uint32* count = (uint32*)data;
	(*count)++;
	return false;		// Don't exit loop...
}

// <LegoRR.exe @00489bb0>
bool32 __cdecl Gods98::AnimClone_SetupFrameArrayCallback(IDirect3DRMFrame3* frame, void* p)
{
	// Build the hierarchy into a table by building an array of pointers to each frame...

	AnimClone* data = (AnimClone*)p;
	data->partArray[data->partCount++] = frame;
	return false;
}

// <LegoRR.exe @00489bd0>
bool32 __cdecl Gods98::AnimClone_WalkTree(IDirect3DRMFrame3* frame, uint32 level,
								AnimCloneWalkTreeCallback Callback, void* data)
{
	LPDIRECT3DRMFRAMEARRAY children;
	LPDIRECT3DRMFRAME3 child;
	LPDIRECT3DRMFRAME child1;
	bool32 finished = false;
	HRESULT r;

	if (Callback(frame, data)) return true;

	if (frame->GetChildren(&children) == D3DRM_OK){
		uint32 count = children->GetSize();
		for (uint32 loop = 0; loop < count; loop++){
			children->GetElement(loop, &child1);
			child1->QueryInterface(Idl::IID_IDirect3DRMFrame3, (void**)&child);
			Container_NoteAddRef(child);
			child1->Release();
			if (AnimClone_WalkTree(child, level+1, Callback, data)) {
				finished = true;
				r = child->Release();
				Container_NoteRelease(child, r);
				break;
			}
			r = child->Release();
			Container_NoteRelease(child, r);
		}
		r = children->Release();
	}

	return finished;
}

// <LegoRR.exe @00489cb0>
void __cdecl Gods98::AnimClone_CreateCopy(IDirect3DRMFrame3* orig, IDirect3DRMFrame3* clone, bool32 lws)
{
	IDirect3DRMFrameArray* children;
	IDirect3DRMFrame3* parent;
	char* name;
	DWORD length;
	Matrix4F mat;
	HRESULT debugr;

	// Link in the visuals...
	if (lws) AnimClone_CloneLwsMesh(orig, clone);
	else AnimClone_ReferenceVisuals(orig, clone);

	// Copy the frame name...
	orig->GetName(&length, nullptr);
	if (length) {
		name = (char*)Mem_Alloc(length);
		orig->GetName(&length, name);
		clone->SetName(name);
		Mem_Free(name);
	}

	// Copy the transformation...
	orig->GetParent(&parent);
	Container_NoteAddRef(parent);
	orig->GetTransform(parent, mat);
	debugr = parent->Release();
	Container_NoteRelease(parent, debugr);
	clone->AddTransform(D3DRMCOMBINE_REPLACE, mat);

	// Do exactly the same for each child of the frame...
	if (orig->GetChildren(&children) == D3DRM_OK){
		uint32 count = children->GetSize();
		for (uint32 loop = 0; loop < count; loop++) {

			IDirect3DRMFrame3* newFrame;
			IDirect3DRMFrame3* childFrame;
			IDirect3DRMFrame* child1;

			children->GetElement(loop, &child1);
			child1->QueryInterface(Idl::IID_IDirect3DRMFrame3, (void**)&childFrame);
			Container_NoteAddRef(childFrame);
			child1->Release();

			// All 'childFrame' are children of 'orig' thus all 'newFrame' should be chilren of 'clone'
			lpD3DRM()->CreateFrame(clone, &newFrame);
			Container_NoteCreation(newFrame);

			AnimClone_CreateCopy(childFrame, newFrame, lws);

			debugr = newFrame->Release();
			Container_NoteRelease(newFrame, debugr);
			debugr = childFrame->Release();
			Container_NoteRelease(childFrame, debugr);
		}
		children->Release();
	}
}

// <LegoRR.exe @00489df0>
void __cdecl Gods98::AnimClone_CloneLwsMesh(IDirect3DRMFrame3* orig, IDirect3DRMFrame3* clone)
{
	IUnknown* iunknown[ANIMCLONE_MAXVISUALS];
	IDirect3DRMUserVisual* uv;
	Mesh* mesh;
	DWORD count;

	orig->GetVisuals(&count, nullptr);
	if (count) {
		Error_Fatal(count>=ANIMCLONE_MAXVISUALS, "ANIMCLONE_MAXVISUALS too small");
		orig->GetVisuals(&count, iunknown);
		for (uint32 loop=0 ; loop<count ; loop++) {
			if (iunknown[loop]->QueryInterface(Idl::IID_IDirect3DRMUserVisual, (void**)&uv) == D3DRM_OK) {
				mesh = (Mesh*) uv->GetAppData();
				Error_Fatal(mesh == nullptr, "Cannot get mesh");
				Mesh_Clone(mesh, clone);
			}
		}
	}
}

// <LegoRR.exe @00489e80>
void __cdecl Gods98::AnimClone_ReferenceVisuals(IDirect3DRMFrame3* orig, IDirect3DRMFrame3* clone)
{
//	IDirect3DRMVisual* *visuals;
	IDirect3DRMVisual* visual;
	DWORD count;

	// Cut down on the Mem_Alloc()s...
	IDirect3DRMVisual* visuals[ANIMCLONE_MAXVISUALS];

	orig->GetVisuals(&count, nullptr);
	if (count) {
		Error_Fatal(count>=ANIMCLONE_MAXVISUALS, "ANIMCLONE_MAXVISUALS too small");
//		visuals = (IDirect3DRMVisual**)Mem_Alloc(sizeof(LPDIRECT3DRMVISUAL) * count);
		orig->GetVisuals(&count, (IUnknown**)visuals);

		for (uint32 loop = 0; loop < count; loop++) {

			visual = visuals[loop];
			clone->AddVisual((IUnknown*) visual);
		}
	}
}

#pragma endregion
