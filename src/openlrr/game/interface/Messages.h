// Messages.h : 
//

#pragma once

#include "../GameCommon.h"


namespace LegoRR
{; // !<---

/**********************************************************************************
 ******** Forward Declarations
 **********************************************************************************/

#pragma region Forward Declarations

struct LegoObject;

#pragma endregion

/**********************************************************************************
 ******** Constants
 **********************************************************************************/

#pragma region Constants

#define MESSAGE_MAXTYPES		65
//#define MESSAGE_MAXSELECTED		100
#define MESSAGE_MAXHOTKEYS		10
#define MESSAGE_MAXACTIONS		2048

#define MESSAGE_NAMEPREFIX		"Message_"

#pragma endregion

/**********************************************************************************
 ******** Enumerations
 **********************************************************************************/

#pragma region Enums

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

struct MessageAction // [LegoRR/Message.c|struct:0x14]
{
	/*00,4*/	Message_Type event;
	/*04,4*/	LegoObject* argumentObj; // (this argument is only used for live objects, but it's not necessarily true that it's the same in source)
	/*08,4*/	undefined4 argument2; // (this can be any types, and depends on the message)
	/*0c,8*/	Point2I position;
	/*14*/
};
assert_sizeof(MessageAction, 0x14);


struct Message_Globs // [LegoRR/Message.c|struct:0x14380|tags:GLOBS]
{
	/*00000,14000*/	MessageAction messageTableX2[2][MESSAGE_MAXACTIONS];//(uint32)Message_Type_Count];
	/*14000,8*/	    uint32 messageCountX2[2];
	/*14008,4*/	    bool32 messageX2Bool;
	/*1400c,190*/	LegoObject* selectedUnitList[LEGO_MAXMULTISELECT];
	/*1419c,a*/	    Gods98::Keys hotkeyList[MESSAGE_MAXHOTKEYS];
	/*141a6,2*/	    undefined2 padding1;
	/*141a8,c8*/	MessageAction hotkeyMessages[MESSAGE_MAXHOTKEYS];
	/*14270,104*/	const char* messageName[Message_Type_Count];
	/*14374,4*/	    undefined4 reserved1;
	/*14378,4*/	    uint32 selectedUnitCount;
	/*1437c,4*/	    uint32 hotkeyCount;
	/*14380*/
};
assert_sizeof(Message_Globs, 0x14380);

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @004ebf20>
extern Message_Globs & messageGlobs;

#pragma endregion

/**********************************************************************************
 ******** Macros
 **********************************************************************************/

#pragma region Macros

#define Message_RegisterName(n)		(messageGlobs.messageName[n]=#n)

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @00451f90>
void __cdecl Message_Initialise(void);

// <LegoRR.exe @00452220>
void __cdecl Message_RemoveLiveObject(LegoObject* liveObj);

// <LegoRR.exe @00452290>
Message_Type __cdecl Message_LookupPTLEventIndex(const char* ptlName);

// <LegoRR.exe @004522d0>
void __cdecl Message_Debug_RegisterSelectedUnitHotkey(Gods98::Keys key, Message_Type messageType, LegoObject* argumentObj, undefined4 argument2, OUT Point2I* position);

// <LegoRR.exe @00452320>
void __cdecl Message_AddMessageAction(Message_Type messageType, LegoObject* argument1Obj, undefined4 argument2, OPTIONAL Point2I* blockPos);

// <LegoRR.exe @00452390>
#define Message_PTL_Update ((void (__cdecl* )(void))0x00452390)
//void __cdecl Message_PTL_Update(void);

// <LegoRR.exe @004526f0>
#define Message_PTL_PickRandomFloor ((void (__cdecl* )(LegoObject* liveObj))0x004526f0)
//void __cdecl Message_PTL_PickRandomFloor(LegoObject* liveObj);

// <LegoRR.exe @00452770>
#define Message_LiveObject_FUN_00452770 ((void (__cdecl* )(LegoObject* liveObj))0x00452770)
//void __cdecl Message_LiveObject_FUN_00452770(LegoObject* liveObj);

// <LegoRR.exe @004527e0>
bool32 __cdecl Message_CopySelectedUnits(OUT LegoObject*** unitsList, OUT uint32* unitsCount);

// <LegoRR.exe @00452840>
LegoObject** __cdecl Message_GetSelectedUnits2(OPTIONAL OUT uint32* count);

// <LegoRR.exe @00452870>
bool32 __cdecl Message_AnyUnitSelected(void);

// <LegoRR.exe @00452880>
LegoObject* __cdecl Message_GetPrimarySelectedUnit(void);

// <LegoRR.exe @004528a0>
LegoObject** __cdecl Message_GetSelectedUnits(void);

// <LegoRR.exe @004528b0>
uint32 __cdecl Message_GetNumSelectedUnits(void);

// <LegoRR.exe @004528c0>
void __cdecl Message_CleanupSelectedUnitsCount(void);

// <LegoRR.exe @004528d0>
#define Message_LiveObject_Check_IsSelected_OrFlags3_200000 ((bool32 (__cdecl* )(LegoObject* liveObj, OUT uint32* index))0x004528d0)
//bool32 __cdecl Message_LiveObject_Check_IsSelected_OrFlags3_200000(LegoObject* liveObj, OUT uint32* index);

// <LegoRR.exe @00452910>
bool32 __cdecl Message_FindIndexOfObject(LegoObject** objsTable, uint32 objsCount, LegoObject* thisObj, OPTIONAL OUT uint32* index);

// <LegoRR.exe @00452950>
#define Message_LiveObject_Check_FUN_00452950 ((bool32 (__cdecl* )(LegoObject* liveObj))0x00452950)
//bool32 __cdecl Message_LiveObject_Check_FUN_00452950(LegoObject* liveObj);

// <LegoRR.exe @00452980>
#define Message_PTL_Select_LiveObject ((bool32 (__cdecl* )(LegoObject* liveObj, sint32 param_2))0x00452980)
//bool32 __cdecl Message_PTL_Select_LiveObject(LegoObject* liveObj, sint32 param_2);

// <LegoRR.exe @004529a0>
#define Message_LiveObject_DoSelect_FUN_004529a0 ((bool32 (__cdecl* )(LegoObject* liveObj, bool32 param_2, bool32 param_3))0x004529a0)
//bool32 __cdecl Message_LiveObject_DoSelect_FUN_004529a0(LegoObject* liveObj, bool32 param_2, bool32 param_3);

// <LegoRR.exe @00452b30>
#define Message_LiveObject_Check_FUN_00452b30 ((bool32 (__cdecl* )(LegoObject* liveObj))0x00452b30)
//bool32 __cdecl Message_LiveObject_Check_FUN_00452b30(LegoObject* liveObj);

// <LegoRR.exe @00452b80>
#define Message_PTL_ReduceSelection ((void (__cdecl* )(void))0x00452b80)
//void __cdecl Message_PTL_ReduceSelection(void);

// <LegoRR.exe @00452ea0>
#define Message_PTL_ClearSelection ((void (__cdecl* )(void))0x00452ea0)
//void __cdecl Message_PTL_ClearSelection(void);

// <LegoRR.exe @00452f10>
#define Message_PTL_Deselect_LiveObject ((bool32 (__cdecl* )(LegoObject* liveObj))0x00452f10)
//bool32 __cdecl Message_PTL_Deselect_LiveObject(LegoObject* liveObj);

// <LegoRR.exe @00452f80>
#define Message_PTL_Debug_DestroyAll ((uint32 (__cdecl* )(void))0x00452f80)
//uint32 __cdecl Message_PTL_Debug_DestroyAll(void);

// <LegoRR.exe @00453020>
#define Message_PTL_FirstPerson ((bool32 (__cdecl* )(uint32 cameraFrame))0x00453020)
//bool32 __cdecl Message_PTL_FirstPerson(uint32 cameraFrame);

#pragma endregion

}
