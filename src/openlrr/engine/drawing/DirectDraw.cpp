// DirectDraw.cpp : 
//

#include "../../platform/windows.h"
#include "../../platform/ddraw.h"
#include "../../platform/d3drm.h"

#include "../core/Errors.h"
#include "../core/Files.h"
#include "../core/Memory.h"
#include "../util/Dxbug.h"
#include "../Graphics.h"
#include "../Main.h"
#include "Bmp.h"

#include "DirectDraw.h"


/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @0076bc80>
Gods98::DirectDraw_Globs & Gods98::directDrawGlobs = *(Gods98::DirectDraw_Globs*)0x0076bc80;

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @00406500>
IDirectDraw4* __cdecl Gods98::noinline(DirectDraw)(void)
{
	log_firstcall();

	return DirectDraw();
}

// <LegoRR.exe @00406510>
IDirectDrawSurface4* __cdecl Gods98::noinline(DirectDraw_bSurf)(void)
{
	log_firstcall();

	return DirectDraw_bSurf();
}

/*
// <unused>
__inline IDirectDrawSurface4* __cdecl Gods98::DirectDraw_fSurf()
{
	return directDrawGlobs.fSurf;
}

// <unused>
__inline bool32 __cdecl Gods98::DirectDraw_FullScreen()
{
	return directDrawGlobs.fullScreen;
}
*/


//__inline bool32 __cdecl DirectDraw_SetupFullScreen(lpDirectDraw_Driver driver, lpDirectDraw_Device device, lpDirectDraw_Mode mode)	{ return DirectDraw_Setup(true, driver, device, mode, 0, 0, 320, 200); }
//__inline bool32 __cdecl DirectDraw_SetupWindowed(lpDirectDraw_Device device, ULONG xPos, ULONG yPos, ULONG width, ULONG height)		{ return DirectDraw_Setup(false, nullptr, device, nullptr, xPos, yPos, width, height); }


// <LegoRR.exe @0047c430>
void __cdecl Gods98::DirectDraw_Initialise(HWND hWnd)
{
	log_firstcall();

	directDrawGlobs.driverCount = 0;
	directDrawGlobs.driverList  = nullptr;
	directDrawGlobs.deviceCount = 0;
	directDrawGlobs.deviceList  = nullptr;
	directDrawGlobs.modeCount   = 0;
	directDrawGlobs.modeList    = nullptr;
	directDrawGlobs.hWnd = hWnd;

	directDrawGlobs.lpDirectDraw = nullptr;
	directDrawGlobs.fSurf = directDrawGlobs.bSurf = directDrawGlobs.zSurf = nullptr;

	/// FIX APPLY: lpFrontClipper was assigned twice, instead of assigning lpBackClipper
	directDrawGlobs.lpFrontClipper = directDrawGlobs.lpBackClipper = nullptr;
}

// <LegoRR.exe @0047c480>
bool32 __cdecl Gods98::DirectDraw_EnumDrivers(OUT Graphics_Driver* list, OUT uint32* count)
{
	log_firstcall();

	directDrawGlobs.driverList = list;
	// Enumerate each driver and record its GUID and description
	legacy::DirectDrawEnumerateA(DirectDraw_EnumDriverCallback, nullptr);

	*count = directDrawGlobs.driverCount;
	return true;
}

// <LegoRR.exe @0047c4b0>
BOOL __stdcall Gods98::DirectDraw_EnumDriverCallback(GUID* lpGUID, char* lpDriverDescription, char* lpDriverName, void* lpContext)
{
	directDrawGlobs.driverList[directDrawGlobs.driverCount].flags = Graphics_DriverFlags::GRAPHICS_DRIVER_FLAG_VALID;

	// Record the GUID
	if (lpGUID != nullptr) directDrawGlobs.driverList[directDrawGlobs.driverCount].guid = *lpGUID;
	else directDrawGlobs.driverList[directDrawGlobs.driverCount].flags |= Graphics_DriverFlags::GRAPHICS_DRIVER_FLAG_PRIMARY;

#pragma message ( "TODO: Find out if driver can work in a window" )
	if (lpGUID == nullptr) directDrawGlobs.driverList[directDrawGlobs.driverCount].flags |= Graphics_DriverFlags::GRAPHICS_DRIVER_FLAG_WINDOWOK;

	std::sprintf(directDrawGlobs.driverList[directDrawGlobs.driverCount].desc, "%s (%s)", lpDriverDescription, lpDriverName);

	directDrawGlobs.driverCount++;

	return true;
}

