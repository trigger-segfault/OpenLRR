// Object.cpp : 
//

#include "../Game.h"
#include "../audio/SFX.h"
#include "../interface/hud/Bubbles.h"
#include "../interface/Encyclopedia.h"
#include "../interface/InfoMessages.h"
#include "../interface/Messages.h"
#include "AITask.h"
#include "Stats.h"
#include "Object.h"


/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @004df790>
LegoRR::LegoObject_Globs & LegoRR::objectGlobs = *(LegoRR::LegoObject_Globs*)0x004df790;

LegoRR::LegoObject_ListSet LegoRR::objectListSet = LegoRR::LegoObject_ListSet(LegoRR::objectGlobs);

#pragma endregion

/**********************************************************************************
 ******** Class Functions
 **********************************************************************************/

#pragma region Class Functions

bool LegoRR::LegoObject_ListSet::FilterSkipUpgradeParts(const LegoObject* liveObj)
{
	return ListSet::IsAlive(liveObj) && !(liveObj->flags3 & LIVEOBJ3_UPGRADEPART);
}

LegoRR::LegoObject_ListSet::enumerable<LegoRR::LegoObject_ListSet::FilterSkipUpgradeParts> LegoRR::LegoObject_ListSet::EnumerateSkipUpgradeParts()
{
	return this->EnumerateWhere<FilterSkipUpgradeParts>();
}

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @00436ee0>
void __cdecl LegoRR::LegoObject_Initialise(void)
{
	std::memset(&objectGlobs, 0, 0xc644); // Exclude end of struct holding non-glob fields. // sizeof(objectGlobs));

	Activity_RegisterName(Activity_Stand);
	Activity_RegisterName(Activity_Route);
	Activity_RegisterName(Activity_RouteRubble);
	Activity_RegisterName(Activity_RunPanic);
	Activity_RegisterName(Activity_Drill);
	Activity_RegisterName(Activity_Teleport);
	Activity_RegisterName(Activity_Walk);
	Activity_RegisterName(Activity_Reinforce);
	Activity_RegisterName(Activity_Reverse);
	Activity_RegisterName(Activity_TurnLeft);
	Activity_RegisterName(Activity_TurnRight);
	Activity_RegisterName(Activity_CantDo);
	Activity_RegisterName(Activity_Emerge);
	Activity_RegisterName(Activity_Enter);
	Activity_RegisterName(Activity_EnterRein);
	Activity_RegisterName(Activity_Collect);
	Activity_RegisterName(Activity_Gather);
	Activity_RegisterName(Activity_Carry);
	Activity_RegisterName(Activity_CarryRubble);
	Activity_RegisterName(Activity_Throw);
	Activity_RegisterName(Activity_CarryTurnLeft);
	Activity_RegisterName(Activity_CarryTurnRight);
	Activity_RegisterName(Activity_CarryStand);
	Activity_RegisterName(Activity_HitLeft);
	Activity_RegisterName(Activity_HitRight);
	Activity_RegisterName(Activity_HitFront);
	Activity_RegisterName(Activity_HitBack);
	Activity_RegisterName(Activity_HitHard);
	Activity_RegisterName(Activity_Dynamite);
	Activity_RegisterName(Activity_Deposit);
	Activity_RegisterName(Activity_Clear);
	Activity_RegisterName(Activity_Place);
	Activity_RegisterName(Activity_Repair);
	Activity_RegisterName(Activity_Slip);
	Activity_RegisterName(Activity_Rest);
	Activity_RegisterName(Activity_Eat);
	Activity_RegisterName(Activity_Stamp);
	Activity_RegisterName(Activity_ThrowMan);
	Activity_RegisterName(Activity_ThrownByRockMonster);
	Activity_RegisterName(Activity_GetUp);
	Activity_RegisterName(Activity_BuildPath);
	Activity_RegisterName(Activity_Upgrade);
	Activity_RegisterName(Activity_Explode);
	Activity_RegisterName(Activity_Unpowered);
	Activity_RegisterName(Activity_FireLaser);
	Activity_RegisterName(Activity_Freezed);
	Activity_RegisterName(Activity_FreezeStart);
	Activity_RegisterName(Activity_FreezeMelt);
	Activity_RegisterName(Activity_Recharge);
	Activity_RegisterName(Activity_WakeUp);
	Activity_RegisterName(Activity_Train);
	Activity_RegisterName(Activity_FloatOn);
	Activity_RegisterName(Activity_FloatOff);
	Activity_RegisterName(Activity_Opening);
	Activity_RegisterName(Activity_Closing);
	Activity_RegisterName(Activity_Open);
	Activity_RegisterName(Activity_Waiting1);
	Activity_RegisterName(Activity_Waiting2);
	Activity_RegisterName(Activity_Waiting3);
	Activity_RegisterName(Activity_Waiting4);
	Activity_RegisterName(Activity_Waiting5);
	Activity_RegisterName(Activity_Waiting6);
	Activity_RegisterName(Activity_Waiting7);
	Activity_RegisterName(Activity_Waiting8);
	Activity_RegisterName(Activity_Waiting9);
	Activity_RegisterName(Activity_Waiting10);
	Activity_RegisterName(Activity_Waiting11);
	Activity_RegisterName(Activity_Waiting12);
	Activity_RegisterName(Activity_Waiting13);
	Activity_RegisterName(Activity_Waiting14);
	Activity_RegisterName(Activity_Waiting15);
	Activity_RegisterName(Activity_Waiting16);
	Activity_RegisterName(Activity_Waiting17);
	Activity_RegisterName(Activity_Waiting18);
	Activity_RegisterName(Activity_Waiting19);
	Activity_RegisterName(Activity_Waiting20);
	Activity_RegisterName(Activity_Waiting21);
	Activity_RegisterName(Activity_Crumble);
	Activity_RegisterName(Activity_TeleportIn);

	AbilityType_RegisterName(LegoObject_AbilityType_Pilot);
	AbilityType_RegisterName(LegoObject_AbilityType_Sailor);
	AbilityType_RegisterName(LegoObject_AbilityType_Driver);
	AbilityType_RegisterName(LegoObject_AbilityType_Dynamite);
	AbilityType_RegisterName(LegoObject_AbilityType_Repair);
	AbilityType_RegisterName(LegoObject_AbilityType_Scanner);

	objectGlobs.toolNullIndex[LegoObject_ToolType_Drill]      = 0;
	objectGlobs.toolNullIndex[LegoObject_ToolType_Spade]      = 0;
	objectGlobs.toolNullIndex[LegoObject_ToolType_Hammer]     = 1;
	objectGlobs.toolNullIndex[LegoObject_ToolType_Spanner]    = 1;
	objectGlobs.toolNullIndex[LegoObject_ToolType_Laser]      = 4;
	objectGlobs.toolNullIndex[LegoObject_ToolType_PusherGun]  = 4;
	objectGlobs.toolNullIndex[LegoObject_ToolType_BirdScarer] = 4;
	objectGlobs.toolNullIndex[LegoObject_ToolType_FreezerGun] = 4;
	objectGlobs.toolNullIndex[LegoObject_ToolType_Barrier]    = 3;
	objectGlobs.toolNullIndex[LegoObject_ToolType_Dynamite]   = 2;
	objectGlobs.toolNullIndex[LegoObject_ToolType_CryOre]     = 4;

	ToolType_RegisterName(LegoObject_ToolType_Drill);
	ToolType_RegisterName(LegoObject_ToolType_Spade);
	ToolType_RegisterName(LegoObject_ToolType_Hammer);
	ToolType_RegisterName(LegoObject_ToolType_Spanner);
	ToolType_RegisterName(LegoObject_ToolType_Laser);
	ToolType_RegisterName(LegoObject_ToolType_PusherGun);
	ToolType_RegisterName(LegoObject_ToolType_BirdScarer);
	ToolType_RegisterName(LegoObject_ToolType_FreezerGun);

	/// NOTE: Special tools that DO NOT have their names registered.
	//ToolType_RegisterName(LegoObject_ToolType_Barrier);
	//ToolType_RegisterName(LegoObject_ToolType_Dynamite);
	//ToolType_RegisterName(LegoObject_ToolType_CryOre);

	objectListSet.Initialise();
	objectGlobs.flags = LegoObject_GlobFlags::OBJECT_GLOB_FLAG_INITIALISED;
}

