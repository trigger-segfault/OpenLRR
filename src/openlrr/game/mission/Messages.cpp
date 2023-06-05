// Messages.cpp : 
//

#include "../../engine/core/Errors.h"
#include "../../engine/core/Maths.h"
#include "../../engine/core/Memory.h"
#include "../../engine/core/Utils.h"
#include "../../engine/input/Input.h"

#include "../audio/SFX.h"
#include "../interface/Interface.h"
#include "../object/AITask.h"
#include "../object/Object.h"
#include "../object/Stats.h"
#include "../world/Construction.h"
#include "../world/ElectricFence.h"
#include "../Game.h"
#include "../Shortcuts.hpp"

#include "PTL.h"
#include "Messages.h"


using Shortcuts::ShortcutID;


/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @004ebf20>
LegoRR::Message_Globs & LegoRR::messageGlobs = *(LegoRR::Message_Globs*)0x004ebf20;

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

using namespace Gods98;


// <LegoRR.exe @00451f90>
void __cdecl LegoRR::Message_Initialise(void)
{
    Message_RegisterName(Message_Null);
    Message_RegisterName(Message_Select);
    Message_RegisterName(Message_Selected);
    Message_RegisterName(Message_ClearSelection);
    Message_RegisterName(Message_Deselect);
    Message_RegisterName(Message_Goto);
    Message_RegisterName(Message_RockMonsterGoto);
    Message_RegisterName(Message_RockMonsterGotoComplete);
    Message_RegisterName(Message_UserGoto);
    Message_RegisterName(Message_FirstPerson);
    Message_RegisterName(Message_TrackObject);
    Message_RegisterName(Message_TopView);
    Message_RegisterName(Message_PlaySample);
    Message_RegisterName(Message_Dig);
    Message_RegisterName(Message_DigComplete);
    Message_RegisterName(Message_Repair);
    Message_RegisterName(Message_RepairComplete);
    Message_RegisterName(Message_Reinforce);
    Message_RegisterName(Message_ReinforceComplete);
    Message_RegisterName(Message_RockFall);
    Message_RegisterName(Message_RockFallComplete);
    Message_RegisterName(Message_GenerateCrystal);
    Message_RegisterName(Message_GenerateCrystalComplete);
    Message_RegisterName(Message_CollectCrystal);
    Message_RegisterName(Message_CollectCrystalComplete);
    Message_RegisterName(Message_CrystalToRefinery);
    Message_RegisterName(Message_CrystalToRefineryComplete);
    Message_RegisterName(Message_GenerateOre);
    Message_RegisterName(Message_GenerateOreComplete);
    Message_RegisterName(Message_CollectOre);
    Message_RegisterName(Message_CollectOreComplete);
    Message_RegisterName(Message_GenerateRockMonster);
    Message_RegisterName(Message_GenerateRockMonsterComplete);
    Message_RegisterName(Message_GatherRock);
    Message_RegisterName(Message_GatherRockComplete);
    Message_RegisterName(Message_PickRandomFloor);
    Message_RegisterName(Message_PickRandomFloorComplete);
    Message_RegisterName(Message_AttackBuilding);
    Message_RegisterName(Message_AttackBuildingComplete);
    Message_RegisterName(Message_Clear);
    Message_RegisterName(Message_ClearComplete);
    Message_RegisterName(Message_GetIn);
    Message_RegisterName(Message_ManSelectedAndMonsterClicked);
    Message_RegisterName(Message_FollowAttack);
    Message_RegisterName(Message_CollectTool);
    Message_RegisterName(Message_ReduceSelection);
    Message_RegisterName(Message_ClearFallIn);
    Message_RegisterName(Message_ClearFallInComplete);
    Message_RegisterName(Message_BuildPath);
    Message_RegisterName(Message_BuildPathComplete);
    Message_RegisterName(Message_Train);
    Message_RegisterName(Message_TrainComplete);
    Message_RegisterName(Message_GenerateCrystalAndOre);
    Message_RegisterName(Message_GenerateCrystalAndOreComplete);
    Message_RegisterName(Message_GenerateFromCryOre);
    Message_RegisterName(Message_GenerateFromCryOreComplete);
    Message_RegisterName(Message_Upgrade);
    Message_RegisterName(Message_UpgradeComplete);
    Message_RegisterName(Message_ClearBuilding);
    Message_RegisterName(Message_ClearBuildingComplete);
    Message_RegisterName(Message_ClearInitial);
    Message_RegisterName(Message_ClearInitialComplete);
    Message_RegisterName(Message_ClearRemovePath);
    Message_RegisterName(Message_ClearRemovePathComplete);
    Message_RegisterName(Message_Debug_DestroyAll);
}

// <LegoRR.exe @00452220>
void __cdecl LegoRR::Message_RemoveEventsWithObject(LegoObject* deadObj)
{
	/// FIXME: Should we also be checking events at (1 - ab)???
    const bool32 ab = messageGlobs.eventAB;
    uint32 count = messageGlobs.eventCounts[ab];
    for (uint32 i = 0; i < count; i++) {
		/// FIXME: We should also be checking message types that use argument2 as an object!!!
        if (deadObj == messageGlobs.eventLists[ab][i].argumentObj) {

			// Replace this removed message with the message at the end of the list.
            messageGlobs.eventLists[ab][i] = messageGlobs.eventLists[ab][--count];
            messageGlobs.eventCounts[ab] = count;
        }
    }
}