// <LegoRR.exe @0047c5a0>
bool32 __cdecl Gods98::DirectDraw_EnumDevices(const Graphics_Driver* driver, OUT Graphics_Device* list, OUT uint32* count)
{
	log_firstcall();

	bool32 res = false;

	directDrawGlobs.deviceCount = 0;

	/// FIXME: Passing const_cast<GUID*>(&driver->guid) instead of assigned guid local variable
	GUID* guid = nullptr;
	if (driver->flags & Graphics_DriverFlags::GRAPHICS_DRIVER_FLAG_PRIMARY) guid = nullptr;
	else guid = const_cast<GUID*>(&driver->guid);

	IDirectDraw* lpDD1;
	if (legacy::DirectDrawCreate(const_cast<GUID*>(&driver->guid), &lpDD1, nullptr) == DD_OK) {
		IDirectDraw4* lpDD;
		if (lpDD1->QueryInterface(IID_IDirectDraw4, (void**)&lpDD) == DD_OK) {

			IDirect3D3* lpD3D;
			if (lpDD->QueryInterface(IID_IDirect3D3, (void**)&lpD3D) == DD_OK) {

				directDrawGlobs.deviceList = list;
				lpD3D->EnumDevices(DirectDraw_EnumDeviceCallback, nullptr);
				res = true;

				lpD3D->Release();
			} else Error_Warn(true, "Unable to obtain Direct3D3");
			lpDD->Release();
		} else Error_Warn(true, "Unable to obtain DirectDraw4");
		lpDD1->Release();
	} else Error_Warn(true, "Unable to create DirectDraw");

	*count = directDrawGlobs.deviceCount;

	return res;
}

// <LegoRR.exe @0047c640>
HRESULT __stdcall Gods98::DirectDraw_EnumDeviceCallback(GUID* lpGuid, char* lpDeviceDescription,
														char* lpDeviceName, D3DDEVICEDESC* lpHWDesc,
														D3DDEVICEDESC* lpHELDesc, void* lpContext)
{
	Graphics_Device* dev = &directDrawGlobs.deviceList[directDrawGlobs.deviceCount];
	D3DDEVICEDESC* desc;

	dev->flags = Graphics_DeviceFlags::GRAPHICS_DEVICE_FLAG_VALID;
	if (lpHWDesc->dcmColorModel != 0){
		dev->flags |= Graphics_DeviceFlags::GRAPHICS_DEVICE_FLAG_HARDWARE;
		desc = lpHWDesc;
	} else desc = lpHELDesc;

	if (desc->dwFlags & D3DDD_COLORMODEL) if (desc->dcmColorModel == D3DCOLOR_RGB) dev->flags |= Graphics_DeviceFlags::GRAPHICS_DEVICE_FLAG_COLOUR;
	if (desc->dwFlags & D3DDD_DEVICERENDERBITDEPTH){
		if (desc->dwDeviceRenderBitDepth & DDBD_8) dev->flags |= Graphics_DeviceFlags::GRAPHICS_DEVICE_FLAG_DEPTH8;
		if (desc->dwDeviceRenderBitDepth & DDBD_16) dev->flags |= Graphics_DeviceFlags::GRAPHICS_DEVICE_FLAG_DEPTH16;
		if (desc->dwDeviceRenderBitDepth & DDBD_24) dev->flags |= Graphics_DeviceFlags::GRAPHICS_DEVICE_FLAG_DEPTH24;
		if (desc->dwDeviceRenderBitDepth & DDBD_32) dev->flags |= Graphics_DeviceFlags::GRAPHICS_DEVICE_FLAG_DEPTH32;
	}

	if (desc->dwFlags & D3DDD_DEVCAPS){
		if (desc->dwDevCaps & D3DDEVCAPS_TEXTUREVIDEOMEMORY) dev->flags |= Graphics_DeviceFlags::GRAPHICS_DEVICE_FLAG_VIDEOTEXTURE;
		else if (desc->dwDevCaps & D3DDEVCAPS_TEXTURESYSTEMMEMORY) dev->flags |= Graphics_DeviceFlags::GRAPHICS_DEVICE_FLAG_SYSTEMTEXTURE;
	}

	dev->guid = *lpGuid;
	std::sprintf(dev->desc, "%s (%s)", lpDeviceName, lpDeviceDescription);

	directDrawGlobs.deviceCount++;

	return D3DENUMRET_OK;
}

