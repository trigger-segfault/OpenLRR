// ElectricFence.cpp : 
//

#include "../../engine/core/Errors.h"
#include "../../engine/core/Maths.h"
#include "../../engine/core/Memory.h"

#include "../effects/Effects.h"
#include "../object/Stats.h"
#include "../object/Weapons.h"

#include "Map3D.h"

#include "ElectricFence.h"


/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @004c8df8>
LegoRR::ElectricFence_Globs & LegoRR::efenceGlobs = *(LegoRR::ElectricFence_Globs*)0x004c8df8;

LegoRR::ElectricFence_ListSet LegoRR::efenceListSet = LegoRR::ElectricFence_ListSet(LegoRR::efenceGlobs);

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @0040ccf0>
void __cdecl LegoRR::ElectricFence_Initialise(Lego_Level* level)
{
	/// FIXME: This code seems fishy, grid lookup is always done with multiples of 0xc, not 0x14.
	///        The wrong block type `BlockElectricFence` may have accidentically been used with here.
	uint32 gridSize = level->width * level->height * 0x14; // sizeof(BlockFenceGrid);

	efenceGlobs.fenceGrid = (BlockFenceGrid*)Gods98::Mem_Alloc(gridSize);
	if (efenceGlobs.fenceGrid != nullptr) {
		std::memset(efenceGlobs.fenceGrid, 0, gridSize);
	}

	efenceGlobs.level = level;

	efenceListSet.Initialise();
	/*for (uint32 loop = 0; loop < ELECTRICFENCE_MAXLISTS; loop++) {
		efenceGlobs.listSet[loop] = nullptr;
	}

	efenceGlobs.freeList = nullptr;
	efenceGlobs.listCount = 0;*/
}

// <LegoRR.exe @0040cd60>
void __cdecl LegoRR::ElectricFence_Shutdown(void)
{
	efenceListSet.Shutdown();
	/*for (uint32 loop = 0; loop < ELECTRICFENCE_MAXLISTS; loop++) {
		Gods98::Mem_Free(efenceGlobs.listSet[loop]);
	}

	efenceGlobs.freeList = nullptr;*/

	if (efenceGlobs.fenceGrid != nullptr) {
		Gods98::Mem_Free(efenceGlobs.fenceGrid);
		efenceGlobs.fenceGrid = nullptr;
	}
}

// <LegoRR.exe @0040cdb0>
void __cdecl LegoRR::ElectricFence_ResetAll(Lego_Level* level)
{
	ElectricFence_Shutdown();
	ElectricFence_Initialise(level);
}

// <LegoRR.exe @0040cdd0>
void __cdecl LegoRR::ElectricFence_UpdateBlockConnections(sint32 bx, sint32 by)
{
	const Point2I DIRECTIONS[4] = {
		{  0,  1 },
		{  1,  0 },
		{  0, -1 },
		{ -1,  0 },
	};

	Level_BlockUpdateSurface(efenceGlobs.level, bx, by, 0); // 0 parameter is reserved/unused

	for (uint32 loop = 0; loop < _countof(DIRECTIONS); loop++) {
		uint32 bxOff = bx + DIRECTIONS[loop].x;
		uint32 byOff = by + DIRECTIONS[loop].y;

		if (Map3D_IsInsideDimensions(efenceGlobs.level->map, bxOff, byOff)) {

			Level_BlockUpdateSurface(efenceGlobs.level, bxOff, byOff, 0); // 0 parameter is reserved/unused
		}
	}
}

// <LegoRR.exe @0040ce80>
LegoRR::BlockElectricFence* __cdecl LegoRR::ElectricFence_AssignBlockObject(LegoObject* liveObj)
{
	Point2I blockPos;
	LegoObject_GetBlockPos(liveObj, &blockPos.x, &blockPos.y);

	return ElectricFence_Create(liveObj, blockPos.x, blockPos.y);
}

// <LegoRR.exe @0040ceb0>
LegoRR::BlockElectricFence* __cdecl LegoRR::ElectricFence_Create(LegoObject* liveObj, sint32 bx, sint32 by)
{
	BlockElectricFence* newEFence = efenceListSet.Add(false); // No need to memzero, all fields are assigned.
	/*if (efenceGlobs.freeList == nullptr) ElectricFence_AddList();

	BlockElectricFence* newEFence = efenceGlobs.freeList;
	efenceGlobs.freeList = newEFence->nextFree;
	newEFence->nextFree = newEFence;*/

	newEFence->attachedObject = liveObj;
	newEFence->blockPos.x = bx;
	newEFence->blockPos.y = by;
	newEFence->timer = 0.0f; // substitution for memset, since this field isn't modified.

	efenceGlobs.fenceGrid[efenceGlobs.level->width * by + bx].flags = FENCEGRID_FLAG_NONE;
	efenceGlobs.fenceGrid[efenceGlobs.level->width * by + bx].efence = newEFence;

	liveObj->flags2 |= LIVEOBJ2_UNK_2000000;
	ElectricFence_UpdateBlockConnections(bx, by);
	return newEFence;
}

