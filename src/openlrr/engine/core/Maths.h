// Maths.h : 
//
/// APIS: -
/// DEPENDENCIES: -
/// DEPENDENTS: Draw, Lws, Mesh, AITask, DynamicPM, ElectricFence, Erode, Fallin,
///             Flocks, Lego, LegoCamera, LegoObject, LightEffects, Map3D, RadarMap,
///             Roof, Smoke, Vehicle

#pragma once

#include "../../common.h"
#include "../geometry.h"


/**********************************************************************************
 ******** Undefines
 **********************************************************************************/

#pragma region Undefines

#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

#pragma endregion


namespace Gods98
{; // !<---

/**********************************************************************************
 ******** Forward Declarations
 **********************************************************************************/

#pragma region Forward Declarations

// forward-declared here to make the inline call `Maths_Vector2DRandom` cleaner.
real32 __cdecl Maths_RandRange(real32 low, real32 high);

#pragma endregion

/**********************************************************************************
 ******** Constants
 **********************************************************************************/

#pragma region Constants

#define M_PI			3.14159265358979323846f
#define M_EPSILON		1.0e-5f                 // Tolerance for REALs

#pragma endregion

/**********************************************************************************
 ******** Macros
 **********************************************************************************/

#pragma region Macros

#define Colour_RGB(r,g,b)		(0xff000000L|(((sint32)((r)*255))<<16)|(((sint32)((g)*255))<<8)|(sint32)((b)*255))
#define Colour_RGBA(r,g,b,a)	((((sint32)((a)*255))<<24)|(((sint32)((r)*255))<<16)|(((sint32)((g) * 255))<<8)|(sint32)((b)*255))

// British spelling rename defines included in Gods98 (unless "Normalise" is the English U.S. spelling...).
#define Maths_Vector3DNormalise(r)			Maths_Vector3DNormalize((r))
#define Maths_Vector2DNormalise(r)			Maths_Vector2DNormalize((r))
// Both Length and Modulus are the same behavior.
#define Maths_Vector3DLength(v)				Maths_Vector3DModulus((v))
#define Maths_Vector2DLength(v)				Maths_Vector2DModulus((v))

#define Maths_Cos(a)						(real32)std::cos((double)(a))
#define Maths_Sin(a)						(real32)std::sin((double)(a))
#define Maths_Tan(a)						(real32)std::tan((double)(a))
#define Maths_ACos(a)						(real32)std::acos((double)(a))
#define Maths_ASin(a)						(real32)std::asin((double)(a))
#define Maths_ATan(a)						(real32)std::atan((double)(a))

#pragma endregion

/**********************************************************************************
 ******** Vector and Misc Functions
 **********************************************************************************/

#pragma region Functions

// <inlined>
__inline real32 Maths_InterpolationDelta(real32 start, real32 end, real32 current) { return (current - start) / (end - start); }

// <inlined>
__inline real32 Maths_Interpolate(real32 start, real32 end, real32 delta) { return start + ((end - start) * delta); }

// <inlined>
__inline Vector3F* Maths_Vector2DMake3D(OUT Vector3F* r, const Point2F* v) { r->x = v->x; r->y = v->y; r->z = 0.0f; return r; }

// <inlined>
__inline Point2F* Maths_Vector3DMake2D(OUT Point2F* r, const Vector3F* v) { r->x = v->x; r->y = v->y; return r; }

// <inlined>
__inline real32 Maths_Vector2DModulus(const Point2F* v) { return (real32)std::sqrt((v->x*v->x)+(v->y*v->y)); }


// <LegoRR.exe @00401240>
real32 __cdecl noinline(Maths_Vector2DDistance)(const Point2F* a, const Point2F* b);
__inline real32 Maths_Vector2DDistance(const Point2F* a, const Point2F* b) { return (real32)std::sqrt(((a->x-b->x)*(a->x-b->x))+((a->y-b->y)*(a->y-b->y))); }


// <inlined>
__inline real32 Maths_Vector2DDotProduct(const Point2F* a, const Point2F* b) { return ((a->x*b->x) + (a->y*b->y)); }

// <inlined>
__inline Point2F* Maths_Vector2DSubtract(OUT Point2F* r, const Point2F* a, const Point2F* b) { r->x = a->x-b->x; r->y = a->y-b->y; return r; }

// <inlined>
__inline Point2F* Maths_Vector2DAdd(OUT Point2F* r, const Point2F* a, const Point2F* b) { r->x = a->x+b->x; r->y = a->y+b->y; return r; }

// <inlined>
__inline Point2F* Maths_Vector2DScale(OUT Point2F* r, const Point2F* a, real32 f) { r->x = a->x*f; r->y = a->y*f; return r; }

// <inlined>
__inline Point2F* Maths_Vector2DNormalize(OUT Point2F* r) { Maths_Vector2DScale(r, r, 1.0f/Maths_Vector2DLength(r)); return r; }

// <inlined>
__inline Point2F* Maths_Vector2DSetLength(OUT Point2F* r, const Point2F* a, real32 l) { real32 m = Maths_Vector2DModulus(a); return Maths_Vector2DScale(r, a, (1.0f/m) * l); }

// <inlined>
__inline Point2F* Maths_Vector2DRandom(OUT Point2F* r) { r->x = Maths_RandRange(0.0f, 1.0f); r->y = Maths_RandRange(0.0f, 1.0f); return Maths_Vector2DNormalise(r); }

// <inlined>
__inline real32 Maths_Vector2DCrossProductZ(const Point2F* a, const Point2F* b) { return (a->x*b->y) - (a->y*b->x); }

// <inlined>
__inline real32 Maths_Vector3DModulus(const Vector3F* v) { return (real32)std::sqrt((v->x*v->x)+(v->y*v->y)+(v->z*v->z)); }

// <inlined>
__inline real32 Maths_Vector3DDistance(const Vector3F* a, const Vector3F* b) { return (real32)std::sqrt(((a->x-b->x)*(a->x-b->x))+((a->y-b->y)*(a->y-b->y))+((a->z-b->z)*(a->z-b->z))); }


// <LegoRR.exe @004013e0>
Vector3F* __cdecl noinline(Maths_Vector3DCrossProduct)(OUT Vector3F* r, const Vector3F* a, const Vector3F* b);
__inline Vector3F* Maths_Vector3DCrossProduct(OUT Vector3F* r, const Vector3F* a, const Vector3F* b) { r->x = (a->y*b->z)-(a->z*b->y); r->y = (a->z*b->x)-(a->x*b->z); r->z = (a->x*b->y)-(a->y*b->x); return r; }

// <LegoRR.exe @00401470>
Vector3F* __cdecl noinline(Maths_Vector3DAdd)(OUT Vector3F* r, const Vector3F* a, const Vector3F* b);
__inline Vector3F* Maths_Vector3DAdd(OUT Vector3F* r, const Vector3F* a, const Vector3F* b) { r->x = (a->x+b->x); r->y = (a->y+b->y); r->z = (a->z+b->z); return r; }

// <LegoRR.exe @00401630>
Vector3F* __cdecl noinline(Maths_Vector3DSubtract)(OUT Vector3F* r, const Vector3F* a, const Vector3F* b);
__inline Vector3F* Maths_Vector3DSubtract(OUT Vector3F* r, const Vector3F* a, const Vector3F* b) { r->x = (a->x-b->x); r->y = (a->y-b->y); r->z = (a->z-b->z); return r; }


// <inlined>
__inline real32 Maths_Vector3DDotProduct(const Vector3F* a, const Vector3F* b) { return (a->x*b->x) + (a->y*b->y) + (a->z*b->z); }


// <LegoRR.exe @00401660>
Vector3F* __cdecl noinline(Maths_Vector3DScale)(OUT Vector3F* r, const Vector3F* a, real32 f);
__inline Vector3F* Maths_Vector3DScale(OUT Vector3F* r, const Vector3F* a, real32 f) { r->x = a->x*f; r->y = a->y*f; r->z = a->z*f; return r; }

// <LegoRR.exe @00401690>
Vector3F* __cdecl noinline(Maths_Vector3DNormalize)(OUT Vector3F* r);
__inline Vector3F* Maths_Vector3DNormalize(OUT Vector3F* r) { Maths_Vector3DScale(r, r, 1.0f/Maths_Vector3DLength(r)); return r; }

/// CUSTOM:
__inline Vector3F* Maths_Vector3DSetLength(OUT Vector3F* r, const Vector3F* a, real32 l) { real32 m = Maths_Vector3DModulus(a); return Maths_Vector3DScale(r, a, (1.0f/m) * l); }


// <inlined>
__inline Vector3F* Maths_Vector3DInterpolate(OUT Vector3F* result, const Vector3F* start, const Vector3F* end, real32 delta) { Maths_Vector3DSubtract(result, end, start); Maths_Vector3DScale(result, result, delta); Maths_Vector3DAdd(result, start, result); return result; }


//// END OF INLINES ////

// <unused>
Area2F* __cdecl Maths_Area2DInterpolate(OUT Area2F* result, const Area2F* start, const Area2F* end, real32 delta);


// <LegoRR.exe @004797c0>
Vector3F* __cdecl Maths_Vector3DRandom(OUT Vector3F* r);


// <unused>
Vector3F* __cdecl Maths_Vector3DReflect(OUT Vector3F* d, const Vector3F* ray, const Vector3F* norm);


// <LegoRR.exe @004797d0>
Vector3F* __cdecl Maths_Vector3DRotate(OUT Vector3F* r, const Vector3F* v, const Vector3F* axis, real32 theta);

// <LegoRR.exe @004797f0>
Vector3F* __cdecl Maths_PlaneNormal(OUT Vector3F* n, const Vector3F* p1, const Vector3F* p2, const Vector3F* p3);

// <LegoRR.exe @004798f0>
real32 __cdecl Maths_TriangleAreaZ(const Vector3F* p1, const Vector3F* p2, const Vector3F* p3, bool32 bfc);

// Legacy `uint16 std::rand();`
// <LegoRR.exe @00479b60>
sint16 __cdecl Maths_Rand(void);

// <LegoRR.exe @00479b70>
real32 __cdecl Maths_RandRange(real32 low, real32 high);

// <LegoRR.exe @00479ba0>
bool32 __cdecl Maths_RayPlaneIntersection(Vector3F* endPoint, const Vector3F* rayOrigin, const Vector3F* ray, const Vector3F* planePoint, const Vector3F* planeNormal);

// <LegoRR.exe @00479cf0>
bool32 __cdecl Maths_RayPlaneDistance(OUT real32* dist, const Vector3F* rayOrigin, const Vector3F* ray, const Vector3F* planePoint, const Vector3F* planeNormal);

// <LegoRR.exe @00479d70>
void __cdecl Maths_RayEndPoint(OUT Vector3F* endPoint, const Vector3F* rayOrigin, const Vector3F* ray, real32 dist);


// <unused>
Point2F* __cdecl Maths_Vector2DLineIntersection(OUT Point2F* intersection, const Point2F* start1, const Point2F* end1, const Point2F* start2, const Point2F* end2);


// <LegoRR.exe @00479db0>
Point2F* __cdecl Maths_Vector2DIntersection(OUT Point2F* intersection, const Point2F* start1, const Point2F* vector1, const Point2F* start2, const Point2F* vector2);

// <LegoRR.exe @00479e40>
bool32 __cdecl Maths_PointInsidePoly(const Point2F* point, const Point2F* fromList, const Point2F* toList, uint32 count);


// <unused>
Vector3F* __cdecl Maths_Vector3DApplyMatrixXYZ(OUT Vector3F* result, const Matrix4F matrix, real32 x, real32 y, real32 z);

// not inlined (or even implemented) by Gods98
// <unused>
__inline Vector3F* Maths_Vector3DApplyMatrix(OUT Vector3F* result, const Matrix4F matrix, const Vector3F* point)
{
	return Maths_Vector3DApplyMatrixXYZ(result, matrix, point->x, point->y, point->z);
}


// <LegoRR.exe @00479ed0>
bool32 __cdecl Maths_RaySphereIntersection(const Vector3F* center, real32 radius, const Vector3F* camera, const Vector3F* line);



#pragma endregion

/**********************************************************************************
 ******** Matrix4 Functions
 **********************************************************************************/

/////////////////////////////////////////////////////////////////////////
// Andy Ray.  3/2/99.
// Some simple matrix functions.
/////////////////////////////////////////////////////////////////////////

#pragma region Functions

// Compared to other Matrix4 multiplication functions, a = right, and b = left
// <LegoRR.exe @00479fa0>
void __cdecl Matrix_Mult(OUT Matrix4F ans, const Matrix4F a, const Matrix4F b);

// Generates the matrix for a rotation of rot radians about the X-Axis
// <LegoRR.exe @0047a010>
void __cdecl Matrix_RotX(OUT Matrix4F ans, real32 rot);

// Generates the matrix for a rotation of rot radians about the Y-Axis
// <LegoRR.exe @0047a060>
void __cdecl Matrix_RotY(OUT Matrix4F ans, real32 rot);

// Generates the matrix for a rotation of rot radians about the Z-Axis
// <LegoRR.exe @0047a0b0>
void __cdecl Matrix_RotZ(OUT Matrix4F ans, real32 rot);

// Generates a translation matrix.
// <LegoRR.exe @0047a100>
void __cdecl Matrix_Translate(OUT Matrix4F ans, const Vector3F* trans);

// Makes the identity matrix
// <LegoRR.exe @0047a130>
void __cdecl Matrix_Identity(OUT Matrix4F ans);

// Makes the zero matrix.
// <LegoRR.exe @0047a160>
void __cdecl Matrix_Zero(OUT Matrix4F ans);

// Copies a matrix.
// <LegoRR.exe @0047a170>
void __cdecl Matrix_Copy(OUT Matrix4F to, const Matrix4F from);


// <unused>
bool32 __cdecl Matrix_Invert(OUT Matrix4F q, const Matrix4F a);

#pragma endregion

}