// <LegoRR.exe @00452290>
LegoRR::Message_Type __cdecl LegoRR::Message_ParsePTLName(const char* ptlName)
{
    for (uint32 i = 0; i < MESSAGE_MAXTYPES; i++) {
        // Skip "Message_" in name
        if (::_stricmp(messageGlobs.messageName[i] + (sizeof(MESSAGE_NAMEPREFIX)-1), ptlName) == 0) {
            return (Message_Type)i;
        }
    }

    /// TODO: Actually return an invalid result on failure
    Error_WarnF(true, "Invalid message name in PTL: %s", ptlName);
    return Message_Type::Message_Null; // safest possible return
    //return (Message_Type)-1;
}

// <LegoRR.exe @004522d0>
void __cdecl LegoRR::Message_RegisterHotKeyEvent(Gods98::Keys key, Message_Type messageType, LegoObject* argument1Obj, Message_Argument argument2, OPTIONAL const Point2I* blockPos)
{
	const uint32 index = messageGlobs.hotKeyCount;
	/// SANITY: Bounds check hotKeyKeyList.
	if (index < MESSAGE_MAXHOTKEYS) {
		messageGlobs.hotKeyCount++;
		messageGlobs.hotKeyKeyList[index] = key;
		Message_Event* message = &messageGlobs.hotKeyEventList[index];

		std::memset(message, 0, sizeof(Message_Event));

		message->type = messageType;
		message->argumentObj = argument1Obj;
		message->argument2 = argument2;
		if (blockPos) {
			message->blockPos = *blockPos;
		}
		//else {
		//	message->blockPos = Point2I { 0, 0 };
		//}
	}
}

// <LegoRR.exe @00452320>
void __cdecl LegoRR::Message_PostEvent(Message_Type messageType, LegoObject* argument1Obj, Message_Argument argument2, OPTIONAL const Point2I* blockPos)
{
	const bool32 ab = messageGlobs.eventAB;
    const uint32 index = messageGlobs.eventCounts[ab];
    if (index < MESSAGE_MAXEVENTS) {
        messageGlobs.eventCounts[ab]++;
		Message_Event* message = &messageGlobs.eventLists[ab][index];

        std::memset(message, 0, sizeof(Message_Event));

		message->type = messageType;
		message->argumentObj = argument1Obj;
		message->argument2 = argument2;
        if (blockPos) {
			message->blockPos = *blockPos;
        }
		//else {
		//	message->blockPos = Point2I { 0, 0 };
		//}
    }
}

