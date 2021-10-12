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

struct IDirectDraw;
struct IDirectDrawSurface4;
struct IDirect3DRM3;
struct IDirect3DRMDevice3;
struct IDirect3DDevice3;
enum _D3DRENDERSTATETYPE;
typedef enum _D3DRENDERSTATETYPE D3DRENDERSTATETYPE;
static_assert(sizeof(D3DRENDERSTATETYPE) == 0x4, "");
struct tagRECT;
typedef struct tagRECT RECT;

#pragma endregion


namespace Gods98
{; // !<---

/**********************************************************************************
 ******** Forward Declarations
 **********************************************************************************/

#pragma region Forward Declarations

struct DirectDraw_Device;

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

#define MAIN_MAXRENDERSTATES				200

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
	MAIN_FLAG_VIDEOTEXTURE				= 0x4,			// (runtime) Set if `device->flags & DIRECTDRAW_FLAG_DEVICE_VIDEOTEXTURE`
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
	MAIN_FLAG_DUALMOUSE					= 0x4000,		// -nodualmouse : (negate) 
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


enum_scoped(MainQuality) : sint32
{
	WIREFRAME      = 0,
	UNLITFLATSHADE = 1, // (not parsed in Lego.cfg)
	FLATSHADE      = 2,
	GOURAUDSHADE   = 3,
	PHONGSHADE     = 4, // (CUSTOM: add remaining render quality combination, does this have any use?)
};
enum_scoped_end(MainQuality, 0x4);


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


struct Main_StateChangeData
{
	/*0,4*/ uint32 origValue;
	/*4,4*/ bool32 changed;
	/*8*/
};
assert_sizeof(Main_StateChangeData, 0x8);


struct Main_Globs
{
	/*000,4*/	HWND hWnd;					// Handle for the LegoRR window
	/*004,4*/	HINSTANCE hInst;			// Handle for the LegoRR module/instance
	/*008,4*/	bool32 active;				// App has focus (forced to true for FullScreen)
	/*00c,4*/	bool32 exit;				// App will quit at the end of the current WinMain loop
	/*010,4*/	const char* className;		// Name of the window class
	/*014,100*/	char programName[256];		// Name of the program (with ".exe" stripped)
	/*114,4*/	IDirect3DRM3* lpD3DRM;		// D3D (retained) engine
	/*118,4*/	IDirect3DRMDevice3* device;	// D3D (retained) device
	/*11c,4*/	IDirect3DDevice3* imDevice;	// D3D (immediate) engine
	/*120,4*/	uint32 fogMethod;			// D3D (retained) scene fog method
	/*124,4*/	uint32 appWidth;			// App resolution width
	/*128,4*/	uint32 appHeight;			// App resolution height
	/*12c,c*/	Main_State currState;		// State holding Initialise/MainLoop/Shutdown callbacks
	/*138,4*/	bool32 stateSet;			// currState has been initialised
	/*13c,4*/	real32 fixedFrameTiming;	// Enforced elapsed-time passed to currState.MainLoop
	/*140,640*/	Main_StateChangeData renderStateData[MAIN_MAXRENDERSTATES];	// D3D (retained) render states
	/*780,4*/	uint32 style;				// Window (normal) style
	/*784,4*/	MainFlags flags;			// Cmdline settings and dynamic runtime flags
	/*788,4*/	uint32 programmerLevel;
	/*78c,80*/	char startLevel[128];
	/*80c,80*/	char languageName[128];
	/*88c,4*/	MainCLFlags clFlags;		// Cmdline -flags for enabling WIP features
	/*890,4*/	HACCEL accels;				// Gods98 functionality has been modified and extended.
	/*894,4*/	MainWindowCallback windowCallback;
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

