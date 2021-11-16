// Object.cpp : 
//

#include "../../engine/core/Maths.h"
#include "../../engine/input/Input.h"

#include "../Game.h"

#include "Object.h"
#include "Stats.h"


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

bool LegoRR::LegoObject_ListSet::FilterSkipIgnoreMes(const LegoObject* liveObj)
{
	return ListSet::IsAlive(liveObj) && !(liveObj->flags3 & LIVEOBJ3_IGNOREME_UNK);
}

LegoRR::LegoObject_ListSet::enumerable<LegoRR::LegoObject_ListSet::FilterSkipIgnoreMes> LegoRR::LegoObject_ListSet::EnumerateSkipIgnoreMes()
{
	return this->EnumerateWhere<FilterSkipIgnoreMes>();
}

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

/*namespace LegoRR
{
	class LegoObject_ListSet : public ListSet::Collection<LegoObject_Globs>
	{
	public:
		//using collection = ListSet::Collection<LegoObject_Globs>;

		LegoObject_ListSet(LegoObject_Globs& cont)
			: ListSet::Collection<LegoObject_Globs>(cont)
		{
		}

	private:
		static bool FilterSkipIgnoreMes(const LegoObject* liveObj)
		{
			return ListSet::IsAlive(liveObj) && !(liveObj->flags3 & LIVEOBJ3_IGNOREME_UNK);
		}
	public:
		auto EnumerateSkipIgnoreMes()
		{
			return this->EnumerateWhere<FilterSkipIgnoreMes>();
			//return collection::enumerable<FilterSkipIgnoreMes>(this->m_cont);
		}
	};

	static LegoObject_ListSet objectListSet = LegoObject_ListSet(objectGlobs);
}*/

// <LegoRR.exe @00437a70>
bool32 __cdecl LegoRR::LegoObject_RunThroughListsSkipIgnoreMes(LegoObject_RunThroughListsCallback callback, void* data)
{
#if 1
	for (LegoObject* liveObj : objectListSet.EnumerateSkipIgnoreMes()) {

		if (callback(liveObj, data))
			return true; // terminate run through listSet
	}
	return false;
#else
	return LegoObject_RunThroughLists(predicateFunc, search, true);
#endif
}

// <LegoRR.exe @00437a90>
bool32 __cdecl LegoRR::LegoObject_RunThroughLists(LegoObject_RunThroughListsCallback callback, void* data, bool32 skipIgnoreMeObjs)
{
#if 1
	for (LegoObject* liveObj : objectListSet.EnumerateAlive()) {
		// Secondary filter
		if (!skipIgnoreMeObjs || !(liveObj->flags3 & LIVEOBJ3_IGNOREME_UNK)) {

			if (callback(liveObj, data))
				return true; // terminate run through listSet
		}
	}
	return false;
#else
	for (uint32 list = 0; list < objectGlobs.listCount; list++) {

		if (objectGlobs.listSet[list]) {
			uint32 count = 0x00000001 << list;

			for (uint32 loop = 0; loop < count; loop++) {
				// LegoRR implementation performed null check on every object, but that
				// would have no effect since we're already null-checking the current list.
				LegoObject* liveObj = &objectGlobs.listSet[list][loop];

				// Is this a valid unremoved object? And check ignoreMe function argument/flag./
				if (liveObj->nextFree == liveObj &&
					(!skipIgnoreMeObjs || !(liveObj->flags3 & LIVEOBJ3_IGNOREME_UNK)))
				{
					if (callback(liveObj, data))
						return true; // terminate run through listSet
				}
			}
		}
	}
	return false;
#endif
}



// <LegoRR.exe @00439c50>
void __cdecl LegoRR::LiveManager_InitFlagsToggle_AndClearNumDrained(void)
{
	if (objectGlobs.flags & LegoObject_GlobFlags::OBJECT_GLOB_FLAG_UPDATING) {
		objectGlobs.flags |= LegoObject_GlobFlags::OBJECT_GLOB_FLAG_UNK_8;
	}
	else {
		objectGlobs.flags |= LegoObject_GlobFlags::OBJECT_GLOB_FLAG_UNK_4;
		if (legoGlobs.currLevel != nullptr) {
			legoGlobs.currLevel->numUnusedCrystals = 0;
		}
	}
}



