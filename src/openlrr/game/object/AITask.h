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
	AITASK_FLAG_PERFORMING         = 0x1,
	AITASK_FLAG_UPDATED            = 0x2,
	AITASK_FLAG_VOLATILE           = 0x4,
	AITASK_FLAG_DIGCONNECTION      = 0x8, // (orig: AITASK_FLAG_VERTEXDIG) Related to SingleWidthDig and Vertex mode dig (see: debug key to destroy wall connections).
	AITASK_FLAG_REALLOCATE         = 0x10,
	AITASK_FLAG_REMOVING           = 0x20, // (orig: AITASK_FLAG_ELIMINATE)
	AITASK_FLAG_DUPLICATE          = 0x40,
	AITASK_FLAG_NOTIMELIMIT        = 0x80,
	AITASK_FLAG_IMMEDIATESELECTION = 0x100,
	AITASK_FLAG_CLONED             = 0x200, // (orig: AITASK_FLAG_ISDUPLICATE) When set, unitList will not be freed during `AITask_Remove`. Set by `AITask_Clone`.
	AITASK_FLAG_CARRYTASK          = 0x400,
	AITASK_FLAG_FACEOUT            = 0x800,
	AITASK_FLAG_WAITINGFORTOOL     = 0x1000,
	AITASK_FLAG_CREATUREREALLOCATE = 0x2000, // Only set by `AITask_DoCollect_Target`.
	AITASK_FLAG_FALLINCLEAR        = 0x4000,
	AITASK_FLAG_WAITINGFORTRAIN    = 0x8000,
	AITASK_FLAG_MAXIMUMPRIORITY    = 0x10000,
	AITASK_FLAG_MANUALYSELECTED    = 0x20000,
	AITASK_FLAG_PAUSEDDUPLICATION  = 0x40000,
	AITASK_FLAG_DISABLED           = 0x80000,
	AITASK_FLAG_ACCEPTCARRYING     = 0x100000,
	AITASK_FLAG_UPGRADEBUILDING    = 0x200000,
};
flags_end(AITaskFlags, 0x4);


enum AITask_GlobFlags : uint32 // [LegoRR/AITask.c|flags:0x4|type:uint]
{
	AITASK_GLOB_FLAG_NONE           = 0,
	AITASK_GLOB_FLAG_INITIALISED    = 0x1,
	AITASK_GLOB_FLAG_REMOVING       = 0x2, // Cleanup stage during Level_Free, this is true during LegoObject_CleanupLevel, and Construction_RemoveAll.
	AITASK_GLOB_FLAG_UPDATINGOBJECT = 0x4, // Always Set at the start of `AITask_Callback_UpdateObject`
	                                       // Always unset at the end of `AITask_Callback_UpdateObject`.
										   // (orig: AI_GLOB_FLAG_DISABLEROUTECANCELATION)

	AITASK_GLOB_FLAG_DISABLEUPDATES = 0x80000000, // Disables update behaviour in AITask_UpdateAll.
	                                              // Flag only observed being set in RockFall beta during editor mode.
												  //  (However the flag itself is still checked in Release).
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
	/*04,4*/	AITask* referrerTask; // Unused field, that's only passed along by certain Message_Events.
	/*08,8*/	Point2I blockPos;
	/*10,4*/	LegoObject* targetObject;
	/*14,4*/	real32 time; // Count-down timer.
	/*18,4*/	real32 timeIn; // Count-down timer.
	/*1c,4*/	sint32 priorityValue; // Field is known to be signed (range: 0,99).
	/*20,4*/	LegoObject_Type objType;
	/*24,4*/	LegoObject_ID objID;
	/*28,4*/	uint32 objLevel;
	/*2c,4*/	uint32 constructHandle;
	/*30,8*/	Point2F blockOffset;
	/*38,4*/	LegoObject_ToolType toolType;
	/*3c,4*/	LegoObject_AbilityType trainFlags;
	/*40,4*/	LegoObject** unitList;
	/*44,4*/	uint32 unitListCount;
	/*48,4*/	LegoObject* assignedToObject; // Likely related to tasks with a two-object relation. Like training at a building.
	/*4c,4*/	AI_Priority priorityType;
	/*50,4*/	uint32 creationTime; // Timestamp of task creation in `AITask_Create`, obtained from `Main_GetTime()`.
	/*54,4*/	Message_Type completeAction;
	/*58,4*/	AITask* getToolTask; // (bi-directional link between GetTool and GetTool_FromText)
	/*5c,4*/	AITaskFlags flags;
	/*60,4*/	AITask* next; // Next in linked lists for `aiGlobs.pendingTaskList`, `aiGlobs.creatureTaskList`, and `LegoObject::aiTask`.
	/*64,4*/	AITask* nextFree; // (for listSet)
	/*68*/
};
assert_sizeof(AITask, 0x68);