// <LegoRR.exe @00452390>
void __cdecl LegoRR::Message_Update(void)
{
	if (!(legoGlobs.flags1 & GAME1_FREEZEINTERFACE)) {
		for (uint32 i = 0; i < messageGlobs.hotKeyCount; i++) {
			if (Input_IsKeyDown(messageGlobs.hotKeyKeyList[i])) {
				Message_Event* message = &messageGlobs.hotKeyEventList[i];
				Message_PostEvent(message->type, message->argumentObj, message->argument2, &message->blockPos);
			}
		}
	}

	const bool32 ab = messageGlobs.eventAB;

	for (uint32 i = 0; i < messageGlobs.eventCounts[ab]; i++) {
		PTL_TranslateEvent(&messageGlobs.eventLists[ab][i]);
	}

	// So I kind of understand what this dual-queue is for.
	// When going through our message loop, we want to be able to queue up new messages,
	//  but don't want to dispatch them this update.
	// Because we're clearing the count, it just seems like a lazy method of copying the current message queue and
	//  clearing it (which could be done with one state). But we REALLY should handle object reference removal and
	//  message removal, so having a dual queue is better.

	/// FIXME: We're dealing with messages that could be referencing dead objects (that may be removed in earlier messages).
	///        How the hell are we supposed to handle this!??
	const uint32 count = messageGlobs.eventCounts[ab];
	messageGlobs.eventCounts[ab] = 0;
	messageGlobs.eventCounts[!ab] = 0;
	messageGlobs.eventAB = !ab;

	for (uint32 i = 0; i < count; i++) {
		const Message_Event* message = &messageGlobs.eventLists[ab][i];

		switch (message->type) {
		case Message_Select: //0x1:
			Message_SelectObject(message->argumentObj, message->argument2.boolean);
			break;
		case Message_ClearSelection: //0x3:
			Message_ClearSelectedUnits();
			break;
		case Message_Deselect: //0x4:
			Message_DeselectObject(message->argumentObj);
			break;
		case Message_Goto: //0x5:
		case Message_RockMonsterGoto: //0x6:
			AITask_Game_PTL_GotoOrRMGoto(message->argumentObj, &message->blockPos, message->argument2.aiTask); // aiTask->field_4
			break;
		case Message_UserGoto: //0x8:
			AITask_QueueGotoBlock_Group(messageGlobs.selectedUnitList, messageGlobs.selectedUnitCount, &message->blockPos, message->argument2.boolean);
			break;
		case Message_FirstPerson: //0x9:
			if (Message_EnterFirstPersonView(message->argument2.uinteger)) {
				/// FIX APPLY: Properly open FP interface menu.
				/// TODO: Should we allow disabling this functionality in-case users want to mess with menus?
				Interface_OpenMenu(Interface_Menu_FP, nullptr);
			}
			break;
		case Message_TrackObject: //0xa:
			if (messageGlobs.selectedUnitCount != 0) {
				Lego_TrackObjectInRadar(messageGlobs.selectedUnitList[0]);
			}
			break;
		case Message_TopView: //0xb:
			if (Lego_GetViewMode() != ViewMode_Top) {
				Lego_SetViewMode(ViewMode_Top, nullptr, 0);
				/// FIX APPLY: Properly go back to non-FP interface menu.
				/// TODO: Should we allow disabling this functionality in-case users want to mess with menus?
				// Reduce is a shorthand for getting the right interface menu, while maintaining existing selection.
				Message_ReduceSelectedUnits();
				//Interface_OpenMenu(Interface_Menu_Main, nullptr);
			}
			break;
		case Message_PlaySample: //0xc:
			SFX_Random_PlaySoundNormal(message->argument2.sfxID, false);
			break;
		case Message_RockFall: //0x13:
			Lego_PTL_RockFall(message->blockPos.x, message->blockPos.y,
							  (Direction)message->argument2.wall.direction,
							  message->argument2.wall.isConnection);
			//				  (Direction)LOWORD(message->argument2),
			//				  (bool32)   HIWORD(message->argument2));
			break;
		case Message_GenerateCrystal: //0x15:
			Level_GenerateCrystal(&message->blockPos, 0, nullptr, true);
			break;
		case Message_CollectCrystal: //0x17:
			AITask_DoCollect(message->argumentObj, 0.0f);
			break;
		case Message_CrystalToRefinery: //0x19:
			AITask_QueueDepositInObject(message->argumentObj, message->argument2.object);
			break;
		case Message_GenerateOre: //0x1b:
			Level_GenerateOre(&message->blockPos, 0, nullptr, true);
			break;
		case Message_CollectOre: //0x1d:
			AITask_DoCollect(message->argumentObj, 0.0f);
			break;
		case Message_GenerateRockMonster: //0x1f:
			LegoObject_TryGenerateRMonsterAtRandomBlock();
			break;
		case Message_GatherRock: //0x21:
			LegoObject_PTL_GatherRock(message->argumentObj);
			break;
		case Message_PickRandomFloor: //0x23:
			Message_PickRandomFloorBlock(message->argumentObj);
			break;
		case Message_AttackBuilding: //0x25:
			LegoObject_PTL_AttackBuilding(message->argumentObj, message->argument2.object);
			break;
		case Message_FollowAttack: //0x2b:
			AITask_Game_PTL_FollowAttack(message->argumentObj, message->argument2.object);
			break;
		case Message_ReduceSelection: //0x2d:
			Message_ReduceSelectedUnits();
			break;
		case Message_GenerateCrystalAndOre: //0x34:
			LegoObject_PTL_GenerateCrystalsAndOre(&message->blockPos, 0);
			break;
		case Message_GenerateFromCryOre: //0x36:
			LegoObject_PTL_GenerateFromCryOre(&message->blockPos);
			break;
		case Message_Debug_DestroyAll: //0x40:
			Message_Debug_DestroySelectedUnits();
			break;
		}
	}
}

// <LegoRR.exe @004526f0>
void __cdecl LegoRR::Message_PickRandomFloorBlock(LegoObject* liveObj)
{
	const Map3D* map = Lego_GetMap();
	const uint32 blockWidth  = map->blockWidth;
	const uint32 blockHeight = map->blockHeight;

#if false
	/// NEW: Too long to fail method (unless there's literally no floor blocks).
	uint32 floorCount = 0;
	for (uint32 by = 0; by < blockHeight; by++) {
		for (uint32 bx = 0; bx < blockWidth; bx++) {

			if (Level_Block_IsGround(bx, by))
				floorCount++;
		}
	}

	if (floorCount > 0) {
		uint32 floorIndex = (uint32)Gods98::Maths_Rand() % floorCount;
		for (uint32 by = 0; by < blockHeight; by++) {
			for (uint32 bx = 0; bx < blockWidth; bx++) {

				if (Level_Block_IsGround(bx, by)) {
					if (floorIndex-- == 0) {
						Point2I blockPos = { bx, by };
						Message_PostEvent(Message_PickRandomFloorComplete, liveObj, MESSAGE_ARGUMENT_NONE, &blockPos);
						return;
					}
				}

			}
		}
	}

#else
	/// OLD: Brute force method, with max of 1000 attempts at finding a valid floor block.
	bool success = false;
	Point2I blockPos = { 0 }; // dummy init

	for (uint32 i = 0; i < 1000; i++) {
		blockPos.x = (sint32)Gods98::Maths_Rand() % (sint32)blockWidth;
		blockPos.y = (sint32)Gods98::Maths_Rand() % (sint32)blockHeight;

		if (Level_Block_IsGround(blockPos.x, blockPos.y)) {
			success = true;
			break;
		}
	}
	if (success) {
		Message_PostEvent(Message_PickRandomFloorComplete, liveObj, MESSAGE_ARGUMENT_NONE, &blockPos);
	}

#endif
}

