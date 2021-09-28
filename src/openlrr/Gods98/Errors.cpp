// Errors.cpp : 
//

#include "../platform/dinput.h"
#include <process.h>

#include "Config.h"
#include "Files.h"

#include "Errors.h"


/// CUSTOM:
#define LOG_CONSOLEWINDOW


/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @00576ce0>
Gods98::Error_Globs & Gods98::errorGlobs = *(Gods98::Error_Globs*)0x00576ce0;

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @0048b520>
void __cdecl Gods98::Error_Initialise(void)
{
	log_firstcall();

	errorGlobs.dumpFile = nullptr;
	errorGlobs.loadLogFile = nullptr;
	errorGlobs.loadErrorLogFile = nullptr;
	errorGlobs.redundantLogFile = nullptr;

	errorGlobs.fullScreen = false;
}

// <LegoRR.exe @0048b540>
void __cdecl Gods98::Error_FullScreen(bool32 on)
{
	log_firstcall();

	errorGlobs.fullScreen = on;
}

// <LegoRR.exe @0048b550>
void __cdecl Gods98::Error_CloseLog(void)
{
	log_firstcall();

	if (errorGlobs.dumpFile) File_Close(errorGlobs.dumpFile);
	if (errorGlobs.loadLogFile) File_Close(errorGlobs.loadLogFile);
	if (errorGlobs.loadErrorLogFile) File_Close(errorGlobs.loadErrorLogFile);

	errorGlobs.dumpFile = errorGlobs.loadLogFile = errorGlobs.loadErrorLogFile = nullptr;
}

// <LegoRR.exe @0048b5b0>
void __cdecl Gods98::Error_Shutdown(void)
{
	log_firstcall();

	Error_CloseLog();

	if (errorGlobs.redundantLogFile = File_Open(errorGlobs.redundantLogName, "w"))
	{
		File_CheckRedundantFiles(errorGlobs.loadLogName);

		File_Close(errorGlobs.redundantLogFile);
	}
}

// <missing>
bool32 __cdecl Gods98::Error_SetDumpFile(const char* errors, const char* loadLog, const char* loadErrorLog, const char* rendundantLog)
{
	bool32 ok = true;


	if (loadLog)
		if (!(errorGlobs.loadLogFile = File_Open(loadLog, "w")))
			ok = false;
		else
			std::strcpy(errorGlobs.loadLogName, loadLog);

	if (loadErrorLog)
		if (!(errorGlobs.loadErrorLogFile = File_Open(loadErrorLog, "w")))
		{
			ok = false;
		}
		else
			Error_LogLoadError(true, "<ErrorNum>[0=InvalidFName,1=UnableToOpen,2=UnableToOpenForWrite,3=UnableToVerifyName4=TextureError] <File>");

	if (errors)
		if (!(errorGlobs.dumpFile = File_Open(errors, "w")))
			ok = false;

	if (rendundantLog)
		std::strcpy(errorGlobs.redundantLogName, rendundantLog);

	return ok;
}

// <missing>
void __cdecl Gods98::Error_DebugBreak(void)
{
	if (::MessageBoxA(nullptr, "Error_DebugBreak() called\nCrash to debugger?", "Debugger", MB_OKCANCEL) == IDOK) {
		::DebugBreak();
	}
}

// <missing>
void __cdecl Gods98::Error_TerminateProgram(const char* msg)
{
	// Call exit 0.  Should probably terminate properly and clean everything up.
//#ifndef _RELEASE
	//if (!DirectDraw_FullScreen()) {
	if (!errorGlobs.fullScreen) {

		char message[1024];
		std::sprintf(message, "Error:\n%s\nLast entry read from config: \"%s\"\n\nSelect Retry to crash to debugger", msg, Config_Debug_GetLastFind());

		::ShowCursor(true);

		int r = ::MessageBoxA(nullptr, message, "Debugger", MB_ABORTRETRYIGNORE);
		if (r == IDRETRY) ::DebugBreak();
		else if (r == IDIGNORE) return;
	}
//#endif // !_RELEASE

	Error_Shutdown();
	std::exit(0);
}

// <missing>
const char* __cdecl Gods98::Error_Format(const char* msg, ...)
{
	static char res[1024];
	std::va_list args;

	va_start(args, msg);
	std::vsprintf(res, msg, args);
	va_end(args);

	return res;
}

