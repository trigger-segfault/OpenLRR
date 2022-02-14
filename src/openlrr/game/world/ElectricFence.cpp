// ElectricFence.cpp : 
//

#include "../../engine/core/Memory.h"

#include "../Game.h"
#include "../object/Object.h"
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
	///        The wrong block type `sizeof(ElectricFence_Block)` may have accidentically been used here.
	const uint32 gridSize = level->width * level->height * 0x14; // sizeof(ElectricFence_GridBlock);

	efenceGlobs.fenceGrid = (ElectricFence_GridBlock*)Gods98::Mem_Alloc(gridSize);
	if (efenceGlobs.fenceGrid != nullptr) {
		std::memset(efenceGlobs.fenceGrid, 0, gridSize);
	}

	efenceGlobs.level = level;

	efenceListSet.Initialise();
}

// <LegoRR.exe @0040cd60>
void __cdecl LegoRR::ElectricFence_Shutdown(void)
{
	if (efenceGlobs.fenceGrid != nullptr) {
		Gods98::Mem_Free(efenceGlobs.fenceGrid);
		efenceGlobs.fenceGrid = nullptr;
	}

	efenceListSet.Shutdown();
}

// <LegoRR.exe @0040cdb0>
void __cdecl LegoRR::ElectricFence_Restart(Lego_Level* level)
{
	ElectricFence_Shutdown();
	ElectricFence_Initialise(level);
}

// <LegoRR.exe @0040cdd0>
//void __cdecl LegoRR::ElectricFence_UpdateBlockConnections(sint32 bx, sint32 by);

// Public creation function for electric fences.
// <LegoRR.exe @0040ce80>
LegoRR::ElectricFence_Block* __cdecl LegoRR::ElectricFence_CreateFence(LegoObject* liveObj)
{
	sint32 bx = 0, by = 0; // dummy init
	LegoObject_GetBlockPos(liveObj, &bx, &by);

	return ElectricFence_Create(liveObj, bx, by);
}

// <LegoRR.exe @0040ceb0>
LegoRR::ElectricFence_Block* __cdecl LegoRR::ElectricFence_Create(LegoObject* liveObj, sint32 bx, sint32 by)
{
	ElectricFence_Block* newEFence = efenceListSet.Add();
	//ListSet::MemZero(newEFence); // No need to memzero, all fields are assigned.

	newEFence->attachedObject = liveObj;
	newEFence->blockPos.x = bx;
	newEFence->blockPos.y = by;
	newEFence->timer = 0.0f;

	//efenceGlobs.fenceGrid[efenceGlobs.level->width * by + bx]
	efenceBlockValue(bx, by).flags = FENCEGRID_FLAG_NONE;
	efenceBlockValue(bx, by).efence = newEFence;

	liveObj->flags2 |= LIVEOBJ2_UNK_2000000;
	ElectricFence_UpdateBlockConnections(bx, by);
	return newEFence;
}

// <LegoRR.exe @0040cf60>
void __cdecl LegoRR::ElectricFence_AddList(void)
{
	// NOTE: This function is no longer called, efenceListSet.Add already does so.
	efenceListSet.AddList();
}

// Public removal function for electric fences.
// <LegoRR.exe @0040cfd0>
void __cdecl LegoRR::ElectricFence_RemoveFence(LegoObject* liveObj)
{
	if (liveObj->flags2 & LIVEOBJ2_UNK_2000000) {

		sint32 bx = 0, by = 0; // dummy init
		LegoObject_GetBlockPos(liveObj, &bx, &by);

		if (efenceBlockValue(bx, by).efence != nullptr) {
			ElectricFence_Remove(efenceBlockValue(bx, by).efence, bx, by);
		}
	}
}

// <LegoRR.exe @0040d030>
void __cdecl LegoRR::ElectricFence_Remove(ElectricFence_Block* dead, sint32 bx, sint32 by)
{
	efenceListSet.Remove(dead);

	//efenceGlobs.fenceGrid[efenceGlobs.level->width * by + bx]
	efenceBlockValue(bx, by).flags = FENCEGRID_FLAG_NONE;
	efenceBlockValue(bx, by).efence = nullptr;

	ElectricFence_UpdateBlockConnections(bx, by);
}

