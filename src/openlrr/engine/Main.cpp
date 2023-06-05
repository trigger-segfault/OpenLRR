// Main.cpp : 
//

#include "../platform/windows.h"
#include "../platform/d3drm.h"		// <d3drmwin.h>
#include "../platform/timeapi.h"
#include "../../../resources/resource.h"

#include "geometry.h"

#include "../cmdline/CLGen.h"
#include "../cmdline/CommandLine.hpp"

#include "audio/Sound.h"
#include "video/Animation.h"
#include "core/Config.h"
#include "core/Errors.h"
#include "core/Files.h"
#include "core/Memory.h"
#include "core/Utils.h"
#include "drawing/DirectDraw.h"
#include "drawing/Draw.h"
#include "drawing/Fonts.h"
#include "drawing/Images.h"
#include "input/Input.h"
#include "util/Dxbug.h"
#include "util/Registry.h"
#include "Graphics.h"
#include "Init.h"

#include "Main.h"

#include "../game/Game.h"  // Gods_Go


/**********************************************************************************
 ******** Game Entry point
 **********************************************************************************/

#pragma region Entry point

namespace Gods98
{; // !<---

// This is the GAME entry point as called by WinMain,
//  this should hook the Main_State loop functions and only perform basic initial setup.
// (this can return bool32, but does not)
// (LRR_Go)
// <LegoRR.exe @0041f950>
/*void __cdecl Gods_Go(const char* programName)
{
	((void (__cdecl*)(const char*))0x0041f950)(programName);
}*/

}
#pragma endregion

/**********************************************************************************
 ******** Constants
 **********************************************************************************/

#pragma region Constants

#define MAIN_PRODUCTREGISTRY	"SOFTWARE\\LEGO Media\\Games\\Rock Raiders"

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @00506800>
Gods98::Main_Globs & Gods98::mainGlobs = *(Gods98::Main_Globs*)0x00506800;

Gods98::Main_Globs2 Gods98::mainGlobs2 = { 0 };

Gods98::Main_CommandLineOptions Gods98::mainOptions = Gods98::Main_CommandLineOptions();

// Visual styles with dll: <https://stackoverflow.com/a/25271493/7517185>
static HANDLE visualStyles_hActCtx = INVALID_HANDLE_VALUE;
static ULONG_PTR visualStyles_cookie = 0;

// Store these here to move them out of WinMain loop, allowing to handle update times in other functions.
static real32 _mainDeltaTime = 1.0f;
static uint32 _mainLastTime = 0;
static real64 _mainAccumulatorMS = 0.0f;
static real32 _mainMaxFrameTiming = 0.0f;
static bool _mainLowPriority = false;

// Time returned by timeGetTime() on startup.
// This is then subtracted so that Main_GetTime() returns OpenLRR's runtime in milliseconds,
//  instead of the computer's runtime.
static uint32 _mainStartTime = 0;

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

/// CUSTOM: Returns true if the game is scaling the resolution of drawing surfaces and 3D rendering.
bool Gods98::Main_IsRenderScaling()
{
	return mainOptions.renderScaling.value_or(false);
}

/// CUSTOM: Gets the rendering scale of the drawing surface, unlike Main_Scale, this affects the resolution that things are drawn at.
sint32 Gods98::Main_RenderScale()
{
	if (Main_IsRenderScaling()) {
		return static_cast<sint32>(mainGlobs2.windowScale);
	}
	else {
		return 1;
	}
}

/// CUSTOM: Gets the drawing scale for the radar.
sint32 Gods98::Main_RadarMapScale()
{
	if (mainOptions.radarMapScale.has_value()) {
		return static_cast<sint32>(*mainOptions.radarMapScale);
	}
	else {
		//return 1;
		// Half of the main render scale, rounded up.
		return std::max(1, ((Gods98::Main_RenderScale() + 1) / 2));
	}
}

/// CUSTOM: Sets the drawing scale for the radar.
void Gods98::Main_SetRadarMapScale(uint32 radarMapScale)
{
	mainOptions.radarMapScale = std::max(1u, radarMapScale);
}

/// CUSTOM: Checks if the scale creates a window larger than the desktop area size.
bool Gods98::Main_IsScaleSupported(uint32 windowScale)
{
	// Fullscreen does not support scaling.
	// Arbitrarily large scale cap to avoid overflow.
	if (Main_FullScreen() || windowScale == 0 || windowScale > 0x10000) 
		return false;

	// We can't validate scaling until our display is setup.
	// Always support the smallest scale.
	if (!Main_IsDisplaySetup() || windowScale == 1)
		return true;

	Rect2I rectDesktop = { 0 }; // dummy init
	HWND hWndDesktop = ::GetDesktopWindow();
	::GetWindowRect(hWndDesktop, reinterpret_cast<RECT*>(&rectDesktop));

	Rect2I rect = {
		0,
		0,
		appWidth() * (sint32)windowScale,
		appHeight() * (sint32)windowScale,
	};
	Main_AdjustWindowRect(&rect);


	const sint32 maxWidth = rectDesktop.right - rectDesktop.left;
	const sint32 maxHeight = rectDesktop.bottom - rectDesktop.top;

	const sint32 scaledWidth = rect.right - rect.left;
	const sint32 scaledHeight = rect.bottom - rect.top;

	return (scaledWidth  > 0 && scaledWidth  <= maxWidth &&
			scaledHeight > 0 && scaledHeight <= maxHeight);
}

/// CUSTOM: Changes the current game window scale.
void Gods98::Main_SetScale(uint32 windowScale)
{
	if (windowScale != mainGlobs2.windowScale && Main_IsScaleSupported(windowScale)) {

		if (Main_IsDisplaySetup() && !Main_FullScreen()) {
			Rect2I appRect = { 0 }; // dummy init
			::GetWindowRect(Main_hWnd(), reinterpret_cast<RECT*>(&appRect));

			Rect2I rect = {
				0,
				0,
				appWidth() * (sint32)windowScale,
				appHeight() * (sint32)windowScale,
			};
			Main_AdjustWindowRect(&rect);

			const sint32 scaledWidth = rect.right - rect.left;
			const sint32 scaledHeight = rect.bottom - rect.top;

			// Change the x,y position of our rect to that of appRect.
			rect.right -= rect.left;
			rect.bottom -= rect.top;
			rect.right += appRect.left;
			rect.bottom += appRect.top;
			rect.left = appRect.left;
			rect.top = appRect.top;

			// If we're increasing the window scale, then move the x/y position back a bit if we're going outside the window bounds.
			// This is probably really broken if used on a secondary monitor... >.>
			if (windowScale > mainGlobs2.windowScale) {
				Main_AdjustWindowPosition(&rect);
			}

			::SetWindowPos(Main_hWnd(), nullptr, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER | SWP_NOACTIVATE);
		}

		mainGlobs2.windowScale = (sint32)windowScale;
	}
}



/// CUSTOM: Adjust rect x,y position so that the bottom,right isn't going off the screen.
void Gods98::Main_AdjustWindowPosition(IN OUT Rect2I* rect)
{
	if (!Main_FullScreen() && Main_hWnd() != nullptr) {
		Rect2I rectDesktop = { 0 }; // dummy init

		/// NEW: Check against actual area with taskbar taken into account.
		if (!::SystemParametersInfoA(SPI_GETWORKAREA, 0, reinterpret_cast<RECT*>(&rectDesktop), 0)) {
			HWND hWndDesktop = ::GetDesktopWindow();
			::GetWindowRect(hWndDesktop, reinterpret_cast<RECT*>(&rectDesktop));
		}

		const sint32 xDiff1 = rect->right - (rectDesktop.right);// -taskBarHeight);
		const sint32 yDiff1 = rect->bottom - (rectDesktop.bottom);// - taskBarHeight);


		if (rect->left > 0 && xDiff1 > 0) {
			rect->left   -= xDiff1;
			rect->right  -= xDiff1;
		}
		if (rect->top  > 0 && yDiff1 > 0) {
			rect->top    -= yDiff1;
			rect->bottom -= yDiff1;
		}

		const sint32 xDiff2 = rect->left;
		const sint32 yDiff2 = rect->top;
		if (xDiff2 < 0) {
			rect->left   -= xDiff2;
			rect->right  -= xDiff2;
		}
		if (yDiff2 < 0) {
			rect->top    -= yDiff2;
			rect->bottom -= yDiff2;
		}
	}
}


/// CUSTOM: A wrapper for the WINAPI Sleep function.
void __cdecl Gods98::Main_Sleep(uint32 milliseconds)
{
	::Sleep(milliseconds);
}


/*/// CUSTOM: Gets if the `MAIN_FLAG_SHOWVERSION` flag is set.
bool32 Gods98::Main_IsShowVersion(void)
{
	return (mainGlobs.flags & MainFlags::MAIN_FLAG_SHOWVERSION);
}*/

/// CUSTOM: Sets if the `MAIN_FLAG_SHOWVERSION` flag is set.
void Gods98::Main_SetShowVersion(bool32 on)
{
	if (on) mainGlobs.flags |= MainFlags::MAIN_FLAG_SHOWVERSION;
	else    mainGlobs.flags &= ~MainFlags::MAIN_FLAG_SHOWVERSION;
}

/*/// CUSTOM: Gets if the `MAIN_FLAG_LEVELSOPEN` flag is set.
bool32 Gods98::Main_IsLevelsOpen(void)
{
	return (mainGlobs.flags & MainFlags::MAIN_FLAG_LEVELSOPEN);
}*/

/// CUSTOM: Sets if the `MAIN_FLAG_LEVELSOPEN` flag is set.
void Gods98::Main_SetLevelsOpen(bool32 on)
{
	if (on) mainGlobs.flags |= MainFlags::MAIN_FLAG_LEVELSOPEN;
	else    mainGlobs.flags &= ~MainFlags::MAIN_FLAG_LEVELSOPEN;
}

/*/// CUSTOM: Gets if the `MAIN_FLAG_TESTERCALL` flag is set.
bool32 Gods98::Main_IsTesterCall(void)
{
	return (mainGlobs.flags & MainFlags::MAIN_FLAG_TESTERCALL);
}*/

/// CUSTOM: Sets if the `MAIN_FLAG_TESTERCALL` flag is set.
void Gods98::Main_SetTesterCall(bool32 on)
{
	if (on) mainGlobs.flags |= MainFlags::MAIN_FLAG_TESTERCALL;
	else    mainGlobs.flags &= ~MainFlags::MAIN_FLAG_TESTERCALL;
}

/*/// CUSTOM: Gets if the `MAIN_FLAG_DEBUGMODE` flag is set.
bool32 Gods98::Main_IsDebugMode(void)
{
	return (mainGlobs.flags & MainFlags::MAIN_FLAG_DEBUGMODE);
}*/

/// CUSTOM: Sets if the `MAIN_FLAG_DEBUGMODE` flag is set.
void Gods98::Main_SetDebugMode(bool32 on)
{
	if (on) mainGlobs.flags |= MainFlags::MAIN_FLAG_DEBUGMODE;
	else    mainGlobs.flags &= ~MainFlags::MAIN_FLAG_DEBUGMODE;
}

/*/// CUSTOM: Gets if the `MAIN_FLAG_DEBUGCOMPLETE` flag is set.
bool32 Gods98::Main_IsDebugComplete(void)
{
	return (mainGlobs.flags & MainFlags::MAIN_FLAG_DEBUGCOMPLETE);
}*/

/// CUSTOM: Sets if the `MAIN_FLAG_DEBUGCOMPLETE` flag is set.
void Gods98::Main_SetDebugComplete(bool32 on)
{
	if (on) mainGlobs.flags |= MainFlags::MAIN_FLAG_DEBUGCOMPLETE;
	else    mainGlobs.flags &= ~MainFlags::MAIN_FLAG_DEBUGCOMPLETE;
}

/*/// CUSTOM: Gets if the `MAIN_FLAG_DUALMOUSE` flag is set.
bool32 Gods98::Main_IsDualMouse(void)
{
	return (mainGlobs.flags & MainFlags::MAIN_FLAG_DUALMOUSE);
}*/

/// CUSTOM: Sets if the `MAIN_FLAG_DUALMOUSE` flag is set.
void Gods98::Main_SetDualMouse(bool32 on)
{
	if (on) mainGlobs.flags |= MainFlags::MAIN_FLAG_DUALMOUSE;
	else    mainGlobs.flags &= ~MainFlags::MAIN_FLAG_DUALMOUSE;
}

/*/// CUSTOM: Gets if the `MAIN_FLAG_DUMPMODE` flag is set.
bool32 Gods98::Main_IsDumpMode(void)
{
	return (mainGlobs.flags & MainFlags::MAIN_FLAG_DUMPMODE);
}*/

/// CUSTOM: Sets if the `MAIN_FLAG_DUMPMODE` flag is set.
void Gods98::Main_SetDumpMode(bool32 on)
{
	if (on) mainGlobs.flags |= MainFlags::MAIN_FLAG_DUMPMODE;
	else    mainGlobs.flags &= ~MainFlags::MAIN_FLAG_DUMPMODE;
}


