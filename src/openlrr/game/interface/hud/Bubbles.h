// hud/Bubbles.h : 
//

#pragma once

#include "../../../common.h"
#include "../../../engine/colour.h"
#include "../../../engine/geometry.h"
#include "../../../engine/undefined.h"

#include "../../../engine/gfx/Containers.h"
#include "../../../engine/drawing/Images.h"

#include "../../GameCommon.h"


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

struct BubbleData // [LegoRR/Bubble.c|struct:0x8]
{
	/*0,4*/	LegoObject* object_0;
	/*4,4*/	real32 float_4;
	/*8*/
};
assert_sizeof(BubbleData, 0x8);


struct Bubble_Globs // [LegoRR/Bubble.c|struct:0x4f4|tags:GLOBS]
{
	/*000,4*/	bool32 ObjectUIsAlwaysVisible; // Object Display HUD/UI over entities in-game
	/*004,9c*/	Gods98::Image* bubbleImages[Bubble_Type_Count];
	/*0a0,9c*/	const char* bubbleName[Bubble_Type_Count];
	/*13c,48*/	undefined1 reserved1[72];
	/*184,a0*/	BubbleData table1[20];
	/*224,a0*/	BubbleData table2[20];
	/*2c4,a0*/	BubbleData table3[20];
	/*364,190*/	BubbleData bigTable[50];
	/*4f4*/
};
assert_sizeof(Bubble_Globs, 0x4f4);

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @00558860>
extern Bubble_Globs & bubbleGlobs;

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @00406fe0>
#define Bubble_Initialise ((void (__cdecl* )(void))0x00406fe0)

// <LegoRR.exe @00407170>
#define Bubble_LoadCFG ((void (__cdecl* )(const Gods98::Config* config))0x00407170)

// <LegoRR.exe @00407230>
#define Bubble_GetBubbleType ((Bubble_Type (__cdecl* )(const char* bubbleName))0x00407230)

// <LegoRR.exe @00407270>
#define Bubble_ToggleObjectUIsAlwaysVisible ((void (__cdecl* )(void))0x00407270)

// <LegoRR.exe @00407290>
#define Bubble_GetObjectUIsAlwaysVisible ((bool32 (__cdecl* )(void))0x00407290)

// <LegoRR.exe @004072a0>
#define Bubble_LiveObject_UpdateBubbleImage ((void (__cdecl* )(LegoObject* liveObj))0x004072a0)

// <LegoRR.exe @004072d0>
#define Bubble_LiveObject_FUN_004072d0 ((void (__cdecl* )(LegoObject* liveObj))0x004072d0)

// <LegoRR.exe @00407340>
#define Bubble_LiveObject_FUN_00407340 ((void (__cdecl* )(LegoObject* liveObj))0x00407340)

// <LegoRR.exe @00407380>
#define Bubble_LiveObject_MiniFigure_FUN_00407380 ((void (__cdecl* )(LegoObject* liveObj))0x00407380)

// <LegoRR.exe @004073e0>
#define Bubble_LiveObject_MiniFIgure_FUN_004073e0 ((void (__cdecl* )(LegoObject* liveObj))0x004073e0)

// <LegoRR.exe @00407440>
#define Bubble_LiveObject_MiniFigure_FUN_00407440 ((void (__cdecl* )(LegoObject* liveObj, real32 param_2))0x00407440)

// <LegoRR.exe @00407470>
#define Bubble_LiveObject_FUN_00407470 ((void (__cdecl* )(LegoObject* liveObj))0x00407470)

// <LegoRR.exe @004074d0>
#define Bubble_Unk_DrawObjectUIs_FUN_004074d0 ((void (__cdecl* )(real32 elapsedAbs))0x004074d0)

// <LegoRR.exe @004077f0>
#define Bubble_LiveObject_GetBubbleImage_FUN_004077f0 ((void (__cdecl* )(LegoObject* liveObj, real32 param_2, OUT Gods98::Image** bubbleImage, OUT Point2F* screenPt))0x004077f0)

// <LegoRR.exe @00407890>
#define Bubble_LiveObject_CallbackDraw_FUN_00407890 ((bool32 (__cdecl* )(LegoObject* liveObj, real32* param_2))0x00407890)

// <LegoRR.exe @00407940>
#define Bubble_LiveObject_GetCurrentBubbleImage ((void (__cdecl* )(LegoObject* liveObj, OUT Gods98::Image** bubbleImage))0x00407940)

#pragma endregion

}
