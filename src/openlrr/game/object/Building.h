// Building.h : 
//

#pragma once

#include "../GameCommon.h"
#include "Upgrade.h"
#include "Weapons.h"


namespace LegoRR
{; // !<---

/**********************************************************************************
 ******** Forward Declarations
 **********************************************************************************/

#pragma region Forward Declarations

struct LegoObject;
struct Map3D;

#pragma endregion

/**********************************************************************************
 ******** Constants
 **********************************************************************************/

#pragma region Constants

#define BUILDING_MAXCAMERAS			4
#define BUILDING_MAXCARRYS			6

 // NOTE: Index five is never observed, this may actually be a max of 5.
#define BUILDING_MAXTOOLS			6

#pragma endregion

/**********************************************************************************
 ******** Enumerations
 **********************************************************************************/

#pragma region Enums

enum BuildingFlags : uint32
{
	BUILDING_FLAG_NONE            = 0,
	BUILDING_FLAG_SOURCE          = 0x1, // Source object container data with original memory allocations. (broken in LegoRR)
	BUILDING_FLAG_POWERLEVELSCENE = 0x2, // powerLevelScene is playing(?)
};
flags_end(BuildingFlags, 0x4);

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

struct BuildingModel // [LegoRR/Building.c|struct:0x14c]
{
	/*000,4*/	LegoObject_ID objID;
	/*004,4*/	Gods98::Container* contAct;	// (ACT, true)
	/*008,4*/	char* carryNullName;		// (ae: CarryNullName)
	/*00c,4*/	char* cameraNullName;		// (ae: CameraNullName)
	/*010,4*/	char* depositNullName;		// (ae: DepositNullName)
	/*014,4*/	char* entranceNullName;		// (ae: EntranceNullName)
	/*018,4*/	char* toolNullName;			// (ae: ToolNullName)
	/*01c,4*/	char* fireNullName;			// (ae: FireNullName) "fire laser"
	/*020,4*/	char* yPivot;				// (ae: yPivot)
	/*024,4*/	char* xPivot;				// (ae: xPivot)
	/*028,18*/	Gods98::Container* carryNulls[BUILDING_MAXCARRYS];
	/*040,10*/	Gods98::Container* cameraNulls[BUILDING_MAXCAMERAS];
	/*050,18*/	Gods98::Container* toolNulls[BUILDING_MAXTOOLS];	// (note: index five is never observed may be array size [5])
	/*068,4*/	Gods98::Container* depositNull;
	/*06c,4*/	Gods98::Container* entranceNull;
	/*070,4*/	uint32 carryNullFrames;		// (ae: CarryNullFrames)
	/*074,4*/	uint32 cameraNullFrames;	// (ae: CameraNullFrames)
	/*078,4*/	uint32 toolNullFrames;		// (ae: ToolNullFrames)
	/*07c,4*/	Gods98::Container* powerLevelScene;	// (ae: PowerLevelScene, LWS, true)
	/*080,4*/	real32 powerLevelTimer;
	/*084,4*/	Point2I* shapePoints;		// (ae: Shape) always Point2I[10]
	/*088,4*/	uint32 shapeCount;			// (ae: Shape)
	/*08c,14*/	UpgradesModel upgrades;
	/*0a0,a8*/	WeaponsModel weapons;
	/*148,4*/	BuildingFlags flags;
	/*14c*/
};
assert_sizeof(BuildingModel, 0x14c);

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// Merged function: Object_GetActivityContainer
// <LegoRR.exe @00406d60>
#define Building_GetActivityContainer ((Gods98::Container* (__cdecl* )(BuildingModel* building))0x00406d60)
//Gods98::Container* __cdecl Building_GetActivityContainer(BuildingModel* building);

// Merged function: Object_SearchForPartName
// <LegoRR.exe @00406e80>
#define Building_SearchForPartName ((Gods98::Container* (__cdecl* )(BuildingModel* building, const char* name, uint32 frameNo))0x00406e80)
//Gods98::Container* __cdecl Building_SearchForPartName(BuildingModel* building, const char* name, uint32 frameNo);

// Merged function: Object_SetOwnerObject
// <LegoRR.exe @004082b0>
#define Building_SetOwnerObject ((void (__cdecl* )(BuildingModel* building, LegoObject* liveObj))0x004082b0)
//void __cdecl Building_SetOwnerObject(BuildingModel* building, LegoObject* liveObj);

// Merged function: Object_IsHidden
// <LegoRR.exe @004085d0>
#define Building_IsHidden ((bool32 (__cdecl* )(BuildingModel* building))0x004085d0)
//bool32 __cdecl Building_IsHidden(BuildingModel* building);




// <LegoRR.exe @00407c90>
#define Building_Load ((bool32 (__cdecl* )(OUT BuildingModel* building, LegoObject_ID objID, Gods98::Container* root, const char* filename, const char* gameName))0x00407c90)
//bool32 __cdecl Building_Load(OUT BuildingModel* building, LegoObject_ID objID, Gods98::Container* root, const char* filename, const char* gameName);

// <LegoRR.exe @00408210>
#define Building_AnimatePowerLevelScene ((void (__cdecl* )(BuildingModel* building, bool32 forward))0x00408210)
//void __cdecl Building_AnimatePowerLevelScene(BuildingModel* building, bool32 forward);

// <LegoRR.exe @00408290>
#define Building_GetShapePoints ((const Point2I* (__cdecl* )(BuildingModel* building, OPTIONAL OUT uint32* shapeCount))0x00408290)
//Point2I* __cdecl Building_GetShapePoints(BuildingModel* building, OPTIONAL OUT uint32* shapeCount);

// <LegoRR.exe @004082d0>
#define Building_SetUpgradeActivity ((void (__cdecl* )(BuildingModel* building, const char* activityName))0x004082d0)
//void __cdecl Building_SetUpgradeActivity(BuildingModel* building, const char* activityName);

// <LegoRR.exe @004084a0>
#define Building_SetActivity ((bool32 (__cdecl* )(BuildingModel* building, const char* activityName, real32 elapsed))0x004084a0)
//bool32 __cdecl Building_SetActivity(BuildingModel* building, const char* activityName, real32 elapsed);

// <LegoRR.exe @00408520>
#define Building_GetCameraNull ((Gods98::Container* (__cdecl* )(BuildingModel* building, uint32 frameNo))0x00408520)
//Gods98::Container* __cdecl Building_GetCameraNull(BuildingModel* building, uint32 frameNo);

// <LegoRR.exe @00408550>
#define Building_Clone ((void (__cdecl* )(IN BuildingModel* srcBuilding, OUT BuildingModel* destBuilding))0x00408550)
//void __cdecl Building_Clone(IN BuildingModel* srcBuilding, OUT BuildingModel* destBuilding);

// <LegoRR.exe @004085a0>
#define Building_Hide ((void (__cdecl* )(BuildingModel* building, bool32 hide))0x004085a0)
//void __cdecl Building_Hide(BuildingModel* building, bool32 hide);


// <LegoRR.exe @004085f0>
#define Building_SetOrientation ((void (__cdecl* )(BuildingModel* building, real32 xDir, real32 yDir))0x004085f0)
// void __cdecl Building_SetOrientation(BuildingModel* building, real32 xDir, real32 yDir);

// <LegoRR.exe @00408640>
#define Building_SetPosition ((void (__cdecl* )(BuildingModel* building, real32 xPos, real32 yPos, GetWorldZCallback zCallback, Map3D* map))0x00408640)
//void __cdecl Building_SetPosition(BuildingModel* building, real32 xPos, real32 yPos, GetWorldZCallback zCallback, Map3D* map);

// <LegoRR.exe @004086a0>
#define Building_GetCarryNull ((Gods98::Container* (__cdecl* )(BuildingModel* building, uint32 frameNo))0x004086a0)
//Gods98::Container* __cdecl Building_GetCarryNull(BuildingModel* building, uint32 frameNo);

// <LegoRR.exe @004086e0>
#define Building_GetDepositNull ((Gods98::Container* (__cdecl* )(BuildingModel* building))0x004086e0)
//Gods98::Container* __cdecl Building_GetDepositNull(BuildingModel* building);

// <LegoRR.exe @00408710>
#define Building_GetEntranceNull ((Gods98::Container* (__cdecl* )(BuildingModel* building))0x00408710)
//Gods98::Container* __cdecl Building_GetEntranceNull(BuildingModel* building);

// <LegoRR.exe @00408740>
#define Building_GetToolNull ((Gods98::Container* (__cdecl* )(BuildingModel* building, uint32 frameNo))0x00408740)
//Gods98::Container* __cdecl Building_GetToolNull(BuildingModel* building, uint32 frameNo);

// <LegoRR.exe @00408780>
#define Building_GetCarryNullFrames ((uint32 (__cdecl* )(BuildingModel* building))0x00408780)
//uint32 __cdecl Building_GetCarryNullFrames(BuildingModel* building);

// <LegoRR.exe @00408790>
#define Building_MoveAnimation ((real32 (__cdecl* )(BuildingModel* building, real32 elapsed, uint32 unkFrameNo))0x00408790)
//real32 __cdecl Building_MoveAnimation(BuildingModel* building, real32 elapsed, uint32 unkFrameNo);

// Returns 0.0f
// <LegoRR.exe @00408860>
//#define Building_GetTransCoef ((real32 (__cdecl* )(BuildingModel* building))0x00408860)
__inline real32 __cdecl Building_GetTransCoef(BuildingModel* building) { return 0.0f; }

// <LegoRR.exe @00408870>
#define Building_Remove ((void (__cdecl* )(BuildingModel* building))0x00408870)
//void __cdecl Building_Remove(BuildingModel* building);

// When `current` is true, the function returns whether the object can currently get this upgrade.
//  (AKA, has the object not gotten the upgrade yet? And does it support the upgrade type?)
// Otherwise the function returns whether the object type supports the upgrade at all.
// <LegoRR.exe @004088a0>
#define Building_CanUpgradeType ((bool32 (__cdecl* )(BuildingModel* building, LegoObject_UpgradeType upgradeType, bool32 current))0x004088a0)
//bool32 __cdecl Building_CanUpgradeType(BuildingModel* building, LegoObject_UpgradeType upgradeType, bool32 current);

// <LegoRR.exe @004088d0>
#define Building_SetUpgradeLevel ((void (__cdecl* )(BuildingModel* building, uint32 objLevel))0x004088d0)
//void __cdecl Building_SetUpgradeLevel(BuildingModel* building, uint32 objLevel);

#pragma endregion

}
