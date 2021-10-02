// platform/windows.h : 
//

#pragma once

#include "targetver.h"

// dinput.h version. NOTE: Normally this is 0x500 (v5) in LegoRR,
//  but we don't need to try to support that.
//#define DIRECTINPUT_VERSION				0x0800
//#define DIRECTINPUT_VERSION				0x0500


#pragma warning (push)
#pragma warning (disable : 4005)

//#define _USE_MATH_DEFINES		// Include constants such as M_PI

//#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>

#pragma warning (pop)


#pragma region API Includes

// Direct X Header Files and Library
//#include <mmsystem.h>					// Multimedia System
//#include <d3d.h>						// Direct3D
//#include <ddraw.h>						// DirectDraw
//#include <dsound.h>						// DirectSound
//#include <dinput.h>						// DirectInput 8.0
// Reintroduced legacy support:

//////////////////////
// TODO: re-add me
//#include <d3drmwin.h>                         // now available as additional include
//////////////////////

//#include <vfw.h>						// AVIFile
//#include <timeapi.h>					// timeGetTime()

// Windows Process and memory management (for inspecting runnings instance of LegoRR.exe)
//#include <memoryapi.h>					// ReadProcessMemory()
//#include <psapi.h>						// EnumProcesses() + OpenProcess()

#pragma endregion
