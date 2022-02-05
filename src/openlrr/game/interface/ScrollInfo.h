// ScrollInfo.h : 
//

#pragma once

#include "../GameCommon.h"


namespace LegoRR
{; // !<---

/**********************************************************************************
 ******** Forward Declarations
 **********************************************************************************/

#pragma region Forward Declarations

#pragma endregion

/**********************************************************************************
 ******** Constants
 **********************************************************************************/

#pragma region Constants

#pragma endregion

/**********************************************************************************
 ******** Enumerations
 **********************************************************************************/

#pragma region Enums

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

struct ScrollInfoSubStruct_1c // [LegoRR/ScrollInfo.c|struct:0x1c]
{
	/*00,8*/	Point2F point1;
	/*08,8*/	Point2F point2;
	/*10,4*/	Gods98::Image* image1;
	/*14,4*/	Gods98::Image* image2;
	/*18,4*/	uint32 flags;
	/*1c*/
};
assert_sizeof(ScrollInfoSubStruct_1c, 0x1c);


struct ScrollInfoSubStruct_28 // [LegoRR/ScrollInfo.c|struct:0x28]
{
	/*00,8*/	Point2F point1;
	/*08,8*/	Point2F point2;
	/*10,4*/	undefined4 field_10;
	/*14,4*/	undefined4 field_14;
	/*18,4*/	Gods98::Image* image;
	/*1c,4*/	sint32 intM1_1c;
	/*20,4*/	sint32 int_20;
	/*24,4*/	bool32 bool_24;
	/*28*/
};
assert_sizeof(ScrollInfoSubStruct_28, 0x28);


struct ScrollInfoStruct_20 // [LegoRR/ScrollInfo.c|struct:0x20]
{
	/*00,4*/	Gods98::Image* image;
	/*04,8*/	Point2F pointOrSize;
	/*0c,4*/	ScrollInfoSubStruct_1c* ptr1c_c;
	/*10,4*/	ScrollInfoSubStruct_1c* ptr1c_10;
	/*14,4*/	ScrollInfoSubStruct_28* substruct28;
	/*18,4*/	Panel_Type panelType;
	/*1c,4*/	uint32 flags; // (1 = used, 4 = hasImage)
	/*20*/
};
assert_sizeof(ScrollInfoStruct_20, 0x20);

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @005023c0>
extern bool32 & s_ScrollInfo_BOOL_005023c0;

// <LegoRR.exe @00553140>
extern ScrollInfoStruct_20 (& g_ScrollInfos)[2];

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

// <LegoRR.exe @00463970>
#define ScrollInfo_Initialise ((void (__cdecl* )(const char* filename, bool32 index, uint32 unkFlags, sint32 param_4, sint32 param_5))0x00463970)

// <LegoRR.exe @004639f0>
#define ScrollInfo_SetXYOrSize_Unk ((void (__cdecl* )(bool32 index, uint32 x_or_width, uint32 y_or_height))0x004639f0)

// <LegoRR.exe @00463a30>
#define ScrollInfo_AddStruct1C ((void (__cdecl* )(bool32 index, bool32 use50InsteadOf4c, const Area2F* rect, OPTIONAL const char* filename1, OPTIONAL const char* filename2))0x00463a30)

// <LegoRR.exe @00463ad0>
#define ScrollInfo_AddCreateStruct28 ((void (__cdecl* )(bool32 index, Area2F* area, sint32 number, sint32 param_4, const char* filename))0x00463ad0)

// <LegoRR.exe @00463b60>
#define ScrollInfo_Mouse_FUN_00463b60 ((bool32 (__cdecl* )(OUT sint32* out_param_1, uint32 mouseX, uint32 mouseY, bool32 param_4, bool32 param_5))0x00463b60)

// <LegoRR.exe @00463ec0>
#define ScrollInfo_FUN_00463ec0 ((bool32 (__cdecl* )(ScrollInfoStruct_20* param_1, ScrollInfoSubStruct_1c* param_2, sint32 mouseX, sint32 mouseY))0x00463ec0)

// <LegoRR.exe @00463f50>
#define ScrollInfo_FUN_00463f50 ((bool32 (__cdecl* )(ScrollInfoStruct_20* param_1, sint32 mouseX, sint32 mouseY))0x00463f50)

// <LegoRR.exe @00463fe0>
#define ScrollInfo_Update ((void (__cdecl* )(bool32 index))0x00463fe0)

// <LegoRR.exe @00464100>
#define ScrollInfo_MathX_RoundToLL ((sint64 (__cdecl* )(real32 x))0x00464100)

// <LegoRR.exe @00464160>
#define ScrollInfo_GetSubStruct28_Fields1C_20 ((void (__cdecl* )(bool32 index, OUT sint32* field1c, OUT sint32* field20))0x00464160)

// <LegoRR.exe @00464190>
#define ScrollInfo_SetSubStruct28_Fields1C_20 ((void (__cdecl* )(bool32 index, sint32 field1c, sint32 field20))0x00464190)

#pragma endregion

}
