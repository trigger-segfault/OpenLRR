// AITask.cpp : 
//

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

 // <LegoRR.exe @00406470>
bool32 __cdecl LegoRR::AITask_RunThroughLists(AITask_RunThroughListsCallback callback, void* data)
{
#if 1
	for (AITask* aiTask : aiListSet.EnumerateAlive()) {

		if (callback(aiTask, data))
			return true; // terminate run through listSet
	}
	return false;
#else
	for (uint32 list = 0; list < aiGlobs.listCount; list++) {

		if (aiGlobs.listSet[list]) {
			uint32 count = 0x00000001 << list;

			for (uint32 loop = 0; loop < count; loop++) {
				// LegoRR implementation performed null check on every task, but that
				// would have no effect since we're already null-checking the current list.
				AITask* aiTask = &aiGlobs.listSet[list][loop];

				// Is this a valid unremoved task?
				if (aiTask->nextFree == aiTask) {
					if (callback(aiTask, data))
						return true; // terminate run through listSet
				}
			}
		}
	}
	return false;
#endif
}

#pragma endregion
