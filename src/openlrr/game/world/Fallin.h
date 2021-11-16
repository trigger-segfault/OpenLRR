// Fallin.h : 
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

struct Fallin_Globs // [LegoRR/Fallin.c|struct:0x4|tags:GLOBS] Just a single field for Fallins (most other settings are found in Lego_Globs)
{
	/*0,4*/	uint32 NumberOfLandSlidesTillCaveIn;
	/*4*/
};
assert_sizeof(Fallin_Globs, 0x4);

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// Single field: fallinGlobs_NumberOfLandSlidesTillCaveIn
// <LegoRR.exe @004a2ee4>
extern Fallin_Globs & fallinGlobs;

// Static function variable
// <LegoRR.exe @004d88c4>
extern real32 & s_Fallin_ElapsedTimer;

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @0040f010>
#define Fallin_Update ((uint32 (__cdecl* )(real32 elapsedGame))0x0040f010)

// <LegoRR.exe @0040f0c0>
#define Fallin_Block_FUN_0040f0c0 ((bool32 (__cdecl* )(const Point2I* blockPos, bool32 param_2))0x0040f0c0)

// <LegoRR.exe @0040f1e0>
#define Fallin_Block_UpdateFallinsUnk1 ((bool32 (__cdecl* )(const Point2I* blockPos))0x0040f1e0)

// <LegoRR.exe @0040f260>
#define Fallin_Block_FUN_0040f260 ((void (__cdecl* )(const Point2I* blockPos, sint32 unkIntensity, bool32 param_3))0x0040f260)

// <LegoRR.exe @0040f510>
#define Fallin_Initialise ((void (__cdecl* )(sint32 numLandSlidesTillCaveIn))0x0040f510)

// <LegoRR.exe @0040f520>
#define Fallin_LandSlideDoCaveIn ((void (__cdecl* )(const Point2I* blockPos, sint32 unkIntensity))0x0040f520)

#pragma endregion

}