// <LegoRR.exe @00452770>
void __cdecl LegoRR::Message_RemoveObjectReference(LegoObject* deadObj)
{
	Message_RemoveEventsWithObject(deadObj);
	Message_DeselectObject(deadObj);

	// Kick the player out of FP view if needed.
	if (legoGlobs.objectFP == deadObj) {
		Lego_SetViewMode(ViewMode_Top, nullptr, 0);
		legoGlobs.objectFP = nullptr;
	}

	// Stop the radar from tracking this object.
	if (legoGlobs.cameraTrack->trackObj == deadObj) {
		legoGlobs.cameraTrack->trackObj = nullptr;

		// Is this setting a last-known/final position?
		LegoObject_GetPosition(deadObj, &legoGlobs.radarCenter.x, &legoGlobs.radarCenter.y);
		
		legoGlobs.flags1 |= GAME1_RADAR_TRACKOBJECTLOST; // Signal lost flag? (radar feature only found in beta)
	}
}

// Allocates and outputs a copied list of all currently selected units.
// <LegoRR.exe @004527e0>
bool32 __cdecl LegoRR::Message_CopySelectedUnits(OUT LegoObject*** unitsList, OUT uint32* unitsCount)
{
    if (messageGlobs.selectedUnitCount != 0) {
        uint32 arraySize = messageGlobs.selectedUnitCount * sizeof(LegoObject*);
        *unitsCount = messageGlobs.selectedUnitCount;
        *unitsList = (LegoObject**)Gods98::Mem_Alloc(arraySize);
        std::memcpy(*unitsList, messageGlobs.selectedUnitList, arraySize);
        return true;
    }
    return false;
}

// <LegoRR.exe @00452840>
LegoRR::LegoObject** __cdecl LegoRR::Message_GetSelectedUnits2(OPTIONAL OUT uint32* count)
{
    if (count) *count = messageGlobs.selectedUnitCount;
    if (messageGlobs.selectedUnitCount != 0)
        return messageGlobs.selectedUnitList;
    else
        return nullptr;
}

// <LegoRR.exe @00452870>
bool32 __cdecl LegoRR::Message_AnyUnitSelected(void)
{
    return (messageGlobs.selectedUnitCount != 0);
}

// <LegoRR.exe @00452880>
LegoRR::LegoObject* __cdecl LegoRR::Message_GetPrimarySelectedUnit(void)
{
    if (messageGlobs.selectedUnitCount != 0)
        return messageGlobs.selectedUnitList[0];
    else
        return nullptr;
}

// <LegoRR.exe @004528a0>
LegoRR::LegoObject** __cdecl LegoRR::Message_GetSelectedUnits(void)
{
    return messageGlobs.selectedUnitList;
}

// <LegoRR.exe @004528b0>
uint32 __cdecl LegoRR::Message_GetNumSelectedUnits(void)
{
    return messageGlobs.selectedUnitCount;
}

// <LegoRR.exe @004528c0>
void __cdecl LegoRR::Message_CleanupSelectedUnitsCount(void)
{
    messageGlobs.selectedUnitCount = 0;
}

// <LegoRR.exe @004528d0>
bool32 __cdecl LegoRR::Message_IsUnitSelected(LegoObject* liveObj, OPTIONAL OUT uint32* index)
{
	// These two checks are slightly different, the first doesn't care about the selected flag,
	//  the other doesn't care about appearing in the list.
	// During normal circumstances, there shouldn't ever be a discrepancy.
	if (index != nullptr) {
		return Message_FindIndexOfObject(messageGlobs.selectedUnitList, messageGlobs.selectedUnitCount, liveObj, index);
	}
	return (liveObj->flags3 & LIVEOBJ3_SELECTED);
}

// <LegoRR.exe @00452910>
bool32 __cdecl LegoRR::Message_FindIndexOfObject(LegoObject** objsTable, uint32 objsCount, LegoObject* thisObj, OPTIONAL OUT uint32* index)
{
    uint32 i = 0;
    for (uint32 i = 0; i < objsCount; i++) {
        if (thisObj == objsTable[i]) {
            if (index) *index = i;
            return true;
        }
    }
    return false;
}

// <LegoRR.exe @00452950>
bool32 __cdecl LegoRR::Message_IsObjectDoubleSelectable(LegoObject* liveObj)
{
	// The double select flag is set during the clear selection operation. That's why we're using an || operator here.
	return Message_IsUnitSelected(liveObj, nullptr) || (liveObj->flags4 & LIVEOBJ4_DOUBLESELECTREADY);
}

// <LegoRR.exe @00452980>
bool32 __cdecl LegoRR::Message_SelectObject(LegoObject* liveObj, bool32 noDoubleSelect)
{
	return Message_SelectObject2(liveObj, noDoubleSelect, true);
}

