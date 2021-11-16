// Creature.h : 
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

struct LegoObject;
struct Map3D;
struct MeshLOD;

#pragma endregion

/**********************************************************************************
 ******** Constants
 **********************************************************************************/

#pragma region Constants

#define CREATURE_MAXCAMERAS			4

#pragma endregion

/**********************************************************************************
 ******** Enumerations
 **********************************************************************************/

#pragma region Enums

enum CreatureFlags : uint32
{
	CREATURE_FLAG_NONE   = 0,
	CREATURE_FLAG_SOURCE = 0x1, // Source object container data with original memory allocations.
};
flags_end(CreatureFlags, 0x4);

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

struct CreatureModel // [LegoRR/Creature.c|struct:0x74] Possibly alphabetically correct names: Bipedal, BasicUnit, etc... (has to be before Bubble, and possible after BezierCurve or AI(Task?))
{
	/*00,4*/	LegoObject_ID objID;
	/*04,4*/	Gods98::Container* contAct; // (ACT, true)
	/*08,18*/	undefined reserved1[0x18];
	/*20,4*/	char* cameraNullName;		// (ae: CameraNullName)
	/*24,4*/	uint32 cameraNullFrames;	// (ae: CameraNullFrames)
	/*28,4*/	BoolTri cameraFlipDir;		// (ae: CameraFlipDir)
	/*2c,4*/	char* drillNullName;		// (ae: DrillNullName)
	/*30,4*/	char* footStepNullName;		// (ae: FootStepNullName)
	/*34,4*/	char* carryNullName;		// (ae: CarryNullName)
	/*38,4*/	char* throwNullName;		// (ae: ThrowNullName)
	/*3c,4*/	char* depositNullName;		// (ae: DepositNullName)
	/*40,4*/	Gods98::Container* drillNull;
	/*44,4*/	Gods98::Container* footStepNull;	// (unused)
	/*48,4*/	Gods98::Container* carryNull;
	/*4c,4*/	Gods98::Container* throwNull;
	/*50,4*/	Gods98::Container* depositNull;
	/*54,10*/	Gods98::Container* cameraNulls[CREATURE_MAXCAMERAS];
	/*64,4*/	MeshLOD* polyMedium;		// (ae: MediumPoly)
	/*68,4*/	MeshLOD* polyHigh;			// (ae: HighPoly)
	/*6c,4*/	MeshLOD* polyFP;			// (ae: FPPoly::Camera#)
	/*70,4*/	CreatureFlags flags;
	/*74*/
};
assert_sizeof(CreatureModel, 0x74);

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// Merged function: Object_Hide
// <LegoRR.exe @00406bc0>
#define Creature_Hide ((void (__cdecl* )(CreatureModel* creature, bool32 hide))0x00406bc0)

// Merged function: Object_GetActivityContainer
// <LegoRR.exe @00406d60>
#define Creature_GetActivityContainer ((Gods98::Container* (__cdecl* )(CreatureModel* creature))0x00406d60)

// Merged function: Object_SearchForPartName
// <LegoRR.exe @00406e80>
#define Creature_SearchForPartName ((Gods98::Container* (__cdecl* )(CreatureModel* creature, const char* name, uint32 frameNo))0x00406e80)

// Merged function: Object_SetOwnerObject
// <LegoRR.exe @004082b0>
#define Creature_SetOwnerObject ((void (__cdecl* )(CreatureModel* creature, LegoObject* liveObj))0x004082b0)

// Merged function: Object_IsHidden
// <LegoRR.exe @004085d0>
#define Creature_IsHidden ((bool32 (__cdecl* )(CreatureModel* creature))0x004085d0)




// <LegoRR.exe @004068b0>
#define Creature_IsCameraFlipDir ((bool32 (__cdecl* )(CreatureModel* creature))0x004068b0)

// <LegoRR.exe @004068c0>
#define Creature_Load ((bool32 (__cdecl* )(CreatureModel* creature, LegoObject_ID objID, Gods98::Container* root, const char* filename, const char* gameName))0x004068c0)

// <LegoRR.exe @00406b30>
#define Creature_SwapPolyMedium ((void (__cdecl* )(CreatureModel* creature, bool32 swap))0x00406b30)

// <LegoRR.exe @00406b60>
#define Creature_SwapPolyHigh ((void (__cdecl* )(CreatureModel* creature, bool32 swap))0x00406b60)

// <LegoRR.exe @00406b90>
#define Creature_SwapPolyFP ((void (__cdecl* )(CreatureModel* creature, bool32 swap, uint32 cameraNo))0x00406b90)


// <LegoRR.exe @00406be0>
#define Creature_Clone ((void (__cdecl* )(IN CreatureModel* srcCreature, OUT CreatureModel* destCreature))0x00406be0)

// <LegoRR.exe @00406c40>
#define Creature_SetAnimationTime ((void (__cdecl* )(CreatureModel* creature, real32 time))0x00406c40)

// <LegoRR.exe @00406c60>
#define Creature_MoveAnimation ((real32 (__cdecl* )(CreatureModel* creature, real32 elapsed, uint32 unkFrameNo))0x00406c60)

// <LegoRR.exe @00406cd0>
#define Creature_GetAnimationTime ((real32 (__cdecl* )(CreatureModel* creature))0x00406cd0)

// <LegoRR.exe @00406cf0>
#define Creature_SetOrientation ((void (__cdecl* )(CreatureModel* creature, real32 xDir, real32 yDir))0x00406cf0)

// <LegoRR.exe @00406d20>
#define Creature_SetPosition ((void (__cdecl* )(CreatureModel* creature, real32 xPos, real32 yPos, GetWorldZCallback zCallback, Map3D* map))0x00406d20)


// <LegoRR.exe @00406d70>
#define Creature_SetActivity ((bool32 (__cdecl* )(CreatureModel* creature, const char* activityName, real32 elapsed))0x00406d70)

// <LegoRR.exe @00406df0>
#define Creature_Remove ((void (__cdecl* )(CreatureModel* creature))0x00406df0)


// <LegoRR.exe @00406eb0>
#define Creature_GetCameraNull ((Gods98::Container* (__cdecl* )(CreatureModel* creature, uint32 frameNo))0x00406eb0)

// <LegoRR.exe @00406ee0>
#define Creature_GetDrillNull ((Gods98::Container* (__cdecl* )(CreatureModel* creature))0x00406ee0)

// <LegoRR.exe @00406f10>
#define Creature_GetCarryNull ((Gods98::Container* (__cdecl* )(CreatureModel* creature))0x00406f10)

// <LegoRR.exe @00406f40>
#define Creature_GetDepositNull ((Gods98::Container* (__cdecl* )(CreatureModel* creature))0x00406f40)

// <LegoRR.exe @00406f70>
#define Creature_GetThrowNull ((bool32 (__cdecl* )(CreatureModel* creature))0x00406f70)

// <LegoRR.exe @00406fc0>
#define Creature_GetTransCoef ((real32 (__cdecl* )(CreatureModel* creature))0x00406fc0)

#pragma endregion

}
