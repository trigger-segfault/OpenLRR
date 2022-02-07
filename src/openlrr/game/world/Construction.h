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

#define CONSTRUCTION_MAXPLACED			40
#define CONSTRUCTION_MAXBUILDINGBASES	5

#pragma endregion

/**********************************************************************************
 ******** Enumerations
 **********************************************************************************/

#pragma region Enums

enum Construction_ZoneFlags : uint32
{
	CONSTRUCTION_FLAG_NONE      = 0,
	CONSTRUCTION_FLAG_READY     = 0x1, // All resources are present, and ready to construct. Turned off after AI requests to build path.
	CONSTRUCTION_FLAG_LAYPATH   = 0x2, // Constucting path tile, not building.
	CONSTRUCTION_FLAG_REQUESTED = 0x4, // Resources have been requested.
	CONSTRUCTION_FLAG_USESTUDS  = 0x8, // Use studs instead of ore (set, but unreferenced).
};
flags_end(Construction_ZoneFlags, 0x4);

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

struct Construction_Zone // [LegoRR/Construction.c|struct:0xd4]
{
	/*00,4*/	LegoObject_ID objID;     // Always used for Buildings, but for only for Paths in shared functions.
	/*04,4*/	Direction direction;     // Direction of Building object when completed.
	/*08,4*/	uint32 handleValue;      // Unique handle for this construction zone.
	/*0c,8*/	Point2I originBlockPos;  // Alternate handle to construction zone. Used as the primary tile for resource placement. Block position of Building object when completed.
	/*14,4*/	Point2I* shapeBlocks;    // Only used by Buildings, transformed shape to match originBlockPos.
	/*18,4*/	uint32 shapeCount;
	/*1c,4*/	uint32 requestCount;     // Number of resources requested. Must be matched by placeCount to finish construction.
	/*20,4*/	uint32 placedCount;
	/*24,4*/	Construction_Zone* next; // Next in linked list.
	/*28,4*/	real32 float_28;         // (ready: 125.0 , unused, possibly old timer for delay before construction)
	/*2c,a0*/	LegoObject* placedObjects[CONSTRUCTION_MAXPLACED];
	/*cc,4*/	uint32 placedCount2;     // Always identical to placedCount, no difference in usage.
	/*d0,4*/	Construction_ZoneFlags flags;
	/*d4*/
};
assert_sizeof(Construction_Zone, 0xd4);


