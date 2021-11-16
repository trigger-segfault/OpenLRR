// ElectricFence.h : 
//

#pragma once

#include "../../common.h"
#include "../../engine/colour.h"
#include "../../engine/geometry.h"
#include "../../engine/undefined.h"

#include "../../engine/core/ListSet.hpp"
#include "../../engine/gfx/Containers.h"
#include "../../engine/drawing/Images.h"

#include "../GameCommon.h"
#include "../Game.h"

#include "../object/Object.h"


namespace LegoRR
{; // !<---

/**********************************************************************************
 ******** Constants
 **********************************************************************************/

#pragma region Constants

#define ELECTRICFENCE_MAXLISTS		32			// 2^32 - 1 possible fences...

#pragma endregion

/**********************************************************************************
 ******** Function typedefs
 **********************************************************************************/

#pragma region Function typedefs

struct BlockElectricFence; // Predeclaration

// Unlike other RunThroughLists callbacks, this one does not return a boolean.
typedef void (__cdecl* ElectricFence_RunThroughListsCallback)(BlockElectricFence* efence, void* data);

#pragma endregion

/**********************************************************************************
 ******** Enumerations
 **********************************************************************************/

#pragma region Enums

enum BlockFenceGrid_Flags : uint32
{
	FENCEGRID_FLAG_NONE       = 0,

	// Grid connection flags for a single fence
	FENCEGRID_DIRECTION_UP    = 0x1, // see engine/geometry.h -> enum DirectionFlags
	FENCEGRID_DIRECTION_RIGHT = 0x2,
	FENCEGRID_DIRECTION_DOWN  = 0x4,
	FENCEGRID_DIRECTION_LEFT  = 0x8,