// <LegoRR.exe @0040cf60>
void __cdecl LegoRR::ElectricFence_AddList(void)
{
	// NOTE: This function is no longer called, efenceListSet.Add already does so.
	log_firstcall();

	//ElectricFence_CheckInit();

	efenceListSet.AddList();
	/*Error_Fatal(efenceGlobs.listCount + 1 >= ELECTRICFENCE_MAXLISTS, "Run out of lists");

	uint32 count = 0x00000001 << efenceGlobs.listCount;

	BlockElectricFence* list;
	if (list = efenceGlobs.listSet[efenceGlobs.listCount] = (BlockElectricFence*)Gods98::Mem_Alloc(sizeof(BlockElectricFence) * count)) {

		efenceGlobs.listCount++;

		for (uint32 loop = 1; loop < count; loop++) {
			list[loop - 1].nextFree = &list[loop];
		}
		list[count - 1].nextFree = efenceGlobs.freeList;
		efenceGlobs.freeList = list;

	}
	else Error_Fatal(true, Gods98::Error_Format("Unable to allocate %d bytes of memory for new list.\n", sizeof(BlockElectricFence) * count));*/
}

// <LegoRR.exe @0040cfd0>
void __cdecl LegoRR::ElectricFence_LiveObject_Destroy(LegoObject* liveObj)
{
	if (liveObj->flags2 & LIVEOBJ2_UNK_2000000) {
		Point2I blockPos;
		LegoObject_GetBlockPos(liveObj, &blockPos.x, &blockPos.y);
		uint32 idx = efenceGlobs.level->width * blockPos.y + blockPos.x;

		if (efenceGlobs.fenceGrid[idx].efence != nullptr) {
			ElectricFence_Remove(efenceGlobs.fenceGrid[idx].efence, blockPos.x, blockPos.y);
		}
	}
}

// <LegoRR.exe @0040d030>
void __cdecl LegoRR::ElectricFence_Remove(BlockElectricFence* dead, sint32 bx, sint32 by)
{
	/*dead->nextFree = efenceGlobs.freeList;
	efenceGlobs.freeList = dead;*/

	efenceGlobs.fenceGrid[efenceGlobs.level->width * by + bx].flags = FENCEGRID_FLAG_NONE;
	efenceGlobs.fenceGrid[efenceGlobs.level->width * by + bx].efence = nullptr;

	efenceListSet.Remove(dead);

	ElectricFence_UpdateBlockConnections(bx, by);
}

// <LegoRR.exe @0040d0a0>
bool32 __cdecl LegoRR::ElectricFence_Debug_PlaceFence(sint32 bx, sint32 by)
{
	if (ElectricFence_CanPlaceFenceAtBlock(bx, by)) {

		Point2F objectPos;
		Map3D_BlockToWorldPos(efenceGlobs.level->map, bx, by, &objectPos.x, &objectPos.y);
		LegoObject* liveObj = LegoObject_CreateInWorld(legoGlobs.contElectricFence, LegoObject_ElectricFence, 0, 0,
													   objectPos.x, objectPos.y, 0.0f);

		ElectricFence_Create(liveObj, bx, by);

		return true;
	}

	return false;
}

// <LegoRR.exe @0040d120>
bool32 __cdecl LegoRR::ElectricFence_Debug_RemoveFence(sint32 bx, sint32 by)
{
	uint32 idx = efenceGlobs.level->width * by + bx;
	if (efenceGlobs.fenceGrid[idx].efence != nullptr) {

		LegoObject* liveObj = efenceGlobs.fenceGrid[idx].efence->attachedObject;
		ElectricFence_LiveObject_Destroy(liveObj);
		LegoObject_Remove(liveObj);

		return true;
	}

	return false;
}

