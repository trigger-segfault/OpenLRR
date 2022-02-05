// ElectricFence.h : 
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

struct ElectricFence_Block;

#pragma endregion

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

// Unlike other RunThroughLists callbacks, this one does not return a boolean.
typedef void (__cdecl* ElectricFence_RunThroughListsCallback)(ElectricFence_Block* efence, void* data);

#pragma endregion

/**********************************************************************************
 ******** Enumerations
 **********************************************************************************/

#pragma region Enums

enum ElectricFence_GridFlags : uint32 // [LegoRR/Detail.c|flags:0x4|type:uint]
{
	FENCEGRID_FLAG_NONE            = 0,

	FENCEGRID_DIRECTION_FLAG_UP    = 0x1, // see engine/geometry.h -> enum DirectionFlags
	FENCEGRID_DIRECTION_FLAG_RIGHT = 0x2,
	FENCEGRID_DIRECTION_FLAG_DOWN  = 0x4,
	FENCEGRID_DIRECTION_FLAG_LEFT  = 0x8,
	FENCEGRID_DIRECTION_MASK       = (FENCEGRID_DIRECTION_FLAG_UP|FENCEGRID_DIRECTION_FLAG_RIGHT|FENCEGRID_DIRECTION_FLAG_DOWN|FENCEGRID_DIRECTION_FLAG_LEFT),

	FENCEGRID_FLAG_UNK_100         = 0x100, // Powered(?)
};
flags_end(ElectricFence_GridFlags, 0x4);

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

struct ElectricFence_Block // [LegoRR/ElectricFence.c|struct:0x14|tags:LISTSET]
{
	/*00,4*/	LegoObject* attachedObject;
	/*04,8*/	Point2I blockPos;
	/*0c,4*/	real32 timer;
	/*10,4*/	ElectricFence_Block* nextFree; // (for listSet)
	/*14*/
};
assert_sizeof(ElectricFence_Block, 0x14);


struct ElectricFence_GridBlock // [LegoRR/ElectricFence.c|struct:0xc|tags:BLOCKGRID] Note that the size when allocating fenceGrid is mistakenly 0x14, but lookup is performed with size 0xc
{
	/*0,4*/	ElectricFence_Block* efence;
	/*4,4*/	LegoObject* studObj; // Glowing lime stud  object placed between 2-block-wide connections
	/*8,4*/	ElectricFence_GridFlags flags;
	/*c*/
};
assert_sizeof(ElectricFence_GridBlock, 0xc);


struct ElectricFence_Globs // [LegoRR/ElectricFence.c|struct:0x90|tags:GLOBS]
{
	/*00,4*/	ElectricFence_GridBlock* fenceGrid; // ElectricFence_Block[width * height]
	/*04,4*/	Lego_Level* level;
	/*08,80*/	ElectricFence_Block* listSet[ELECTRICFENCE_MAXLISTS];
	/*88,4*/	ElectricFence_Block* freeList;
	/*8c,4*/	uint32 listCount; // (no listSet flags)
	/*90*/
};
assert_sizeof(ElectricFence_Globs, 0x90);


using ElectricFence_ListSet = ListSet::WrapperCollection<ElectricFence_Globs>;

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
 ******** Macros
 **********************************************************************************/

#pragma region Macros

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @0040ccf0>
#define ElectricFence_Initialise ((void (__cdecl* )(Lego_Level* level))0x0040ccf0)

// <LegoRR.exe @0040cd60>
#define ElectricFence_Shutdown ((void (__cdecl* )(void))0x0040cd60)

// <LegoRR.exe @0040cdb0>
#define ElectricFence_ResetAll ((void (__cdecl* )(Lego_Level* level))0x0040cdb0)

// <LegoRR.exe @0040cdd0>
#define ElectricFence_UpdateBlockConnections ((void (__cdecl* )(sint32 bx, sint32 by))0x0040cdd0)

// <LegoRR.exe @0040ce80>
#define ElectricFence_AssignBlockObject ((ElectricFence_Block* (__cdecl* )(LegoObject* liveObj))0x0040ce80)

// <LegoRR.exe @0040ceb0>
#define ElectricFence_Create ((ElectricFence_Block* (__cdecl* )(LegoObject* liveObj, sint32 bx, sint32 by))0x0040ceb0)

