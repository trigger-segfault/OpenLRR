#pragma once

#include "../common.h"

#pragma region Geometry Typedefs

typedef real32 Matrix3F[3][3];
static_assert(sizeof(Matrix3F) == 0x24, "");

typedef real32 Matrix4F[4][4];
static_assert(sizeof(Matrix4F) == 0x40, "");

#pragma endregion

#pragma region Geometry Structs

//COORD
struct Coord2I
{
	/*0,2*/ sint16 x;
	/*2,2*/ sint16 y;
	/*4*/
};
static_assert(sizeof(Coord2I) == 0x4, "");


//POINT
struct Point2I
{
	/*0,4*/ sint32 x;
	/*4,4*/ sint32 y;
	/*8*/
};
static_assert(sizeof(Point2I) == 0x8, "");


struct Point2U
{
	/*0,4*/ uint32 x;
	/*4,4*/ uint32 y;
	/*8*/
};
static_assert(sizeof(Point2U) == 0x8, "");

// (VECTOR2D)
struct Point2F
{
	/*0,4*/ real32 x;
	/*4,4*/ real32 y;
	/*8*/
};
static_assert(sizeof(Point2F) == 0x8, "");


//D3DVECTOR (VECTOR3D)
struct Vector3F
{
	/*0,4*/ real32 x;
	/*4,4*/ real32 y;
	/*8,4*/ real32 z;
	/*c*/
};
static_assert(sizeof(Vector3F) == 0xc, "");


//D3DRMVECTOR4D (VECTOR4D)
struct Vector4F
{
	/*00,4*/ real32 x;
	/*04,4*/ real32 y;
	/*08,4*/ real32 z;
	/*0c,4*/ real32 w;
	/*10*/
};
static_assert(sizeof(Vector4F) == 0x10, "");


struct Size2I
{
	/*0,4*/ sint32 width;
	/*4,4*/ sint32 height;
	/*8*/
};
static_assert(sizeof(Size2I) == 0x8, "");


struct Size2F
{
	/*0,4*/ real32 width;
	/*4,4*/ real32 height;
	/*8*/
};
static_assert(sizeof(Size2F) == 0x8, "");


struct Area2I //Rect2I
{
	union {
		struct {
			/*00,4*/ sint32 x;
			/*04,4*/ sint32 y;
			/*08,4*/ sint32 width;
			/*0c,4*/ sint32 height;
			/*10*/
		};
		struct {
			/*00,8*/ Point2I point;
			/*08,8*/ Size2I  size;
			/*10*/
		};
	};
};
static_assert(sizeof(Area2I) == 0x10, "");

//typedef Rect2I Area2I;


// (AREA2D)
struct Area2F //Rect2F
{
	union {
		struct {
			/*00,4*/ real32 x;
			/*04,4*/ real32 y;
			/*08,4*/ real32 width;
			/*0c,4*/ real32 height;
			/*10*/
		};
		struct {
			/*00,8*/ Point2F point;
			/*08,8*/ Size2F  size;
			/*10*/
		};
	};
};
static_assert(sizeof(Area2F) == 0x10, "");

//typedef Rect2F Area2F;


struct P1P2Rect2I
{
	union {
		struct {
			/*00,4*/ sint32 x1;
			/*04,4*/ sint32 y1;
			/*08,4*/ sint32 x2;
			/*0c,4*/ sint32 y2;
			/*10*/
		};
		struct {
			/*00,8*/ Point2I p1;
			/*08,8*/ Point2I p2;
			/*10*/
		};
	};
};
static_assert(sizeof(P1P2Rect2I) == 0x10, "");


struct P1P2Rect2F
{
	union {
		struct {
			/*00,4*/ real32 x1;
			/*04,4*/ real32 y1;
			/*08,4*/ real32 x2;
			/*0c,4*/ real32 y2;
			/*10*/
		};
		struct {
			/*00,8*/ Point2F p1;
			/*08,8*/ Point2F p2;
			/*10*/
		};
	};
};
static_assert(sizeof(P1P2Rect2F) == 0x10, "");


//RECT
struct LTRBRect2I
{
	/*00,4*/ sint32 left;
	/*04,4*/ sint32 top;
	/*08,4*/ sint32 right;
	/*0c,4*/ sint32 bottom;
	/*10*/
};
static_assert(sizeof(LTRBRect2I) == 0x10, "");


struct LTRBRect2F
{
	/*00,4*/ real32 left;
	/*04,4*/ real32 top;
	/*08,4*/ real32 right;
	/*0c,4*/ real32 bottom;
	/*10*/
};
static_assert(sizeof(LTRBRect2F) == 0x10, "");


//D3DRMVERTEX (VERTEX3D)
struct Vertex3F
{
	/*00,c*/ Vector3F position;
	/*0c,c*/ Vector3F normal;
	/*18,4*/ real32 tu;
	/*1c,4*/ real32 tv;
	/*20,4*/ colour32 colour;
	/*24*/
};
static_assert(sizeof(Vertex3F) == 0x24, "");


//BOX3D
struct Box3F
{
	/*00,c*/ Vector3F min;
	/*0c,c*/ Vector3F max;
	/*18*/
};
static_assert(sizeof(Box3F) == 0x18, "");

#pragma endregion