// <LegoRR.exe @0043b530>
void __cdecl LegoRR::LegoObject_UpdateAll(real32 elapsedGame)
{
	if (objectGlobs.flags & LegoObject_GlobFlags::OBJECT_GLOB_FLAG_UNK_4) {
		Game_LevelClearBlockFlag2_100_Points28C();
	}

	objectGlobs.flags |= LegoObject_GlobFlags::OBJECT_GLOB_FLAG_UPDATING;
#if 1
	for (LegoObject* liveObj : objectListSet.EnumerateAlive()) {
		LegoObject_Callback_Update(liveObj, &elapsedGame);
	}
#else
	LegoObject_RunThroughLists(LegoObject_Callback_Update, &elapsedGame, false);
#endif
	objectGlobs.flags &= ~LegoObject_GlobFlags::OBJECT_GLOB_FLAG_UPDATING;


	if (objectGlobs.flags & LegoObject_GlobFlags::OBJECT_GLOB_FLAG_UNK_4) {
		Game_UpdateLevelBlocks_PointsAAC();
	}

	if (!(objectGlobs.flags & LegoObject_GlobFlags::OBJECT_GLOB_FLAG_UNK_8)) {
		objectGlobs.flags &= ~LegoObject_GlobFlags::OBJECT_GLOB_FLAG_UNK_4;
	}
	else {
		LiveManager_InitFlagsToggle_AndClearNumDrained();
		// This flag is being cleared because the above function may set it.
		objectGlobs.flags &= ~LegoObject_GlobFlags::OBJECT_GLOB_FLAG_UNK_8;
	}

	Flocks_Update_FUN_0044c1c0(&elapsedGame);

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

#if 1
	for (LegoObject* liveObj : objectListSet.EnumerateSkipIgnoreMes()) {
		// We can skip past LegoObject_Callback_Remove and just call LegoObject_Remove directly.
		LegoObject_Remove(liveObj);
		//LegoObject_Callback_Remove(liveObj, nullptr);
	}
#else
	LegoObject_RunThroughListsSkipIgnoreMes(LegoObject_Callback_Remove, nullptr);
#endif

	objectGlobs.flags &= ~LegoObject_GlobFlags::OBJECT_GLOB_FLAG_REMOVING;
}

// <LegoRR.exe @0043b610>
bool32 __cdecl LegoRR::LegoObject_Callback_Remove(LegoObject *liveObj, void* data_unused)
{
	// Always returns false
	return LegoObject_Remove(liveObj);
}