	/// RENDER:
	// All options passed to Main_Setup3D
	//  (most of these can't really be used as the Mesh module currently is)
	MainQuality			renderQuality;
	bool32				dither;
	bool32				linearFilter;
	bool32				mipMap;
	bool32				mipMapLinear;
	bool32				blendTransparency;
	bool32				sortTransparency;
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
 ******** Functions
 **********************************************************************************/

#pragma region Functions

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



// <inlined>
__inline IDirect3DRM3* lpD3DRM(void) { return mainGlobs.lpD3DRM; }

// <inlined>
__inline IDirect3DRMDevice3* lpDevice(void) { return mainGlobs.device; }

// <inlined>
__inline IDirect3DDevice3* lpIMDevice(void) { return mainGlobs.imDevice; }

// <inlined>
__inline bool32 Main_VideoTexture(void) { return mainGlobs.flags & MainFlags::MAIN_FLAG_VIDEOTEXTURE; }

// <inlined>
__inline bool32 Main_MIPMapEnabled(void) { return mainGlobs.flags & MainFlags::MAIN_FLAG_MIPMAPENABLED; }

// <inlined>
__inline bool32 Main_FullScreen(void) { return mainGlobs.flags & MainFlags::MAIN_FLAG_FULLSCREEN; }

// <inlined>
__inline uint32 Main_GetFogMethod(void) { return mainGlobs.fogMethod; }

// <inlined>
__inline void Main_SetFogMethod(uint32 m) { mainGlobs.fogMethod = m; }

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


// <LegoRR.exe @00477e90>
void __cdecl Main_DisableTextureManagement(void);

// <LegoRR.exe @00477eb0>
void __cdecl Main_ParseCommandLine(const char* lpszCmdLine, OUT bool32* nosound, OUT bool32* insistOnCD);


// <missing>
const char* __cdecl Main_GetLanguage(void);

// <missing>
bool32 __cdecl Main_DumpUnknownPhrases(void);


// <LegoRR.exe @004781f0>
void __cdecl Main_LoopUpdate(bool32 clear);

// <LegoRR.exe @00478230>
__inline MainCLFlags Main_GetCLFlags(void) { return mainGlobs.clFlags; }
MainCLFlags __cdecl noinline(Main_GetCLFlags)(void);

// <LegoRR.exe @00478240>
uint32 __cdecl Main_GetWindowsBitDepth(void);

// <LegoRR.exe @00478260>
void __cdecl Main_Finalise3D(void);

// <LegoRR.exe @00478290>
bool32 __cdecl Main_SetState(Main_State* state);

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

// <LegoRR.exe @00478490>
bool32 __cdecl Main_SetupDirect3D(const DirectDraw_Device* dev, IDirectDraw* ddraw1, IDirectDrawSurface4* backSurf, bool32 doubleBuffered);

// <LegoRR.exe @004785d0>
void __cdecl Main_AdjustWindowRect(IN OUT Rect2I* rect);

// <LegoRR.exe @004785f0>
void __cdecl Main_Setup3D(MainQuality renderQuality, bool32 dither, bool32 linearFilter, bool32 mipMap,
						  bool32 mipMapLinear, bool32 blendTransparency, bool32 sortTransparency);

// <LegoRR.exe @00478690>
void __cdecl Main_SetTitle(const char* title);


// <missing>
uint32 __cdecl Main_GetTrianglesDrawn(bool32 total);


// <LegoRR.exe @004786b0>
bool32 __cdecl Main_InitApp(HINSTANCE hInstance);

// <LegoRR.exe @00478780>
LRESULT __cdecl Main_WndProc_Fullscreen(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// <LegoRR.exe @00478980>
LRESULT __cdecl Main_WndProc_Windowed(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// <LegoRR.exe @00478b40>
LRESULT __stdcall Main_WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// <LegoRR.exe @00478b90>
void __cdecl Main_ChangeRenderState(D3DRENDERSTATETYPE dwRenderStateType, uint32 dwRenderState);

/// NOTE: newer engine version has argument: BOOL force, but LegoRR DOES NOT have this argument.
///        (it's possible this argument has been inlined, as it negates calling the entire function body.)
// <LegoRR.exe @00478c00>
void __cdecl Main_RestoreStates(void);


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

/*__inline bool32 Main_SetCDVolume(real32 leftVolume, real32 rightVolume) {
	return Main_CDVolume(&leftVolume, &rightVolume, true);
}
__inline bool32 Main_GetCDVolume(OUT real32* leftVolume, OUT real32* rightVolume) {
	return Main_CDVolume(leftVolume, rightVolume, false);
}*/


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