// <LegoRR.exe @0040d170>
bool32 __cdecl LegoRR::ElectricFence_CanPlaceFenceAtBlock(sint32 bx, sint32 by)
{
	// Original layout of array (technically separate arrays)
	/*const Point2I DIRECTIONS[8] = {
		{  0,  1 },
		{  1,  0 },
		{  0, -1 },
		{ -1,  0 },
		{  0,  2 },
		{  2,  0 },
		{  0, -2 },
		{ -2,  0 },
	};*/

	// Alternate between +1 and +2 offsets
	const Point2I DIRECTIONS[8] = {
		// {short}, {long} beam
		{  0,  1 }, {  0,  2 },
		{  1,  0 }, {  2,  0 },
		{  0, -1 }, {  0, -2 },
		{ -1,  0 }, { -2,  0 },
	};

	sint32 idx = efenceGlobs.level->width * by + bx;
	
	// Check if the terrain we're placing a fence on is valid
	// Interestingly, there's no Lake surface check... (but that only effects the debug hotkey [J])
	if (!(efenceGlobs.level->blocks[idx].flags1 & 0x8) || // !BLOCK1_FLOOR
		(efenceGlobs.level->blocks[idx].terrain == Lego_SurfaceType_Lava) ||
		(efenceGlobs.level->blocks[idx].terrain == Lego_SurfaceType_Water) || 
		(efenceGlobs.fenceGrid[idx].efence != nullptr) ||
		ElectricFence_CheckBuildingAtBlock(efenceGlobs.level, bx, by, false))
	{
		return false;
	}

	// Find a nearby fence block that we can connect to
	/// NOTE: Original loop checked offset 1, then offset 2 in a single cycle,
	///       This has been changed to just include both in the same array and reduce code.
	for (uint32 loop = 0; loop < _countof(DIRECTIONS); loop++) {
		uint32 bxOff = (uint32)(bx + DIRECTIONS[loop].x);
		uint32 byOff = (uint32)(by + DIRECTIONS[loop].y);

		uint32 idxOff = efenceGlobs.level->width * byOff + bxOff;
		if (Map3D_IsInsideDimensions(efenceGlobs.level->map, bxOff, byOff) &&
			(efenceGlobs.fenceGrid[idxOff].efence != nullptr ||
			 ElectricFence_CheckBuildingAtBlock(efenceGlobs.level, bxOff, byOff, false))) // building check?
		{
			return true;
		}
	}
	return false;
}

// <LegoRR.exe @0040d320>
bool32 __cdecl LegoRR::ElectricFence_CheckBuildingAtBlock(Lego_Level* level, sint32 bx, sint32 by, bool32 checkPowered)
{
	Point2I blockPos = { bx, by };

	if (Level_Block_IsSolidBuilding(bx, by, true)) {

		if (!checkPowered || Level_Block_IsPowered(&blockPos)) {
			return true;
		}
	}
	return false;
}

// <LegoRR.exe @0040d380>
void __cdecl LegoRR::ElectricFence_Update(real32 elapsedGame)
{
	for (BlockElectricFence* efence : efenceListSet.EnumerateAlive()) {
		ElectricFence_CallbackUpdate(efence, &elapsedGame);
	}
	//ElectricFence_RunThroughLists(ElectricFence_CallbackUpdate, &elapsedGame);

	// Both callbacks always returns false to enumerate entire listSet.
	for (LegoObject* obj : objectListSet.EnumerateSkipIgnoreMes()) {
		ElectricFence_LiveObject_Callback_FUN_0040d650(obj, nullptr); 
	}
	for (LegoObject* obj : objectListSet.EnumerateSkipIgnoreMes()) {
		ElectricFence_LiveObject_Callback_FUN_0040d510(obj, &elapsedGame);
	}
	//LegoObject_RunThroughListsSkipIgnoreMes(ElectricFence_LiveObject_Callback_FUN_0040d650, nullptr);
	//LegoObject_RunThroughListsSkipIgnoreMes(ElectricFence_LiveObject_Callback_FUN_0040d510, &elapsedGame);
}

// <LegoRR.exe @0040d3c0>
void __cdecl LegoRR::ElectricFence_RunThroughLists(ElectricFence_RunThroughListsCallback callback, void* data)
{
#if 1
	for (BlockElectricFence* efence : efenceListSet.EnumerateAlive()) {
		callback(efence, data);
	}
#else
	for (uint32 list = 0; list < efenceGlobs.listCount; list++) {

		if (efenceGlobs.listSet[list]) {
			uint32 count = 0x00000001 << list;

			for (uint32 loop = 0; loop < count; loop++) {
				// LegoRR implementation performed null check on every fence, but that
				// would have no effect since we're already null-checking the current list.
				BlockElectricFence* efence = &efenceGlobs.listSet[list][loop];

				// Is this a valid unremoved fence?
				if (efence->nextFree == efence) {
					callback(efence, data);
				}
			}
		}
	}
#endif
}