// <LegoRR.exe @0047c770>
bool32 __cdecl Gods98::DirectDraw_EnumModes(const Graphics_Driver* driver, bool32 fullScreen, OUT Graphics_Mode* list, OUT uint32* count)
{
	log_firstcall();

	bool32 res = false;

	directDrawGlobs.modeCount = 0;

	if (driver) {
		if (driver->flags & Graphics_DriverFlags::GRAPHICS_DRIVER_FLAG_VALID) {

			GUID* guid = nullptr;
			if (!(driver->flags & Graphics_DriverFlags::GRAPHICS_DRIVER_FLAG_PRIMARY))
				guid = const_cast<GUID*>(&driver->guid);

			IDirectDraw* lpDD1;
			if (legacy::DirectDrawCreate(guid, &lpDD1, nullptr) == DD_OK) {
				IDirectDraw4* lpDD;
				if (lpDD1->QueryInterface(IID_IDirectDraw4, (void**)&lpDD) == DD_OK) {
					
					directDrawGlobs.modeList = list;
					lpDD->EnumDisplayModes(0, nullptr, &fullScreen, DirectDraw_EnumModeCallback);
					res = true;
					
					lpDD->Release();
				} else Error_Warn(true, "Unable to obtain DirectDraw2");
				lpDD1->Release();
			} else Error_Warn(true, "Unable to create DirectDraw");
		} else Error_Fatal(true, "Invalid driver passed to DirectDraw_EnumModes()");
	} else Error_Fatal(true, "NULL passed as driver to DirectDraw_EnumModes()");
			
	*count = directDrawGlobs.modeCount;
	return res;
}

// <LegoRR.exe @0047c810>
HRESULT __stdcall Gods98::DirectDraw_EnumModeCallback(DDSURFACEDESC2* lpDDSurfaceDesc, void* lpContext)
{
	Graphics_Mode* mode = &directDrawGlobs.modeList[directDrawGlobs.modeCount];
	bool32 fullScreen = *(bool32*)lpContext;

	mode->flags = Graphics_ModeFlags::GRAPHICS_MODE_FLAG_VALID;
	mode->width = lpDDSurfaceDesc->dwWidth;
	mode->height = lpDDSurfaceDesc->dwHeight;
	mode->bitDepth = lpDDSurfaceDesc->ddpfPixelFormat.dwRGBBitCount;

	if (fullScreen) std::sprintf(mode->desc, "%ix%i (%i bit)", lpDDSurfaceDesc->dwWidth, lpDDSurfaceDesc->dwHeight, lpDDSurfaceDesc->ddpfPixelFormat.dwRGBBitCount);
	else std::sprintf(mode->desc, "%ix%i", lpDDSurfaceDesc->dwWidth, lpDDSurfaceDesc->dwHeight);

	if (!fullScreen && directDrawGlobs.modeCount) {
		if (mode->bitDepth == Graphics_GetWindowsBitDepth()) directDrawGlobs.modeCount++;
		else mode->flags &= ~Graphics_ModeFlags::GRAPHICS_MODE_FLAG_VALID;
	} else directDrawGlobs.modeCount++;

	return D3DENUMRET_OK;
}

