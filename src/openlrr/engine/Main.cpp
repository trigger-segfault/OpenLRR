// Main.cpp : 
//

#include "../platform/windows.h"
#include "../platform/d3drm.h"		// <d3drmwin.h>
#include "../platform/timeapi.h"
#include "../../../resources/resource.h"

#include "geometry.h"

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
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @00506800>
Gods98::Main_Globs & Gods98::mainGlobs = *(Gods98::Main_Globs*)0x00506800;

Gods98::Main_Globs2 Gods98::mainGlobs2 = { 0 };

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

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


// <LegoRR.exe @00477a60>
sint32 __stdcall Gods98::Main_WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
							          _In_ LPSTR     lpCmdLine, _In_     sint32    nCmdShow)
{
	log_firstcall();

	bool32 setup = false, nosound = false, insistOnCD = false;
	const char* productName = nullptr;
	const char* productRegistry = nullptr;
	char mutexName[128];
	char noHALMsg[1024];


	/// OLD LEGORR: Mutex setup is called at the very beginning in LegoRR
	///  (but it's essential to be the first here)
	/*productName = "Lego Rock Raiders";
	productRegistry = "SOFTWARE\\LEGO Media\\Games\\Rock Raiders";

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
	for (s=cl= ::GetCommandLineA() ; *cl!='\0' ; cl++) if (*cl == '\\') s = cl+1;		// Find the last backslash.
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
	productRegistry = "SOFTWARE\\LEGO Media\\Games\\Rock Raiders";

	if (productName) { // this realistically shoudn't ever be false
		std::sprintf(mutexName, "%s Mutex", productName);
		::CreateMutexA(nullptr, true, mutexName);
		if (::GetLastError() == ERROR_ALREADY_EXISTS) {
			return 0;  // App is already running
		}
	}


	/// NEW GODS98: Not called in LegoRR WinMain
	// Moved after mutex check, so we wont't need to call ::CoUninitialize() there.
	::CoInitialize(nullptr);


	// Setup the default globals
	mainGlobs.className = mainGlobs.programName;
	mainGlobs.active = false;
	mainGlobs.exit = false;
	mainGlobs.stateSet = false;
	mainGlobs.hInst = hInstance;
	mainGlobs.fixedFrameTiming = 0.0f;
	mainGlobs.flags = MainFlags::MAIN_FLAG_NONE;

	{
		char commandLine[1024];
		char tempStr[1024];

		if (Registry_GetValue(productRegistry, "StandardParameters", RegistryValue::String, tempStr, sizeof(tempStr))) {
			std::sprintf(commandLine, "%s %s", lpCmdLine, tempStr);
		} else std::sprintf(commandLine, "%s", lpCmdLine);

		/// NEW GODS98: Not present in LegoRR.exe
//		//mainGlobs.flags |= MainFlags::MAIN_FLAG_BEST;
		//mainGlobs.flags |= MainFlags::MAIN_FLAG_DUALMOUSE;
		Main_ParseCommandLine(commandLine, &nosound, &insistOnCD);
	}

	if (!Registry_GetValue(productRegistry, "NoHALMessage", RegistryValue::String, noHALMsg, sizeof(noHALMsg))) {
		std::sprintf(noHALMsg, "No DirectX 3D accelerator could be found.");
	}

	// Initialise everything (memory, window, DirectDraw, etc.)
	Error_Initialise();
	Mem_Initialise();

	/// OPENLRR TEMP: Heavier measures against OpenLRR-named executables causing crashes from WAD load failures
	const char* wadProgramName = "LegoRR";
	if (::_stricmp(mainGlobs.programName, "OpenLRR") != 0 &&
		::_stricmp(mainGlobs.programName, "OpenLRR-d") != 0)
	{
		// Only use the executable name when not "OpenLRR(-d)". This is a terrible solution...
		wadProgramName = mainGlobs.programName;
	}
	File_Initialise(wadProgramName, insistOnCD, productRegistry);

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
		if (Sound_Initialise(nosound) &&
			Init_Initialise(setup /*true in LegoRR*/,
							mainGlobs.flags & MainFlags::MAIN_FLAG_DEBUGMODE,
							mainGlobs.flags & MainFlags::MAIN_FLAG_BEST,
							mainGlobs.flags & MainFlags::MAIN_FLAG_WINDOW,
							noHALMsg))
		{
			Animation_Initialise(DirectDraw());

			Draw_Initialise(nullptr);
			/// NEW GODS98: This is called in LegoRR_Initialise, but not in Gods98
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
					real32 time = 1.0f;
					//uint32 currTime;

					uint32 lastTime = Main_GetTime();

					while (!mainGlobs.exit) {
						
						// Handle windows messages and input...
						INPUT.lClicked = false;
						INPUT.rClicked = false;
						INPUT.lDoubleClicked = false;
						INPUT.rDoubleClicked = false;

						Main_HandleIO();
						// In fullscreen mode we will always be the active application or I will eat my hat.
						//  --Whoever wrote this 20 years ago, I'll hold you to it
						if (mainGlobs.flags & MainFlags::MAIN_FLAG_FULLSCREEN) mainGlobs.active = true;

						Input_ReadKeys();
						Input_ReadMouse2();

						// Run the main loop (pass 1.0f as the initial timing value)
						if (!mainGlobs.currState.MainLoop(time)) mainGlobs.exit = true;

						// Update the device and flip the surfaces...
						Graphics_Finalise3D();
						DirectDraw_Flip();
						// Set as not-updated again while the game loop is running.
						mainGlobs.flags &= ~MainFlags::MAIN_FLAG_UPDATED;

						if (mainGlobs.flags & MainFlags::MAIN_FLAG_DUMPMODE) {
							// In LegoRR, this state is never reachable.
							time = STANDARD_FRAMERATE / 30.0f;

						} else if (mainGlobs.flags & MainFlags::MAIN_FLAG_PAUSED) {
							// In LegoRR, this state is never reachable because Main_SetPaused is never used.
							if (mainGlobs2.advanceFrames > 0) {
								mainGlobs2.advanceFrames--;
								// We could bulk-execute this, but it'll likely break the engine.
								time = 1.0f; // ((mainGlobs.fixedFrameTiming == 0.0f) ? 1.0f : mainGlobs.fixedFrameTiming);
							}
							else {
								time = 0.0f;
							}
							lastTime = Main_GetTime();
						} else if (mainGlobs.fixedFrameTiming == 0.0f) { // no fps defined
							// Measure the time taken over the last frame (to be passed next loop)
							uint32 currTime = Main_GetTime();
							time = ((real32)(currTime - lastTime)) / (1000.0f / STANDARD_FRAMERATE); // milliseconds to standard units
							lastTime = currTime;
#ifndef _UNLIMITEDUPDATETIME
							// LegoRR compiles with this preprocessor undefined (this check still happens).
							if (time > 3.0f) {
								time = 3.0f;
							}
#endif // _UNLIMITEDUPDATETIME

						} else {
							// Always update with the same time, regardless of actual time passed.
							time = mainGlobs.fixedFrameTiming;
							lastTime = Main_GetTime();
						}
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

	/// NEW GODS98: Not called in LegoRR WinMain
	::CoUninitialize();

	return 0;
}


// <missing>
void __cdecl Gods98::Main_Exit(void)
{
	/// NEW GODS98: Not called in LegoRR WinMain, so it's likely this wasn't
	///  called here either, making Main_Exit an inline alias for `std::exit(0);`
	::CoUninitialize();
	std::exit(0);
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
	log_firstcall();

	Main_HandleIO();
	Input_ReadKeys();
	Input_ReadMouse2();

	// Update the device and flip the surfaces...
	Graphics_Finalise3D();
	DirectDraw_Flip();

	if (clear) DirectDraw_Clear(nullptr, 0 /*black*/);
	mainGlobs.flags &= ~MainFlags::MAIN_FLAG_UPDATED;

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

	return legacy::timeGetTime();
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
			(sint32) (xPos + width),
			(sint32) (yPos + height),
		};
		mainGlobs.style = WS_POPUP | WS_SYSMENU | WS_CAPTION;
		
		Main_AdjustWindowRect(&rect);
		
		::SetWindowLongA(mainGlobs.hWnd, GWL_STYLE, mainGlobs.style);
		::SetWindowPos(mainGlobs.hWnd, nullptr, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER);

		/// FIX APPLY: (by removing) Show cursor over title bar
		///  This has been removed along with DirectDraw_Setup's fullscreen ShowCursor(false).
		///  The fix is then handled with the Main_WndProc message WM_SETCURSOR.
		//::ShowCursor(false);

	} else {
		HWND hWndDesktop = ::GetDesktopWindow();

		Rect2I rect = { 0 }; // dummy init
		::GetWindowRect(hWndDesktop, (RECT*)&rect);

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
	log_firstcall();

	if (!(mainGlobs.flags & MainFlags::MAIN_FLAG_FULLSCREEN)) {
		/// CUSTOM: Added menu
		::AdjustWindowRect((RECT*)rect, mainGlobs.style, Main_GetMenu() != nullptr);// false);

		//::AdjustWindowRect((RECT*)rect, mainGlobs.style, false);
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
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
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
				rect.right  = rect.left + appWidth();
				rect.bottom = rect.top + appHeight();

				Main_AdjustWindowRect(&rect);
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
