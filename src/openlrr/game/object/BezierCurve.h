// BezierCurve.h : 
//

#pragma once

#include "../../common.h"
#include "../../engine/geometry.h"
#include "../../engine/undefined.h"


namespace LegoRR
{; // !<---

/**********************************************************************************
 ******** Constants
 **********************************************************************************/

#pragma region Constants

#define ROUTING_MAXPOINTS		50

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

struct RoutingBlock // [LegoRR/Routing.c|struct:0x14]
{
	/*00,8*/	Point2I	blockPos;
	/*08,8*/	Point2F	worldPos;
	/*10,1*/	uint8	flagsByte_10;
	/*11,1*/	uint8	byte_11;
	/*12,2*/	undefined field_0x12_0x13[2];
	/*14*/
};
assert_sizeof(RoutingBlock, 0x14);


struct RoutingData // [LegoRR/Routing.c|struct:0x25c]
{
	/*000,4*/	uint32	count;
	/*004,190*/	Point2F	points[ROUTING_MAXPOINTS];
	/*194,c8*/	real32	distances[ROUTING_MAXPOINTS];
	/*25c*/
};
assert_sizeof(RoutingData, 0x25c);

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @00406520>
void __cdecl Routing_Curve(OUT Point2F* r, const Point2F* p0, const Point2F* const p1, Point2F* const p2, Point2F* const p3, real32 t);

// sqrt(((a.x-b.x)*(a.x-b.x)) + ((a.y-b.y)*(a.y-b.y)))
// <LegoRR.exe @00406660>
real32 __cdecl Routing_Vector2DDistance(const Point2F* a, const Point2F* b);

// r = norm(r) * newLength
// NOTE: Unlike `Maths_Vector2DSetLength`, this function modifies the input point.
// <LegoRR.exe @00406690>
Point2F* __cdecl Routing_Vector2DChangeLength(IN OUT Point2F* r, real32 newLength);

// <LegoRR.exe @004066e0>
real32 __cdecl Routing_UpdateDistances(RoutingData* route);

// <LegoRR.exe @00406750>
void __cdecl Routing_BuildPoints(RoutingData* route, Point2F* p0, Point2F* p1, Point2F* p2, Point2F* p3, uint32 count);

// <LegoRR.exe @004067f0>
uint32 __cdecl Routing_Interpolate(RoutingData* route, real32 currentDist, OUT Point2F* r);

#pragma endregion

}
