#pragma once

#include "windows.h"
#include <d3drmwin.h>


/**********************************************************************************
 ******** IIDs
 **********************************************************************************/

#pragma region IIDs
namespace Idl
{; // !<---

/// TODO: We're currently just relying on the IID defined in LegoRR,
///        since our `d3drm.lib` won't give it to us. Later on this should be manually defined.

// {4a1b1be6-bfed-11d1-8ed8-00a0c967a482}
// <LegoRR.exe @004a0958>
extern IID & IID_IDirect3DRMViewport2;
// {eb16cb03-d271-11ce-ac48-0000c03825a1}
// <LegoRR.exe @004a0968>
extern IID & IID_IDirect3DRMFrame;
// {ff6b7f70-a40e-11d1-91f9-0000f8758e66}
// <LegoRR.exe @004a0988>
extern IID & IID_IDirect3DRMFrame3;
// {a3a80d01-6e12-11cf-ac4a-0000c03825a1}
// <LegoRR.exe @004a09a8>
extern IID & IID_IDirect3DRMMesh;
// {4516ec77-8f20-11d0-9b6d-0000c0781bc3}
// <LegoRR.exe @004a09c8>
extern IID & IID_IDirect3DRMMeshBuilder2;
// {eb16cb09-d271-11ce-ac48-0000c03825a1}
// <LegoRR.exe @004a0a18>
extern IID & IID_IDirect3DRMTexture;
// {59163de0-6d43-11cf-ac4a-0000c03825a1}
// <LegoRR.exe @004a0b48>
extern IID & IID_IDirect3DRMUserVisual;
// {4516ec83-8f20-11d0-9b6d-0000c0781bc3}
// <LegoRR.exe @004a0bd8>
extern IID & IID_IDirect3DRM3;

}
#pragma endregion


namespace legacy
{; // !<---

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @0049b970>
__inline HRESULT Direct3DRMCreate(LPDIRECT3DRM* lplpDirect3DRM)
{
	return ((HRESULT(__stdcall*)(LPDIRECT3DRM*))0x0049b970)(lplpDirect3DRM);
}

// <LegoRR.exe @0049b976>
__inline LPD3DVECTOR D3DRMVectorRandom(LPD3DVECTOR d)
{
	return ((LPD3DVECTOR(__stdcall*)(LPD3DVECTOR))0x0049b976)(d);
}

// <LegoRR.exe @0049b97c>
__inline LPD3DVECTOR D3DRMVectorRotate(LPD3DVECTOR r, LPD3DVECTOR v, LPD3DVECTOR axis, D3DVALUE theta)
{
	return ((LPD3DVECTOR(__stdcall*)(LPD3DVECTOR, LPD3DVECTOR, LPD3DVECTOR, D3DVALUE))0x0049b97c)(r, v, axis, theta);
}

#pragma endregion

}
