// Loader.h : 
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

// Default section name for all game data loaded on game boot.
#define LOADER_SECTION_GAMEDATA			"Game Data"

// Loader profile filename (total file sizes of loaded data when sound is enabled).
#define LOADER_PROFILE_NAME				"LoaderProfile.txt"
// Loader profile filename (total file sizes of loaded data when sound is disabled).
#define LOADER_PROFILE_NOSOUND_NAME		"LoaderProfileNoSound.txt"


// LOW HARDCODED LIMIT!! 49 levels + 1 section for game data :(
#define LOADER_MAXSECTIONS				50

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
	/*008,258*/	LoaderSection sectionList[LOADER_MAXSECTIONS]; // Very bad hardcoded list size. No more than 49 levels and 1 for Game Data.
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
 ******** Macros
 **********************************************************************************/

#pragma region Macros

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @0044de10>
void __cdecl Loader_Initialise(const char* loadScreenName, const char* shutdownScreenName, Gods98::Font* font5HI, const char* profileName, Direction progressDirection, const char* progressBarName, const Area2F* progressWindow, const char* loadingText);

// <LegoRR.exe @0044e000>
void __cdecl Loader_Shutdown(const char* filename);

// Sets the current section being loaded. Set to nullptr to disable loader screen.
// <LegoRR.exe @0044e0a0>
void __cdecl Loader_display_loading_bar(const char* section);

// Callback when a data file is loaded, which is used to add to the load size/progress of the currently active section.
// <LegoRR.exe @0044e180>
void __cdecl Loader_FileLoadCallback(const char* filename, uint32 fileSize, void* data);

// Displays the "Shutdown" screen image (only seen during a full shutdown with -programmer 10).
// <LegoRR.exe @0044e360>
void __cdecl Loader_display_shutdown(void);

#pragma endregion

}