// <LegoRR.exe @0040d420>
bool32 __cdecl LegoRR::ElectricFence_FUN_0040d420(LegoObject* liveObj, uint32 bx, uint32 by)
{
	for (BlockElectricFence* efence : efenceListSet.EnumerateAlive()) {
		ElectricFence_CallbackUpdate(efence, nullptr);
	}
	//ElectricFence_RunThroughLists(ElectricFence_CallbackUpdate, nullptr);

	// Callback always returns false to enumerate entire listSet.
	for (LegoObject* obj : objectListSet.EnumerateSkipIgnoreMes()) {
		ElectricFence_LiveObject_Callback_FUN_0040d650(obj, nullptr);
	}
	//LegoObject_RunThroughListsSkipIgnoreMes(ElectricFence_LiveObject_Callback_FUN_0040d650, nullptr);

	if (liveObj != nullptr) {
		// If an object is passed, use its blockPos instead of bx,by
		Point2F objectPos;
		LegoObject_GetPosition(liveObj, &objectPos.x, &objectPos.y);
		Map3D_WorldToBlockPos_NoZ(efenceGlobs.level->map, objectPos.x, objectPos.y, (sint32*)&bx, (sint32*)&by);

		if (!(StatsObject_GetStatsFlags2(liveObj) & STATS2_SELFPOWERED) &&
			!(liveObj->flags3 & LIVEOBJ3_HASPOWER))
		{
			return false;
		}
	}
	else {
		if ((efenceGlobs.fenceGrid[efenceGlobs.level->width * by + bx].efence == nullptr) &&
			!ElectricFence_CheckBuildingAtBlock(efenceGlobs.level, bx, by, false))
		{
			return false;
		}
	}

	ElectricFence_Block_FUN_0040d780(bx, by, true, 0.0f);
	return true;
}

// <LegoRR.exe @0040d510>
bool32 __cdecl LegoRR::ElectricFence_LiveObject_Callback_FUN_0040d510(LegoObject* liveObj, void* pElapsedGame)
{
	real32 elapsedGame = *(real32*)pElapsedGame;
	if (liveObj->type == LegoObject_Building) {
		Point2I blockPos;
		LegoObject_GetBlockPos(liveObj, &blockPos.x, &blockPos.y);

		if (ElectricFence_CheckBuildingAtBlock(efenceGlobs.level, blockPos.x, blockPos.y, true)) {

			Point2F faceDir;
			LegoObject_GetFaceDirection(liveObj, &faceDir);

			Direction rotation;
			if (std::abs(faceDir.x) <= std::abs(faceDir.y)) {
				if (faceDir.y > 0.0f)
					rotation = DIRECTION_UP;
				else
					rotation = DIRECTION_DOWN;
			}
			else {
				if (faceDir.x > 0.0f)
					rotation = DIRECTION_RIGHT;
				else
					rotation = DIRECTION_LEFT;
			}

			uint32 shapeCount; // pElapsedGame
			const Point2I* buildingShape = Building_GetShapePoints(liveObj->building, (uint32*)&shapeCount);
			const Point2I* shapePoints = SelectPlace_TransformShapePoints(&blockPos, buildingShape, shapeCount, rotation);

			// Shape handling for buildings is very jank.
			for (uint32 loop = 0; loop < shapeCount; loop++) {
				if ((loop == (shapeCount - 1)) ||
					(shapePoints[loop].x != shapePoints[loop+1].x) ||
					(shapePoints[loop].y != shapePoints[loop+1].y))
				{
					ElectricFence_Block_FUN_0040d780(shapePoints[loop].x, shapePoints[loop].y, false, elapsedGame);
				}
				else {
					loop++;
				}
			}
		}
	}

	return false; // false to continue with callback
}

// <LegoRR.exe @0040d650>
bool32 __cdecl LegoRR::ElectricFence_LiveObject_Callback_FUN_0040d650(LegoObject* liveObj, void* data)
{

	if (liveObj->type == LegoObject_Building) {

		Point2I blockPos;
		LegoObject_GetBlockPos(liveObj, &blockPos.x, &blockPos.y);

		efenceGlobs.fenceGrid[efenceGlobs.level->width * blockPos.y * blockPos.x].flags = FENCEGRID_FLAG_NONE;
	}
	return false;
}

