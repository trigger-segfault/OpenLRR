#pragma once

#include "../common.h"
#include "../Types/geometry.h"
#include "Fonts.h"
#include "Maths.h"


namespace Gods98
{; // !<---

/**********************************************************************************
 ******** Constants
 **********************************************************************************/

#pragma region Constants

#define TEXTWINDOW_MAXSTRINGLEN			1024
#define TEXTWINDOW_MAXLINES				256

#define	TEXTWINDOW_DISPLAYDELAY			25

#pragma endregion

/**********************************************************************************
 ******** Enumerations
 **********************************************************************************/

#pragma region Enums

//#define TEXTWINDOW_FLAG_WINDOWWRAPPED	0x00000001
//#define TEXTWINDOW_FLAG_OVERLAY			0x00000002
//#define TEXTWINDOW_FLAG_CENTERED		0x00000004

namespace _ns_TextWindowFlags {
enum TextWindowFlags : uint32
{
	TEXTWINDOW_FLAG_NONE = 0, // (unused)

	TEXTWINDOW_FLAG_WINDOWWRAPPED = 0x1,
	TEXTWINDOW_FLAG_OVERLAY = 0x2,
	TEXTWINDOW_FLAG_CENTERED = 0x4,
};
DEFINE_ENUM_FLAG_OPERATORS(TextWindowFlags);
static_assert(sizeof(TextWindowFlags) == 0x4, "");
} using TextWindowFlags = _ns_TextWindowFlags::TextWindowFlags;

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

struct TextWindow
{
	/*000,4*/ Font* font;
	/*004,10*/ Area2F windowSize;
	/*014,4*/ char* windowBuffer;
	/*018,400*/ char secondaryBuffer[TEXTWINDOW_MAXSTRINGLEN];
	/*418,4*/ uint32 windowBufferSize;
	/*41c,4*/ uint32 windowBufferEnd;
	/*420,400*/ uint32 lineList[TEXTWINDOW_MAXLINES];
	/*820,4*/ uint32 usedLines; // (count)
	/*824,4*/ uint32 maxLines;  // (capacity)
	/*828,4*/ real32 displayDelay;
	/*82c,4*/ TextWindowFlags flags;
	/*830*/
};// TextWindow, * lpTextWindow;
static_assert(sizeof(TextWindow) == 0x830, "");

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @004853b0>
TextWindow* __cdecl TextWindow_Create(Font* font, const Area2F* size, uint32 bufferSize);

// <LegoRR.exe @00485420>
void __cdecl TextWindow_EnableCentering(TextWindow* window, bool32 enable);

// <LegoRR.exe @00485450>
void __cdecl TextWindow_Remove(TextWindow* window);

// <LegoRR.exe @00485470>
void __cdecl TextWindow_ChangePosition(TextWindow* window, sint32 xPos, sint32 yPos);

// <LegoRR.exe @00485490>
void __cdecl TextWindow_ChangeSize(TextWindow* window, uint32 width, uint32 height);

// <LegoRR.exe @004854d0>
void __cdecl TextWindow_PagePrintF(TextWindow* window, uint32 page, const char* msg, ...);

// <LegoRR.exe @004854f0>
void __cdecl TextWindow_PrintF(TextWindow* window, const char* msg, ...);

// <LegoRR.exe @00485510>
void __cdecl TextWindow_VPrintF(TextWindow* window, uint32 page, const char* msg, std::va_list args);

// <LegoRR.exe @004855c0>
void __cdecl TextWindow_PrintFOverlay(TextWindow* window, bool32 oneFrame, const char* msg, ...);

// <LegoRR.exe @00485650>
bool32 __cdecl TextWindow_Update(TextWindow* window, uint32 posFromEnd, real32 time, OUT sint32* lpLowestPoint);

// <LegoRR.exe @004859d0>
bool32 __cdecl TextWindow_UpdateOverlay(TextWindow* window, real32 time, OUT sint32* lpLowestPoint);

// <LegoRR.exe @00485c70>
void __cdecl TextWindow_Clear(TextWindow* window);

// <LegoRR.exe @00485cc0>
void __cdecl TextWindow_GetInfo(TextWindow* window, OUT uint32* linesUsed, OUT uint32* linesInWindow);


// <unused>
sint32 __cdecl TextWindow_GetDrawPos(TextWindow* window, uint32 chrPos, uint32 line, uint32 xpos, bool32 check);

// <unused>
sint32 __cdecl TextWindow_GetRow(TextWindow* window, sint32 chrPos, sint32 dir);

// <unused>
void __cdecl TextWindow_SetFont(TextWindow* window, Font* font);

#pragma endregion

}