// <missing>
void __cdecl Gods98::Error_Out(bool32 ErrFatal, const char* lpOutputString, ...)
{
	if (errorGlobs.dumpFile) {

		std::va_list args;
		char msg[512];
		va_start(args, lpOutputString);
		File_VPrintF(errorGlobs.dumpFile, lpOutputString, args);
		std::vsprintf(msg, lpOutputString, args);
		va_end(args);
		File_Flush(errorGlobs.dumpFile);
		if (ErrFatal) Error_TerminateProgram(msg);

	}
	else {


		HANDLE heventDBWIN;  /* DBWIN32 synchronization object */
		HANDLE heventData;   /* data passing synch object */
		HANDLE hSharedFile;  /* memory mapped file shared data */
		LPSTR lpszSharedMem;
		char achBuffer[500];

		/* create the output buffer */
		std::va_list args;
		va_start(args, lpOutputString);
		std::vsprintf(achBuffer, lpOutputString, args);
		va_end(args);

		/*
		Do a regular OutputDebugString so that the output is
		still seen in the debugger window if it exists.

		  This ifdef is necessary to avoid infinite recursion
		  from the inclusion of W95TRACE.H
		*/
#ifdef LOG_CONSOLEWINDOW
#ifdef _UNICODE
		std::wprintf(achBuffer);
#else
		std::printf(achBuffer);
#endif
#else
#ifdef _cplusplus
#ifdef _UNICODE
		::OutputDebugStringW(achBuffer);
#else
		::OutputDebugStringA(achBuffer);
#endif
#else
#ifdef _UNICODE
		::OutputDebugStringW(achBuffer);
#else
		::OutputDebugStringA(achBuffer);
#endif
#endif
#endif

		/// CHANGE: GetVersionEx is deprecated, and it's not worthing replacing this.
		/* bail if it's not Win95 */
		{
			/*//OSVERSIONINFOEX VerInfoEx = { 0 };
			//VerInfoEx.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
			OSVERSIONINFO VerInfo;
			VerInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
			::VerifyVersionInfoA(&VerInfoEx, 0, 0);
			::GetVersionExA(&VerInfo);
			if (VerInfo.dwPlatformId != VER_PLATFORM_WIN32_WINDOWS)*/
			{
				/// TEST: try removing this
				//if (ErrFatal) Error_TerminateProgram(achBuffer);
				//return;
			}
		}

		/* make sure DBWIN is open and waiting */
		heventDBWIN = ::OpenEvent(EVENT_MODIFY_STATE, FALSE, "DBWIN_BUFFER_READY");
		if (!heventDBWIN)
		{
			//MessageBox(NULL, "DBWIN_BUFFER_READY nonexistent", NULL, MB_OK);
			if (ErrFatal) Error_TerminateProgram(achBuffer);
			return;
		}

		/* get a handle to the data synch object */
		heventData = ::OpenEvent(EVENT_MODIFY_STATE, FALSE, "DBWIN_DATA_READY");
		if (!heventData)
		{
			// MessageBox(NULL, "DBWIN_DATA_READY nonexistent", NULL, MB_OK);
			::CloseHandle(heventDBWIN);
			if (ErrFatal) Error_TerminateProgram(achBuffer);
			return;
		}

		hSharedFile = ::CreateFileMapping((HANDLE)-1, nullptr, PAGE_READWRITE, 0, 4096, "DBWIN_BUFFER");
		if (!hSharedFile)
		{
			//MessageBox(NULL, "DebugTrace: Unable to create file mapping object DBWIN_BUFFER", "Error", MB_OK);
			::CloseHandle(heventDBWIN);
			::CloseHandle(heventData);
			if (ErrFatal) Error_TerminateProgram(achBuffer);
			return;
		}

		lpszSharedMem = (LPSTR)::MapViewOfFile(hSharedFile, FILE_MAP_WRITE, 0, 0, 512);
		if (!lpszSharedMem)
		{
			//MessageBox(NULL, "DebugTrace: Unable to map shared memory", "Error", MB_OK);
			::CloseHandle(heventDBWIN);
			::CloseHandle(heventData);
			if (ErrFatal) Error_TerminateProgram(achBuffer);
			return;
		}

		/* wait for buffer event */
		::WaitForSingleObject(heventDBWIN, INFINITE);

		/* write it to the shared memory */
		*((LPDWORD)lpszSharedMem) = ::_getpid();
		::wsprintf(lpszSharedMem + sizeof(DWORD), "%s", achBuffer);

		/* signal data ready event */
		::SetEvent(heventData);

		/* clean up handles */
		::CloseHandle(hSharedFile);
		::CloseHandle(heventData);
		::CloseHandle(heventDBWIN);

		if (ErrFatal) Error_TerminateProgram(achBuffer);
	}
}

// <missing>
void __cdecl Gods98::Error_Log(File* logFile, bool32 log, const char* lpOutputString, ...)
{
	if (log && logFile) {
		std::va_list args;
		char msg[512];

		va_start(args, lpOutputString);
		File_VPrintF(logFile, lpOutputString, args);
		std::vsprintf(msg, lpOutputString, args);
		va_end(args);
		File_Flush(logFile);

	}
}

#pragma endregion
