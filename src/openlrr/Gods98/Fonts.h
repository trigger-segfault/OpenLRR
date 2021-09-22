// Fonts.h : 
//
/// APIS: -
/// DEPENDENCIES: Images, (Errors, Memory)
/// DEPENDENTS: Main, TextWindow, Credits, FrontEnd, HelpWindow, Lego, LegoObject
///             Loader, Objective, Panel, Rewards, ToolTip

#pragma once

#include "../common.h"
#include "../Types/geometry.h"


namespace Gods98
{; // !<---

/**********************************************************************************
 ******** Forward Declarations
 **********************************************************************************/

#pragma region Forward Declarations

struct Font;
struct Image;

#pragma endregion

/**********************************************************************************
 ******** Function Typedefs
 **********************************************************************************/

#pragma region Function Typedefs

typedef void (__cdecl* FontRunThroughListsCallback)(Font* font, void* data);

#pragma endregion
	
/**********************************************************************************
 ******** Constants
 **********************************************************************************/

#pragma region Constants

#define FONT_MAXLISTS			32			// 2^32 - 1 possible fonts...

#define FONT_LASTCHARACTER		202			// Last character in the font image.


#define FONT_GRIDWIDTH			10
#define FONT_GRIDHEIGHT			19
#define FONT_MAXSTRINGLEN		1024
#define FONT_MAXIMAGEPERSTRING	10

#pragma endregion

/**********************************************************************************
 ******** Enumerations
 **********************************************************************************/

#pragma region Enums

//#define FONT_FLAG_INITIALISED	0x00000001
//#define FONT_FLAG_WINDOWSET		0x00000002 // (unused)
//#define FONT_FLAG_WINDOWWRAPPED	0x00000004 // (unused)


namespace _ns_Font_GlobFlags {
enum Font_GlobFlags : uint32
{
	FONT_FLAG_NONE = 0,
	FONT_FLAG_INITIALISED = 0x1,
};
DEFINE_ENUM_FLAG_OPERATORS(Font_GlobFlags);
static_assert(sizeof(Font_GlobFlags) == 0x4, "");
} using Font_GlobFlags = _ns_Font_GlobFlags::Font_GlobFlags;

namespace _ns_FontFlags {
enum FontFlags : uint32
{
	FONT_FLAG_NONE = 0, // (unused)

	FONT_FLAG_WINDOWSET = 0x2, // (unused)
	FONT_FLAG_WINDOWWRAPPED = 0x4, // (unused)
};
DEFINE_ENUM_FLAG_OPERATORS(FontFlags);
static_assert(sizeof(FontFlags) == 0x4, "");
} using FontFlags = _ns_FontFlags::FontFlags;

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

struct Font
{
	/*000,4*/ Image* image;
	/*000,be0*/ Area2F posSet[FONT_GRIDWIDTH][FONT_GRIDHEIGHT];
	/*be4,4*/ uint32 fontHeight;
	/*be8,4*/ uint32 tabWidth;
	/*bec,4*/ FontFlags flags;
	/*bf0,4*/ Font* nextFree;
	/*bf4*/
};
static_assert(sizeof(Font) == 0xbf4, "");


struct Font_Globs
{
	/*00,80*/ Font* listSet[FONT_MAXLISTS];		// Fonts list
	/*80,4*/ Font* freeList;
	/*84,4*/ uint32 listCount;						// number of lists.
	/*88,4*/ Font_GlobFlags flags;
	/*8c*/
};
static_assert(sizeof(Font_Globs) == 0x8c, "");

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @00507528>
extern Font_Globs & fontGlobs;

#pragma endregion

/**********************************************************************************
 ******** Macros
 **********************************************************************************/

#ifdef DEBUG
	#define Font_CheckInit()			if (!(fontGlobs.flags & FONT_FLAG_INITIALISED)) Error_Fatal(TRUE, "Error: Font_Intitialise() Has Not Been Called");
#else
	#define Font_CheckInit()
#endif

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

