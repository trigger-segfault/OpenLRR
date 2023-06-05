// Collision.h : 
//

#pragma once

#include "../GameCommon.h"


namespace LegoRR
{; // !<---

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @00408900>
//#define Collision_DistanceToLine ((real32 (__cdecl* )(const Point2F* point, const Point2F* start, const Point2F* end))0x00408900)
real32 __cdecl Collision_DistanceToLine(const Point2F* point, const Point2F* start, const Point2F* end);

// Does not return 0 when point is inside polygon, only the distance to the outline.
// <LegoRR.exe @00408a30>
//#define Collision_DistanceToPolyOutline ((real32 (__cdecl* )(const Point2F* point, const Point2F* fromList, const Point2F* toList, uint32 count))0x00408a30)
real32 __cdecl Collision_DistanceToPolyOutline(const Point2F* point, const Point2F* fromList, const Point2F* toList, uint32 count);

// Returns result
// <LegoRR.exe @00408a90>
//#define Collision_PointOnLine ((Point2F* (__cdecl* )(const Point2F* start, const Point2F* end, const Point2F* point, OUT Point2F* result))0x00408a90)
Point2F* __cdecl Collision_PointOnLine(const Point2F* start, const Point2F* end, const Point2F* point, OUT Point2F* result);

// Returns result
// <LegoRR.exe @00408b20>
//#define Collision_PointOnLineRay ((Point2F* (__cdecl* )(const Point2F* start, const Point2F* ray, const Point2F* point, OUT Point2F* result))0x00408b20)
Point2F* __cdecl Collision_PointOnLineRay(const Point2F* start, const Point2F* ray, const Point2F* point, OUT Point2F* result);

#pragma endregion

}