// <LegoRR.exe @00437310>
void __cdecl LegoRR::LegoObject_Shutdown(void)
{
	LegoObject_RemoveAll();

	if (objectGlobs.UnkSurfaceGrid_1_TABLE != nullptr) {
		Gods98::Mem_Free(objectGlobs.UnkSurfaceGrid_1_TABLE);
	}

	/// REFACTOR: Moved below freeing `UnkSurfaceGrid_1_TABLE`.
	objectListSet.Shutdown();
	objectGlobs.flags = LegoObject_GlobFlags::OBJECT_GLOB_FLAG_NONE;

	// Yup, this assignment really exists after the above one (originally) next to objectListSet.Shutdown()....
	// It has no effect, so we can keep it commented out.
	//objectGlobs.flags &= ~LegoObject_GlobFlags::OBJECT_GLOB_FLAG_INITIALISED;
}



// <LegoRR.exe @00437800>
bool32 __cdecl LegoRR::LegoObject_Remove(LegoObject* deadObj)
{
	// Decrement number of this object type/id/level present in the level.
	objectGlobs.objectPrevLevels[deadObj->type][deadObj->id][deadObj->objLevel]--;

	// Stop currently-playing sound handles.
	if (deadObj->flags4 & LIVEOBJ4_DRILLSOUNDPLAYING) {
		SFX_Sound3D_StopSound(deadObj->drillSoundHandle);
	}
	if (deadObj->flags4 & LIVEOBJ4_ENGINESOUNDPLAYING) {
		SFX_Sound3D_StopSound(deadObj->engineSoundHandle);
	}

	// Clear all projectileObject fields referencing this object.
	for (LegoObject* obj : objectListSet.EnumerateSkipUpgradeParts()) {
		// Always returns false to enumerate the full list.
		Weapon_Callback_RemoveProjectileReference(obj, deadObj);
	}
	//LegoObject_RunThroughListsSkipUpgradeParts(Weapon_Callback_RemoveProjectileReference, deadObj);

	if (deadObj->type == LegoObject_RockMonster) {
		AITask_RemoveAttackRockMonsterReferences(deadObj);
	}

	if (deadObj->contMiniTeleportUp != nullptr) {
		Gods98::Container_Remove(deadObj->contMiniTeleportUp);
	}

	if (deadObj->carryingThisObject != nullptr) {
		LegoObject_DropCarriedObject(deadObj->carryingThisObject, false);
	}

	// This function has no side effects with deadObj, we can safely reorganize other cleanup around this call.
	LegoObject_WaterVehicle_Unregister(deadObj);

	if (deadObj->flags4 & LIVEOBJ4_LASERTRACKER) {
		legoGlobs.flags1 &= ~GAME1_LASERTRACKER;
	}

	/// TODO: Check if a single routeToObject can be referenced my multiple objects at once.
	///       If that's the case, then is clearing these flags causing problems?
	/// REFACTOR: First block moved from above `LegoObject_WaterVehicle_Unregister`.
	if (deadObj->flags4 & LIVEOBJ4_UNK_40) {
		deadObj->flags4 &= ~LIVEOBJ4_UNK_40;
		if (deadObj->routeToObject != nullptr) {
			deadObj->routeToObject->flags4 &= ~LIVEOBJ4_UNK_40;
		}
	}
	if (deadObj->flags4 & LIVEOBJ4_UNK_80) {
		if (deadObj->routeToObject != nullptr) {
			deadObj->routeToObject->flags4 &= ~LIVEOBJ4_UNK_80;
		}
		deadObj->routeToObject = nullptr;
	}

	LegoObject_Route_End(deadObj, false);
	LegoObject_DoBuildingsCallback_AttackByBoulder(deadObj);
	LegoObject_DropCarriedObject(deadObj, false); // false : don't put away.

	AITask_RemoveTargetObjectReferences(deadObj);

	// Clear first teleportDownObject field referencing this object.
	LegoObject_RemoveTeleportDownReference(deadObj);

	// Remove encyclopedia reference to this object.
	Encyclopedia_RemoveCurrentReference(deadObj);

	if ((StatsObject_GetStatsFlags1(deadObj) & STATS1_PROCESSCRYSTAL) ||
		(StatsObject_GetStatsFlags1(deadObj) & STATS1_PROCESSORE))
	{
		// Probably to remove static task saying: "Hey! if you need processed resources, then come on down to <buildingNameHere>!"
		AITask_LiveObject_FUN_00403940(deadObj);
	}

	// Remove object from selection.
	// Cancel first-person view if this is the objectFP.
	// And clear the camera tracker reference to this object.
	Message_RemoveObjectReference(deadObj);

	// Removes info messages referencing this object.
	Info_RemoveObjectReferences(deadObj);
	Bubble_RemoveObjectReferences(deadObj);

	// Clear routeToObject fields referencing this object.
	LegoObject_RemoveRouteToReferences(deadObj);

	Lego_RemoveRecordObject(deadObj);


	/// FIX APPLY: Remove object references created by K register/get in vehicle debug key.
	if (gamectrlGlobs.dbgGetInVehicle == deadObj) gamectrlGlobs.dbgGetInVehicle = nullptr;
	

	// Remove the model, and other object type-specific cleanup.
	switch (deadObj->type) {
	case LegoObject_Vehicle:
		Vehicle_Remove(deadObj->vehicle);
		Gods98::Mem_Free(deadObj->vehicle);
		break;

	case LegoObject_MiniFigure:
		Creature_Remove(deadObj->miniFigure);
		Gods98::Mem_Free(deadObj->miniFigure);
		break;

	case LegoObject_RockMonster:
		if (StatsObject_GetStatsFlags1(deadObj) & STATS1_FLOCKS) {
			LegoObject_FlocksDestroy(deadObj);
		}
		Creature_Remove(deadObj->rockMonster);
		Gods98::Mem_Free(deadObj->rockMonster);
		break;

	case LegoObject_Building:
		Building_Remove(deadObj->building);
		Gods98::Mem_Free(deadObj->building);
		break;

	case LegoObject_UpgradePart:
		/// REFACTOR: This case was checked after flag `LIVEOBJ3_SIMPLEOBJECT`. But these cases are mutually exclusive.
		Upgrade_Part_Remove(deadObj->upgradePart);
		Gods98::Mem_Free(deadObj->upgradePart);
		break;

	default: // Simple object types:
		if (deadObj->flags3 & LIVEOBJ3_SIMPLEOBJECT) {
			Gods98::Container_Remove(deadObj->other);
		}
		break;
	}


	objectListSet.Remove(deadObj);

	return false;
}

