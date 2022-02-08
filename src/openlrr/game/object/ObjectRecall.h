// ObjectRecall.h : 
//

#pragma once

#include "../GameCommon.h"


namespace LegoRR
{; // !<---

/**********************************************************************************
 ******** Constants
 **********************************************************************************/

#pragma region Constants

#define OBJECTRECALL_FILE_SIGNATURE		"RROS"

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

// Moved to ObjectRecall.h, as a temporary location that both FrontEnd.h and Object.h can rely on without needing to include these heavy modules.
#if true
struct SaveStruct_18 // [LegoRR/save.c|struct:0x18]
{
	/*00,18*/	undefined field_0x0_0x17[24];
	/*18*/
};
assert_sizeof(SaveStruct_18, 0x18);
#endif


struct RROSFileHeader // [LegoRR/ObjectRecall.c|struct:0x8] For .osf Object Recall save files
{
	/*0,4*/	char signature[4]; // "RROS"
	/*4,4*/	uint32 count;
	///*8,0*/	ObjectRecallEntry entries[];
	/*8*/
};
assert_sizeof(RROSFileHeader, 0x8);


struct ObjectRecallEntry // [LegoRR/ObjectRecall.c|struct:0x14] Object recall structure (for .osf file)
{
	/*00,4*/	LegoObject_AbilityFlags abilityFlags; // LiveFlags5
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
extern ObjectRecall_Globs & objectRecallGlobs;

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @00459450>
#define ObjectRecall_StoreMiniFigure ((void (__cdecl* )(const LegoObject* liveObj))0x00459450)

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