/*/// CUSTOM: Gets if the `CL_FLAG_BLOCKFADE` flag is set.
bool32 Gods98::Main_IsCLBlockFade(void)
{
	return (mainGlobs.clFlags & MainCLFlags::CL_FLAG_BLOCKFADE);
}*/

/// CUSTOM: Sets if the `CL_FLAG_BLOCKFADE` flag is set.
void Gods98::Main_SetCLBlockFade(bool32 on)
{
	if (on) mainGlobs.clFlags |= MainCLFlags::CL_FLAG_BLOCKFADE;
	else    mainGlobs.clFlags &= ~MainCLFlags::CL_FLAG_BLOCKFADE;
}


/*/// CUSTOM: Gets if the specified command line `-flags` value is set.
bool32 Gods98::Main_IsCLFlag(MainCLFlags clFlag)
{
	return (mainGlobs.clFlags & clFlag);
}*/

/// CUSTOM: Sets if the specified command line `-flags` value is set.
void Gods98::Main_SetCLFlag(MainCLFlags clFlag, bool32 on)
{
	if (on) mainGlobs.clFlags |= clFlag;
	else    mainGlobs.clFlags &= ~clFlag;
}


/// CUSTOM: Gets the elapsed time for the current update tick.
real32 Gods98::Main_GetDeltaTime()
{
	return _mainDeltaTime;
}

/// CUSTOM: Calculates the elapsed time for the current update tick.
void Gods98::Main_UpdateDeltaTime()
{
	const real64 realMaxDeltaMS = (3.0 * (1000.0 / STANDARD_FRAMERATE)); // 3 ticks max.
	real64 minDeltaMS = 0.0;
	real64 maxDeltaMS = realMaxDeltaMS;
	if (mainGlobs.flags & MainFlags::MAIN_FLAG_DUMPMODE) {
		minDeltaMS = maxDeltaMS = std::min(maxDeltaMS, (1000.0 / 30.0));
	}
	else {
		if (mainGlobs.fixedFrameTiming != 0.0f) { // -fpslock <framerate>
			minDeltaMS = maxDeltaMS = std::min(maxDeltaMS, (mainGlobs.fixedFrameTiming * (1000.0 / STANDARD_FRAMERATE))); // Standard units to milliseconds.
		}
		// Defining -fpslock and -fpscap will allow setting the min and max frame rates.
		if (_mainMaxFrameTiming != 0.0f) { // -fpscap <framerate>
			minDeltaMS = std::min(maxDeltaMS, (_mainMaxFrameTiming * (1000.0 / STANDARD_FRAMERATE))); // Standard units to milliseconds.
		}
	}

	/// CHANGE: Always update last time, in-case the user switches modes.
	uint32 lastTime = _mainLastTime;
	uint32 currTime = Main_GetTime();

	real32 delta;

	/*if (mainGlobs.flags & MainFlags::MAIN_FLAG_DUMPMODE) {
		// In LegoRR, this state is never reachable.
		delta = STANDARD_FRAMERATE / 30.0f;

	}
	else */if (mainGlobs.flags & MainFlags::MAIN_FLAG_PAUSED) {
		// In LegoRR, this state is never reachable because Main_SetPaused is never used.
		if (mainGlobs2.advanceFrames > 0) {
			mainGlobs2.advanceFrames--;
			// We could bulk-execute this, but it'll likely break the engine.
			delta = 1.0f; // ((mainGlobs.fixedFrameTiming == 0.0f) ? 1.0f : mainGlobs.fixedFrameTiming);
		}
		else {
			delta = 0.0f;
		}
	}
	//else if (mainGlobs.fixedFrameTiming == 0.0f) { // No fps lock or cap defined.
	else if (minDeltaMS == 0.0f) { // No fps lock or cap defined.
		// Measure the time taken over the last frame (to be passed next loop)
		delta = (currTime - lastTime) / (1000.0f / STANDARD_FRAMERATE); // Milliseconds to standard units.

#ifndef _UNLIMITEDUPDATETIME
		// LegoRR compiles with this preprocessor undefined (this check still happens).
		if (delta > 3.0f) {
			delta = 3.0f;
		}
#endif // _UNLIMITEDUPDATETIME

	}
	else {
		if (lastTime == 0) {
			lastTime = currTime;
		}
		// OLD CODE:
		// Always update with the same time, regardless of actual time passed.
		//delta = mainGlobs.fixedFrameTiming;

		/// FIX APPLY: -fpslock now locks framerate and waits an appropriate amount of time.
		/// TODO: This still needs a lot of work, and can probably be improved.
		_mainAccumulatorMS += static_cast<real64>(currTime - lastTime);

		lastTime = currTime;
		while (_mainAccumulatorMS < minDeltaMS) {
			/// TODO: This is an arbitrary choice at the moment.
			if (_mainLowPriority && (minDeltaMS - _mainAccumulatorMS > 1.0)) { // -lowcpu
				Main_Sleep(static_cast<uint32>(minDeltaMS - _mainAccumulatorMS));
			}

			currTime = Main_GetTime();
			_mainAccumulatorMS += static_cast<real64>(currTime - lastTime);
			lastTime = currTime;
		}

		// We don't have separate physics and rendering logic, so we have to drop extra frames.
		real64 deltaMS = std::min(_mainAccumulatorMS, maxDeltaMS);
		_mainAccumulatorMS -= deltaMS;
		_mainAccumulatorMS = std::min(_mainAccumulatorMS, realMaxDeltaMS);

		delta = static_cast<real32>(deltaMS / (1000.0 / STANDARD_FRAMERATE)); // Milliseconds to standard units.
	}

	_mainDeltaTime = delta;
	_mainLastTime = currTime;
}


// <LegoRR.exe @00401b30>
uint32 __cdecl Gods98::noinline(Main_ProgrammerMode)(void)
{
	return Main_ProgrammerMode();
}

// <LegoRR.exe @00401b40>
const char* __cdecl Gods98::noinline(Main_GetStartLevel)(void)
{
	return Main_GetStartLevel();
}

// <LegoRR.exe @00401b70>
sint32 __cdecl Gods98::noinline(appWidth)(void)
{
	return appWidth();
}

// <LegoRR.exe @00401b80>
sint32 __cdecl Gods98::noinline(appHeight)(void)
{
	return appHeight();
}


/// CUSTOM: Run before OpenLRR initialisation to perform command line parsing and initial setup.
bool Gods98::Main_Initialise(_In_ HINSTANCE hInstanceDll,
							 _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
							 _In_ LPSTR     lpCmdLine, _In_     sint32    nCmdShow)
{
	log_firstcall();

	// Make sure this is the same time function as used in Main_GetTime().
	// Or we can just call Main_GetTime() first, since _mainStartTime will be 0 then.
	_mainStartTime = legacy::timeGetTime();

	//bool32 setup = false, nosound = false, insistOnCD = false;
	const char* productName = nullptr;

	// <https://docs.microsoft.com/en-us/windows/win32/api/processenv/nf-processenv-getcommandlinea>
	// From MS Docs: `LPSTR GetCommandLineA();`
	//  "The lifetime of the returned value is managed by the system,
	//   applications should not free or modify this value."
	//
	//  ...good GODS, what have they done!??
	/// FIX LEGORR: Create a copy of string returned by `::GetCommandLineA();`
	///  (replaces `::GetCommandLineA();` with variable `getCL`, and frees memory when done)
	char* getCL = Util_StrCpy(::GetCommandLineA());
	char* cl;
	char* s;
	for (s = cl = getCL; *cl != '\0'; cl++) {
		if (*cl == '\\') s = cl+1;		// Find the last backslash.
	}
	std::strcpy(mainGlobs.programName, s);

	// zero-out double quote '\"' characters (hopefully it's impossible for it to start with a quote character)
	for (s = mainGlobs.programName; *s != '\0'; s++) {
		if (*s == '"') *s = '\0';		// Terminate at any '"'.
	}
	// find the last '.' for file extension
	for (s = cl = mainGlobs.programName; *cl != '\0'; cl++) {
		if (*cl == '.') s = cl+1;	// Find the last dot.
	}

	if (s != mainGlobs.programName) {
		// separate executable name from extension,
		// this name without extension will be the basis for many constant lookups
		::_strupr(s);																	// Ensure .exe is in upper case.
		if (s = std::strstr(mainGlobs.programName, ".EXE")) *s = '\0';					// Strip off .EXE

		/// OPENLRR TEMP: Use the '-' as a separator for the base exe name
		if (s = std::strstr(mainGlobs.programName, "-")) *s = '\0';					// Strip off '-'
	}
	Mem_Free(getCL); // no longer used


	/// OLD LEGORR: Product mutex name was hardcoded to this:
	productName = MUTEX_NAME; // "Lego Rock Raiders";
	/// NEW GODS98: Mutex setup is called later than in LegoRR
	//productName = mainGlobs.programName;

	/// NEW GODS98: Not called in LegoRR WinMain
	// Moved after mutex check, so we wont't need to call ::CoUninitialize() there.
	::CoInitialize(nullptr);

	//HANDLE hActCtx;
	ACTCTX actCtx;
	std::memset(&actCtx, 0, sizeof(actCtx));
	actCtx.cbSize = sizeof(actCtx);
	actCtx.hModule = hInstanceDll;
	actCtx.lpResourceName = MAKEINTRESOURCE(1); // Manifest resource file
	actCtx.dwFlags = ACTCTX_FLAG_HMODULE_VALID | ACTCTX_FLAG_RESOURCE_NAME_VALID;

	// Enable manifest visual styles (which can't be turned on normally from a dll).
	visualStyles_hActCtx = ::CreateActCtxA(&actCtx);
	if (visualStyles_hActCtx != INVALID_HANDLE_VALUE) {
		::ActivateActCtx(visualStyles_hActCtx, &visualStyles_cookie);
	}


	// Setup the default globals
	mainGlobs.className = mainGlobs.programName;
	mainGlobs.active = false;
	mainGlobs.exit = false;
	mainGlobs.stateSet = false;
	mainGlobs.hInst = hInstance;
	mainGlobs.fixedFrameTiming = 0.0f;
	mainGlobs.flags = MainFlags::MAIN_FLAG_NONE;
	mainGlobs2.windowScale = 1;

	// Define default command line settings:
	//mainOptions.menu = true;
	//mainOptions.log = false;
	mainGlobs.flags |= MainFlags::MAIN_FLAG_WINDOW; // Windowed selected in Mode Selection dialog by default.

	/// NEW GODS98: Not present in LegoRR.exe
//	//mainGlobs.flags |= MainFlags::MAIN_FLAG_BEST;
	//mainGlobs.flags |= MainFlags::MAIN_FLAG_DUALMOUSE;

	// Handle command line arguments
	{
		mainOptions.arguments.clear();

		// Split the passed command line arguments.
		// The command line arguments have the program name, parse and skip it.
		// lpCmdLine DOES NOT include the program name argument.
		CommandLine::SplitArguments(lpCmdLine, mainOptions.arguments, false);// true, true);

		if (!mainOptions.arguments.empty() && CommandLine::ArgumentEquals(mainOptions.arguments[0], "clgen")) {
			CLGen::CLGen_WinMain(hInstanceDll, hPrevInstance, lpCmdLine, nCmdShow);
			return false;
		}

		/// OPENLRR CUSTOM: Option to disable StandardParameters
		// We need to parse this early.
		// Usage: -noclgen
		mainOptions.noCLGen = CommandLine::FindOption(mainOptions.arguments, "-noclgen");

		// Usage: -cl <presetname>
		// Manually specify a configuration for command line options using the existing CLGen.dat format.
		std::string param;
		if (CommandLine::FindParameter(mainOptions.arguments, "-cl", param)) {
			mainOptions.noCLGen = true;
			mainOptions.clgenName = param;

			if (CLGen::CLGen_Open(CLGEN_FILENAME)) {
				for (uint32 i = 0; i < CLGen::CLGen_GetPresetCount(); i++) {
					const CLGen::CLGen_Preset* preset = CLGen::CLGen_GetPreset(i);
					if (CommandLine::ArgumentEquals(preset->displayName, mainOptions.clgenName.value())) {

						// Append these arguments at the end of our existing command line arguments.
						CommandLine::SplitArguments(preset->options, mainOptions.arguments, false);
						break;
					}
				}
				CLGen::CLGen_Close();
			}
		}
		else {
			char clgenCmdLine[4096] = { '\0' }; // dummy init
			if (!mainOptions.noCLGen.value_or(false) &&
				Registry_GetValue(MAIN_PRODUCTREGISTRY, "StandardParameters", RegistryValue::String, clgenCmdLine, sizeof(clgenCmdLine)))
			{
				// Append these arguments at the end of our existing command line arguments.
				CommandLine::SplitArguments(clgenCmdLine, mainOptions.arguments, false);
			}
		}

		// Parse and process all command line arguments.
		Main_ParseCommandLineOptions();
	}

	// OpenLRR: This might be handy later on
	//	if (Util_StrIStr(lpCmdLine, "-setup")) setup = true;


	if (!mainOptions.noInstance.value_or(false)) {
		char mutexName[128];
		std::sprintf(mutexName, "%s Mutex", productName);
		::CreateMutexA(nullptr, true, mutexName);
		if (::GetLastError() == ERROR_ALREADY_EXISTS) {
			::MessageBoxA(nullptr, "Another instance of OpenLRR is already running", nullptr, MB_OK);
			return false;  // App is already running
		}
	}

	return true;
}

