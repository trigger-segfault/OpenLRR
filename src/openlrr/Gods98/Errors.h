#pragma once

#include "../common.h"
#include "Files.h"


namespace Gods98
{; // !<---

/**********************************************************************************
 ******** Constants
 **********************************************************************************/

#pragma region Constants

#define ERROR_LOADLOG			"Errors\\loadLog.dmp"
#define ERROR_LOADERRORLOG		"Errors\\loadErrorLog.dmp"
#define ERROR_REDUNDANTLOG		"Errors\\redundantFiles.dmp"

#pragma endregion

/**********************************************************************************
 ******** Enumerations
 **********************************************************************************/

#pragma region Enums

enum Error_LoadError : sint32
{
	Error_LoadError_InvalidFName,
	Error_LoadError_UnableToOpen,
	Error_LoadError_UnableToOpenForWrite,
	Error_LoadError_UnableToVerifyName,
	Error_LoadError_RMTexture,

	Error_LoadError_Count
};
static_assert(sizeof(Error_LoadError) == 0x4, "");

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

struct Error_Globs
{
	// [globs: start]
	/*000,4*/ File_Dummy* dumpFile;
	/*004,4*/ File_Dummy* loadLogFile;
	/*008,4*/ File_Dummy* loadErrorLogFile;
	/*00c,4*/ File_Dummy* redundantLogFile;
	/*010,400*/ char loadLogName[1024];
	/*410,400*/ char redundantLogName[1024];
	/*810,4*/ bool32 warnCalled;
	/*814,4*/ bool32 fullScreen;
	// [globs: end]
	/*818*/
};
static_assert(sizeof(Error_Globs) == 0x818, "");

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @00576ce0>
extern Error_Globs & errorGlobs;

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

//#ifndef _RELEASE

#define Error_Warn(b,s)						{ if (b) { Error_Out(false, "%s(%i): Warning: %s\n", __FILE__, __LINE__, (s)); Error_SetWarn(); } }
#define Error_Fatal(b,s)					{ if (b) { Error_Out(true, "%s(%i): Fatal: %s\n", __FILE__, __LINE__, (s)); } }
#define Error_Debug(s)						Error_Out(false, "%s", (s))
#define Error_LogLoad(b,s)					{ Error_Log( errorGlobs.loadLogFile, (b), "%s\n", (s) ); }
#define Error_LogLoadError(b,s)				{ Error_Log( errorGlobs.loadErrorLogFile, (b), "%s\n", (s) ); }
#define Error_LogRedundantFile(b,s)			{ Error_Log( errorGlobs.redundantLogFile, (b), "%s\n", (s) ); }

/*#else

#define Error_Warn(b,s)			(b)
#define Error_Fatal(b,s)		(b)
#define Error_Debug(s)
__inline void Error_CheckWarn(bool32 check) {  }
#define Error_LogLoad(b,s)					(b)
#define Error_LogLoadError(b,s)				(b)
#define Error_LogRedundantFile(b,s)			(b)

#endif*/

#pragma region Functions

// <LegoRR.exe @0048b520>
void __cdecl Error_Initialise(void);

// <LegoRR.exe @0048b540>
void __cdecl Error_FullScreen(bool32 on);

// <LegoRR.exe @0048b550>
void __cdecl Error_CloseLog(void);

// <LegoRR.exe @0048b5b0>
void __cdecl Error_Shutdown(void);

// <missing>
bool32 __cdecl Error_SetDumpFile(const char* errors, const char* loadLog, const char* loadErrorLog, const char* rendundantLog);

// <missing>
void __cdecl Error_DebugBreak(void);

// <missing>
void __cdecl Error_TerminateProgram(const char* msg);

// <missing>
const char* __cdecl Error_Format(const char* msg, ...);

// <missing>
void __cdecl Error_Out(bool32 ErrFatal, const char* lpOutputString, ...);

// <missing>
void __cdecl Error_Log(File_Dummy* logFile, bool32 log, const char* lpOutputString, ...);


__inline void Error_SetWarn(void) { errorGlobs.warnCalled = true; }
__inline void Error_CheckWarn(bool32 check) { if (!check) errorGlobs.warnCalled = false; else if (errorGlobs.warnCalled) Error_TerminateProgram("Check warning message log"); }

#pragma endregion

}
