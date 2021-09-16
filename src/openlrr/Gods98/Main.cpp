// Main.cpp : 
//

#include <d3drmwin.h>

#include "../Legacy/legacy_d3drm.h"
#include "../Legacy/legacy_timeapi.h"
#include "../Types/geometry.h"

#include "Animation.h"
#include "Config.h"
#include "DirectDraw.h"
#include "Draw.h"
#include "Dxbug.h"
#include "Errors.h"
#include "Files.h"
#include "Fonts.h"
#include "Images.h"
#include "Input.h"
#include "Memory.h"
#include "Registry.h"
#include "Sound.h"
#include "Utils.h"
#include "../Gods98Init/Init.h"

#include "Main.h"

#include "../LegoRR/Lego.h"  // Gods_Go


/**********************************************************************************
 ******** Forward Global Namespace Declarations
 **********************************************************************************/

#pragma region Forward Declarations

/*#define Input_InitKeysAndDI ((void (__cdecl*)(void))0x0047f050)
#define Input_ReadKeys ((void (__cdecl*)(void))0x0047f1b0)
#define Input_ReadMouse2 ((void (__cdecl*)(void))0x0047f2d0)
#define Input_ReleaseKeysAndDI ((void (__cdecl*)(void))0x0047f290)*/


/// TODO: Remove me once Sound module is finished
//#define Sound_Initialise ((bool32(__cdecl*)(bool32))0x00488e10)

/// TODO: Remove me once Animation module is finished
//#define Animation_Initialise ((void(__cdecl*)(IDirectDraw4*))0x0047ef40)
/// TODO: Remove me once Animation module is finished
//#define Draw_Initialise ((void(__cdecl*)(const Area2F*))0x00486140)
/// TODO: Remove/choose to add me once Images module and Lego_Initialise are finished
//#define Image_Initialise ((void(__cdecl*)(void*))0x0047d6d0)
/// TODO: Remove me once Gods_Go function is implemented
//#define Gods_Go ((void (__cdecl*)(const char*))0x0041f950)

#pragma endregion


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

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

/*
// <inlined>
__inline IDirect3DRM3* __cdecl Gods98::lpD3DRM()
{
	return mainGlobs.lpD3DRM;
}

// <inlined>
__inline IDirect3DRMDevice3* __cdecl Gods98::lpDevice()
{
	return mainGlobs.device;
}

// <inlined>
__inline IDirect3DDevice3* __cdecl Gods98::lpIMDevice()
{
	return mainGlobs.imDevice;
}

// <missing>
__inline bool32 __cdecl Gods98::Main_VideoTexture()
{
	return mainGlobs.flags & MainFlags::MAIN_FLAG_VIDEOTEXTURE;
}

// <inlined>
__inline bool32 __cdecl Gods98::Main_MIPMapEnabled()
{
	return mainGlobs.flags & MainFlags::MAIN_FLAG_MIPMAPENABLED;
}

// <inlined>
__inline bool32 __cdecl Gods98::Main_FullScreen()
{
	return mainGlobs.flags & MainFlags::MAIN_FLAG_FULLSCREEN;
}

// <inlined>
__inline uint32 __cdecl Gods98::Main_GetFogMethod()
{
	return mainGlobs.fogMethod;
}

// <inlined>
__inline void __cdecl Gods98::Main_SetFogMethod(uint32 m)
{
	mainGlobs.fogMethod = m;
}

// <inlined>
__inline HWND __cdecl Gods98::Main_hWnd(void)
{
	return mainGlobs.hWnd;
}

// <inlined>
__inline HINSTANCE __cdecl Gods98::Main_hInst(void)
{
	return mainGlobs.hInst;
}

// <inlined>
__inline bool32 __cdecl Gods98::Main_AppActive()
{
	return mainGlobs.active;
}

// <inlined>
__inline Gods98::MainFlags __cdecl Gods98::Main_GetFlags()
{
	return mainGlobs.flags;
}
*/

