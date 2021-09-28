// platform/windows.cpp : 
//

#include "windows.h"


// Enable visual styles
//#pragma comment(linker, "\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")


#pragma region Library Comments

// Direct X Header Files and Library
#pragma comment(lib, "ddraw.lib")		// DirectDraw
#pragma comment(lib, "dsound.lib")		// DirectSound
#pragma comment(lib, "dinput8.lib")		// DirectInput 8.0
#pragma comment(lib, "winmm.lib")		// Multimedia System and timeGetTime()
#pragma comment(lib, "dxguid.lib")		// GUID and IID constants

// Reintroduced legacy support:
//#pragma comment(lib, "d3drm.lib")		// Direct3DRM    (included in linker settings)
//#pragma comment(lib, "d3d8.lib")		// Direct3D 8.0

#pragma comment(lib, "vfw32.lib")		// AVIFile

// Windows Process and memory management (for inspecting runnings instance of LegoRR.exe)
//#pragma comment(lib, "psapi.lib")		// EnumProcesses() + OpenProcess()
// 
// Used by Movie.cpp
// <https://docs.microsoft.com/en-us/previous-versions/windows/embedded/aa451220(v=msdn.10)>
#pragma comment(lib, "Strmbase.lib")		// IAMMultiMediaStream interfaces
#pragma comment(lib, "Strmiids.lib")		// IAMMultiMediaStream IID constants

#pragma endregion