// <LegoRR.exe @0047c8d0>
bool32 __cdecl Gods98::DirectDraw_Setup(bool32 fullscreen, const Graphics_Driver* driver, const Graphics_Device* device,
								const Graphics_Mode* mode, uint32 xPos, uint32 yPos, uint32 width, uint32 height)
{
	log_firstcall();

	if (driver && !(driver->flags & Graphics_DriverFlags::GRAPHICS_DRIVER_FLAG_VALID)) driver = nullptr;
	if (device && !(device->flags & Graphics_DeviceFlags::GRAPHICS_DEVICE_FLAG_VALID)) device = nullptr;
	if (mode   && !(mode->flags   & Graphics_ModeFlags::GRAPHICS_MODE_FLAG_VALID))     mode   = nullptr;

	uint32 bpp = 16;
	if (mode) { // mode has priority over passed width/height values
		width  = mode->width;
		height = mode->height;
		bpp    = mode->bitDepth;
	}

	directDrawGlobs.width = width;
	directDrawGlobs.height = height;
	directDrawGlobs.fullScreen = fullscreen;

	GUID* guid = nullptr;
	if (driver && !(driver->flags & Graphics_DriverFlags::GRAPHICS_DRIVER_FLAG_PRIMARY))
		guid = const_cast<GUID*>(&driver->guid);

	Main_SetupDisplay(fullscreen, xPos, yPos, width, height);

	IDirectDraw* ddraw1;
	if (legacy::DirectDrawCreate(guid, &ddraw1, nullptr) == DD_OK) {
		if (ddraw1->QueryInterface(IID_IDirectDraw4, (void**)&directDrawGlobs.lpDirectDraw) == DD_OK) {
			if (directDrawGlobs.lpDirectDraw->SetCooperativeLevel(directDrawGlobs.hWnd, fullscreen?(DDSCL_EXCLUSIVE|DDSCL_FULLSCREEN):DDSCL_NORMAL) == DD_OK) {
				HRESULT r;
				if (fullscreen) r = directDrawGlobs.lpDirectDraw->SetDisplayMode(width, height, bpp, 0, 0);
				else r = DD_OK;

				if (r == DD_OK) {
					DDSURFACEDESC2 desc;
					std::memset(&desc, 0, sizeof(desc));
					desc.dwSize = sizeof(desc);
					desc.dwFlags = DDSD_CAPS;
					desc.ddsCaps.dwCaps = DDSCAPS_3DDEVICE | DDSCAPS_PRIMARYSURFACE;
					if (fullscreen) {
						desc.dwFlags |= DDSD_BACKBUFFERCOUNT;
						desc.dwBackBufferCount = 1;
						desc.ddsCaps.dwCaps |= DDSCAPS_FLIP | DDSCAPS_COMPLEX;
					}
					
					if (directDrawGlobs.lpDirectDraw->CreateSurface(&desc, &directDrawGlobs.fSurf, nullptr) == DD_OK) {
						
						if (!fullscreen) {
							// Create the back buffer
							desc.ddsCaps.dwCaps &= ~DDSCAPS_PRIMARYSURFACE;
							desc.ddsCaps.dwCaps |= DDSCAPS_OFFSCREENPLAIN;
							desc.dwFlags |= DDSD_HEIGHT | DDSD_WIDTH;
							desc.dwWidth  = width;
							desc.dwHeight = height;
							r = directDrawGlobs.lpDirectDraw->CreateSurface(&desc, &directDrawGlobs.bSurf, nullptr);
						} else {
							DDSCAPS2 ddscaps;
							std::memset(&ddscaps, 0, sizeof(ddscaps));
							ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
							r = directDrawGlobs.fSurf->GetAttachedSurface(&ddscaps, &directDrawGlobs.bSurf);
						}
						
						if (r == DD_OK) {
							if (DirectDraw_CreateClipper(fullscreen, width, height)) {
								if (Graphics_SetupDirect3D(device, ddraw1, directDrawGlobs.bSurf, fullscreen)) {

									// Everything went OK, so tidy up and return
									ddraw1->Release();

									/// NOTE: global cursor behavior to pay attention to
									///  when fixing cursor visibility over title bar.
									/// FIX APPLY: (by removing) Show cursor over title bar
									///  This has been removed along with Main_SetupDisplay's windowed ShowCursor(false).
									///  The fix is then handled with the Main_WndProc message WM_SETCURSOR.
									//if (fullscreen) ::ShowCursor(false);

									return true;
									
								}
							}
							directDrawGlobs.bSurf->Release();
							directDrawGlobs.bSurf = nullptr;
						} else Error_Warn(true, "Error creating back surface");
						directDrawGlobs.fSurf->Release();
						directDrawGlobs.fSurf = nullptr;
					} else Error_Warn(true, "Error creating front surface");
				} else Error_Warn(true, "Cannot set Display Mode");
			} else Error_Warn(true, "Cannot set Cooperative Level");
			directDrawGlobs.lpDirectDraw->Release();
			directDrawGlobs.lpDirectDraw = nullptr;
		} else Error_Warn(true, "Cannot obtain DirectDraw2");
		ddraw1->Release();
	} else Error_Warn(true, "Cannot create DirectDraw");
	
	return false;
}

