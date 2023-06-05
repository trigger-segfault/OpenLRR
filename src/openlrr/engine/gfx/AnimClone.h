// AnimClone.h : 
//
/// APIS: IDirect3DRM3, IDirect3DRMAnimationSet2, IDirect3DRMFrame[13|Array],
///       IDirect3DRMUserVisual, IDirect3DRMVisual
/// DEPENDENCIES: Lws, Main, Mesh, (Containers, Errors, Memory)
/// DEPENDENTS: Containers

#pragma once

#include "../../common.h"


/**********************************************************************************
 ******** Forward Global Namespace Declarations
 **********************************************************************************/

#pragma region Forward Declarations

struct IDirect3DRMAnimationSet2;
struct IDirect3DRMFrame3;

#pragma endregion


namespace Gods98
{; // !<---

/**********************************************************************************
 ******** Forward Declarations
 **********************************************************************************/

#pragma region Forward Declarations

struct Lws_Info; // from `engine/gfx/Lws.h`

#pragma endregion

/**********************************************************************************
 ******** Function Typedefs
 **********************************************************************************/

#pragma region Function Typedefs

typedef bool32 (__cdecl* AnimClone_WalkTreeCallback)(IDirect3DRMFrame3* frame, void* data);

#pragma endregion

/**********************************************************************************
 ******** Constants
 **********************************************************************************/

#pragma region Constants

#define ANIMCLONE_MAXVISUALS		4

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

struct AnimClone
{
	/*00,4*/ AnimClone* clonedFrom;
	/*04,4*/ Lws_Info* scene;
	/*08,4*/ IDirect3DRMAnimationSet2* animSet;
	/*0c,4*/ bool32 lws;
	/*10,4*/ IDirect3DRMFrame3* root;
	/*14,4*/ IDirect3DRMFrame3** partArray;
	/*18,4*/ uint32 partCount;
	/*1c,4*/ uint32 frameCount;
	/*20*/
};
assert_sizeof(AnimClone, 0x20);

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @004897e0>
AnimClone* __cdecl AnimClone_Register(IDirect3DRMAnimationSet2* animSet, IDirect3DRMFrame3* root, uint32 frameCount);

// <LegoRR.exe @00489880>
AnimClone* __cdecl AnimClone_RegisterLws(Lws_Info* scene, IDirect3DRMFrame3* root, uint32 frameCount);

// <LegoRR.exe @00489920>
AnimClone* __cdecl AnimClone_Make(AnimClone* orig, IDirect3DRMFrame3* parent, OPTIONAL OUT uint32* frameCount);

// <LegoRR.exe @00489a10>
void __cdecl AnimClone_Remove(AnimClone* dead);


// This function performs the same accessor,
// shared between 3 different structure types.
//  uint32 __cdecl Flic_GetWidth(Flic* fsp);
//  bool32 __cdecl AnimClone_IsLws(AnimClone* clone);
//  uint32 __cdecl Flocks_GetNumSubdata(Flocks* flocksData);
// (shared) "AnimClone_IsLws__Flic_GetWidth"
// THIS FUNCTION MUST BE HOOKED ON AN INDIVIDUAL BASIS
// There are 5 calls made to this:
//  type:Flic (Flic_GetWidth)  -> FUN_004120e0  <@004120f7>
//                                      Panel_Draw  <@0045ab17>
//                                      Pointer_DrawPointer  <@0045cfc8>
//  type:FlocksData (Flocks_GetNumSubdata) -> LegoObject_Flocks_FUN_0044bef0  <@0044bfc3>
//  type:AnimClone (AnimClone_IsLws) -> Container_FormatPartName  <@00473f60>
// 
// Only called by Container_FormatPartName
// <called @00473f60>
// <LegoRR.exe @00489a90>
bool32 __cdecl AnimClone_IsLws(const AnimClone* clone);


// <LegoRR.exe @00489aa0>
void __cdecl AnimClone_SetTime(AnimClone* clone, real32 time, OPTIONAL const real32* oldTime);

// <LegoRR.exe @00489ba0>
bool32 __cdecl AnimClone_FrameCountCallback(IDirect3DRMFrame3* frame, void* data);

// <LegoRR.exe @00489bb0>
bool32 __cdecl AnimClone_SetupFrameArrayCallback(IDirect3DRMFrame3* frame, void* p);

// <LegoRR.exe @00489bd0>
bool32 __cdecl AnimClone_WalkTree(IDirect3DRMFrame3* frame, uint32 level,
								  AnimClone_WalkTreeCallback callback, void* data);

// <LegoRR.exe @00489cb0>
void __cdecl AnimClone_CreateCopy(IDirect3DRMFrame3* orig, IDirect3DRMFrame3* clone, bool32 lws);

// <LegoRR.exe @00489df0>
void __cdecl AnimClone_CloneLwsMesh(IDirect3DRMFrame3* orig, IDirect3DRMFrame3* clone);

// <LegoRR.exe @00489e80>
void __cdecl AnimClone_ReferenceVisuals(IDirect3DRMFrame3* orig, IDirect3DRMFrame3* clone);

#pragma endregion

}

