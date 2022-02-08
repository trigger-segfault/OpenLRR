// Creature.h : 
//

#pragma once

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
//void __cdecl Creature_Hide(CreatureModel* creature, bool32 hide);

// Merged function: Object_GetActivityContainer
// <LegoRR.exe @00406d60>
#define Creature_GetActivityContainer ((Gods98::Container* (__cdecl* )(CreatureModel* creature))0x00406d60)
//Gods98::Container* __cdecl Upgrade_Part_GetActivityContainer(CreatureModel* creature);

// Merged function: Object_SearchForPartName
// <LegoRR.exe @00406e80>
#define Creature_SearchForPartName ((Gods98::Container* (__cdecl* )(CreatureModel* creature, const char* name, uint32 frameNo))0x00406e80)
//Gods98::Container* __cdecl Creature_SearchForPartName(CreatureModel* creature, const char* name, uint32 frameNo);

// Merged function: Object_SetOwnerObject
// <LegoRR.exe @004082b0>
#define Creature_SetOwnerObject ((void (__cdecl* )(CreatureModel* creature, LegoObject* liveObj))0x004082b0)
//void __cdecl Creature_SetOwnerObject(CreatureModel* creature, LegoObject* liveObj);

// Merged function: Object_IsHidden
// <LegoRR.exe @004085d0>
#define Creature_IsHidden ((bool32 (__cdecl* )(CreatureModel* creature))0x004085d0)
//bool32 __cdecl Creature_IsHidden(CreatureModel* creature);




// <LegoRR.exe @004068b0>
//#define Creature_IsCameraFlipDir ((bool32 (__cdecl* )(CreatureModel* creature))0x004068b0)
__inline bool32 __cdecl Creature_IsCameraFlipDir(CreatureModel* creature) { return creature->cameraFlipDir == BoolTri::BOOL3_TRUE; }

// <LegoRR.exe @004068c0>
#define Creature_Load ((bool32 (__cdecl* )(OUT CreatureModel* creature, LegoObject_ID objID, Gods98::Container* root, const char* filename, const char* gameName))0x004068c0)
//bool32 __cdecl Creature_Load(OUT CreatureModel* creature, LegoObject_ID objID, Gods98::Container* root, const char* filename, const char* gameName);

// <LegoRR.exe @00406b30>
#define Creature_SwapPolyMedium ((void (__cdecl* )(CreatureModel* creature, bool32 swap))0x00406b30)
//void __cdecl Creature_SwapPolyMedium(CreatureModel* creature, bool32 swap);

// <LegoRR.exe @00406b60>
#define Creature_SwapPolyHigh ((void (__cdecl* )(CreatureModel* creature, bool32 swap))0x00406b60)
//void __cdecl Creature_SwapPolyHigh(CreatureModel* creature, bool32 swap);

// <LegoRR.exe @00406b90>
#define Creature_SwapPolyFP ((void (__cdecl* )(CreatureModel* creature, bool32 swap, uint32 cameraNo))0x00406b90)
//void __cdecl Creature_SwapPolyFP(CreatureModel* creature, bool32 swap, sint32 cameraNo);


// <LegoRR.exe @00406be0>
#define Creature_Clone ((void (__cdecl* )(IN CreatureModel* srcCreature, OUT CreatureModel* destCreature))0x00406be0)
//void __cdecl Creature_Clone(IN CreatureModel* srcCreature, OUT CreatureModel* destCreature);

// <LegoRR.exe @00406c40>
#define Creature_SetAnimationTime ((void (__cdecl* )(CreatureModel* creature, real32 time))0x00406c40)
//void __cdecl Creature_SetAnimationTime(CreatureModel* creature, real32 time);

// <LegoRR.exe @00406c60>
#define Creature_MoveAnimation ((real32 (__cdecl* )(CreatureModel* creature, real32 elapsed, uint32 unkFrameNo))0x00406c60)
//real32 __cdecl Creature_MoveAnimation(CreatureModel* creature, real32 elapsed, uint32 unkFrameNo);

// <LegoRR.exe @00406cd0>
#define Creature_GetAnimationTime ((real32 (__cdecl* )(CreatureModel* creature))0x00406cd0)
//real32 __cdecl Creature_GetAnimationTime(CreatureModel* creature);

// <LegoRR.exe @00406cf0>
#define Creature_SetOrientation ((void (__cdecl* )(CreatureModel* creature, real32 xDir, real32 yDir))0x00406cf0)
//void __cdecl Creature_SetOrientation(CreatureModel* creature, real32 xDir, real32 yDir);

// <LegoRR.exe @00406d20>
#define Creature_SetPosition ((void (__cdecl* )(CreatureModel* creature, real32 xPos, real32 yPos, GetWorldZCallback zCallback, Map3D* map))0x00406d20)
//void __cdecl Creature_SetPosition(CreatureModel* creature, real32 xPos, real32 yPos, GetWorldZCallback zCallback, Map3D* map);


// <LegoRR.exe @00406d70>
#define Creature_SetActivity ((bool32 (__cdecl* )(CreatureModel* creature, const char* activityName, real32 elapsed))0x00406d70)
//bool32 __cdecl Creature_SetActivity(CreatureModel* creature, const char* activityName, real32 elapsed);

// <LegoRR.exe @00406df0>
#define Creature_Remove ((void (__cdecl* )(CreatureModel* creature))0x00406df0)
//void __cdecl Creature_Remove(CreatureModel* creature);


// <LegoRR.exe @00406eb0>
#define Creature_GetCameraNull ((Gods98::Container* (__cdecl* )(CreatureModel* creature, uint32 frameNo))0x00406eb0)
//Gods98::Container* __cdecl Creature_GetCameraNull(CreatureModel* creature, uint32 frameNo);

// <LegoRR.exe @00406ee0>
#define Creature_GetDrillNull ((Gods98::Container* (__cdecl* )(CreatureModel* creature))0x00406ee0)
//Gods98::Container* __cdecl Creature_GetDrillNull(CreatureModel* creature);

// <LegoRR.exe @00406f10>
#define Creature_GetCarryNull ((Gods98::Container* (__cdecl* )(CreatureModel* creature))0x00406f10)
//Gods98::Container* __cdecl Creature_GetCarryNull(CreatureModel* creature);

// <LegoRR.exe @00406f40>
#define Creature_GetDepositNull ((Gods98::Container* (__cdecl* )(CreatureModel* creature))0x00406f40)
//Gods98::Container* __cdecl Creature_GetDepositNull(CreatureModel* creature);

// Checks if the model has a throw null, and performs initialisation for the null if not found.
// 
// A more correct name would be Creature_HasThrowNull, but the fact that this performs initialisation is still awkward to convey.
// Alt name: Creature_CheckThrowNull
// <LegoRR.exe @00406f70>
#define Creature_GetThrowNull ((bool32 (__cdecl* )(CreatureModel* creature))0x00406f70)

// <LegoRR.exe @00406fc0>
#define Creature_GetTransCoef ((real32 (__cdecl* )(CreatureModel* creature))0x00406fc0)
//real32 __cdecl Creature_GetTransCoef(CreatureModel* creature);

#pragma endregion

}