// <LegoRR.exe @0047cb90>
void __cdecl Gods98::DirectDraw_Flip(void)
{
	log_firstcall();

	if (directDrawGlobs.fullScreen) {
		HRESULT r = directDrawGlobs.fSurf->Flip(nullptr, DDFLIP_WAIT);
		Error_Fatal(r == DDERR_SURFACELOST, "Surface lost");
	}
	else DirectDraw_BlitBuffers();
}

// <LegoRR.exe @0047cbb0>
bool32 __cdecl Gods98::DirectDraw_SaveBMP(IDirectDrawSurface4* surface, const char* fname)
{
	bool32 ok = false;

	File* ofp;
	if (ofp = File_Open(fname, "wb")) {

		DDSURFACEDESC2 desc;
		std::memset(&desc, 0, sizeof(desc));
		desc.dwSize = sizeof(desc);
		if (surface->Lock(nullptr, &desc, DDLOCK_WAIT, nullptr) == DD_OK) {
			// (width * 3 bytes per pixel), then rounded up to unit of 4 bytes (BMP padding)
			uint32 lineWidth = ((desc.dwWidth * 3) + 3) & ~0x3;
			uint32 bufferSize = lineWidth * desc.dwHeight;

			uint8* buffer;
			if (buffer = (uint8*)Mem_Alloc(bufferSize)) {
				std::memset(buffer, 0, bufferSize);

				uint32 rBits = 0, gBits = 0, bBits = 0;
				uint32 bitsPerPixel = desc.ddpfPixelFormat.dwRGBBitCount;
				uint32 bytesPerPixel = bitsPerPixel / 8;
				for (uint32 loop=0; loop<32; loop++) if ((desc.ddpfPixelFormat.dwRBitMask >> loop) & 0x1) rBits++;
				for (uint32 loop=0; loop<32; loop++) if ((desc.ddpfPixelFormat.dwGBitMask >> loop) & 0x1) gBits++;
				for (uint32 loop=0; loop<32; loop++) if ((desc.ddpfPixelFormat.dwBBitMask >> loop) & 0x1) bBits++;
				uint32 rShift = gBits + bBits;
				uint32 gShift = bBits;
				uint32 bShift = 0;

				for (uint32 y=0; y<desc.dwHeight; y++) {
					for (uint32 x=0; x<desc.dwWidth; x++) {
						uint32 ul = ((uint32*)(&((uint8*)desc.lpSurface)[(y * desc.lPitch) + (x * bytesPerPixel)]))[0];
						ul >>= 32 - bitsPerPixel;
						buffer[(((desc.dwHeight-1) - y) * lineWidth) + (x*3) + 2] = (uint8)(((ul >> rShift) << (8 - rBits)));
						buffer[(((desc.dwHeight-1) - y) * lineWidth) + (x*3) + 1] = (uint8)(((ul >> gShift) << (8 - gBits)));
						buffer[(((desc.dwHeight-1) - y) * lineWidth) + (x*3) + 0] = (uint8)(((ul >> bShift) << (8 - bBits)));
					}
				}

				ok = true;
			}

			surface->Unlock(nullptr);

			BMP_Header header = { 0 };
			header.bmp_str[0] = 'B';
			header.bmp_str[1] = 'M';
			header.file_size = bufferSize + sizeof(BMP_Header);
			header.img_offset = sizeof(BMP_Header);
			header.img_header_size = 40;
			header.wid = desc.dwWidth;
			header.hgt = desc.dwHeight;
			header.planes = 1;
			header.bits_per_pixel = 24;
			header.compression = 0;
			header.comp_img_size = 0;
			header.horz_res = 1;
			header.vert_res = 1;
			header.colours = 0;
			header.important_colours = 0;

			if (ok) {
				File_Write(&header, sizeof(BMP_Header), 1, ofp);
				File_Write(buffer, bufferSize, 1, ofp);
			}

			Mem_Free(buffer);
		}
		File_Close(ofp);
	}

	return ok;
}