// <LegoRR.exe @0040d6a0>
void __cdecl LegoRR::ElectricFence_CallbackUpdate(BlockElectricFence* efence, void* pElapsedGame)
{
	const Point2I DIRECTIONS[4] = {
		{  0,  1 },
		{  1,  0 },
		{  0, -1 },
		{ -1,  0 },
	};

	efenceGlobs.fenceGrid[efenceGlobs.level->width * efence->blockPos.y * efence->blockPos.x].flags = FENCEGRID_FLAG_NONE;

	for (uint32 loop = 0; loop < _countof(DIRECTIONS); loop++) {
		uint32 bxOff = (uint32)(efence->blockPos.x + DIRECTIONS[loop].x);
		uint32 byOff = (uint32)(efence->blockPos.y + DIRECTIONS[loop].y);

		uint32 idxOff = efenceGlobs.level->width * byOff + bxOff;
		if (Map3D_IsInsideDimensions(efenceGlobs.level->map, bxOff, byOff)) {
			efenceGlobs.fenceGrid[idxOff].flags = FENCEGRID_FLAG_NONE;
		}
	}

	if (pElapsedGame != nullptr && efence->timer > 0.0f) {
		efence->timer -= *(real32*)pElapsedGame;
	}
}

// <LegoRR.exe @0040d780>
void __cdecl LegoRR::ElectricFence_Block_FUN_0040d780(uint32 bx, uint32 by, bool32 param_3, real32 param_4)
{
	// Alternate between +1 and +2 offsets
	const Point2I DIRECTIONS[4][2] = { // [directions][short|long]
		// {short}, {long} beam
		{ {  0,  1 }, {  0,  2 } },
		{ {  1,  0 }, {  2,  0 } },
		{ {  0, -1 }, {  0, -2 } },
		{ { -1,  0 }, { -2,  0 } },
	};

	uint32 idx = efenceGlobs.level->width * by + bx;
	efenceGlobs.fenceGrid[idx].flags |= FENCEGRID_FLAG_UNK_100;

	for (uint32 loop = 0; loop < _countof(DIRECTIONS); loop++) {
		bool found = false;

		/// NOTE: Boolean negated from assembly to match standard short/long boolean pattern
		//        (originally false was used for longBeam, while Effects uses the opposite)
		bool32 longBeam = false; // dummy init
		uint32 bxFound = 0, byFound = 0; // dummy init

		uint32 bxS = bx + DIRECTIONS[loop][0].x;
		uint32 byS = by + DIRECTIONS[loop][0].y;
		uint32 idxS = efenceGlobs.level->width * byS + bxS;

		// Check short connection:
		if (Map3D_IsInsideDimensions(efenceGlobs.level->map, bxS, byS)
			&& // check for fence or powered building (power source) at +1
			(efenceGlobs.fenceGrid[idxS].efence != nullptr ||
			 ElectricFence_CheckBuildingAtBlock(efenceGlobs.level, bxS, byS, true))
			&& // check for no building at +0
			 !ElectricFence_CheckBuildingAtBlock(efenceGlobs.level, bx, by, false))
		{
			found = true;
			longBeam = false;
			bxFound = bxS;
			byFound = byS;
		}

		// If short not found, check long connection:
		if (!found) {
			uint32 bxL = bx + DIRECTIONS[loop][1].x;
			uint32 byL = by + DIRECTIONS[loop][1].y;
			uint32 idxL = efenceGlobs.level->width * byL + bxL;

			if (Map3D_IsInsideDimensions(efenceGlobs.level->map, bxL, byL)
				&& // check for no building at +1
				(!ElectricFence_CheckBuildingAtBlock(efenceGlobs.level, bxS, byS, false) ||
				 !ElectricFence_CheckBuildingAtBlock(efenceGlobs.level, bx, by, false))
				&& // check for fence or powered building (power source) at +2
				(efenceGlobs.fenceGrid[idxL].efence != nullptr ||
				 ElectricFence_CheckBuildingAtBlock(efenceGlobs.level, bxL, byL, true))
				&& // check for no building at +0
				 !ElectricFence_CheckBuildingAtBlock(efenceGlobs.level, bx, by, false))
			{
				found = true;
				longBeam = true;
				bxFound = bxL;
				byFound = byL;
			}
		}

		if (found) {
			// I think this flag specifies that a stud exists, which is why idxShort is used for longBeam here.
			// Actually it may be "powered" state, the block is active, regardless of being a fence or stud.
			if (longBeam) { // longBeam
				efenceGlobs.fenceGrid[idxS].flags |= FENCEGRID_FLAG_UNK_100;
			}

			uint32 idxFound = efenceGlobs.level->width * byFound + bxFound;
			if ((efenceGlobs.fenceGrid[idxFound].efence == nullptr) ||
				(efenceGlobs.fenceGrid[idxFound].efence->timer <= 0.0f))
			{
				//if (!(efenceGlobs.fenceGrid[idxFound].flags & 1 << ((byte)loop & 0x1f))) {
				if (!DirectionTestFlag(efenceGlobs.fenceGrid[idxFound].flags, loop)) {

					// Apply mirrored direction flag
					efenceGlobs.fenceGrid[idx].flags |= (BlockFenceGrid_Flags)DirectionToFlag(DirectionFlip(loop)); // (1 << ((loop - 2) & 0x3));

					real32 cap200 = param_4;
					if (param_4 > 200.0f) cap200 = 200.0f; // cap

					// Chance is equal to distance to fence?
					if (param_3 || (cap200 != 0.0f && ((uint32)Gods98::Maths_Rand() % (uint32)(200.0f / cap200)) == 0)) {
						// Performs some spawn action
						ElectricFence_Block_UnkAreaDistanceBetweenBlocks(bx, by, bxFound, byFound);
					}
				}

				if (!ElectricFence_CheckBuildingAtBlock(efenceGlobs.level, bxFound, byFound, false) &&
					!(efenceGlobs.fenceGrid[idxFound].flags & FENCEGRID_FLAG_UNK_100))
				{
					ElectricFence_Block_FUN_0040d780(bxFound, byFound, param_3, param_4);
				}
			}
		}
	}
}

