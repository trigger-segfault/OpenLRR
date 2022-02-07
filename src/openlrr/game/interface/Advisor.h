// Advisor.h : 
//

#pragma once

#include "../../common.h"
#include "../../engine/geometry.h"
#include "../../engine/core/Config.h"
#include "../../engine/gfx/Viewports.h"

#include "../audio/SFX.h"
#include "../world/Camera.h"
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

enum AdvisorFlags : uint32 // [LegoRR/Advisor.c|flags:0x4|type:uint]
{
	ADVISOR_FLAG_NONE    = 0,
	ADVISOR_FLAG_NOPANEL = 0x10000,
	ADVISOR_FLAG_USED    = 0x20000, // This Advisor_Type has been loaded
	ADVISOR_FLAG_HASTEXT = 0x40000,
};
flags_end(AdvisorFlags, 0x4);


enum Advisor_GlobFlags : uint32 // [LegoRR/Advisor.c|flags:0x4|type:uint]
{
	ADVISOR_GLOB_FLAG_NONE      = 0,
	ADVISOR_GLOB_FLAG_ANIMATING = 0x1,
	ADVISOR_GLOB_FLAG_LOOPING   = 0x2,
};
flags_end(Advisor_GlobFlags, 0x4);

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

struct AdvisorAnimData // [LegoRR/Advisor.c|struct:0xc]
{
	/*0,4*/	Gods98::Container* cont;
	/*4,4*/	real32 loopStartTime; // ignored when not looping
	/*8,4*/	real32 loopEndTime;   // ignored when not looping
	/*c*/
};
assert_sizeof(AdvisorAnimData, 0xc);


struct AdvisorPositionData // [LegoRR/Advisor.c|struct:0x24]
{
	/*00,4*/	Advisor_Anim animType;
	/*04,4*/	Text_Type textType;
	/*08,4*/	SFX_ID sfxID;
	/*0c,4*/	Panel_Type panelType;
	/*10,4*/	real32 x; // Mutable x position
	/*14,4*/	real32 y; // Mutable y position
	/*18,4*/	real32 origX; // Immutable x position (set once)
	/*1c,4*/	real32 origY; // Immutable y position (set once)
	/*20,4*/	AdvisorFlags flags; // (init: 0x20000), 0x10000 = NULL panel, 0x40000 = non-NULL text
	/*24*/
};
assert_sizeof(AdvisorPositionData, 0x24);


struct Advisor_Globs // [LegoRR/Advisor.c|struct:0x410|tags:GLOBS]
{
	/*000,54*/	const char* positionName[Advisor_Type_Count];
	/*054,2f4*/	AdvisorPositionData positions[Advisor_Type_Count];
	/*348,2c*/	const char* animName[Advisor_Anim_Count];
	/*374,84*/	AdvisorAnimData anims[Advisor_Anim_Count];
	/*3f8,4*/	Advisor_Type currType;
	/*3fc,4*/	Gods98::Container* cameraCont;
	/*400,4*/	Gods98::Viewport* viewMain;
	/*404,4*/	real32 viewZ; // Z position of every advisor type (init: 0.96f)
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
 ******** Macros
 **********************************************************************************/

#pragma region Macros

#define Advisor_RegisterPositionName(n)		(advisorGlobs.positionName[n]=#n)
#define Advisor_RegisterAnimName(n)			(advisorGlobs.animName[n]=#n)

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @00401000>
void __cdecl Advisor_Initialise(const char* gameName, LegoCamera* cameraMain, Gods98::Viewport* viewMain);

// Hard stop on the advisor animation, immediately hides.
// <LegoRR.exe @004011c0>
void __cdecl Advisor_Stop(void);

// param_unused: 200.0f is passed from Lego_Initialise
// <LegoRR.exe @00401210>
void __cdecl Advisor_InitViewport(real32 param_unused);

// <LegoRR.exe @00401270>
void __cdecl Advisor_LoadAnims(const Gods98::Config* config, const char* gameName);

// <LegoRR.exe @004013a0>
bool32 __cdecl Advisor_GetAdvisorType(const char* advisorName, OUT Advisor_Type* advisorType);

// <LegoRR.exe @00401430>
bool32 __cdecl Advisor_GetAnimType(const char* animName, OUT Advisor_Anim* animType);

// <LegoRR.exe @004014a0>
void __cdecl Advisor_LoadPositions(const Gods98::Config* config, const char* gameName, sint32 screenWidth, sint32 screenHeight);

// <LegoRR.exe @004016f0>
void __cdecl Advisor_AddPosition(Advisor_Type advisorType, Advisor_Anim animType, Text_Type textType, SFX_ID sfxID, Panel_Type panelType, real32 x, real32 y);

// <LegoRR.exe @00401780>
void __cdecl Advisor_SetParameters(Advisor_Type advisorType, Panel_Type panelType, real32 x, real32 y);

// <LegoRR.exe @004017d0>
void __cdecl Advisor_GetOrigPos(Advisor_Type advisorType, OUT real32* origX, OUT real32* origY);

// <LegoRR.exe @00401800>
bool32 __cdecl Advisor_Start(Advisor_Type advisorType, bool32 loop);

// <LegoRR.exe @00401870>
void __cdecl Advisor_PlaySFX(Advisor_Type advisorType);

// <LegoRR.exe @004018d0>
void __cdecl Advisor_Update(real32 elapsed);

// Called by Advisor_Update, returns false when the animation has finished (and is not looping).
// <LegoRR.exe @004019b0>
bool32 __cdecl Advisor_MoveAnimation(Advisor_Anim animType, real32 elapsed);

// Ends the advisor loop while allowing the final animation cycle can finish.
// <LegoRR.exe @00401a60>
void __cdecl Advisor_End(void);

// <LegoRR.exe @00401a70>
void __cdecl Advisor_ViewportTransform(Advisor_Type advisorType);

// <LegoRR.exe @00401b60>
bool32 __cdecl Advisor_IsAnimating(void);

#pragma endregion

}
