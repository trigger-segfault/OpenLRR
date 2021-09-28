#pragma once

#include "windows.h"

#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0500
#elif DIRECTINPUT_VERSION != 0x0500
#undef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0500
#endif

#include <dinput.h>


namespace legacy
{; // !<---

/**********************************************************************************
 ******** Constants
 **********************************************************************************/

#pragma region Constants

#define LEGACY_DIRECTINPUT_VERSION 0x0500

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @0049ca68>
__inline HRESULT DirectInputCreateA(HINSTANCE hinst, DWORD dwVersion, LPDIRECTINPUTA* ppDI, LPUNKNOWN punkOuter)
{
	return ((HRESULT(__stdcall*)(HINSTANCE, DWORD, LPDIRECTINPUTA*, LPUNKNOWN))0x0049ca68)(hinst, dwVersion, ppDI, punkOuter);
}

#pragma endregion

}
