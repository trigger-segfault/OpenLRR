// Water.h : 
//

#pragma once

#include "../GameCommon.h"


namespace LegoRR
{; // !<---

/**********************************************************************************
 ******** Forward Declarations
 **********************************************************************************/

#pragma region Forward Declarations

#pragma endregion

/**********************************************************************************
 ******** Constants
 **********************************************************************************/

#pragma region Constants

//#define WATER_MAXPOOLBLOCKS			100
//#define WATER_MAXPOOLS				10

#pragma endregion

/**********************************************************************************
 ******** Enumerations
 **********************************************************************************/

#pragma region Enums

enum WaterFlags : uint32 // [LegoRR/Water.c|flags:0x4|type:uint]
{
	WATER_FLAG_NONE       = 0,
	WATER_FLAG_HIDDEN_UNK = 0x1,
	WATER_FLAG_STATE_A    = 0x2,
	WATER_FLAG_STATE_B    = 0x4,
};
flags_end(WaterFlags, 0x4);

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs


struct WaterEntryGroup // [LegoRR/Water.c|struct:0x18]
{
	/*00,4*/	uint32 index;
	/*04,4*/	Direction direction;
	/*08,4*/	real32 float_maxDirZ_8;
	/*0c,4*/	real32 elapsedUp_c; // elapsed count-up timer
	/*10,4*/	real32 elapsedDown_10; // elapsed count-down timer
	/*14,4*/	bool32 bool_14;
	/*18*/
};
assert_sizeof(WaterEntryGroup, 0x18);


struct WaterEntry // [LegoRR/Water.c|struct:0x428]
{
	/*000,320*/	Point2F points[100];
	/*320,4*/	uint32 pointCount;
	/*324,f0*/	WaterEntryGroup groups[10];
	/*414,4*/	uint32 groupCount;
	/*418,4*/	real32 dig_z_418; // (init: -10000.0, or vertPos if PREDUG)
	/*41c,4*/	real32 dig_z2_41c;
	/*420,4*/	Gods98::Container* contMeshTrans;
	/*424,4*/	WaterFlags flags; // (0x1 = visible/active?)
	/*428*/
};
assert_sizeof(WaterEntry, 0x428);


struct WaterEntryPair // [LegoRR/Water.c|struct:0x8] This struct seems to be used solely for qsort ordering by pointsCount(?)
{
	/*0,4*/	WaterEntry* first;
	/*4,4*/	WaterEntry* second;
	/*8*/
};
assert_sizeof(WaterEntryPair, 0x8);


struct Water_Globs // [LegoRR/Water.c|struct:0x29ec|tags:GLOBS] Module globals for the unfinished "Water flooding" feature.
{
	/*0000,2990*/	WaterEntry entryTable[10];
	/*2990,4*/	uint32 entryCount;
	/*2994,50*/	WaterEntryPair pairTable[10];
	/*29e4,4*/	uint32 pairCount;
	/*29e8,4*/	real32 digDepth; // (assigned, but never used)
	/*29ec*/
};
assert_sizeof(Water_Globs, 0x29ec);

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @0054a520>
extern Water_Globs & waterGlobs;

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

// <LegoRR.exe @0046de50>
#define Water_Initialise ((void (__cdecl* )(Gods98::Container* contRoot, Lego_Level* level))0x0046de50)
//void __cdecl Water_Initialise(Gods98::Container* contRoot, Lego_Level* level);

// <LegoRR.exe @0046dfd0>
#define Water_InitGroups ((void (__cdecl* )(Gods98::Container* contRoot, Lego_Level* level))0x0046dfd0)
//void __cdecl Water_InitGroups(Gods98::Container* contRoot, Lego_Level* level);

// <LegoRR.exe @0046e140>
#define Water_InitVertices ((void (__cdecl* )(Gods98::Container* contRoot, Lego_Level* level))0x0046e140)
//void __cdecl Water_InitVertices(Gods98::Container* contRoot, Lego_Level* level);

// <LegoRR.exe @0046e480>
#define Water_Debug_LogContainerMesh ((void (__cdecl* )(bool32 logWithValues))0x0046e480)
//void __cdecl Water_Debug_LogContainerMesh(bool32 logWithValues);

// <LegoRR.exe @0046e4e0>
#define Water_Block_DestroyWallComplete ((void (__cdecl* )(Lego_Level* level, uint32 bx, uint32 by))0x0046e4e0)
//void __cdecl Water_Block_DestroyWallComplete(Lego_Level* level, uint32 bx, uint32 by);

// <LegoRR.exe @0046e5f0>
#define Water_Block_Debug_WKey ((void (__cdecl* )(uint32 bx, uint32 by))0x0046e5f0)
//void __cdecl Water_Block_Debug_WKey(uint32 bx, uint32 by);

// <LegoRR.exe @0046e650>
#define Water_Update ((void (__cdecl* )(Lego_Level* level, real32 elapsedGame))0x0046e650)
//void __cdecl Water_Update(Lego_Level* level, real32 elapsedGame);

// <LegoRR.exe @0046e8d0>
#define Water_UpdateMap3DBlocks ((void (__cdecl* )(Lego_Level* level))0x0046e8d0)
//void __cdecl Water_UpdateMap3DBlocks(Lego_Level* level);

// <LegoRR.exe @0046eb60>
#define Water_Block_SubDestroyWall_AndDebug_WKey ((WaterEntry* (__cdecl* )(uint32 bx, uint32 by, OUT uint32* groupIndex))0x0046eb60)
//WaterEntry* __cdecl Water_Block_SubDestroyWall_AndDebug_WKey(uint32 bx, uint32 by, OPTIONAL OUT uint32* groupIndex);

// DATA: const WaterEntry*
// <LegoRR.exe @0046ec60>
#define Water_QsortCompare ((sint32 (__cdecl* )(const void* a, const void* b))0x0046ec60)
//sint32 __cdecl Water_QsortCompare(const void* a, const void* b);

// <LegoRR.exe @0046ec90>
#define Water_InitSearchAddPairs ((WaterEntry* (__cdecl* )(sint32 y, uint32 xAlign, uint32 xMax))0x0046ec90)
//WaterEntry* __cdecl Water_InitSearchAddPairs(sint32 y, uint32 xAlign, uint32 xMax);

// <LegoRR.exe @0046ed90>
#define Water_InitAddPoint ((void (__cdecl* )(WaterEntry* entry, uint32 y, uint32 xAlign, uint32 xMax))0x0046ed90)
//void __cdecl Water_InitAddPoint(WaterEntry* entry, uint32 y, uint32 xAlign, uint32 xMax);

// <LegoRR.exe @0046edf0>
#define Water_InitAddPointFirst ((void (__cdecl* )(uint32 y, uint32 xAlign, uint32 xMax))0x0046edf0)
//void __cdecl Water_InitAddPointFirst(uint32 y, uint32 xAlign, uint32 xMax);

#pragma endregion

}