// <LegoRR.exe @00477a60>
sint32 __stdcall Gods98::Main_WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
							          _In_ LPSTR     lpCmdLine, _In_     sint32    nCmdShow)
{
	log_firstcall();

	bool32 setup = false; // , nosound = false, insistOnCD = false;
	const char* productName = nullptr;
	char noHALMsg[1024];

#if false
	char mutexName[128];

	/// OLD LEGORR: Mutex setup is called at the very beginning in LegoRR
	///  (but it's essential to be the first here)
	/*productName = "Lego Rock Raiders";

	if (productName) { // this is never false
		std::sprintf(mutexName, "%s Mutex", productName);
		::CreateMutexA(nullptr, true, mutexName);
		if (::GetLastError() == ERROR_ALREADY_EXISTS) {
			return 0;  // App is already running
		}
	}*/


	// This is code that was commented out in Gods98, nothing more.
//	Test();
//	{
//		extern void __cdecl Init_Init(const char* cmdLine);
//		Init_Init(lpCmdLine);
//	}


	// <https://docs.microsoft.com/en-us/windows/win32/api/processenv/nf-processenv-getcommandlinea>
	// From MS Docs: `LPSTR GetCommandLineA();`
	//  "The lifetime of the returned value is managed by the system,
	//   applications should not free or modify this value."
	//
	//  ...good GODS, what have they done!??
	/// FIX LEGORR: Create a copy of string returned by `::GetCommandLineA();`
	///  (replaces `::GetCommandLineA();` with variable `getCL`, and frees memory when done)
	char* getCL = Util_StrCpy(::GetCommandLineA());
	char* cl;
	char* s;
	for (s=cl= getCL ; *cl!='\0' ; cl++) if (*cl == '\\') s = cl+1;		// Find the last backslash.
	std::strcpy(mainGlobs.programName, s);

	// zero-out double quote '\"' characters (hopefully it's impossible for it to start with a quote character)
	for (s=mainGlobs.programName ; *s!='\0' ; s++) if (*s == '"') *s = '\0';		// Terminate at any '"'.
    // find the last '.' for file extension
	for (s=cl=mainGlobs.programName ; *cl!='\0' ; cl++) if (*cl == '.') s = cl+1;	// Find the last dot.

	if (s != mainGlobs.programName) {
		// separate executable name from extension,
		// this name without extension will be the basis for many constant lookups
		::_strupr(s);																	// Ensure .exe is in upper case.
		if (s = std::strstr(mainGlobs.programName, ".EXE")) *s = '\0';					// Strip off .EXE

		/// OPENLRR TEMP: Use the '-' as a separator for the base exe name
		if (s = std::strstr(mainGlobs.programName, "-")) *s = '\0';					// Strip off '-'
	}
	Mem_Free(getCL); // no longer used


	/// OLD LEGORR: Product mutex name was hardcoded to this:
	productName = MUTEX_NAME; // "Lego Rock Raiders";
	/// NEW GODS98: Mutex setup is called later than in LegoRR
	//productName = mainGlobs.programName;

	if (productName) { // this realistically shoudn't ever be false
		std::sprintf(mutexName, "%s Mutex", productName);
		::CreateMutexA(nullptr, true, mutexName);
		if (::GetLastError() == ERROR_ALREADY_EXISTS) {
			return 0;  // App is already running
		}
	}


	// Setup the default globals
	mainGlobs.className = mainGlobs.programName;
	mainGlobs.active = false;
	mainGlobs.exit = false;
	mainGlobs.stateSet = false;
	mainGlobs.hInst = hInstance;
	mainGlobs.fixedFrameTiming = 0.0f;
	mainGlobs.flags = MainFlags::MAIN_FLAG_NONE;
	mainGlobs2.windowScale = 1;
#endif

	/// OLD LEGORR: Product mutex name was hardcoded to this:
	productName = MUTEX_NAME; // "Lego Rock Raiders";
	/// NEW GODS98: Mutex setup is called later than in LegoRR
	//productName = mainGlobs.programName;


	/// NEW GODS98: Not called in LegoRR WinMain
	// Moved after mutex check, so we wont't need to call ::CoUninitialize() there.
	//::CoInitialize(nullptr);

#if false
	{
		char commandLine[1024];
		char tempStr[1024];

		if (Registry_GetValue(MAIN_PRODUCTREGISTRY, "StandardParameters", RegistryValue::String, tempStr, sizeof(tempStr))) {
			std::sprintf(commandLine, "%s %s", lpCmdLine, tempStr);
		} else std::sprintf(commandLine, "%s", lpCmdLine);

		/// NEW GODS98: Not present in LegoRR.exe
//		//mainGlobs.flags |= MainFlags::MAIN_FLAG_BEST;
		//mainGlobs.flags |= MainFlags::MAIN_FLAG_DUALMOUSE;
		Main_ParseCommandLine(commandLine, &nosound, &insistOnCD);
	}
#endif

	if (!Registry_GetValue(MAIN_PRODUCTREGISTRY, "NoHALMessage", RegistryValue::String, noHALMsg, sizeof(noHALMsg))) {
		std::sprintf(noHALMsg, "No DirectX 3D accelerator could be found.");
	}

	// Initialise everything (memory, window, DirectDraw, etc.)
	Error_Initialise();
	Mem_Initialise();

	// Initialise to default directories before setting up file system.
	// This can be better-handled once we decide to stop using the native fileGlobs memory.
	if (mainOptions.dataDir) File_SetDataDir(mainOptions.dataDir.value());
	if (mainOptions.wadDir) File_SetWadDir(mainOptions.wadDir.value());
	File_SetDataPriority(mainOptions.dataFirst.value_or(false));
	File_SetWadsEnabled(!mainOptions.noWads.value_or(false));
	File_SetCDEnabled(!mainOptions.noCD.value_or(false));

	const char* wadFileName = "LegoRR";
	if (mainOptions.wadName) wadFileName = mainOptions.wadName->c_str();

	File_Initialise(wadFileName, mainOptions.insistOnCD.value_or(false), MAIN_PRODUCTREGISTRY);

	Config_Initialise();
	/// OLD LEGORR: This is called earlier than in Gods98
	///  (this had to be moved due to attempting access of Main_hWnd() before assignment)
	//Input_InitKeysAndDI();

	/// NEW GODS98: This function doesn't exist in LegoRR.exe... but it *should*
	/// TODO: Re-add me once Font module is finished
	Font_Initialise();

	
	// OpenLRR: This might be handy later on
	//	if (Util_StrIStr(lpCmdLine, "-setup")) setup = true;
#pragma message ("Defaulting to setup mode")
	setup = true;

	if (Main_InitApp(hInstance)) {
		DirectDraw_Initialise(mainGlobs.hWnd);
		if (Sound_Initialise(mainOptions.noSound.value_or(false)) &&
			Init_Initialise(setup /*true in LegoRR*/,
							mainGlobs.flags & MainFlags::MAIN_FLAG_DEBUGMODE,
							mainGlobs.flags & MainFlags::MAIN_FLAG_BEST,
							mainGlobs.flags & MainFlags::MAIN_FLAG_WINDOW,
							noHALMsg))
		{
			Animation_Initialise(DirectDraw());

			Draw_Initialise(nullptr);
			/// NEW GODS98: This is called in Lego_Initialise, but not in Gods98
			//Image_Initialise();

			/// NEW GODS98: This is called later than in LegoRR
			///  (this had to be put here due to attempting access of Main_hWnd() before assignment)
			Input_InitKeysAndDI();

			// Call the Gods_Go() funtion in the application
			//  (this acts as a sort-of entrypoint for LegoRR game code)
			//  (we can also perform OpenLRR hooking in this function,
			//   rather than using Main_SetState)
			Gods_Go(mainGlobs.programName);

			// If the game wants to run in state mode...
			if (mainGlobs.stateSet) {

				// Run their initialisation code (if required)...
				if (mainGlobs.currState.Initialise != nullptr) {
					if (!mainGlobs.currState.Initialise()) {

						// Initialisation failed so drop out...
						Error_Warn(true, "Initialisation function failed...");
						mainGlobs.currState.Initialise = nullptr;
						mainGlobs.currState.MainLoop = nullptr;
						mainGlobs.currState.Shutdown = nullptr;

					}
				}

#ifdef _DEBUG_2
			// REMOVED: This debug code isn't helpful
			/*{
				File* logFile = File_Open("notthere.dat", "rb");		// Log a failed file open in FileMon
			}*/
#endif // _DEBUG_2


				// If? a main loop is specified then run it until it returns false...
				if (mainGlobs.currState.MainLoop != nullptr) {
					
					// Use the MultiMedia timer to give a 'realtime passed' value
					// per frame to the main loop (in 25th's of a second)
					_mainDeltaTime = 1.0f;

					_mainLastTime = Main_GetTime();

					while (!mainGlobs.exit) {
						
						// Handle windows messages and input...
						INPUT.lClicked = false;
						INPUT.rClicked = false;
						INPUT.lDoubleClicked = false;
						INPUT.rDoubleClicked = false;

						Main_HandleIO();
						Main_HandleCursorClipping();
						// In fullscreen mode we will always be the active application or I will eat my hat.
						//  --Whoever wrote this 20 years ago, I'll hold you to it
						if (mainGlobs.flags & MainFlags::MAIN_FLAG_FULLSCREEN) mainGlobs.active = true;

						Input_ReadKeys();
						Input_ReadMouse2();

						// Run the main loop (pass 1.0f as the initial timing value)
						if (!mainGlobs.currState.MainLoop(_mainDeltaTime)) mainGlobs.exit = true;

						// Update the device and flip the surfaces...
						Graphics_Finalise3D();
						DirectDraw_Flip();
						// Set as not-updated again while the game loop is running.
						mainGlobs.flags &= ~MainFlags::MAIN_FLAG_UPDATED;

						Main_UpdateDeltaTime();
					}
				}

#ifdef _DEBUG_2
			// REMOVED: This debug code isn't helpful
			/*{
				File* logFile = File_Open("notthere.dat", "rb");
			}*/
#endif // _DEBUG_2

			/*
#ifdef CONFIG_DEVELOPMENTMODE
			{
				const char* loc;
				if (loc = Util_StrIStr(lpCmdLine, "-languagedump")) {
					char* s;
					uint32 index = 0;
					char file[FILE_MAXPATH];
					for (s=&loc[std::strlen("-languagedump")] ; '\0'!=*s ; s++) if (' ' != *s) break;
					for ( ; '\0' != *s ; s++) {
						if (' ' == *s) break;
						else file[index++] = *s;
					}
					file[index] = '\0';
					Config_DumpUnknownPhrases(file);
				}
			}
#endif // CONFIG_DEVELOPMENTMODE
			*/

				// Shutdown if required...
				if (mainGlobs.currState.Shutdown != nullptr) mainGlobs.currState.Shutdown();
				
			} else Error_Warn(true, "No State Set: Exiting...");
		} else Error_Warn(true, "Initialisation Failed: Exiting...");
		DirectDraw_Shutdown();
		DestroyWindow(mainGlobs.hWnd);
	} else Error_Warn(true, "Unable to initialise window");

	Input_ReleaseKeysAndDI();
	Config_Shutdown();

	// This code was found commented out in Gods98, nothing more.
//#ifdef _GODS98_USEWAD_
//	if (mainGlobs.wad != WAD_ERROR) File_CloseWad(mainGlobs.wad);
//#endif // _GODS98_USEWAD_

	// The very last things...
	Mem_Shutdown(false);

	Error_CloseLog();

	Error_Shutdown();

	if (visualStyles_hActCtx != INVALID_HANDLE_VALUE) {
		// Turn off visual styles
		::DeactivateActCtx(0, visualStyles_cookie);
		::ReleaseActCtx(visualStyles_hActCtx);
		visualStyles_hActCtx = INVALID_HANDLE_VALUE;
		visualStyles_cookie = 0;
	}

	/// NEW GODS98: Not called in LegoRR WinMain
	::CoUninitialize();

	return 0;
}


// <missing>
__declspec(noreturn) void __cdecl Gods98::Main_Exit(void)
{
	/// NEW GODS98: Not called in LegoRR WinMain, so it's likely this wasn't
	///  called here either, making Main_Exit an inline alias for `std::exit(0);`
	::CoUninitialize();
	std::exit(0);
}


