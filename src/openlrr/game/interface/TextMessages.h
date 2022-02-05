// TextMessages.h : 
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

struct Text_Globs // [LegoRR/Text.c|struct:0x4dc|tags:GLOBS]
{
	/*000,68*/	const char* textName[Text_Type_Count];
	/*068,68*/	char* textMessages[Text_Type_Count];
	/*0d0,68*/	Gods98::Image* textImages[Text_Type_Count];
	/*138,340*/	char textImagesSFX[Text_Type_Count][32];
	/*478,4*/	Text_Type currType;
	/*47c,4*/	uint32 textCount;
	/*480,4*/	uint32 textFlags; // (0x8000: ?, 0x10000: ?, unkFlags param, -= 1 operation???)
	/*484,4*/	char* currText; // Pointer to NERPsMessage raw text
	/*488,4*/	real32 float_488;
	/*48c,4*/	Gods98::TextWindow* textWnd_48c;
	/*490,4*/	Gods98::TextWindow* textWnd_490;
	/*494,4*/	undefined4 reserved1;
	/*498,4*/	uint32 uint_498;
	/*49c,10*/	Area2F MsgPanel_Rect1;
	/*4ac,4*/	real32 float_4ac;
	/*4b0,4*/	real32 MsgPanel_Float20;
	/*4b4,4*/	real32 MsgPanel_Float42;
	/*4b8,4*/	real32 float_4b8;
	/*4bc,10*/	Area2F MsgPanel_Rect2;
	/*4cc,8*/	Point2I TextImagePosition;
	/*4d4,4*/	uint32 TextPanelFlags; // (0x1: ?, 0x4: ?)
	/*4d8,4*/	real32 TextPauseTime;
	/*4dc*/
};
assert_sizeof(Text_Globs, 0x4dc);

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @00504190>
extern Text_Globs & textGlobs;

#pragma endregion

/**********************************************************************************
 ******** Macros
 **********************************************************************************/

#pragma region Macros

#define Text_RegisterName(n)		(textGlobs.textName[n]=#n)

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @0046aab0>
#define Text_Load ((void (__cdecl* )(Gods98::TextWindow* param_1, Gods98::TextWindow* param_2, sint32 imageX, sint32 imageY, real32 pauseTime))0x0046aab0)

// <LegoRR.exe @0046ac10>
#define Text_Initialise ((void (__cdecl* )(const char* filename, uint32 param_2, uint32 param_3, uint32 unused_int, const Area2F* param_5, const Area2F* unused_rect, real32 param_7))0x0046ac10)

// <LegoRR.exe @0046aca0>
#define Text_UpdatePositionAndSize ((void (__cdecl* )(void))0x0046aca0)

// <LegoRR.exe @0046ad50>
#define Text_Clear ((void (__cdecl* )(void))0x0046ad50)

// <LegoRR.exe @0046ad90>
#define Text_GetTextType ((bool32 (__cdecl* )(const char* textName, OUT Text_Type* textType))0x0046ad90)

// <LegoRR.exe @0046add0>
#define Text_SetMessage ((void (__cdecl* )(Text_Type textType, const char* textMessage))0x0046add0)

// <LegoRR.exe @0046ae70>
#define Text_SetMessageWithImage ((void (__cdecl* )(Text_Type textType, const char* message, const char* filename, const char* sfxName))0x0046ae70)

// <LegoRR.exe @0046aee0>
#define Text_SetNERPsMessage ((void (__cdecl* )(const char* text, bool32 unkFlags))0x0046aee0)

// <LegoRR.exe @0046af20>
#define Text_DisplayMessage ((void (__cdecl* )(Text_Type textType, bool32 changeTiming, bool32 setFlag4))0x0046af20)

// <LegoRR.exe @0046afc0>
#define Text_Update ((void (__cdecl* )(real32 elapsedAbs))0x0046afc0)

#pragma endregion

}