// <LegoRR.exe @00437a70>
bool32 __cdecl LegoRR::LegoObject_RunThroughListsSkipUpgradeParts(LegoObject_RunThroughListsCallback callback, void* data)
{
	for (LegoObject* liveObj : objectListSet.EnumerateSkipUpgradeParts()) {

		if (callback(liveObj, data))
			return true; // terminate run through listSet
	}
	return false;
	//return LegoObject_RunThroughLists(callback, search, true);
}

// <LegoRR.exe @00437a90>
bool32 __cdecl LegoRR::LegoObject_RunThroughLists(LegoObject_RunThroughListsCallback callback, void* data, bool32 skipUpgradeParts)
{
	for (LegoObject* liveObj : objectListSet.EnumerateAlive()) {
		// Secondary filter
		if (!skipUpgradeParts || !(liveObj->flags3 & LIVEOBJ3_UPGRADEPART)) {

			if (callback(liveObj, data))
				return true; // terminate run through listSet
		}
	}
	return false;
}



// <LegoRR.exe @00438580>
LegoRR::LegoObject* __cdecl LegoRR::LegoObject_Create_internal(void)
{
	LegoObject* newLiveObj = objectListSet.Add();
	ListSet::MemZero(newLiveObj);

	newLiveObj->point_2f4.y = -1.0f;
	newLiveObj->point_2f4.x = -1.0f;

	return newLiveObj;
}

