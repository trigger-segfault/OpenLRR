// Dependencies.h : 
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

#define DEPENDENCY_MAXUNLOCKS		32

#pragma endregion

/**********************************************************************************
 ******** Enumerations
 **********************************************************************************/

#pragma region Enums

enum DependencyFlags : uint32 // [LegoRR/Dependencies.c|flags:0x4|type:uint]
{
	DEPENDENCY_FLAG_NONE        = 0,
	DEPENDENCY_FLAG_HITONCESTAY = 0x1, // Specifies that the dependencies only have to previously been met
	DEPENDENCY_FLAG_UNK_2       = 0x2, // Set when the dependencies have previously been met
	DEPENDENCY_FLAG_UNK_4       = 0x4,
	DEPENDENCY_FLAG_UNK_8       = 0x8,
	DEPENDENCY_FLAG_UNK_100     = 0x100,
};
flags_end(DependencyFlags, 0x4);

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

struct DependencyRequirement // [LegoRR/Dependencies.c|struct:0x10]
{
	/*00,4*/	LegoObject_Type objType;
	/*04,4*/	LegoObject_ID objID;
	/*08,4*/	uint32 objLevel;
	/*0c,4*/	bool32 hasLevel;
	/*10*/
};
assert_sizeof(DependencyRequirement, 0x10);


struct DependencyUnlocks // [LegoRR/Dependencies.c|struct:0x204]
{
	/*000,80*/	LegoObject_Type objTypes[DEPENDENCY_MAXUNLOCKS];
	/*080,80*/	LegoObject_ID objIDs[DEPENDENCY_MAXUNLOCKS];
	/*100,80*/	uint32 objLevels[DEPENDENCY_MAXUNLOCKS];
	/*180,80*/	bool32 objHasLevels[DEPENDENCY_MAXUNLOCKS]; // (unlocks for a specific level)
	/*200,4*/	uint32 count;
	/*204*/
};
assert_sizeof(DependencyUnlocks, 0x204);


struct DependencyData // [LegoRR/Dependencies.c|struct:0xc4]
{
	/*00,40*/	uint32 numRequirements[OBJECT_MAXLEVELS]; // [levels:16]
	/*40,40*/	DependencyRequirement* requirements[OBJECT_MAXLEVELS]; // [levels:16][*numRequired] Ptr to table of size numRequired
	/*80,4*/	bool32 manualLevel; // Force requirements for specific level if key format: "<HitOnceStay|*>:Object:<#>"
	/*84,40*/	DependencyFlags levelFlags[OBJECT_MAXLEVELS]; // [levels:16] (1 = HitOnceStay)
	/*c4*/
};
assert_sizeof(DependencyData, 0xc4);


struct Dependencies_Globs // [LegoRR/Dependencies.c|struct:0xe5b4|tags:GLOBS]
{
	/*0000,e5b0*/	DependencyData table[LegoObject_Type_Count][LegoObject_ID_Count];
	/*e5b0,4*/	bool32 disabled; // (Debug feature with F11 key)
	/*e5b4*/
};
assert_sizeof(Dependencies_Globs, 0xe5b4);

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @004b9bc8>
extern Dependencies_Globs & dependencyGlobs;

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

// <inlined>
inline bool Dependencies_IsEnabled() { return !dependencyGlobs.disabled; }


// <LegoRR.exe @0040aa40>
//#define Dependencies_SetEnabled ((void (__cdecl* )(bool32 on))0x0040aa40)
void __cdecl Dependencies_SetEnabled(bool32 on);

// <LegoRR.exe @0040aa60>
//#define Dependencies_Reset_ClearAllLevelFlags_10c ((void (__cdecl* )(void))0x0040aa60)
void __cdecl Dependencies_Reset_ClearAllLevelFlags_10c(void);

// <LegoRR.exe @0040aaa0>
//#define Dependencies_Initialise ((void (__cdecl* )(const Gods98::Config* config, const char* gameName))0x0040aaa0)
void __cdecl Dependencies_Initialise(const Gods98::Config* config, const char* gameName);

// <LegoRR.exe @0040add0>
//#define Dependencies_Object_FUN_0040add0 ((bool32 (__cdecl* )(LegoObject_Type objType, LegoObject_ID objID, uint32 objLevel))0x0040add0)
bool32 __cdecl Dependencies_Object_FUN_0040add0(LegoObject_Type objType, LegoObject_ID objID, uint32 objLevel);

// <LegoRR.exe @0040ae70>
//#define Dependencies_LiveObject_CallbackCheck_FUN_0040ae70 ((bool32 (__cdecl* )(LegoObject* liveObj, DependencyRequirement* otherObj))0x0040ae70)
bool32 __cdecl Dependencies_LiveObject_CallbackCheck_FUN_0040ae70(LegoObject* liveObj, DependencyRequirement* otherObj);

// <LegoRR.exe @0040aec0>
//#define Dependencies_Object_GetRequirements ((void (__cdecl* )(LegoObject_Type objType, LegoObject_ID objID, uint32 objLevel, OUT DependencyRequirement** requirements, OUT uint32* count))0x0040aec0)
void __cdecl Dependencies_Object_GetRequirements(LegoObject_Type objType, LegoObject_ID objID, uint32 objLevel, OUT DependencyRequirement** requirements, OUT uint32* count);

// <LegoRR.exe @0040af30>
//#define Dependencies_Object_Unlock ((void (__cdecl* )(LegoObject_Type objType, LegoObject_ID objID, uint32 objLevel, DependencyUnlocks* unlocks))0x0040af30)
void __cdecl Dependencies_Object_Unlock(LegoObject_Type objType, LegoObject_ID objID, uint32 objLevel, DependencyUnlocks* unlocks);

// <LegoRR.exe @0040b0e0>
//#define Dependencies_Prepare_Unk ((void (__cdecl* )(void))0x0040b0e0)
void __cdecl Dependencies_Prepare_Unk(void);

// <LegoRR.exe @0040b180>
//#define Dependencies_Object_IsLevelFlag4 ((bool32 (__cdecl* )(LegoObject_Type objType, LegoObject_ID objID, uint32 objLevel))0x0040b180)
bool32 __cdecl Dependencies_Object_IsLevelFlag4(LegoObject_Type objType, LegoObject_ID objID, uint32 objLevel);

// <LegoRR.exe @0040b1d0>
//#define Dependencies_Object_AddLevelFlag_100 ((void (__cdecl* )(LegoObject_Type objType, LegoObject_ID objID, uint32 objLevel))0x0040b1d0)
void __cdecl Dependencies_Object_AddLevelFlag_100(LegoObject_Type objType, LegoObject_ID objID, uint32 objLevel);

// <LegoRR.exe @0040b210>
//#define Dependencies_Object_GetLevelFlag_100 ((bool32 (__cdecl* )(LegoObject_Type objType, LegoObject_ID objID, uint32 objLevel))0x0040b210)
bool32 __cdecl Dependencies_Object_GetLevelFlag_100(LegoObject_Type objType, LegoObject_ID objID, uint32 objLevel);

#pragma endregion

}
