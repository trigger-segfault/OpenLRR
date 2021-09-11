#pragma once

#include "../common.h"



#pragma region Colour Structs

struct ColourRGBI
{
	/*0,4*/ uint32 red;
	/*4,4*/ uint32 green;
	/*8,4*/ uint32 blue;
	/*c*/
};
static_assert(sizeof(ColourRGBI) == 0xc, "");


struct ColourRGBF
{
	/*0,4*/ real32 red;
	/*4,4*/ real32 green;
	/*8,4*/ real32 blue;
	/*c*/
};
static_assert(sizeof(ColourRGBF) == 0xc, "");


struct ColourRGBAI
{
	/*00,4*/ uint32 red;
	/*04,4*/ uint32 green;
	/*08,4*/ uint32 blue;
	/*0c,4*/ uint32 alpha;
	/*10*/
};
static_assert(sizeof(ColourRGBAI) == 0x10, "");


struct ColourRGBAF
{
	/*00,4*/ real32 red;
	/*04,4*/ real32 green;
	/*08,4*/ real32 blue;
	/*0c,4*/ real32 alpha;
	/*10*/
};
static_assert(sizeof(ColourRGBAF) == 0x10, "");


#pragma pack(push, 1)

struct ColourRGBPacked
{
	/*0,1*/ uint8 red;
	/*1,1*/ uint8 green;
	/*2,1*/ uint8 blue;
	/*3*/
};
static_assert(sizeof(ColourRGBPacked) == 0x3, "");


struct ColourRGBAPacked
{
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
};
static_assert(sizeof(ColourRGBAPacked) == 0x4, "");

#pragma pack(pop)

#pragma endregion
