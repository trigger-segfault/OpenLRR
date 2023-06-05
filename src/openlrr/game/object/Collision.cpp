// Collision.cpp : 
//

#include "../../engine/core/Errors.h"
#include "../../engine/core/Maths.h"

#include "Collision.h"


/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @00408900>
real32 __cdecl LegoRR::Collision_DistanceToLine(const Point2F* point, const Point2F* start, const Point2F* end)
{
	// Vector between two endpoints(?)
	Point2F ray;
	Gods98::Maths_Vector2DSubtract(&ray, end, start);
	Gods98::Maths_Vector2DNormalize(&ray);

	Point2F vector1;
	Gods98::Maths_Vector2DSubtract(&vector1, point, end);
	real32 length = Gods98::Maths_Vector2DModulus(&vector1);
	Gods98::Maths_Vector2DScale(&vector1, &vector1, 1.0f / length); // Normalize without recalculating the length.

	real32 dot = Gods98::Maths_Vector2DDotProduct(&vector1, &ray);
	if (dot < 0.0f) {

		Point2F vector2;
		Gods98::Maths_Vector2DSubtract(&vector2, point, start);
		length = Gods98::Maths_Vector2DModulus(&vector2);
		Gods98::Maths_Vector2DScale(&vector2, &vector2, 1.0f / length); // Normalize without recalculating the length.

		dot = Gods98::Maths_Vector2DDotProduct(&vector2, &ray);
		if (dot > 0.0f) {
			// The closest point on the line is an endpoint.
			length *= std::sin(std::acos(/*(long double)*/dot)); // Maybe keep long double cast for more-accurate calculations?
		}
	}
	return length;
}

// <LegoRR.exe @00408a30>
real32 __cdecl LegoRR::Collision_DistanceToPolyOutline(const Point2F* point, const Point2F* fromList, const Point2F* toList, uint32 count)
{
	Error_Fatal((count == 0), "Collision_DistanceToPolyOutline: Point list is empty.");

	real32 minValue = Collision_DistanceToLine(point, &fromList[0], &toList[0]);

	for (uint32 i = 1; i < count; i++) {
		const real32 value = Collision_DistanceToLine(point, &fromList[i], &toList[i]);
		if (value < minValue) {
			minValue = value;
		}
	}
	return minValue;
}

// Returns result
// <LegoRR.exe @00408a90>
Point2F* __cdecl LegoRR::Collision_PointOnLine(const Point2F* start, const Point2F* end, const Point2F* point, OUT Point2F* result)
{
	/// REFACTOR: This function is identical to Collision_PointOnLineRay, with the exception of the new subtraction with param start.
	///           Just call the other function after subtracting.
	Point2F ray;
	Gods98::Maths_Vector2DSubtract(&ray, end, start);
	return Collision_PointOnLineRay(start, &ray, point, result);
}

// Returns result
// <LegoRR.exe @00408b20>
Point2F* __cdecl LegoRR::Collision_PointOnLineRay(const Point2F* start, const Point2F* ray, const Point2F* point, OUT Point2F* result)
{
	Point2F norm = *ray;
	Gods98::Maths_Vector2DNormalize(&norm);

	Point2F diff;
	Gods98::Maths_Vector2DSubtract(&diff, point, start);

	const real32 dot = Gods98::Maths_Vector2DDotProduct(&diff, &norm);

	// Don't use result in-place of temp in-case result is also param start.
	Point2F temp;
	Gods98::Maths_Vector2DScale(&temp, &norm, dot);
	Gods98::Maths_Vector2DAdd(result, start, &temp);
	return result;
}

#pragma endregion