// <LegoRR.exe @0040d0a0>
//bool32 __cdecl LegoRR::ElectricFence_Debug_PlaceFence(sint32 bx, sint32 by);

// <LegoRR.exe @0040d120>
//bool32 __cdecl LegoRR::ElectricFence_Debug_RemoveFence(sint32 bx, sint32 by);

// <LegoRR.exe @0040d170>
//bool32 __cdecl LegoRR::ElectricFence_CanPlaceFenceAtBlock(sint32 bx, sint32 by);

// <LegoRR.exe @0040d320>
//bool32 __cdecl LegoRR::ElectricFence_CheckBuildingAtBlock(Lego_Level* level, sint32 bx, sint32 by, bool32 checkPowered);

// <LegoRR.exe @0040d380>
//void __cdecl LegoRR::ElectricFence_UpdateAll(real32 elapsedGame);

// <LegoRR.exe @0040d3c0>
void __cdecl LegoRR::ElectricFence_RunThroughLists(ElectricFence_RunThroughListsCallback callback, void* data)
{
	// ElectricFence has the only RunThroughLists function without a conditional stop.
	for (ElectricFence_Block* efence : efenceListSet.EnumerateAlive()) {
		callback(efence, data);
	}
}

// <LegoRR.exe @0040d420>
//bool32 __cdecl LegoRR::ElectricFence_FUN_0040d420(LegoObject* liveObj, uint32 bx, uint32 by);

// DATA: real32* pElapsedGame
// <LegoRR.exe @0040d510>
//bool32 __cdecl LegoRR::ElectricFence_Callback_FUN_0040d510(LegoObject* liveObj, void* pElapsedGame);

// <LegoRR.exe @0040d650>
//bool32 __cdecl LegoRR::ElectricFence_LiveObject_Callback_FUN_0040d650(LegoObject* liveObj, void* unused);

// DATA: real32* pElapsedGame
// <LegoRR.exe @0040d6a0>
//void __cdecl LegoRR::ElectricFence_CallbackUpdate(ElectricFence_Block* efence, void* pElapsedGame);

// <LegoRR.exe @0040d780>
//void __cdecl LegoRR::ElectricFence_Block_FUN_0040d780(uint32 bx, uint32 by, bool32 param_3, real32 param_4);

// <LegoRR.exe @0040db50>
//bool32 __cdecl LegoRR::ElectricFence_Block_UnkAreaDistanceBetweenBlocks(uint32 bxFrom, uint32 byFrom, uint32 bxTo, uint32 byTo);

// returns parameter: vector
// <LegoRR.exe @0040dcc0>
//Vector3F* __cdecl LegoRR::ElectricFence_Block_GetDistanceBetweenBlocks(uint32 bxFrom, uint32 byFrom, uint32 bxTo, uint32 byTo, OUT Vector3F* vector);

// <LegoRR.exe @0040dd70>
//bool32 __cdecl LegoRR::ElectricFence_LiveObject_FUN_0040dd70(LegoObject* liveObj);

// <LegoRR.exe @0040dff0>
//void __cdecl LegoRR::ElectricFence_LiveObject_SparkBlock_FUN_0040dff0(LegoObject* liveObj, Point2F* objWorldPos, uint32 bx, uint32 by);

// <LegoRR.exe @0040e110>
//bool32 __cdecl LegoRR::ElectricFence_Block_FUN_0040e110(Lego_Level* level, uint32 bx, uint32 by);

// <LegoRR.exe @0040e280>
//void __cdecl LegoRR::ElectricFence_Block_ElecFenceStud_FUN_0040e280(Lego_Level* level, uint32 bx, uint32 by, bool32 addNew);

// <LegoRR.exe @0040e390>
//bool32 __cdecl LegoRR::ElectricFence_Block_IsFence(sint32 bx, sint32 by);

#pragma endregion