// <LegoRR.exe @004529a0>
bool32 __cdecl LegoRR::Message_SelectObject2(LegoObject* liveObj, bool32 noDoubleSelect, bool32 interrupt) // actually shouldFaceCamera
{
	// An object being carried can't be selected.
	if (liveObj->carryingThisObject != nullptr) {
		liveObj = liveObj->carryingThisObject;
	}

	if (Message_IsObjectSelectable(liveObj)) {
		if (!noDoubleSelect) { // allowDoubleSelect
			if (Message_IsObjectDoubleSelectable(liveObj)) {
				LegoObject_TryEnterLaserTrackerMode(liveObj);
				return false; // Units in laser tracker mode are not considered selected.
			}
		}

		if (!Message_IsUnitSelected(liveObj, nullptr) && messageGlobs.selectedUnitCount < LEGO_MAXMULTISELECT) {
			for (LegoObject* obj : objectListSet.EnumerateSkipUpgradeParts()) {
				LegoObject_Callback_ExitLaserTrackerMode(obj, nullptr);
			}
			//LegoObject_RunThroughListsSkipUpgradeParts(LegoObject_Callback_ExitLaserTrackerMode, nullptr);

			messageGlobs.selectedUnitList[messageGlobs.selectedUnitCount] = liveObj;
			messageGlobs.selectedUnitCount++;

			liveObj->flags3 |= LIVEOBJ3_SELECTED;

			if (liveObj->type != LegoObject_Building &&
				!(liveObj->flags1 & (LIVEOBJ1_STORING|LIVEOBJ1_SLIPPING|LIVEOBJ1_RESTING)) &&
				!(liveObj->flags2 & LIVEOBJ2_UNK_100))
			{
				LegoObject_Route_End(liveObj, false);
				AITask_VariousGatherTasks_FUN_00403a90(liveObj);

				if (liveObj->type == LegoObject_MiniFigure && interrupt) {
					Vector3F cameraDir;
					Gods98::Container_GetOrientation(legoGlobs.cameraMain->cont4, nullptr, &cameraDir, nullptr);
					const Point2F faceTowardsDir = Point2F { -cameraDir.x, -cameraDir.y };

					LegoObject_Interrupt(liveObj, false, false);
					LegoObject_FaceTowardsCamera(liveObj, &faceTowardsDir);
				}
			}

			if (liveObj->flags3 & LIVEOBJ3_CANYESSIR) {
				const StatsFlags1 sflags1 = StatsObject_GetStatsFlags1(liveObj);

				if (!(sflags1 & STATS1_CANBEDRIVEN) || liveObj->driveObject != nullptr) {
					Container* cont = LegoObject_GetActivityContainer(liveObj);
					SFX_Random_PlaySound3DOnContainer(cont, SFX_YesSir, false, true, nullptr);
					return true;
				}
			}
		}
	}
	return false;
}

// <LegoRR.exe @00452b30>
bool32 __cdecl LegoRR::Message_IsObjectSelectable(LegoObject* liveObj)
{
	bool canSelect = (liveObj->flags3 & LIVEOBJ3_CANSELECT);
	bool ignoreActive = false;

	if (!canSelect && Lego_IsAllowDebugKeys() && Lego_IsAllowEditMode()) {
		if (Shortcut_IsDown(ShortcutID::Edit_SelectMonstersModifier)) {
			if (liveObj->type == LegoObject_RockMonster || liveObj->type == LegoObject_SpiderWeb) {
				canSelect = true;
			}
		}
		if (Shortcut_IsDown(ShortcutID::Edit_SelectResourcesModifier)) {
			if (liveObj->type == LegoObject_PowerCrystal || liveObj->type == LegoObject_Ore ||
				liveObj->type == LegoObject_Barrier || liveObj->type == LegoObject_Dynamite ||
				liveObj->type == LegoObject_OohScary)
			{
				canSelect = true;
			}
			else if (liveObj->type == LegoObject_ElectricFence && !(liveObj->flags2 & LIVEOBJ2_ACTIVEELECTRICFENCE)) {
				ignoreActive = true;
				canSelect = true;
			}
		}
	}

	if (canSelect) {
		if ((ignoreActive || LegoObject_IsActive(liveObj, true)) &&
			!(liveObj->flags2 & (LIVEOBJ2_THROWN|LIVEOBJ2_DRIVING)) &&
			!(liveObj->flags1 & (LIVEOBJ1_SLIPPING|LIVEOBJ1_RESTING)) &&
			(liveObj->type != LegoObject_Vehicle || !(liveObj->flags1 & LIVEOBJ1_CLEARING))) // Can't select dozer while in the clearing animation?
		{
			return true;
		}
	}
	return false;
}