// <LegoRR.exe @0047cee0>
void __cdecl Gods98::DirectDraw_ReturnFrontBuffer(void)
{
	log_firstcall();

	if (directDrawGlobs.fullScreen) {
		directDrawGlobs.bSurf->Blt(nullptr, directDrawGlobs.fSurf, nullptr, DDBLT_WAIT, nullptr);
	}
}

// <LegoRR.exe @0047cf10>
void __cdecl Gods98::DirectDraw_BlitBuffers(void)
{
	log_firstcall();

	// Calculate the destination blitting rectangle
	//	::GetClientRect(directDrawGlobs.hWnd, &dest);

	POINT pt = { 0, 0 };
	RECT dest = { // sint32 casts to stop compiler from complaining
		0, 0,
		(sint32) directDrawGlobs.width,
		(sint32) directDrawGlobs.height,
	};
	::ClientToScreen(directDrawGlobs.hWnd, &pt);
	::OffsetRect(&dest, pt.x, pt.y);

	// Fill out the source of the blit
	RECT src = { // sint32 casts to stop compiler from complaining
		0, 0,
		(sint32) directDrawGlobs.width,
		(sint32) directDrawGlobs.height,
	};

	HRESULT r = directDrawGlobs.fSurf->Blt(&dest, directDrawGlobs.bSurf, &src, DDBLT_WAIT, nullptr);
	Error_Fatal(r == DDERR_SURFACELOST, "Front surface lost");

/*		while (true){

		if ((ddrval = directDrawGlobs.fSurf->lpVtbl->Blt(directDrawGlobs.fSurf, &dest, directDrawGlobs.bSurf, &src, DDBLT_WAIT, nullptr)) == DD_OK){
			break;
		if(ddrval == DDERR_SURFACELOST) if(!Ddraw_RestoreSurfaces()) break;
		if(ddrval != DDERR_WASSTILLDRAWING) break;
	}

*/

//	return Succeeded;
}

// <LegoRR.exe @0047cfb0>
void __cdecl Gods98::DirectDraw_Shutdown(void)
{
	log_firstcall();

	if (directDrawGlobs.fSurf) directDrawGlobs.fSurf->Release();
	if (directDrawGlobs.lpFrontClipper) directDrawGlobs.lpFrontClipper->Release();
	if (directDrawGlobs.lpBackClipper) directDrawGlobs.lpBackClipper->Release();
	if (directDrawGlobs.lpDirectDraw) {
		if (directDrawGlobs.fullScreen) directDrawGlobs.lpDirectDraw->RestoreDisplayMode();
		if (directDrawGlobs.lpDirectDraw) directDrawGlobs.lpDirectDraw->Release();
	}
}

// Adjust the texture usage for cards that don't like 8 bit textures...
// <LegoRR.exe @0047d010>
void __cdecl Gods98::DirectDraw_AdjustTextureUsage(IN OUT uint32* textureUsage)
{
	log_firstcall();

	// Adjust the texture usage for cards that don't like 8 bit textures...

	DDPIXELFORMAT pixelFormat;

	std::memset(&pixelFormat, 0, sizeof(pixelFormat));
	pixelFormat.dwSize = sizeof(DDPIXELFORMAT);

	if (lpDevice()->FindPreferredTextureFormat(DDBD_8, D3DRMFPTF_PALETTIZED, &pixelFormat) != D3DRM_OK) {

		std::memset(&pixelFormat, 0, sizeof(pixelFormat));
		pixelFormat.dwSize = sizeof(DDPIXELFORMAT);
		
		if (lpDevice()->FindPreferredTextureFormat(DDBD_16, 0, &pixelFormat) == D3DRM_OK) {

			// multiply by BYTES-per-pixel
			*textureUsage *= (pixelFormat.dwRGBBitCount / 8);
		}
	}
}

// <LegoRR.exe @0047d090>
bool32 __cdecl Gods98::DirectDraw_GetAvailTextureMem(OUT uint32* total, OUT uint32* avail)
{
	log_firstcall();

	DDSCAPS2 ddsc;

	std::memset(&ddsc, 0, sizeof(ddsc));
	ddsc.dwCaps = DDSCAPS_TEXTURE;

	*total = 0;
	*avail = 0;

	DWORD total_ = 0;
	DWORD avail_ = 0;
	if (directDrawGlobs.lpDirectDraw->GetAvailableVidMem(&ddsc, &total_, &avail_) == DD_OK) {
		*total = (uint32)total_;
		*avail = (uint32)avail_;
		return true;
	}

	return false;
}

