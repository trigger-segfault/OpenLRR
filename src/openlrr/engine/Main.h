// Main.h : 
//
/// APIS: IDirect3DRM3, IDirect3DRMDevice3, 
///       IDirect3DDevice3
///       IDirectDraw4, IDirectDrawSurface4
/// DEPENDENCIES: Animation, Config, DirectDraw, Draw, Files, Input, Registry,
///               Sound, Utils, Init, Gods_Go, (Dxbug, Errors, Fonts, Images, Memory)
/// DEPENDENTS: ...

#pragma once

#include "../platform/windows.h"

#include "../common.h"
#include "geometry.h"


/**********************************************************************************
 ******** Forward Global Namespace Declarations
 **********************************************************************************/

#pragma region Forward Declarations

struct IDirect3DRM3;
struct IDirect3DRMDevice3;
struct IDirect3DDevice3;

#pragma endregion


namespace Gods98
{; // !<---

/**********************************************************************************
 ******** Forward Declarations
 **********************************************************************************/

#pragma region Forward Declarations

#pragma endregion

/**********************************************************************************
 ******** Function Typedefs
 **********************************************************************************/

#pragma region Function Typedefs

typedef bool32 (__cdecl* MainStateInitialise)(void);
typedef bool32 (__cdecl* MainStateMainLoop)(real32 elapsedTime);
typedef void (__cdecl* MainStateShutdown)(void);
// (unused)
typedef void (__cdecl* MainWindowCallback)(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

#pragma endregion

/**********************************************************************************
 ******** Constants
 **********************************************************************************/

#pragma region Constants

#define GRAPHICS_MAXRENDERSTATES			200

#pragma endregion

/**********************************************************************************
 ******** Enumerations
 **********************************************************************************/

#pragma region Enums

flags_scoped(MainFlags) : uint32
{
	MAIN_FLAG_NONE						= 0,

	MAIN_FLAG_UPDATED					= 0x1,			// (runtime) D3D (retained) engine has been updated this loop
	MAIN_FLAG_FULLSCREEN				= 0x2,			// (runtime) App is in fullscreen mode
	MAIN_FLAG_VIDEOTEXTURE				= 0x4,			// (runtime) Set if `device->flags & GRAPHICS_DEVICE_FLAG_VIDEOTEXTURE`
	MAIN_FLAG_MIPMAPENABLED				= 0x8,			// (runtime) 
	MAIN_FLAG_PAUSED					= 0x10,			// (runtime) Game loop elapsed time is 0.0 (different from in-game pause)
	MAIN_FLAG_DONTMANAGETEXTURES		= 0x20,			// -nm : (runtime: Set if not -ftm)
	MAIN_FLAG_BEST						= 0x40,			// -best : Choose best-fit driver/device/screen mode on startup
	MAIN_FLAG_DUMPMODE					= 0x80,			// (runtime)
	MAIN_FLAG_WINDOW					= 0x100,		// -window : App WILL launch in windowed mode with -best
	MAIN_FLAG_STARTLEVEL				= 0x200,		// -startlevel : mainGlobs.startLevel buffer is valid
	MAIN_FLAG_CLEANSAVES				= 0x400,		// -cleansaves : Remove all save data
	MAIN_FLAG_SAVELANGFILE				= 0x800,		// -langsuffix : (disabled)
	MAIN_FLAG_LANGDUMPUNKNOWN			= 0x1000,		// -langdump : (disabled)
	MAIN_FLAG_DEBUGMODE					= 0x2000,		// -debug : Basic debug mode features
	MAIN_FLAG_DUALMOUSE					= 0x4000,		// -dualmouse : 
	MAIN_FLAG_DEBUGCOMPLETE				= 0x8000,		// -debugcomplete : full debug mode features, triggers -debug
	MAIN_FLAG_TESTERCALL				= 0x10000,		// -testercall : Similar to -testlevels, but with extended behaviour
	MAIN_FLAG_LEVELSOPEN				= 0x20000,		// -testlevels : All levels are unlocked
	MAIN_FLAG_FORCETEXTUREMANAGEMENT	= 0x40000,		// -ftm : 
	MAIN_FLAG_FORCEVERTEXFOG			= 0x80000,		// -fvf : 
	MAIN_FLAG_REDUCESAMPLES				= 0x100000,		// -reducesamples : '!' key prefix ignores Samples
	MAIN_FLAG_SHOWVERSION				= 0x200000,		// -showversion : 
	MAIN_FLAG_REDUCEANIMATION			= 0x400000,		// -reduceanimation : '!' key prefix ignores Animations
	MAIN_FLAG_REDUCEPROMESHES			= 0x800000,		// -reducepromeshes : '!' key prefix ignores Promeshes(?)
	MAIN_FLAG_REDUCEFLICS				= 0x1000000,	// -reduceflics : '!' key prefix ignores Flics (.flh)
	MAIN_FLAG_REDUCEIMAGES				= 0x2000000,	// -reduceimages : '!' key prefix ignores Images
};
flags_scoped_end(MainFlags, 0x4);


// Experimental commandline flags used by LegoRR
flags_scoped(MainCLFlags) : uint32
{
	CL_FLAG_NONE		= 0,

	// Blocks will fade between textures. Looks neat with Power Paths lighting up with power.
	// Only seems to work correctly with -ftm(?) commandline option.
	// Otherwise there's just a delay, then instant-texture-change.
	CL_FLAG_BLOCKFADE	= 0x8000,
};
flags_scoped_end(MainCLFlags, 0x4);


// We can safely hardcode these values since D3DRM won't change
// same as enum Main_Quality (just different symbol names)
// (unused for now, until Graphics can be untangled from Main)
enum class Graphics_FogMethod : uint32
{
	Vertex = 0x1, // = D3DRMFOGMETHOD_VERTEX,
	Table  = 0x2, // = D3DRMFOGMETHOD_TABLE,
	Any    = 0x4, // = D3DRMFOGMETHOD_ANY,
};
assert_sizeof(Graphics_FogMethod, 0x4);


// same as enum Main_Quality (just different symbol names)
enum class Graphics_Quality : uint32
{
	Wireframe = 0, // (placeholder: D3DRMRENDER_WIREFRAME)
	UnlitFlat = 1, // (placeholder: D3DRMRENDER_UNLITFLAT) lighting not supported by immediate mode, same as Flat
	Flat      = 2, // (placeholder: D3DRMRENDER_FLAT)
	Gouraud   = 3, // (placeholder: D3DRMRENDER_GOURAUD)
	Phong     = 4, // (placeholder: D3DRMRENDER_PHONG) CUSTOM: add remaining render quality combination, does this have any use?

	/*/// OLD:
	WIREFRAME      = 0,
	UNLITFLATSHADE = 1,
	FLATSHADE      = 2,
	GOURAUDSHADE   = 3,
	PHONGSHADE     = 4,*/
};
assert_sizeof(Graphics_Quality, 0x4);


/// CUSTOM: Behaviour for correcting 100% invisible mouse cursor in windowed mode.
enum class CursorVisibility
{
	Never,		// Never show the mouse cursor over the window or its border.
	TitleBar,	// Show mouse cursor over the title bar (and system menu).
	Always,		// Always show mouse cursor, even when inside the client area.

	Count,

	Default = Never,
};

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

struct Main_State
{
	/*0,4*/ MainStateInitialise Initialise;
	/*4,4*/ MainStateMainLoop MainLoop;
	/*8,4*/ MainStateShutdown Shutdown;
	/*c*/
};
assert_sizeof(Main_State, 0xc);


struct Graphics_StateChangeData
{
	/*0,4*/ uint32 origValue;
	/*4,4*/ bool32 changed;
	/*8*/
};
assert_sizeof(Graphics_StateChangeData, 0x8);


struct Main_Globs
{
	/*000,4*/	HWND hWnd;					// [WND] Handle for the LegoRR window
	/*004,4*/	HINSTANCE hInst;			// [WND] Handle for the LegoRR module/instance

	// [mandatory address]
	/*008,4*/	bool32 active;				// App has focus (forced to true for FullScreen)

	/*00c,4*/	bool32 exit;				// App will quit at the end of the current WinMain loop
	/*010,4*/	const char* className;		// [WND] Name of the window class
	/*014,100*/	char programName[256];		// Name of the program (with ".exe" stripped)
	/*114,4*/	IDirect3DRM3* lpD3DRM;		// [D3DRM] engine
	/*118,4*/	IDirect3DRMDevice3* device;	// [D3DRM] device
	/*11c,4*/	IDirect3DDevice3* imDevice;	// [D3D] engine
	/*120,4*/	/*Graphics_FogMethod*/ uint32 fogMethod;	// [D3DRM] scene fog method

	// [mandatory address]
	/*124,4*/	uint32 appWidth;			// App resolution width
	/*128,4*/	uint32 appHeight;			// App resolution height

	/*12c,c*/	Main_State currState;		// State holding Initialise/MainLoop/Shutdown callbacks
	/*138,4*/	bool32 stateSet;			// currState has been initialised

	// [mandatory address]
	/*13c,4*/	real32 fixedFrameTiming;	// Enforced elapsed-time passed to currState.MainLoop

	/*140,640*/	Graphics_StateChangeData renderStateData[GRAPHICS_MAXRENDERSTATES];	// D3D (retained) render states
	/*780,4*/	uint32 style;				// [WND] Window (normal) style

	// [mandatory address]
	/*784,4*/	MainFlags flags;			// Command line options and dynamic runtime flags
	/*788,4*/	uint32 programmerLevel;

	/*78c,80*/	char startLevel[128];
	/*80c,80*/	char languageName[128];
	/*88c,4*/	MainCLFlags clFlags;		// Cmdline -flags for enabling WIP features
	/*890,4*/	HACCEL accels;				// [WND] Gods98 functionality has been modified and extended.
	/*894,4*/	MainWindowCallback windowCallback; // [WND]
	/*898*/
};
assert_sizeof(Main_Globs, 0x898);


/// CUSTOM: Extension of `Main_Globs` for OpenLRR-exclusive globals
struct Main_Globs2
{
	/// STATE:
	bool32				cmdlineParsed;	// Main_ParseCommandLine has been called
	bool32				displaySetup;	// Main_SetupDisplay has been called

	/// USER-SPECIFIED:
	HICON				icon;			// Icon assigned to the titlebar and Windows taskbar
	HMENU				menu;			// System menu shown at the top of the window
	HCURSOR				cursor;			// (unused) Cursor shown over the window client area
	bool32				iconOwner;		// Cleanup is performed on icon
	bool32				menuOwner;		// Cleanup is performed on menu
	bool32				cursorOwner;	// Cleanup is performed on cursor
	bool32				accelsOwner;	// Cleanup is performed on accels (accels is in mainGlobs)

	/// FIXES:
	CursorVisibility	cursorVisibility;	// Mouse cursor visibilty fix behaviour

	/// CONTROL:
	sint32				advanceFrames;	// Number of frames to advance when in the Main_IsPaused state

	uint32				windowScale;
};

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @00506800>
extern Main_Globs & mainGlobs;

/// CUSTOM: Extension of `Main_Globs` for OpenLRR-exclusive globals
extern Main_Globs2 mainGlobs2;

#pragma endregion


/**********************************************************************************
 ******** Game Entry point
 **********************************************************************************/

#pragma region Entry point

// NOTE: This function is only declared here. Other modules may choose to implement it.
//  (currently moved to OpenLRR module)
// This is the GAME entry point as called by WinMain,
//  this should hook the Main_State loop functions and only perform basic initial setup.
// <LegoRR.exe @0041f950>
void __cdecl Gods_Go(const char* programName);

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

/// CUSTOM: Scaling for game window (nearest neighbour)
inline sint32 Main_Scale() { return static_cast<sint32>(mainGlobs2.windowScale); }

/// CUSTOM: Checks if the scale creates a window larger than the desktop area size.
bool Main_IsScaleSupported(uint32 windowScale);

/// CUSTOM: Changes the current game window scale.
void Main_SetScale(uint32 windowScale);

/// CUSTOM: Adjust rect x,y position so that the bottom,right isn't going off the screen. (Only checks desktop size!!)
void Main_AdjustWindowPosition(IN OUT Rect2I* rect);


/// CUSTOM:
__inline HACCEL Main_GetAccel(void) { return mainGlobs.accels; }

/// CUSTOM:
__inline HICON Main_GetIcon(void) { return mainGlobs2.icon; }

/// CUSTOM:
__inline HMENU Main_GetMenu(void) { return mainGlobs2.menu; }

/// CUSTOM: Get type of behaviour for mouse cursor visibility fix.
__inline CursorVisibility Main_GetCursorVisibility(void) { return mainGlobs2.cursorVisibility; }

/// CUSTOM: Get if Main_InitApp has been called. (just checks hWnd)
__inline bool32 Main_IsWindowSetup(void) { return mainGlobs.hWnd != nullptr; }

/// CUSTOM: Get if Main_ParseCommandLine has been called.
__inline bool32 Main_IsCommandLineParsed(void) { return mainGlobs2.cmdlineParsed; }

/// CUSTOM: Get if Main_SetupDisplay has been called.
__inline bool32 Main_IsDisplaySetup(void) { return mainGlobs2.displaySetup; }


/// CUSTOM: A wrapper for the WINAPI Sleep function.
void __cdecl Main_Sleep(uint32 milliseconds);


/// CUSTOM: Gets if the `MAIN_FLAG_SHOWVERSION` flag is set.
__inline bool32 Main_IsShowVersion(void) { return (mainGlobs.flags & MainFlags::MAIN_FLAG_SHOWVERSION); }

/// CUSTOM: Sets if the `MAIN_FLAG_SHOWVERSION` flag is set.
void Main_SetShowVersion(bool32 on);

/// CUSTOM: Gets if the `MAIN_FLAG_LEVELSOPEN` flag is set.
__inline bool32 Main_IsLevelsOpen(void) { return (mainGlobs.flags & MainFlags::MAIN_FLAG_LEVELSOPEN); }

/// CUSTOM: Sets if the `MAIN_FLAG_LEVELSOPEN` flag is set.
void Main_SetLevelsOpen(bool32 on);

/// CUSTOM: Gets if the `MAIN_FLAG_TESTERCALL` flag is set.
__inline bool32 Main_IsTesterCall(void) { return (mainGlobs.flags & MainFlags::MAIN_FLAG_TESTERCALL); }

/// CUSTOM: Sets if the `MAIN_FLAG_TESTERCALL` flag is set.
void Main_SetTesterCall(bool32 on);

/// CUSTOM: Gets if the `MAIN_FLAG_DEBUGMODE` flag is set.
__inline bool32 Main_IsDebugMode(void) { return (mainGlobs.flags & MainFlags::MAIN_FLAG_DEBUGMODE); }

/// CUSTOM: Sets if the `MAIN_FLAG_DEBUGMODE` flag is set.
void Main_SetDebugMode(bool32 on);

/// CUSTOM: Gets if the `MAIN_FLAG_DEBUGCOMPLETE` flag is set.
__inline bool32 Main_IsDebugComplete(void) { return (mainGlobs.flags & MainFlags::MAIN_FLAG_DEBUGCOMPLETE); }

/// CUSTOM: Sets if the `MAIN_FLAG_DEBUGCOMPLETE` flag is set.
void Main_SetDebugComplete(bool32 on);

/// CUSTOM: Gets if the `MAIN_FLAG_DUALMOUSE` flag is set.
__inline bool32 Main_IsDualMouse(void) { return (mainGlobs.flags & MainFlags::MAIN_FLAG_DUALMOUSE); }

/// CUSTOM: Sets if the `MAIN_FLAG_DUALMOUSE` flag is set.
void Main_SetDualMouse(bool32 on);

/// CUSTOM: Gets if the `MAIN_FLAG_DUMPMODE` flag is set.
__inline bool32 Main_IsDumpMode(void) { return (mainGlobs.flags & MainFlags::MAIN_FLAG_DUMPMODE); }

/// CUSTOM: Sets if the `MAIN_FLAG_DUMPMODE` flag is set.
void Main_SetDumpMode(bool32 on);


/// CUSTOM: Gets if the `CL_FLAG_BLOCKFADE` flag is set.
__inline bool32 Main_IsCLBlockFade(void) { return (mainGlobs.clFlags & MainCLFlags::CL_FLAG_BLOCKFADE); }

/// CUSTOM: Sets if the `CL_FLAG_BLOCKFADE` flag is set.
void Main_SetCLBlockFade(bool32 on);


/// CUSTOM: Gets if the specified command line `-flags` value is set.
__inline bool32 Main_IsCLFlag(MainCLFlags clFlag) { return (mainGlobs.clFlags & clFlag); }

/// CUSTOM: Sets if the specified command line `-flags` value is set.
void Main_SetCLFlag(MainCLFlags clFlag, bool32 on);


/// CUSTOM: Gets if the game is set to close gracefully at the end of the next update loop.
__inline bool32 Main_IsClosing(void) { return mainGlobs.exit; }

/// CUSTOM: Sets if the game should close gracefully at the end of the next update loop.
__inline void Main_CloseApp(bool32 close) { mainGlobs.exit = close; }



// Gets if the `MAIN_FLAG_FULLSCREEN` flag is set.
// <inlined>
__inline bool32 Main_FullScreen(void) { return (mainGlobs.flags & MainFlags::MAIN_FLAG_FULLSCREEN); }


// <inlined>
__inline HWND Main_hWnd(void) { return mainGlobs.hWnd; }

// <inlined>
__inline HINSTANCE Main_hInst(void) { return mainGlobs.hInst; }

// <inlined>
__inline bool32 Main_AppActive(void) { return mainGlobs.active; }

// <inlined>
__inline MainFlags Main_GetFlags(void) { return mainGlobs.flags; }


// <LegoRR.exe @00401b30>
uint32 __cdecl noinline(Main_ProgrammerMode)(void);
__inline uint32 Main_ProgrammerMode(void) { return mainGlobs.programmerLevel; }

/// CUSTOM:
__inline void Main_SetProgrammerMode(uint32 mode) { mainGlobs.programmerLevel = mode; }


// <LegoRR.exe @00401b40>
const char* __cdecl noinline(Main_GetStartLevel)(void);
__inline const char* Main_GetStartLevel(void) { return (mainGlobs.flags & MainFlags::MAIN_FLAG_STARTLEVEL) ? mainGlobs.startLevel : nullptr; }

// <LegoRR.exe @00401b70>
sint32 __cdecl noinline(appWidth)(void);
__inline sint32 appWidth(void) { return mainGlobs.appWidth; }

// <LegoRR.exe @00401b80>
sint32 __cdecl noinline(appHeight)(void);
__inline sint32 appHeight(void) { return mainGlobs.appHeight; }


// <LegoRR.exe @00477a60>
sint32 __stdcall Main_WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
							  _In_ LPSTR     lpCmdLine, _In_     sint32    nCmdShow);


// <missing>
void __cdecl Main_Exit(void);


// <LegoRR.exe @00477eb0>
void __cdecl Main_ParseCommandLine(const char* lpCmdLine, OUT bool32* nosound, OUT bool32* insistOnCD);


// <missing>
const char* __cdecl Main_GetLanguage(void);

// <missing>
bool32 __cdecl Main_DumpUnknownPhrases(void);


// <LegoRR.exe @004781f0>
void __cdecl Main_LoopUpdate(bool32 clear);

// <LegoRR.exe @00478230>
__inline MainCLFlags Main_GetCLFlags(void) { return mainGlobs.clFlags; }
MainCLFlags __cdecl noinline(Main_GetCLFlags)(void);

/// CUSTOM:
__inline void Main_SetCLFlags(MainCLFlags clFlags) { mainGlobs.clFlags = clFlags; }


// <LegoRR.exe @00478290>
bool32 __cdecl Main_SetState(const Main_State* state);

// <LegoRR.exe @004782c0>
uint32 __cdecl Main_GetTime(void);


// <missing>
void __cdecl Main_SetFixedFrameTiming(real32 time);


// <LegoRR.exe @004782d0>
bool32 __cdecl Main_DispatchMessage(const MSG* msg);

// <LegoRR.exe @00478300>
void __cdecl Main_HandleIO(void);

// <LegoRR.exe @00478370>
void __cdecl Main_SetupDisplay(bool32 fullScreen, uint32 xPos, uint32 yPos, uint32 width, uint32 height);


// <LegoRR.exe @004785d0>
void __cdecl Main_AdjustWindowRect(IN OUT Rect2I* rect);


// <LegoRR.exe @00478690>
void __cdecl Main_SetTitle(const char* title);


// <LegoRR.exe @004786b0>
bool32 __cdecl Main_InitApp(HINSTANCE hInstance);

// <LegoRR.exe @00478780>
LRESULT __cdecl Main_WndProc_Fullscreen(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// <LegoRR.exe @00478980>
LRESULT __cdecl Main_WndProc_Windowed(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// <LegoRR.exe @00478b40>
LRESULT __stdcall Main_WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


// <missing>
void __cdecl Main_PauseApp(bool32 pause);

// <missing>
bool32 __cdecl Main_IsPaused(void);


// <LegoRR.exe @00478c40>
//bool32 __cdecl noinline(Main_SetCDVolume)(real32 leftVolume, real32 rightVolume);
bool32 __cdecl Main_SetCDVolume(real32 leftVolume, real32 rightVolume);

// <LegoRR.exe @00478c60>
//bool32 __cdecl noinline(Main_GetCDVolume)(OUT real32* leftVolume, OUT real32* rightVolume);
bool32 __cdecl Main_GetCDVolume(OUT real32* leftVolume, OUT real32* rightVolume);

// <LegoRR.exe @00478c80>
bool32 __cdecl Main_CDVolume(IN OUT real32* leftVolume, IN OUT real32* rightVolume, bool32 set);


// <missing>
void __cdecl Main_SetWindowCallback(MainWindowCallback callback);

// This is kept for backwards compatibility,
//  if we ever work with unoptimized Beta builds.
// <missing>
void __cdecl Main_SetAccel1(HACCEL accels);

/// EXTENDED: Added ownership parameter and handling.
void __cdecl Main_SetAccel(HACCEL accels, bool32 owner);

/// CUSTOM:
void __cdecl Main_SetIcon(HICON icon, bool32 owner);

/// CUSTOM:
void __cdecl Main_SetMenu(HMENU menu, bool32 owner);

/// CUSTOM: Set type of behaviour for mouse cursor visibility fix.
void __cdecl Main_SetCursorVisibility(CursorVisibility newCursorVisibility);

/// CUSTOM: Get number of frames advance to when in the Main_IsPaused state (always returns >= 0).
sint32 __cdecl Main_GetAdvanceFrames(void);

/// CUSTOM: Set to advance one frame when in the Main_IsPaused state.
void __cdecl Main_AdvanceFrame(void);

/// CUSTOM: Set number of frames advance to when in the Main_IsPaused state.
void __cdecl Main_SetAdvanceFrames(sint32 frames);

/// CUSTOM: Add number of frames advance to when in the Main_IsPaused state.
void __cdecl Main_AddAdvanceFrames(sint32 frames);

/// CUSTOM: Get if the mouse is currently over a visible section of the main window.
bool32 __cdecl Main_MouseInsideWindow(void);

#pragma endregion

}
