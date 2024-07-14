// Dependencies.cpp : 
//

#include "Dependencies.h"
#include "../Game.h"
#include "../object/Stats.h"

#include "../../engine/core/Utils.h"


/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @004b9bc8>
LegoRR::Dependencies_Globs & LegoRR::dependencyGlobs = *(LegoRR::Dependencies_Globs*)0x004b9bc8;

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @0040aa40>
void __cdecl LegoRR::Dependencies_SetEnabled(bool32 on)
{
	dependencyGlobs.disabled = (on == 0);
}

// <LegoRR.exe @0040aa60>
void __cdecl LegoRR::Dependencies_Reset_ClearAllLevelFlags_10c(void)
{
	for (uint32 i = 0; i < LegoObject_Type_Count; i++) {
		for (uint32 j = 0; j < LegoObject_ID_Count; j++) {
			for (uint32 k = 0; k < OBJECT_MAXLEVELS; k++) {
				dependencyGlobs.table[i][j].levelFlags[k] &= ~(DependencyFlags::DEPENDENCY_FLAG_UNK_4 | DependencyFlags::DEPENDENCY_FLAG_UNK_8 | DependencyFlags::DEPENDENCY_FLAG_UNK_100);
			}
		}
	}
}

// <LegoRR.exe @0040aaa0>
void __cdecl LegoRR::Dependencies_Initialise(const Gods98::Config* config, const char* gameName)
{
	char* targetParts[3];
	char* requireParts[10];
	char* requireObjParts[2];

	for (const Gods98::Config* prop = Gods98::Config_FindArray(config, Config_ID(gameName, "Dependencies")); prop != nullptr; prop = Gods98::Config_GetNextItem(prop)) {
		char* itemName = Gods98::Util_StrCpy(Gods98::Config_GetItemName(prop));
		uint32 numParts = Gods98::Util_Tokenise(itemName, targetParts, ":");

		LegoObject_Type objectType;
		LegoObject_ID objectID;
		if (Lego_GetObjectByName(targetParts[1], &objectType, &objectID, nullptr)) {
			int objLevel = 0;

			int cmp = ::_stricmp(targetParts[0], "HitOnceStay");

			if (numParts == 3) {
				objLevel = std::atoi(targetParts[2]);
				Stats_GetLevels(objectType, objectID);
				if (cmp == 0) {
					dependencyGlobs.table[objectType][objectID].levelFlags[objLevel] |= DependencyFlags::DEPENDENCY_FLAG_HITONCESTAY;
				}

				dependencyGlobs.table[objectType][objectID].manualLevel = true;
			}
			else {
				objLevel = 0;
				if (cmp == 0) {
					for (uint32 i = 0; i < OBJECT_MAXLEVELS; i++) {
						dependencyGlobs.table[objectType][objectID].levelFlags[i] |= DependencyFlags::DEPENDENCY_FLAG_HITONCESTAY;
					}
				}

				dependencyGlobs.table[objectType][objectID].manualLevel = false;
			}

			char* dataString = Gods98::Util_StrCpy(Gods98::Config_GetDataString(prop));
			int numRequirements = Gods98::Util_Tokenise(dataString, requireParts, ",");

			DependencyRequirement* requirements = (DependencyRequirement*)Gods98::Mem_Alloc(numRequirements * sizeof(DependencyRequirement));
			dependencyGlobs.table[objectType][objectID].requirements[objLevel] = requirements;
			dependencyGlobs.table[objectType][objectID].numRequirements[objLevel] = numRequirements;

			if (numRequirements != 0) {
				// The first requirement in the config, is the last in the array
				// Because the first requirement is displaysed on the right of the list
				for (int i = numRequirements - 1, partIndex = 0; i >= 0; i--, partIndex++) {
					int numParts = Gods98::Util_Tokenise(requireParts[partIndex], requireObjParts, ":");

					LegoObject_Type requireObjType;
					LegoObject_ID requireObjID;
					if (Lego_GetObjectByName(requireObjParts[0], &requireObjType, &requireObjID, nullptr)) {
						dependencyGlobs.table[objectType][objectID].requirements[objLevel][i].objType = requireObjType;
						dependencyGlobs.table[objectType][objectID].requirements[objLevel][i].objID = requireObjID;

						if (numParts == 2) {
							int requireObjLevel = std::atoi(requireObjParts[1]);
							Stats_GetLevels(requireObjType, requireObjID);
							dependencyGlobs.table[objectType][objectID].requirements[objLevel][i].objLevel = requireObjLevel;
							dependencyGlobs.table[objectType][objectID].requirements[objLevel][i].hasLevel = true;
						}
						else {
							dependencyGlobs.table[objectType][objectID].requirements[objLevel][i].hasLevel = false;
						}
					}
				}
			}

			Gods98::Mem_Free(dataString);
		}

		Gods98::Mem_Free(itemName);
	}

	Dependencies_Prepare_Unk();
}