/// CUSTOM:
void Gods98::Main_ParseCommandLineOptions()
{
	using namespace CommandLine;

	const std::vector<std::string>& args = mainOptions.arguments; // Shorthand name

	std::string param;
	//std::vector<std::string> params;

	if (FindOption(args, "-insistOnCD")) mainOptions.insistOnCD = true;
	if (FindOption(args, "-nosound")) mainOptions.noSound = true;

	// Usage: -help
	//  (functionality not added yet)
	if (FindOption(args, "-help")) mainOptions.help = true;


	// Debug options:
	if (FindOption(args, "-debug")) {
		mainGlobs.flags |= MainFlags::MAIN_FLAG_DEBUGMODE;
	}
	if (FindOption(args, "-debugcomplete")) {
		/// REFACTOR: Instead of automatically including `-debug` due to how arguments *were* searched for,
		///           Add the flag manually for this option.
		mainGlobs.flags |= MainFlags::MAIN_FLAG_DEBUGMODE;
		mainGlobs.flags |= MainFlags::MAIN_FLAG_DEBUGCOMPLETE;
	}
	if (FindOption(args, "-testercall")) {
		mainGlobs.flags |= MainFlags::MAIN_FLAG_TESTERCALL;
		mainGlobs.programmerLevel = 2;
	}
	if (FindOption(args, "-testlevels")) mainGlobs.flags |= MainFlags::MAIN_FLAG_LEVELSOPEN;
	if (FindOption(args, "-showversion")) mainGlobs.flags |= MainFlags::MAIN_FLAG_SHOWVERSION;

	// Window options:
	if (FindOption(args, "-best")) mainGlobs.flags |= MainFlags::MAIN_FLAG_BEST;
	if (FindOption(args, "-window")) mainGlobs.flags |= MainFlags::MAIN_FLAG_WINDOW;
	if (FindOption(args, "-fullscreen")) mainGlobs.flags &= ~MainFlags::MAIN_FLAG_WINDOW;

	if (FindOption(args, "-dualmouse")) mainGlobs.flags |= MainFlags::MAIN_FLAG_DUALMOUSE;
	if (FindOption(args, "-nodualmouse")) mainGlobs.flags &= ~MainFlags::MAIN_FLAG_DUALMOUSE;

	// Graphics handling options:
	if (FindOption(args, "-nm")) mainGlobs.flags |= MainFlags::MAIN_FLAG_DONTMANAGETEXTURES;
	if (FindOption(args, "-ftm")) mainGlobs.flags |= MainFlags::MAIN_FLAG_FORCETEXTUREMANAGEMENT;
	if (FindOption(args, "-fvf")) mainGlobs.flags |= MainFlags::MAIN_FLAG_FORCEVERTEXFOG;

	// Reduce resource options:
	if (FindOption(args, "-reduceanimation")) mainGlobs.flags |= MainFlags::MAIN_FLAG_REDUCEANIMATION;
	if (FindOption(args, "-reduceflics")) mainGlobs.flags |= MainFlags::MAIN_FLAG_REDUCEFLICS;
	if (FindOption(args, "-reduceimages")) mainGlobs.flags |= MainFlags::MAIN_FLAG_REDUCEIMAGES;
	if (FindOption(args, "-reducepromeshes")) mainGlobs.flags |= MainFlags::MAIN_FLAG_REDUCEPROMESHES;
	if (FindOption(args, "-reducesamples")) mainGlobs.flags |= MainFlags::MAIN_FLAG_REDUCESAMPLES;


	// Usage: -cleansaves
	// Set this to cause reinitialisation of the save games.
	if (FindOption(args, "-cleansaves")) mainGlobs.flags |= MainFlags::MAIN_FLAG_CLEANSAVES;
	

#ifdef CONFIG_DEVELOPMENTMODE

	if (FindOption(args, "-langdump")) mainGlobs.flags |= MainFlags::MAIN_FLAG_LANGDUMPUNKNOWN;

	if (FindParameter(args, "-langsuffix", param)) {
		std::strcpy(mainGlobs.languageName, param.c_str());
		mainGlobs.flags |= MainFlags::MAIN_FLAG_SAVELANGFILE;
	}

	{
		const char* langTable[NUM_LANG] = {
			"040c-french", "0007-german", "000a-spanish", "0010-italian",
			"0013-dutch", "0006-danish", "001d-swedish", "0014-norwegian"
		};

		fileGlobs.langCheck = false;
		char langDir[128];
		std::memset(fileGlobs.langDir, 0, _MAX_PATH);

		for (uint32 i = 0; i < _countof(langTable); i++) {
			//const char* lp = langTable[i] + 5; // start after the "XXXX-"
			//char langLine[30];
			// i.e. "-french"
			//std::sprintf(langLine, "-%s", lp);
			auto langLine = std::string(lp).insert(0, 1, '-');
			//auto langLine = std::string(langTable[i]).substr(5).insert(0, 1, '-');
			if (FindOption(args, langLine)) {
				// i.e. "languages\\040c-french"
				//      "@languages\\040c-french\\french"
				std::sprintf(fileGlobs.langDir, "languages\\%s", langTable[i]);
				std::sprintf(langDir, "@languages\\%s\\%s", langTable[i], lp);
				fileGlobs.langCheck = true;
			}
		}

		if (fileGlobs.langCheck) {
			char temp[128];
			std::sprintf(temp, "%s.new.lang", langDir);
			Config_SetLanguageDatabase(temp);
			std::sprintf(temp, "%s.cct", langDir);
			Config_SetCharacterTable(temp);
		}
	}

	if (FindParameter(args, "-langfile", param)) {
		Config_SetLanguageDatabase(param.c_str());
	}
	if (FindParameter(args, "-CharTable", param)) {
		Config_SetCharacterTable(param.c_str());
	}
	if (FindParameter(args, "-charconvertfile", param)) {
		Config_SetCharacterConvertFile(param.c_str());
	}
#endif // CONFIG_DEVELOPMENTMODE


	// Usage: -startlevel <Levels::Name>
	if (FindParameter(args, "-startlevel", param)) {
		std::strcpy(mainGlobs.startLevel, param.c_str());

		mainGlobs.flags |= MainFlags::MAIN_FLAG_STARTLEVEL;
	}

	// Usage: -flags <decnumber>
	// Usage: -flags 0x<hexnumber>
	if (FindParameter(args, "-flags", param)) {
		if (param.length() >= 1 && param[0] == '-') {
			// Signed decimal, allow shortcut like "-1" for all flags.
			mainGlobs.clFlags = static_cast<MainCLFlags>(std::atoi(param.c_str()));
		}
		else if (param.length() >= 2 && param[0] == '0' && std::tolower(param[1]) == 'x') {
			// Unsigned hex.
			mainGlobs.clFlags = static_cast<MainCLFlags>(std::strtoul(param.c_str() + 2, nullptr, 16));
		}
		else {
			// Unsigned decimal.
			mainGlobs.clFlags = static_cast<MainCLFlags>(std::strtoul(param.c_str(), nullptr, 10));
		}
	}

	// Usage: -fpslock <framerate>
	if (FindParameter(args, "-fpslock", param)) {
		uint32 fps = static_cast<uint32>(std::max(0, std::atoi(param.c_str())));
		if (fps != 0) {
			mainGlobs.fixedFrameTiming = STANDARD_FRAMERATE / static_cast<real32>(fps);
		}
	}

	// Usage: -fpscap <framerate>
	if (FindParameter(args, "-fpscap", param)) {
		uint32 fps = static_cast<uint32>(std::max(0, std::atoi(param.c_str())));
		if (fps != 0) {
			_mainMaxFrameTiming = STANDARD_FRAMERATE / static_cast<real32>(fps);
		}
	}

	// Usage: -lowcpu
	// Sleeps between game loop ticks when used with -fpslock or -fpscap
	if (FindOption(args, "-lowcpu")) _mainLowPriority;


	// Usage: -programmer [level=1]
	if (FindParameter(args, "-programmer", param)) {
		mainGlobs.programmerLevel = static_cast<uint32>(std::max(0, std::atoi(param.c_str())));
		if (mainGlobs.programmerLevel == 0)
			mainGlobs.programmerLevel = 1; // Default level if number isn't specified.
	}
	else {
		mainGlobs.programmerLevel = 0;
	}


	/// LRRCE: Custom commandline options used by Community Edition.
	// Usage: -res <W>x<H>
	// Usage: -res <W>,<H>
	// Change the resolution of the game.
	// Note that support for this is not guaranteed. Things likely will break.
	if (FindParameter(args, "-res", param)) {
		// Valid separators: 'x', 'X', or ','
		size_t idx = param.find('x');
		if (idx == std::string::npos) idx = param.find('X');
		if (idx == std::string::npos) idx = param.find(',');
		if (idx != std::string::npos) {

			mainOptions.res = Size2U {
				static_cast<uint32>(std::max(0, std::atoi(param.substr(0, idx).c_str()))),
				static_cast<uint32>(std::max(0, std::atoi(param.substr(idx + 1).c_str()))),
			};

			if (mainOptions.res->width == 0 || mainOptions.res->height == 0) {
				mainOptions.res = std::nullopt;
			}
		}
	}

	// Usage: -pos <X>,<Y>
	// Change the X,Y position of the window on startup.
	if (FindParameter(args, "-pos", param)) {
		// Valid separators: ','
		size_t idx = param.find(',');
		if (idx != std::string::npos) {

			mainOptions.pos = Point2I {
				std::atoi(param.substr(0, idx).c_str()),
				std::atoi(param.substr(idx + 1).c_str()),
			};

			//if (mainOptions.pos->x == 0 || mainOptions.pos->y == 0) {
			//	mainOptions.pos = std::nullopt; // non-zero values are treated as disabled
			//}
		}
	}

	// Usage: -bpp <bitdepth>
	// Change the bit depth used for mode selection during game startup.
	// Valid options: 8, 16, 24, 32
	// Note that support for this is not guaranteed. Things WILL break.
	if (FindParameter(args, "-bpp", param)) {
		mainOptions.bitDepth = static_cast<uint32>(std::max(0, std::atoi(param.c_str())));

		if (*mainOptions.bitDepth != 8  && *mainOptions.bitDepth != 16 &&
			*mainOptions.bitDepth != 24 && *mainOptions.bitDepth != 32)
		{
			mainOptions.bitDepth = std::nullopt; // Invalid bit depths are disabled
		}
	}

	// Usage: -datadir <directory>
	// Change the folder where loose Data files are searched for. This replaces the "Data" folder name.
	// i.e -datadir "C:\OpenLRR\GameFiles" will look for "C:\OpenLRR\GameFiles\Lego.cfg".
	if (FindParameter(args, "-datadir", param)) {
		// Strip trailing slash.
		if (File_NormalizePath(param, false, true) && !param.empty()) {
			mainOptions.dataDir = param;
		}
	}
	// Usage: -waddir <directory>
	// Change the folder where WAD files are searched for.
	if (FindParameter(args, "-waddir", param) && !param.empty()) {
		// Strip trailing slash.
		if (File_NormalizePath(param, false, true) && !param.empty()) {
			mainOptions.wadDir = param;
		}
	}

	// Check for this command first, so that -wadname and -gamename can have priority,
	// Usage: -name <name>
	// A shorthand for using both the -wadname and -gamename options seen below.
	if (FindParameter(args, "-name", param) && !param.empty()) {
		mainOptions.wadName = param;
		mainOptions.gameName = param;
	}
	// Usage: -wadname <name>
	// Changes the name used to look for WAD files with.
	if (FindParameter(args, "-wadname", param)) {
		mainOptions.wadName = param;
	}
	// Usage: -gamename <Legoname>
	// Changes the root property name used in Lego.cfg lookup. Name should ALWAYS start with "Lego".
	// Includes all CFG-like files: .cfg, .ae, .ol, .ptl
	if (FindParameter(args, "-gamename", param) && !param.empty()) {
		mainOptions.gameName = param;
	}

	// Usage: -nnscale <number>
	// Changes integer scaling of the window.
	// i.e. -nnscale 2 creates a window that is twice as large as the game resolution.
	if (FindParameter(args, "-nnscale", param)) {
		mainGlobs2.windowScale = static_cast<uint32>(std::max(1, std::atoi(param.c_str())));
		mainOptions.renderScaling = false;
	}
	// Usage: -scale <number>
	// Scales the resolution that drawing surfaces and 3D rendering are displayed at.
	// i.e. -scale 2 creates a window that is twice as large as the game resolution.
	if (FindParameter(args, "-scale", param)) {
		mainGlobs2.windowScale = static_cast<uint32>(std::max(1, std::atoi(param.c_str())));
		mainOptions.renderScaling = true;
	}

	// Usage: -radarscale <number>
	// Scales the pixel resolution of the radar map, independent of the render scale.
	// i.e. -radarscale 1 will always draw the radar map at 1x1 pixel resolution, regardless of -renderscale.
	if (FindParameter(args, "-radarscale", param)) {
		mainOptions.radarMapScale = static_cast<uint32>(std::max(1, std::atoi(param.c_str())));
	}

	if (mainGlobs2.windowScale <= 0)
		mainGlobs2.windowScale = 1; // Default scale. Whether invalid or command not used.

	// Usage: -clipcursor [=on|menu|off]
	// Traps the cursor in the game window while in windowed mode. (Press ALT to untrap the cursor temporarily).
	if (FindParameter(args, "-clipcursor", param) || FindOption(args, "-clipcursor")) {

		if (ArgumentEquals(param, "menu"))     mainGlobs2.cursorClipping = CursorClipping::MenuArea;
		else if (ArgumentEquals(param, "off")) mainGlobs2.cursorClipping = CursorClipping::Off;
		else if (ArgumentEquals(param, "on"))  mainGlobs2.cursorClipping = CursorClipping::GameArea;
		else                                   mainGlobs2.cursorClipping = CursorClipping::GameArea;
	}


	// usage: -noCD
	// usage: -useCD
	// Disable checking for files on the CD (requires that necessary Data files are stored locally).
	if (FindOption(args, "-noCD")) mainOptions.noCD = true;
	if (FindOption(args, "-useCD")) mainOptions.noCD = false;
	// usage: -nowad
	// usage: -isewad
	// Disable checking for files in WAD files (all required files must be extracted to the Data directory).
	if (FindOption(args, "-nowad")) mainOptions.noWads = true;
	if (FindOption(args, "-usewad")) mainOptions.noWads = false;

	// Usage: -datafirst
	// Usage: -wadfirst
	// Loose Data files will be loaded before those found in WAD files.
	// Useful for when only a handful of files need to be modified, while all others are unchanged.
	if (FindOption(args, "-datafirst")) mainOptions.dataFirst = true;
	if (FindOption(args, "-wadfirst")) mainOptions.dataFirst = false;

	// Usage: -log
	// Usage: -nolog
	// The Console log window will start showing or hidden.
	if (FindOption(args, "-log")) mainOptions.log = true;
	if (FindOption(args, "-nolog")) mainOptions.log = false;

	// Usage: -menu
	// Usage: -nomenu
	// The System menu bar will start showing or hidden.
	if (FindOption(args, "-menu")) mainOptions.menu = true;
	if (FindOption(args, "-nomenu")) mainOptions.menu = false;

	// Usage: -nointro
	// Splash videos and images will not display at all on startup (rather than displaying but always being skippable).
	if (FindOption(args, "-nointro")) mainOptions.noIntro = true;
	// Usage: -novideo
	// Disables non-startup videos like level intros and the end-game outro.
	if (FindOption(args, "-novideo")) mainOptions.noVideo = true;
	// Usage: -noskip
	// Disables the ability to skip normally-unskippable videos.
	if (FindOption(args, "-noskip")) mainOptions.noSkip = true;


	// Usage: -nomutex
	// Multiple instances of OpenLRR will be allowed to run at the same time.
	if (FindOption(args, "-noinstance")) mainOptions.noInstance = true;


	// Usage: -loglevels nodebug,notrace,noinfo,warn,fatal
	// Turn on or off individual levels of logging.
	if (FindParameter(args, "-loglevels", param) && !param.empty()) {
		size_t start = 0;
		size_t end = 0; // dummy init
		do {
			end = param.find(',', start);

			std::string logType = param.substr(start, std::min(end, param.length()) - start);

			// Check for the "no" (off) prefix.
			const bool on = !ArgumentStartsWith(logType, "no");
			if (!on) logType = logType.substr(2);

			if (ArgumentEquals(logType, "debug")) Error_SetDebugVisible(on);
			if (ArgumentEquals(logType, "trace")) Error_SetTraceVisible(on);
			if (ArgumentEquals(logType, "info"))  Error_SetInfoVisible(on);
			if (ArgumentEquals(logType, "warn"))  Error_SetWarnVisible(on);
			if (ArgumentEquals(logType, "fatal")) Error_SetFatalVisible(on);

			start = end + 1;
		} while (end != std::string::npos);
	}

	// Usage: -cfgfirst
	// Look for Lego.cfg file in the Data Directory before WAD files.
	if (FindOption(args, "-cfgfirst")) mainOptions.configFirst = true;

	// Usage: -cfgfile <filename>
	// Change the name of the Lego.cfg config file to load (must be a relative datadir path).
	if (FindParameter(args, "-cfgfile", param) && !param.empty()) {
		// Uniform slashes.
		Gods98::File_NormalizePath(param);
		mainOptions.configName = param;
	}

	// Usage: -cfgadd <filename>
	// Append extra configs to the Lego.cfg config to overwrite properties (command can be used multiple times).
	for (size_t i = 0; i < args.size(); i++) {
		const auto& arg = args[i];
		if (ArgumentEquals(arg, "-cfgadd") && i + 1 < args.size()) {
			param = args[i+1];
			if (!param.empty()) {
				// Uniform slashes.
				Gods98::File_NormalizePath(param);
				mainOptions.configAppends.push_back(param);
				i++; // Skip past and consume parameter.
			}
		}
	}

	// Save this so we can choose when to modify commandline options.
	mainGlobs2.cmdlineParsed = true;
}