// <LegoRR.exe @004385d0>
void __cdecl LegoRR::LegoObject_AddList(void)
{
	// NOTE: This function is no longer called, objectListSet.Add already does so.
	objectListSet.AddList();
}




// <LegoRR.exe @00439c50>
void __cdecl LegoRR::LegoObject_RequestPowerGridUpdate(void)
{
	if (objectGlobs.flags & LegoObject_GlobFlags::OBJECT_GLOB_FLAG_UPDATING) {
		// We're in the update loop. Recalculating power now is dangerous when only some of the
		// objects have been update while others are awaiting an update.
		// Delay recalculation until the end of the update loop.
		objectGlobs.flags |= LegoObject_GlobFlags::OBJECT_GLOB_FLAG_POWERNEEDSUPDATE;
	}
	else {
		// Not in the update loop, go straight to power grid recalculation mode.
		objectGlobs.flags |= LegoObject_GlobFlags::OBJECT_GLOB_FLAG_POWERUPDATING;

		if (legoGlobs.currLevel != nullptr) {
			// We need to reset this before PowerGrid recalculation.
			legoGlobs.currLevel->crystalsDrained = 0;
		}
	}
}



// <LegoRR.exe @0043b530>
void __cdecl LegoRR::LegoObject_UpdateAll(real32 elapsedGame)
{
	// Were we updating our power grid after the update loop last tick?
	if (objectGlobs.flags & LegoObject_GlobFlags::OBJECT_GLOB_FLAG_POWERUPDATING) {
		Level_PowerGrid_ClearBlockPowered_100_Points28C();
	}


	objectGlobs.flags |= LegoObject_GlobFlags::OBJECT_GLOB_FLAG_UPDATING;

	for (LegoObject* obj : objectListSet.EnumerateAlive()) {
		LegoObject_Callback_Update(obj, &elapsedGame);
	}
	//LegoObject_RunThroughLists(LegoObject_Callback_Update, &elapsedGame, false);
	
	objectGlobs.flags &= ~LegoObject_GlobFlags::OBJECT_GLOB_FLAG_UPDATING;
	

	// Were we updating our power grid after the update loop last tick?
	if (objectGlobs.flags & LegoObject_GlobFlags::OBJECT_GLOB_FLAG_POWERUPDATING) {
		Level_PowerGrid_UpdateLevelBlocks_PointsAAC();
	}

	// Stop updating power grid if a recalculation is no longer needed.
	if (!(objectGlobs.flags & LegoObject_GlobFlags::OBJECT_GLOB_FLAG_POWERNEEDSUPDATE)) {
		objectGlobs.flags &= ~LegoObject_GlobFlags::OBJECT_GLOB_FLAG_POWERUPDATING;
	}
	else {
		// Otherwise, restart the request for a power grid update (turns on POWERUPDATING flag).
		LegoObject_RequestPowerGridUpdate();
		objectGlobs.flags &= ~LegoObject_GlobFlags::OBJECT_GLOB_FLAG_POWERNEEDSUPDATE;
	}

	LegoObject_Flocks_Update_FUN_0044c1c0(&elapsedGame);

	objectGlobs.LiveManager_TimerUnk -= elapsedGame;
	if (objectGlobs.LiveManager_TimerUnk <= 0.0f) {
		objectGlobs.LiveManager_TimerUnk = 125.0f;
		Lego_UpdateAll3DSounds(true);
	}
}

