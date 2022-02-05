// Construction.h : 
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

#pragma endregion

/**********************************************************************************
 ******** Enumerations
 **********************************************************************************/

#pragma region Enums

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

struct Construction_Block // [LegoRR/Construction.c|struct:0xd4]
{
	/*00,4*/	LegoObject_ID objID;
	/*04,4*/	Direction direction;
	/*08,4*/	sint32 int_8;
	/*0c,8*/	Point2I pointi_c;
	/*14,4*/	Point2I* shapePoints;
	/*18,4*/	uint32 shapeCount;
	/*1c,8*/	undefined field_0x1c_0x23[8];
	/*24,4*/	Construction_Block* next_24;
	/*28,4*/	undefined field_0x28_0x2b[4];
	/*2c,4*/	LegoObject* liveObjs_2c[1];
	/*30,9c*/	undefined field_0x30_0xcb[156];
	/*cc,4*/	uint32 liveObjsCount_cc;
	/*d0,4*/	uint32 flags;
	/*d4*/
};

struct Construction_Globs // [LegoRR/Construction.c|struct:0x38|tags:GLOBS]
{
	/*00,4*/	uint32 uint_0; // (related to Construction->int_8)
	/*04,4*/	Construction_Block* construct_4;
	/*08,14*/	undefined4 reserved1Table[5]; // (probably related to unused buildingBaseTable)
	/*1c,14*/	char* buildingBaseTable[5]; // empty table, no names
	/*30,4*/	uint32 buildingBaseCount;
	/*34,4*/	bool32 disableCryOreDrop;
	/*38*/
};
assert_sizeof(Construction_Globs, 0x38);

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @004b9a20>
extern Construction_Globs & constructionGlobs;

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

// <LegoRR.exe @00408bb0>
#define Construction_GetBuildingBase ((uint32 (__cdecl* )(const char* name))0x00408bb0)

// <LegoRR.exe @00408c10>
#define Construction_FUN_00408c10 ((void (__cdecl* )(sint32 param_1, LegoObject* liveObj))0x00408c10)

// <LegoRR.exe @00408ca0>
#define Construction_FUN_00408ca0 ((bool32 (__cdecl* )(sint32 param_1, sint32 param_2, sint32 param_3))0x00408ca0)

// <LegoRR.exe @00408d40>
#define Construction_FUN_00408d40 ((sint32 (__cdecl* )(Construction_Block* construct, LegoObject_Type objType, LegoObject_ID objID))0x00408d40)

// <LegoRR.exe @00408d80>
#define Construction_SpawnCarryableObject ((void (__cdecl* )(const Point2I* blockPos, LegoObject_Type objType, LegoObject_ID objID, sint32 objLevel, uint32 count))0x00408d80)

// <LegoRR.exe @00408df0>
#define Construction_FUN_00408df0 ((void (__cdecl* )(const Point2I* blockPos, Point2I* shapePoints, uint32 shapeCount))0x00408df0)

// <LegoRR.exe @00408fd0>
#define Construction_BlockCheck_FUN_00408fd0 ((bool32 (__cdecl* )(const Point2I* blockPos))0x00408fd0)

// <LegoRR.exe @00408ff0>
#define Construction_BlockCheck_FUN_00408ff0 ((Construction_Block* (__cdecl* )(const Point2I* blockPos, OPTIONAL sint32* param_2))0x00408ff0)

// <LegoRR.exe @00409040>
#define Construction_Block_FUN_00409040 ((void (__cdecl* )(const Point2I* blockPos))0x00409040)

// <LegoRR.exe @00409080>
#define Construction_Cancel2 ((void (__cdecl* )(const Point2I* blockPos))0x00409080)

// <LegoRR.exe @00409110>
#define Construction_Update ((void (__cdecl* )(real32 elapsedGame))0x00409110)

// <LegoRR.exe @004091a0>
#define Construction_LiveObject_DoBlockStructD4_Callbacks_FUN_004091a0 ((bool32 (__cdecl* )(Construction_Block* construct))0x004091a0)

// <LegoRR.exe @004091c0>
#define Construction_LiveObject_Callback_FindIn_BlockStructD4Table ((bool32 (__cdecl* )(LegoObject* liveObj, Construction_Block* data))0x004091c0)

// <LegoRR.exe @00409230>
#define Construction_LayPath ((bool32 (__cdecl* )(const Point2I* blockPos))0x00409230)

// <LegoRR.exe @00409280>
#define Construction_CreateBlock ((Construction_Block* (__cdecl* )(const Point2I* blockPos))0x00409280)

// <LegoRR.exe @004092e0>
#define Construction_Level_Block_FUN_004092e0 ((void (__cdecl* )(const Point2I* blockPos))0x004092e0)

// <LegoRR.exe @00409380>
#define Construction_Level_Blocks_UnkPowerCrystalProc_FUN_00409380 ((bool32 (__cdecl* )(const Point2I* blockPos, sint32 crystalCount))0x00409380)

// <LegoRR.exe @004093a0>
#define Construction_Level_Block_UnkDrainedPower_FUN_004093a0 ((bool32 (__cdecl* )(const Point2I* blockPos, sint32 crystalCount))0x004093a0)

// <LegoRR.exe @00409480>
#define Construction_FUN_00409480 ((void (__cdecl* )(Construction_Block* construct))0x00409480)

// <LegoRR.exe @00409530>
#define Construction_Cancel ((void (__cdecl* )(const Point2I* blockPos))0x00409530)

// <LegoRR.exe @004096c0>
#define Construction_Start ((sint32 (__cdecl* )(LegoObject_ID objID, const Point2I* blockPos, Direction direction, const Point2I* shapePoints, uint32 count))0x004096c0)

// <LegoRR.exe @00409870>
#define Construction_FlattenGround ((void (__cdecl* )(const Point2I* blockPos))0x00409870)

// <LegoRR.exe @00409900>
#define Construction_RemoveAll ((void (__cdecl* )(void))0x00409900)

// <LegoRR.exe @00409920>
#define Construction_Free ((void (__cdecl* )(Construction_Block* construct))0x00409920)

// <LegoRR.exe @00409970>
#define Construction_FUN_00409970 ((void (__cdecl* )(Construction_Block* construct))0x00409970)

// <LegoRR.exe @004099c0>
#define Construction_AddShape_FUN_004099c0 ((LegoObject* (__cdecl* )(LegoObject_ID objID, const Point2I* blockPos, Direction direction, const Point2I* shapePoints, uint32 shapeCount, bool32 teleportDown))0x004099c0)

// <LegoRR.exe @00409a60>
#define Construction_Complete ((LegoObject* (__cdecl* )(Construction_Block* construct, bool32 teleportDown))0x00409a60)

// <LegoRR.exe @00409c00>
#define Construction_Block_FUN_00409c00 ((void (__cdecl* )(const Point2I* blockPos))0x00409c00)

// <LegoRR.exe @00409c70>
#define Game_DisableCryOreDrop ((void (__cdecl* )(bool32 param_1))0x00409c70)

// <LegoRR.exe @00409c80>
#define LiveObject_Building_Remove_FUN_00409c80 ((void (__cdecl* )(LegoObject* liveObj))0x00409c80)

// <LegoRR.exe @00409e50>
#define LiveObject_GenerateCryOre_FromCosts ((void (__cdecl* )(LegoObject* liveObj))0x00409e50)

// <LegoRR.exe @00409f20>
#define LiveObject_BuildingDestroy ((void (__cdecl* )(LegoObject* liveObj))0x00409f20)

#pragma endregion

}