// <LegoRR.exe @00477eb0>
void __cdecl Gods98::Main_ParseCommandLine(const char* lpCmdLine, OUT bool32* nosound, OUT bool32* insistOnCD)
{
	log_firstcall();

	const char* loc;

	if (Util_StrIStr(lpCmdLine, "-insistOnCD")) *insistOnCD = true;
	if (Util_StrIStr(lpCmdLine, "-nosound")) *nosound = true;
	if (Util_StrIStr(lpCmdLine, "-debug")) mainGlobs.flags |= MainFlags::MAIN_FLAG_DEBUGMODE;
	if (Util_StrIStr(lpCmdLine, "-nm")) mainGlobs.flags |= MainFlags::MAIN_FLAG_DONTMANAGETEXTURES;
	if (Util_StrIStr(lpCmdLine, "-ftm")) mainGlobs.flags |= MainFlags::MAIN_FLAG_FORCETEXTUREMANAGEMENT;
	if (Util_StrIStr(lpCmdLine, "-fvf")) mainGlobs.flags |= MainFlags::MAIN_FLAG_FORCEVERTEXFOG;
	if (Util_StrIStr(lpCmdLine, "-best")) mainGlobs.flags |= MainFlags::MAIN_FLAG_BEST;
	if (Util_StrIStr(lpCmdLine, "-window")) mainGlobs.flags |= MainFlags::MAIN_FLAG_WINDOW;
	if (Util_StrIStr(lpCmdLine, "-dualmouse")) mainGlobs.flags |= MainFlags::MAIN_FLAG_DUALMOUSE;
	if (Util_StrIStr(lpCmdLine, "-debugcomplete")) mainGlobs.flags |= MainFlags::MAIN_FLAG_DEBUGCOMPLETE;
	if (Util_StrIStr(lpCmdLine, "-testercall")) { mainGlobs.flags |= MainFlags::MAIN_FLAG_TESTERCALL; mainGlobs.programmerLevel=2; }
	if (Util_StrIStr(lpCmdLine, "-testlevels")) mainGlobs.flags |= MainFlags::MAIN_FLAG_LEVELSOPEN;
	if (Util_StrIStr(lpCmdLine, "-reducesamples")) mainGlobs.flags |= MainFlags::MAIN_FLAG_REDUCESAMPLES;
	if (Util_StrIStr(lpCmdLine, "-showversion")) mainGlobs.flags |= MainFlags::MAIN_FLAG_SHOWVERSION;
	if (Util_StrIStr(lpCmdLine, "-reduceanimation")) mainGlobs.flags |= MainFlags::MAIN_FLAG_REDUCEANIMATION;
	if (Util_StrIStr(lpCmdLine, "-reducepromeshes")) mainGlobs.flags |= MainFlags::MAIN_FLAG_REDUCEPROMESHES;
	if (Util_StrIStr(lpCmdLine, "-reduceflics")) mainGlobs.flags |= MainFlags::MAIN_FLAG_REDUCEFLICS;
	if (Util_StrIStr(lpCmdLine, "-reduceimages")) mainGlobs.flags |= MainFlags::MAIN_FLAG_REDUCEIMAGES;
#ifdef CONFIG_DEVELOPMENTMODE
	char langDir[128];

	if (Util_StrIStr(lpCmdLine, "-langdump")) mainGlobs.flags |= MainFlags::MAIN_FLAG_LANGDUMPUNKNOWN;
	if (loc = Util_StrIStr(lpCmdLine, "-langsuffix")) {
		const char* s;
		uint32 index = 0;
		for (s=&loc[strlen("-langsuffix")] ; '\0'!=*s ; s++) if (' ' != *s) break;
		for ( ; '\0' != *s ; s++) {
			if (' ' == *s) break;
			else mainGlobs.languageName[index++] = *s;
		}
		mainGlobs.languageName[index] = '\0';
		mainGlobs.flags |= MainFlags::MAIN_FLAG_SAVELANGFILE;
	}
	// *** Tony wos here

	fileGlobs.langCheck = false;
	::ZeroMemory(fileGlobs.langDir, _MAX_PATH);

#define NUM_LANG 8

	{
		const char* langTable[NUM_LANG]={"040c-french","0007-german","000a-spanish",
			"0010-italian","0013-dutch","0006-danish","001d-swedish","0014-norwegian"};

		char langLine[30];

		for(uint32 i=0;i<NUM_LANG;i++)
		{
			char* lp2=langTable[i];
			lp2+=5;
			std::sprintf(langLine,"-%s",lp2);
			if (loc = Util_StrIStr(lpCmdLine, langLine)) {
				char* lp = lp2;
				std::sprintf(fileGlobs.langDir, "languages\\%s", langTable[i]);
				std::sprintf(langDir, "@languages\\%s\\%s", langTable[i], lp);
				fileGlobs.langCheck = true;
			}
		}
	}
	if(fileGlobs.langCheck) {
		char temp[128];
		std::sprintf(temp, "%s.new.lang", langDir);
		Config_SetLanguageDatabase(temp);
		std::sprintf(temp, "%s.cct", langDir);
		Config_SetCharacterTable(temp);
	}

	//
	if (loc = Util_StrIStr(lpCmdLine, "-langfile")) {
		char* s;
		uint32 index = 0;
		char file[FILE_MAXPATH];
		for (s=&loc[strlen("-langfile")] ; '\0'!=*s ; s++) if (' ' != *s) break;
		for ( ; '\0' != *s ; s++) {
			if (' ' == *s) break;
			else file[index++] = *s;
		}
		file[index] = '\0';
		Config_SetLanguageDatabase(file);
	}
	if (loc = Util_StrIStr(lpCmdLine, "-CharTable")) {
		char* s;
		uint32 index = 0;
		char file[FILE_MAXPATH];
		for (s=&loc[strlen("-CharTable")] ; '\0'!=*s ; s++) if (' ' != *s) break;
		for ( ; '\0' != *s ; s++) {
			if (' ' == *s) break;
			else file[index++] = *s;
		}
		file[index] = '\0';
		Config_SetCharacterTable(file);
	}
	if (loc = Util_StrIStr(lpCmdLine, "-charconvertfile")) {
		char* s;
		uint32 index = 0;
		char file[FILE_MAXPATH];
		for (s=&loc[strlen("-charconvertfile")] ; '\0'!=*s ; s++) if (' ' != *s) break;
		for ( ; '\0' != *s ; s++) {
			if (' ' == *s) break;
			else file[index++] = *s;
		}
		file[index] = '\0';
		Config_SetCharacterConvertFile(file);
	}
#endif // CONFIG_DEVELOPMENTMODE
	if (loc = Util_StrIStr(lpCmdLine, "-startlevel")) {
		const char* s;
		uint32 index = 0;
		for (s=&loc[std::strlen("-startlevel")] ; *s!='\0' ; s++) if (*s != ' ') break;
		for ( ; *s != '\0' ; s++) {
			if (*s == ' ') break;
			else mainGlobs.startLevel[index++] = *s;
		}
		mainGlobs.startLevel[index] = '\0';
		mainGlobs.flags |= MainFlags::MAIN_FLAG_STARTLEVEL;
	}
	if (loc = Util_StrIStr(lpCmdLine, "-flags")) mainGlobs.clFlags = (MainCLFlags)std::atoi(&loc[std::strlen("-flags")]);
	if (loc = Util_StrIStr(lpCmdLine, "-fpslock")) {
		uint32 fps = std::atoi(&loc[std::strlen("-fpslock")]);
		if (fps) mainGlobs.fixedFrameTiming = STANDARD_FRAMERATE / (real32) fps;
	}
	if (loc = Util_StrIStr(lpCmdLine, "-programmer")) {
		mainGlobs.programmerLevel = std::atoi(&loc[std::strlen("-programmer")]);
		if (mainGlobs.programmerLevel == 0) mainGlobs.programmerLevel = 1;
	} else mainGlobs.programmerLevel = 0;

	// Set this to cause reinitialisation of the save games.
	if (Util_StrIStr(lpCmdLine, "-cleansaves")) mainGlobs.flags |= MainFlags::MAIN_FLAG_CLEANSAVES;

	// Save this so we can choose when to modify commandline options.
	mainGlobs2.cmdlineParsed = true;
}


