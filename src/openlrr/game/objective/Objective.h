// Objective.h : 
//

#pragma once

#include "../GameCommon.h"


namespace LegoRR
{; // !<---

/**********************************************************************************
 ******** Forward Declarations
 **********************************************************************************/

#pragma region Forward Declarations

struct ObjectiveData;

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

enum Objective_GlobFlags : uint32 // [LegoRR/Objective.c|flags:0x4|type:uint] STATUSREADY means next status has been set, but has not been "updated" yet? HITTIMEFAIL is unused and replaced by SHOWACHIEVEDADVISOR.
{
	OBJECTIVE_GLOB_FLAG_NONE                = 0,
	OBJECTIVE_GLOB_FLAG_BRIEFING            = 0x1,
	OBJECTIVE_GLOB_FLAG_COMPLETED           = 0x2,
	OBJECTIVE_GLOB_FLAG_FAILED              = 0x4,
	OBJECTIVE_GLOB_FLAG_STATUSREADY         = 0x8,
	OBJECTIVE_GLOB_FLAG_SHOWBRIEFINGADVISOR = 0x10,
	OBJECTIVE_GLOB_FLAG_SHOWACHIEVEDADVISOR = 0x20,
	OBJECTIVE_GLOB_FLAG_HITTIMEFAIL         = 0x40,
	OBJECTIVE_GLOB_FLAG_SHOWFAILEDADVISOR   = 0x80,
	OBJECTIVE_GLOB_FLAG_CRYSTAL             = 0x100,
	OBJECTIVE_GLOB_FLAG_ORE                 = 0x200,
	OBJECTIVE_GLOB_FLAG_BLOCK               = 0x400,
	OBJECTIVE_GLOB_FLAG_TIMER               = 0x800,
	OBJECTIVE_GLOB_FLAG_CONSTRUCTION        = 0x1000,
};
flags_end(Objective_GlobFlags, 0x4);

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

struct Objective_Globs // [LegoRR/Objective.c|struct:0x28c|tags:GLOBS] Globals for objective messages (Chief briefing, etc).
{
	/*000,4*/	Objective_GlobFlags flags;
	/*004,4*/	Gods98::File* file; // PTRFileStream_00500bc4
	/*008,80*/	char filename[128]; // CHAR_ARRAY_00500bc8
	/*088,10*/	char* messages[4]; // [Briefing,Completion,Failure,CrystalFailure] Strings containing text of entire status message (pages are separated with '\a').
	/*098,180*/	undefined reserved1[384]; // (possibly unused array of char[3][128])
	/*218,10*/	uint32 currentPages[4]; // (1-indexed) Current page number of the displayed status type. (g_Objective_StatusUnkCounts)
	/*228,10*/	uint32 currentPageStates[4]; // (1-indexed) State tracking for page to switch to (this is only used to check if the above field needs to trigger an update). (g_Objective_StatusUnkCounts2)
	/*238,10*/	uint32 pageCounts[4]; // Number of pages for the specific status. (g_Objective_StatusBellCounts)
	/*248,10*/	Gods98::TextWindow* textWindows[4]; // Text windows for the specific status. (g_Objective_StatusTextWindows)
	/*258,c*/	Gods98::TextWindow* pageTextWindows[3]; // PTRTextWindow_00500e18, PTRTextWindow_00500e1c, PTRTextWindow_00500e20
	/*264,4*/	Gods98::TextWindow* beginTextWindow; // Unknown usage, only worked with when line "[BEGIN]" is found (PTRTextWindow_00500e24)
	/*268,4*/	undefined4 reserved2;
	/*26c,4*/	bool32 hasBeginText; // True when text has been assigned to beginTextWindow (BOOL_00500e2c)
	/*270,4*/	bool32 achieved; // True if the level was has ended successfully. (g_LevelIsComplete)
	/*274,4*/	bool32 objectiveSwitch; // (see: NERPFunc__SetObjectiveSwitch)
	/*278,4*/	char* soundName;
	/*27c,4*/	sint32 soundHandle; // (init: -1 when unused) INT_00500e3c
	/*280,4*/	real32 soundTimer; // (init: (playTime - 1.5f) * 25.0f) FLOAT_00500e40
	/*284,4*/	bool32 showing; // True when an objective is currently being shown or changed to(?)
	/*288,4*/	bool32 endTeleportEnabled; // (cfg: ! DisableEndTeleport, default: false (enabled))
	/*28c*/
};

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @00500bc0>
extern Objective_Globs & objectiveGlobs;

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

// <LegoRR.exe @004577a0>
#define Objective_LoadObjectiveText ((void (__cdecl* )(const Gods98::Config* config, const char* gameName, const char* levelName, Lego_Level* level, const char* filename))0x004577a0)

// <LegoRR.exe @00458000>
#define Objective_LoadLevel ((void (__cdecl* )(const Gods98::Config* config, const char* gameName, const char* levelName, Lego_Level* level, uint32 screenWidth, uint32 screenHeight))0x00458000)

// <LegoRR.exe @00458840>
#define Objective_SetCryOreObjectives ((void (__cdecl* )(Lego_Level* level, uint32 crystals, uint32 ore))0x00458840)

// <LegoRR.exe @00458880>
#define Objective_SetBlockObjective ((void (__cdecl* )(Lego_Level* level, const Point2I* blockPos))0x00458880)

// <LegoRR.exe @004588b0>
#define Objective_SetTimerObjective ((void (__cdecl* )(Lego_Level* level, real32 timer, bool32 hitTimeFail))0x004588b0)

// <LegoRR.exe @004588e0>
#define Objective_SetConstructionObjective ((void (__cdecl* )(Lego_Level* level, LegoObject_Type objType, LegoObject_ID objID))0x004588e0)

// <LegoRR.exe @00458910>
#define Objective_IsObjectiveAchieved ((bool32 (__cdecl* )(void))0x00458910)

// <LegoRR.exe @00458920>
#define Objective_SetEndTeleportEnabled ((void (__cdecl* )(bool32 on))0x00458920)

// <LegoRR.exe @00458930>
#define Objective_SetStatus ((void (__cdecl* )(LevelStatus status))0x00458930)

// <LegoRR.exe @00458ba0>
#define Objective_ProgrammerModeGT3_FUN_00458ba0 ((void (__cdecl* )(void))0x00458ba0)

// <LegoRR.exe @00458c60>
#define Objective_IsEnded ((bool32 (__cdecl* )(void))0x00458c60)

// <LegoRR.exe @00458c80>
#define Objective_HandleKeys ((bool32 (__cdecl* )(bool32 spaceKeyHeld, bool32 leftButtonReleasedUnk, OUT bool32* gotoNextLevel))0x00458c80)

// <LegoRR.exe @00458ea0>
#define Objective_Update ((void (__cdecl* )(Gods98::TextWindow* textWnd, Lego_Level* level, real32 elapsedGame, real32 elapsedAbs))0x00458ea0)

// <LegoRR.exe @00459310>
#define Objective_CheckCompleted ((bool32 (__cdecl* )(Lego_Level* level, OUT bool32* timerFailed, real32 elapsed))0x00459310)

// <LegoRR.exe @004593c0>
#define Objective_Callback_CountObjects ((bool32 (__cdecl* )(LegoObject* liveObj, ObjectiveData* objective))0x004593c0)

#pragma endregion

}
