// openlrr.h : 
//

#pragma once

#include "platform/windows.h"

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
};

// Configuration settings for OpenLRR
struct OpenLRR_Config
{
	uint32 dummy; // keep until we have some fields added
};

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

extern OpenLRR_Globs openlrrGlobs;

extern OpenLRR_Config openlrrConfig;

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region I/O Functions

// DLL instance handle passed by `DllMain`.
inline HINSTANCE OpenLRR_hInstDll(void)		{ return openlrrGlobs.hInstDll; }
// EXE instance handle passed by `WinMain`.
inline HINSTANCE OpenLRR_hInstMain(void)	{ return openlrrGlobs.hInstMain; }
// CONOUT$ file opened by `MakeConsole`.
inline FILE* OpenLRR_conout(void)			{ return openlrrGlobs.conout; }
// How OpenLRR has been injected and attached to `LegoRR.exe`.
inline InjectMethod OpenLRR_GetMethod(void)	{ return openlrrGlobs.method; }
// State how OpenLRR has been injected and attached to `LegoRR.exe`.
inline InjectMethod OpenLRR_SetMethod(InjectMethod newMethod) { return openlrrGlobs.method = newMethod; }

// Open a console window for easy logging with `printf`.
FILE* MakeConsole(void);

// Inject openlrr.dll into LegoRR.exe by hooking `WinMain` with `RunOpenLRR`.
bool InjectOpenLRR(HINSTANCE hInstanceDll);

// Perform OpenLRR initialisation then call `Gods98::Main_WinMain`.
extern "C" __declspec(dllexport) sint32 __stdcall StartOpenLRR(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, sint32 cmdShow);

// Community Edition requires that this function be exported by lrrce.dll.
// This requires `extern "C"` and `__cdecl` to avoid function name mangling.
// Do not remove this, as launching via the Community Edition method is still supported!
extern "C" __declspec(dllexport) void __cdecl Dummy(void);

#pragma endregion
