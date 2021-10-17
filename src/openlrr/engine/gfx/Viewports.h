// Viewports.h : 
//
/// APIS: IDirect3DRM3, IDirect3DRMDevice3, IDirect3DRMFrame3, IDirect3DRMViewport2,
///       IDirect3DViewport[13]
/// DEPENDENCIES: Containers, DirectDraw, Main, Maths, Mesh, (Dxbug, Errors, Memory)
/// DEPENDENTS: 

#pragma once

#include "../../common.h"
#include "../geometry.h"


/**********************************************************************************
 ******** Forward Global Namespace Declarations
 **********************************************************************************/

#pragma region Forward Declarations

struct IDirect3DRMFrame3;
struct IDirect3DRMViewport2;

#pragma endregion


namespace Gods98
{; // !<---

/**********************************************************************************
 ******** Forward Declarations
 **********************************************************************************/

#pragma region Forward Declarations

struct Container;

#pragma endregion

/**********************************************************************************
 ******** Constants
 **********************************************************************************/

#pragma region Constants

#define VIEWPORT_MAXLISTS			32			// 2^32 - 1 possible viewports...

#pragma endregion

/**********************************************************************************
 ******** Enumerations
 **********************************************************************************/

#pragma region Enums

flags_scoped(Viewport_GlobFlags) : uint32
{
	VIEWPORT_GLOB_FLAG_NONE        = 0,
	VIEWPORT_GLOB_FLAG_INITIALISED = 0x1,
};
flags_scoped_end(Viewport_GlobFlags, 0x4);

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

struct Viewport
{
	/*00,4*/ real32 xoffset;
	/*04,4*/ real32 yoffset;			// Position on viewport normalised to between 0.0 and 1.0
	/*08,4*/ real32 width;
	/*0c,4*/ real32 height;				// Width and height of the viewport normalised as above
	/*10,4*/ IDirect3DRMViewport2* lpVP;		// Pointer to D3D viewport interface
	/*14,4*/ real32 smoothFOV;
	/*18,4*/ bool32 rendering;
	/*1c,4*/ Viewport* nextFree;
	/*20*/
};
assert_sizeof(Viewport, 0x20);


struct Viewport_PickInfo
{
	/*00,4*/ Container* pickCont;
	/*04,4*/ uint32 group;
	/*08,4*/ uint32 face;
	/*0c,c*/ Vector3F position;
	/*18*/
};
assert_sizeof(Viewport_PickInfo, 0x18);


struct Viewport_Pick
{
	/*0,4*/ Viewport_PickInfo* pickArray;
	/*4,4*/ uint32 pickCount;
	/*8*/
};
assert_sizeof(Viewport_Pick, 0x8);


struct Viewport_Globs
{
	/*00,80*/ Viewport* listSet[VIEWPORT_MAXLISTS];
	/*80,4*/ Viewport* freeList;
	/*84,4*/ uint32 listCount;
	/*88,4*/ Viewport_GlobFlags flags;
	/*8c*/
};
assert_sizeof(Viewport_Globs, 0x8c);

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @0076bce0>
extern Viewport_Globs & viewportGlobs;

#pragma endregion

/**********************************************************************************
 ******** Macros
 **********************************************************************************/

#pragma region Macros

#ifdef DEBUG
	#define Viewport_CheckInit()			if (!(viewportGlobs.flags & Viewport_GlobFlags::VIEWPORT_GLOB_FLAG_INITIALISED)) Error_Fatal(true, "Error: Viewport_Intitialise() Has Not Been Called");
#else
	#define Viewport_CheckInit()
#endif

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @00477010>
void __cdecl Viewport_Initialise(void);

// <LegoRR.exe @00477040>
void __cdecl Viewport_Shutdown(void);

/// APIS: IDirect3DRMDevice3
// <LegoRR.exe @00477080>
Viewport* __cdecl Viewport_Create(real32 xPos, real32 yPos, real32 width, real32 height, Container* camera);

/// APIS: IDirect3DRM3, IDirect3DRMDevice3, IDirect3DRMFrame3, IDirect3DRMViewport2
// <LegoRR.exe @00477110>
Viewport* __cdecl Viewport_CreatePixel(sint32 xPos, sint32 yPos, uint32 width, uint32 height, Container* camera);

/// APIS: IDirect3DRMViewport2
// <LegoRR.exe @004771d0>
void __cdecl Viewport_GetSize(Viewport* vp, OUT uint32* width, OUT uint32* height);

/// APIS: IDirect3DRMFrame3, IDirect3DRMViewport2
// <LegoRR.exe @00477210>
void __cdecl Viewport_SetCamera(Viewport* vp, Container* cont);

/// APIS: IDirect3DRMFrame3, IDirect3DRMViewport2
// <LegoRR.exe @00477230>
Container* __cdecl Viewport_GetCamera(Viewport* vp);

/// APIS: IDirect3DRMViewport2
// <LegoRR.exe @00477270>
void __cdecl Viewport_SetBackClip(Viewport* vp, real32 dist);

/// APIS: IDirect3DRMViewport2
// <LegoRR.exe @00477290>
real32 __cdecl Viewport_GetBackClip(Viewport* vp);

/// APIS: IDirect3DRMViewport2
// <LegoRR.exe @004772b0>
real32 __cdecl Viewport_GetFrontClip(Viewport* vp);

/// APIS: IDirect3DRMFrame3, IDirect3DRMViewport2,
///       IDirect3DViewport, IDirect3DViewport3
// <LegoRR.exe @004772d0>
void __cdecl Viewport_Clear(Viewport* vp, bool32 full);

/// APIS: IDirect3DRMFrame3, IDirect3DRMViewport2
// <LegoRR.exe @00477410>
void __cdecl Viewport_Render(Viewport* vp, Container* root, real32 delta);

/// APIS: IDirect3DRMViewport2
// <LegoRR.exe @004774e0>
void __cdecl Viewport_Remove(Viewport* dead);

// (Field of View, FOV)
// <LegoRR.exe @00477500>
void __cdecl Viewport_SmoothSetField(Viewport* vp, real32 fov);

/// APIS: IDirect3DRMViewport2
// (Field of View, FOV)
// <LegoRR.exe @00477510>
void __cdecl Viewport_SetField(Viewport* vp, real32 fov);

/// APIS: IDirect3DRMViewport2
// <LegoRR.exe @00477530>
real32 __cdecl Viewport_GetField(Viewport* vp);

/// APIS: IDirect3DRMViewport2
// <LegoRR.exe @00477550>
void __cdecl Viewport_InverseTransform(Viewport* vp, OUT Vector3F* dest, const Vector4F* src);

/// APIS: IDirect3DRMViewport2
// <LegoRR.exe @00477570>
void __cdecl Viewport_Transform(Viewport* vp, OUT Vector4F* dest, const Vector3F* src);

// <LegoRR.exe @00477590>
Point2F* __cdecl Viewport_WorldToScreen(Viewport* vp, OUT Point2F* screen, const Vector3F* world);

/// APIS: IDirect3DRMFrame3, IDirect3DRMViewport2
// <LegoRR.exe @004775d0>
IDirect3DRMFrame3* __cdecl Viewport_GetScene(Viewport* vp);

// <LegoRR.exe @00477630>
void __cdecl Viewport_AddList(void);

/// APIS: IDirect3DRMViewport2
// <LegoRR.exe @004776a0>
void __cdecl Viewport_RemoveAll(void);

#pragma endregion

}