struct Construction_Globs // [LegoRR/Construction.c|struct:0x38|tags:GLOBS]
{
	/*00,4*/	uint32 nextHandleValue; // Next handle value used when creating a construction zone. (never reset)
	/*04,4*/	Construction_Zone* constructList; // Head of linked list.
	/*08,14*/	undefined4 unused_buildingBaseTable[CONSTRUCTION_MAXBUILDINGBASES]; // (probably related to unused buildingBaseTable)
	/*1c,14*/	char* buildingBaseName[CONSTRUCTION_MAXBUILDINGBASES]; // empty table, no names
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

// <inlined>
__inline LegoObject_Type Construction_GetObjectType(const Construction_Zone* construct)
{
	return ((construct->flags & CONSTRUCTION_FLAG_LAYPATH) ? LegoObject_Path : LegoObject_Building);
}

// <inlined, unused>
__inline bool32 Construction_IsCryOreDropDisabled(void) { return constructionGlobs.disableCryOreDrop; }



// Would have looked up the baseName in a list of up to 5 names, and returned the index (or buildingBaseCount on failure).
// No building bases are ever defined, so the function will always return 0 (aka buildingBaseCount).
// <LegoRR.exe @00408bb0>
#define Construction_GetBuildingBase ((uint32 (__cdecl* )(const char* baseName))0x00408bb0)
//uint32 __cdecl Construction_GetBuildingBase(const char* baseName);

// <LegoRR.exe @00408c10>
#define Construction_Zone_PlaceResource ((void (__cdecl* )(uint32 constructHandle, LegoObject* placedObj))0x00408c10)
//void __cdecl Construction_Zone_PlaceResource(uint32 constructHandle, LegoObject* placedObj);

// <LegoRR.exe @00408ca0>
#define Construction_Zone_NeedsMoreOfResource ((bool32 (__cdecl* )(uint32 constructHandle, LegoObject_Type objType, LegoObject_ID objID))0x00408ca0)
//bool32 __cdecl Construction_Zone_NeedsMoreOfResource(uint32 constructHandle, LegoObject_Type objType, LegoObject_ID objID);

// <LegoRR.exe @00408d40>
#define Construction_Zone_CountOfResourcePlaced ((uint32 (__cdecl* )(Construction_Zone* construct, LegoObject_Type objType, LegoObject_ID objID))0x00408d40)
//uint32 __cdecl Construction_Zone_CountOfResourcePlaced(Construction_Zone* construct, LegoObject_Type objType, LegoObject_ID objID);

// <LegoRR.exe @00408d80>
#define Construction_Zone_RequestResource ((void (__cdecl* )(const Point2I* originBlockPos, LegoObject_Type objType, LegoObject_ID objID, uint32 objLevel, uint32 count))0x00408d80)
//void __cdecl Construction_Zone_RequestResource(const Point2I* originBlockPos, LegoObject_Type objType, LegoObject_ID objID, uint32 objLevel, uint32 count);

// <LegoRR.exe @00408df0>
#define Construction_Zone_RequestBarriers ((void (__cdecl* )(const Point2I* originBlockPos, const Point2I* shapeBlocks, uint32 shapeCount))0x00408df0)
//void __cdecl Construction_Zone_RequestBarriers(const Point2I* originBlockPos, const Point2I* shapeBlocks, uint32 shapeCount);

// Checks if a construction zone origin exists at the specified block position.
// <LegoRR.exe @00408fd0>
#define Construction_Zone_ExistsAtBlock ((bool32 (__cdecl* )(const Point2I* originBlockPos))0x00408fd0)
//bool32 __cdecl Construction_Zone_ExistsAtBlock(const Point2I* originBlockPos);

// `originBlockPos` or `constructHandle` must be non-null. `originBlockPos` has priority.
// <LegoRR.exe @00408ff0>
#define Construction_Zone_FindByHandleOrAtBlock ((Construction_Zone* (__cdecl* )(OPTIONAL const Point2I* originBlockPos, OPTIONAL const uint32* constructHandle))0x00408ff0)
//Construction_Zone* __cdecl Construction_Zone_FindByHandleOrAtBlock(OPTIONAL const Point2I* originBlockPos, OPTIONAL const uint32* constructHandle);

// <LegoRR.exe @00409040>
#define Construction_Zone_CompletePath ((void (__cdecl* )(const Point2I* originBlockPos))0x00409040)
//void __cdecl Construction_Zone_CompletePath(const Point2I* originBlockPos);

// <LegoRR.exe @00409080>
#define Construction_Zone_CancelPath ((void (__cdecl* )(const Point2I* originBlockPos))0x00409080)
//void __cdecl Construction_Zone_CancelPath(const Point2I* originBlockPos);

// <LegoRR.exe @00409110>
#define Construction_UpdateAll ((void (__cdecl* )(real32 elapsedGame))0x00409110)
//void __cdecl Construction_UpdateAll(real32 elapsedGame);

// This check is used to prevent buildings from completing while unrelated units are inside the construction zone.
// <LegoRR.exe @004091a0>
#define Construction_Zone_NoForeignObjectsInside ((bool32 (__cdecl* )(Construction_Zone* construct))0x004091a0)
//bool32 __cdecl Construction_Zone_NoForeignObjectsInside(Construction_Zone* construct);

// DATA: Construction_Zone*
// <LegoRR.exe @004091c0>
#define Construction_Zone_ObjectCallback_IsForeignObjectInside ((bool32 (__cdecl* )(LegoObject* liveObj, void* data))0x004091c0)
//bool32 __cdecl Construction_Zone_ObjectCallback_IsForeignObjectInside(LegoObject* liveObj, void* data);

// Starts a construction zone for laying down a Power Path.
// <LegoRR.exe @00409230>
#define Construction_Zone_StartPath ((bool32 (__cdecl* )(const Point2I* originBlockPos))0x00409230)
//bool32 __cdecl Construction_Zone_StartPath(const Point2I* originBlockPos);

// <LegoRR.exe @00409280>
#define Construction_Zone_Create ((Construction_Zone* (__cdecl* )(const Point2I* originBlockPos))0x00409280)
//Construction_Zone* __cdecl Construction_Zone_Create(const Point2I* originBlockPos);

// <LegoRR.exe @004092e0>
#define Construction_PowerGrid_PowerAdjacentBlocks ((void (__cdecl* )(const Point2I* blockPos))0x004092e0)
//void __cdecl Construction_PowerGrid_PowerAdjacentBlocks(const Point2I* blockPos);

// Returns false if a building cannot be powered.
// <LegoRR.exe @00409380>
#define Construction_PowerGrid_DrainAdjacentBlocks ((bool32 (__cdecl* )(const Point2I* blockPos, sint32 crystalDrainedAmount))0x00409380)
//bool32 __cdecl Construction_PowerGrid_DrainAdjacentBlocks(const Point2I* blockPos, sint32 crystalDrainedAmount);

// <LegoRR.exe @004093a0>
#define Construction_PowerGrid_DrainAdjacentBlocks_Recurse ((bool32 (__cdecl* )(const Point2I* blockPos, sint32 crystalDrainedAmount))0x004093a0)
//bool32 __cdecl Construction_PowerGrid_DrainAdjacentBlocks_Recurse(const Point2I* blockPos, sint32 crystalDrainedAmount);

// Internally used for Paths during `Construction_UpdateAll`. Does NOT use objID field.
// <LegoRR.exe @00409480>
#define Construction_Zone_RequestPathResources ((void (__cdecl* )(Construction_Zone* construct))0x00409480)
//void __cdecl Construction_Zone_RequestPathResources(Construction_Zone* construct);

// <LegoRR.exe @00409530>
#define Construction_Zone_CancelBuilding ((void (__cdecl* )(const Point2I* originBlockPos))0x00409530)
//void __cdecl Construction_Zone_CancelBuilding(const Point2I* originBlockPos);

// Starts a construction zone for a Building object.
// Returns handle value to construction zone.
// <LegoRR.exe @004096c0>
#define Construction_Zone_StartBuilding ((uint32 (__cdecl* )(LegoObject_ID buildingObjID, const Point2I* originBlockPos, Direction direction, const Point2I* shapeBlocks, uint32 count))0x004096c0)
//uint32 __cdecl Construction_Zone_StartBuilding(LegoObject_ID buildingObjID, const Point2I* originBlockPos, Direction direction, const Point2I* shapeBlocks, uint32 count);

// <LegoRR.exe @00409870>
#define Construction_FlattenGround ((void (__cdecl* )(const Point2I* blockPos))0x00409870)
//void __cdecl Construction_FlattenGround(const Point2I* blockPos);

// <LegoRR.exe @00409900>
#define Construction_RemoveAll ((void (__cdecl* )(void))0x00409900)
//void __cdecl Construction_RemoveAll(void);

// Frees and removes the construction zone from the global list.
// <LegoRR.exe @00409920>
#define Construction_Zone_Free ((void (__cdecl* )(Construction_Zone* construct))0x00409920)
//void __cdecl Construction_Zone_Free(Construction_Zone* construct);

// <LegoRR.exe @00409970>
#define Construction_Zone_ConsumePlacedResources ((void (__cdecl* )(Construction_Zone* construct))0x00409970)
//void __cdecl Construction_Zone_ConsumePlacedResources(Construction_Zone* construct);

// Immediately spawns a Building object without the need of a construction zone.
// <LegoRR.exe @004099c0>
#define Construction_SpawnBuilding ((LegoObject* (__cdecl* )(LegoObject_ID objID, const Point2I* originBlockPos, Direction direction, const Point2I* shapeBlocks, uint32 shapeCount, bool32 teleportDown))0x004099c0)
//LegoObject* __cdecl Construction_SpawnBuilding(LegoObject_ID objID, const Point2I* originBlockPos, Direction direction, const Point2I* shapeBlocks, uint32 shapeCount, bool32 teleportDown);

// <LegoRR.exe @00409a60>
#define Construction_Zone_CompleteBuilding ((LegoObject* (__cdecl* )(Construction_Zone* construct, bool32 teleportDown))0x00409a60)
//LegoObject* __cdecl Construction_Zone_CompleteBuilding(Construction_Zone* construct, bool32 teleportDown);

// Calls `AITask_Block_DeselectDig` (@00402a10) for the 4 blocks surrounding blockPos.
// <LegoRR.exe @00409c00>
#define Construction_DeselectAdjacentDig ((void (__cdecl* )(const Point2I* blockPos))0x00409c00)
//void __cdecl Construction_DeselectAdjacentDig(const Point2I* blockPos);

// Disables dropping resource costs for an object when teleported up. Generally used at the end of a level.
// <LegoRR.exe @00409c70>
#define Construction_DisableCryOreDrop ((void (__cdecl* )(bool32 disabled))0x00409c70)
//void __cdecl Construction_DisableCryOreDrop(bool32 disabled);

// The process before directly removing a Building object.
// - Unsets building tile flags.
// - Disconnects efences.
// - Spawns dust effects (based on flags4).
// - Generates cryore drops for the build cost of the object.
// etc...
// <LegoRR.exe @00409c80>
#define Construction_CleanupBuildingFoundation ((void (__cdecl* )(LegoObject* liveObj))0x00409c80)
//void __cdecl Construction_CleanupBuildingFoundation(LegoObject* liveObj);

// Spawns the resource costs of an object (i.e. when a building is teleported up/destroyed).
// <LegoRR.exe @00409e50>
#define Construction_GenerateCryOreDrop ((void (__cdecl* )(LegoObject* liveObj))0x00409e50)
//void __cdecl Construction_GenerateCryOreDrop(LegoObject* liveObj);

// A wrapper around `LegoObject_Remove` for use with Building objects.
// <LegoRR.exe @00409f20>
#define Construction_RemoveBuildingObject ((void (__cdecl* )(LegoObject* liveObj))0x00409f20)
//void __cdecl Construction_RemoveBuildingObject(LegoObject* liveObj);

#pragma endregion

}
