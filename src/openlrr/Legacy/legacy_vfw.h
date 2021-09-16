#pragma once

#include "../common.h"
#include <vfw.h>						// AVIFile


namespace legacy
{; // !<---

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @0049b98e>
__inline HRESULT AVIStreamInfoA(_In_ PAVISTREAM pavi, _Out_writes_bytes_(lSize) LPAVISTREAMINFOA psi, _In_ LONG lSize)
{
	return ((HRESULT(__stdcall*)(PAVISTREAM, LPAVISTREAMINFOA, LONG))0x0049b98e)(pavi, psi, lSize);
}

// <LegoRR.exe @0049b994>
__inline PGETFRAME AVIStreamGetFrameOpen(_In_ PAVISTREAM pavi, _In_opt_ LPBITMAPINFOHEADER lpbiWanted)
{
	return ((PGETFRAME(__stdcall*)(PAVISTREAM, LPBITMAPINFOHEADER))0x0049b994)(pavi, lpbiWanted);
}

// <LegoRR.exe @0049b99a>
__inline HRESULT AVIStreamOpenFromFileA(_Outptr_ PAVISTREAM FAR* ppavi, _In_ LPCSTR szFile, _In_ DWORD fccType, _In_ LONG lParam, _In_ UINT mode, _In_opt_ CLSID FAR* pclsidHandler)
{
	return ((HRESULT(__stdcall*)(PAVISTREAM*, LPCSTR, DWORD, LONG, UINT, CLSID*))0x0049b99a)(ppavi, szFile, fccType, lParam, mode, pclsidHandler);
}

// <LegoRR.exe @0049b9a0>
__inline ULONG AVIStreamRelease(PAVISTREAM pavi)
{
	return ((ULONG(__stdcall*)(PAVISTREAM))0x0049b9a0)(pavi);
}

// <LegoRR.exe @0049b9a6>
__inline LPVOID AVIStreamGetFrame(_In_ PGETFRAME pg, _In_ LONG lPos)
{
	return ((LPVOID(__stdcall*)(PGETFRAME, LONG))0x0049b9a6)(pg, lPos);
}

// <LegoRR.exe @0049b9ac>
__inline void AVIFileInit(void)
{
	((void(__stdcall*)(void))0x0049b9ac)();
}

#pragma endregion

}