// result:
//  0 = None
//  1 = Activity_Route (or Activity_Carry if LIVEOBJ1_CARRYING)
//  2 = Activity_Walk  (unused)
//  3 = Activity_Drill
//  4 = Activity_Stand
// <LegoRR.exe @00442dd0>
sint32 __cdecl LegoRR::LegoObject_FP_UpdateMovement(LegoObject* liveObj, real32 elapsed, OUT real32* transSpeed)
{
	bool bVar2;

	sint32 actionResult = 0;


	const bool32 backingUp = (liveObj->forwardSignFP < 0);

	real32 routeSpeed, tmp_transSpeed;
	LegoObject_CalculateSpeeds(liveObj, elapsed, &routeSpeed, &tmp_transSpeed);

	Vector3F origPos, origDir, origUp;
	Gods98::Container* cont = LegoObject_GetActivityContainer(liveObj);
	Gods98::Container_GetOrientation(cont, nullptr, &origDir, &origUp);
	Gods98::Container_GetPosition(cont, nullptr, &origPos);

	if (liveObj->rotateSpeedFP != 0.0f) {
		real32 rotation = liveObj->rotateSpeedFP * elapsed;

		// When backing up in a vehicle, reverse the direction for turn controls.
		if (backingUp && liveObj->type == LegoObject_Vehicle)
			rotation *= -1.0f;

		Gods98::Container_AddRotation(cont, Gods98::Container_Combine::Before, 0.0f, 1.0f, 0.0f, rotation);


		liveObj->faceDirection = origDir;

		actionResult = 1; // Activity_Route (or Activity_Carry if LIVEOBJ1_CARRYING)
	}

	//elapsed = (float)liveObj->strafeSignFP;
	if (liveObj->strafeSignFP != 0 || liveObj->forwardSignFP != 0) {
		Vector3F translation = {
			(real32)liveObj->strafeSignFP,
			0.0f,
			(real32)liveObj->forwardSignFP,
		};
		Gods98::Maths_Vector3DSetLength(&translation, &translation, routeSpeed); // routeSpeed applies elapsed

		Gods98::Container_AddTranslation(cont, Gods98::Container_Combine::Before, translation.x, 0.0f, translation.z);

		/*fVar10 = (real32)liveObj->forwardSignFP;
		fVar9 = (real32)liveObj->strafeSignFP;
		local_30.y = 0.0f;
		fVar8 = 1.0f / std::sqrt(fVar10 * fVar10 + fVar9 * fVar9);
		local_30.z = fVar8 * fVar10 * routeSpeed;
		local_30.x = fVar8 * fVar9 * routeSpeed;
		Gods98::Container_AddTranslation(cont, Gods98::Container_Combine::Before, local_30.x, 0.0f, local_30.z);*/

		actionResult = 1; // Activity_Route (or Activity_Carry if LIVEOBJ1_CARRYING)
	}
	
	//if (actionResult == 0) goto LAB_00443210;
	if (actionResult != 0) {
		bVar2 = false;

		Point2F newObjectPos = { 0.0f, 0.0f }; // dummy init
		Point2I newBlockPos = { 0, 0 }; // dummy init

		uint32 bx = 0, by = 0; // dummy init

		LegoObject_GetBlockPos(liveObj, (sint32*)&newBlockPos.x, (sint32*)&newBlockPos.y);
		LegoObject_GetPosition(liveObj, &newObjectPos.x, &newObjectPos.y);

		LegoObject* collideObj = LegoObject_DoCollisionCallbacks_FUN_00446030(liveObj, &newObjectPos, 0.0f, true);
		if (collideObj != nullptr) {

			Point2F objectPos = { origPos.x, origPos.y };
			Point2F collidePos = { 0.0f, 0.0f }; // dummy init
			LegoObject_GetPosition(collideObj, &collidePos.x, &collidePos.y);

			//Gods98::Maths_Vector2DDistance(&collidePos, &newObjectPos, &collidePos);
			//Gods98::Maths_Vector2DDistance(&objectPos, &newObjectPos, &objectPos);

			// Maths_Vector2DDirectionTo (collidePos -> newObjectPos)
			//                           (origPos    -> newObjectPos)
			Gods98::Maths_Vector2DNormalize(Gods98::Maths_Vector2DSubtract(&collidePos, &newObjectPos, &collidePos));
			Gods98::Maths_Vector2DNormalize(Gods98::Maths_Vector2DSubtract(&objectPos, &newObjectPos, &objectPos));

			if (Gods98::Maths_Vector2DDotProduct(&collidePos, &objectPos) <= 0.0f) {
				bVar2 = true;
			}

			/*Gods98::Maths_Vector2DSubtract(&collidePos, &newObjectPos, &collidePos);
			Gods98::Maths_Vector2DSubtract(&objectPos, &newObjectPos, &objectPos);
			
			real32 norm1 = 1.0f / Gods98::Maths_Vector2DModulus(&objectPos);
			real32 norm2 = 1.0f / Gods98::Maths_Vector2DModulus(&collidePos);

			//collidePos.x = newObjectPos.x - collidePos.x;
			//collidePos.y = newObjectPos.y - collidePos.y;

			//objectPos.x = newObjectPos.x - objectPos.x;
			//objectPos.y = newObjectPos.y - objectPos.y;

			//norm1 = 1.0f / std::sqrt(objectPos.y * objectPos.y + objectPos.x * objectPos.x);
			//norm2 = 1.0f / std::sqrt(collidePos.y * collidePos.y + collidePos.x * collidePos.x);
			
			fVar8 = norm2 * collidePos.y * norm1 * objectPos.y + norm2 * collidePos.x * norm1 * objectPos.x;
			if (fVar8 <= 0.0f) {
				bVar2 = true;
			}*/
		}

		if (!Game_IsNoclipOn() &&
			(!LiveObject_Routing_GetCrossTerrainType(liveObj, newBlockPos.x, newBlockPos.y, newBlockPos.x, newBlockPos.y, true) ||
			 bVar2 ||
			 Level_Block_IsSeamWall(newBlockPos.x, newBlockPos.y)))
		{
			// Block movement when colliding with a wall (and noclip is off).
			Gods98::Container_SetPosition(cont, nullptr, origPos.x, origPos.y, origPos.z);
			Gods98::Container_SetOrientation(cont, nullptr, origDir.x, origDir.y, origDir.z, origUp.x, origUp.y, origUp.z);

			liveObj->faceDirection = origDir;
		}
		else {
			if (Level_Block_IsWall(newBlockPos.x, newBlockPos.y)) {
				bool32 gap = Level_Block_IsGap(newBlockPos.x, newBlockPos.y);
				Map3D_FUN_0044fe50(Lego_GetMap(), newObjectPos.x, newObjectPos.y, gap, 0.0f, &newObjectPos.x, &newObjectPos.y);
			}

			switch (liveObj->type) {
			case LegoObject_Vehicle:
				Vehicle_SetPosition(liveObj->vehicle, newObjectPos.x, newObjectPos.y, LegoObject_GetWorldZCallback, Lego_GetMap());
				break;
			case LegoObject_MiniFigure:
				// Minifigure Z positioning dips them down a little when over Lake terrain.
				Creature_SetPosition(liveObj->miniFigure, newObjectPos.x, newObjectPos.y, LegoObject_GetWorldZCallback_Lake, Lego_GetMap());
				break;
			case LegoObject_RockMonster:
				Creature_SetPosition(liveObj->rockMonster, newObjectPos.x, newObjectPos.y, LegoObject_GetWorldZCallback, Lego_GetMap());
				break;
			}
		}

		if (Level_Block_IsWall(newBlockPos.x, newBlockPos.y) &&
			LiveObject_BlockCheck_FUN_004326a0(liveObj, newBlockPos.x, newBlockPos.y, false, true) &&
			(liveObj->type != LegoObject_MiniFigure || LegoObject_HasToolEquipped(liveObj, LegoObject_ToolType_Drill)))
		{
			actionResult = 3; // Activity_Drill
		}

		if (backingUp && !(StatsObject_GetStatsFlags1(liveObj) & STATS1_CROSSLAND)) {
			actionResult = 4; // Activity_Stand
		}

	}


	liveObj->strafeSignFP = 0;
	liveObj->forwardSignFP = 0;
	liveObj->rotateSpeedFP = 0.0f;

	if (actionResult != 0) *transSpeed = tmp_transSpeed;

	return actionResult;
}


