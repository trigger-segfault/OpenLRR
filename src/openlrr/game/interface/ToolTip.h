// ToolTip.h : 
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

#define TOOLTIP_MAXICONS			20
#define TOOLTIP_BUFFERSIZE			512

#pragma endregion

/**********************************************************************************
 ******** Enumerations
 **********************************************************************************/

#pragma region Enums

enum ToolTipFlags : uint32 // [LegoRR/ToolTip.c|flags:0x4|type:uint]
{
	TOOLTIP_FLAG_NONE   = 0,
	TOOLTIP_FLAG_UNK_1  = 0x1,
	TOOLTIP_FLAG_UNK_2  = 0x2,
	TOOLTIP_FLAG_UNK_4  = 0x4,
	TOOLTIP_FLAG_UNK_8  = 0x8,
	TOOLTIP_FLAG_UNK_10 = 0x10,
};
flags_end(ToolTipFlags, 0x4);

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

struct ToolTipData // [LegoRR/ToolTip.c|struct:0x27c]
{
	/*000,4*/	real32 timer;
	/*004,200*/	char textBuffer[TOOLTIP_BUFFERSIZE];
	/*204,4*/	uint32 textWidth;
	/*208,4*/	uint32 textLineCount;
	/*20c,50*/	Gods98::Image* iconList[TOOLTIP_MAXICONS];
	/*25c,4*/	uint32 iconCount;
	/*260,4*/	sint32 field_260;
	/*264,4*/	undefined4 field_264;
	/*268,4*/	undefined4 field_268;
	/*26c,4*/	sint32 field_26c; // box width?
	/*270,4*/	Gods98::Image* tooltipImage;
	/*274,4*/	SFX_ID sfxType;
	/*278,4*/	ToolTipFlags flags; // (flag 0x8 IMAGE is not for "iconList")
	/*27c*/
};
assert_sizeof(ToolTipData, 0x27c);


struct ToolTip_Globs // [LegoRR/ToolTip.c|struct:0x61c4|tags:GLOBS]
{
	/*0000,4*/	Gods98::Font* font; // (init only)
	/*0004,4*/	uint32 fontHeight; // (init only)
	/*0008,4*/	uint32 int2_8; // (init only)
	/*000c,4*/	sint32 int1_c; // (init only)
	/*0010,4*/	uint32 width; // (init only)
	/*0014,4*/	uint32 height; // (init only)
	/*0018,4*/	sint32 int32_18; // (init only)
	/*001c,4*/	real32 hoverTime; // Duration before showing tooltip  (init only)
	/*0020,24*/	real32 rgbFloats[9]; // [r:g:b(3)][norm:hi:lo(3)]  (init only)
	/*0044,9c*/	const char* toolTipName[ToolTip_Type_Count]; // (init only)
	/*00e0,60e4*/	ToolTipData toolTipData[ToolTip_Type_Count];
	/*61c4*/
};
assert_sizeof(ToolTip_Globs, 0x61c4);

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @004ab64c>
extern bool32 & g_ToolTipIsSFXPlaying;

// <LegoRR.exe @0054cf20>
extern ToolTip_Globs & toolTipGlobs;

#pragma endregion

/**********************************************************************************
 ******** Macros
 **********************************************************************************/

#pragma region Macros

#define ToolTip_RegisterName(n)		(toolTipGlobs.toolTipName[n]=#n)

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @0046b490>
#define ToolTip_Initialise ((void (__cdecl* )(Gods98::Font* font, sint32 value2, sint32 value1, real32 float1, uint32 width, uint32 height, sint32 value32, real32 red, real32 green, real32 blue))0x0046b490)
//void __cdecl ToolTip_Initialise(Gods98::Font* font, sint32 value2, sint32 value1, real32 float1, uint32 width, uint32 height, sint32 value32, real32 red, real32 green, real32 blue);

// <LegoRR.exe @0046b790>
#define ToolTip_GetType ((bool32 (__cdecl* )(const char* toolTipName, OUT ToolTip_Type* toolTipType))0x0046b790)
//bool32 __cdecl ToolTip_GetType(const char* toolTipName, OUT ToolTip_Type* toolTipType);

// <LegoRR.exe @0046b7e0>
#define ToolTip_SetText ((void (__cdecl* )(ToolTip_Type toolTipType, const char* msg, ...))0x0046b7e0)
//void __cdecl ToolTip_SetText(ToolTip_Type toolTipType, const char* msg, ...);

// <LegoRR.exe @0046b920>
#define ToolTip_AddIcon ((void (__cdecl* )(ToolTip_Type toolTipType, Gods98::Image* image))0x0046b920)
//void __cdecl ToolTip_AddIcon(ToolTip_Type toolTipType, Gods98::Image* image);

// <LegoRR.exe @0046b9c0>
#define ToolTip_SetSFX ((void (__cdecl* )(ToolTip_Type toolTipType, SFX_ID sfxType))0x0046b9c0)
//void __cdecl ToolTip_SetSFX(ToolTip_Type toolTipType, SFX_ID sfxType);

// <LegoRR.exe @0046b9f0>
#define ToolTip_SetFlag10 ((void (__cdecl* )(ToolTip_Type toolTipType, bool32 state))0x0046b9f0)
//void __cdecl ToolTip_SetFlag10(ToolTip_Type toolTipType, bool32 state);

// <LegoRR.exe @0046ba30>
#define ToolTip_AddFlag4 ((void (__cdecl* )(ToolTip_Type toolTipType))0x0046ba30)
//void __cdecl ToolTip_AddFlag4(ToolTip_Type toolTipType);

// <LegoRR.exe @0046ba60>
#define ToolTip_ResetTimer ((void (__cdecl* )(ToolTip_Type toolTipType))0x0046ba60)
//void __cdecl ToolTip_ResetTimer(ToolTip_Type toolTipType);

// <LegoRR.exe @0046ba80>
#define ToolTip_Update ((void (__cdecl* )(uint32 mousex, uint32 mousey, real32 elapsedAbs))0x0046ba80)
//void __cdecl ToolTip_Update(uint32 mousex, uint32 mousey, real32 elapsedAbs);

// For some weird reason, Area2F is passed BY VALUE here.
// The only reason this was even determined was due to the weird compiler behavior when calling this function.
// <LegoRR.exe @0046bb70>
#define ToolTip_DrawBox ((void (__cdecl* )(Area2F valueRect, real32 rcRed, real32 rcGreen, real32 rcBlue, real32 ln1Red, real32 ln1Green, real32 ln1Blue, real32 ln2Red, real32 ln2Green, real32 ln2Blue, bool32 halfTrans))0x0046bb70)
//void __cdecl ToolTip_DrawBox(Area2F valueRect, real32 rcRed, real32 rcGreen, real32 rcBlue, real32 ln1Red, real32 ln1Green, real32 ln1Blue, real32 ln2Red, real32 ln2Green, real32 ln2Blue, bool32 halfTrans);

// <LegoRR.exe @0046bef0>
#define ToolTip_Draw ((void (__cdecl* )(ToolTipData* toolTip, uint32 x, uint32 y))0x0046bef0)
//void __cdecl ToolTip_Draw(ToolTipData* toolTip, uint32 x, uint32 y);

#pragma endregion

}