// <LegoRR.exe @0047d0e0>
void __cdecl Gods98::DirectDraw_Clear(const Area2F* window, uint32 colour)
{
	log_firstcall();

	DDBLTFX bltFX;

	std::memset(&bltFX, 0, sizeof(DDBLTFX));
	bltFX.dwSize = sizeof(DDBLTFX);
	bltFX.dwFillColor = DirectDraw_GetColour(directDrawGlobs.bSurf, colour);

	if (window) {
		/// FIXME: Cast from float to unsigned
		RECT rect = { // sint32 casts to stop compiler from complaining
			(sint32) (uint32) window->x,
			(sint32) (uint32) window->y,
			(sint32) (uint32) (window->x + window->width),
			(sint32) (uint32) (window->y + window->height)
		};
		directDrawGlobs.bSurf->Blt(&rect, nullptr, nullptr, DDBLT_COLORFILL|DDBLT_WAIT, &bltFX);
	} else directDrawGlobs.bSurf->Blt(nullptr, nullptr, nullptr, DDBLT_COLORFILL|DDBLT_WAIT, &bltFX);
}

// <LegoRR.exe @0047d1a0>
bool32 __cdecl Gods98::DirectDraw_CreateClipper(bool32 fullscreen, uint32 width, uint32 height)
{
	log_firstcall();

	if (directDrawGlobs.lpDirectDraw->CreateClipper(0, &directDrawGlobs.lpBackClipper, nullptr) == DD_OK){

		HRGN handle = ::CreateRectRgn(0, 0, width, height);
		uint32 size = ::GetRegionData(handle, 0, nullptr);
		RGNDATA* region = (RGNDATA*)Mem_Alloc(size);
		::GetRegionData(handle, size, region);

		if (directDrawGlobs.lpBackClipper->SetClipList(region, 0) == DD_OK){
			
			Mem_Free(region);

			if (directDrawGlobs.bSurf->SetClipper(directDrawGlobs.lpBackClipper) == DD_OK){
				if (!fullscreen) {
					// Create the window clipper
					if (directDrawGlobs.lpDirectDraw->CreateClipper(0, &directDrawGlobs.lpFrontClipper, nullptr) == DD_OK){
						// Associate the clipper with the window (obtain window sizes).
						if (directDrawGlobs.lpFrontClipper->SetHWnd(0, directDrawGlobs.hWnd) == DD_OK){
							if (directDrawGlobs.fSurf->SetClipper(directDrawGlobs.lpFrontClipper) == DD_OK){
								
								return true;
								
							} else Error_Warn(true, "Cannot attach clipper to front buffer");
						} else Error_Warn(true, "Cannot initialise clipper from hWnd");
					} else Error_Warn(true, "Cannot create front clipper");
				} else {
					// Fullscreen, we can skip the rest above
					return true;
				}
				
			} else Error_Warn(true, "Cannot attach clipper to back buffer");
			directDrawGlobs.lpBackClipper->Release();
			directDrawGlobs.lpBackClipper = nullptr;
		} else Error_Warn(true, "Cannot set clip list");
		Mem_Free(region);

	}

	return false;
}

