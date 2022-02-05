// RadarMap.h : 
//

#pragma once

#include "../../engine/drawing/Draw.h"

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

struct RadarMap // [LegoRR/RadarMap.c|struct:0x3c]
{
	/*00,4*/	Map3D* map;
	/*04,10*/	Area2F screenRect;
	/*14,4*/	real32 arrowTiltOrZoom;
	/*18,4*/	real32 BlockSize;
	/*1c,8*/	Point2F arrowPos;
	/*24,10*/	Area2F worldRect;
	/*34,4*/	Gods98::Draw_Rect* drawRectList;
	/*38,4*/	uint32 drawRectCount;
	/*3c*/
};
assert_sizeof(RadarMap, 0x3c);


struct RadarStructF_10 // [LegoRR/RadarMap.c|struct:0x10]
{
	/*00,4*/	real32 width; // [0,1] as ratio of appWidth
	/*04,4*/	real32 height; // [0,1] as ratio of appHeight
	/*08,4*/	undefined4 field_8;
	/*0c,4*/	real32 float_c; // (init: 1.0)
	/*10*/
};
assert_sizeof(RadarStructF_10, 0x10);


struct RadarMap_Globs // [LegoRR/RadarMap.c|struct:0x3f4|tags:GLOBS]
{
	/*000,168*/	ColourRGBF colourTable[30]; // (constant, RGBf [0,255] -> [0,1] on RadarMap_Initialise)
	/*168,4*/	sint32 strucfUnkInt_1; // (init: -1)
	/*16c,4*/	sint32 strucfUnkInt_2; // (init: -1)
	/*170,140*/	RadarStructF_10 structfTable_1[20];
	/*2b0,140*/	RadarStructF_10 structfTable_2[20];
	/*3f0,4*/	uint32 structfCount;
	/*3f4*/
};
assert_sizeof(RadarMap_Globs, 0x3f4);

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @004a9f28>
extern RadarMap_Globs & radarmapGlobs;

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

// <LegoRR.exe @0045db60>
#define RadarMap_SetTVTiltOrZoom ((void (__cdecl* )(RadarMap* radarMap, real32 tvTiltOrZoom))0x0045db60)

// <LegoRR.exe @0045db70>
#define RadarMap_Initialise ((void (__cdecl* )(void))0x0045db70)

// <LegoRR.exe @0045dd50>
#define RadarMap_Create ((RadarMap* (__cdecl* )(Map3D* map, const Area2F* radarViewRect, real32 tvTiltOrZoom))0x0045dd50)

// <LegoRR.exe @0045ddb0>
#define RadarMap_Free_UnwindMultiUse ((void (__cdecl* )(RadarMap* radarMap))0x0045ddb0)

// <LegoRR.exe @0045ddc0>
#define RadarMap_DrawDotCircle_FUN_0045ddc0 ((void (__cdecl* )(RadarMap* radarMap, const Point2F* param_2, real32 param_3, real32 param_4))0x0045ddc0)

// <LegoRR.exe @0045de80>
#define RadarMap_Draw_FUN_0045de80 ((void (__cdecl* )(RadarMap* radarMap, const Point2F* position))0x0045de80)

// <LegoRR.exe @0045e6c0>
#define RadarMap_CanLiveObjectShowOnRadar ((bool32 (__cdecl* )(LegoObject* liveObj))0x0045e6c0)

// <LegoRR.exe @0045e720>
#define RadarMap_LiveObject_Callback_FUN_0045e720 ((bool32 (__cdecl* )(LegoObject* liveObj, RadarMap* radarMap))0x0045e720)

// <LegoRR.exe @0045e920>
#define RadarMap_InsideRadarScreen ((bool32 (__cdecl* )(RadarMap* radarMap, uint32 mouseX, uint32 mouseY))0x0045e920)

// <LegoRR.exe @0045e990>
#define RadarMap_ScreenToWorldBlockPos ((bool32 (__cdecl* )(RadarMap* radarMap, uint32 mouseX, uint32 mouseY, OPTIONAL OUT Point2F* worldPos, OUT sint32* bx, OUT sint32* by))0x0045e990)

// <LegoRR.exe @0045eae0>
#define RadarMap_TrySelectObject ((bool32 (__cdecl* )(RadarMap* radarMap, uint32 mouseX, uint32 mouseY, OUT LegoObject** liveObj, OPTIONAL OUT Point2F* objPosition))0x0045eae0)

// DATA: SearchRadarObjectRadius_10*
// <LegoRR.exe @0045eba0>
#define RadarMap_LiveObjectCallback_InsideSquareRadius ((bool32 (__cdecl* )(LegoObject* liveObj, void* search))0x0045eba0)

// <LegoRR.exe @0045ec00>
#define RadarMap_FUN_0045ec00 ((void (__cdecl* )(RadarMap* radarMap, IN OUT Area2F* param_2))0x0045ec00)

// <LegoRR.exe @0045eca0>
#define RadarMap_DrawBlock ((bool32 (__cdecl* )(Gods98::Draw_Rect* drawRect, uint32 bx, uint32 by))0x0045eca0)

#pragma endregion

}
