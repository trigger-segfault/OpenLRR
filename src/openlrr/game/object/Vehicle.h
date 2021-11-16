// Vehicle.h : 
//

#pragma once

#include "../../common.h"
#include "../../engine/colour.h"
#include "../../engine/geometry.h"
#include "../../engine/undefined.h"

#include "../../engine/gfx/Containers.h"
#include "../../engine/drawing/Images.h"

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
struct MeshLOD;

#pragma endregion

/**********************************************************************************
 ******** Constants
 **********************************************************************************/

#pragma region Constants

#define VEHICLE_MAXCAMERAS			4
#define VEHICLE_MAXCARRYS			7

#define VEHICLE_MAXWHEELS			6

#pragma endregion

/**********************************************************************************
 ******** Enumerations
 **********************************************************************************/

#pragma region Enums


enum VehicleFlags : uint32
{
	VEHICLE_FLAG_NONE        = 0,
	VEHICLE_FLAG_SOURCE      = 0x1, // Source object container data with original memory allocations.
	VEHICLE_FLAG_HIDDEN      = 0x2, // Vehicles need to track this manually due to having 1-2 sub-containers.
	VEHICLE_FLAG_NOACTIVITY1 = 0x4, // Tells the model to not animate contAct1.
	                                // Set when an assigned activity is not set on contAct1 AND HOLDMISSING is true.
	VEHICLE_FLAG_HOLDMISSING = 0x8, // (ae: HoldMissing TRUE)
};
flags_end(VehicleFlags, 0x4);

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

struct VehicleModel // [LegoRR/Vehicle.c|struct:0x1ec]
{
	/*000,4*/	LegoObject_ID objID;
	/*004,4*/	char* wheelNullName;		// (ae: WheelNullName)
	/*008,4*/	Gods98::Container* contAct1;		// (ACT, true)
	/*00c,4*/	Gods98::Container* contAct2;		// (ACT, true) Optional second ae file (seen for Grannit Grinder legs) This container has priority for finding null frames
	/*010,18*/	Gods98::Container* wheelNulls[VEHICLE_MAXWHEELS];	// (ae:WheelMesh, LWO, false) Table for wheel nulls that are assigned a position calculated by wheelRefNulls
	/*028,18*/	Gods98::Container* wheelRefNulls[VEHICLE_MAXWHEELS];// Root wheel nulls that are used to calculate terrain-relative positioning
	/*040,4*/	uint32 wheelNullFrames;
	/*044,18*/	undefined reserved1[0x18];
	/*05c,c*/	Vector3F wheelVector_5c;	// another vector used in wheelNulls positioning
	/*068,4*/	real32 wheelRadius;			// (ae: WheelRadius)
	/*06c,48*/	Vector3F wheelNullPositions[VEHICLE_MAXWHEELS];		// Live position of wheelNulls
	/*0b4,4*/	char* drillNullName;		// (ae: DrillNullName)
	/*0b8,4*/	char* depositNullName;		// (ae: DepositNullName)
	/*0bc,4*/	char* fireNullName;			// (ae: FireNullName) "fire laser"
	/*0c0,4*/	char* driverNullName;		// (ae: DriverNullName) 
	/*0c4,4*/	char* yPivot;				// (ae: xPivot)
	/*0c8,4*/	char* xPivot;				// (ae: yPivot)
	/*0cc,4*/	Gods98::Container* drillNull;
	/*0d0,4*/	Gods98::Container* depositNull;
	/*0d4,4*/	Gods98::Container* driverNull;
	/*0d8,4*/	BoolTri cameraFlipDir;		// (ae: CameraFlipDir)
	/*0dc,4*/	char* carryNullName;		// (ae: CarryNullName)
	/*0e0,4*/	char* cameraNullName;		// (ae: CameraNullName)
	/*0e4,1c*/	Gods98::Container* carryNulls[VEHICLE_MAXCARRYS];
	/*100,c*/	undefined reserved2[0xc];
	/*10c,10*/	Gods98::Container* cameraNulls[VEHICLE_MAXCAMERAS];
	/*11c,4*/	uint32 carryNullFrames;		// (ae: CarryNullFrames)
	/*120,4*/	uint32 cameraNullFrames;	// (ae: CameraNullFrames)
	/*124,14*/	UpgradesModel upgrades;
	/*138,a8*/	WeaponsModel weapons;
	/*1e0,4*/	MeshLOD* polyMedium1;
	/*1e4,4*/	MeshLOD* polyMedium2;
	/*1e8,4*/	VehicleFlags flags;
	/*1ec*/
};
assert_sizeof(VehicleModel, 0x1ec);

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @0046c690>
#define Vehicle_IsCameraFlipDir ((bool32 (__cdecl* )(VehicleModel* vehicle))0x0046c690)

// <LegoRR.exe @0046c6b0>
#define Vehicle_SetActivity ((bool32 (__cdecl* )(VehicleModel* vehicle, const char* activityName, real32 elapsed))0x0046c6b0)

// <LegoRR.exe @0046c7d0>
#define Vehicle_SetUpgradeActivity ((void (__cdecl* )(VehicleModel* vehicle, const char* activityName))0x0046c7d0)

// <LegoRR.exe @0046c9b0>
#define Vehicle_Load ((bool32 (__cdecl* )(VehicleModel* vehicle, LegoObject_ID objID, Gods98::Container* root, const char* filename, const char* gameName))0x0046c9b0)