// <LegoRR.exe @0047d2c0>
void __cdecl Gods98::DirectDraw_Blt8To16(IDirectDrawSurface4* target, IDirectDrawSurface4* source, BMP_PaletteEntry* palette)
{
	log_firstcall();

	DDSURFACEDESC2 tDesc, sDesc;

	std::memset(&tDesc, 0, sizeof(tDesc));
	std::memset(&sDesc, 0, sizeof(sDesc));
	tDesc.dwSize = sizeof(tDesc);
	sDesc.dwSize = sizeof(sDesc);

	if (source->Lock(nullptr, &sDesc, DDLOCK_WAIT, nullptr) == DD_OK) {
		if (sDesc.ddpfPixelFormat.dwRGBBitCount == 8) {
			if (target->Lock(nullptr, &tDesc, DDLOCK_WAIT, nullptr) == DD_OK) {
				if (tDesc.ddpfPixelFormat.dwRGBBitCount == 16 && tDesc.dwWidth == sDesc.dwWidth && tDesc.dwHeight == sDesc.dwHeight) {

					uint32 rBits = 0, gBits = 0, bBits = 0;
					for (uint32 loop=0; loop<32; loop++) if ((tDesc.ddpfPixelFormat.dwRBitMask >> loop) & 0x1) rBits++;
					for (uint32 loop=0; loop<32; loop++) if ((tDesc.ddpfPixelFormat.dwGBitMask >> loop) & 0x1) gBits++;
					for (uint32 loop=0; loop<32; loop++) if ((tDesc.ddpfPixelFormat.dwBBitMask >> loop) & 0x1) bBits++;

					for (uint32 y=0 ; y<sDesc.dwHeight ; y++) {
						for (uint32 x=0 ; x<sDesc.dwWidth ; x+=4) {

							uint32 write = 0;
							uint32 read = ((uint32*) (&((uint8*) sDesc.lpSurface)[(y * sDesc.lPitch) + x]))[0];
							for (sint32 loop=3; loop>=0; loop--) {
								
								uint8 index = (uint8) (read >> (8 * loop));
								uint8 r = palette[index].peRed   >> (8 - rBits);
								uint8 g = palette[index].peGreen >> (8 - gBits);
								uint8 b = palette[index].peBlue  >> (8 - bBits);

								if (loop & 0x1) write = ((r << (gBits + bBits)) | (g << bBits) | b) << 16;
								else {
									uint32 offset = (loop & 0x2)?1:0;

									write |= ((r << (gBits + bBits)) | (g << bBits) | b);
									((uint32*) (&((uint8*) tDesc.lpSurface)[(y * tDesc.lPitch) + (x * 2)]))[offset] = write;
								}
							}
						}
					}

				}
				target->Unlock(nullptr);
			}
		}
		source->Unlock(nullptr);

		Error_Fatal(sDesc.ddpfPixelFormat.dwRGBBitCount != 8, "Source surface is not 8bits deep");
		Error_Fatal(tDesc.ddpfPixelFormat.dwRGBBitCount != 16, "Target surface is not 16bits deep");
		Error_Fatal(tDesc.dwWidth != sDesc.dwWidth || tDesc.dwHeight != sDesc.dwHeight, "Source and destination surfaces differ in dimension");
	}
}

// <LegoRR.exe @0047d590>
uint32 __cdecl Gods98::DirectDraw_GetColour(IDirectDrawSurface4* surf, uint32 colour)
{
	log_firstcall();

	DDPIXELFORMAT pf;

	uint32 r = (colour & 0x00ff0000) >> 16;
	uint32 g = (colour & 0x0000ff00) >> 8;
	uint32 b = (colour & 0x000000ff);

	std::memset(&pf, 0, sizeof(pf));
	pf.dwSize = sizeof(pf);
	surf->GetPixelFormat(&pf);
	if (pf.dwFlags & DDPF_RGB) {
		uint32 rbc = DirectDraw_GetNumberOfBits(pf.dwRBitMask);
		uint32 gbc = DirectDraw_GetNumberOfBits(pf.dwGBitMask);
		uint32 bbc = DirectDraw_GetNumberOfBits(pf.dwBBitMask);

		r = r >> (8 - rbc);
		g = g >> (8 - gbc);
		b = b >> (8 - bbc);

		r = r << (gbc + bbc);
		g = g << (bbc);

		return (r|g|b);
	} else {
		IDirectDrawPalette* pal;
		PALETTEENTRY entries[256];

		surf->GetPalette(&pal);
		pal->GetEntries(0, 0, 256, entries);
		for (uint32 loop=0; loop<256; loop++) {
			if (entries[loop].peRed   == r &&
				entries[loop].peGreen == g &&
				entries[loop].peBlue  == b) {

				return loop;
			}
		}
	}
	return 0;
}

// <LegoRR.exe @0047d6b0>
uint32 __cdecl Gods98::DirectDraw_GetNumberOfBits(uint32 mask)
{
	log_firstcall();

	uint32 bc = 0;
	while (mask) {
		mask = mask & (mask - 1);
		bc++;
	}
	return bc;
}

#pragma endregion
