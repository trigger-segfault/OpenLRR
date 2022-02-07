// HelpWindow.h : 
//

#pragma once

#include "../../common.h"

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

// For some reason this does not match DEPENDENCY_MAXUNLOCKS (32)...
#define HELPWINDOW_MAXUNLOCKS		30

#define HELPWINDOW_MAXBUTTONS		4

#pragma endregion

/**********************************************************************************
 ******** Enumerations
 **********************************************************************************/

#pragma region Enums

enum HelpWindow_GlobFlags : uint32 // [LegoRR/HelpWindow.c|flags:0x4|type:uint]
{
	HELPWINDOW_GLOB_FLAG_NONE    = 0,
	HELPWINDOW_GLOB_FLAG_UNK_1   = 0x1,
	HELPWINDOW_GLOB_FLAG_UNK_2   = 0x2,
	HELPWINDOW_GLOB_FLAG_UNK_4   = 0x4,
	HELPWINDOW_GLOB_FLAG_UNK_8   = 0x8,
	HELPWINDOW_GLOB_FLAG_UNK_10  = 0x10,
	HELPWINDOW_GLOB_FLAG_UNK_20  = 0x20,
	HELPWINDOW_GLOB_FLAG_ENABLED = 0x40,
};
flags_end(HelpWindow_GlobFlags, 0x4);

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

struct HelpWindowInfoLevels // [LegoRR/HelpWindow.c|struct:0x40]
{
	/*00,40*/	char* levels[OBJECT_MAXLEVELS];
	/*40*/
};
assert_sizeof(HelpWindowInfoLevels, 0x40);


// Same struct as MenuButton (which is possibly only used by HelpWindow)
struct HelpWindowButton // [LegoRR/FrontEnd.c|struct:0x1c]
{
	/*00,10*/	Area2F window;
	/*10,4*/	Gods98::Image* hiImage;
	/*14,4*/	Gods98::Image* loImage;
	/*18,4*/	char* text;
	/*1c*/
};
assert_sizeof(HelpWindowButton, 0x1c);


struct HelpWindow_Globs // [LegoRR/HelpWindow.c|struct:0xd6c|tags:GLOBS]
{
	/*000,4*/	Gods98::Font* font;
	/*004,8*/	Point2F TextWindowPosition;
	/*00c,4*/	real32 TextWindowBottom; // (Position.y + height)
	/*010,4*/	Gods98::Image* BackgroundImage;
	/*014,8*/	Point2F BackgroundPosition;
	/*01c,70*/	HelpWindowButton Buttons[HELPWINDOW_MAXBUTTONS]; // [Continue, CancelFutureButton, NULL, NULL]
	/*08c,4*/	Gods98::TextWindow* textWnd;
	/*090,3c0*/	char* VehicleInfos[LegoObject_ID_Count][OBJECT_MAXLEVELS];
	/*450,3c0*/	char* MiniFigureInfos[LegoObject_ID_Count][OBJECT_MAXLEVELS];
	/*810,3c0*/	char* BuildingInfos[LegoObject_ID_Count][OBJECT_MAXLEVELS];
	/*bd0,4*/	char* CanBuildMessage;
	/*bd4,4*/	char* CanTransportMessage;
	/*bd8,4*/	char* CanBuildAndTransportMessage;
	/*bdc,4*/	char* activeObjName;
	/*be0,4*/	LegoObject_Type activeObjType;
	/*be4,4*/	LegoObject_ID activeObjID;
	/*be8,4*/	uint32 activeObjLevel;
	/*bec,78*/	LegoObject_Type unlockedObjTypes[HELPWINDOW_MAXUNLOCKS];
	/*c64,78*/	LegoObject_ID unlockedObjIDs[HELPWINDOW_MAXUNLOCKS];
	/*cdc,78*/	uint32 unlockedObjLevels[HELPWINDOW_MAXUNLOCKS];
	/*d54,4*/	uint32 unlockedCount;
	/*d58,4*/	real32 float_d58;
	/*d5c,4*/	real32 float_d5c;
	/*d60,4*/	uint32 count_d60;
	/*d64,4*/	undefined4 field_d64;
	/*d68,4*/	HelpWindow_GlobFlags flags;
	/*d6c*/
};
assert_sizeof(HelpWindow_Globs, 0xd6c);

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @004dc8e8>
extern HelpWindow_Globs & helpwindowGlobs;

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

