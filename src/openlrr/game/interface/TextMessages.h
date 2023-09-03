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

enum Text_GlobFlags : uint32 // [LegoRR/Text.c|flags:0x4|type:uint]
{
	TEXT_GLOB_FLAG_NONE  = 0,
	TEXT_GLOB_FLAG_NERPSMSG = 0x1,
	TEXT_GLOB_FLAG_FLASHING = 0x4,
};
flags_end(Text_GlobFlags, 0x4);

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
	/*480,4*/	uint32 jankCounter; // THE JANK COUNTER: (previously named textFlags)
									//  bits 0-14 : Counter value.
									//  bit    15 : Counter decrement disabled flag.
									//  bit    16 : When counter value is decremented past zero, switches to bit 15 flag.
									//              Bit 16 is ALWAYS set with the counter value (or bit 15 flag when counter == 0).
									// From how this counter field is used. The decrement operation and counter value CHANGES ABSOLUTELY NOTHING.
	/*484,4*/	const char* currText; // Pointer to NERPsMessage raw text
	/*488,4*/	real32 flashTimer;
	/*48c,4*/	Gods98::TextWindow* textOnlyWindow;
	/*490,4*/	Gods98::TextWindow* textImageWindow; // Same as textOnlyWindow, but used instead when there's an image.
	/*494,4*/	undefined4 reserved1;
	/*498,4*/	uint32 msgYPos;
	/*49c,10*/	Area2F MsgPanel_Rect1;
	/*4ac,4*/	real32 winLowPos;
	/*4b0,4*/	real32 MsgPanel_Float20;
	/*4b4,4*/	real32 msgXPos;
	/*4b8,4*/	real32 winFontHeightOffs;
	/*4bc,10*/	Area2F MsgPanel_Rect2;
	/*4cc,8*/	Point2I TextImagePosition;
	/*4d4,4*/	Text_GlobFlags TextPanelFlags; // (0x1: ?, 0x4: ?)
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
void __cdecl Text_Load(Gods98::TextWindow* textOnlyWindow, Gods98::TextWindow* textImageWindow, sint32 imageX, sint32 imageY, real32 pauseTime);

// <LegoRR.exe @0046ac10>
void __cdecl Text_Initialise(const char* filename, uint32 x, uint32 y, uint32 unused_int, const Area2F* msgRect, const Area2F* unused_rect, real32 param_7);

// <LegoRR.exe @0046aca0>
void __cdecl Text_UpdatePositionAndSize(void);

// <LegoRR.exe @0046ad50>
//#define Text_Clear ((void (__cdecl* )(void))0x0046ad50)
void __cdecl Text_Clear(void);

// <LegoRR.exe @0046ad90>
bool32 __cdecl Text_GetTextType(const char* textName, OUT Text_Type* textType);

// <LegoRR.exe @0046add0>
void __cdecl Text_SetMessage(Text_Type textType, const char* textMessage);

// <LegoRR.exe @0046ae70>
void __cdecl Text_SetMessageWithImage(Text_Type textType, const char* message, const char* filename, const char* sfxName);

// <LegoRR.exe @0046aee0>
void __cdecl Text_SetNERPsMessage(const char* text, uint32 unusedCounter);

// <LegoRR.exe @0046af20>
void __cdecl Text_DisplayMessage(Text_Type textType, bool32 changeTiming, bool32 setFlagFlashing);

// <LegoRR.exe @0046afc0>
void __cdecl Text_Update(real32 elapsedAbs);

#pragma endregion

}
