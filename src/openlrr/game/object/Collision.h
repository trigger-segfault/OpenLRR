// Collision.h : 
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

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals


#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @00408900>
#define Collision_Maths_Sub_FUN_00408900 ((real32 (__cdecl* )(const Point2F* param_1, const Point2F* param_2, const Point2F* param_3))0x00408900)

// <LegoRR.exe @00408a30>
#define Collision_Maths_FUN_00408a30 ((real32 (__cdecl* )(const Point2F* point, const Point2F* fromList, const Point2F* toList, uint32 count))0x00408a30)

// Returns result
// <LegoRR.exe @00408a90>
#define Collision_Maths_FUN_00408a90 ((Point2F* (__cdecl* )(const Point2F* param_1, const Point2F* param_2, const Point2F* position, OUT Point2F* result))0x00408a90)

// Returns result
// <LegoRR.exe @00408b20>
#define Collision_MathUnk_Vector2D_FUN_00408b20 ((Point2F* (__cdecl* )(const Point2F* param_1, const Point2F* param_2, const Point2F* position, OUT Point2F* result))0x00408b20)

#pragma endregion

}
