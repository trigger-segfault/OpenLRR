// ObjInfo.h : 
//

#pragma once

#include "../../GameCommon.h"


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

#define OBJINFO_HUNGERIMAGECOUNT	5

#pragma endregion

/**********************************************************************************
 ******** Enumerations
 **********************************************************************************/

#pragma region Enums

enum ObjInfo_GlobFlags : uint32 // [LegoRR/ObjInfo.c|flags:0x4|type:uint]
{
	OBJINFO_GLOB_FLAG_NONE               = 0,
	OBJINFO_GLOB_FLAG_HUNGERIMAGES       = 0x1,
	OBJINFO_GLOB_FLAG_HEALTHBAR          = 0x2,
	OBJINFO_GLOB_FLAG_HEALTHBAR_VERTICAL = 0x8,
	OBJINFO_GLOB_FLAG_BUBBLEIMAGES       = 0x10,
};
flags_end(ObjInfo_GlobFlags, 0x4);

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

struct ObjInfo_Globs // [LegoRR/ObjInfo.c|struct:0x78|tags:GLOBS]
{
	/*00,4*/	ObjInfo_GlobFlags flags;
	/*04,8*/	Point2F HealthBarPosition;
	/*0c,8*/	Size2F HealthBarWidthHeight;
	/*14,4*/	uint32 HealthBarBorderSize;
	/*18,c*/	real32 HealthBarBorderRGB_r[3]; // [normal, hi, lo]
	/*24,c*/	real32 HealthBarBorderRGB_g[3]; // [normal, hi, lo]
	/*30,c*/	real32 HealthBarBorderRGB_b[3]; // [normal, hi, lo]
	/*3c,c*/	ColourRGBF HealthBarRGB;
	/*48,c*/	ColourRGBF HealthBarBackgroundRGB;
	/*54,8*/	Point2F HungerPosition;
	/*5c,14*/	Gods98::Image* HungerImages[OBJINFO_HUNGERIMAGECOUNT];
	/*70,8*/	Point2F BubblePosition;
	/*78*/
};
assert_sizeof(ObjInfo_Globs, 0x78);

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @00500e68>
extern ObjInfo_Globs & objinfoGlobs;

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

/// CUSTOM: Invalidate existing pre-rendered health bars. Use when changing health bar settings at runtime.
void ObjInfo_InvalidateHealthBars();

/// CUSTOM:
void ObjInfo_TryBeginDraw();

/// CUSTOM:
void ObjInfo_TryEndDraw();


// <LegoRR.exe @004597f0>
//#define ObjInfo_Initialise ((void (__cdecl* )(const Gods98::Config* config, const char* gameName))0x004597f0)
void __cdecl ObjInfo_Initialise(const Gods98::Config* config, const char* gameName);

// <LegoRR.exe @00459820>
//#define ObjInfo_LoadHealthBar ((bool32 (__cdecl* )(const Gods98::Config* config, const char* gameName))0x00459820)
bool32 __cdecl ObjInfo_LoadHealthBar(const Gods98::Config* config, const char* gameName);

// <LegoRR.exe @00459bc0>
//#define ObjInfo_LoadHunger ((bool32 (__cdecl* )(const Gods98::Config* config, const char* gameName))0x00459bc0)
bool32 __cdecl ObjInfo_LoadHunger(const Gods98::Config* config, const char* gameName);

// <LegoRR.exe @00459c80>
//#define ObjInfo_LoadHungerImages ((void (__cdecl* )(const Gods98::Config* config, const char* gameName))0x00459c80)
void __cdecl ObjInfo_LoadHungerImages(const Gods98::Config* config, const char* gameName);

// <LegoRR.exe @00459d10>
//#define ObjInfo_LoadBubble ((bool32 (__cdecl* )(const Gods98::Config* config, const char* gameName))0x00459d10)
bool32 __cdecl ObjInfo_LoadBubble(const Gods98::Config* config, const char* gameName);

// DRAW MODE: Only Draw API drawing calls can be used within this function.
// <LegoRR.exe @00459dc0>
//#define ObjInfo_DrawHealthBar ((void (__cdecl* )(LegoObject* liveObj, sint32 screenX, sint32 screenY))0x00459dc0)
void __cdecl ObjInfo_DrawHealthBar(LegoObject* liveObj, sint32 screenX, sint32 screenY);

// <LegoRR.exe @0045a210>
//#define ObjInfo_DrawHungerImage ((void (__cdecl* )(LegoObject* liveObj, sint32 screenX, sint32 screenY))0x0045a210)
void __cdecl ObjInfo_DrawHungerImage(LegoObject* liveObj, sint32 screenX, sint32 screenY);

// <LegoRR.exe @0045a290>
//#define ObjInfo_DrawBubbleImage ((void (__cdecl* )(Gods98::Image* image, sint32 screenX, sint32 screenY))0x0045a290)
void __cdecl ObjInfo_DrawBubbleImage(Gods98::Image* image, sint32 screenX, sint32 screenY);

#pragma endregion

}