// <LegoRR.exe @004468d0>
void __cdecl LegoRR::LegoObject_CalculateSpeeds(LegoObject* liveObj, real32 elapsed, OUT real32* routeSpeed, OUT real32* transSpeed)
{
	/*LiveFlags1 LVar1;
	LegoObject_Type LVar2;
	ulonglong uVar3;
	LegoObject* liveObj;
	float* pfVar4;
	Map3D* map;
	BOOL BVar5;
	StatsFlags2 SVar6;
	float10 fVar7;
	Point2I* out_bx;
	int* out_by;
	float* out_unk_z;
	float local_1c;
	Point2I local_18;
	float local_10;
	float local_c;
	uint local_8;
	undefined4 uStack4;*/


	if ((liveObj->flags3 & LIVEOBJ3_SIMPLEOBJECT) || liveObj->type == LegoObject_UpgradePart) {
		*transSpeed = elapsed;
		return; // end of function
	}


	real32 transCoef;
	switch (liveObj->type) {
	case LegoObject_Vehicle:
		transCoef = Vehicle_GetTransCoef(liveObj->vehicle);
		break;
	case LegoObject_MiniFigure:
		transCoef = Creature_GetTransCoef(liveObj->miniFigure);
		break;
	case LegoObject_RockMonster:
		transCoef = Creature_GetTransCoef(liveObj->rockMonster);
		break;
	case LegoObject_Building:
		transCoef = Building_GetTransCoef(liveObj->building); // always 0.0f
		break;
	}


	*routeSpeed = StatsObject_GetRouteSpeed(liveObj);

	if (liveObj->flags1 & LIVEOBJ1_DRILLING) {
		*routeSpeed *= OBJECT_DRILL_COEF /*0.05f*/;
	}
	else if ((liveObj->flags1 & LIVEOBJ1_CARRYING) && liveObj->type != LegoObject_Vehicle) {
		// Vehicles don't get slowed down when carrying.
		*routeSpeed *= OBJECT_CARRY_COEF /*0.5f*/;
	}
	else if (liveObj->flags1 & LIVEOBJ1_LIFTING) { // LIFT_COEF = CARRY_COEF
		*routeSpeed *= OBJECT_CARRY_COEF /*0.5f*/;
	}
	/*else {
		*routeSpeed = StatsObject_GetRouteSpeed(liveObj);
	}*/

	//bool32 caughtInWeb = (liveObj->flags1 & LIVEOBJ1_CAUGHTINWEB);
	if (liveObj->flags1 & LIVEOBJ1_CAUGHTINWEB) { // (liveObj->flags1 & LIVEOBJ1_CAUGHTINWEB)
		*routeSpeed *= OBJECT_WEB_COEF /*0.05f*/;
	}


	//float local_1c;
	//Point2I local_18;
	//Point2F local_10;
	real32 local_1c = 1.0f;
	Point2I blockPos = { 0, 0 }; // dummy init
	Point2F objectPos = { 0.0f, 0.0f }; // dummy init

	LegoObject_GetPosition(liveObj, &objectPos.x, &objectPos.y);

	if (Map3D_WorldToBlockPos(Lego_GetMap(), objectPos.x, objectPos.y, &blockPos.x, &blockPos.y, &local_1c) &&
		Level_Block_IsGround(blockPos.x, blockPos.y))
	{
		real32 groundCoef = 1.0f;

		const uint32 rubbleLayers = Level_Block_GetRubbleLayers(&blockPos);
		if (rubbleLayers != 0) {
			// Delta between 1.0f and RubbleCoef based on number of rubble layers out of 4.
			//const real32 layersDelta = (rubbleLayers / (real32)BLOCK_MAXRUBBLELAYERS /*4.0f*/);
			const real32 rubbleCoef = StatsObject_GetRubbleCoef(liveObj);
			groundCoef *= Gods98::Maths_Interpolate(1.0f, rubbleCoef, (rubbleLayers / (real32)BLOCK_MAXRUBBLELAYERS /*4.0f*/));
			//groundCoef *= (1.0f - ((1.0f - rubbleCoef) * (rubbleLayers / (real32)BLOCK_MAXRUBBLELAYERS /*4.0f*/)));
		}

		if (Level_Block_IsPath(&blockPos)) {
			groundCoef *= StatsObject_GetPathCoef(liveObj);
		}

		if (liveObj->type == LegoObject_MiniFigure) {
			uint32 idx = legoGlobs.currLevel->width * blockPos.y + blockPos.x;
			if (legoGlobs.currLevel->blocks[idx].terrain == Lego_SurfaceType_Lake) {
				groundCoef *= (1.0f - (local_1c * OBJECT_LAKE_COEF /*0.8f*/));
			}
		}

		// Condition moved to top of block
		//if (Level_Block_IsGround(blockPos.x, blockPos.y)) {
			*routeSpeed *= groundCoef;
		//}
	}

	if (transCoef != 0.0f)
		*transSpeed = *routeSpeed / transCoef;
	else
		*transSpeed = 1.0f;

	if (liveObj->type == LegoObject_Building &&
		(StatsObject_GetStatsFlags2(liveObj) & STATS2_UPGRADEBUILDING) &&
		(liveObj->flags2 & LIVEOBJ2_UPGRADING))
	{
		*transSpeed *= StatsObject_GetFunctionCoef(liveObj);
	}

	*routeSpeed *= elapsed;
	*transSpeed *= elapsed;
}