// <missing>
const char* __cdecl Gods98::Main_GetLanguage(void)
{
	if (mainGlobs.flags & MainFlags::MAIN_FLAG_SAVELANGFILE) return mainGlobs.languageName;
	return nullptr;
}

// <missing>
bool32 __cdecl Gods98::Main_DumpUnknownPhrases(void)
{
	return mainGlobs.flags & MainFlags::MAIN_FLAG_LANGDUMPUNKNOWN;
}


// <LegoRR.exe @004781f0>
void __cdecl Gods98::Main_LoopUpdate(bool32 clear)
{
	Main_LoopUpdate2(clear, true);
}

/// CUSTOM: Extension of Main_LoopUpdate to optionally disable all graphics updates.
void __cdecl Gods98::Main_LoopUpdate2(bool clear, bool updateGraphics)
{
	log_firstcall();

	/// FIX APPLY: Reset clicked states here too.
	INPUT.lClicked = false;
	INPUT.rClicked = false;
	INPUT.lDoubleClicked = false;
	INPUT.rDoubleClicked = false;

	Main_HandleIO();
	Main_HandleCursorClipping();
	Input_ReadKeys();
	Input_ReadMouse2();

	if (updateGraphics) {
		// Update the device and flip the surfaces...
		Graphics_Finalise3D();
		DirectDraw_Flip();

		if (clear) DirectDraw_ClearRGB(nullptr, 0, 0, 0); // black
		mainGlobs.flags &= ~MainFlags::MAIN_FLAG_UPDATED;
	}

	Main_UpdateDeltaTime();

	/// FIX APPLY: Allow the window close button to function within menus and screens by
	///            forcefully exiting the game when LRR is deep in a nested UI loop.
	if (mainGlobs.exit) Main_Exit();
}

// <LegoRR.exe @00478230>
Gods98::MainCLFlags __cdecl Gods98::noinline(Main_GetCLFlags)(void)
{
	return Main_GetCLFlags();
}

// <LegoRR.exe @00478290>
bool32 __cdecl Gods98::Main_SetState(const Main_State* state)
{
	log_firstcall();

	/// TODO: This is a good place to setup wrapping around Main_State functions.
	///  function pointers allow using original functions without overwriting the assembly.
	if (state != nullptr) {
		mainGlobs.currState = *state;
		mainGlobs.stateSet = true;
		return true;
	}
	return false;
}

// <LegoRR.exe @004782c0>
uint32 __cdecl Gods98::Main_GetTime(void)
{
	log_firstcall();

	// Return OpenLRR's runtime instead of the computer's runtime.
	return legacy::timeGetTime() - _mainStartTime;
}


// <missing>
void __cdecl Gods98::Main_SetFixedFrameTiming(real32 time)
{
	mainGlobs.fixedFrameTiming = time;
}


// <LegoRR.exe @004782d0>
bool32 __cdecl Gods98::Main_DispatchMessage(const MSG* msg)
{
	log_firstcall();

	if ((mainGlobs.flags & MainFlags::MAIN_FLAG_FULLSCREEN) &&
		(msg->message == WM_ACTIVATEAPP || msg->message == WM_SYSKEYDOWN ||
		 msg->message == WM_SYSKEYUP)) return false;
	else return true;
}

// <LegoRR.exe @00478300>
void __cdecl Gods98::Main_HandleIO(void)
{
	log_firstcall();

	MSG msg;

	// Look at windows messages (if there are any)

	/// NEW GODS98: New API to pair with the Main_SetAccel function
	if (mainGlobs.accels != nullptr) {
		while (::PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE)) {
			if (!::TranslateAcceleratorA(Main_hWnd(), mainGlobs.accels, &msg)) {
				::TranslateMessage(&msg);
				::DispatchMessageA(&msg);
			}
		}
	}
	else {
		while (::PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE)) {
			if (Main_DispatchMessage(&msg)) {
				::TranslateMessage(&msg);
				::DispatchMessageA(&msg);
			}
		}
	}
}

// <LegoRR.exe @00478370>
void __cdecl Gods98::Main_SetupDisplay(bool32 fullScreen, uint32 xPos, uint32 yPos, uint32 width, uint32 height)
{
	log_firstcall();

	mainGlobs.appWidth = width;
	mainGlobs.appHeight = height;
	if (fullScreen) mainGlobs.flags |= MainFlags::MAIN_FLAG_FULLSCREEN;

	Error_FullScreen(fullScreen);

	if (!fullScreen) {
		// Adjust the window to any new settings...
		
		Rect2I rect = { // sint32 casts to stop compiler from complaining
			(sint32) xPos,
			(sint32) yPos,
			(sint32) (xPos + width * Main_Scale()),
			(sint32) (yPos + height * Main_Scale()),
		};
		mainGlobs.style = WS_POPUP | WS_SYSMENU | WS_CAPTION;
		
		Main_AdjustWindowRect(&rect);
		Main_AdjustWindowPosition(&rect);
		
		::SetWindowLongA(mainGlobs.hWnd, GWL_STYLE, mainGlobs.style);
		::SetWindowPos(mainGlobs.hWnd, nullptr, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER);

		/// FIX APPLY: (by removing) Show cursor over title bar
		///  This has been removed along with DirectDraw_Setup's fullscreen ShowCursor(false).
		///  The fix is then handled with the Main_WndProc message WM_SETCURSOR.
		//::ShowCursor(false);

	}
	else {
		HWND hWndDesktop = ::GetDesktopWindow();

		Rect2I rect = { 0 }; // dummy init
		::GetWindowRect(hWndDesktop, reinterpret_cast<RECT*>(&rect));

		::SetWindowPos(mainGlobs.hWnd, nullptr, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER);

		/// FIX APPLY: (by removing) Show cursor over title bar
		///  This has been removed along with DirectDraw_Setup's fullscreen ShowCursor(false).
		///  The fix is then handled with the Main_WndProc message WM_SETCURSOR.
		//::SetCursor(nullptr);
	}

	// We need to track this state for menu assignment and window resizing.
	mainGlobs2.displaySetup = true;

	::ShowWindow(mainGlobs.hWnd, SW_SHOW);
	::SetActiveWindow(mainGlobs.hWnd);
}

// <LegoRR.exe @004785d0>
void __cdecl Gods98::Main_AdjustWindowRect(IN OUT Rect2I* rect)
{
	return Main_AdjustWindowRect2(rect, false);
}

/// CUSTOM: Adjust window rect while choosing to include or exclude the menu bar.
void Gods98::Main_AdjustWindowRect2(IN OUT Rect2I* rect, bool cutOutMenuBar)
{
	log_firstcall();

	if (!(mainGlobs.flags & MainFlags::MAIN_FLAG_FULLSCREEN)) {
		/// CUSTOM: Added menu
		::AdjustWindowRect(reinterpret_cast<RECT*>(rect), mainGlobs.style, !cutOutMenuBar && (Main_GetMenu() != nullptr));// false);

		//::AdjustWindowRect(reinterpret_cast<RECT*>(rect), mainGlobs.style, false);
	}
}

/// CUSTOM: Handles trapping the mouse cursor in the game window.
void Gods98::Main_HandleCursorClipping()
{
	if (!(mainGlobs.flags & MainFlags::MAIN_FLAG_FULLSCREEN) && mainGlobs.active) {

		switch (mainGlobs2.cursorClipping) {
		case CursorClipping::Off:

			::ClipCursor(nullptr);
			break;
		case CursorClipping::MenuArea:
		case CursorClipping::GameArea:
			{
				Rect2I clipRect = { 0 };
				::GetClientRect(mainGlobs.hWnd, reinterpret_cast<RECT*>(&clipRect));
				Point2I screenPt = { 0, 0 };
				::ClientToScreen(mainGlobs.hWnd, reinterpret_cast<POINT*>(&screenPt));
				clipRect.left   += screenPt.x;
				clipRect.top    += screenPt.y;
				clipRect.right  += screenPt.x;
				clipRect.bottom += screenPt.y;

				if (mainGlobs2.cursorClipping == CursorClipping::MenuArea) {
					Rect2I withMenu = { 0, 0, 0, 0 };
					Rect2I withoutMenu = { 0, 0, 0, 0 };
					Main_AdjustWindowRect2(&withMenu, false);
					Main_AdjustWindowRect2(&withoutMenu, true);
					const sint32 menuBarHeight = (withoutMenu.top - withMenu.top);

					clipRect.top -= menuBarHeight;
				}

				::ClipCursor(reinterpret_cast<RECT*>(&clipRect));
			}
			break;
		}
	}
	else {
		// Never perform cursor clipping in fullscreen mode.
		::ClipCursor(nullptr);
	}
}

// <LegoRR.exe @00478690>
void __cdecl Gods98::Main_SetTitle(const char* title)
{
	log_firstcall();

	::SetWindowTextA(mainGlobs.hWnd, title);
}

// <LegoRR.exe @004786b0>
bool32 __cdecl Gods98::Main_InitApp(HINSTANCE hInstance)
{
	log_firstcall();

	WNDCLASSA wc = { 0 }; // dummy init
	
	// Register the window style
	wc.style = CS_DBLCLKS;
	wc.lpfnWndProc = Main_WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;

	// Don't assign anything to wc.hIcon so that we
	// can keep <none> as our default class icon.
	wc.hIcon = nullptr;

//	wc.hCursor = ::LoadCursor( nullptr, IDC_ARROW );
//	wc.hbrBackground = ::GetStockObject(BLACK_BRUSH);
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = mainGlobs.className;
	
	// Check to ensure the class was actually registered
	if (::RegisterClassA(&wc)) {
		
		// Create the main window
		
		if (mainGlobs.hWnd = ::CreateWindowExA(WS_EX_APPWINDOW, mainGlobs.className, "",
			WS_POPUP | WS_SYSMENU,
			0, 0, 100, 100,
			nullptr, nullptr, hInstance, nullptr))
		{
			/// CUSTOM: Now is the time to add in our icon/menu
			///         if one was previously given to us.

			if (mainGlobs2.icon) {
				// NOTE: Always set ICON_SMALL before ICON_BIG
				::SendMessageA(mainGlobs.hWnd, WM_SETICON, ICON_SMALL, (LPARAM)mainGlobs2.icon);
				::SendMessageA(mainGlobs.hWnd, WM_SETICON, ICON_BIG,   (LPARAM)mainGlobs2.icon);
			}

			if (mainGlobs2.menu) {
				::SetMenu(mainGlobs.hWnd, mainGlobs2.menu);
			}
			
			::SetFocus(mainGlobs.hWnd);
			
			return true;
			
		} else ::MessageBoxA(nullptr, "Unable to Create Main Window", "Fatal Error", MB_OK);
	} else ::MessageBoxA(nullptr, "Unable to register window class", "Fatal Error", MB_OK);
	
	return false;
}

