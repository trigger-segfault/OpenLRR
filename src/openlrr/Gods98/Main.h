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

namespace _ns_MainFlags {
enum MainFlags : uint32
{
	MAIN_FLAG_NONE						= 0,

	MAIN_FLAG_UPDATED					= 0x1,
	MAIN_FLAG_FULLSCREEN				= 0x2,
	MAIN_FLAG_VIDEOTEXTURE				= 0x4,
	MAIN_FLAG_MIPMAPENABLED				= 0x8,
	MAIN_FLAG_PAUSED					= 0x10,
	MAIN_FLAG_DONTMANAGETEXTURES		= 0x20,
	MAIN_FLAG_BEST						= 0x40,
	MAIN_FLAG_DUMPMODE					= 0x80,
	MAIN_FLAG_WINDOW					= 0x100,
	MAIN_FLAG_STARTLEVEL				= 0x200,
	MAIN_FLAG_CLEANSAVES				= 0x400,
	MAIN_FLAG_SAVELANGFILE				= 0x800,
	MAIN_FLAG_LANGDUMPUNKNOWN			= 0x1000,
	MAIN_FLAG_DEBUGMODE					= 0x2000,
	MAIN_FLAG_DUALMOUSE					= 0x4000,
	MAIN_FLAG_DEBUGCOMPLETE				= 0x8000,
	MAIN_FLAG_TESTERCALL				= 0x10000,
	MAIN_FLAG_LEVELSOPEN				= 0x20000,
	MAIN_FLAG_FORCETEXTUREMANAGEMENT	= 0x40000,
	MAIN_FLAG_FORCEVERTEXFOG			= 0x80000,
	MAIN_FLAG_REDUCESAMPLES				= 0x100000,
	MAIN_FLAG_SHOWVERSION				= 0x200000,
	MAIN_FLAG_REDUCEANIMATION			= 0x400000,
	MAIN_FLAG_REDUCEPROMESHES			= 0x800000,
	MAIN_FLAG_REDUCEFLICS				= 0x1000000,
	MAIN_FLAG_REDUCEIMAGES				= 0x2000000,
};
DEFINE_ENUM_FLAG_OPERATORS(MainFlags);
static_assert(sizeof(MainFlags) == 0x4, "");
} using MainFlags = _ns_MainFlags::MainFlags;


namespace _ns_MainQuality {
enum MainQuality : sint32
{
	WIREFRAME      = 0,
	UNLITFLATSHADE = 1, // (not parsed in Lego.cfg)
	FLATSHADE      = 2,
	GOURAUDSHADE   = 3,
};
static_assert(sizeof(MainQuality) == 0x4, "");
} using MainQuality = _ns_MainQuality::MainQuality;

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
static_assert(sizeof(Main_State) == 0xc, "");


struct Main_StateChangeData
{
	/*0,4*/ uint32 origValue;
	/*4,4*/ bool32 changed;
	/*8*/
};
static_assert(sizeof(Main_StateChangeData) == 0x8, "");


struct Main_Globs
{
	/*000,4*/ HWND hWnd;
	/*004,4*/ HINSTANCE hInst;
	/*008,4*/ bool32 active;
	/*00c,4*/ bool32 exit;
	/*010,4*/ const char* className;
	/*014,100*/ char programName[256];
	/*114,4*/ IDirect3DRM3* lpD3DRM;
	/*118,4*/ IDirect3DRMDevice3* device;
	/*11c,4*/ IDirect3DDevice3* imDevice;
	/*120,4*/ uint32 fogMethod;
	/*124,4*/ uint32 appWidth;
	/*128,4*/ uint32 appHeight;
	/*12c,c*/ Main_State currState;
	/*138,4*/ bool32 stateSet;
	/*13c,4*/ real32 fixedFrameTiming;
	/*140,640*/ Main_StateChangeData renderStateData[MAIN_MAXRENDERSTATES];
	/*780,4*/ uint32 style;
	/*784,4*/ MainFlags flags;
	/*788,4*/ uint32 programmerLevel;
	/*78c,80*/ char startLevel[128];
	/*80c,80*/ char languageName[128];
	/*88c,4*/ uint32 clFlags;
	/*890,4*/ HACCEL accels;
	/*894,4*/ MainWindowCallback windowCallback;
	/*898*/

};
static_assert(sizeof(Main_Globs) == 0x898, "");

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @00506800>
extern Main_Globs & mainGlobs;

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

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
uint32 __cdecl Main_GetCLFlags(void);

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
void __cdecl Main_AdjustWindowRect(IN OUT RECT* rect);

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
void __cdecl Main_ChangeRenderState(D3DRENDERSTATETYPE dwRenderStateType, DWORD dwRenderState);

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

// <missing>
void __cdecl Main_SetAccel(HACCEL accels);

#pragma endregion

}
