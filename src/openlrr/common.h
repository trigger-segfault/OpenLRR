// common.h : include file for standard system include files,
//            or project specific include files.
//

#pragma once

//#include "targetver.h"


// Header File Preprocessors:
#pragma region Include Defines
// suppress C4996 (moved to preprocessor)
//#define _CRT_SECURE_NO_WARNINGS

// Include constants such as M_PI
#define _USE_MATH_DEFINES
// Exclude rarely-used stuff from Windows headers
#define WIN32_LEAN_AND_MEAN
// dinput.h version. NOTE: Normally this is 0x500 (v5) in LegoRR,
//  but we don't need to try to support that.
//#define DIRECTINPUT_VERSION				0x800
#define DIRECTINPUT_VERSION				0x500
#pragma endregion

// Header File Includes:
#pragma region Windows Header File
// Windows Header Files
#include <windows.h>
#pragma endregion

#pragma region C/C++ RunTime Header Files
// C++ RunTime Header Files
#include <cmath>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <cassert>
#include <ctime>
//#include <typeinfo>
// C++ STL Header Files
#include <string>
#include <array>
#include <vector>
#include <map>
#include <functional>
#include <type_traits>

// Windows C RunTime Header Files
#include <direct.h>
#include <io.h>
#pragma endregion

#pragma region API Header Files
// Direct X Header Files and Library
#include <mmsystem.h>					// Multimedia System
#include <d3d.h>						// Direct3D
#include <ddraw.h>						// DirectDraw
#include <dsound.h>						// DirectSound
#include <dinput.h>						// DirectInput 8.0
// Reintroduced legacy support:

//////////////////////
// TODO: re-add me
//#include <d3drmwin.h>                         // now available as additional include
////#include "../DirectX/D3DRM/d3drmwin.h"		// Direct3DRM
//////////////////////

#include <vfw.h>						// AVIFile
#include <timeapi.h>					// timeGetTime()

// Windows Process and memory management (for inspecting runnings instance of LegoRR.exe)
#include <memoryapi.h>					// ReadProcessMemory()
#include <psapi.h>						// EnumProcesses() + OpenProcess()
#pragma endregion

// Library Comments for Header Files:
#pragma region Library Comments
/*// Direct X Header Files and Library
#pragma comment(lib, "ddraw.lib")		// DirectDraw
#pragma comment(lib, "dsound.lib")		// DirectSound
#pragma comment(lib, "dinput8.lib")		// DirectInput 8.0
#pragma comment(lib, "winmm.lib")		// Multimedia System and timeGetTime()
#pragma comment(lib, "dxguid.lib")		// GUID + IID constants

// Reintroduced legacy support:
//#pragma comment(lib, "d3drm.lib")		// Direct3DRM
//#pragma comment(lib, "d3d8.lib")		// Direct3D 8.0

#pragma comment(lib, "vfw32.lib")		// AVIFile

// Windows Process and memory management (for inspecting runnings instance of LegoRR.exe)
#pragma comment(lib, "psapi.lib")		// EnumProcesses() + OpenProcess()*/
#pragma endregion
#pragma comment(lib, "winmm.lib")		// Multimedia System and timeGetTime()


// Project-global Defines:
// Most of these are from older projects, and will be refactored-away later
#pragma region Global Configuration Settings

// Always show cursor? (default is false)
#define SHOWCURSOR true
// Default to windowed mode when opening the Mode Selection Dialog (default is false)
#define DEFAULTWINDOWED true

// On startup, the program will only run if another instance is not already running. (default is true)
#define USEMUTEX true
// Name used for mutex that should be exclusive to this program (name will have " Mutex" appended).
#define MUTEX_NAME "OpenLRR"
// Original mutex name used by LEGO Rock Raiders.
//#define MUTEX_NAME "Lego Rock Raiders"

// Don't read standard command line parameters into our arguments
#define IGNORESTANDARDPARAMETERS true

#pragma endregion

// Project-debug Defines:
#pragma region Debug Configuration Settings

// Removes sections of code that initialize unused features
#define DEBUG_REMOVEUNUSED
// Set this here to stop CD-ROM drive from spinning up all the time.
#define DEBUG_IGNORECD
// Include 32-bit registered screen mode
#define DEBUG_INCLUDE32BIT
// Ignore whether our desired screen mode is registerd by LRR (such as ignoring bit depth)
//#define DEBUG_IGNOREREGISTERED

#pragma endregion

#define STANDARD_FRAMERATE		25.0f



typedef char char8;
typedef signed char schar8;
typedef unsigned char uchar8;


typedef signed char sint8;
typedef unsigned char uint8;
typedef short sint16;
typedef unsigned short uint16;
typedef int sint32;
typedef unsigned int uint32;
typedef long long sint64;
typedef unsigned long long uint64;

typedef float real32;

typedef int bool32;

typedef unsigned int colour32;

enum BoolTri : bool32
{
	BOOL3_FALSE = 0,
	BOOL3_TRUE  = 1,
	BOOL3_ERROR = 2, // would `BOOL3_ERR` be better?
};
static_assert(sizeof(BoolTri) == 0x4, "");


#define log_firstcall() { static bool _log_firstcallbool = false; \
	if (!_log_firstcallbool) {_log_firstcallbool = true; \
		std::printf("%s called\n", __FUNCTION__); } }