// Similar to `Vehicle_Load`, this does not free the passed pointer.
// <LegoRR.exe @0046d0d0>
#define Vehicle_Remove ((void (__cdecl* )(VehicleModel* vehicle))0x0046d0d0)

// <LegoRR.exe @0046d190>
#define Vehicle_SwapPolyMedium ((void (__cdecl* )(VehicleModel* vehicle, bool32 swap))0x0046d190)

// There's no stored HighPoly data, this just calls `Vehicle_SwapPolyMedium`.
// <LegoRR.exe @0046d1e0>
#define Vehicle_SwapPolyHigh ((void (__cdecl* )(VehicleModel* vehicle, bool32 swap))0x0046d1e0)

// When `current` is true, the function returns whether the object can currently get this upgrade.
//  (AKA, has the object not gotten the upgrade yet? And does it support the upgrade type?)
// Otherwise the function returns whether the object type supports the upgrade at all.
// <LegoRR.exe @0046d200>
#define Vehicle_CanUpgradeType ((bool32 (__cdecl* )(VehicleModel* vehicle, LegoObject_UpgradeType upgradeType, bool32 current))0x0046d200)

// <LegoRR.exe @0046d240>
#define Vehicle_GetUpgradeLevel ((uint32 (__cdecl* )(VehicleModel* vehicle))0x0046d240)

// <LegoRR.exe @0046d250>
#define Vehicle_SetUpgradeLevel ((void (__cdecl* )(VehicleModel* vehicle, uint32 objLevel))0x0046d250)

// <LegoRR.exe @0046d280>
#define Vehicle_HideWheels ((void (__cdecl* )(VehicleModel* vehicle, bool32 hide))0x0046d280)

// <LegoRR.exe @0046d2b0>
#define Vehicle_Clone ((bool32 (__cdecl* )(IN VehicleModel* srcVehicle, OUT VehicleModel* destVehicle))0x0046d2b0)

// <LegoRR.exe @0046d400>
#define Vehicle_SetOwnerObject ((void (__cdecl* )(VehicleModel* vehicle, LegoObject* liveObj))0x0046d400)

// <LegoRR.exe @0046d460>
#define Vehicle_GetAnimationTime ((real32 (__cdecl* )(VehicleModel* vehicle))0x0046d460)

// <LegoRR.exe @0046d480>
#define Vehicle_MoveAnimation ((real32 (__cdecl* )(VehicleModel* vehicle, real32 elapsed1, real32 elapsed2, uint32 unkFrameNo))0x0046d480)

// <LegoRR.exe @0046d520>
#define Vehicle_PopulateWheels ((void (__cdecl* )(VehicleModel* vehicle))0x0046d520)

// <LegoRR.exe @0046d580>
#define Vehicle_Hide ((void (__cdecl* )(VehicleModel* vehicle, bool32 hide))0x0046d580)

// <LegoRR.exe @0046d5f0>
#define Vehicle_IsHidden ((bool32 (__cdecl* )(VehicleModel* vehicle))0x0046d5f0)

// <LegoRR.exe @0046d610>
#define Vehicle_SetOrientation ((void (__cdecl* )(VehicleModel* vehicle, real32 xDir, real32 yDir, real32 zDir))0x0046d610)

// <LegoRR.exe @0046d640>
#define Vehicle_SetPosition ((void (__cdecl* )(VehicleModel* vehicle, real32 xPos, real32 yPos, GetWorldZCallback zCallback, Map3D* map))0x0046d640)

// <LegoRR.exe @0046dca0>
#define Vehicle_GetActivityContainer ((Gods98::Container* (__cdecl* )(VehicleModel* vehicle))0x0046dca0)

// <LegoRR.exe @0046dcb0>
#define Vehicle_GetNull ((Gods98::Container* (__cdecl* )(VehicleModel* vehicle, const char* nullName, uint32 nullFrameNo))0x0046dcb0)

// <LegoRR.exe @0046dd10>
#define Vehicle_GetCameraNull ((Gods98::Container* (__cdecl* )(VehicleModel* vehicle, uint32 frameNo))0x0046dd10)

// <LegoRR.exe @0046dd50>
#define Vehicle_GetDrillNull ((Gods98::Container* (__cdecl* )(VehicleModel* vehicle))0x0046dd50)

// <LegoRR.exe @0046dd80>
#define Vehicle_GetDepositNull ((Gods98::Container* (__cdecl* )(VehicleModel* vehicle))0x0046dd80)

// <LegoRR.exe @0046ddb0>
#define Vehicle_GetDriverNull ((Gods98::Container* (__cdecl* )(VehicleModel* vehicle))0x0046ddb0)

// <LegoRR.exe @0046dde0>
#define Vehicle_GetCarryNull ((Gods98::Container* (__cdecl* )(VehicleModel* vehicle, uint32 frameNo))0x0046dde0)

// <LegoRR.exe @0046de20>
#define Vehicle_GetCarryNullFrames ((uint32 (__cdecl* )(VehicleModel* vehicle))0x0046de20)

// <LegoRR.exe @0046de30>
#define Vehicle_GetTransCoef ((real32 (__cdecl* )(VehicleModel* vehicle))0x0046de30)

#pragma endregion

}