// <LegoRR.exe @0040db50>
bool32 __cdecl LegoRR::ElectricFence_Block_UnkAreaDistanceBetweenBlocks(uint32 bxFrom, uint32 byFrom, uint32 bxTo, uint32 byTo)
{
	// Original layout of array (technically separate arrays)
	/*const Point2I DIRECTIONS[2][4] = { // [short|long][directions]
		{
			{  0,  1 },
			{  1,  0 },
			{  0, -1 },
			{ -1,  0 },
		},{
			{  0,  2 },
			{  2,  0 },
			{  0, -2 },
			{ -2,  0 },
		},
	};*/

	// Alternate between +1 and +2 offsets
	const Point2I DIRECTIONS[8] = {
		// {short}, {long} beam
		{  0,  1 }, {  0,  2 },
		{  1,  0 }, {  2,  0 },
		{  0, -1 }, {  0, -2 },
		{ -1,  0 }, { -2,  0 },
	};

	bool found = false;
	/// NOTE: Boolean negated from assembly to match standard short/long boolean pattern
	//        (originally false was used for longBeam, while Effects uses the opposite)
	bool32 longBeam = false; // dummy init

	for (uint32 loop = 0; loop < _countof(DIRECTIONS); loop++) {
		// Short/long beam distance check:
		if (bxTo == (bxFrom + DIRECTIONS[loop].x) &&
			byTo == (byFrom + DIRECTIONS[loop].y))
		{
			found = true;

			// Even indices are short distances, while odd indices are long.
			longBeam = (loop & 0x1);
			break;
		}
	}

	if (found) {
		Vector3F vecDist; // technically direction/distance, but this might be treated like a direction (NEEDS VERIFICATION)
		ElectricFence_Block_GetDistanceBetweenBlocks(bxFrom, byFrom, bxTo, byTo, &vecDist);

		Vector3F worldPos;
		Map3D_BlockToWorldPos(efenceGlobs.level->map, bxFrom, byFrom, &worldPos.x, &worldPos.y);
		worldPos.z = Map3D_GetWorldZ(efenceGlobs.level->map, worldPos.x, worldPos.y);


		Effect_ElectricFenceBeam_Spawn(longBeam, worldPos.x, worldPos.y, worldPos.z, vecDist.x, vecDist.y, vecDist.z);
		return true;
	}
	return false;

}

