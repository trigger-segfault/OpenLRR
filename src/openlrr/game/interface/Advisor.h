// Advisor.h : 
//

#pragma once

#include "../../common.h"
#include "../../engine/colour.h"
#include "../../engine/geometry.h"
#include "../../engine/undefined.h"

#include "../../engine/gfx/Containers.h"
#include "../../engine/gfx/Viewports.h"
#include "../../engine/drawing/Images.h"

#include "../GameCommon.h"


namespace LegoRR
{; // !<---

/**********************************************************************************
 ******** Forward Declarations
 **********************************************************************************/

#pragma region Forward Declarations

struct LegoCamera;

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

enum AdvisorPositionFlags : uint32 // [LegoRR/Advisor.c|flags:0x4|type:uint]
{
	ADVISORPOS_NONE    = 0,
	ADVISORPOS_NOPANEL = 0x10000,
	ADVISORPOS_DEFAULT = 0x20000,
	ADVISORPOS_HASTEXT = 0x40000,
};
flags_end(AdvisorPositionFlags, 0x4);


enum Advisor_GlobFlags : uint32 // [LegoRR/Advisor.c|flags:0x4|type:uint]
{
	ADVISOR_GLOB_FLAG_NONE  = 0,
	ADVISOR_GLOB_FLAG_UNK_1 = 0x1,
	ADVISOR_GLOB_FLAG_UNK_2 = 0x2,
};
flags_end(Advisor_GlobFlags, 0x4);

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

struct AdvisorAnimData // [LegoRR/Advisor.c|struct:0xc]
{
	/*0,4*/	Gods98::Container* resData;
	/*4,4*/	real32 loopStartTime; // ignored when not looping
	/*8,4*/	real32 loopEndTime; // ignored when not looping
	/*c*/
};
assert_sizeof(AdvisorAnimData, 0xc);


struct AdvisorPositionData // [LegoRR/Advisor.c|struct:0x24]
{
	/*00,4*/	Advisor_Anim animType;
	/*04,4*/	Text_Type textType;
	/*08,4*/	sint32 sfxIndex;
	/*0c,4*/	Panel_Type panelType;
	/*10,8*/	Point2F point1;
	/*18,8*/	Point2F point2; // Identical to point1
	/*20,4*/	AdvisorPositionFlags flags; // (init: 0x20000), 0x10000 = NULL panel, 0x40000 = non-NULL text
	/*24*/
};
assert_sizeof(AdvisorPositionData, 0x24);


struct Advisor_Globs // [LegoRR/Advisor.c|struct:0x410|tags:GLOBS]
{
	/*000,54*/	const char* positionName[Advisor_Type_Count];
	/*054,2f4*/	AdvisorPositionData advisorPositions[Advisor_Type_Count];
	/*348,2c*/	const char* animName[Advisor_Anim_Count];
	/*374,84*/	AdvisorAnimData advisorAnims[Advisor_Anim_Count];
	/*3f8,4*/	Advisor_Type currentType;
	/*3fc,4*/	Gods98::Container* cameraCont;
	/*400,4*/	Gods98::Viewport* viewMain;
	/*404,4*/	real32 position_Z; // Always set to 0.96f  (was probably configurable at one point)
	/*408,4*/	Gods98::Container* lightCont;
	/*40c,4*/	Advisor_GlobFlags flags;
	/*410*/
};
assert_sizeof(Advisor_Globs, 0x410);

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @004b3db0>
extern Advisor_Globs & advisorGlobs;

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @00401000>
#define Advisor_Initialise ((void (__cdecl* )(const char* gameName, LegoCamera* cameraMain, Gods98::Viewport* viewMain))0x00401000)

// <LegoRR.exe @004011c0>
#define Advisor_Cleanup ((void (__cdecl* )(void))0x004011c0)

// <LegoRR.exe @00401210>
#define Advisor_InitViewport ((void (__cdecl* )(real32 param_unused))0x00401210)

// <LegoRR.exe @00401240>
#define Maths_Vector2DDistance ((real32 (__cdecl* )(const Point2F* a, const Point2F* b))0x00401240)

// <LegoRR.exe @00401270>
#define Advisor_LoadAdvisorAnims ((void (__cdecl* )(const Gods98::Config* config, const char* keyRoot))0x00401270)

// <LegoRR.exe @004013a0>
#define Advisor_GetAdvisorType ((bool32 (__cdecl* )(const char* advisorName, OUT Advisor_Type* advisorType))0x004013a0)


// <LegoRR.exe @00401430>
#define Advisor_GetAnimType ((bool32 (__cdecl* )(const char* animName, OUT Advisor_Anim* animType))0x00401430)


// <LegoRR.exe @004014a0>
#define Advisor_LoadAdvisorPositions ((void (__cdecl* )(const Gods98::Config* config, const char* keyRoot, sint32 screenWidth, sint32 screenHeight))0x004014a0)


// <LegoRR.exe @004016f0>
#define Advisor_AddAdvisorPosition ((void (__cdecl* )(Advisor_Type advisorType, Advisor_Anim animType, Text_Type textType, sint32 sfxIndex, Panel_Type panelType, real32 x, real32 y))0x004016f0)

// <LegoRR.exe @00401780>
#define Advisor_SetParameters ((void (__cdecl* )(Advisor_Type advisorType, Panel_Type panelType, real32 x1, real32 y1))0x00401780)

// <LegoRR.exe @004017d0>
#define Advisor_GetPoint2 ((void (__cdecl* )(Advisor_Type advisorType, OUT real32* x2, OUT real32* y2))0x004017d0)

// <LegoRR.exe @00401800>
#define Advisor_SetCurrentAdvisor ((bool32 (__cdecl* )(Advisor_Type advisorType, bool32 setFlag2))0x00401800)

// <LegoRR.exe @00401870>
#define Advisor_SetCurrentSound ((void (__cdecl* )(Advisor_Type advisorType))0x00401870)

// <LegoRR.exe @004018d0>
#define Advisor_Update ((void (__cdecl* )(real32 elapsed))0x004018d0)

// <LegoRR.exe @004019b0>
#define Advisor_UpdateAnimation ((bool32 (__cdecl* )(Advisor_Anim animType, real32 elapsed))0x004019b0)

// <LegoRR.exe @00401a60>
#define Advisor_ClearFlag2 ((void (__cdecl* )(void))0x00401a60)

// <LegoRR.exe @00401a70>
#define Advisor_ViewportTransform ((void (__cdecl* )(Advisor_Type advisorType))0x00401a70)


// <LegoRR.exe @00401b60>
#define Advisor_GetFlag1 ((bool32 (__cdecl* )(void))0x00401b60)

#pragma endregion

}
