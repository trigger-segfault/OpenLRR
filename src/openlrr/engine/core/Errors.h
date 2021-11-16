// Errors.h : 
//

#pragma once

#include "../../common.h"


namespace Gods98
{; // !<---

/**********************************************************************************
 ******** Forward Declarations
 **********************************************************************************/

#pragma region Forward Declarations

struct File;

#pragma endregion

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

enum class Error_LoadError : sint32
{
	InvalidFName,
	UnableToOpen,
	UnableToOpenForWrite,
	UnableToVerifyName,
	RMTexture,

	Count,
};
assert_sizeof(Error_LoadError, 0x4);

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

struct Error_Globs
{
	// [globs: start]
	/*000,4*/ File* dumpFile;
	/*004,4*/ File* loadLogFile;
	/*008,4*/ File* loadErrorLogFile;
	/*00c,4*/ File* redundantLogFile;
	/*010,400*/ char loadLogName[1024];
	/*410,400*/ char redundantLogName[1024];
	/*810,4*/ bool32 warnCalled;
	/*814,4*/ bool32 fullScreen;
	// [globs: end]
	/*818*/
};
assert_sizeof(Error_Globs, 0x818);

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

#define Error__FILE__						Gods98::Error_RelativeFile(__FILE__)


#define Error_Debug(s)

#define Error_Warn(b,s)						{ if (b) { Gods98::Error_Out(false, "%s(%i): Warning: %s\n", Error__FILE__, __LINE__, (s)); Gods98::Error_SetWarn(); } }
#define Error_Fatal(b,s)					{ if (b) { Gods98::Error_Out(true, "%s(%i): Fatal: %s\n", Error__FILE__, __LINE__, (s)); } }
//#define Error_Debug(s)						Gods98::Error_Out(false, "%s", (s))
#define Error_LogLoad(b,s)					{ Gods98::Error_Log( Gods98::errorGlobs.loadLogFile, (b), "%s\n", (s) ); }
#define Error_LogLoadError(b,s)				{ Gods98::Error_Log( Gods98::errorGlobs.loadErrorLogFile, (b), "%s\n", (s) ); }
#define Error_LogRedundantFile(b,s)			{ Gods98::Error_Log( Gods98::errorGlobs.redundantLogFile, (b), "%s\n", (s) ); }

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
void __cdecl Error_Log(File* logFile, bool32 log, const char* lpOutputString, ...);


__inline void Error_SetWarn(void) { errorGlobs.warnCalled = true; }
__inline void Error_CheckWarn(bool32 check) { if (!check) errorGlobs.warnCalled = false; else if (errorGlobs.warnCalled) Error_TerminateProgram("Check warning message log"); }


/// CUSTOM: An alternative to __FILE__ macro that cuts off filepaths at the "\src"
const char* Error_RelativeFile(const char* fname);

#pragma endregion

}