// <LegoRR.exe @00401b30>
uint32 __cdecl Gods98::noinline(Main_ProgrammerMode)(void)
{
	log_firstcall();

	return Main_ProgrammerMode();
}

// <LegoRR.exe @00401b40>
const char* __cdecl Gods98::noinline(Main_GetStartLevel)(void)
{
	log_firstcall();

	return Main_GetStartLevel();
	//return (mainGlobs.flags & MainFlags::MAIN_FLAG_STARTLEVEL) ? mainGlobs.startLevel : nullptr;
}

// <LegoRR.exe @00401b70>
sint32 __cdecl Gods98::noinline(appWidth)(void)
{
	log_firstcall();

	return appWidth();
}

// <LegoRR.exe @00401b80>
sint32 __cdecl Gods98::noinline(appHeight)(void)
{
	log_firstcall();

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

	/// NEW GODS98: Not called in LegoRR WinMain
	::CoInitialize(nullptr);


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
	}
	Mem_Free(getCL); // no longer used


	/// OLD LEGORR: Product mutex name was hardcoded to this:
	productName = "Lego Rock Raiders";
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


	// Setup the default globals
	mainGlobs.className = mainGlobs.programName;
	mainGlobs.active = false;
	mainGlobs.exit = false;
	mainGlobs.stateSet = false;
	mainGlobs.hInst = hInstance;
	mainGlobs.fixedFrameTiming = 0.0f;
	mainGlobs.flags = MainFlags::MAIN_FLAG_NONE /*0x0*/;

	{
		char commandLine[1024];
		char tempStr[1024];

		if (Registry_GetValue(productRegistry, "StandardParameters", RegistryType::REGISTRY_STRING_VALUE, tempStr, sizeof(tempStr))) {
			std::sprintf(commandLine, "%s %s", lpCmdLine, tempStr);
		} else std::sprintf(commandLine, "%s", lpCmdLine);

		/// NEW GODS98: Not present in LegoRR.exe
//		//mainGlobs.flags |= MainFlags::MAIN_FLAG_BEST;
		//mainGlobs.flags |= MainFlags::MAIN_FLAG_DUALMOUSE;
		Main_ParseCommandLine(commandLine, &nosound, &insistOnCD);
	}

	if (!Registry_GetValue(productRegistry, "NoHALMessage", RegistryType::REGISTRY_STRING_VALUE, noHALMsg, sizeof(noHALMsg))) {
		std::sprintf(noHALMsg, "No DirectX 3D accelerator could be found.");
	}

	// Initialise everything (memory, window, DirectDraw, etc.)
	Error_Initialise();
	Mem_Initialise();
	File_Initialise(mainGlobs.programName, insistOnCD, productRegistry);
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
			if (mainGlobs.stateSet){

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
				if (mainGlobs.currState.MainLoop) {
					
					// Use the MultiMedia timer to give a 'realtime passed' value
					// per frame to the main loop (in 25th's of a second)
					real32 time = 1.0f;
					//uint32 currTime;

					uint32 lastTime = legacy::timeGetTime();

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
						Main_Finalise3D();
						DirectDraw_Flip();
						// Set as not-updated again while the game loop is running.
						mainGlobs.flags &= ~MainFlags::MAIN_FLAG_UPDATED;

						if (mainGlobs.flags & MainFlags::MAIN_FLAG_DUMPMODE) {
							// In LegoRR, this state is never reachable.
							time = STANDARD_FRAMERATE / 30.0f;

						} else if (mainGlobs.flags & MainFlags::MAIN_FLAG_PAUSED) {
							// In LegoRR, this state is never reachable because Main_SetPaused is never used.
							time = 0.0f;
							lastTime = legacy::timeGetTime();
						} else if (mainGlobs.fixedFrameTiming == 0.0f) { // no fps defined
							// Measure the time taken over the last frame (to be passed next loop)
							uint32 currTime = legacy::timeGetTime();
							time = ((real32)(currTime - lastTime)) / (1000.0f / STANDARD_FRAMERATE);
							lastTime = currTime;
#ifndef _UNLIMITEDUPDATETIME
							// LegoRR compiles with this preprocessor undefined (this check still happens).
							if (time > 3.0f) {
								time = 3.0f;
							}
#endif // _UNLIMITEDUPDATETIME

						} else {
							time = mainGlobs.fixedFrameTiming;
							lastTime = legacy::timeGetTime();
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
	//::CoUninitialize();
	std::exit(0);
}


// <LegoRR.exe @00477e90>
void __cdecl Gods98::Main_DisableTextureManagement(void)
{
	log_firstcall();

	if (!(mainGlobs.flags & MainFlags::MAIN_FLAG_FORCETEXTUREMANAGEMENT)) {
		mainGlobs.flags |= MainFlags::MAIN_FLAG_DONTMANAGETEXTURES;
	}
}

// <LegoRR.exe @00477eb0>
void __cdecl Gods98::Main_ParseCommandLine(const char* lpCmdLine, OUT bool32* nosound, OUT bool32* insistOnCD)
{
	log_firstcall();

	const char* loc;
	char langDir[128];

	if (Util_StrIStr(lpCmdLine, "-insistOnCD")) *insistOnCD = true;
	if (Util_StrIStr(lpCmdLine, "-nosound")) *nosound = true;
	if (Util_StrIStr(lpCmdLine, "-debug")) mainGlobs.flags |= MainFlags::MAIN_FLAG_DEBUGMODE;
	if (Util_StrIStr(lpCmdLine, "-nm")) mainGlobs.flags |= MainFlags::MAIN_FLAG_DONTMANAGETEXTURES;
	if (Util_StrIStr(lpCmdLine, "-ftm")) mainGlobs.flags |= MainFlags::MAIN_FLAG_FORCETEXTUREMANAGEMENT;
	if (Util_StrIStr(lpCmdLine, "-fvf")) mainGlobs.flags |= MainFlags::MAIN_FLAG_FORCEVERTEXFOG;
	if (Util_StrIStr(lpCmdLine, "-best")) mainGlobs.flags |= MainFlags::MAIN_FLAG_BEST;
	if (Util_StrIStr(lpCmdLine, "-window")) mainGlobs.flags |= MainFlags::MAIN_FLAG_WINDOW;
	if (Util_StrIStr(lpCmdLine, "-nodualmouse")) mainGlobs.flags &= ~MainFlags::MAIN_FLAG_DUALMOUSE;
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
	if (loc = Util_StrIStr(lpCmdLine, "-flags")) mainGlobs.clFlags = std::atoi(&loc[std::strlen("-flags")]);
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
	Main_Finalise3D();
	DirectDraw_Flip();

	if (clear) DirectDraw_Clear(nullptr, 0 /*black*/);
	mainGlobs.flags &= ~MainFlags::MAIN_FLAG_UPDATED;
}

// <LegoRR.exe @00478230>
uint32 __cdecl Gods98::Main_GetCLFlags(void)
{
	log_firstcall();

	return mainGlobs.clFlags;
}

// <LegoRR.exe @00478240>
uint32 __cdecl Gods98::Main_GetWindowsBitDepth(void)
{
	log_firstcall();

	return ::GetDeviceCaps(::GetDC(Main_hWnd()), BITSPIXEL);
}

// <LegoRR.exe @00478260>
void __cdecl Gods98::Main_Finalise3D(void)
{
	log_firstcall();

	if (!(mainGlobs.flags & MainFlags::MAIN_FLAG_UPDATED)) {
		mainGlobs.device->Update();
		mainGlobs.flags |= MainFlags::MAIN_FLAG_UPDATED;
	}
}

// <LegoRR.exe @00478290>
bool32 __cdecl Gods98::Main_SetState(Main_State* state)
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
	if (mainGlobs.accels != nullptr)
	{
		while (::PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE)) {
			if (!::TranslateAcceleratorA(Main_hWnd(), mainGlobs.accels, &msg)) {
				::TranslateMessage(&msg);
				::DispatchMessageA(&msg);
			}
		}
	}
	else
	{
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
		
		RECT rect = {
			xPos,
			yPos,
			xPos + width,
			yPos + height,
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

		RECT rect;
		::GetWindowRect(hWndDesktop, &rect);
		::SetWindowPos(mainGlobs.hWnd, nullptr, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER);

		// Shows an empty/no(?) cursor in the fullscreen window
		::SetCursor((HCURSOR)false); // NOTE: false was used here by Gods98, though it may be a mistake
	}

	::ShowWindow(mainGlobs.hWnd, SW_SHOW);
	::SetActiveWindow(mainGlobs.hWnd);
}

// <LegoRR.exe @00478490>
bool32 __cdecl Gods98::Main_SetupDirect3D(const DirectDraw_Device* dev, IDirectDraw* ddraw1, IDirectDrawSurface4* backSurf, bool32 doubleBuffered)
{
	log_firstcall();

	
	LPGUID guid = nullptr;
	LPDIRECT3DRM lpD3DRM1;
//	LPDIRECT3D3 d3d3;
//	LPDIRECT3DDEVICE3 d3ddev3;
	HRESULT r;
	LPDIRECTDRAWSURFACE surf1 = nullptr; // dummy init
	
	if (dev) {
		guid = const_cast<GUID*>(&dev->guid);
		if (dev->flags & DirectDraw_DeviceFlags::DIRECTDRAW_FLAG_DEVICE_VIDEOTEXTURE)
			mainGlobs.flags |= MainFlags::MAIN_FLAG_VIDEOTEXTURE;
	}

#pragma message ( "CAPS testing is a complete waste of time for this" )
	if (!(mainGlobs.flags & MainFlags::MAIN_FLAG_FORCEVERTEXFOG) &&
		(dev->flags & DirectDraw_DeviceFlags::DIRECTDRAW_FLAG_DEVICE_HARDWARE))
	{
		Main_SetFogMethod(D3DRMFOGMETHOD_TABLE);
	} else {
		Main_SetFogMethod(D3DRMFOGMETHOD_VERTEX);
	}

	// Create D3DRM and the device...
	
	if (::Direct3DRMCreate(&lpD3DRM1) == D3DRM_OK) {
		/// NOTE: referencing LegoRR's IID_IDirect3DRM3 data (define this ourselves!)
		if (lpD3DRM1->QueryInterface(Idl::IID_IDirect3DRM3, (void**)&mainGlobs.lpD3DRM) == D3DRM_OK){
			backSurf->QueryInterface(IID_IDirectDrawSurface, (void**)&surf1);
			/// NOTE ASM: Decompiled code shows dev->guid,
			///  but this is only because it's the first structure item, and referenced
			///  by address. Meaning a NULL dev is "functionally safe".
			if ((r = mainGlobs.lpD3DRM->CreateDeviceFromSurface(guid, ddraw1, surf1, 0, &mainGlobs.device)) == D3DRM_OK){

				LPDIRECT3DDEVICE2 imdev2;
				mainGlobs.device->GetDirect3DDevice2( &imdev2 );
				imdev2->QueryInterface( IID_IDirect3DDevice3, (void**)&mainGlobs.imDevice );
				imdev2->Release();

				// This is code that was commented out in Gods98, and nothing more.
//			if ((r = mainGlobs.lpD3DRM->CreateDeviceFromSurface(nullptr, ddraw1, surf1, 0, &mainGlobs.device)) == D3DRM_OK){
//			if (D3DRM_OK == ddraw1->lpVtbl->QueryInterface(ddraw1, &IID_IDirect3D3, &d3d3)){
//				r = 1;
//				if (0 == std::memcmp(guid, &IID_IDirect3DRGBDevice, sizeof(GUID))){
//					r = d3d3->CreateDevice(IID_IDirect3DMMXDevice, backSurf, &d3ddev3, nullptr);
//				}
//				if (r != D3DRM_OK) {
//					r = d3d3->CreateDevice(guid, backSurf, &d3ddev3, nullptr);
//				}

//				if (D3DRM_OK == r){

//					LPDIRECT3D2 d3d2;
//					LPDIRECT3DDEVICE2 d3ddev2;
//					d3d3->lpVtbl->QueryInterface(d3d3, &IID_IDirect3D2, &d3d2);
//					d3ddev3->lpVtbl->QueryInterface(d3ddev3, &IID_IDirect3DDevice2, &d3ddev2);
//					mainGlobs.lpD3DRM->lpVtbl->CreateDeviceFromD3D(mainGlobs.lpD3DRM, d3d2, d3ddev2, &mainGlobs.device);
//					d3ddev3->lpVtbl->Release(d3ddev3);
//					d3ddev2->lpVtbl->Release(d3ddev2);
//					d3d2->lpVtbl->Release(d3d2);

//					d3d3->lpVtbl->Release(d3d3);


				if (doubleBuffered) mainGlobs.device->SetBufferCount(2);
				return true;
					
			} else {
				CHKDD(r);
				Error_Warn(true, "Unable to create Device");
			}
//				d3d3->lpVtbl->Release(d3d3);
//			} else Error_Warn(true, "Unable to obtain Direct3D3");
		} else Error_Warn(true, "Failed query for IID_IDirect3DRM3");
	} else Error_Warn(true, "Unable to create lpD3DRM1");

	CHKRM(r);

	return false;
}

// <LegoRR.exe @004785d0>
void __cdecl Gods98::Main_AdjustWindowRect(IN OUT RECT* rect)
{
	log_firstcall();

	if (!(mainGlobs.flags & MainFlags::MAIN_FLAG_FULLSCREEN)) {
		::AdjustWindowRect(rect, mainGlobs.style, false);
	}
}

// <LegoRR.exe @004785f0>
void __cdecl Gods98::Main_Setup3D(MainQuality renderQuality, bool32 dither, bool32 linearFilter, bool32 mipMap,
						  bool32 mipMapLinear, bool32 blendTransparency, bool32 sortTransparency)
{
	D3DRMRENDERQUALITY quality = D3DRMRENDER_WIREFRAME;
	DWORD renderMode = 0;

	if (renderQuality == MainQuality::WIREFRAME)      quality = D3DRMRENDER_WIREFRAME;
	if (renderQuality == MainQuality::FLATSHADE)      quality = D3DRMRENDER_FLAT;
	if (renderQuality == MainQuality::UNLITFLATSHADE) quality = D3DRMRENDER_UNLITFLAT;
	if (renderQuality == MainQuality::GOURAUDSHADE)   quality = D3DRMRENDER_GOURAUD;

	/// NEW GODS98: Not present in LegoRR (these arguments ended up being unused)
	//if (blendTransparency) renderMode |= D3DRMRENDERMODE_BLENDEDTRANSPARENCY;
	//if (sortTransparency)  renderMode |= D3DRMRENDERMODE_SORTEDTRANSPARENCY;

	D3DRMTEXTUREQUALITY textureMode;
	if (linearFilter) {
		if (mipMap) {
			if (mipMapLinear) textureMode = D3DRMTEXTURE_LINEARMIPLINEAR;
			else textureMode = D3DRMTEXTURE_MIPLINEAR;
			mainGlobs.flags |= MainFlags::MAIN_FLAG_MIPMAPENABLED;
		} else textureMode = D3DRMTEXTURE_LINEAR;
	} else textureMode = D3DRMTEXTURE_NEAREST;

	if (mainGlobs.device){

		// Should check device caps here...

		mainGlobs.device->SetDither(dither);
		mainGlobs.device->SetQuality(quality);
		mainGlobs.device->SetTextureQuality(textureMode);
//		mainGlobs.device->SetRenderMode(D3DRMRENDERMODE_BLENDEDTRANSPARENCY|D3DRMRENDERMODE_SORTEDTRANSPARENCY |D3DRMRENDERMODE_DISABLESORTEDALPHAZWRITE);
		/// NEW GODS98: Not present in LegoRR
		//mainGlobs.device->SetRenderMode(renderMode);

	} else Error_Fatal(true, "Device not initialised");
}

// <LegoRR.exe @00478690>
void __cdecl Gods98::Main_SetTitle(const char* title)
{
	log_firstcall();

	::SetWindowTextA(mainGlobs.hWnd, title);
}


// <missing>
uint32 __cdecl Gods98::Main_GetTrianglesDrawn(bool32 total)
{
	static uint32 last = 0;

	uint32 curr = mainGlobs.device->GetTrianglesDrawn();

	uint32 count;
	if (!total) count = curr - last;
	else count = curr;

	last = curr;
	return count;
}


// <LegoRR.exe @004786b0>
bool32 __cdecl Gods98::Main_InitApp(HINSTANCE hInstance)
{
	log_firstcall();

	WNDCLASSA wc = { 0 }; // dummy init
	//	DWORD WIN_STYLE;
	
	// Register the window style
	wc.style = CS_DBLCLKS;
	wc.lpfnWndProc = Main_WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	/// CHANGE: Load small icon into main window title bar
	wc.hIcon = ::LoadIconA(hInstance, MAKEINTRESOURCEA(113));
	//wc.hIcon = nullptr;
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

	return 0L; // default for all top-level switch breaks
}

// <LegoRR.exe @00478b40>
LRESULT __stdcall Gods98::Main_WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	log_firstcall();
	
	/// APPLY FIX: Show cursor in when hovering over title bar
	/// <https://stackoverflow.com/a/14134212/7517185>
    if (message == WM_SETCURSOR)
    {
		// are we inside the client area? If so, erase the cursor like normal
        if (LOWORD(lParam) == HTCLIENT)
        {
            ::SetCursor(nullptr);
            return (LRESULT)true;
        }

		// Otherwise, show the cursor again
        return ::DefWindowProcA(hWnd, message, wParam, lParam);
    }

	if (mainGlobs.flags & MainFlags::MAIN_FLAG_FULLSCREEN)
	{
		return Main_WndProc_Fullscreen(hWnd, message, wParam, lParam);
	}
	else
	{
		return Main_WndProc_Windowed(hWnd, message, wParam, lParam);
	}
}