#if true
// <LegoRR.exe @00452b80>
void __cdecl LegoRR::Message_ReduceSelectedUnits(void)
{
	/// DECOMPILER WARNING: The decompiled code for this function was VERY BROKEN. It's flooded with gotos, and the logic is
	///                     straight-up wrong in same cases (like unmanned vehicles being reduced when there's no driver).

	/// REFACTOR: Switch function from storing bools for every reduce type, to using a priority enum.
	///           So much cleaner now...

	// Disabled for initial implementation commit.
	const bool allowSpecial = (Lego_IsAllowDebugKeys() && Lego_IsAllowEditMode());

	// Default to no reduce type (main interface, unselect all units).
	Message_ReduceType reduceType = Message_ReduceType_Count;

	// Find the highest priority (lowest value) reduce type from the list of selected units.
	for (uint32 i = 0; i < messageGlobs.selectedUnitCount; i++) {
		LegoObject* unit = messageGlobs.selectedUnitList[i];
		const StatsFlags1 sflags1 = StatsObject_GetStatsFlags1(unit);

		if (unit->type == LegoObject_MiniFigure && unit->driveObject == nullptr) {
			reduceType = std::min(reduceType, Message_ReduceType_MiniFigure);
			break;
		}
		else if (sflags1 & STATS1_CANBEDRIVEN) {
			// Drivable vehicles:
			if (unit->driveObject == nullptr) {
				reduceType = std::min(reduceType, Message_ReduceType_UnmannedVehicle);
			}
			else if ((sflags1 & STATS1_CROSSLAND) || (unit->flags4 & LIVEOBJ4_DOCKOCCUPIED)) {
				reduceType = std::min(reduceType, Message_ReduceType_LandVehicle);
			}
			else {
				reduceType = std::min(reduceType, Message_ReduceType_WaterVehicle);
			}
		}
		else {
			if (unit->type == LegoObject_Vehicle) {
				// Self-driving vehicles:
				if ((sflags1 & STATS1_CROSSLAND) || (unit->flags4 & LIVEOBJ4_DOCKOCCUPIED)) {
					reduceType = std::min(reduceType, Message_ReduceType_LandVehicle);
				}
				else {
					reduceType = std::min(reduceType, Message_ReduceType_WaterVehicle);
				}
			}
			else if (unit->type == LegoObject_Building) {
				reduceType = std::min(reduceType, Message_ReduceType_Building);
			}
			else if (unit->type == LegoObject_ElectricFence && (unit->flags2 & LIVEOBJ2_ACTIVEELECTRICFENCE)) {
				reduceType = std::min(reduceType, Message_ReduceType_ElectricFence);
			}
			else if (allowSpecial) {
				// Special reduce types introduced in OpenLRR.
				// Though some of these like Monster must have existed during development.
				if (unit->type == LegoObject_RockMonster) {
					if (Shortcut_IsDown(ShortcutID::Edit_SelectMonstersModifier)) {
						reduceType = std::min(reduceType, Message_ReduceType_Monster);
					}
				}
				// Unknown behaviour, so disabled for now.
				else if (unit->type == LegoObject_SpiderWeb) {
					if (Shortcut_IsDown(ShortcutID::Edit_SelectMonstersModifier)) {
						reduceType = std::min(reduceType, Message_ReduceType_SpiderWeb);
					}
				}
				// Can cause crashes with the AI, so disable this for now.
				else if (unit->type == LegoObject_PowerCrystal || unit->type == LegoObject_Ore) {
					if (Shortcut_IsDown(ShortcutID::Edit_SelectResourcesModifier)) {
						reduceType = std::min(reduceType, Message_ReduceType_Resource);
					}
				}
				// Unknown behaviour, so disabled for now.
				else if (unit->type == LegoObject_Dynamite || unit->type == LegoObject_Barrier || unit->type == LegoObject_OohScary ||
						 (unit->type == LegoObject_ElectricFence && !(unit->flags2 & LIVEOBJ2_ACTIVEELECTRICFENCE)))
				{
					if (Shortcut_IsDown(ShortcutID::Edit_SelectResourcesModifier)) {
						reduceType = std::min(reduceType, Message_ReduceType_Equipment);
					}
				}
			}
		}
	}

	// From all of our possible reduceTypes, pick the reduceType with the highest priority and disable all other reduceTypes.
	// Open the interface menu for the highest-priority selected reduceType.
	switch (reduceType) {
	case Message_ReduceType_MiniFigure:
		Interface_OpenMenu(Interface_Menu_LegoMan, nullptr);
		break;
	case Message_ReduceType_LandVehicle:
		Interface_OpenMenu(Interface_Menu_LandVehicle, nullptr);
		break;
	case Message_ReduceType_WaterVehicle:
		Interface_OpenMenu(Interface_Menu_WaterVehicle, nullptr);
		break;
	case Message_ReduceType_UnmannedVehicle:
		Interface_OpenMenu(Interface_Menu_UnmannedVehicle, nullptr);
		break;
	case Message_ReduceType_Building:
		Interface_OpenMenu(Interface_Menu_Building, nullptr);
		break;
	case Message_ReduceType_ElectricFence:
		Interface_OpenMenu(Interface_Menu_ElectricFence, nullptr);
		break;

	case Message_ReduceType_Monster:
		// Crashes:
		//Interface_OpenMenu(Interface_Menu_LegoMan, nullptr);
		//Interface_OpenMenu(Interface_Menu_Building, nullptr);

		// Works: Even the tele' up button functions (for "standard" monster types).
		Interface_OpenMenu(Interface_Menu_ElectricFence, nullptr); // Just a tele' up button.
		//Interface_OpenMenu(Interface_Menu_Main, nullptr);
		break;
	case Message_ReduceType_SpiderWeb:
		// Unknown:
		Interface_OpenMenu(Interface_Menu_ElectricFence, nullptr); // Just a tele' up button.
		break;
	case Message_ReduceType_Resource:
		// Works: Even the tele' up button functions.
		Interface_OpenMenu(Interface_Menu_ElectricFence, nullptr); // Just a tele' up button.
		break;
	case Message_ReduceType_Equipment:
		// Unknown:
		Interface_OpenMenu(Interface_Menu_ElectricFence, nullptr); // Just a tele' up button.
		break;

	default: // Fallback to main interface if no matching units found.
		//reduceType = Message_ReduceType_Count;
		Interface_OpenMenu(Interface_Menu_Main, nullptr);
		break;
	}


	// Go through the list of selected units, and remove units that are excluded by our reduceType.
	for (uint32 i = 0; i < messageGlobs.selectedUnitCount; i++) {
		LegoObject* unit = messageGlobs.selectedUnitList[i];
		const StatsFlags1 sflags1 = StatsObject_GetStatsFlags1(unit);
		bool reduce = false;

		switch (reduceType) {

		case Message_ReduceType_MiniFigure:
			if (unit->type != LegoObject_MiniFigure || unit->driveObject != nullptr) {
				// 1. This is not a mini-figure.
				// 2. This is a driver (select the vehicle instead).
				reduce = true;
			}
			break;

		case Message_ReduceType_LandVehicle:
			/// FIX APPLY: Self-driving vehicles now properly differentiate between land and water.
			if (!(sflags1 & STATS1_CROSSLAND) && !(unit->flags4 & LIVEOBJ4_DOCKOCCUPIED)) {
				// 1. This is a water vehicle.
				// 2. This is not a docked water vehicle (not treated as land vehicle).
				reduce = true;
			}
			else if (sflags1 & STATS1_CANBEDRIVEN) {
				if (unit->driveObject == nullptr) {
					// 3. This is an unmanned vehicle.
					reduce = true;
				}
			}
			else if (unit->type != LegoObject_Vehicle) {
				// 4. This isn't a self-driving vehicle.
				reduce = true;
			}
			break;

		case Message_ReduceType_WaterVehicle:
			/// FIX APPLY: Self-driving vehicles now properly differentiate between land and water.
			if ((sflags1 & STATS1_CROSSLAND) || (unit->flags4 & LIVEOBJ4_DOCKOCCUPIED)) {
				// 1. This is a land vehicle.
				// 2. This is a docked water vehicle (treated as land vehicle).
				reduce = true;
			}
			else if (sflags1 & STATS1_CANBEDRIVEN) {
				if (unit->driveObject == nullptr) {
					// 3. This is an unmanned vehicle.
					reduce = true;
				}
			}
			else if (unit->type != LegoObject_Vehicle) {
				// 4. This isn't a self-driving vehicle.
				reduce = true;
			}
			break;

		case Message_ReduceType_UnmannedVehicle:
			/// DECOMPILER FIX: THE DECOMPILER LOGIC HERE *WAS* WRONG!!!
			///                 A vehicle *was* considered unmanned if it was drivable WITH A DRIVER (not without).
			if (!(sflags1 & STATS1_CANBEDRIVEN) || unit->driveObject != nullptr) {
				// 1. This is a self-driving vehicle (or not a vehicle at all).
				// 2. This is not an unmanned vehicle.
				reduce = true;
			}
			break;

		case Message_ReduceType_Building:
			if (unit->type != LegoObject_Building) {
				reduce = true;
			}
			break;

		case Message_ReduceType_ElectricFence:
			if (unit->type != LegoObject_ElectricFence || !(unit->flags2 & LIVEOBJ2_ACTIVEELECTRICFENCE)) {
				// 1. This is not an electric fence.
				// 2. This electric fence is not active (not placed).
				reduce = true;
			}
			break;

		case Message_ReduceType_Monster:
			if (unit->type != LegoObject_RockMonster) {
				reduce = true;
			}
			break;

		case Message_ReduceType_SpiderWeb:
			if (unit->type != LegoObject_SpiderWeb) {
				reduce = true;
			}
			break;

		case Message_ReduceType_Resource:
			if (unit->type != LegoObject_PowerCrystal && unit->type != LegoObject_Ore) {
				reduce = true;
			}
			break;

		case Message_ReduceType_Equipment:
			if (unit->type == LegoObject_ElectricFence && (unit->flags2 & LIVEOBJ2_ACTIVEELECTRICFENCE)) {
				// 1. This electric fence is active (already placed).
				reduce = true;
			}
			else if (unit->type != LegoObject_Dynamite && unit->type != LegoObject_Barrier && unit->type != LegoObject_OohScary) {
				// 2. Not a valid other equipment type (dynamite, construction barrier, or sonic blaster (carryable)).
				reduce = true;
			}
			break;

		default: // Main interface, all selected objects are reduced.
			reduce = true;
			break;
		}


		if (reduce) { // hasMain check is redundant here, but wasn't in original decompiled function.

			// Replace this reduced object with the object at the end of the list.
			messageGlobs.selectedUnitList[i] = messageGlobs.selectedUnitList[messageGlobs.selectedUnitCount - 1];
			messageGlobs.selectedUnitCount--;
			i--;

			unit->flags3 &= ~LIVEOBJ3_SELECTED;
			/// FIXME: Should we also remove LIVEOBJ4_LASERTRACKERMODE, like with ClearSelection?
			//unit->flags4 &= ~LIVEOBJ4_LASERTRACKERMODE;
		}
	}
}
#endif

