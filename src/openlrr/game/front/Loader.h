// Loader.h : 
//

#pragma once

#include "../../common.h"
#include "../../engine/colour.h"
#include "../../engine/geometry.h"
#include "../../engine/undefined.h"

#include "../../engine/gfx/Containers.h"
#include "../../engine/drawing/Fonts.h"
#include "../../engine/drawing/Images.h"

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

enum Loader_GlobFlags : uint32 // [LegoRR/Loader.c|flags:0x4|type:uint]
{
	LOADER_GLOB_FLAG_NONE    = 0,
	LOADER_GLOB_FLAG_ENABLED = 0x1,
};
flags_end(Loader_GlobFlags, 0x4);

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

struct LoaderSection // [LegoRR/Loader.c|struct:0xc]
{
	/*0,4*/	char* name; // Name of the section files are being loaded from
	/*4,4*/	uint32 currentSize; // Current total size of files loaded for this section
	/*8,4*/	uint32 totalSize; // Predefined total "expected" size for files to load from this section
	/*c*/
};
assert_sizeof(LoaderSection, 0xc);


struct Loader_Globs // [LegoRR/Loader.c|struct:0x290|tags:GLOBS]
{
	/*000,4*/	Gods98::Image* LoadScreen;
	/*004,4*/	Gods98::Font* font;
	/*008,258*/	LoaderSection sectionList[50];
	/*260,4*/	LoaderSection* current; // current section being loaded
	/*264,4*/	Gods98::Image* ShutdownScreen;
	/*268,4*/	Gods98::Image* ProgressBar;
	/*26c,10*/	Area2F ProgressWindow;
	/*27c,4*/	Direction ProgressDirection; // expand direction of progress bar: U, R, D, L
	/*280,4*/	char* LoadingText;
	/*284,4*/	uint32 LoadingWidth; // measured width of font with LoadingText
	/*288,4*/	real32 progressLast; // percentage of filesize for section (stores percent of last render)
	/*28c,4*/	Loader_GlobFlags flags; // (1 = show loading bar)
	/*290*/
};
assert_sizeof(Loader_Globs, 0x290);

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @00556e00>
extern Loader_Globs & loaderGlobs;

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions


#pragma endregion

}