 // <missing>
void __cdecl Font_Initialise(void);

// <missing>
void __cdecl Font_Shutdown(void);


// <LegoRR.exe @00401b90>
uint32 __cdecl noinline(Font_GetStringWidth)(Font* font, const char* msg, ...);

// <LegoRR.exe @00401bc0>
void __cdecl noinline(Font_GetStringInfo)(Font* font, OPTIONAL OUT uint32* width,
								OPTIONAL OUT uint32* lineCount, const char* msg, ...);

// <LegoRR.exe @0047a1a0>
Font* __cdecl Font_Load(const char* fname);

// <LegoRR.exe @0047a410>
void __cdecl Font_VGetStringInfo(const Font* font, OPTIONAL OUT uint32* width,
								OPTIONAL OUT uint32* lineCount, const char* msg, std::va_list args);


// <LegoRR.exe @0047a440>
uint32 __cdecl Font_PrintF(const Font* font, sint32 x, sint32 y, const char* msg, ...);

// <LegoRR.exe @0047a470>
uint32 __cdecl Font_VPrintF(const Font* font, sint32 x, sint32 y, const char* msg, std::va_list args);

// <LegoRR.exe @0047a4a0>
uint32 __cdecl Font_VPrintF2(const Font* font, sint32 x, sint32 y, bool32 render,
									OPTIONAL OUT uint32* lineCount, const char* msg, std::va_list args);

// <LegoRR.exe @0047a730>
uint32 __cdecl Font_OutputChar(const Font* font, sint32 x, sint32 y, char c, bool32 render);

// <LegoRR.exe @0047a7e0>
uint32 __cdecl Font_GetCharWidth(const Font* font, char c);

// <LegoRR.exe @0047a800>
uint32 __cdecl Font_GetHeight(const Font* font);

// <LegoRR.exe @0047a810>
void __cdecl Font_Remove(Font* dead);

// <LegoRR.exe @0047a840>
Font* __cdecl Font_Create(Image* image);

// <LegoRR.exe @0047a880>
void __cdecl Font_AddList(void);


// <missing>
void __cdecl Font_GetBackgroundColour(Font* font, OUT real32* r, OUT real32* g, OUT real32* b);

// <missing>
void __cdecl Font_SetTabWidth(Font* font, uint32 width, bool32 spaces);

// <missing>
void __cdecl Font_RemoveCallback(Font* font, void* data);

// <missing>
void __cdecl Font_RunThroughLists(FontRunThroughListsCallback Callback, void* data);

// (inline form for OpenLRR usage)
__inline uint32 Font_GetStringWidth(Font* font, const char* msg, ...) {
	uint32 width;
	std::va_list args;
	va_start(args, msg);
	Font_VGetStringInfo(font, &width, NULL, msg, args);
	va_end(args);
	return width;
}


// <missing or inlined>
__inline uint32 Font_GetLineCount(Font* font, const char* msg, ...) {
	uint32 lineCount;
	std::va_list args;
	va_start(args, msg);
	Font_VGetStringInfo(font, nullptr, &lineCount, msg, args);
	va_end(args);
	return lineCount;
}

// (inline form for OpenLRR usage)
__inline void Font_GetStringInfo(Font* font, OPTIONAL OUT uint32* width,
	OPTIONAL OUT uint32* lineCount, const char* msg, ...) {
	std::va_list args;
	va_start(args, msg);
	Font_VGetStringInfo(font, width, lineCount, msg, args);
	va_end(args);
}

/*uint32 __cdecl Font_GetLineCount(Font* font, const char* msg, ...);
void __cdecl Font_SetTabWidth(Font* font, uint32 width, bool32 spaces);

void __cdecl Font_GetBackgroundColour(Font* font, OUT real32* r, OUT real32* g, OUT real32* b);
void __cdecl Font_RemoveCallback(Font* font, void* data);
void __cdecl Font_RunThroughLists(FontRunThroughListsCallback Callback, void* data);*/

#pragma endregion

}
