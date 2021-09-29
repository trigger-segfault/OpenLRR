#pragma once

#include "../common.h"


#pragma region Colour Structs

/// TODO: Add `r`, `g`, `b`, `a` field aliases to all colours. They're much cleaner.

struct ColourRGBI {
	/*0,4*/ uint32 red;
	/*4,4*/ uint32 green;
	/*8,4*/ uint32 blue;
	/*c*/
}; assert_sizeof(ColourRGBI, 0xc);


struct ColourRGBF {
	/*0,4*/ real32 red;
	/*4,4*/ real32 green;
	/*8,4*/ real32 blue;
	/*c*/
}; assert_sizeof(ColourRGBF, 0xc);


struct ColourRGBAI {
	/*00,4*/ uint32 red;
	/*04,4*/ uint32 green;
	/*08,4*/ uint32 blue;
	/*0c,4*/ uint32 alpha;
	/*10*/
}; assert_sizeof(ColourRGBAI, 0x10);


struct ColourRGBAF {
	/*00,4*/ real32 red;
	/*04,4*/ real32 green;
	/*08,4*/ real32 blue;
	/*0c,4*/ real32 alpha;
	/*10*/
}; assert_sizeof(ColourRGBAF, 0x10);


#pragma pack(push, 1)

// 24-bit packed RGB-ordered colour channels
struct ColourRGBPacked {
	/*0,1*/ uint8 red;
	/*1,1*/ uint8 green;
	/*2,1*/ uint8 blue;
	/*3*/
}; assert_sizeof(ColourRGBPacked, 0x3);

// 32-bit packed RGBA-ordered colour channels
struct ColourRGBAPacked {
	union {
		struct {
			/*0,1*/ uint8 red;
			/*1,1*/ uint8 green;
			/*2,1*/ uint8 blue;
			/*3,1*/ uint8 alpha;
			/*4*/
		};
		/*0,4*/ uint32 rgbaColour;
		/*4*/
	};
}; assert_sizeof(ColourRGBAPacked, 0x4);


// 24-bit packed BGR-ordered colour channels
struct ColourBGRPacked {
	/*0,1*/ uint8 blue;
	/*1,1*/ uint8 green;
	/*2,1*/ uint8 red;
	/*3*/
}; assert_sizeof(ColourBGRPacked, 0x3);

// 32-bit packed BGRA-ordered colour channels
struct ColourBGRAPacked {
	union {
		struct {
			/*0,1*/ uint8 blue;
			/*1,1*/ uint8 green;
			/*2,1*/ uint8 red;
			/*3,1*/ uint8 alpha;
			/*4*/
		};
		/*0,4*/ uint32 bgraColour;
		/*4*/
	};
}; assert_sizeof(ColourRGBAPacked, 0x4);

#pragma pack(pop)

#pragma endregion