// <LegoRR.exe @00449ec0>
void __cdecl LegoRR::LiveManager_AllCallback_FUN_00449ec0(void)
{
#if 1
	for (LegoObject* liveObj : objectListSet.EnumerateAlive()) {
		LegoObject_Callback_FUN_00449fb0(liveObj, nullptr);
	}
#else
	LegoObject_RunThroughLists(LegoObject_Callback_FUN_00449fb0, nullptr, false);
#endif
}

// <LegoRR.exe @0044b080>
void __cdecl LegoRR::LegoObject_SetGlobFlag20(bool32 state)
{
	if (state) objectGlobs.flags |= LegoObject_GlobFlags::OBJECT_GLOB_FLAG_LEVELENDING;
	else       objectGlobs.flags &= ~LegoObject_GlobFlags::OBJECT_GLOB_FLAG_LEVELENDING;
}


namespace LegoRR {
	static bool32 __cdecl LegoObject_CameraCycleFindNextUnit(bool32 noBuildings)
	{
		for (LegoRR::LegoObject* liveObj : LegoRR::objectListSet.EnumerateSkipIgnoreMes()) {
			if (LegoRR::LegoObject_Callback_CameraCycleFindUnit(liveObj, &noBuildings))
				return true; // New unit added to cycle list and camera moved.
		}
		return false;
	}
}

