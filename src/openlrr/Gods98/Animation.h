// Animation.h : Header file for the C G98CAnimation wrapper around the C++ implementation.
//
/// PURPOSE: Animation is only by the Credits looping starfield background AVI.
///          This uses the AVIFile API, and thus is likely restricted to this file format.
///          This is likely used over the VideoPlayer class, since it offers more flexibility
///          with rendering, which allows drawing the credits text over-top and manually handling
///          video looping.
///
/// INITIALISE: Call `Animation_Initialise(IDirectDraw4*)` once on startup. `IDirectDraw4*` is not owned by this module.
///             (no cleanup)
/// GLOBALS: private
/// FILEIO: Data[std]
/// APIS: IDirectDraw4, IDirectDrawClipper(unused), IDirectDrawPalette, IDirectDrawSurface[14]
///       IAVIStream, IGetFrame
/// DEPENDENCIES: DirectDraw, Files
/// DEPENDENTS: Credits

#pragma once

#include "../common.h"


/**********************************************************************************
 ******** Forward Global Namespace Declarations
 **********************************************************************************/

#pragma region Forward Declarations

struct IDirectDraw4;
struct tagRECT;
typedef struct tagRECT RECT;

#pragma endregion


namespace Gods98
{; // !<---

/**********************************************************************************
 ******** Typedefs
 **********************************************************************************/

#pragma region Typedefs

// C wrapper typedef around actual class
//typedef void* lpAnimation;
typedef void Animation_t;

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

struct Animation_Globs
{
	/*0,1*/ bool g98NoAvis;
	/*1,3*/ uint8 padding1[3];
	/*4,4*/ IDirectDraw4* ddraw;
	/*8*/
};
static_assert(sizeof(Animation_Globs) == 0x8, "");

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @00534998>
extern Animation_Globs & animationGlobs;

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @0047ef40>
void __cdecl Animation_Initialise(IDirectDraw4* directDraw);


// <missing>
void __cdecl Animation_ShutDown(void);


// <LegoRR.exe @0047ef50>
Animation_t* __cdecl Animation_Load(const char* fName);

// <LegoRR.exe @0047efb0>
bool32 __cdecl Animation_Update(Animation_t* anim);

// <LegoRR.exe @0047efc0>
void __cdecl Animation_BlitToBackBuffer(Animation_t* anim, const RECT* destRect);

// Sets the animation time in frame units.
// <LegoRR.exe @0047f000>
void __cdecl Animation_SetTime(Animation_t* anim, uint32 time);

// Gets the animation time in frame units.
// <LegoRR.exe @0047f010>
uint32 __cdecl Animation_GetTime(const Animation_t* anim);

// Gets the animation length in frame units.
// <LegoRR.exe @0047f020>
uint32 __cdecl Animation_GetLength(const Animation_t* anim);

// <LegoRR.exe @0047f030>
void __cdecl Animation_Free(Animation_t* anim);

// <LegoRR.exe @0047f040>
bool32 __cdecl Animation_IsOk(const Animation_t* anim);

#pragma endregion

}