// <LegoRR.exe @0043b5e0>
void __cdecl LegoRR::LegoObject_RemoveAll(void)
{
	objectGlobs.flags |= LegoObject_GlobFlags::OBJECT_GLOB_FLAG_REMOVING;

	/// TODO: WHY IS SKIPUPGRADEPARTS BEING USED??? Shouldn't we be trying to remove all objects?
	///       Or was that intended to happen after this loop, but was forgotten?
	///       It's also possible upgrade parts are just handled in a wildly different way.
	for (LegoObject* obj : objectListSet.EnumerateSkipUpgradeParts()) {
		// LegoObject_Callback_Remove is just a wrapper around LegoObject_Remove. *something something, with extra steps.*
		LegoObject_Remove(obj);
		//LegoObject_Callback_Remove(obj, nullptr);
	}
	//LegoObject_RunThroughListsSkipUpgradeParts(LegoObject_Callback_Remove, nullptr);

	objectGlobs.flags &= ~LegoObject_GlobFlags::OBJECT_GLOB_FLAG_REMOVING;
}

// RunThroughLists wrapper for LegoObject_Remove.
// <LegoRR.exe @0043b610>
bool32 __cdecl LegoRR::LegoObject_Callback_Remove(LegoObject* liveObj, void* unused)
{
	// ALWAYS returns false, and even if it didn't we still wouldn't want its return value.
	/*return*/ LegoObject_Remove(liveObj);
	return false;
}




// <LegoRR.exe @00449ec0>
void __cdecl LegoRR::LegoObject_HideAllCertainObjects(void)
{
	for (LegoObject* obj : objectListSet.EnumerateAlive()) {
		LegoObject_Callback_HideCertainObjects(obj, nullptr);
	}
	//LegoObject_RunThroughLists(LegoObject_Callback_HideCertainObjects, nullptr, false);
}



// <LegoRR.exe @0044b080>
void __cdecl LegoRR::LegoObject_SetLevelEnding(bool32 ending)
{
	if (ending) objectGlobs.flags |= LegoObject_GlobFlags::OBJECT_GLOB_FLAG_LEVELENDING;
	else        objectGlobs.flags &= ~LegoObject_GlobFlags::OBJECT_GLOB_FLAG_LEVELENDING;
}

// <LegoRR.exe @0044b0a0>
void __cdecl LegoRR::LegoObject_FUN_0044b0a0(LegoObject* liveObj)
{
	if (!(objectGlobs.flags & LegoObject_GlobFlags::OBJECT_GLOB_FLAG_LEVELENDING) &&
		(liveObj->type == LegoObject_Vehicle) && !(liveObj->flags4 & LIVEOBJ4_UNK_800))
	{
		LegoObject* routeToObj = ((liveObj->flags4 & LIVEOBJ4_UNK_40) ? liveObj->routeToObject : nullptr);

		LegoObject* spawnTargetObj = (routeToObj != nullptr ? routeToObj : liveObj);

		uint32 crystalCost = Stats_GetCostCrystal(LegoObject_Vehicle, liveObj->id, 0);
		LegoObject_SpawnDropCrystals_FUN_0044b110(spawnTargetObj, crystalCost, (routeToObj != nullptr));
		liveObj->flags4 |= LIVEOBJ4_UNK_800;
	}
}




#pragma endregion