// <LegoRR.exe @00478b90>
void __cdecl Gods98::Main_ChangeRenderState(D3DRENDERSTATETYPE dwRenderStateType, DWORD dwRenderState)
{
	log_firstcall();

	Error_Fatal(dwRenderStateType >= MAIN_MAXRENDERSTATES, "RenderState type is out of range");

	DWORD currValue;
	Main_StateChangeData* data = &mainGlobs.renderStateData[dwRenderStateType];
	HRESULT r = mainGlobs.imDevice->GetRenderState(dwRenderStateType, &currValue);
	Error_Fatal(r != D3D_OK, Error_Format("Failed to GetRenderState(%i)", dwRenderStateType));

	if (currValue != dwRenderState) {

//		if( D3D_OK ==
		mainGlobs.imDevice->SetRenderState(dwRenderStateType, dwRenderState);
			// )
		{
			if (data->changed) {
				if (data->origValue == currValue) data->changed = false;
			} else {
				data->origValue = currValue;
				data->changed = true;
			}

		}
	}
}

/// NOTE: newer engine version has argument: BOOL force, but LegoRR DOES NOT have this argument.
///        (it's possible this argument has been inlined, as it negates calling the entire function body.)
// <LegoRR.exe @00478c00>
void __cdecl Gods98::Main_RestoreStates(void)
{
	log_firstcall();

	// NEW GODS98: BOOL force argument
	//if (force) {
	for (uint32 loop = 0; loop < MAIN_MAXRENDERSTATES; loop++) {
		Main_StateChangeData* data = &mainGlobs.renderStateData[loop];
		if (data->changed) {
			mainGlobs.imDevice->SetRenderState((D3DRENDERSTATETYPE)loop, data->origValue);
			data->changed = false;
		}
	}
	//}
}