// <LegoRR.exe @004180c0>
#define HelpWindow_SetFont ((void (__cdecl* )(Gods98::Font* font))0x004180c0)

// <LegoRR.exe @004180d0>
#define HelpWindow_ClearFlag1 ((void (__cdecl* )(void))0x004180d0)

// <LegoRR.exe @004180e0>
#define HelpWindow_Initialise ((void (__cdecl* )(const Gods98::Config* config, const char* gameName))0x004180e0)

// <LegoRR.exe @00418380>
#define HelpWindow_LoadLevelsInfo ((void (__cdecl* )(const Gods98::Config* config, const char* gameName))0x00418380)

// <LegoRR.exe @00418520>
#define HelpWindow_LoadButtons ((void (__cdecl* )(const Gods98::Config* config, const char* gameName))0x00418520)

// <LegoRR.exe @00418730>
#define HelpWindow_IfFlag4_AndParam_Clear1_Set2_Else_Clear3 ((void (__cdecl* )(bool32 state))0x00418730)

// <LegoRR.exe @00418760>
#define HelpWindow_RecallDependencies ((void (__cdecl* )(LegoObject_Type objType, LegoObject_ID objID, uint32 objLevel, bool32 noHelpWindow))0x00418760)

// <LegoRR.exe @00418850>
#define HelpWindow_Object_Unlock ((void (__cdecl* )(LegoObject_Type objType, LegoObject_ID objID, uint32 objLevel))0x00418850)

// <LegoRR.exe @00418900>
#define HelpWindow_Close_FUN_00418900 ((void (__cdecl* )(void))0x00418900)

// <LegoRR.exe @00418930>
#define HelpWindow_FUN_00418930 ((void (__cdecl* )(void))0x00418930)

// <LegoRR.exe @00418cd0>
#define HelpWindow_FUN_00418cd0 ((bool32 (__cdecl* )(uint32 mouseX, uint32 mouseY, bool32 leftButton, bool32 leftLast, bool32 leftReleased, real32 elapsed))0x00418cd0)

// <LegoRR.exe @00418eb0>
#define HelpWindow_ToolTip_FUN_00418eb0 ((void (__cdecl* )(sint32 param_1))0x00418eb0)

// <LegoRR.exe @00418ef0>
#define HelpWindow_FUN_00418ef0 ((bool32 (__cdecl* )(sint32 param_1, sint32 param_2, sint32 param_3))0x00418ef0)

// <LegoRR.exe @00418f60>
#define HelpWindow_FUN_00418f60 ((void (__cdecl* )(undefined4 param_1, real32 elapsed))0x00418f60)

// <LegoRR.exe @00419030>
#define HelpWindow_DrawButtons ((void (__cdecl* )(void))0x00419030)

// <LegoRR.exe @00419080>
#define HelpWindow_GetButtonImage ((Gods98::Image* (__cdecl* )(sint32 buttonIndex))0x00419080)

// <LegoRR.exe @004190d0>
#define HelpWindow_IsEnabled_AndFlags_3_AndNoTutorialFlags ((bool32 (__cdecl* )(void))0x004190d0)

// <LegoRR.exe @00419120>
#define HelpWindow_IsEnabled ((bool32 (__cdecl* )(void))0x00419120)

// <LegoRR.exe @00419130>
#define HelpWindow_SetEnabled ((void (__cdecl* )(bool32 toggle, bool32 enable))0x00419130)

#pragma endregion

}
