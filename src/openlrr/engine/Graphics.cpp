// Graphics.cpp : 
//

#include "../platform/windows.h"
#include "../platform/d3drm.h"		// <d3drmwin.h>
#include "../platform/timeapi.h"
#include "../../../resources/resource.h"

#include "geometry.h"

#include "audio/Sound.h"
#include "video/Animation.h"
#include "core/Config.h"
#include "core/Errors.h"
#include "core/Files.h"
#include "core/Memory.h"
#include "core/Utils.h"
#include "drawing/DirectDraw.h"
#include "drawing/Draw.h"
#include "drawing/Fonts.h"
#include "drawing/Images.h"
#include "input/Input.h"
#include "util/Dxbug.h"
#include "util/Registry.h"
#include "Init.h"

#include "Main.h"

#include "Graphics.h"


/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

Gods98::Graphics_Globs Gods98::graphicsGlobs = { 0 };

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions


/*bool32 Gods98::Graphics_IsDither(void)
{
	return graphicsGlobs.dither;
}*/

void Gods98::Graphics_SetDither(bool32 on)
{
	graphicsGlobs.dither = on;
}

/*bool32 Gods98::Graphics_IsLinearFilter(void)
{
	return graphicsGlobs.linearFilter;
}*/

void Gods98::Graphics_SetLinearFilter(bool32 on)
{
	graphicsGlobs.linearFilter = on;
}

/*bool32 Gods98::Graphics_IsMIPMap(void)
{
	return graphicsGlobs.mipMap;
}*/

void Gods98::Graphics_SetMIPMap(bool32 on)
{
	graphicsGlobs.mipMap = on;
}

/*bool32 Gods98::Graphics_IsMIPMapLinear(void)
{
	return graphicsGlobs.mipMapLinear;
}*/

void Gods98::Graphics_SetMIPMapLinear(bool32 on)
{
	graphicsGlobs.mipMapLinear = on;
}

/*bool32 Gods98::Graphics_IsBlendTransparency(void)
{
	return graphicsGlobs.blendTransparency;
}*/

void Gods98::Graphics_SetBlendTransparency(bool32 on)
{
	graphicsGlobs.blendTransparency = on;
}

/*bool32 Gods98::Graphics_IsSortTransparency(void)
{
	return graphicsGlobs.sortTransparency;
}*/

void Gods98::Graphics_SetSortTransparency(bool32 on)
{
	graphicsGlobs.sortTransparency = on;
}


/*Gods98::Graphics_Quality Gods98::Graphics_GetRenderQuality(void)
{
	return graphicsGlobs.renderQuality;
}*/

void Gods98::Graphics_SetRenderQuality(Graphics_Quality renderQuality)
{
	graphicsGlobs.renderQuality = renderQuality;
}


// <LegoRR.exe @00477e90>
void __cdecl Gods98::Graphics_DisableTextureManagement(void)
{
	log_firstcall();

	if (!(mainGlobs.flags & MainFlags::MAIN_FLAG_FORCETEXTUREMANAGEMENT)) {
		mainGlobs.flags |= MainFlags::MAIN_FLAG_DONTMANAGETEXTURES;
	}
}

// <LegoRR.exe @00478240>
uint32 __cdecl Gods98::Graphics_GetWindowsBitDepth(void)
{
	log_firstcall();

	return ::GetDeviceCaps(::GetDC(Main_hWnd()), BITSPIXEL);
}

// <LegoRR.exe @00478260>
void __cdecl Gods98::Graphics_Finalise3D(void)
{
	log_firstcall();

	if (!(mainGlobs.flags & MainFlags::MAIN_FLAG_UPDATED)) {
		mainGlobs.device->Update();
		mainGlobs.flags |= MainFlags::MAIN_FLAG_UPDATED;
	}
}