// <missing>
void __cdecl Gods98::Main_PauseApp(bool32 pause)
{
	if (pause)
		mainGlobs.flags |= MainFlags::MAIN_FLAG_PAUSED;
	else
		mainGlobs.flags &= ~MainFlags::MAIN_FLAG_PAUSED;

}

// <missing>
bool32 __cdecl Gods98::Main_IsPaused(void)
{
	return (mainGlobs.flags & MainFlags::MAIN_FLAG_PAUSED);
}


// <LegoRR.exe @00478c40>
//bool32 __cdecl Gods98::noinline(Main_SetCDVolume)(real32 leftVolume, real32 rightVolume)
bool32 __cdecl Gods98::Main_SetCDVolume(real32 leftVolume, real32 rightVolume)
{
	log_firstcall();

	return Main_CDVolume(&leftVolume, &rightVolume, true);
}

// <LegoRR.exe @00478c60>
//bool32 __cdecl Gods98::noinline(Main_GetCDVolume)(OUT real32* leftVolume, OUT real32* rightVolume)
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
	uint32 deviceCount = ::mixerGetNumDevs();
	MIXERCAPS caps;
	MIXERLINE mixerLine;
	uint32 cConnections;
	uint32 uwVolumeL, uwVolumeR;
	HMIXER hMixer;
	bool32 result = false;

	if (set) {
		if (*leftVolume > 1.0f) *leftVolume = 1.0f;
		if (*leftVolume < 0.0f) *leftVolume = 0.0f;
		if (*rightVolume > 1.0f) *rightVolume = 1.0f;
		if (*rightVolume < 0.0f) *rightVolume = 0.0f;
	}

	for (uint32 loop=0 ; loop<deviceCount ; loop++) {

		if (::mixerOpen(&hMixer, loop, 0, 0, MIXER_OBJECTF_MIXER) == MMSYSERR_NOERROR) {
			HMIXEROBJ mixer = (HMIXEROBJ)hMixer; // second variable created to avoid extra casting

			std::memset(&caps, 0, sizeof(caps));
			::mixerGetDevCapsA(loop, &caps, sizeof(caps));

			for (uint32 sub=0 ; sub<caps.cDestinations ; sub++) {

				std::memset(&mixerLine, 0, sizeof(mixerLine));
				mixerLine.cbStruct = sizeof(mixerLine);
				mixerLine.dwDestination = sub;

				if (::mixerGetLineInfoA(mixer, &mixerLine, MIXER_GETLINEINFOF_DESTINATION) == MMSYSERR_NOERROR) {

					cConnections = mixerLine.cConnections;

					for (uint32 conn=0 ; conn<cConnections ; conn++) {

						mixerLine.cbStruct = sizeof(mixerLine);
						mixerLine.dwDestination = sub;
						mixerLine.dwSource = conn;

						if (::mixerGetLineInfoA(mixer, &mixerLine, MIXER_GETLINEINFOF_SOURCE) == MMSYSERR_NOERROR) {
							if (mixerLine.dwComponentType == MIXERLINE_COMPONENTTYPE_SRC_COMPACTDISC) {

								MIXERLINECONTROLS controls;
								LPMIXERCONTROL controlList;

								std::memset(&controls, 0, sizeof(controls));
								controls.cbStruct = sizeof(controls);
								controls.dwLineID = mixerLine.dwLineID;
								controls.cControls = mixerLine.cControls;
								controls.cbmxctrl = sizeof(*controlList);

								controlList = (LPMIXERCONTROL)Mem_Alloc(sizeof(*controlList) * controls.cControls);
								controls.pamxctrl = controlList;

								if (::mixerGetLineControlsA(mixer, &controls, MIXER_GETLINECONTROLSF_ALL) == MMSYSERR_NOERROR) {
									for (uint32 controlLoop=0 ; controlLoop<mixerLine.cControls ; controlLoop++) {
										if ((controlList[controlLoop].dwControlType & MIXERCONTROL_CT_CLASS_FADER) &&
											(controlList[controlLoop].dwControlType & MIXERCONTROL_CONTROLTYPE_VOLUME)) {

											LPMIXERCONTROLDETAILS_UNSIGNED valueList;

											MIXERCONTROLDETAILS details;
											std::memset(&details, 0, sizeof(details));
											details.cbStruct = sizeof(details);
											details.dwControlID = controlList[controlLoop].dwControlID;
											details.cbDetails = sizeof(*valueList);
											details.cChannels = mixerLine.cChannels;
											details.cMultipleItems = 0;

											valueList = (LPMIXERCONTROLDETAILS_UNSIGNED)Mem_Alloc(sizeof(*valueList) * mixerLine.cChannels);
											details.paDetails = valueList;

											if (set) {
												uwVolumeL = controlList[controlLoop].Bounds.dwMinimum + (uint32) (*leftVolume * (controlList[controlLoop].Bounds.dwMaximum - controlList[controlLoop].Bounds.dwMinimum));
												uwVolumeR = controlList[controlLoop].Bounds.dwMinimum + (uint32) (*rightVolume * (controlList[controlLoop].Bounds.dwMaximum - controlList[controlLoop].Bounds.dwMinimum));
												if (mixerLine.cChannels == 2) {
													valueList[0].dwValue = uwVolumeL;
													valueList[1].dwValue = uwVolumeR;
												} else {
													for (uint32 vLoop=0 ; vLoop<mixerLine.cChannels ; vLoop++) valueList[vLoop].dwValue = (uwVolumeL + uwVolumeR) / 2;
												}
												if (::mixerSetControlDetails(mixer, &details, MIXER_GETCONTROLDETAILSF_VALUE) == MMSYSERR_NOERROR) {
													result = true;
												}
											} else {
												if (::mixerGetControlDetailsA(mixer, &details, MIXER_GETCONTROLDETAILSF_VALUE) == MMSYSERR_NOERROR) {

													*leftVolume = (real32) (valueList[0].dwValue - controlList[controlLoop].Bounds.dwMinimum) / (controlList[controlLoop].Bounds.dwMaximum - controlList[controlLoop].Bounds.dwMinimum);
													if (mixerLine.cChannels == 2) {
														*rightVolume = (real32) (valueList[1].dwValue - controlList[controlLoop].Bounds.dwMinimum) / (controlList[controlLoop].Bounds.dwMaximum - controlList[controlLoop].Bounds.dwMinimum);
													} else {
														*rightVolume = *leftVolume;
													}
													result = true;
												}
											}

											Mem_Free(valueList);
										}
									}
								}
								Mem_Free(controlList);
							}
						}
					}
				}
			}

			::mixerClose(hMixer);
		}
	}

	return result;
}


// <missing>
void __cdecl Gods98::Main_SetWindowCallback(MainWindowCallback callback)
{
	mainGlobs.windowCallback = callback;
}

// <missing>
void __cdecl Gods98::Main_SetAccel(HACCEL accels)
{
	mainGlobs.accels = accels;
}

#pragma endregion
