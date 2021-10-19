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

#define BEZIERCURVE_MAXPOINTS		50

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

struct BezierCurve // [LegoRR/Routing.c|struct:0x25c]
{
	/*000,4*/	uint32	count;
	/*004,190*/	Point2F	points[BEZIERCURVE_MAXPOINTS];
	/*194,c8*/	real32	distances[BEZIERCURVE_MAXPOINTS];
	/*25c*/
};
assert_sizeof(BezierCurve, 0x25c);

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @00406520>
void __cdecl BezierCurve_Curve(OUT Point2F* r, const Point2F* p0, const Point2F* p1, const Point2F* p2, const Point2F* p3, real32 t);

// sqrt(((a.x-b.x)*(a.x-b.x)) + ((a.y-b.y)*(a.y-b.y)))
// <LegoRR.exe @00406660>
real32 __cdecl BezierCurve_Vector2DDistance(const Point2F* a, const Point2F* b);

// r = norm(r) * newLength
// NOTE: Unlike `Maths_Vector2DSetLength`, this function modifies the input point.
// <LegoRR.exe @00406690>
Point2F* __cdecl BezierCurve_Vector2DChangeLength(IN OUT Point2F* r, real32 newLength);

// <LegoRR.exe @004066e0>
real32 __cdecl BezierCurve_UpdateDistances(BezierCurve* curve);

// <LegoRR.exe @00406750>
void __cdecl BezierCurve_BuildPoints(BezierCurve* curve, const Point2F* p0, const Point2F* p1, const Point2F* p2, const Point2F* p3, uint32 count);

// <LegoRR.exe @004067f0>
uint32 __cdecl BezierCurve_Interpolate(const BezierCurve* curve, real32 currentDist, OUT Point2F* r);

#pragma endregion

}