// Checks whether all dependencies for an object are currently met
// <LegoRR.exe @0040add0>
bool32 __cdecl LegoRR::Dependencies_Object_FUN_0040add0(LegoObject_Type objType, LegoObject_ID objID, uint32 objLevel)
{
	if (!dependencyGlobs.disabled) {
		DependencyFlags* flags = &dependencyGlobs.table[objType][objID].levelFlags[objLevel];

		if ((*flags & DependencyFlags::DEPENDENCY_FLAG_UNK_2) == 0) {
			DependencyRequirement* requirements;
			uint32 count;
			Dependencies_Object_GetRequirements(objType, objID, objLevel, &requirements, &count);

			if (objType != LegoObject_Type::LegoObject_None) {
				for (uint32 i = 0; i < count; i++) {
					bool hasRequirement = false;
					for (auto obj : objectListSet.EnumerateSkipUpgradeParts()) {
						if (Dependencies_LiveObject_CallbackCheck_FUN_0040ae70(obj, &requirements[i])) {
							hasRequirement = true;
							break;
						}
					}
					//bool hasRequirement = LegoObject_RunThroughListsSkipUpgradeParts(Dependencies_LiveObject_CallbackCheck_FUN_0040ae70, (void*) &requirements[i]);
					if (!hasRequirement) {
						return false;
					}
				}
			}

			if ((*flags & DependencyFlags::DEPENDENCY_FLAG_HITONCESTAY) != DependencyFlags::DEPENDENCY_FLAG_NONE) {
				*flags |= DependencyFlags::DEPENDENCY_FLAG_UNK_2;
			}
		}
	}
	return true;
}

// Checks if a live object satisfies the dependency requirement
// <LegoRR.exe @0040ae70>
bool32 __cdecl LegoRR::Dependencies_LiveObject_CallbackCheck_FUN_0040ae70(LegoObject* liveObj, DependencyRequirement* otherObj)
{
	if (!LegoObject_CanStoredObjectTypeBeSpawned(otherObj->objType)) {
		if (otherObj->objType != liveObj->type) {
			return false;
		}

		if (otherObj->objID != liveObj->id) {
			return false;
		}

		if (otherObj->hasLevel && (liveObj->objLevel < otherObj->objLevel)) {
			return false;
		}
	}
	return true;
}

// <LegoRR.exe @0040aec0>
void __cdecl LegoRR::Dependencies_Object_GetRequirements(LegoObject_Type objType, LegoObject_ID objID, uint32 objLevel, OUT DependencyRequirement** requirements, OUT uint32* count)
{
	if (dependencyGlobs.table[objType][objID].manualLevel) {
		*requirements = dependencyGlobs.table[objType][objID].requirements[objLevel];
		*count = dependencyGlobs.table[objType][objID].numRequirements[objLevel];
	}
	*requirements = dependencyGlobs.table[objType][objID].requirements[0];
	*count = dependencyGlobs.table[objType][objID].numRequirements[0];
}