// <LegoRR.exe @00478780>
LRESULT __cdecl Gods98::Main_WndProc_Fullscreen(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	log_firstcall();

	/// NEW GODS98: This is not in LegoRR, but may fix some mouse input handling issues
	//INPUT.lClicked = false;
	//INPUT.rClicked = false;

	/// ADDED: API not used in LegoRR, but found in Gods98
	if (mainGlobs.windowCallback != nullptr) mainGlobs.windowCallback(hWnd, message, wParam, lParam);

	switch (message)  
	{

	// Key board messages
	case WM_KEYDOWN:
	case WM_KEYUP:
		return 0;

	// Mouse
	case WM_MOUSEMOVE:
		return 0;

	// Handle single or dual mouse
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	//case WM_MBUTTONDOWN:
	//case WM_MBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	//case WM_XBUTTONDOWN:
	//case WM_XBUTTONUP:
		{
			/// IMPORTANT: Consider that the faulty switch breaking may have intended
			///  control flow to fall into the end of the function:
			///  `::DefWindowProcA();` for Fullscreen.
			if (mainGlobs.flags & MainFlags::MAIN_FLAG_DUALMOUSE) {
				// Both buttons valid
				switch (message)
				{
				/// OLD GODS98: This is from a commmented-out preprocessor,
				///  and matches LegoRR's code.
				case WM_LBUTTONDOWN:
					INPUT.mslb = INPUT.lClicked = true;
					return 0;
				case WM_LBUTTONUP:
					INPUT.mslb = INPUT.lClicked = false;
					return 0;
				case WM_RBUTTONDOWN:
					INPUT.msrb = INPUT.rClicked = true;
					return 0;
				case WM_RBUTTONUP:
					INPUT.msrb = INPUT.rClicked = false;
					return 0;

				/// NEW GODS98: Newer input handling used instead of what's above,
				///  however this is not what's used by LegoRR.
				/*case WM_LBUTTONDOWN:
					INPUT.mslb		= true;
					INPUT.lClicked	= false;
					return 0;
				case WM_LBUTTONUP:
					INPUT.mslb		= false;
					INPUT.lClicked	= true;
					return 0;
				case WM_RBUTTONDOWN:
					INPUT.msrb		= true;
					INPUT.rClicked	= false;
					return 0;
				case WM_RBUTTONUP:
					INPUT.msrb		= false;
					INPUT.rClicked	= true;
					return 0;*/
				}
			}
			else {
				// Merged buttons
				switch (message)
				{
				/// OLD GODS98: This is from a commmented-out preprocessor,
				///  and matches LegoRR's code.
				case WM_LBUTTONDOWN:
				case WM_RBUTTONDOWN:
					if (!INPUT.mslb) {
						INPUT.lClicked = true;
						INPUT.rClicked = true;
					}
					INPUT.mslb = INPUT.msrb = true;

					/// FIXME LEGORR: Bad switch fallthrough into WM_LBUTTONDBLCLK.
					INPUT.lDoubleClicked = true;
					return 0; // use this instead of `break;`, bad switch fallthrough

				case WM_LBUTTONUP:
				case WM_RBUTTONUP:
					INPUT.lClicked = false;
					INPUT.rClicked = false;
					INPUT.mslb = INPUT.msrb = false;

					/// FIXME LEGORR: Bad switch fallthrough into WM_LBUTTONDBLCLK.
					INPUT.lDoubleClicked = true;
					return 0; // use this instead of `break;`, bad switch fallthrough

				/// NEW GODS98: Newer input handling used instead of what's above,
				///  however this is not what's used by LegoRR.
				/// NOTE: The accidental switch fallthrough is still present here,
				///  it's very likely not intentional. Should change `break;` to `return 0;`
				///  if using this code.
				/*case WM_LBUTTONDOWN:
				case WM_RBUTTONDOWN:
					if (!INPUT.mslb) {
						INPUT.lClicked = false;
						INPUT.rClicked = false;
					}
					INPUT.mslb = INPUT.msrb = true;

					/// FIXME GODS98: Bad switch fallthrough into WM_LBUTTONDBLCLK.
					INPUT.lDoubleClicked = true;
					return 0; // use this instead of `break;`, bad switch fallthrough
					//break;
				case WM_LBUTTONUP:
				case WM_RBUTTONUP:
					INPUT.lClicked = true;
					INPUT.rClicked = true;
					INPUT.mslb = INPUT.msrb = false;
					
					/// FIXME GODS98: Bad switch fallthrough into WM_LBUTTONDBLCLK.
					INPUT.lDoubleClicked = true;
					return 0; // use this instead of `break;`, bad switch fallthrough
					//break;
					*/
				}
			}
		}

	case WM_LBUTTONDBLCLK:
		INPUT.lDoubleClicked = true;
		return 0;
	case WM_RBUTTONDBLCLK:
		INPUT.rDoubleClicked = true;			
		return 0;
	//case WM_MBUTTONDBLCLK:
	//case WM_XBUTTONDBLCLK:
	//	return 0;

	// Exit messages
	case WM_CLOSE:
		/// TODO: Consider handling close button here with immediate termination
		///  (assuming the user is okay with losing save progress)
		mainGlobs.exit = true;
		break; // -> `return ::DefWindowProcA();`
	case WM_QUIT:
	case WM_DESTROY:
		mainGlobs.exit = true;
		break; // -> `return ::DefWindowProcA();`

	// Window messages
	case WM_SIZE:
	case WM_MOVE:
		return 0;

	case WM_ACTIVATE:
		return 0;

	case WM_ACTIVATEAPP:
		mainGlobs.active = (bool32) wParam; // true if this window is being activated
		return 0;

	case WM_PAINT:
	case WM_CREATE:
	case WM_COMMAND:
		break; // -> `return ::DefWindowProcA();`

	case WM_WINDOWPOSCHANGING:
		{
			WINDOWPOS *pos = (WINDOWPOS *) lParam;
			pos->flags &= ~(SWP_NOOWNERZORDER);
			return 0;
		}

	case WM_ENTERMENULOOP:
		return 0;

    case WM_EXITMENULOOP:
		return 0;

	case WM_NCACTIVATE:
		return 0;

	case WM_SYSKEYDOWN:
		return 0;

	default:
		break; // -> `return ::DefWindowProcA();`
	}

	return ::DefWindowProcA(hWnd, message, wParam, lParam); // default for all top-level switch breaks
}

// <LegoRR.exe @00478980>
LRESULT __cdecl Gods98::Main_WndProc_Windowed(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	log_firstcall();

	/// POSSIBLE MISSING GODS98: These two are present in Gods98 WndProc_Fullscreen (but not in LegoRR)
	//INPUT.lClicked = false;
	//INPUT.rClicked = false;

	/// ADDED: API not used in LegoRR, but found in Gods98
	if (mainGlobs.windowCallback != nullptr) mainGlobs.windowCallback(hWnd, message, wParam, lParam);

	switch (message)
	{
		
	// Exit messages
	case WM_CLOSE:
		/// TODO: Consider handling close button here with immediate termination
		///  (assuming the user is okay with losing save progress)
		mainGlobs.exit = true;
		break; // -> `return 0;`
	case WM_QUIT:
	case WM_DESTROY:
		mainGlobs.exit = true;
		break; // -> `return 0;`

	// Handle single or dual mouse
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
		{
			/// IMPORTANT: Consider that the faulty switch breaking may have intended
			///  control flow to fall into the end of the function:
			///  `return 0;` for Windowed (although `::DefWindowProcA();` is the end for Fullscreen)
			if (mainGlobs.flags & MainFlags::MAIN_FLAG_DUALMOUSE) {
				// Both buttons valid
				switch (message)
				{
				/// OLD GODS98: This is from a commmented-out preprocessor,
				///  and matches LegoRR's code.
				case WM_LBUTTONDOWN:
					::SetCapture(hWnd);
					INPUT.mslb = INPUT.lClicked = true;
					/// FIXME LEGORR: Bad switch fallthrough into WM_LBUTTONDBLCLK.
					INPUT.lDoubleClicked = true;
					return 0; // use this instead of `break;`, bad switch fallthrough

				case WM_LBUTTONUP:
					::ReleaseCapture();
					INPUT.mslb = INPUT.lClicked = false;
					/// FIXME LEGORR: Bad switch fallthrough into WM_LBUTTONDBLCLK.
					INPUT.lDoubleClicked = true;
					return 0; // use this instead of `break;`, bad switch fallthrough

				case WM_RBUTTONDOWN:
					/// NOTE GODS98: Newer version includes ::SetCapture() for RMB
					INPUT.msrb = INPUT.rClicked = true;
					/// FIXME LEGORR: Bad switch fallthrough into WM_LBUTTONDBLCLK.
					INPUT.lDoubleClicked = true;
					return 0; // use this instead of `break;`, bad switch fallthrough

				case WM_RBUTTONUP:
					/// NOTE GODS98: Newer version includes ::ReleaseCapture() for RMB
					INPUT.msrb = INPUT.rClicked = false;
					/// FIXME LEGORR: Bad switch fallthrough into WM_LBUTTONDBLCLK.
					INPUT.lDoubleClicked = true;
					return 0; // use this instead of `break;`, bad switch fallthrough

				/// NEW GODS98: Newer input handling used instead of what's above,
				///  however this is not what's used by LegoRR.
				/*case WM_LBUTTONDOWN:
					::SetCapture(hWnd);
					INPUT.mslb		= true;
					INPUT.lClicked	= false;		
					return 0;

				case WM_LBUTTONUP:
					::ReleaseCapture();
					INPUT.mslb		= false;
					INPUT.lClicked	= true;
					return 0;

				case WM_RBUTTONDOWN:
					::SetCapture(hWnd);
					INPUT.msrb		= true;
					INPUT.rClicked	= false;
					return 0;

				case WM_RBUTTONUP:
					::ReleaseCapture();
					INPUT.msrb		= false;
					INPUT.rClicked	= true;
					return 0;
					*/
				}
			}
			else {
				// Merged buttons
				switch (message)
				{
				/// OLD GODS98: This is from a commmented-out preprocessor,
				///  and matches LegoRR's code.
				case WM_LBUTTONDOWN:
				case WM_RBUTTONDOWN:
					::SetCapture(hWnd);
					if (!INPUT.mslb) {
						INPUT.lClicked = true;
						INPUT.rClicked = true;
					}
					INPUT.mslb = INPUT.msrb = true;
					/// FIXME LEGORR: Bad switch fallthrough into WM_LBUTTONDBLCLK.
					INPUT.lDoubleClicked = true;
					return 0; // use this instead of `break;`, bad switch fallthrough

				case WM_LBUTTONUP:
				case WM_RBUTTONUP:
					::ReleaseCapture();
					INPUT.lClicked = false;
					INPUT.rClicked = false;
					INPUT.mslb = INPUT.msrb = false;
					/// FIXME LEGORR: Bad switch fallthrough into WM_LBUTTONDBLCLK.
					INPUT.lDoubleClicked = true;
					return 0; // use this instead of `break;`, bad switch fallthrough

				/// NEW GODS98: Newer input handling used instead of what's above,
				///  however this is not what's used by LegoRR.
				/// NOTE: The accidental switch fallthrough is still present here,
				///  it's very likely not intentional. Should change `break;` to `return 0;`
				///  if using this code.
				/*case WM_LBUTTONDOWN:
				case WM_RBUTTONDOWN:
					::SetCapture(hWnd);
					if (!INPUT.mslb) {
						INPUT.lClicked = false;
						INPUT.rClicked = false;
					}
					INPUT.mslb = INPUT.msrb = true;

					/// FIXME GODS98: Bad switch fallthrough into WM_LBUTTONDBLCLK.
					INPUT.lDoubleClicked = true;
					return 0; // use this instead of `break;`, bad switch fallthrough
					//break;
				case WM_LBUTTONUP:
				case WM_RBUTTONUP:
					::ReleaseCapture();
					INPUT.lClicked = true;
					INPUT.rClicked = true;
					INPUT.mslb = INPUT.msrb = false;

					/// FIXME GODS98: Bad switch fallthrough into WM_LBUTTONDBLCLK.
					INPUT.lDoubleClicked = true;
					return 0; // use this instead of `break;`, bad switch fallthrough
					//break;
					*/
				}
			}
		}

	case WM_LBUTTONDBLCLK:
		INPUT.lDoubleClicked = true;
		return 0;
		//break; // -> `return 0;` (was break intentional?)
	case WM_RBUTTONDBLCLK:
		INPUT.rDoubleClicked = true;
		return 0;
		//break; // -> `return 0;` (was break intentional?)

	case WM_ACTIVATEAPP:
		mainGlobs.active = (bool32) wParam; // true if this window is being activated
		return 0;

	case WM_ACTIVATE:
		break; // -> `return 0;`

	default:
		// The choosing of moving Windowed `::DefWindowProcA();` to the default statement may
		//  have something to do with changing how `break;` performs for windowed/fullscreen.
		// When window messages should be handled likely has some important differences in
		//  these two modes.
		return ::DefWindowProcA(hWnd, message, wParam, lParam);
	}

	return 0; // default for all top-level switch breaks
}

// <LegoRR.exe @00478b40>
LRESULT __stdcall Gods98::Main_WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	log_firstcall();
	
	/// APPLY FIX: Show cursor in when hovering over title bar
	/// <https://stackoverflow.com/a/14134212/7517185>
	if (message == WM_SETCURSOR) {

		// Only handle if mouse is inside the main window.
		if ((HWND)wParam == Main_hWnd()) {
			switch (mainGlobs2.cursorVisibility) {
			case CursorVisibility::Never:
				::SetCursor(nullptr);
				return (LRESULT)true;

			case CursorVisibility::TitleBar:
				// are we inside the client area? If so, erase the cursor like normal
				if (LOWORD(lParam) == HTCLIENT) {
					::SetCursor(nullptr);
					return (LRESULT)true;
				}
				break;

			case CursorVisibility::Always:
				break; // nothing to handle, default cursor will be shown
			}

			// Show the cursor as normal
			return ::DefWindowProcA(hWnd, message, wParam, lParam);
		}
    }

	if (mainGlobs.flags & MainFlags::MAIN_FLAG_FULLSCREEN) {
		return Main_WndProc_Fullscreen(hWnd, message, wParam, lParam);
	}
	else {
		return Main_WndProc_Windowed(hWnd, message, wParam, lParam);
	}
}


// <missing>
void __cdecl Gods98::Main_PauseApp(bool32 pause)
{
	if (pause)
		mainGlobs.flags |= MainFlags::MAIN_FLAG_PAUSED;
	else
		mainGlobs.flags &= ~MainFlags::MAIN_FLAG_PAUSED;

	/// CUSTOM:
	Main_SetAdvanceFrames(0);
}

