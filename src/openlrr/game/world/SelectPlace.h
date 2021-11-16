// SelectPlace.h : 
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

struct SelectPlace // [LegoRR/SelectPlace.c|struct:0x8] Building selection highlight shown on map when placing
{
	/*0,4*/	Gods98::Container* contMesh;
	/*4,4*/	real32 floatValue; // (init: 5.0)
	/*8*/
};
assert_sizeof(SelectPlace, 0x8);

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

// <LegoRR.exe @004641c0>
#define SelectPlace_Create ((SelectPlace* (__cdecl* )(Gods98::Container* contRoot, real32 float5_0))0x004641c0)

// <LegoRR.exe @004643d0>
#define SelectPlace_TransformShapePoints ((const Point2I* (__cdecl* )(const Point2I* translation, const Point2I* shapePoints, sint32 shapeCount, Direction rotation))0x004643d0)

// <LegoRR.exe @00464480>
#define SelectPlace_DrawTiles ((uint32 (__cdecl* )(SelectPlace* selectPlace, const Point2I* blockPos, const Point2I* shapePoints, uint32 shapeCount, Direction direction, Map3D* surfMap, sint32 waterEntrances))0x00464480)

// <LegoRR.exe @004649e0>
#define SelectPlace_Hide ((void (__cdecl* )(SelectPlace* selectPlace, bool32 hide))0x004649e0)

#pragma endregion

}