// <LegoRR.exe @0040cf60>
#define ElectricFence_AddList ((void (__cdecl* )(void))0x0040cf60)

// <LegoRR.exe @0040cfd0>
#define ElectricFence_LiveObject_Destroy ((void (__cdecl* )(LegoObject* liveObj))0x0040cfd0)

// <LegoRR.exe @0040d030>
#define ElectricFence_Remove ((void (__cdecl* )(ElectricFence_Block* efence, sint32 bx, sint32 by))0x0040d030)

// <LegoRR.exe @0040d0a0>
#define ElectricFence_Debug_PlaceFence ((bool32 (__cdecl* )(sint32 bx, sint32 by))0x0040d0a0)

// <LegoRR.exe @0040d120>
#define ElectricFence_Debug_RemoveFence ((bool32 (__cdecl* )(sint32 bx, sint32 by))0x0040d120)

// <LegoRR.exe @0040d170>
#define ElectricFence_CanPlaceFenceAtBlock ((bool32 (__cdecl* )(sint32 bx, sint32 by))0x0040d170)

// <LegoRR.exe @0040d320>
#define ElectricFence_CheckBuildingAtBlock ((bool32 (__cdecl* )(Lego_Level* level, sint32 bx, sint32 by, bool32 checkPowered))0x0040d320)

// <LegoRR.exe @0040d380>
#define ElectricFence_Update ((void (__cdecl* )(real32 elapsedGame))0x0040d380)

// <LegoRR.exe @0040d3c0>
#define ElectricFence_RunThroughLists ((void (__cdecl* )(ElectricFence_RunThroughListsCallback callback, void* data))0x0040d3c0)

// <LegoRR.exe @0040d420>
#define ElectricFence_FUN_0040d420 ((bool32 (__cdecl* )(LegoObject* liveObj, uint32 bx, uint32 by))0x0040d420)

// <LegoRR.exe @0040d510>
#define ElectricFence_Callback_FUN_0040d510 ((bool32 (__cdecl* )(LegoObject* liveObj, real32* pElapsedGame))0x0040d510)

// <LegoRR.exe @0040d650>
#define ElectricFence_LiveObject_Callback_FUN_0040d650 ((bool32 (__cdecl* )(LegoObject* liveObj, void* data))0x0040d650)

// <LegoRR.exe @0040d6a0>
#define ElectricFence_CallbackUpdate ((void (__cdecl* )(ElectricFence_Block* efence, real32* pElapsedGame))0x0040d6a0)

// <LegoRR.exe @0040d780>
#define ElectricFence_Block_FUN_0040d780 ((void (__cdecl* )(uint32 bx, uint32 by, bool32 param_3, real32 param_4))0x0040d780)

// <LegoRR.exe @0040db50>
#define ElectricFence_Block_UnkAreaDistanceBetweenBlocks ((bool32 (__cdecl* )(uint32 bxFrom, uint32 byFrom, uint32 bxTo, uint32 byTo))0x0040db50)

// returns parameter: vector
// <LegoRR.exe @0040dcc0>
#define ElectricFence_Block_GetDistanceBetweenBlocks ((Vector3F* (__cdecl* )(uint32 bxFrom, uint32 byFrom, uint32 bxTo, uint32 byTo, OUT Vector3F* vector))0x0040dcc0)

// <LegoRR.exe @0040dd70>
#define ElectricFence_LiveObject_FUN_0040dd70 ((bool32 (__cdecl* )(LegoObject* liveObj))0x0040dd70)

// <LegoRR.exe @0040dff0>
#define ElectricFence_LiveObject_SparkBlock_FUN_0040dff0 ((void (__cdecl* )(LegoObject* liveObj, const Point2F* objWorldPos, uint32 bx, uint32 by))0x0040dff0)

// <LegoRR.exe @0040e110>
#define ElectricFence_Block_FUN_0040e110 ((bool32 (__cdecl* )(Lego_Level* level, uint32 bx, uint32 by))0x0040e110)

// <LegoRR.exe @0040e280>
#define ElectricFence_Block_ElecFenceStud_FUN_0040e280 ((void (__cdecl* )(Lego_Level* level, uint32 bx, uint32 by, bool32 addNew))0x0040e280)

// <LegoRR.exe @0040e390>
#define ElectricFence_Block_IsFence ((bool32 (__cdecl* )(sint32 bx, sint32 by))0x0040e390)

#pragma endregion

}
