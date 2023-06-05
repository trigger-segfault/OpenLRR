// AITask.cpp : 
//

#include "../../engine/Main.h"

#include "Object.h"
#include "AITask.h"


/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @004b41c8>
LegoRR::AITask_Globs & LegoRR::aiGlobs = *(LegoRR::AITask_Globs*)0x004b41c8;

LegoRR::AITask_ListSet LegoRR::aiListSet = LegoRR::AITask_ListSet(LegoRR::aiGlobs);

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

/// BETA: RockFall
// <LegoStripped.exe @00402723>
const char* __cdecl LegoRR::AITask_GetTaskName(const AITask* aiTask)
{
	return aiGlobs.taskName[aiTask->taskType];
}

// When turned on, AITasks will not update during `AITask_UpdateAll`. Used for Editor Mode.
/// BETA: RockFall
// <LegoStripped.exe @00402791>
void __cdecl LegoRR::AITask_DisableTaskUpdates(BOOL disabled)
{
	if (disabled) aiGlobs.flags |= AITASK_GLOB_FLAG_DISABLEUPDATES;
	else          aiGlobs.flags &= ~AITASK_GLOB_FLAG_DISABLEUPDATES;
}


// <LegoRR.exe @00401bf0>
bool32 __cdecl LegoRR::AIPriority_GetType(const char* priorityName, OUT AI_Priority* priorityType)
{
	for (uint32 i = 0; i < AI_Priority_Count; i++) {
		if (::_stricmp(aiGlobs.priorityName[i], priorityName) == 0) {
			*priorityType = (AI_Priority)i;
			return true;
		}
	}
	return false;
}

// <LegoRR.exe @00401c30>
void __cdecl LegoRR::AITask_Initialise(void)
{
	AITask_RegisterName(AITask_Type_Goto);
	AITask_RegisterName(AITask_Type_Follow);
	AITask_RegisterName(AITask_Type_FollowAttack);
	AITask_RegisterName(AITask_Type_Collect);
	AITask_RegisterName(AITask_Type_Gather);
	AITask_RegisterName(AITask_Type_Deposit);
	AITask_RegisterName(AITask_Type_Dump);
	AITask_RegisterName(AITask_Type_Request);
	AITask_RegisterName(AITask_Type_Dig);
	AITask_RegisterName(AITask_Type_Dynamite);
	AITask_RegisterName(AITask_Type_Repair);
	AITask_RegisterName(AITask_Type_Reinforce);
	AITask_RegisterName(AITask_Type_Clear);
	AITask_RegisterName(AITask_Type_Wait);
	AITask_RegisterName(AITask_Type_AnimationWait);
	AITask_RegisterName(AITask_Type_ElecFence);
	AITask_RegisterName(AITask_Type_Eat);
	AITask_RegisterName(AITask_Type_GotoEat);
	AITask_RegisterName(AITask_Type_FindDriver);
	AITask_RegisterName(AITask_Type_GetTool);
	AITask_RegisterName(AITask_Type_BirdScarer);
	AITask_RegisterName(AITask_Type_Upgrade);
	AITask_RegisterName(AITask_Type_BuildPath);
	AITask_RegisterName(AITask_Type_Train);
	AITask_RegisterName(AITask_Type_Depart);
	AITask_RegisterName(AITask_Type_AttackPath);
	AITask_RegisterName(AITask_Type_AttackRockMonster);
	AITask_RegisterName(AITask_Type_Recharge);
	AITask_RegisterName(AITask_Type_Dock);
	AITask_RegisterName(AITask_Type_AttackObject);
	AITask_RegisterName(AITask_Type_FindLoad);

	AIPriority_RegisterName(AI_Priority_Crystal);
	AIPriority_RegisterName(AI_Priority_Ore);
	AIPriority_RegisterName(AI_Priority_DefaultCollect);
	AIPriority_RegisterName(AI_Priority_Destruction);
	AIPriority_RegisterName(AI_Priority_Construction);
	AIPriority_RegisterName(AI_Priority_Request);
	AIPriority_RegisterName(AI_Priority_Reinforce);
	AIPriority_RegisterName(AI_Priority_Repair);
	AIPriority_RegisterName(AI_Priority_Clearing);
	AIPriority_RegisterName(AI_Priority_Storage);
	AIPriority_RegisterName(AI_Priority_Refining);
	AIPriority_RegisterName(AI_Priority_HealthLow);
	AIPriority_RegisterName(AI_Priority_GetIn);
	AIPriority_RegisterName(AI_Priority_Upgrade);
	AIPriority_RegisterName(AI_Priority_BuildPath);
	AIPriority_RegisterName(AI_Priority_AttackRockMonster);
	AIPriority_RegisterName(AI_Priority_Barrier);
	AIPriority_RegisterName(AI_Priority_FindLoad);
	AIPriority_RegisterName(AI_Priority_Recharge);
	AIPriority_RegisterName(AI_Priority_UpgradeBuilding);
	AIPriority_RegisterName(AI_Priority_Gather);
	AIPriority_RegisterName(AI_Priority_Steal);
	AIPriority_RegisterName(AI_Priority_Punch);
	AIPriority_RegisterName(AI_Priority_Depart);
	AIPriority_RegisterName(AI_Priority_AttackPath);
	AIPriority_RegisterName(AI_Priority_AttackObject);
	AIPriority_RegisterName(AI_Priority_Congregate);


	// Assign default priority value (of `49`) to all tasks before assigning individual values.
	for (uint32 i = 0; i < AI_Priority_Count; i++) {
		aiGlobs.priorityValues[i] = 49;
	}

	aiGlobs.priorityValues[AI_Priority_Crystal]           = 55;

	aiGlobs.priorityValues[AI_Priority_Destruction]       = 40;
	aiGlobs.priorityValues[AI_Priority_Construction]      = 60;
	aiGlobs.priorityValues[AI_Priority_Request]           = 20;
	aiGlobs.priorityValues[AI_Priority_Reinforce]         = 70;

	aiGlobs.priorityValues[AI_Priority_Clearing]          = 20;
	aiGlobs.priorityValues[AI_Priority_Storage]           =  5;
	aiGlobs.priorityValues[AI_Priority_Refining]          = 15;

	aiGlobs.priorityValues[AI_Priority_AttackRockMonster] = 56;

	aiGlobs.priorityValues[AI_Priority_UpgradeBuilding]   = 99;
	aiGlobs.priorityValues[AI_Priority_Gather]            = 49;
	aiGlobs.priorityValues[AI_Priority_Steal]             = 90;
	aiGlobs.priorityValues[AI_Priority_Punch]             = 49;
	aiGlobs.priorityValues[AI_Priority_Depart]            =  0;
	aiGlobs.priorityValues[AI_Priority_AttackPath]        = 20;
	aiGlobs.priorityValues[AI_Priority_AttackObject]      = 30;
	aiGlobs.priorityValues[AI_Priority_Congregate]        = 99;


	aiListSet.Initialise();
	aiGlobs.flags = AITASK_GLOB_FLAG_INITIALISED;
}



