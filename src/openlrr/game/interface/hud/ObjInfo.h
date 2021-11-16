// hud/ObjInfo.h : 
//

#pragma once

#include "../../../common.h"
#include "../../../engine/colour.h"
#include "../../../engine/geometry.h"
#include "../../../engine/undefined.h"

#include "../../../engine/gfx/Containers.h"
#include "../../../engine/drawing/Images.h"

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
	/*5c,14*/	Gods98::Image* HungerImages[5];
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
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @004597f0>
#define ObjInfo_Initialise ((void (__cdecl* )(const Gods98::Config* config, const char* rootPath))0x004597f0)

// <LegoRR.exe @00459820>
#define ObjInfo_LoadHealthBar ((bool32 (__cdecl* )(const Gods98::Config* config, const char* rootPath))0x00459820)

// <LegoRR.exe @00459bc0>
#define ObjInfo_LoadHunger ((bool32 (__cdecl* )(const Gods98::Config* config, const char* rootPath))0x00459bc0)

// <LegoRR.exe @00459c80>
#define ObjInfo_LoadHungerImages ((void (__cdecl* )(const Gods98::Config* config, const char* rootPath))0x00459c80)

// <LegoRR.exe @00459d10>
#define ObjInfo_LoadBubble ((bool32 (__cdecl* )(const Gods98::Config* config, const char* rootPath))0x00459d10)

// <LegoRR.exe @00459dc0>
#define ObjInfo_DrawHealthBar ((void (__cdecl* )(LegoObject* liveObj, sint32 screenX, sint32 screenY))0x00459dc0)

// <LegoRR.exe @0045a210>
#define ObjInfo_DrawHungerImage ((void (__cdecl* )(LegoObject* liveObj, sint32 screenX, sint32 screenY))0x0045a210)

// <LegoRR.exe @0045a290>
#define ObjInfo_DrawBubbleImage ((void (__cdecl* )(Gods98::Image* image, sint32 screenX, sint32 screenY))0x0045a290)

#pragma endregion

}