// <LegoRR.exe @0044c810>
void __cdecl LegoRR::LegoObject_CameraCycleUnits(void)
{
	// Cycle-able units have now been validated since level-start (once the function exits).
	objectGlobs.flags |= LegoObject_GlobFlags::OBJECT_GLOB_FLAG_CYCLEUNITS;

#if 0
	// Start by trying to cycle through buildings.
	bool32 noBuildings = false;
	if (LegoObject_CameraCycleFindNextUnit(noBuildings))
		return; // New building added to cycle list and camera moved. We're done.


	// If no buildings are constructed, then cycle through minifigure unit types instead.
	if (objectGlobs.cycleBuildingCount == 0) {
		// Change cycle unit type to minifigures.
		noBuildings = true;
		if (LegoObject_CameraCycleFindNextUnit(noBuildings))
			return; // New minifigure added to cycle list and camera moved. We're done.
	}


	// If cycle-able units exist, then all valid units were already in the list.
	if (objectGlobs.cycleUnitCount != 0) {
		// Reset the cycle list, then jump to the first available unit of our cycle unit type (building or minifigure).

		// This behavior is used so that the cycle list is guaranteed to not repeat itself until all units
		// have been visited once. After all units have been visited, the list is started anew.
		//  (This effectively functions like a music shuffle, without the intention of shuffling.)

		// Note that the order of units may change if new valid units were created in the middle of a cycle,
		// This is because the order of the cycle is first and foremost based on where the object is in the listSet,
		// And the listSet is not intended to be ordered based on when a unit is added, as freed up slots earlier in
		// the list may be used instead.
		objectGlobs.cycleUnitCount = 0;
		objectGlobs.cycleBuildingCount = 0;

		// Find first unit to cycle to.
		LegoObject_CameraCycleFindNextUnit(noBuildings);
	}

#else
	// Start by trying to cycle through buildings.
	bool32 noBuildings = false;
	for (LegoObject* liveObj : objectListSet.EnumerateSkipIgnoreMes()) {
		if (LegoObject_Callback_CameraCycleFindUnit(liveObj, &noBuildings))
			return; // New building added to cycle list and camera moved. We're done.
	}


	// If no buildings are constructed, then cycle through minifigure unit types instead.
	if (objectGlobs.cycleBuildingCount == 0) {
		// Change cycle unit type to minifigures.
		noBuildings = true;
		for (LegoObject* liveObj : objectListSet.EnumerateSkipIgnoreMes()) {
			if (LegoObject_Callback_CameraCycleFindUnit(liveObj, &noBuildings))
				return; // New minifigure added to cycle list and camera moved. We're done.
		}
	}


	// If cycle-able units exist, then all valid units were already in the list.
	if (objectGlobs.cycleUnitCount != 0) {
		// Reset the cycle list, then jump to the first available unit of our cycle unit type (building or minifigure).

		// This behavior is used so that the cycle list is guaranteed to not repeat itself until all units
		// have been visited once. After all units have been visited, the list is started anew.
		//  (This effectively functions like a music shuffle, without the intention of shuffling.)

		// Note that the order of units may change if new valid units were created in the middle of a cycle,
		// This is because the order of the cycle is first and foremost based on where the object is in the listSet,
		// And the listSet is not intended to be ordered based on when a unit is added, as freed up slots earlier in
		// the list may be used instead.
		objectGlobs.cycleUnitCount = 0;
		objectGlobs.cycleBuildingCount = 0;

		// Find first unit to cycle to.
		for (LegoObject* liveObj : objectListSet.EnumerateSkipIgnoreMes()) {
			if (LegoObject_Callback_CameraCycleFindUnit(liveObj, &noBuildings))
				return; // First building/minifigure added to cycle list and camera moved. We're done.
		}
	}
#endif
}