// <LegoRR.exe @00478490>
bool32 __cdecl Gods98::Graphics_SetupDirect3D(const Graphics_Device* dev, IDirectDraw* ddraw1, IDirectDrawSurface4* backSurf, bool32 doubleBuffered)
{
	log_firstcall();

	HRESULT r = 0;

	GUID* guid = nullptr;
	if (dev) {
		guid = const_cast<GUID*>(&dev->guid);
		if (dev->flags & Graphics_DeviceFlags::GRAPHICS_DEVICE_FLAG_VIDEOTEXTURE)
			mainGlobs.flags |= MainFlags::MAIN_FLAG_VIDEOTEXTURE;
	}

#pragma message ( "CAPS testing is a complete waste of time for this" )
	if (!(mainGlobs.flags & MainFlags::MAIN_FLAG_FORCEVERTEXFOG) &&
		dev && (dev->flags & Graphics_DeviceFlags::GRAPHICS_DEVICE_FLAG_HARDWARE))
	{
		Graphics_SetFogMethod(D3DRMFOGMETHOD_TABLE);
	} else {
		Graphics_SetFogMethod(D3DRMFOGMETHOD_VERTEX);
	}

	// Create D3DRM and the device...
	
	IDirect3DRM* lpD3DRM1;
	if (::Direct3DRMCreate(&lpD3DRM1) == D3DRM_OK) {
		/// NOTE: referencing LegoRR's IID_IDirect3DRM3 data (define this ourselves!)
		if (lpD3DRM1->QueryInterface(Idl::IID_IDirect3DRM3, (void**)&mainGlobs.lpD3DRM) == D3DRM_OK) {

			IDirectDrawSurface* surf1 = nullptr; // dummy init
			backSurf->QueryInterface(IID_IDirectDrawSurface, (void**)&surf1);

			/// NOTE ASM: Decompiled code shows dev->guid,
			///  but this is only because it's the first structure item, and referenced
			///  by address. Meaning a NULL dev is "functionally safe".
			if ((r = mainGlobs.lpD3DRM->CreateDeviceFromSurface(guid, ddraw1, surf1, 0, &mainGlobs.device)) == D3DRM_OK) {

				IDirect3DDevice2* imdev2;
				mainGlobs.device->GetDirect3DDevice2(&imdev2);
				imdev2->QueryInterface(IID_IDirect3DDevice3, (void**)&mainGlobs.imDevice);
				imdev2->Release();

				// This is code that was commented out in Gods98, and nothing more.
//			IDirect3D3* d3d3;
//			if ((r = mainGlobs.lpD3DRM->CreateDeviceFromSurface(nullptr, ddraw1, surf1, 0, &mainGlobs.device)) == D3DRM_OK) {
//			if (ddraw1->lpVtbl->QueryInterface(ddraw1, &IID_IDirect3D3, &d3d3) == D3DRM_OK) {
//				IDirect3DDevice3* d3ddev3;
//				r = 1;
//				if (std::memcmp(guid, &IID_IDirect3DRGBDevice, sizeof(GUID)) == 0) {
//					r = d3d3->CreateDevice(IID_IDirect3DMMXDevice, backSurf, &d3ddev3, nullptr);
//				}
//				if (r != D3DRM_OK) {
//					r = d3d3->CreateDevice(guid, backSurf, &d3ddev3, nullptr);
//				}
//
//				if (D3DRM_OK == r) {
//					IDirect3D2* d3d2;
//					IDirect3DDevice2* d3ddev2;
//					d3d3->lpVtbl->QueryInterface(d3d3, &IID_IDirect3D2, &d3d2);
//					d3ddev3->lpVtbl->QueryInterface(d3ddev3, &IID_IDirect3DDevice2, &d3ddev2);
//					mainGlobs.lpD3DRM->lpVtbl->CreateDeviceFromD3D(mainGlobs.lpD3DRM, d3d2, d3ddev2, &mainGlobs.device);
//					d3ddev3->lpVtbl->Release(d3ddev3);
//					d3ddev2->lpVtbl->Release(d3ddev2);
//					d3d2->lpVtbl->Release(d3d2);
//
//					d3d3->lpVtbl->Release(d3d3);

				if (doubleBuffered) mainGlobs.device->SetBufferCount(2);
				return true;
					
			} else {
				CHKDD(r);
				Error_Warn(true, "Unable to create Device");
			}
//				d3d3->lpVtbl->Release(d3d3);
//			} else Error_Warn(true, "Unable to obtain Direct3D3");
		} else Error_Warn(true, "Failed query for IID_IDirect3DRM3");
	} else Error_Warn(true, "Unable to create lpD3DRM1");

	CHKRM(r);

	return false;
}

