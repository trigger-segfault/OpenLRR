// Erode.h : 
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

struct Erode_Globs // [LegoRR/Erode.c|struct:0xfa14|tags:GLOBS]
{
	/*0000,3e80*/	Point2I UnkBlocksList[2000];
	/*3e80,4*/	uint32 UnkBlocksCount;
	/*3e84,3e80*/	Point2I activeBlocks[2000];
	/*7d04,1f40*/	bool32 activeStates[2000];
	/*9c44,1f40*/	real32 activeTimers[2000]; // (countdown timers)
	/*bb84,1f40*/	Point2I lockedBlocks[1000];
	/*dac4,fa0*/	real32 lockedTimers[1000]; // (countdown timers)
	/*ea64,fa0*/	bool32 lockedStates[1000];
	/*fa04,4*/	real32 elapsedTimer; // (count-up elapsed timer)
	/*fa08,4*/	real32 ErodeTriggerTime; // (init: Lego.cfg)
	/*fa0c,4*/	real32 ErodeErodeTime; // (init: Lego.cfg)
	/*fa10,4*/	real32 ErodeLockTime; // (init: Lego.cfg)
	/*fa14*/
};
assert_sizeof(Erode_Globs, 0xfa14);

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @004c8eb0>
extern Erode_Globs & erodeGlobs;

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @0040e860>
#define Erode_Initialise ((void (__cdecl* )(real32 triggerTime, real32 erodeTime, real32 lockTime))0x0040e860)

// <LegoRR.exe @0040e8c0>
#define Erode_GetFreeActiveIndex ((bool32 (__cdecl* )(OUT sint32* index))0x0040e8c0)

// <LegoRR.exe @0040e8f0>
#define Erode_GetBlockErodeRate ((real32 (__cdecl* )(const Point2I* blockPos))0x0040e8f0)

// <LegoRR.exe @0040e940>
#define Erode_AddActiveBlock ((void (__cdecl* )(const Point2I* blockPos, sint32 unkModulusNum))0x0040e940)

// <LegoRR.exe @0040e9e0>
#define Erode_Update ((void (__cdecl* )(real32 elapsedGame))0x0040e9e0)

// <LegoRR.exe @0040ed30>
#define Erode_AddLockedBlock ((void (__cdecl* )(const Point2I* blockPos))0x0040ed30)

// <LegoRR.exe @0040ed80>
#define Erode_Block_FUN_0040ed80 ((void (__cdecl* )(const Point2I* blockPos, bool32 doState2_else_add3))0x0040ed80)

// <LegoRR.exe @0040eee0>
#define Erode_IsBlockLocked ((bool32 (__cdecl* )(const Point2I* blockPos))0x0040eee0)

// <LegoRR.exe @0040ef30>
#define Erode_FindAdjacentBlockPos ((bool32 (__cdecl* )(const Point2I* blockPos, OUT Point2I* adjacentblockPos))0x0040ef30)

#pragma endregion

}