// <missing>
bool32 __cdecl Gods98::Main_IsPaused(void)
{
	return (mainGlobs.flags & MainFlags::MAIN_FLAG_PAUSED);
}


// <LegoRR.exe @00478c40>
bool32 __cdecl Gods98::Main_SetCDVolume(real32 leftVolume, real32 rightVolume)
{
	log_firstcall();

	return Main_CDVolume(&leftVolume, &rightVolume, true);
}

// <LegoRR.exe @00478c60>
bool32 __cdecl Gods98::Main_GetCDVolume(OUT real32* leftVolume, OUT real32* rightVolume)
{
	log_firstcall();

	return Main_CDVolume(leftVolume, rightVolume, false);
}

// <LegoRR.exe @00478c80>
bool32 __cdecl Gods98::Main_CDVolume(IN OUT real32* leftVolume, IN OUT real32* rightVolume, bool32 set)
{
	log_firstcall();

	/// TODO: Go over and check this function against LegoRR,
	///  this is one of the few Gods98 functions not closely looked at.

	bool32 result = false;

	if (set) {
		if (*leftVolume  > 1.0f) *leftVolume  = 1.0f;
		if (*leftVolume  < 0.0f) *leftVolume  = 0.0f;
		if (*rightVolume > 1.0f) *rightVolume = 1.0f;
		if (*rightVolume < 0.0f) *rightVolume = 0.0f;
	}

	uint32 deviceCount = ::mixerGetNumDevs();

	for (uint32 devID = 0; devID < deviceCount; devID++) {

		HMIXER hMixer;
		if (::mixerOpen(&hMixer, devID, 0, 0, MIXER_OBJECTF_MIXER) != MMSYSERR_NOERROR)
			continue; // safely continue, nothing to cleanup from this for-loop yet

		HMIXEROBJ mixer = (HMIXEROBJ)hMixer; // second variable created to avoid extra casting

		MIXERCAPS caps;
		std::memset(&caps, 0, sizeof(caps));
		::mixerGetDevCapsA(devID, &caps, sizeof(caps));

		for (uint32 destID = 0; destID < caps.cDestinations; destID++) {

			MIXERLINE mixerLine;
			std::memset(&mixerLine, 0, sizeof(mixerLine));
			mixerLine.cbStruct = sizeof(mixerLine);
			mixerLine.dwDestination = destID;

			if (::mixerGetLineInfoA(mixer, &mixerLine, MIXER_GETLINEINFOF_DESTINATION) != MMSYSERR_NOERROR)
				continue; // safely continue, nothing to cleanup from this for-loop yet

			uint32 cConnections = mixerLine.cConnections;

			for (uint32 connID = 0; connID < cConnections; connID++) {

				mixerLine.cbStruct = sizeof(mixerLine);
				mixerLine.dwDestination = destID;
				mixerLine.dwSource = connID;

				
				if (::mixerGetLineInfoA(mixer, &mixerLine, MIXER_GETLINEINFOF_SOURCE) != MMSYSERR_NOERROR ||
					(mixerLine.dwComponentType != MIXERLINE_COMPONENTTYPE_SRC_COMPACTDISC))
				{
					continue; // safely continue, nothing to cleanup from this for-loop yet
				}

				MIXERLINECONTROLS controls;
				MIXERCONTROL* ctrlList;

				std::memset(&controls, 0, sizeof(controls));
				controls.cbStruct = sizeof(controls);
				controls.dwLineID = mixerLine.dwLineID;
				controls.cControls = mixerLine.cControls;
				controls.cbmxctrl = sizeof(*ctrlList);

				ctrlList = (MIXERCONTROL*)Mem_Alloc(sizeof(*ctrlList) * controls.cControls);
				controls.pamxctrl = ctrlList;

				if (::mixerGetLineControlsA(mixer, &controls, MIXER_GETLINECONTROLSF_ALL) == MMSYSERR_NOERROR) {
					for (uint32 ctrlLoop = 0; ctrlLoop < mixerLine.cControls; ctrlLoop++) {

						if (!(ctrlList[ctrlLoop].dwControlType & MIXERCONTROL_CT_CLASS_FADER) ||
							!(ctrlList[ctrlLoop].dwControlType & MIXERCONTROL_CONTROLTYPE_VOLUME))
						{
							continue; // safely continue, nothing to cleanup from this for-loop yet
						}

						MIXERCONTROLDETAILS_UNSIGNED* valueList;

						MIXERCONTROLDETAILS details;
						std::memset(&details, 0, sizeof(details));
						details.cbStruct = sizeof(details);
						details.dwControlID = ctrlList[ctrlLoop].dwControlID;
						details.cbDetails = sizeof(*valueList);
						details.cChannels = mixerLine.cChannels;
						details.cMultipleItems = 0;

						valueList = (MIXERCONTROLDETAILS_UNSIGNED*)Mem_Alloc(sizeof(*valueList) * mixerLine.cChannels);
						details.paDetails = valueList;

						uint32 boundsMin = ctrlList[ctrlLoop].Bounds.dwMinimum;
						uint32 boundsRange = (ctrlList[ctrlLoop].Bounds.dwMaximum - boundsMin);
						if (set) {
							uint32 uwVolumeL = boundsMin + (uint32)(*leftVolume  * boundsRange);
							uint32 uwVolumeR = boundsMin + (uint32)(*rightVolume * boundsRange);
							if (mixerLine.cChannels == 2) {
								valueList[0].dwValue = uwVolumeL;
								valueList[1].dwValue = uwVolumeR;
							}
							else {
								for (uint32 vLoop = 0; vLoop < mixerLine.cChannels; vLoop++)
									valueList[vLoop].dwValue = (uwVolumeL + uwVolumeR) / 2;
							}
							if (::mixerSetControlDetails(mixer, &details, MIXER_SETCONTROLDETAILSF_VALUE) == MMSYSERR_NOERROR) {
								result = true;
							}
						}
						else {
							if (::mixerGetControlDetailsA(mixer, &details, MIXER_GETCONTROLDETAILSF_VALUE) == MMSYSERR_NOERROR) {

								*leftVolume = (real32)(valueList[0].dwValue - boundsMin) / boundsRange;
								if (mixerLine.cChannels == 2)
									*rightVolume = (real32)(valueList[1].dwValue - boundsMin) / boundsRange;
								else
									*rightVolume = *leftVolume;

								result = true;
							}
						}

						Mem_Free(valueList);
					}
				}
				Mem_Free(ctrlList);
			}
		}

		::mixerClose(hMixer);
	}

	return result;
}


// <missing>
void __cdecl Gods98::Main_SetWindowCallback(MainWindowCallback callback)
{
	mainGlobs.windowCallback = callback;
}


// This is kept for backwards compatibility,
//  if we ever work with unoptimized Beta builds.
// <missing>
void __cdecl Gods98::Main_SetAccel1(HACCEL accels)
{
	Main_SetAccel(accels, false); // no ownership management (original behavior)
}


/// EXTENDED: Added ownership parameter and handling.
void __cdecl Gods98::Main_SetAccel(HACCEL newAccels, bool32 owner)
{
	if (mainGlobs.accels != newAccels) {
		if (mainGlobs.accels && mainGlobs2.accelsOwner) {
			// Perform cleanup if we took ownership of the previous accels.
			::DestroyAcceleratorTable(mainGlobs.accels);
		}

		mainGlobs.accels = newAccels;
		mainGlobs2.accelsOwner = (owner && newAccels != nullptr);

		// No setup required, accels is simply used in Main_HandleIO.
	}
}

/// CUSTOM:
void __cdecl Gods98::Main_SetIcon(HICON newIcon, bool32 owner)
{
	if (mainGlobs2.icon != newIcon) {
		if (mainGlobs2.icon && mainGlobs2.iconOwner) {
			// Perform cleanup if we took ownership of the previous icon.
			::DestroyIcon(mainGlobs2.icon);
		}

		mainGlobs2.icon = newIcon;
		mainGlobs2.iconOwner = (owner && newIcon != nullptr);

		if (Main_hWnd() && Main_IsDisplaySetup()) {
			// NOTE: Always set ICON_SMALL before ICON_BIG
			::SendMessageA(Main_hWnd(), WM_SETICON, ICON_SMALL, (LPARAM)newIcon);
			::SendMessageA(Main_hWnd(), WM_SETICON, ICON_BIG,   (LPARAM)newIcon);
		}
	}
}

/// CUSTOM:
void __cdecl Gods98::Main_SetMenu(HMENU newMenu, bool32 owner)
{
	if (mainGlobs2.menu != newMenu) {
		if (mainGlobs2.menu && mainGlobs2.menuOwner) {
			// Perform cleanup if we took ownership of the previous menu.
			::DestroyMenu(mainGlobs2.menu);
		}

		mainGlobs2.menu = newMenu;
		mainGlobs2.menuOwner = (owner && newMenu != nullptr);

		if (Main_hWnd()) {
			::SetMenu(Main_hWnd(), newMenu);

			if (Main_IsDisplaySetup() && !Main_FullScreen()) {
				// Re-adjust window size to account for difference with/without menu.

				Rect2I rect = { 0 }; // dummy init
				::GetWindowRect(Main_hWnd(), reinterpret_cast<RECT*>(&rect));
				rect.right  = rect.left + appWidth() * Main_Scale();
				rect.bottom = rect.top + appHeight() * Main_Scale();

				Main_AdjustWindowRect(&rect);
				Main_AdjustWindowPosition(&rect);
				// Don't pass position, since we're not modifying that.
				::SetWindowPos(Main_hWnd(), nullptr, 0, 0, rect.right - rect.left, rect.bottom - rect.top, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
			}
		}
	}
}

/// CUSTOM: Set behavior for mouse cursor visibility.
void __cdecl Gods98::Main_SetCursorVisibility(CursorVisibility newCursorVisibility)
{
	if (newCursorVisibility != mainGlobs2.cursorVisibility) {
		// Make any changes to transition out of the current Cursor fix setting.
		switch (mainGlobs2.cursorVisibility) {
		case CursorVisibility::Never:
			::ShowCursor(true); // Raise our ShowCursor count back up to 1
			break;

		case CursorVisibility::TitleBar:
			break; // fully handled by WM_SETCURSOR

		case CursorVisibility::Always:
			break; // fully handled by WM_SETCURSOR
		}

		mainGlobs2.cursorVisibility = newCursorVisibility;

		// Transition into the new cursor fix setting.
		switch (newCursorVisibility) {
		case CursorVisibility::Never:
			::ShowCursor(false); // Lower our ShowCursor count back down to 0
			break;

		case CursorVisibility::TitleBar:
			break; // fully handled by WM_SETCURSOR

		case CursorVisibility::Always:
			/*// This is not required, as WM_SETCURSOR will trigger the
			//  moment the cursor moves or produces another related event.
			if (mouseInsideWindow && ::GetCursor() == nullptr) {
				// Force-display of the cursor now, without moving.
				::SetCursor(mainGlobs2.hCursorArrow);
			}*/
			break;
		}

	}
}

/// CUSTOM: Get type of behaviour for mouse trapping in the game window.
void Gods98::Main_SetCursorClipping(CursorClipping newCursorClipping)
{
	mainGlobs2.cursorClipping = newCursorClipping;
}

/// CUSTOM: Get number of frames advance to when in the Main_Paused state (always returns >= 0).
sint32 __cdecl Gods98::Main_GetAdvanceFrames(void)
{
	if (mainGlobs.flags & MainFlags::MAIN_FLAG_PAUSED) { // alt: if (Main_IsPaused()) {
		return std::max(0, mainGlobs2.advanceFrames);
	}
	return 0;
}

/// CUSTOM: Set to advance one frame when in the Main_Paused state.
void __cdecl Gods98::Main_AdvanceFrame(void)
{
	Main_SetAdvanceFrames(1);
}

/// CUSTOM: Set number of frames advance to when in the Main_Paused state.
void __cdecl Gods98::Main_SetAdvanceFrames(sint32 frames)
{
	if (mainGlobs.flags & MainFlags::MAIN_FLAG_PAUSED) { // alt: if (Main_IsPaused()) {
		mainGlobs2.advanceFrames = std::max(0, frames);
	}
}

/// CUSTOM: Add number of frames advance to when in the Main_Paused state.
void __cdecl Gods98::Main_AddAdvanceFrames(sint32 frames)
{
	Main_SetAdvanceFrames(mainGlobs2.advanceFrames + frames);
}

/// CUSTOM: Get if the mouse is currently over a visible section of the main window.
bool32 __cdecl Gods98::Main_MouseInsideWindow(void)
{
	POINT cursorPos = { 0, 0 };
	if (::GetCursorPos(&cursorPos)) {
		HWND hWndTarget = WindowFromPoint(cursorPos);
		if (hWndTarget == Main_hWnd()) {
			return true;
		}
	}
	return false;
}

#pragma endregion