struct AITask_Globs // [LegoRR/AITask.c|struct:0x4e9c|tags:GLOBS]
{
	/*0000,30*/	AITask* listSet[AITASK_MAXLISTS];
	/*0030,4*/	AITask* freeList;
	/*0034,4*/	uint32 listCount;
	/*0038,7c*/	const char* taskName[AITask_Type_Count];
	/*00b4,6c*/	const char* priorityName[AI_Priority_Count];
	/*0120,6c*/	sint32 priorityValues[AI_Priority_Count];
	/*018c,4*/	AITask* pendingTaskList;
	/*0190,4*/	AITask* creatureTaskList;
	/*0194,c8*/	LegoObject* freeUnitList[50];
	/*025c,4*/	uint32 freeUnitCount;
	/*0260,c8*/	LegoObject* freeCreatureList[50];
	/*0328,4*/	uint32 freeCreatureCount;
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

#define AITask_RegisterName(n)		(aiGlobs.taskName[n]=#n)
#define AIPriority_RegisterName(n)	(aiGlobs.priorityName[n]=#n)

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

/// BETA: RockFall
// <LegoStripped.exe @00402723>
const char* __cdecl AITask_GetTaskName(const AITask* aiTask);

// When turned on, AITasks will not update during `AITask_UpdateAll`. Used for Editor Mode.
/// BETA: RockFall
// <LegoStripped.exe @00402791>
void __cdecl AITask_DisableTaskUpdates(BOOL disabled);

// <LegoRR.exe @00401bf0>
//#define AIPriority_GetType ((bool32 (__cdecl* )(const char* priorityName, OUT AI_Priority* priorityType))0x00401bf0)
bool32 __cdecl AIPriority_GetType(const char* priorityName, OUT AI_Priority* priorityType);

// <LegoRR.exe @00401c30>
//#define AITask_Initialise ((void (__cdecl* )(void))0x00401c30)
void __cdecl AITask_Initialise(void);

// <LegoRR.exe @00401f40>
#define AITask_FUN_00401f40 ((void (__cdecl* )(AITask_Type taskType, LegoObject* liveObj, OPTIONAL const Point2I* blockPos))0x00401f40)

// <LegoRR.exe @00401fa0>
#define AITask_SetPriorityType ((void (__cdecl* )(AITask* aiTask, AI_Priority priorityType))0x00401fa0)

// <LegoRR.exe @00401fd0>
#define AITask_Game_SetPriorityValue ((void (__cdecl* )(AI_Priority priorityType, uint32 aiTaskValue))0x00401fd0)

// <LegoRR.exe @00401fe0>
#define AITask_Game_SetPriorityOff ((void (__cdecl* )(AI_Priority priorityType, bool32 off))0x00401fe0)

// <LegoRR.exe @00401ff0>
#define AITask_Game_IsPriorityOff ((bool32 (__cdecl* )(AI_Priority priorityType))0x00401ff0)

// <LegoRR.exe @00402000>
//#define AITask_Shutdown ((void (__cdecl* )(void))0x00402000)
void __cdecl AITask_Shutdown(void);

// First call in Level_Free is false, which performs most cleanup and sets the cleanup flag.
// Second call in Level_Free is true, which simply unsets this flag.
// <LegoRR.exe @00402040>
#define AITask_CleanupLevel ((void (__cdecl* )(bool32 end))0x00402040)

// <LegoRR.exe @004020b0>
#define AITask_Game_SetNoGather ((void (__cdecl* )(bool32 noGather))0x004020b0)

// <LegoRR.exe @00402120>
#define AITask_Callback_Remove ((bool32 (__cdecl* )(AITask* aiTask))0x00402120)

// <LegoRR.exe @00402150>
#define AITask_UpdateAll ((void (__cdecl* )(real32 elapsedGame))0x00402150)

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
#define AITask_RemoveAttackPathReferences ((bool32 (__cdecl* )(const Point2I* blockPos))0x00402970)

// DATA: Point2I** (technically struct { Point2I*, undefined4 unused }). But the second field is never accessed.
// <LegoRR.exe @004029a0>
#define AITask_Callback_RemoveAttackPathReference ((bool32 (__cdecl* )(AITask* aiTask, void* pBlockPos))0x004029a0)

// Removes all `AttackRockMonster` type tasks whose targetObject references match the specified object.
// <LegoRR.exe @004029d0>
#define AITask_RemoveAttackRockMonsterReferences ((void (__cdecl* )(LegoObject* targetObj))0x004029d0)

// Removes the `AttackRockMonster` type task if its targetObject reference match the specified object.
// DATA: LegoObject* targetObj
// <LegoRR.exe @004029f0>
#define AITask_Callback_RemoveAttackRockMonsterReference ((bool32 (__cdecl* )(AITask* aiTask, void* targetObj))0x004029f0)

// Removes references to task types: AITask_Type_Dig (if digConnection matches the task's flags), AITask_Type_Dynamite, AITask_Type_Reinforce
// Marks the block as "not busy" for matched tasks.
// Clears block highlight (always, regardless of match).
// <LegoRR.exe @00402a10>
#define AITask_RemoveWallReferences ((void (__cdecl* )(const Point2I* blockPos, bool32 digConnection))0x00402a10)

// Removes references to task types: AITask_Type_Reinforce
// Clears block highlight (if matched).
// <LegoRR.exe @00402a60>
#define AITask_RemoveReinforceReferences ((bool32 (__cdecl* )(const Point2I* blockPos))0x00402a60)

// Matches references to task types: AITask_Type_Clear
// If delay is true, then matched tasks will have their timer reset to 500.0f (20 seconds)?
// If delay is false, all task references will be removed.
// <LegoRR.exe @00402a90>
#define AITask_DelayOrRemoveClearReferences ((bool32 (__cdecl* )(const Point2I* blockPos, bool32 delay))0x00402a90)

// DATA: SearchAITaskDeselect_8*
// struct SearchAITaskDeselect_8 {
//     const Point2I* blockPos;
//     bool32 digConnection;
// };
// <LegoRR.exe @00402ae0>
#define AITask_Callback_RemoveWallReference ((bool32 (__cdecl* )(AITask* aiTask, void* search))0x00402ae0)

// DATA: Point2I**
// <LegoRR.exe @00402b50>
#define AITask_Callback_RemoveReinforceReference ((bool32 (__cdecl* )(AITask* aiTask, void* search))0x00402b50)

// DATA: Point2I** pBlockPos (really struct with second unused field)
// <LegoRR.exe @00402ba0>
#define AITask_Callback_RemoveClearReference ((bool32 (__cdecl* )(AITask* aiTask, void* pBlockPos))0x00402ba0)

// Resets the float_18 timer on the task to 500.0f (20 seconds)?
// DATA: Point2I** pBlockPos (really struct with second unused field)
// <LegoRR.exe @00402bd0>
#define AITask_Callback_DelayClearReference ((bool32 (__cdecl* )(AITask* aiTask, void* pBlockPos))0x00402bd0)

// <LegoRR.exe @00402c00>
//#define AITask_RemoveGetToolReferences ((void (__cdecl* )(AITask* aiTask))0x00402c00)
void __cdecl AITask_RemoveGetToolReferences(AITask* aiTask);

// Returns true if aiTask->getToolTask matches otherTask. Usage in-game is completely broken and does nothing either way.
// DATA: AITask* otherTask
// <LegoRR.exe @00402c60>
#define AITask_Callback_FindGetToolReference ((bool32 (__cdecl* )(AITask* aiTask, void* otherTask))0x00402c60)

// <LegoRR.exe @00402c80>
#define AITask_DoDynamite_AtBlockPos ((bool32 (__cdecl* )(const Point2I* blockPos, bool32 noRoutingUnk))0x00402c80)

// <LegoRR.exe @00402d70>
#define AITask_DoBirdScarer_AtPosition ((void (__cdecl* )(const Point2I* blockPos))0x00402d70)

// <LegoRR.exe @00402df0>
#define AITask_DoFindLoad ((void (__cdecl* )(LegoObject* targetObj))0x00402df0)

// <LegoRR.exe @00402e60>
#define AITask_DoRepair_Target ((void (__cdecl* )(LegoObject* targetObj, bool32 condition))0x00402e60)

// <LegoRR.exe @00402ea0>
#define AITask_QueueGotoEat_Target ((void (__cdecl* )(LegoObject* liveObj, LegoObject* targetObj))0x00402ea0)

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

// If reevalTasks is true, then `AITask_ReevaluateObjectTasks` is called,
// <LegoRR.exe @004031a0>
#define AITask_QueueGotoBlock_Group ((void (__cdecl* )(LegoObject** unitList, uint32 unitCount, const Point2I* blockPos, bool32 reevalTasks))0x004031a0)

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
#define AITask_StopRepairForObject ((void (__cdecl* )(LegoObject* liveObj))0x00403490)

// <LegoRR.exe @004034f0>
#define AITask_Game_PTL_GotoOrRMGoto ((void (__cdecl* )(LegoObject* liveObj, const Point2I* blockPos, OPTIONAL AITask* referrerTask))0x004034f0)

// <LegoRR.exe @00403540>
#define AITask_QueueDepositInObject ((void (__cdecl* )(LegoObject* liveObj, LegoObject* targetObj))0x00403540)

// <LegoRR.exe @00403580>
#define AITask_PushFollowObject_Group ((void (__cdecl* )(LegoObject** unitList, uint32 unitCount, LegoObject* targetObj))0x00403580)

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

// Removes all tasks whose targetObject references match the specified object.
//  (marks for removal)
// <LegoRR.exe @00403980>
#define AITask_RemoveTargetObjectReferences ((void (__cdecl* )(LegoObject* targetObj))0x00403980)
//void __cdecl AITask_RemoveTargetObjectReferences(LegoObject* targetObj);

// Removes the task if its targetObject reference match the specified object.
//  (marks for removal)
// DATA: LegoObject* targetObj
// <LegoRR.exe @00403a00>
#define AITask_Callback_RemoveTargetObjectReference ((bool32 (__cdecl* )(AITask* aiTask, void* targetObj))0x00403a00)
//bool32 __cdecl AITask_Callback_RemoveTargetObjectReference(AITask* aiTask, void* targetObj);

// Called by `LegoObject_Route_End` when existing routeBlocks were present.
// <LegoRR.exe @00403a20>
#define AITask_Route_End ((void (__cdecl* )(LegoObject* liveObj, bool32 completed))0x00403a20)

// <LegoRR.exe @00403a70>
#define AITask_LiveObject_FUN_00403a70 ((void (__cdecl* )(LegoObject* liveObj))0x00403a70)

// <LegoRR.exe @00403a90>
#define AITask_VariousGatherTasks_FUN_00403a90 ((void (__cdecl* )(LegoObject* liveObj))0x00403a90)

// <LegoRR.exe @00403b30>
#define AITask_LiveObject_FUN_00403b30 ((void (__cdecl* )(LegoObject* holderObj, AITask_Type taskType, LegoObject* carriedObj))0x00403b30)

// <LegoRR.exe @00403c40>
#define AITask_LiveObject_SetAITaskUnk ((void (__cdecl* )(LegoObject* liveObj1, AITask_Type taskType, LegoObject* liveObj2, bool32 param_4))0x00403c40)

// <LegoRR.exe @00403e20>
#define AITask_RemoveCollectReferences ((bool32 (__cdecl* )(LegoObject* targetObj))0x00403e20)

// DATA: LegoObject* targetObj
// <LegoRR.exe @00403e40>
#define AITask_Callback_RemoveCollectReference ((bool32 (__cdecl* )(AITask* aiTask, void* targetObj))0x00403e40)

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

// Seems to clean up existing tasks with certain flags, in the object's task list.
// <LegoRR.exe @00404030>
#define AITask_ReevaluateObjectTasks ((void (__cdecl* )(LegoObject* liveObj))0x00404030)
//void __cdecl AITask_ReevaluateObjectTasks(LegoObject* liveObj);

// Returns the last task in the object's linked list of tasks.
// Or null if the object has no tasks.
// <LegoRR.exe @004040c0>
#define AITask_GetObjectTaskListTail ((AITask* (__cdecl* )(LegoObject* liveObj))0x004040c0)

// <LegoRR.exe @004040e0>
#define AITask_LiveObject_IsCurrentTaskType ((bool32 (__cdecl* )(LegoObject* liveObj, AITask_Type taskType))0x004040e0)

// <LegoRR.exe @00404110>
#define AITask_LiveObject_FUN_00404110 ((void (__cdecl* )(LegoObject* liveObj))0x00404110)
//void __cdecl AITask_LiveObject_FUN_00404110(LegoObject* liveObj);

// Called during AITask_UpdateAll, to update two count-down timers for all tasks.
// DATA: real32* pElapsedGame
// <LegoRR.exe @00404180>
#define AITask_Callback_UpdateTask ((bool32 (__cdecl* )(AITask* aiTask, void* pElapsedGame))0x00404180)

// DATA: real32* pElapsedGame
// <LegoRR.exe @004041b0>
#define AITask_Callback_UpdateObject ((bool32 (__cdecl* )(LegoObject* liveObj, void* pElapsedGame))0x004041b0)

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

// Removes references to assignedToObject. But only from the `aiGlobs.pendingTaskList` linked list.
// <LegoRR.exe @00406310>
#define AITask_RemoveAssignedToObjectReferences ((void (__cdecl* )(LegoObject* assignedToObj))0x00406310)

// <LegoRR.exe @00406330>
//#define AITask_Clone ((AITask* (__cdecl* )(AITask* aiTask))0x00406330)
AITask* __cdecl AITask_Clone(AITask* aiTask);

// <LegoRR.exe @00406370>
//#define AITask_Create ((AITask* (__cdecl* )(AITask_Type taskType))0x00406370)
AITask* __cdecl AITask_Create(AITask_Type taskType);

// levelCleanup is only true when called by `AITask_Callback_Remove`.
// <LegoRR.exe @004063b0>
//#define AITask_Remove ((void (__cdecl* )(AITask* aiTask, bool32 levelCleanup))0x004063b0)
void __cdecl AITask_Remove(AITask* dead, bool32 levelCleanup);

// <LegoRR.exe @004063f0>
//#define AITask_AddList ((void (__cdecl* )(void))0x004063f0)
void __cdecl AITask_AddList(void);

// <LegoRR.exe @00406470>
//#define AITask_RunThroughLists ((bool32 (__cdecl* )(AITask_RunThroughListsCallback callback, void* data))0x00406470)
bool32 __cdecl AITask_RunThroughLists(AITask_RunThroughListsCallback callback, void* data);


#pragma endregion

}