	FENCEGRID_FLAG_UNK_100    = 0x100, // Powered(?)
};
flags_end(BlockFenceGrid_Flags, 0x4);

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

struct BlockElectricFence // [LegoRR/ElectricFence.c|struct:0x14|tags:LISTSET]
{
	/*00,4*/	LegoObject* attachedObject;
	/*04,8*/	Point2I blockPos;
	/*0c,4*/	real32 timer;
	/*10,4*/	BlockElectricFence* nextFree; // (for listSet)
	/*14*/
};
assert_sizeof(BlockElectricFence, 0x14);


//  Note that the size when allocating fenceGrid is mistakenly 0x14, but lookup is performed with size 0xc.
struct BlockFenceGrid // [LegoRR/ElectricFence.c|struct:0xc|tags:BLOCKGRID]
{
	/*0,4*/	BlockElectricFence* efence;
	/*4,4*/	LegoObject* studObj; // Glowing lime stud  object placed between 2-block-wide connections
	/*8,4*/	BlockFenceGrid_Flags flags;
	/*c*/
};
assert_sizeof(BlockFenceGrid, 0xc);


struct ElectricFence_Globs // [LegoRR/ElectricFence.c|struct:0x90|tags:GLOBS]
{
	/*00,4*/	BlockFenceGrid* fenceGrid; // BlockFenceGrid[width * height]
	/*04,4*/	Lego_Level* level;
	/*08,80*/	BlockElectricFence* listSet[ELECTRICFENCE_MAXLISTS];
	/*88,4*/	BlockElectricFence* freeList;
	/*8c,4*/	uint32 listCount; // (no flags)
	/*90*/
};
assert_sizeof(ElectricFence_Globs, 0x90);


using ElectricFence_ListSet = ListSet::Collection<ElectricFence_Globs>;

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @004c8df8>
extern ElectricFence_Globs & efenceGlobs;

extern ElectricFence_ListSet efenceListSet;

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @0040ccf0>
void __cdecl ElectricFence_Initialise(Lego_Level* level);

// <LegoRR.exe @0040cd60>
void __cdecl ElectricFence_Shutdown(void);

// Should change this name to restart
// <LegoRR.exe @0040cdb0>
void __cdecl ElectricFence_ResetAll(Lego_Level* level);

// <LegoRR.exe @0040cdd0>
void __cdecl ElectricFence_UpdateBlockConnections(sint32 bx, sint32 by);

// <LegoRR.exe @0040ce80>
BlockElectricFence* __cdecl ElectricFence_AssignBlockObject(LegoObject* liveObj);

// <LegoRR.exe @0040ceb0>
BlockElectricFence* __cdecl ElectricFence_Create(LegoObject* liveObj, sint32 bx, sint32 by);

// <LegoRR.exe @0040cf60>
void __cdecl ElectricFence_AddList(void);

// <LegoRR.exe @0040cfd0>
void __cdecl ElectricFence_LiveObject_Destroy(LegoObject* liveObj);

// <LegoRR.exe @0040d030>
void __cdecl ElectricFence_Remove(BlockElectricFence* dead, sint32 bx, sint32 by);

// <LegoRR.exe @0040d0a0>
bool32 __cdecl ElectricFence_Debug_PlaceFence(sint32 bx, sint32 by);

// <LegoRR.exe @0040d120>
bool32 __cdecl ElectricFence_Debug_RemoveFence(sint32 bx, sint32 by);

// <LegoRR.exe @0040d170>
bool32 __cdecl ElectricFence_CanPlaceFenceAtBlock(sint32 bx, sint32 by);

// <LegoRR.exe @0040d320>
bool32 __cdecl ElectricFence_CheckBuildingAtBlock(Lego_Level* level, sint32 bx, sint32 by, bool32 checkPowered);

// <LegoRR.exe @0040d380>
void __cdecl ElectricFence_Update(real32 elapsedGame);

// Change name to ElectricFence_RunThroughLists
// <LegoRR.exe @0040d3c0>
void __cdecl ElectricFence_RunThroughLists(ElectricFence_RunThroughListsCallback callback, void* data);

// <LegoRR.exe @0040d420>
bool32 __cdecl ElectricFence_FUN_0040d420(LegoObject* liveObj, uint32 bx, uint32 by);

// <LegoRR.exe @0040d510>
bool32 __cdecl ElectricFence_LiveObject_Callback_FUN_0040d510(LegoObject* liveObj, void* pElapsedGame);

// Some sort of removal or reset callback maybe?
// <LegoRR.exe @0040d650>
bool32 __cdecl ElectricFence_LiveObject_Callback_FUN_0040d650(LegoObject* liveObj, void* data);

// <LegoRR.exe @0040d6a0>
void __cdecl ElectricFence_CallbackUpdate(BlockElectricFence* efence, void* pElapsedGame);

// <LegoRR.exe @0040d780>
void __cdecl ElectricFence_Block_FUN_0040d780(uint32 bx, uint32 by, bool32 param_3, real32 param_4);

// <LegoRR.exe @0040db50>
bool32 __cdecl ElectricFence_Block_UnkAreaDistanceBetweenBlocks(uint32 bxFrom, uint32 byFrom, uint32 bxTo, uint32 byTo);

// <LegoRR.exe @0040dcc0>
Vector3F* __cdecl ElectricFence_Block_GetDistanceBetweenBlocks(uint32 bxFrom, uint32 byFrom, uint32 bxTo, uint32 byTo, Vector3F* out_vector);

// <LegoRR.exe @0040dd70>
bool32 __cdecl ElectricFence_LiveObject_FUN_0040dd70(LegoObject* liveObj);

// <LegoRR.exe @0040dff0>
void __cdecl ElectricFence_LiveObject_SparkBlock_FUN_0040dff0(LegoObject* liveObj, Point2F* param_2, uint32 bx, uint32 by);

// <LegoRR.exe @0040e110>
bool32 __cdecl ElectricFence_Block_FUN_0040e110(Lego_Level* level, uint32 bx, uint32 by);

// <LegoRR.exe @0040e280>
void __cdecl ElectricFence_Block_ElecFenceStud_FUN_0040e280(Lego_Level* level, uint32 bx, uint32 by, bool32 addNew);

// <LegoRR.exe @0040e390>
bool32 __cdecl ElectricFence_Block_IsFence(sint32 bx, sint32 by);

#pragma endregion

}