// <LegoRR.exe @0040af30>
void __cdecl LegoRR::Dependencies_Object_Unlock(LegoObject_Type objType, LegoObject_ID objID, uint32 objLevel, DependencyUnlocks* unlocks)
{
	unlocks->count = 0;

	for (uint32 i = 0; i < LegoObject_Type_Count; i++) {
		for (uint32 j = 0; j < LegoObject_ID_Count; j++) {
			if (!dependencyGlobs.table[i][j].manualLevel) {
				DependencyRequirement* requirements;
				uint32 numRequirements;
				Dependencies_Object_GetRequirements((LegoObject_Type)i, (LegoObject_ID)j, 0, &requirements, &numRequirements);

				for (uint32 reqIndex = 0; reqIndex < numRequirements; reqIndex++) {
					if (requirements[reqIndex].objType == objType && requirements[reqIndex].objID == objID && (requirements[reqIndex].objLevel == 0 || requirements[reqIndex].objLevel == objLevel)) {
						unlocks->objTypes[unlocks->count] = (LegoObject_Type)i;
						unlocks->objIDs[unlocks->count] = (LegoObject_ID)j;
						unlocks->objHasLevels[unlocks->count] = false;
						unlocks->count++;
					}
				}
			}
			else {
				for (uint32 level = 0; level < OBJECT_MAXLEVELS; level++) {
					DependencyRequirement* requirements;
					uint32 numRequirements;
					Dependencies_Object_GetRequirements((LegoObject_Type)i, (LegoObject_ID)j, level, &requirements, &numRequirements);

					for (uint32 reqIndex = 0; reqIndex < numRequirements; reqIndex++) {
						if (requirements[reqIndex].objType == objType && requirements[reqIndex].objID == objID && (requirements[reqIndex].objLevel == 0 || requirements[reqIndex].objLevel == objLevel)) {
							unlocks->objTypes[unlocks->count] = (LegoObject_Type)i;
							unlocks->objIDs[unlocks->count] = (LegoObject_ID)j;
							unlocks->objHasLevels[unlocks->count] = false;
							unlocks->count++;
						}
					}
				}
			}
		}
	}
}

// <LegoRR.exe @0040b0e0>
void __cdecl LegoRR::Dependencies_Prepare_Unk(void)
{
	for (uint32 objType = 0; objType < LegoObject_Type_Count; objType++) {
		for (uint32 objID = 0; objID < LegoObject_ID_Count; objID++) {
			for (uint32 objLevel = 0; objLevel < OBJECT_MAXLEVELS; objLevel++) {
				if (objLevel == 1 && dependencyGlobs.table[objType][objID].manualLevel) {
					break;
				}

				DependencyFlags flags = dependencyGlobs.table[objType][objID].levelFlags[objLevel];
				if ((flags & DependencyFlags::DEPENDENCY_FLAG_UNK_8) == 0) {
					if (!Dependencies_Object_FUN_0040add0((LegoObject_Type)objType, (LegoObject_ID)objID, objLevel)) {
						if ((flags & DependencyFlags::DEPENDENCY_FLAG_UNK_4) != 0) {
							flags &= ~DependencyFlags::DEPENDENCY_FLAG_UNK_4;
							flags |= DependencyFlags::DEPENDENCY_FLAG_UNK_8;
						}
					}
					else {
						if ((flags & DependencyFlags::DEPENDENCY_FLAG_UNK_4) == 0) {
							flags |= DependencyFlags::DEPENDENCY_FLAG_UNK_4;
						}
						else {
							flags &= ~DependencyFlags::DEPENDENCY_FLAG_UNK_4;
							flags |= DependencyFlags::DEPENDENCY_FLAG_UNK_8;
						}
					}

					dependencyGlobs.table[objType][objID].levelFlags[objLevel] = flags;
				}
			}
		}
	}
}

// <LegoRR.exe @0040b180>
bool32 __cdecl LegoRR::Dependencies_Object_IsLevelFlag4(LegoObject_Type objType, LegoObject_ID objID, uint32 objLevel)
{
	if (dependencyGlobs.table[objType][objID].manualLevel) {
		return dependencyGlobs.table[objType][objID].levelFlags[objLevel] & DependencyFlags::DEPENDENCY_FLAG_UNK_4;
	}
	return dependencyGlobs.table[objType][objID].levelFlags[0] & DependencyFlags::DEPENDENCY_FLAG_UNK_4;
}

// <LegoRR.exe @0040b1d0>
void __cdecl LegoRR::Dependencies_Object_AddLevelFlag_100(LegoObject_Type objType, LegoObject_ID objID, uint32 objLevel)
{
	dependencyGlobs.table[objType][objID].levelFlags[objLevel] |= DependencyFlags::DEPENDENCY_FLAG_UNK_100;
}

// <LegoRR.exe @0040b210>
bool32 __cdecl LegoRR::Dependencies_Object_GetLevelFlag_100(LegoObject_Type objType, LegoObject_ID objID, uint32 objLevel)
{
	return dependencyGlobs.table[objType][objID].levelFlags[objLevel] & DependencyFlags::DEPENDENCY_FLAG_UNK_100;
}

#pragma endregion
