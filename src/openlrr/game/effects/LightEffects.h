// LightEffects.h : 
//

#pragma once

#include "../GameCommon.h"


namespace LegoRR
{; // !<---

/**********************************************************************************
 ******** Forward Declarations
 **********************************************************************************/

#pragma region Forward Declarations

#pragma endregion

/**********************************************************************************
 ******** Constants
 **********************************************************************************/

#pragma region Constants

#pragma endregion

/**********************************************************************************
 ******** Enumerations
 **********************************************************************************/

#pragma region Enums

enum LightEffects_GlobFlags : uint32 // [LegoRR/LightEffects.c|flags:0x4|type:uint] LightEffects_GlobFlags, Flags for LightEffectsManager global variable @004ebec8
{
	LIGHTFX_GLOB_FLAG_NONE         = 0,
	LIGHTFX_GLOB_FLAG_HASBLINK     = 0x1,
	LIGHTFX_GLOB_FLAG_HASFADE      = 0x4,
	LIGHTFX_GLOB_FLAG_FADING       = 0x8,
	LIGHTFX_GLOB_FLAG_FADE_FORWARD = 0x10,
	LIGHTFX_GLOB_FLAG_FADE_REVERSE = 0x20,
	LIGHTFX_GLOB_FLAG_HASMOVE      = 0x40,
	LIGHTFX_GLOB_FLAG_MOVING       = 0x80,
	LIGHTFX_GLOB_FLAG_DISABLED     = 0x100,
	LIGHTFX_GLOB_FLAG_DIMOUT       = 0x200,
	LIGHTFX_GLOB_FLAG_DIMIN_DONE   = 0x400,
	LIGHTFX_GLOB_FLAG_DIMOUT_DONE  = 0x1000,
};
flags_end(LightEffects_GlobFlags, 0x4);

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

struct LightEffects_Globs // [LegoRR/LightEffects.c|struct:0xf4|tags:GLOBS] LightEffects module globals @004ebdd8
{
	/*00,4*/	Gods98::Container* resSpotlight; // [Res+Move] init
	/*04,4*/	Gods98::Container* resRootLight; // [Move] init
	/*08,c*/	ColourRGBF initialRGB; // [Color] init
	/*14,c*/	ColourRGBF currentRGB; // [Color+Blink+Fade] init+update
	/*20,c*/	ColourRGBF BlinkRGBMax; // [Blink] init
	/*2c,4*/	real32 blinkTime; // [Blink] update
	/*30,8*/	Range2F RandomRangeForTimeBetweenBlinks; // [Blink] init
	/*38,4*/	real32 blinkChange; // [Blink] update
	/*3c,4*/	real32 MaxChangeAllowed; // [Blink] init
	/*40,c*/	ColourRGBF fadeDestRGB; // [Fade] update
	/*4c,c*/	ColourRGBF FadeRGBMin; // [Fade] init
	/*58,c*/	ColourRGBF FadeRGBMax; // [Fade] init
	/*64,4*/	real32 fadeTime; // [Fade] update
	/*68,8*/	Range2F RandomRangeForTimeBetweenFades; // [Fade] init
	/*70,c*/	ColourRGBF fadeSpeedRGB; // [Fade] update
	/*7c,8*/	Range2F RandomRangeForFadeTimeFade; // [Fade] init
	/*84,4*/	real32 fadeHoldTime; // [Fade] update
	/*88,8*/	Range2F RandomRangeForHoldTimeOfFade; // [Fade] init
	/*90,c*/	ColourRGBF fadePosRGB; // [Fade] update
	/*9c,c*/	Vector3F resPosition; // [Move] init+update
	/*a8,c*/	Vector3F MoveLimit; // [Move] init
	/*b4,4*/	real32 moveTime; // [Move] update
	/*b8,8*/	Range2F RandomRangeForTimeBetweenMoves; // [Move] init
	/*c0,4*/	real32 moveSpeed; // [Move] update
	/*c4,8*/	Range2F RandomRangeForSpeedOfMove; // [Move] init
	/*cc,c*/	Vector3F vectorMove; // [Move] update
	/*d8,4*/	real32 moveDist; // [Move] update
	/*dc,8*/	Range2F RandomRangeForDistOfMove; // [Move] init
	/*e4,c*/	undefined1 reserved1[12]; // possibly an unused Vector3F/ColourRGBF
	/*f0,4*/	LightEffects_GlobFlags flags; // [all] init+update
	/*f4*/
};
assert_sizeof(LightEffects_Globs, 0xf4);

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @004ebdd8>
extern LightEffects_Globs & lightGlobs;

#pragma endregion

/**********************************************************************************
 ******** Macros
 **********************************************************************************/

#pragma region Macros

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @0044c9d0>
#define LightEffects_Initialise ((void (__cdecl* )(Gods98::Container* resRootSpotlight, Gods98::Container* resRootLight, real32 initialRed, real32 initialGreen, real32 initialBlue))0x0044c9d0)

// <LegoRR.exe @0044ca20>
#define LightEffects_ResetColor ((void (__cdecl* )(void))0x0044ca20)

// <LegoRR.exe @0044ca50>
#define LightEffects_SetDisabled ((void (__cdecl* )(bool32 disabled))0x0044ca50)

// <LegoRR.exe @0044ca80>
#define LightEffects_Load ((bool32 (__cdecl* )(const Gods98::Config* config, const char* gameName))0x0044ca80)

// <LegoRR.exe @0044cab0>
#define LightEffects_LoadBlink ((bool32 (__cdecl* )(const Gods98::Config* config, const char* gameName))0x0044cab0)

// <LegoRR.exe @0044cc30>
#define LightEffects_SetBlink ((void (__cdecl* )(real32 red, real32 green, real32 blue, real32 maxChange, real32 minRange, real32 maxRange))0x0044cc30)

// <LegoRR.exe @0044cc80>
#define LightEffects_LoadFade ((bool32 (__cdecl* )(const Gods98::Config* config, const char* gameName))0x0044cc80)

// <LegoRR.exe @0044ced0>
#define LightEffects_SetFade ((void (__cdecl* )(real32 minRed, real32 minGreen, real32 minBlue, real32 maxRed, real32 maxGreen, real32 maxBlue, real32 minTime, real32 maxTime, real32 minFade, real32 maxFade, real32 minHold, real32 maxHold))0x0044ced0)

// <LegoRR.exe @0044cf60>
#define LightEffects_LoadMove ((bool32 (__cdecl* )(const Gods98::Config* config, const char* gameName))0x0044cf60)

// <LegoRR.exe @0044d1b0>
#define LightEffects_SetMove ((void (__cdecl* )(real32 minTime, real32 maxTime, real32 minSpeed, real32 maxSpeed, real32 minDist, real32 maxDist, real32 limitX, real32 limitY, real32 limitZ))0x0044d1b0)

// <LegoRR.exe @0044d230>
#define LightEffects_InvalidatePosition ((void (__cdecl* )(void))0x0044d230)

// <LegoRR.exe @0044d260>
#define LightEffects_Update ((void (__cdecl* )(real32 elapsed))0x0044d260)

// <LegoRR.exe @0044d2b0>
#define LightEffects_UpdateResource ((void (__cdecl* )(void))0x0044d2b0)

// <LegoRR.exe @0044d390>
#define LightEffects_UpdateBlink ((void (__cdecl* )(real32 elapsed))0x0044d390)

// <LegoRR.exe @0044d510>
#define LightEffects_UpdateBlink_FlipSign ((void (__cdecl* )(IN OUT real32* ref_value))0x0044d510)

// <LegoRR.exe @0044d540>
#define LightEffects_UpdateFade ((void (__cdecl* )(real32 elapsed))0x0044d540)

// <LegoRR.exe @0044d9d0>
#define LightEffects_RandomizeFadeSpeedRGB ((void (__cdecl* )(void))0x0044d9d0)

// <LegoRR.exe @0044da20>
#define LightEffects_UpdateMove ((void (__cdecl* )(real32 elapsed))0x0044da20)

// <LegoRR.exe @0044dc60>
#define LightEffects_CheckMoveLimit ((bool32 (__cdecl* )(const Vector3F* vector))0x0044dc60)

// <LegoRR.exe @0044dce0>
#define LightEffects_SetDimmerMode ((void (__cdecl* )(bool32 isDimoutMode))0x0044dce0)

// <LegoRR.exe @0044dd10>
#define LightEffects_UpdateDimmer ((void (__cdecl* )(real32 elapsed))0x0044dd10)

#pragma endregion

}
