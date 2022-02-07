// SelectPlace.h : 
//

#pragma once

#include "../GameCommon.h"


namespace LegoRR
{; // !<---

/**********************************************************************************
 ******** Constants
 **********************************************************************************/

#pragma region Constants

#define SELECTPLACE_MAXSHAPEPOINTS		20

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

struct SelectPlace // [LegoRR/SelectPlace.c|struct:0x8] Building selection highlight shown on map when placing
{
	/*0,4*/	Gods98::Container* contMesh;
	/*4,4*/	real32 tileDepth; // (init: 5.0) Z height of each coloured square when drawing the building placement grid.
	/*8*/
};
assert_sizeof(SelectPlace, 0x8);

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @005023c8>
extern Point2I (& s_TransformShapePoints)[SELECTPLACE_MAXSHAPEPOINTS];

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

// Prefer defining this in the cpp file. So that in the future, this include file can be made independent of `Containers.h`.
// <inlined, unused>
//__inline bool32 SelectPlace_IsHidden(const SelectPlace* selectPlace) { return Gods98::Container_IsHidden(selectPlace); }



// `tileDepth` is the Z height of each coloured square when drawing the building placement grid.
// <LegoRR.exe @004641c0>
#define SelectPlace_Create ((SelectPlace* (__cdecl* )(Gods98::Container* contRoot, real32 tileDepth))0x004641c0)
//SelectPlace* __cdecl SelectPlace_Create(Gods98::Container* contRoot, real32 tileDepth);

// Returns a temporary list of transformed points. This value is valid until the next function call.
// <LegoRR.exe @004643d0>
#define SelectPlace_TransformShapePoints ((const Point2I* (__cdecl* )(const Point2I* translation, const Point2I* shapePoints, sint32 shapeCount, Direction rotation))0x004643d0)
//const Point2I* __cdecl SelectPlace_TransformShapePoints(const Point2I* translation, const Point2I* shapePoints, sint32 shapeCount, Direction rotation);

// Checks the validity of a selection placement and updates the render state of the SelectPlace tiles.
// Returns the temporary result of SelectPlace_TransformShapePoints, or null.
// `waterEntrances` is a signed int, because a value of `-1` (or lower) is required to define the origin tile as a Yellow Path tile.
// <LegoRR.exe @00464480>
#define SelectPlace_CheckAndUpdate ((const Point2I* (__cdecl* )(SelectPlace* selectPlace, const Point2I* blockPos, const Point2I* shapePoints, uint32 shapeCount, Direction direction, Map3D* map, sint32 waterEntrances))0x00464480)
//const Point2I* __cdecl SelectPlace_CheckAndUpdate(SelectPlace* selectPlace, const Point2I* blockPos, const Point2I* shapePoints, uint32 shapeCount, Direction direction, Map3D* map, sint32 waterEntrances);

// <LegoRR.exe @004649e0>
#define SelectPlace_Hide ((void (__cdecl* )(SelectPlace* selectPlace, bool32 hide))0x004649e0)
//void __cdecl SelectPlace_Hide(SelectPlace* selectPlace, bool32 hide);

#pragma endregion

}