// <LegoRR.exe @0040dcc0>
Vector3F* __cdecl LegoRR::ElectricFence_Block_GetDistanceBetweenBlocks(uint32 bxFrom, uint32 byFrom, uint32 bxTo, uint32 byTo, Vector3F* out_vector)
{
	Vector3F worldPosFrom;
	Map3D_BlockToWorldPos(efenceGlobs.level->map, bxFrom, byFrom, &worldPosFrom.x, &worldPosFrom.y);
	worldPosFrom.z = Map3D_GetWorldZ(efenceGlobs.level->map, worldPosFrom.x, worldPosFrom.y);

	Vector3F worldPosTo;
	Map3D_BlockToWorldPos(efenceGlobs.level->map, bxTo, byTo, &worldPosTo.x, &worldPosTo.y);
	worldPosTo.z = Map3D_GetWorldZ(efenceGlobs.level->map, worldPosTo.x, worldPosTo.y);

	Gods98::Maths_Vector3DSubtract(out_vector, &worldPosTo, &worldPosFrom);
	//out_vector->x = worldPosTo.x - worldPosFrom.x;
	//out_vector->y = worldPosTo.y - worldPosFrom.y;
	//out_vector->z = worldPosTo.z - worldPosFrom.z;

	return out_vector;
}

// <LegoRR.exe @0040dd70>
bool32 __cdecl LegoRR::ElectricFence_LiveObject_FUN_0040dd70(LegoObject* liveObj)
{
	const Point2I DIRECTIONS[4] = {
		{  0,  1 },
		{  1,  0 },
		{  0, -1 },
		{ -1,  0 },
	};

	Point2F objectPos;
	LegoObject_GetPosition(liveObj, &objectPos.x, &objectPos.y);

	uint32 by;
	uint32 bx;
	Map3D_WorldToBlockPos_NoZ(efenceGlobs.level->map, objectPos.x, objectPos.y, (sint32*)&bx, (sint32*)&by);

	uint32 idx = efenceGlobs.level->width * by + bx;

	if ((efenceGlobs.fenceGrid[idx].flags & FENCEGRID_FLAG_UNK_100) &&
		!ElectricFence_CheckBuildingAtBlock(efenceGlobs.level, bx, by, false))
	{
		bool found = false;
		uint32 bxFound = bx;
		uint32 byFound = by;

		// Check for the closest fence block.
		if (efenceGlobs.fenceGrid[idx].efence != nullptr) {
			// Fence exists at object block pos, no need to loop through others.
			if (efenceGlobs.fenceGrid[idx].efence->timer <= 0.0f) {
				found = true; // Alt: just assign if condition
			}
		}
		else {
			// Otherwise find the closest fence (if one exists).

			// Direction of minimum distance fence.
			uint32 dirMin = 0; // dummy init
			real32 distMin = 0.0f; // 0.0f is considered unset (with extra checks in-place)

			for (uint32 loop = 0; loop < _countof(DIRECTIONS); loop++) {
				uint32 bxOff = bx + DIRECTIONS[loop].x;
				uint32 byOff = by + DIRECTIONS[loop].y;
				uint32 idxOff = efenceGlobs.level->width * byOff + bxOff;

				if ((efenceGlobs.fenceGrid[idxOff].efence != nullptr) &&
					(efenceGlobs.fenceGrid[idxOff].efence->timer > 0.0f))
				{
					Point2F worldOffPos;
					Map3D_BlockToWorldPos(efenceGlobs.level->map, bxOff, byOff, &worldOffPos.x, &worldOffPos.y);

					real32 dist = Gods98::Maths_Vector2DDistance(&objectPos, &worldOffPos);

					if (dist < distMin || distMin == 0.0f) {
						found = true;
						distMin = dist;
						dirMin = loop;
					}
				}
			}

			if (found) {
				bxFound = bx + DIRECTIONS[dirMin].x;
				byFound = by + DIRECTIONS[dirMin].y;

				// Alt: add to previously assigned, but less easy to read
				//bxFound += DIRECTIONS[dirMin].x;
				//byFound += DIRECTIONS[dirMin].y;
			}
		}

		if (found) {
			// Produce sparks then restart recharge timer (visual only?).

			ElectricFence_LiveObject_SparkBlock_FUN_0040dff0(liveObj, &objectPos, bx, by);
			ElectricFence_FUN_0040d420(nullptr, bxFound, byFound);

			uint32 weaponType = Weapon_GetWeaponTypeByName("FenceSpark");
			real32 rechargeTime = Weapon_GetRechargeTime(weaponType);
			efenceGlobs.fenceGrid[efenceGlobs.level->width * byFound + bxFound].efence->timer = rechargeTime;
			return true;
		}
	}
	return false;
}

