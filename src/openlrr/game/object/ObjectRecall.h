// ObjectRecall.h : 
//

#pragma once

#include "../../common.h"
#include "../../engine/colour.h"
#include "../../engine/geometry.h"
#include "../../engine/undefined.h"

#include "../../engine/gfx/Containers.h"
#include "../../engine/drawing/Images.h"

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

struct RROSFileHeader // [LegoRR/ObjectRecall.c|struct:0x8] For .osf Object Recall save files
{
	/*0,4*/	char signature[4]; // "RROS"
	/*4,4*/	uint32 count;
	/*8*/
};
assert_sizeof(RROSFileHeader, 0x8);


struct ObjectRecallEntry // [LegoRR/ObjectRecall.c|struct:0x14] Object recall structure (for .osf file)
{
	/*00,4*/	LegoObject_AbilityFlags flags5; // LiveFlags5
	/*04,4*/	uint32 level;
	/*08,c*/	char customName[12];
	/*14*/
};
assert_sizeof(ObjectRecallEntry, 0x14);


struct ObjectRecall_Globs // [LegoRR/ObjectRecall.c|struct:0x18|tags:GLOBS]
{
	/*00,4*/	ObjectRecallEntry* recallList;
	/*04,4*/	uint32 recallUsed;
	/*08,4*/	uint32 recallCapacity;
	/*0c,4*/	ObjectRecallEntry* recallNewList;
	/*10,4*/	uint32 recallNewCount;
	/*14,4*/	bool32 loaded;
	/*18*/
};
assert_sizeof(ObjectRecall_Globs, 0x18);

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @00500e50>
extern ObjectRecall_Globs & objectrecallGlobs;

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @00459450>
#define ObjectRecall_StoreMiniFigure ((void (__cdecl* )(LegoObject* liveObj))0x00459450)

// <LegoRR.exe @00459500>
#define ObjectRecall_RecallMiniFigure ((bool32 (__cdecl* )(LegoObject* liveObj))0x00459500)

// <LegoRR.exe @00459560>
#define ObjectRecall_Save_FreeObjectRecall ((void (__cdecl* )(void))0x00459560)

// <LegoRR.exe @004595a0>
#define ObjectRecall_Save_CopyToNewObjectRecallData ((void (__cdecl* )(void))0x004595a0)

// <LegoRR.exe @00459620>
#define ObjectRecall_Save_CreateNewObjectRecall ((bool32 (__cdecl* )(void))0x00459620)

// <LegoRR.exe @00459690>
#define ObjectRecall_IsLoaded ((bool32 (__cdecl* )(void))0x00459690)

// <LegoRR.exe @004596a0>
#define ObjectRecall_SaveRROSFile ((bool32 (__cdecl* )(const char* filename))0x004596a0)

// <LegoRR.exe @00459730>
#define ObjectRecall_LoadRROSFile ((bool32 (__cdecl* )(const char* filename))0x00459730)

#pragma endregion

}