// <LegoRR.exe @004785f0>
void __cdecl Gods98::Graphics_Setup3D(Graphics_Quality renderQuality, bool32 dither, bool32 linearFilter, bool32 mipMap,
									  bool32 mipMapLinear, bool32 blendTransparency, bool32 sortTransparency)
{
	log_firstcall();

	graphicsGlobs.renderQuality = renderQuality;
	graphicsGlobs.dither        = dither;
	graphicsGlobs.linearFilter  = linearFilter;
	graphicsGlobs.mipMap        = mipMap;
	graphicsGlobs.mipMapLinear  = mipMapLinear;
	graphicsGlobs.blendTransparency = blendTransparency;
	graphicsGlobs.sortTransparency  = sortTransparency;

	D3DRMRENDERQUALITY quality;// = D3DRMRENDER_WIREFRAME; // default on invalid value
	switch (renderQuality) {
	default: // default to Wireframe on invalid value
	case Graphics_Quality::Wireframe: quality = D3DRMRENDER_WIREFRAME; break;
	case Graphics_Quality::UnlitFlat: quality = D3DRMRENDER_UNLITFLAT; break;
	case Graphics_Quality::Flat:      quality = D3DRMRENDER_FLAT;      break;
	case Graphics_Quality::Gouraud:   quality = D3DRMRENDER_GOURAUD;   break;
	/// CUSTOM: Added to match rest of the set, but this will never be passed as a parameter
	case Graphics_Quality::Phong:     quality = D3DRMRENDER_PHONG;     break;
	}

	/// NEW GODS98: Not present in LegoRR (these arguments ended up being unused)
	DWORD renderMode = 0;
	if (blendTransparency) renderMode |= D3DRMRENDERMODE_BLENDEDTRANSPARENCY;
	if (sortTransparency)  renderMode |= D3DRMRENDERMODE_SORTEDTRANSPARENCY;

	D3DRMTEXTUREQUALITY textureMode;
	if (!linearFilter) {
		textureMode = D3DRMTEXTURE_NEAREST;
		//mainGlobs.flags &= ~MainFlags::MAIN_FLAG_MIPMAPENABLED;
	}
	else if (!mipMap) {
		textureMode = D3DRMTEXTURE_LINEAR;
		//mainGlobs.flags &= ~MainFlags::MAIN_FLAG_MIPMAPENABLED;
	}
	else {
		if (!mipMapLinear)
			textureMode = D3DRMTEXTURE_MIPLINEAR;
		else
			textureMode = D3DRMTEXTURE_LINEARMIPLINEAR;

		mainGlobs.flags |= MainFlags::MAIN_FLAG_MIPMAPENABLED;
	}

	if (mainGlobs.device) {

		// Should check device caps here...

		mainGlobs.device->SetDither(dither);
		mainGlobs.device->SetQuality(quality);
		mainGlobs.device->SetTextureQuality(textureMode);
//		mainGlobs.device->SetRenderMode(D3DRMRENDERMODE_BLENDEDTRANSPARENCY|D3DRMRENDERMODE_SORTEDTRANSPARENCY |D3DRMRENDERMODE_DISABLESORTEDALPHAZWRITE);
		/// NEW GODS98: Not present in LegoRR
		mainGlobs.device->SetRenderMode(renderMode);

	} else Error_Fatal(true, "Device not initialised");
}


// <missing>
uint32 __cdecl Gods98::Graphics_GetTrianglesDrawn(bool32 total)
{
	static uint32 last = 0;

	uint32 curr = mainGlobs.device->GetTrianglesDrawn();

	uint32 count;
	if (!total) count = curr - last;
	else count = curr;

	last = curr;
	return count;
}

// <LegoRR.exe @00478b90>
void __cdecl Gods98::Graphics_ChangeRenderState(D3DRENDERSTATETYPE dwRenderStateType, uint32 dwRenderState)
{
	log_firstcall();

	Error_Fatal(dwRenderStateType >= GRAPHICS_MAXRENDERSTATES, "RenderState type is out of range");

	DWORD currValue;
	Graphics_StateChangeData* data = &mainGlobs.renderStateData[dwRenderStateType];
	HRESULT r = mainGlobs.imDevice->GetRenderState(dwRenderStateType, &currValue);
	Error_Fatal(r != D3D_OK, Error_Format("Failed to GetRenderState(%i)", dwRenderStateType));

	if (currValue != dwRenderState) {

//		if( D3D_OK ==
		mainGlobs.imDevice->SetRenderState(dwRenderStateType, dwRenderState);
			// )
		{
			if (data->changed) {
				if (data->origValue == currValue) data->changed = false;
			}
			else {
				data->origValue = currValue;
				data->changed = true;
			}

		}
	}
}

/// NOTE: newer engine version has argument: BOOL force, but LegoRR DOES NOT have this argument.
///        (it's possible this argument has been inlined, as it negates calling the entire function body.)
// <LegoRR.exe @00478c00>
void __cdecl Gods98::Graphics_RestoreStates(void)
{
	log_firstcall();

	// NEW GODS98: BOOL force argument
	//if (force) {
	for (uint32 loop = 0; loop < GRAPHICS_MAXRENDERSTATES; loop++) {
		Graphics_StateChangeData* data = &mainGlobs.renderStateData[loop];
		if (data->changed) {
			mainGlobs.imDevice->SetRenderState((D3DRENDERSTATETYPE)loop, data->origValue);
			data->changed = false;
		}
	}
	//}
}

#pragma endregion