#if true
// <LegoRR.exe @00452ea0>
void __cdecl LegoRR::Message_ClearSelectedUnits(void)
{
	for (uint32 i = 0; i < messageGlobs.selectedUnitCount; i++) {
		LegoObject* unit = messageGlobs.selectedUnitList[i];
		if (unit->flags3 & LIVEOBJ3_SELECTED) {
			// I don't know where this flag is cleared (outside of teleporting up).
			// But it must be removed somewhere soon after calling this, since the debug tooltip never shows this flag.
			unit->flags4 |= LIVEOBJ4_DOUBLESELECTREADY; // Next selection can be a double-selection.
		}
		unit->flags3 &= ~LIVEOBJ3_SELECTED;
		unit->flags4 &= ~LIVEOBJ4_LASERTRACKERMODE;
	}
	messageGlobs.selectedUnitCount = 0;
}
#endif

// <LegoRR.exe @00452f10>
bool32 __cdecl LegoRR::Message_DeselectObject(LegoObject* liveObj)
{
	if (messageGlobs.selectedUnitCount != 0) {
		uint32 index;
		if (Message_IsUnitSelected(liveObj, &index)) {
			if (index == 0) {
				// Primary unit has been deselected.
				/// FIXME: Should we be clearing the entire selected list in this scenario?
				Interface_OpenMenu(Interface_Menu_Main, nullptr);
			}

			// Replace this deselected object with the object at the end of the list.
			messageGlobs.selectedUnitList[index] = messageGlobs.selectedUnitList[messageGlobs.selectedUnitCount - 1];
			messageGlobs.selectedUnitCount--;

			liveObj->flags3 &= ~LIVEOBJ3_SELECTED;
			/// FIXME: Should we also remove LIVEOBJ4_LASERTRACKERMODE, like with ClearSelection?

			return true;
		}
	}
	return false;
}