// <LegoRR.exe @0044c8b0>
bool32 __cdecl LegoRR::LegoObject_Callback_CameraCycleFindUnit(LegoObject* liveObj, OPTIONAL void* pNoBuildings)
{
	//bool32 noBuildings = false;
	//if (pNoBuildings != nullptr) noBuildings = *(bool32*)pNoBuildings;

	// Only check for units of a specific object type:
	// (defaults to Buildings when null)
	if (pNoBuildings != nullptr && *(bool32*)pNoBuildings) {
		if (liveObj->type != LegoObject_MiniFigure) return false;
	}
	else {
		if (liveObj->type != LegoObject_Building) return false;
	}

	// Don't check for units already in cycleUnits:
	for (uint32 loop = 0; loop < objectGlobs.cycleUnitCount; loop++) {
		if (objectGlobs.cycleUnits[loop] == liveObj)
			return false;
	}

	if (objectGlobs.cycleUnitCount < _countof(objectGlobs.cycleUnits)) {

		objectGlobs.cycleUnits[objectGlobs.cycleUnitCount++] = liveObj;
		if (liveObj->type == LegoObject_Building)
			objectGlobs.cycleBuildingCount++;

		Lego_GotoInfo(liveObj, nullptr, false);

		uint32 bx, by; // dummy outputs

		Vector3F worldPos;
		if (Map3D_GetIntersections(legoGlobs.currLevel->map, legoGlobs.viewMain, Gods98::msx(), Gods98::msy(), &bx, &by, &worldPos)) {
			Gods98::Container_SetPosition(legoGlobs.rootLight, nullptr, worldPos.x, worldPos.y, worldPos.z - 250.0f);
			return true;
		}
	}
	else {
		objectGlobs.cycleUnitCount = 0;
		objectGlobs.cycleBuildingCount = 0;
	}
	return true; // Is returning true here intentional?
	             // So cycle functionality breaks after 256 building or minifigure units I guess...?
}

#pragma endregion