// <LegoRR.exe @0040dff0>
void __cdecl LegoRR::ElectricFence_LiveObject_SparkBlock_FUN_0040dff0(LegoObject* liveObj, Point2F* objWorldPos, uint32 bx, uint32 by)
{
	Point2F efencePos;
	Map3D_BlockToWorldPos(efenceGlobs.level->map, bx, by, &efencePos.x, &efencePos.y);
	real32 efenceZ = Map3D_GetWorldZ(efenceGlobs.level->map, efencePos.x, efencePos.y);

	Point2F objectPos = *objWorldPos;

	real32 objectZ = Map3D_GetWorldZ(efenceGlobs.level->map, objectPos.x, objectPos.y);
	real32 collHeight = StatsObject_GetCollHeight(liveObj);
	objectPos.x -= efencePos.x;
	objectPos.y -= efencePos.y;

	// false == shortBeam
	Effect_ElectricFenceBeam_Spawn(false, efencePos.x, efencePos.y, efenceZ,
								   objectPos.x, objectPos.y, ((objectZ - collHeight * 0.5f) - efenceZ));

	uint32 weaponType = Weapon_GetWeaponTypeByName("FenceSpark");
	Weapon_LegoObject_FUN_0046f530(liveObj, weaponType, true, 0.0f, &objectPos);

	LegoObject_UnkActivityCrumble_FUN_004424d0(liveObj);
}

// <LegoRR.exe @0040e110>
bool32 __cdecl LegoRR::ElectricFence_Block_FUN_0040e110(Lego_Level* level, uint32 bx, uint32 by)
{
	const Point2I DIRECTIONS[4] = {
		{  0,  1 },
		{  1,  0 },
		{  0, -1 },
		{ -1,  0 },
	};

	if (((efenceGlobs.fenceGrid == nullptr) ||
		 (efenceGlobs.fenceGrid[efenceGlobs.level->width * by + bx].efence != nullptr)) ||
		(ElectricFence_CheckBuildingAtBlock(level, bx, by, false))) {
		return false;
	}

	for (uint32 loop = 0; loop < _countof(DIRECTIONS); loop++) {
		sint32 bx1 = bx + DIRECTIONS[loop].x;
		sint32 by1 = by + DIRECTIONS[loop].y;
		sint32 idx1 = efenceGlobs.level->width * by1 + bx1;

		if ((efenceGlobs.fenceGrid[idx1].efence != nullptr) ||
			ElectricFence_CheckBuildingAtBlock(level, bx1, by1, false))
		{
			Direction dir2 = DirectionFlip(loop); // (loop - 2) & 0x3; // positive modulus 4

			sint32 bx2 = bx + DIRECTIONS[dir2].x;
			sint32 by2 = by + DIRECTIONS[dir2].y;
			sint32 idx2 = efenceGlobs.level->width * by2 + bx2;

			if ((efenceGlobs.fenceGrid[idx2].efence != nullptr) ||
				(ElectricFence_CheckBuildingAtBlock(level, bx2, by2, false) &&
				 !ElectricFence_CheckBuildingAtBlock(level, bx1, by1, false)))
			{
				return true;
			}
		}
	}
	return false;
}

// <LegoRR.exe @0040e280>
void __cdecl LegoRR::ElectricFence_Block_ElecFenceStud_FUN_0040e280(Lego_Level* level, uint32 bx, uint32 by, bool32 addNew)
{
	if (efenceGlobs.fenceGrid != nullptr) {
		uint32 idx = efenceGlobs.level->width * by + bx;
		if (addNew) {
			if (efenceGlobs.fenceGrid[idx].studObj == nullptr) {
				Point2F worldPos;
				Map3D_BlockToWorldPos(level->map, bx, by, &worldPos.x, &worldPos.y);
				LegoObject* studObj = LegoObject_CreateInWorld(legoGlobs.contElectricFenceStud,
															   LegoObject_ElectricFenceStud,
															   0, 0, worldPos.x, worldPos.y, 0.0f);
				efenceGlobs.fenceGrid[idx].studObj = studObj;
			}
		}
		else {
			if (efenceGlobs.fenceGrid[idx].studObj != nullptr) {
				LegoObject_Remove(efenceGlobs.fenceGrid[idx].studObj);
				efenceGlobs.fenceGrid[idx].studObj = nullptr;
				efenceGlobs.fenceGrid[idx].flags = FENCEGRID_FLAG_NONE;
			}
		}
	}
}

// <LegoRR.exe @0040e390>
bool32 __cdecl LegoRR::ElectricFence_Block_IsFence(sint32 bx, sint32 by)
{
	return (efenceGlobs.fenceGrid[efenceGlobs.level->width * by + bx].efence != nullptr);
}


#pragma endregion
