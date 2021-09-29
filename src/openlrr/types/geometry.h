#pragma once

#include "../common.h"


#pragma region Geometry enums

enum Direction : sint32
{
	DIRECTION__INVALID = -1,

	DIRECTION_UP     = 0,
	DIRECTION_RIGHT  = 1,
	DIRECTION_DOWN   = 2,
	DIRECTION_LEFT   = 3,

	DIRECTION__COUNT = 4,
};
assert_sizeof(Direction, 0x4);

#pragma endregion


#pragma region Geometry typedefs

typedef real32 Matrix3F[3][3];
assert_sizeof(Matrix3F, 0x24);

typedef real32 Matrix4F[4][4];
assert_sizeof(Matrix4F, 0x40);

#pragma endregion


#pragma region Geometry structs

// COORD
struct Coord2I {
	/*0,2*/ sint16 x;
	/*2,2*/ sint16 y;
	/*4*/
}; assert_sizeof(Coord2I, 0x4);


// POINT
struct Point2I {
	/*0,4*/ sint32 x;
	/*4,4*/ sint32 y;
	/*8*/
}; assert_sizeof(Point2I, 0x8);

struct Point2U {
	/*0,4*/ uint32 x;
	/*4,4*/ uint32 y;
	/*8*/
}; assert_sizeof(Point2U, 0x8);

// (VECTOR2D)
struct Point2F {
	/*0,4*/ real32 x;
	/*4,4*/ real32 y;
	/*8*/
}; assert_sizeof(Point2F, 0x8);

//typedef Point2F Vector2F;


// D3DVECTOR (VECTOR3D)
struct Vector3F {
	/*0,4*/ real32 x;
	/*4,4*/ real32 y;
	/*8,4*/ real32 z;
	/*c*/
}; assert_sizeof(Vector3F, 0xc);


// D3DRMVECTOR4D (VECTOR4D)
struct Vector4F {
	/*00,4*/ real32 x;
	/*04,4*/ real32 y;
	/*08,4*/ real32 z;
	/*0c,4*/ real32 w;
	/*10*/
}; assert_sizeof(Vector4F, 0x10);


// SIZE
struct Size2I {
	/*0,4*/ sint32 width;
	/*4,4*/ sint32 height;
	/*8*/
}; assert_sizeof(Size2I, 0x8);

struct Size2U {
	/*0,4*/ uint32 width;
	/*4,4*/ uint32 height;
	/*8*/
}; assert_sizeof(Size2U, 0x8);

struct Size2F {
	/*0,4*/ real32 width;
	/*4,4*/ real32 height;
	/*8*/
}; assert_sizeof(Size2F, 0x8);


struct Range2I {
	/*0,4*/ sint32 min;
	/*4,4*/ sint32 max;
	/*8*/
}; assert_sizeof(Range2I, 0x8);

struct Range2U {
	/*0,4*/ uint32 min;
	/*4,4*/ uint32 max;
	/*8*/
}; assert_sizeof(Range2U, 0x8);

struct Range2F {
	/*0,4*/ real32 min;
	/*4,4*/ real32 max;
	/*8*/
}; assert_sizeof(Range2F, 0x8);


// (point/size rect)
struct Area2I {
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
}; assert_sizeof(Area2I, 0x10);

// (point/size rect)
struct Area2U {
	union {
		struct {
			/*00,4*/ uint32 x;
			/*04,4*/ uint32 y;
			/*08,4*/ uint32 width;
			/*0c,4*/ uint32 height;
			/*10*/
		};
		struct {
			/*00,8*/ Point2U point;
			/*08,8*/ Size2U  size;
			/*10*/
		};
	};
}; assert_sizeof(Area2U, 0x10);

// (AREA2D, point/size rect)
struct Area2F {
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
}; assert_sizeof(Area2F, 0x10);


// RECT (margin rect)
struct Rect2I {
	/*00,4*/ sint32 left;
	/*04,4*/ sint32 top;
	/*08,4*/ sint32 right;
	/*0c,4*/ sint32 bottom;
	/*10*/
}; assert_sizeof(Rect2I, 0x10);

// (margin rect)
struct Rect2U {
	/*00,4*/ uint32 left;
	/*04,4*/ uint32 top;
	/*08,4*/ uint32 right;
	/*0c,4*/ uint32 bottom;
	/*10*/
}; assert_sizeof(Rect2U, 0x10);

// (margin rect)
struct Rect2F {
	/*00,4*/ real32 left;
	/*04,4*/ real32 top;
	/*08,4*/ real32 right;
	/*0c,4*/ real32 bottom;
	/*10*/
}; assert_sizeof(Rect2F, 0x10);


// (min/max rect)
struct Box2I {
	union {
		struct {
			/*00,4*/ sint32 x1;
			/*04,4*/ sint32 y1;
			/*08,4*/ sint32 x2;
			/*0c,4*/ sint32 y2;
			/*10*/
		};
		struct {
			/*00,8*/ Point2I min;
			/*08,8*/ Point2I max;
			/*10*/
		};
	};
}; assert_sizeof(Box2I, 0x10);

// (min/max rect)
struct Box2U {
	union {
		struct {
			/*00,4*/ uint32 x1;
			/*04,4*/ uint32 y1;
			/*08,4*/ uint32 x2;
			/*0c,4*/ uint32 y2;
			/*10*/
		};
		struct {
			/*00,8*/ Point2U min;
			/*08,8*/ Point2U max;
			/*10*/
		};
	};
}; assert_sizeof(Box2U, 0x10);

// (min/max rect)
struct Box2F {
	union {
		struct {
			/*00,4*/ real32 x1;
			/*04,4*/ real32 y1;
			/*08,4*/ real32 x2;
			/*0c,4*/ real32 y2;
			/*10*/
		};
		struct {
			/*00,8*/ Point2F min;
			/*08,8*/ Point2F max;
			/*10*/
		};
	};
}; assert_sizeof(Box2F, 0x10);


// (BOX3D, min/max rect)
struct Box3F {
	/*00,c*/ Vector3F min;
	/*0c,c*/ Vector3F max;
	/*18*/
}; assert_sizeof(Box3F, 0x18);


// D3DRMVERTEX (VERTEX3D)
struct Vertex {
	/*00,c*/ Vector3F position;
	/*0c,c*/ Vector3F normal;
	/*18,4*/ real32 tu;
	/*1c,4*/ real32 tv;
	/*20,4*/ colour32 colour;
	/*24*/
}; assert_sizeof(Vertex, 0x24);

#pragma endregion
