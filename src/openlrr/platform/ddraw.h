#pragma once

#include "windows.h"
#include <ddraw.h>


namespace legacy
{; // !<---

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @0049b982>
__inline HRESULT DirectDrawEnumerateA(LPDDENUMCALLBACKA lpCallback, LPVOID lpContext)
{
	return ((HRESULT(__stdcall*)(LPDDENUMCALLBACKA, LPVOID))0x0049b982)(lpCallback, lpContext);
}

// <LegoRR.exe @0049b988>
__inline HRESULT DirectDrawCreate(LPGUID lpGUID, IDirectDraw** lplpIDirectDraw, LPUNKNOWN pUnkOuter)
{
	return ((HRESULT(__stdcall*)(LPGUID, IDirectDraw**, LPUNKNOWN))0x0049b988)(lpGUID, lplpIDirectDraw, pUnkOuter);
}

#pragma endregion

}
