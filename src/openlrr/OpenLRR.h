// OpenLRR.h : 
//

#pragma once

#include "platform/windows.h"

#include "engine/Main.h"

#include "common.h"


/**********************************************************************************
 ******** Enumerations
 **********************************************************************************/

#pragma region Enums

// Method used to run OpenLRR
enum class InjectMethod
{
	Unknown,
	NotAttached, // There is no `LegoRR.exe`. OpenLRR is running on its own
	Injector,    // OpenLRR-Injector method. Launcher program that safely injects `openlrr.dll` before `LegoRR.exe` can run
	DllImport,   // Community Edition method. An existing executable that imports `openlrr.dll`
	DllStart,    // OpenRCT2 method. An existing executable that imports `openlrr.dll` and calls `StartOpenLRR`
};

// User-selectable icon to display with the window.
enum class OpenLRRIcon
{
	None,
	Native,		// Whatever is present at resource 113 in the native-loaded executable.
	OpenLRR,	// Teal rim with "OR" letters
	Teal,		// Teal rim						(not included by default)
	Gold,		// Gold rim						(not included by default)
	CDROM,		// Teal rim with "RR" letters	(not included by default)

	Count,
};

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

// Global state variables for OpenLRR
struct OpenLRR_Globs
{
	HINSTANCE		hInstDll;  // DLL instance handle passed by `DllMain`
	HINSTANCE		hInstMain; // EXE instance handle passed by `WinMain`
	FILE*			conout;    // CONOUT$ file opened by `MakeConsole`
	InjectMethod	method;    // How OpenLRR has been injected and attached to `LegoRR.exe`

	HICON			iconList[(uint32)OpenLRRIcon::Count];
	HMENU			menu;
	HACCEL			accels;

	Gods98::Main_State	legoState; // Main_State assigned by Lego_Gods_Go
	char			legoProgramName[256]; // Copied program name that is ensured to start with Lego*

	// Game variables backup:
	uint32			orig_programmerLevel;

	bool			wobblyWorld; // Fun mode where ALL blocks use the same texture "UV Wobbles" effect as lava and water.
};

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

extern OpenLRR_Globs openlrrGlobs;

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region I/O Functions

// DLL instance handle passed by `DllMain`.
inline HINSTANCE OpenLRR_hInstDll(void)		{ return openlrrGlobs.hInstDll; }
// EXE instance handle passed by `WinMain`.
inline HINSTANCE OpenLRR_hInstMain(void)	{ return openlrrGlobs.hInstMain; }

// How OpenLRR has been injected and attached to `LegoRR.exe`.
inline InjectMethod OpenLRR_GetMethod(void)	{ return openlrrGlobs.method; }
// State how OpenLRR has been injected and attached to `LegoRR.exe`.
inline InjectMethod OpenLRR_SetMethod(InjectMethod newMethod) { return openlrrGlobs.method = newMethod; }


// Gets if the wobbly world fun effects are enabled.
inline bool OpenLRR_IsWobblyWorld(void) { return openlrrGlobs.wobblyWorld; }
// Sets if the wobbly world, fun effects are enabled. This needs some hardcoded handling to turn off mid-game.
void OpenLRR_SetWobblyWorld(bool on);
// Called during `OpenLRR_MainLoop_Wrapper` to forcefully make all block textures wobble.
void OpenLRR_UpdateWobblyWorld(void);


bool32 __cdecl OpenLRR_Initialise_Wrapper(void);
bool32 __cdecl OpenLRR_MainLoop_Wrapper(real32 elapsed);
void __cdecl OpenLRR_Shutdown_Wrapper(void);


// Open a console window for easy logging with `printf`.
FILE* MakeConsole(void);

// Inject openlrr.dll into LegoRR.exe by hooking `WinMain` with `RunOpenLRR`.
bool InjectOpenLRR(HINSTANCE hInstanceDll);

// Perform OpenLRR initialisation then call `Gods98::Main_WinMain`.
sint32 __stdcall StartOpenLRRInjected(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, sint32 cmdShow);
extern "C" __declspec(dllexport) sint32 __cdecl StartOpenLRR(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, sint32 cmdShow);

// Community Edition requires that this function be exported by lrrce.dll.
// This requires `extern "C"` and `__cdecl` to avoid function name mangling.
// Do not remove this, as launching via the Community Edition method is still supported!
//extern "C" __declspec(dllexport) void __cdecl Dummy(void);

#pragma endregion