// <LegoRR.exe @00452f80>
uint32 __cdecl LegoRR::Message_Debug_DestroySelectedUnits(void)
{
	// Adding a keybind for this method sounds like fun :)

	// This is a very messy approach of removing all selected objects, how do we know there's
	//  no side effects of removing one object, removing others that may also be selected?
	for (uint32 i = messageGlobs.selectedUnitCount; i > 0; i--) {
		LegoObject* unit = messageGlobs.selectedUnitList[0];
		const StatsFlags1 sflags1 = StatsObject_GetStatsFlags1(unit);

		if ((sflags1 & STATS1_CANBEDRIVEN) && unit->driveObject != nullptr) {
			unit->driveObject->flags2 &= ~LIVEOBJ2_DRIVING;
			unit->driveObject->driveObject = nullptr; // Clear driveObject's reference to us. This looks fishy at first, but it's not.
		}

		if (unit->type == LegoObject_Building) {
			// This will call LegoObject_Remove, which... (see below for the chain...)
			Construction_RemoveBuildingObject(unit);
		}
		else {
			if (unit->type == LegoObject_ElectricFence) {
				// Removes fence grid structure, does not perform object removal.
				ElectricFence_RemoveFence(unit);
			}

			// This will call Message_RemoveObjectReference, which calls Message_DeselectObject,
			//  so index 0 **SHOULD** always be the next object to deselect. This method is disgusting.

			// Alternatively, this *may* be done in the scenario that we WANT to delete all selected objects...
			// It's possible that a side effect could deselect (but not remove) an object, and we'd still want to trash that, right?
			LegoObject_Remove(unit);
		}
	}

	// This probably isn't supposed to return a value, but keep it anyway.
	return messageGlobs.selectedUnitCount;
}

// <LegoRR.exe @00453020>
bool32 __cdecl LegoRR::Message_EnterFirstPersonView(uint32 cameraFrame)
{
	// If already in an FP view (with a valid unit), then change which FP camera we're using.
	if (legoGlobs.objectFP != nullptr) {
		LegoObject* unit = legoGlobs.objectFP;

		if ((unit->flags3 & LIVEOBJ3_CANFIRSTPERSON) && (unit->type == LegoObject_MiniFigure || unit->driveObject != nullptr)) {
			Lego_SetViewMode(ViewMode_FP, unit, cameraFrame);
			return true;
		}
	}

	// Otherwise find the first selected unit that's a valid FP target.
	for (uint32 i = 0; i < messageGlobs.selectedUnitCount; i++) {
		LegoObject* unit = messageGlobs.selectedUnitList[i];

		if (unit->type == LegoObject_MiniFigure && unit->driveObject != nullptr) {
			unit = unit->driveObject;
		}
		if ((unit->flags3 & LIVEOBJ3_CANFIRSTPERSON) && (unit->type == LegoObject_MiniFigure || unit->driveObject != nullptr)) {
			Lego_SetViewMode(ViewMode_FP, unit, cameraFrame);
			return true;
		}
	}

	return false;
}

#pragma endregion
