// AITask.h : 
//

#pragma once

#include "../../engine/core/ListSet.hpp"

#include "../GameCommon.h"


namespace LegoRR
{; // !<---

/**********************************************************************************
 ******** Forward Declarations
 **********************************************************************************/

#pragma region Forward Declarations

struct AITask;

#pragma endregion

/**********************************************************************************
 ******** Constants
 **********************************************************************************/

#pragma region Constants

#define AITASK_MAXLISTS				12			// 2^12 - 1 possible tasks...

#pragma endregion

/**********************************************************************************
 ******** Function Typedefs
 **********************************************************************************/

#pragma region Function Typedefs

typedef bool32 (__cdecl* AITask_RunThroughListsCallback)(AITask* aiTask, void* data);

#pragma endregion

/**********************************************************************************
 ******** Enumerations
 **********************************************************************************/

#pragma region Enums

enum AITaskFlags : uint32 // [LegoRR/AITask.c|flags:0x4|type:uint]
{
	AITASK_FLAG_NONE = 0,

	// Todo...
};
flags_end(AITaskFlags, 0x4);


enum AITask_GlobFlags : uint32 // [LegoRR/AITask.c|flags:0x4|type:uint]
{
	AITASK_GLOB_FLAG_NONE = 0,

	// Todo...
};
flags_end(AITask_GlobFlags, 0x4);

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

struct AITask // [LegoRR/AITask.c|struct:0x68|tags:LISTSET]
{
	/*00,4*/	AITask_Type taskType;
	/*04,4*/	undefined4 field_4;
	/*08,8*/	Point2I blockPos;
	/*10,4*/	LegoObject* object_10;
	/*14,4*/	real32 unkExpiryTime;
	/*18,4*/	real32 float_18;
	/*1c,4*/	uint32 priorityValue;
	/*20,4*/	LegoObject_Type objType;
	/*24,4*/	LegoObject_ID objID;
	/*28,4*/	uint32 objLevel;
	/*2c,4*/	uint32 constructHandle;
	/*30,8*/	Point2F pointf_30; // probably a table
	/*38,4*/	LegoObject_ToolType toolType;
	/*3c,4*/	LegoObject_AbilityType trainFlags;
	/*40,4*/	LegoObject** unitList;
	/*44,4*/	uint32 unitListCount;
	/*48,4*/	LegoObject* object_48;
	/*4c,4*/	AI_Priority priorityType;
	/*50,4*/	uint32 taskStartTime;
	/*54,4*/	Message_Type completeAction;
	/*58,4*/	AITask* aiTask_58;
	/*5c,4*/	AITaskFlags flags; // not set means ptr_40 is assigned
	/*60,4*/	AITask* next; // assigned to DAT_004b4358
	/*64,4*/	AITask* nextFree; // (for listSet)
	/*68*/
};
assert_sizeof(AITask, 0x68);


struct AITask_Globs // [LegoRR/AITask.c|struct:0x4e9c|tags:GLOBS]
{
	/*0000,30*/	AITask* listSet[AITASK_MAXLISTS];
	/*0030,4*/	AITask* freeList;
	/*0034,4*/	uint32 listCount;
	/*0038,7c*/	const char* aitaskName[AITask_Type_Count];
	/*00b4,6c*/	const char* priorityName[AI_Priority_Count];
	/*0120,6c*/	uint32 priorityValues[AI_Priority_Count];
	/*018c,4*/	AITask* AITaskUnkPtr;
	/*0190,4*/	AITask* AITaskDataNext;
	/*0194,c8*/	LegoObject* liveObjsTable_1[50];
	/*025c,4*/	uint32 liveObjsCount_1;
	/*0260,c8*/	LegoObject* liveObjsTable_2[50];
	/*0328,4*/	uint32 liveObjsCount_2;
	/*032c,4b00*/	uint32 requestObjCounts[LegoObject_Type_Count][LegoObject_ID_Count][OBJECT_MAXLEVELS];
	/*4e2c,6c*/	bool32 disabledPriorities[AI_Priority_Count];
	/*4e98,4*/	AITask_GlobFlags flags;
	/*4e9c*/
};
assert_sizeof(AITask_Globs, 0x4e9c);


using AITask_ListSet = ListSet::WrapperCollection<AITask_Globs>;

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @004b41c8>
extern AITask_Globs & aiGlobs;

extern AITask_ListSet aiListSet;

#pragma endregion

/**********************************************************************************
 ******** Macros
 **********************************************************************************/

#pragma region Macros

#define AITask_RegisterName(n)		(aiGlobs.aitaskName[n]=#n)
#define AIPriority_RegisterName(n)	(aiGlobs.priorityName[n]=#n)

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @00401bf0>
#define AIPriority_GetType ((bool32 (__cdecl* )(const char* aiPriorityName, OUT AI_Priority* priorityType))0x00401bf0)

// <LegoRR.exe @00401c30>
#define AITask_Initialise ((void (__cdecl* )(void))0x00401c30)

// <LegoRR.exe @00401f40>
#define AITask_FUN_00401f40 ((void (__cdecl* )(AITask_Type taskType, LegoObject* liveObj, OPTIONAL const Point2I* blockPos))0x00401f40)

// <LegoRR.exe @00401fa0>
#define AITask_SetPriorityType ((void (__cdecl* )(AITask* aiTask, AI_Priority priorityType))0x00401fa0)

// <LegoRR.exe @00401fd0>
#define AITask_Game_SetAITaskValue2 ((void (__cdecl* )(sint32 index, uint32 aiTaskValue))0x00401fd0)

// <LegoRR.exe @00401fe0>
#define AITask_Game_SetIsPriorityDisabled ((void (__cdecl* )(sint32 priorityIndex, bool32 isDisabled))0x00401fe0)

// <LegoRR.exe @00401ff0>
#define AITask_Game_IsPriorityDisabled ((bool32 (__cdecl* )(sint32 priorityIndex))0x00401ff0)

// <LegoRR.exe @00402000>
#define AITask_Shutdown ((void (__cdecl* )(void))0x00402000)

// <LegoRR.exe @00402040>
#define AITask_Reset_Or_ClearFlag2 ((void (__cdecl* )(bool32 unsetFlag2))0x00402040)

// <LegoRR.exe @004020b0>
#define AITask_Game_SetNoGather ((void (__cdecl* )(bool32 noGather))0x004020b0)

// <LegoRR.exe @00402120>
#define AITask_RemoveCallback ((bool32 (__cdecl* )(AITask* aiTask))0x00402120)

// <LegoRR.exe @00402150>
#define AITask_Update ((void (__cdecl* )(real32 elapsedGame))0x00402150)

// <LegoRR.exe @00402240>
#define AITask_FUN_00402240 ((void (__cdecl* )(IN OUT AITask** aiTask))0x00402240)

// <LegoRR.exe @00402440>
#define AITask_DoDig_AtBlockPos ((void (__cdecl* )(const Point2I* blockPos, bool32 param_2, bool32 param_3))0x00402440)

// <LegoRR.exe @00402530>
#define AITask_UnkInitRouting_FUN_00402530 ((void (__cdecl* )(AITask* aiTask, bool32 dropCarried))0x00402530)

// <LegoRR.exe @004025a0>
#define AITask_DoAttackRockMonster_Target ((void (__cdecl* )(LegoObject* targetObj))0x004025a0)

// <LegoRR.exe @004025f0>
#define AITask_LiveObject_FUN_004025f0 ((void (__cdecl* )(LegoObject* liveObj))0x004025f0)

// <LegoRR.exe @00402630>
#define AITask_IsCollectAndSameTarget ((bool32 (__cdecl* )(AITask* aiTask, LegoObject* liveObj))0x00402630)

// <LegoRR.exe @00402650>
#define AITask_DoCollect ((void (__cdecl* )(LegoObject* liveObj, real32 param_2))0x00402650)

// <LegoRR.exe @004026d0>
#define AITask_DoBuildPath_AtPosition ((void (__cdecl* )(const Point2I* blockPos))0x004026d0)

// <LegoRR.exe @00402730>
#define AITask_DoGather_Count ((void (__cdecl* )(sint32 count))0x00402730)

// <LegoRR.exe @00402780>
#define AITask_DoCollect_Target ((void (__cdecl* )(LegoObject* targetObj))0x00402780)

// <LegoRR.exe @004027c0>
#define AITask_DoTrain_Target ((void (__cdecl* )(LegoObject* targetObj, LegoObject_AbilityFlags trainFlags, bool32 param_3))0x004027c0)

// <LegoRR.exe @00402890>
#define AITask_DoFindDriver_Target ((void (__cdecl* )(LegoObject* targetObj))0x00402890)

// <LegoRR.exe @00402970>
#define AITask_DoCallbacks_IfAttackPath_FUN_00402970 ((bool32 (__cdecl* )(const Point2I* blockPos))0x00402970)

// DATA: Point2I**
// <LegoRR.exe @004029a0>
#define AITask_Callback_IfAttackPath_FUN_004029a0 ((bool32 (__cdecl* )(AITask* aiTask, void* search))0x004029a0)

// <LegoRR.exe @004029d0>
#define AITask_LiveObject_DoCallbacks_FUN_004029d0 ((bool32 (__cdecl* )(void* liveObj))0x004029d0)

// <LegoRR.exe @004029f0>
#define AITask_Callback_FUN_004029f0 ((bool32 (__cdecl* )(AITask* aiTask, LegoObject* liveObj))0x004029f0)

// <LegoRR.exe @00402a10>
#define AITask_Block_DeselectDig ((void (__cdecl* )(const Point2I* blockPos, bool32 digConnection))0x00402a10)

// <LegoRR.exe @00402a60>
#define AITask_Block_FUN_00402a60 ((bool32 (__cdecl* )(const Point2I* blockPos))0x00402a60)

// <LegoRR.exe @00402a90>
#define AITask_DoCallbacks_Block_FUN_00402a90 ((bool32 (__cdecl* )(const Point2I* blockPos, bool32 param_2))0x00402a90)

// DATA: SearchAITaskDeselect_8*
// struct SearchAITaskDeselect_8 {
//     const Point2I* blockPos;
//     bool32 digConnection;
// };
// <LegoRR.exe @00402ae0>
#define AITask_Callback_DeselectDig ((bool32 (__cdecl* )(AITask* aiTask, void* search))0x00402ae0)

// DATA: Point2I**
// <LegoRR.exe @00402b50>
#define AITask_Callback_FUN_00402b50 ((bool32 (__cdecl* )(AITask* aiTask, void* search))0x00402b50)

// DATA: Point2I**
// <LegoRR.exe @00402ba0>
#define AITask_Callback_FUN_00402ba0 ((bool32 (__cdecl* )(AITask* aiTask, void* search))0x00402ba0)

// DATA: Point2I**
// <LegoRR.exe @00402bd0>
#define AITask_Callback_FUN_00402bd0 ((bool32 (__cdecl* )(AITask* aiTask, void* search))0x00402bd0)

// <LegoRR.exe @00402c00>
#define AITask_Release_2 ((void (__cdecl* )(AITask* aiTask))0x00402c00)

// <LegoRR.exe @00402c60>
#define AITask_Callback_IsSameTaskAsField58 ((bool32 (__cdecl* )(AITask* aiTask, AITask* otherTask))0x00402c60)

// <LegoRR.exe @00402c80>
#define AITask_DoDynamite_AtBlockPos ((bool32 (__cdecl* )(const Point2I* blockPos, bool32 noRoutingUnk))0x00402c80)

// <LegoRR.exe @00402d70>
#define AITask_DoBirdScarer_AtPosition ((void (__cdecl* )(const Point2I* blockPos))0x00402d70)

// <LegoRR.exe @00402df0>
#define AITask_DoFindLoad ((void (__cdecl* )(LegoObject* targetObj))0x00402df0)

// <LegoRR.exe @00402e60>
#define AITask_DoRepair_Target ((void (__cdecl* )(LegoObject* targetObj, bool32 condition))0x00402e60)

// <LegoRR.exe @00402ea0>
#define AITask_DoGotoEat_Target ((void (__cdecl* )(LegoObject* liveObj, LegoObject* targetObj))0x00402ea0)

// <LegoRR.exe @00402ee0>
#define AITask_DoElecFence ((void (__cdecl* )(const Point2I* blockPos))0x00402ee0)

// <LegoRR.exe @00402f60>
#define AITask_DoReinforce_AtBlockPos ((void (__cdecl* )(const Point2I* blockPos))0x00402f60)

// <LegoRR.exe @00402fe0>
#define AITask_DoClear_AtPosition ((void (__cdecl* )(const Point2I* blockPos, Message_Type completeAction))0x00402fe0)

// <LegoRR.exe @00403010>
#define AITask_DoGetTool_FromTask ((void (__cdecl* )(AITask* aiTask))0x00403010)

// <LegoRR.exe @004030c0>
#define AITask_DoGetTool ((bool32 (__cdecl* )(LegoObject_ToolType toolType))0x004030c0)

// <LegoRR.exe @00403110>
#define AITask_Game_SelectedUnits_UnkEquippedTool_FUN_00403110 ((bool32 (__cdecl* )(LegoObject_ToolType toolType))0x00403110)

// <LegoRR.exe @004031a0>
#define AITask_Game_PTL_UserGoto ((void (__cdecl* )(LegoObject** liveObjsTable, sint32 count, const Point2I* blockPos, bool32 param_4))0x004031a0)

// <LegoRR.exe @00403250>
#define AITask_DoUpgrade ((void (__cdecl* )(LegoObject* liveObj, sint32 newObjLevel))0x00403250)

// <LegoRR.exe @004032f0>
#define AITask_DoDock ((void (__cdecl* )(LegoObject* liveObj))0x004032f0)

// <LegoRR.exe @00403320>
#define AITask_DoGoto_Congregate ((void (__cdecl* )(const Point2I* blockPos))0x00403320)

// <LegoRR.exe @00403360>
#define AITask_StopGoto_Congregate ((void (__cdecl* )(const Point2I* blockPos))0x00403360)

// <LegoRR.exe @004033d0>
#define AITask_DoAttackObject ((void (__cdecl* )(LegoObject* liveObj))0x004033d0)

// <LegoRR.exe @00403410>
#define AITask_DoAttackPath ((void (__cdecl* )(Point2I* blockPos))0x00403410)

// <LegoRR.exe @00403450>
#define AITask_DoRepair ((void (__cdecl* )(LegoObject* liveObj))0x00403450)

// <LegoRR.exe @00403490>
#define AITask_LiveObject_FUN_00403490 ((void (__cdecl* )(LegoObject* liveObj))0x00403490)

// <LegoRR.exe @004034f0>
#define AITask_Game_PTL_GotoOrRMGoto ((void (__cdecl* )(LegoObject* liveObj, const Point2I* blockPos, undefined4 param_3))0x004034f0)

// <LegoRR.exe @00403540>
#define AITask_Game_PTL_CrystalToRefinery ((void (__cdecl* )(LegoObject* liveObj, LegoObject* targetObj))0x00403540)

// <LegoRR.exe @00403580>
#define AITask_DoFollow_Group ((void (__cdecl* )(LegoObject** liveObjsTable, sint32 count, LegoObject* targetObj))0x00403580)

// <LegoRR.exe @004035f0>
#define AITask_Game_PTL_FollowAttack ((void (__cdecl* )(LegoObject* liveObj, LegoObject* targetObj))0x004035f0)

// Sets flags 0x20 for all aiTasks with the same constructionHandle and meeting object requirements (or checkObjType is false).
// <LegoRR.exe @00403630>
#define AITask_Construction_FUN_00403630 ((void (__cdecl* )(uint32 constructHandle, bool32 checkObjType, LegoObject_Type objType, LegoObject_ID objID))0x00403630)

// <LegoRR.exe @00403680>
#define AITask_DoRequest_ObjectType ((void (__cdecl* )(LegoObject_Type objType, LegoObject_ID objID, sint32 objLevel, const Point2I* blockPos, undefined4 param_5, Point2F* pointFloat, bool32 param_7))0x00403680)

// <LegoRR.exe @00403740>
#define AITask_PlaceRequestObject ((void (__cdecl* )(const Point2I* blockPos, LegoObject_Type objType, LegoObject_ID objID, sint32 objLevel))0x00403740)

// <LegoRR.exe @004037f0>
#define AITask_DoDeposit_ObjectType ((AITask* (__cdecl* )(LegoObject* liveObj, LegoObject_Type objType, LegoObject_ID objID, sint32 objLevel))0x004037f0)

// <LegoRR.exe @00403840>
#define AITask_DoRequestObjectCallbacks ((void (__cdecl* )(LegoObject* liveObj))0x00403840)

// <LegoRR.exe @004038d0>
#define AITask_LiveObject_FUN_004038d0 ((void (__cdecl* )(LegoObject* liveObj))0x004038d0)

// <LegoRR.exe @00403900>
#define AITask_DoGotoEat ((void (__cdecl* )(LegoObject* liveObj))0x00403900)

// <LegoRR.exe @00403940>
#define AITask_LiveObject_FUN_00403940 ((bool32 (__cdecl* )(LegoObject* liveObj))0x00403940)

// <LegoRR.exe @00403960>
#define AITask_Callback_FUN_00403960 ((bool32 (__cdecl* )(AITask* aiTask, LegoObject* liveObj))0x00403960)

// <LegoRR.exe @00403980>
#define AITask_LiveObject_FUN_00403980 ((void (__cdecl* )(LegoObject* liveObj))0x00403980)

// <LegoRR.exe @00403a00>
#define AITask_Callback_FUN_00403a00 ((bool32 (__cdecl* )(AITask* aiTask, LegoObject* liveObj))0x00403a00)

// <LegoRR.exe @00403a20>
#define AITask_LiveObject_FUN_00403a20 ((void (__cdecl* )(LegoObject* liveObj, bool32 param_2))0x00403a20)

// <LegoRR.exe @00403a70>
#define AITask_LiveObject_FUN_00403a70 ((void (__cdecl* )(LegoObject* liveObj))0x00403a70)

// <LegoRR.exe @00403a90>
#define AITask_VariousGatherTasks_FUN_00403a90 ((void (__cdecl* )(LegoObject* liveObj))0x00403a90)

// <LegoRR.exe @00403b30>
#define AITask_LiveObject_FUN_00403b30 ((void (__cdecl* )(LegoObject* holderObj, AITask_Type taskType, LegoObject* carriedObj))0x00403b30)

// <LegoRR.exe @00403c40>
#define AITask_LiveObject_SetAITaskUnk ((void (__cdecl* )(LegoObject* liveObj1, AITask_Type taskType, LegoObject* liveObj2, bool32 param_4))0x00403c40)

// <LegoRR.exe @00403e20>
#define AITask_LiveObject_FUN_00403e20 ((bool32 (__cdecl* )(LegoObject* liveObj))0x00403e20)

// <LegoRR.exe @00403e40>
#define AITask_Callback_FUN_00403e40 ((bool32 (__cdecl* )(AITask* aiTask, LegoObject* liveObj))0x00403e40)

// <LegoRR.exe @00403e60>
#define AITask_DoAnimationWait ((void (__cdecl* )(LegoObject* liveObj))0x00403e60)

// <LegoRR.exe @00403e90>
#define AITask_LiveObject_Unk_UpdateAITask_AnimationWait ((void (__cdecl* )(LegoObject* liveObj))0x00403e90)

// DATA: AITask**
// <LegoRR.exe @00403eb0>
#define AITask_QSortCompare ((sint32 (__cdecl* )(const void* task_a, const void* task_b))0x00403eb0)

// <LegoRR.exe @00403f60>
#define AITask_InitTask_1 ((AITask* (__cdecl* )(AITask* aiTask, AI_Priority priorityType))0x00403f60)

// <LegoRR.exe @00403fa0>
#define AITask_Init_2_NoPriority ((void (__cdecl* )(AITask* aiTask))0x00403fa0)

// <LegoRR.exe @00403fd0>
#define AITask_Game_UnkLiveObjectHandleDynamite ((void (__cdecl* )(LegoObject* liveObj))0x00403fd0)

// <LegoRR.exe @00404030>
#define AITask_LiveObject_FUN_00404030 ((void (__cdecl* )(LegoObject* liveObj))0x00404030)

// <LegoRR.exe @004040c0>
#define AITask_LiveObject_AITask2F0_GetLast ((AITask* (__cdecl* )(LegoObject* liveObj))0x004040c0)

// <LegoRR.exe @004040e0>
#define AITask_LiveObject_IsCurrentTaskType ((bool32 (__cdecl* )(LegoObject* liveObj, AITask_Type taskType))0x004040e0)

// <LegoRR.exe @00404110>
#define AITask_LiveObject_FUN_00404110 ((AITask* (__cdecl* )(LegoObject* liveObj))0x00404110)

// <LegoRR.exe @00404180>
#define AITask_Callback_FUN_00404180 ((bool32 (__cdecl* )(AITask* aiTask, real32* param_2))0x00404180)

// <LegoRR.exe @004041b0>
#define AITask_LiveObject_Callback_DoAITask ((bool32 (__cdecl* )(LegoObject* liveObj, real32* pElapsedGame))0x004041b0)

// <LegoRR.exe @00404d30>
#define AITask_LiveObject_FUN_00404d30 ((bool32 (__cdecl* )(LegoObject* liveObj, const Point2I* blockPos2, real32* param_3))0x00404d30)

// <LegoRR.exe @00404e00>
#define AITask_LiveObject_FUN_00404e00 ((bool32 (__cdecl* )(LegoObject* liveObj))0x00404e00)

// <LegoRR.exe @00404e40>
#define AITask_FUN_00404e40 ((bool32 (__cdecl* )(LegoObject_Type objType, LegoObject_ID objID, sint32 objLevel))0x00404e40)

// <LegoRR.exe @00404e80>
#define AITask_Callback_FUN_00404e80 ((bool32 (__cdecl* )(AITask* aiTask, sint32* param_2))0x00404e80)

// <LegoRR.exe @00404ef0>
#define AITask_FUN_00404ef0 ((bool32 (__cdecl* )(AITask* aiTask, LegoObject* liveObj_2, real32* param_3, real32* param_4, undefined4* param_5, sint32 param_6, sint32 param_7))0x00404ef0)

// <LegoRR.exe @00405880>
#define AITask_FUN_00405880 ((void (__cdecl* )(void))0x00405880)

// <LegoRR.exe @00405b40>
#define AITask_FUN_00405b40 ((void (__cdecl* )(void))0x00405b40)

// <LegoRR.exe @00406290>
#define AITask_FUN_00406290 ((void (__cdecl* )(AITask* aiTask1, AITask* aiTask2, LegoObject* liveObj))0x00406290)

// <LegoRR.exe @00406310>
#define AITask_LiveObject_ReleaseTaskReferences ((void (__cdecl* )(LegoObject* liveObj))0x00406310)

// <LegoRR.exe @00406330>
#define AITask_FUN_00406330 ((AITask* (__cdecl* )(AITask* aiTask))0x00406330)

// <LegoRR.exe @00406370>
#define AITask_Create ((AITask* (__cdecl* )(AITask_Type taskType))0x00406370)

// <LegoRR.exe @004063b0>
#define AITask_Remove ((void (__cdecl* )(AITask* aiTask, bool32 noRelease2))0x004063b0)

// <LegoRR.exe @004063f0>
#define AITask_AddList ((void (__cdecl* )(void))0x004063f0)

// <LegoRR.exe @00406470>
#define AITask_RunThroughLists ((bool32 (__cdecl* )(AITask_RunThroughListsCallback callback, void* data))0x00406470)


#pragma endregion

}