// <LegoRR.exe @00402000>
void __cdecl LegoRR::AITask_Shutdown(void)
{
	aiListSet.Shutdown();
	aiGlobs.flags = AITASK_GLOB_FLAG_NONE;
}



// <LegoRR.exe @00402c00>
void __cdecl LegoRR::AITask_RemoveGetToolReferences(AITask* aiTask)
{
	if (aiTask->taskType == AITask_Type_GetTool) {
		if (aiTask->getToolTask != nullptr) {
			/// FIXME: This is USELESS USELESS USELESS USELESS! Passing the address of this function's
			///        parameter on the stack when it's trying to compare against field getToolTask!!!

			// Now for the real treat. THIS FUNCTION DOES NOTHING ANYWAY.
			// The callback returns true if the fields match, but outside of that.
			// ABSOLUTELY NOTHING is done with the result.
			AITask_RunThroughLists(AITask_Callback_FindGetToolReference, &aiTask);
		}
	}
	else {
		if (aiTask->getToolTask != nullptr) {

			// I'm going to assume this is a list of child tasks, that aren't managed by themselves or something(?)
			AITask* aiTaskUnk = aiGlobs.pendingTaskList; // Linked list for something.
			while (aiTaskUnk != nullptr) {

				if ((aiTaskUnk->taskType == AITask_Type_GetTool) && (aiTask->getToolTask == aiTaskUnk)) {
					aiTaskUnk->flags |= AITASK_FLAG_REMOVING; // Mark for removal.
					// No break after finding a match.... Why??
					// Having a duplicate task in the linked list can *potentially* cause infinite loops.
				}
				aiTaskUnk = aiTaskUnk->next;
			}
		}
	}
}



// <LegoRR.exe @00406330>
LegoRR::AITask* __cdecl LegoRR::AITask_Clone(AITask* aiTask)
{
	AITask* newAITask = AITask_Create(aiTask->taskType);

	ListSet::MemCopy(newAITask, aiTask);

	newAITask->flags |= AITASK_FLAG_CLONED;
	newAITask->next = nullptr;

	return newAITask;
}

// <LegoRR.exe @00406370>
LegoRR::AITask* __cdecl LegoRR::AITask_Create(AITask_Type taskType)
{
	AITask* newAITask = aiListSet.Add();
	ListSet::MemZero(newAITask);

	newAITask->creationTime = Gods98::Main_GetTime();
	newAITask->taskType = taskType;

	return newAITask;
}

// <LegoRR.exe @004063b0>
void __cdecl LegoRR::AITask_Remove(AITask* deadTask, bool32 levelCleanup)
{
	if (!levelCleanup) {
		// Extra removal behaviour that should NOT be called during level cleanup.
		AITask_RemoveGetToolReferences(deadTask);
	}

	// Free all dynamically allocated memory housed in this structure (as long as we're not a clone).
	if (deadTask->unitList != nullptr && !(deadTask->flags & AITASK_FLAG_CLONED)) {
		Gods98::Mem_Free(deadTask->unitList);
	}

	aiListSet.Remove(deadTask);
}

// <LegoRR.exe @004063f0>
void __cdecl LegoRR::AITask_AddList(void)
{
	// NOTE: This function is no longer called, aiListSet.Add already does so.
	aiListSet.AddList();
}

// <LegoRR.exe @00406470>
bool32 __cdecl LegoRR::AITask_RunThroughLists(AITask_RunThroughListsCallback callback, void* data)
{
	for (AITask* task : aiListSet.EnumerateAlive()) {
		if (callback(task, data))
			return true;
	}
	return false;
}

#pragma endregion
