#pragma once

#include "windows.h"
#include <dsound.h>


namespace legacy
{; // !<---

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @0049ca6e>
__inline HRESULT DirectSoundCreate(_In_opt_ LPCGUID pcGuidDevice, _Outptr_ LPDIRECTSOUND* ppDS, _Pre_null_ LPUNKNOWN pUnkOuter)
{
	return ((HRESULT(__stdcall*)(LPCGUID, LPDIRECTSOUND*,LPUNKNOWN))0x0049ca6e)(pcGuidDevice, ppDS, pUnkOuter);
}

#pragma endregion

}
