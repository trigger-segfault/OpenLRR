// Images.cpp : 
//

#include "../../platform/ddraw.h"
#include "../../platform/d3drm.h"

#include "../core/Errors.h"
#include "../core/Files.h"
#include "../core/Memory.h"
#include "../util/Dxbug.h"
#include "Bmp.h"
#include "DirectDraw.h"

#include "Images.h"


/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @00534908>
Gods98::Image_Globs & Gods98::imageGlobs = *(Gods98::Image_Globs*)0x00534908;

Gods98::Image_ListSet Gods98::imageListSet = Gods98::Image_ListSet(Gods98::imageGlobs);

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @0047d6d0>
void __cdecl Gods98::Image_Initialise(void)
{
	log_firstcall();

	if (imageGlobs.flags & Image_GlobFlags::IMAGE_GLOB_FLAG_INITIALISED) Error_Fatal(true, "Images already initialised");

	imageListSet.Initialise();
	imageGlobs.flags = Image_GlobFlags::IMAGE_GLOB_FLAG_INITIALISED;
}

// <LegoRR.exe @0047d6f0>
void __cdecl Gods98::Image_Shutdown(void)
{
	log_firstcall();

	Image_RemoveAll();

	imageListSet.Shutdown();
	imageGlobs.flags = Image_GlobFlags::IMAGE_GLOB_FLAG_NONE;
}

// <LegoRR.exe @0047d730>
void __cdecl Gods98::Image_Remove(Image* dead)
{
	log_firstcall();

	Image_CheckInit();
	Error_Fatal(!dead, "NULL passed to Image_Remove()");

	dead->surface->Release();

	imageListSet.Remove(dead);
}

// <LegoRR.exe @0047d750>
bool32 __cdecl Gods98::Image_CopyToDataToSurface(IDirectDrawSurface4* surface, BMP_Image* image)
{
	log_firstcall();

	DDSURFACEDESC2 desc = { 0 };
	desc.dwSize = sizeof(desc);

	if (surface->Lock(nullptr, &desc, DDLOCK_WAIT | DDLOCK_WRITEONLY, nullptr) == DD_OK) {
		switch (image->depth) {
		case 8:
			Image_8BitSourceCopy(&desc, image);
			break;
		case 16:
			Error_Warn(true, "Image_CopyToDataToSurface: 16-bit source image not supported, expected 8-bit or 16-bit");
			break;
		case 24:
			Image_24BitSourceCopy(&desc, image);
			break;
		case 32:
			Error_Warn(true, "Image_CopyToDataToSurface: 32-bit source image not supported, expected 8-bit or 16-bit");
			break;
		default:
			Error_Warn(true, "Image_CopyToDataToSurface: Invalid source image bit depth");
			break;
		}
		
		surface->Unlock(nullptr);

		return true;
	}
	else Error_Warn(true, "Could not lock surface");

	return false;
}

// <LegoRR.exe @0047d7e0>
bool32 __cdecl Gods98::Image_8BitSourceCopy(const DDSURFACEDESC2* desc, BMP_Image* image)
{
	log_firstcall();

	switch (desc->ddpfPixelFormat.dwRGBBitCount) {
	case 8:
		Error_Warn(true, "Image_8BitSourceCopy: 8-bit destination surface not supported, expected 16-bit");
		return false;

	case 16:
		{
			/// HARDCODED 16-BIT IMAGE HANDLING
			uint32 bmpLineLength = image->bytes_per_line;
			uint32 surfaceLineLength = ((uint32)desc->lPitch) / 2;
			const BMP_PaletteEntry* pal = image->palette; // D3DRMPALETTEENTRY

			const uint8* bmpPtr = (uint8*)image->buffer1;
			uint16* surfPtr = (uint16*)desc->lpSurface;

			uint32 rMask = desc->ddpfPixelFormat.dwRBitMask;
			uint32 gMask = desc->ddpfPixelFormat.dwGBitMask;
			uint32 bMask = desc->ddpfPixelFormat.dwBBitMask;

			uint32 rBitCount = Image_CountMaskBits(rMask);
			uint32 gBitCount = Image_CountMaskBits(gMask);
			uint32 bBitCount = Image_CountMaskBits(bMask);

			uint32 rBitShift = Image_CountMaskBitShift(rMask);
			uint32 gBitShift = Image_CountMaskBitShift(gMask);
			uint32 bBitShift = Image_CountMaskBitShift(bMask);

			for (sint32 h = 0; h < image->height; h++) {
				for (sint32 w = 0; w < image->width; w++) {
					uint8 r = pal[*bmpPtr].red;
					uint8 g = pal[*bmpPtr].green;
					uint8 b = pal[*bmpPtr].blue;
					bmpPtr++;

					// Mix the colours
					uint16 surfaceValue =
						((r >> (8 - rBitCount)) << rBitShift) |
						((g >> (8 - gBitCount)) << gBitShift) |
						((b >> (8 - bBitCount)) << bBitShift);
					*(surfPtr++) = surfaceValue;
				}
				// Do the pitch thing
				bmpPtr += bmpLineLength - image->width;
				surfPtr += surfaceLineLength - image->width;
			}
			Image_FlipSurface(desc);
		}
		return true;

	case 24:
		Error_Warn(true, "Image_8BitSourceCopy: 24-bit destination surface not supported, expected 16-bit");
		return false;
	case 32:
		Error_Warn(true, "Image_8BitSourceCopy: 32-bit destination surface not supported, expected 16-bit");
		return false;
	default:
		/// FIX SANITY: Return false on invalid/unexpected bitCount
		Error_Warn(true, "Image_8BitSourceCopy: Invalid destination surface bit depth");
		return false;
	}
}

// <LegoRR.exe @0047d9c0>
uint32 __cdecl Gods98::Image_CountMaskBits(uint32 mask)
{
	log_firstcall();

	uint32 count = 0;

	for (uint32 i = 0; i < (sizeof(mask) * 8); i++) {
		if (mask & (1 << i)) count++;
	}
	return count;
}

// <LegoRR.exe @0047d9e0>
uint32 __cdecl Gods98::Image_CountMaskBitShift(uint32 mask)
{
	log_firstcall();

	for (uint32 i = 0; i < (sizeof(mask) * 8); i++) {
		if (mask & (1 << i)) return i;
	}
	return 0xffffffffU;
}

// <LegoRR.exe @0047da00>
void __cdecl Gods98::Image_FlipSurface(const DDSURFACEDESC2* desc)
{
	log_firstcall();

	/// HARDCODED 16-BIT IMAGE HANDLING
	uint32 pixelsPerLine = desc->lPitch / 2;
	uint32 height = desc->dwHeight;
	uint16* start = (uint16*)desc->lpSurface;
	uint16* end = (uint16*)desc->lpSurface + ((height - 1) * pixelsPerLine);
	uint16* temp = (uint16*)Mem_Alloc(sizeof(uint16) * pixelsPerLine);

	for (uint32 h = 0; h < desc->dwHeight / 2; h++) {

		std::memcpy(temp, start, sizeof(uint16) * pixelsPerLine);
		std::memcpy(start, end, sizeof(uint16) * pixelsPerLine);
		std::memcpy(end, temp, sizeof(uint16) * pixelsPerLine);
		start += pixelsPerLine;
		end -= pixelsPerLine;
	}
	Mem_Free(temp);
}

// <LegoRR.exe @0047dac0>
bool32 __cdecl Gods98::Image_24BitSourceCopy(const DDSURFACEDESC2* desc, BMP_Image* image)
{
	log_firstcall();

	switch (desc->ddpfPixelFormat.dwRGBBitCount) {
	case 8:
		Error_Warn(true, "Image_24BitSourceCopy: 8-bit destination surface not supported, expected 16-bit");
		return false;

	case 16:
		{
			/// HARDCODED 16-BIT IMAGE HANDLING
			// Not convinced that this is correct as it does 
			// not round up properly unless we have even sizes.
			uint32 bmpLineLength = image->bytes_per_line;
			uint32 surfaceLineLength = ((uint32)desc->lPitch) / 2;

			const uint8* bmpPtr =  (uint8*)image->buffer1;
			uint16* surfPtr = (uint16*)desc->lpSurface;

			uint32 rMask = desc->ddpfPixelFormat.dwRBitMask;
			uint32 gMask = desc->ddpfPixelFormat.dwGBitMask;
			uint32 bMask = desc->ddpfPixelFormat.dwBBitMask;

			uint32 rBitCount = Image_CountMaskBits(rMask);
			uint32 gBitCount = Image_CountMaskBits(gMask);
			uint32 bBitCount = Image_CountMaskBits(bMask);

			uint32 rBitShift = Image_CountMaskBitShift(rMask);
			uint32 gBitShift = Image_CountMaskBitShift(gMask);
			uint32 bBitShift = Image_CountMaskBitShift(bMask);

			for (sint32 h = 0; h < image->height; h++) {
				for (sint32 w = 0; w < image->width; w++) {
					uint8 b = *bmpPtr++;
					uint8 g = *bmpPtr++;
					uint8 r = *bmpPtr++;

					// Mix the colours
					uint16 surfaceValue =
						((r >> (8 - rBitCount)) << rBitShift) |
						((g >> (8 - gBitCount)) << gBitShift) |
						((b >> (8 - bBitCount)) << bBitShift);
					*(surfPtr++) = surfaceValue;
				}
				// Do the pitch thing
				bmpPtr += bmpLineLength - (image->width*3);
				surfPtr += surfaceLineLength - image->width;
			}
			Image_FlipSurface(desc);
		}
		return true;

	case 24:
		Error_Warn(true, "Image_24BitSourceCopy: 24-bit destination surface not supported, expected 16-bit");
		return false;
	case 32:
		Error_Warn(true, "Image_24BitSourceCopy: 32-bit destination surface not supported, expected 16-bit");
		return false;
	default:
		/// FIX SANITY: Return false on invalid/unexpected bitCount
		Error_Warn(true, "Image_24BitSourceCopy: Invalid destination surface bit depth");
		return false;
	}
}

// <LegoRR.exe @0047dc90>
Gods98::Image* __cdecl Gods98::Image_LoadBMPScaled(const char* fileName, uint32 width, uint32 height)
{
	log_firstcall();

	BMP_Image image = { 0 };  // D3DRMIMAGE
	char name[1024];

	COLORREF penZero, pen255;
	penZero = pen255 = Image_RGB2CR(0, 0, 0);

	uint32 size;
	void* buffer;
	if (buffer = File_LoadBinary(fileName, &size)) {

		BMP_Parse(buffer, size, &image);
		if (!image.rgb) {
			penZero = Image_RGB2CR(image.palette[0].red, image.palette[0].green, image.palette[0].blue);
			pen255 = Image_RGB2CR(image.palette[255].red, image.palette[255].green, image.palette[255].blue);
		}

	}
	else return nullptr;

	std::sprintf(name, "Data\\%s", fileName);

	DDSURFACEDESC2 ddsd = { 0 };
	std::memset(&ddsd, 0, sizeof(ddsd));

	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
	ddsd.dwWidth = image.width;
	ddsd.dwHeight = image.height;

	IDirectDrawSurface4* surface = nullptr;
	if (DirectDraw()->CreateSurface(&ddsd, &surface, nullptr) == DD_OK) {

		if (Image_CopyToDataToSurface(surface, &image)) {

			Image* newImage;
			if (newImage = Image_Create(surface, image.width, image.height, penZero, pen255)) {
				BMP_Cleanup(&image);

				if (buffer) Mem_Free(buffer);
				return newImage;
			}
			else Error_Warn(true, "Could not create image");
			
		}
		else Error_Warn(true, "Could not copy image to surface");

	}
	else Error_Warn(true, "Could not create surface");

	if (surface) surface->Release();
	BMP_Cleanup(&image);
	if (buffer) Mem_Free(buffer);
	return nullptr;
}

// <LegoRR.exe @0047de50>
COLORREF __cdecl Gods98::Image_RGB2CR(uint8 r, uint8 g, uint8 b)
{
	log_firstcall();

	COLORREF cr;
	uint8* ptr = (uint8*)&cr;
	ptr[0] = r;
	ptr[1] = g;
	ptr[2] = b;
	ptr[3] = 0;
	return cr;
}

// <LegoRR.exe @0047de80>
void __cdecl Gods98::Image_SetPenZeroTrans(Image* image)
{
	log_firstcall();

	Error_Fatal(!image, "NULL passed as image to Image_SetupTrans()");

	DDCOLORKEY ColourKey;
	ColourKey.dwColorSpaceLowValue = image->penZero;
	ColourKey.dwColorSpaceHighValue = image->penZero;
	
	image->surface->SetColorKey(DDCKEY_SRCBLT, &ColourKey);
	image->flags |= ImageFlags::IMAGE_FLAG_TRANS;
}

// <LegoRR.exe @0047deb0>
void __cdecl Gods98::Image_SetupTrans(Image* image, real32 lowr, real32 lowg, real32 lowb, real32 highr, real32 highg, real32 highb)
{
	log_firstcall();

	Error_Fatal(!image, "NULL passed as image to Image_SetupTrans()");

	COLORREF low = Image_RGB2CR((uint8)(lowr*255), (uint8)(lowg*255), (uint8)(lowb*255));
	COLORREF high = Image_RGB2CR((uint8)(highr*255), (uint8)(highg*255), (uint8)(highb*255));

	DDCOLORKEY ColourKey;
	ColourKey.dwColorSpaceLowValue = Image_DDColorMatch(image->surface, low);
	ColourKey.dwColorSpaceHighValue = Image_DDColorMatch(image->surface, high);
	
	image->surface->SetColorKey(DDCKEY_SRCBLT, &ColourKey);
	image->flags |= ImageFlags::IMAGE_FLAG_TRANS;
}

// <LegoRR.exe @0047df70>
void __cdecl Gods98::Image_DisplayScaled(Image* image, const Area2F* src, const Point2F* destPos, const Point2F* destSize)
{
	// function taken from: `if (!(image->flags & ImageFlags::IMAGE_FLAG_TEXTURE))` of `Image_DisplayScaled2`

	log_firstcall();

	Error_Fatal(!image, "NULL passed as image to Image_DisplayScaled()");

	RECT r_src, r_dst;

	if (src) {
		r_src.left = (sint32)src->x;
		r_src.top  = (sint32)src->y;
		r_src.right  = (sint32)(src->x + src->width);
		r_src.bottom = (sint32)(src->y + src->height);
	}

	if (destPos) {
/*
		if (destPos->x < 0) destPos->x = appWidth()  + destPos->x;
		if (destPos->y < 0) destPos->y = appHeight() + destPos->y;
*/
		//r_dst.left = (uint32)destPos->x;
		//r_dst.top  = (uint32)destPos->y;
		r_dst.left = (sint32)destPos->x;
		r_dst.top  = (sint32)destPos->y;
		if (destSize) {
			r_dst.right  = (sint32)(destPos->x + destSize->x);
			r_dst.bottom = (sint32)(destPos->y + destSize->y);
		}
		else if (src) {
			r_dst.right  = (sint32)(destPos->x + src->width);
			r_dst.bottom = (sint32)(destPos->y + src->height);
		}
		else {
			r_dst.right  = (sint32)(destPos->x + image->width);
			r_dst.bottom = (sint32)(destPos->y + image->height);
		}
	}

	uint32 flags = DDBLT_WAIT;
	if (image->flags & ImageFlags::IMAGE_FLAG_TRANS) flags |= DDBLT_KEYSRC;
	DirectDraw_bSurf()->Blt((destPos)?&r_dst:nullptr, image->surface, (src)?&r_src:nullptr, flags, nullptr);
}

// <LegoRR.exe @0047e120>
void* __cdecl Gods98::Image_LockSurface(Image* image, OUT uint32* pitch, OUT uint32* bpp)
{
	log_firstcall();

	DDSURFACEDESC2 desc;
	std::memset(&desc, 0, sizeof(DDSURFACEDESC2));
	desc.dwSize = sizeof(DDSURFACEDESC2);

	if (image->surface->Lock(nullptr, &desc, DDLOCK_WAIT, nullptr) == DD_OK) {
		*pitch = desc.lPitch;
		*bpp = desc.ddpfPixelFormat.dwRGBBitCount;
		return desc.lpSurface;
	}

	return nullptr;
}

// <LegoRR.exe @0047e190>
void __cdecl Gods98::Image_UnlockSurface(Image* image)
{
	log_firstcall();

	image->surface->Unlock(nullptr);
}

// <LegoRR.exe @0047e1b0>
colour32 __cdecl Gods98::Image_GetPen255(Image* image)
{
	log_firstcall();

	DDPIXELFORMAT pf;
	uint32 pen = 0;

	std::memset(&pf, 0, sizeof(DDPIXELFORMAT));
	pf.dwSize = sizeof(DDPIXELFORMAT);

	if (image->surface->GetPixelFormat(&pf) == DD_OK) {
//		if (pf.dwRGBBitCount == 8) return 0xff000000;
		uint8* s = (uint8*)&pen;
		uint8* t = (uint8*)&image->pen255;
		s[0] = t[3];
		s[1] = t[2];
		s[2] = t[1];
		s[3] = t[0];
//		return image->pen255 << (32 - pf.dwRGBBitCount);
		return pen;
	}

	return 0;
}

// <LegoRR.exe @0047e210>
uint32 __cdecl Gods98::Image_GetPixelMask(Image* image)
{
	log_firstcall();

	DDPIXELFORMAT pf;
	std::memset(&pf, 0, sizeof(DDPIXELFORMAT));
	pf.dwSize = sizeof(DDPIXELFORMAT);

	if (image->surface->GetPixelFormat(&pf) == DD_OK) {
		// This seems to be used for A<RGB/BGR> order,
		// where A is the least-significant byte,
		// and the RGB channels cover the most significant bytes(?)
		// Mask for a 24-bit colour would be  0xffffff00
		// Mask for a 16-bit colour would be  0xffff0000
		// See Image_GetPen255 and Font_Load

		return 0xffffffff << (32 - pf.dwRGBBitCount);
	}
	return 0;
}

// <LegoRR.exe @0047e260>
bool32 __cdecl Gods98::Image_GetPixel(Image* image, uint32 x, uint32 y, OUT colour32* colour)
{
	log_firstcall();

	DDSURFACEDESC2 desc;
	std::memset(&desc, 0, sizeof(DDSURFACEDESC2));
	desc.dwSize = sizeof(DDSURFACEDESC2);

	if (x < image->width && y < image->height) {
		if (image->surface->Lock(nullptr, &desc, DDLOCK_WAIT|DDLOCK_READONLY, nullptr) == DD_OK) {
			uint32 bpp = desc.ddpfPixelFormat.dwRGBBitCount;
			uint32 col = *((uint32*)&((uint8*)desc.lpSurface)[(y * desc.lPitch) + (x * (bpp / 8))]);
 			*colour = col >> (32 - bpp);
			image->surface->Unlock(nullptr);
			return true;
		}
	}

	return false;
}

// <LegoRR.exe @0047e310>
Gods98::Image* __cdecl Gods98::Image_Create(IDirectDrawSurface4* surface, uint32 width, uint32 height, COLORREF penZero, COLORREF pen255)
{
	log_firstcall();

	Image_CheckInit();

	Image* newImage = imageListSet.Add();

	newImage->flags = ImageFlags::IMAGE_FLAG_NONE;
	newImage->width = width;
	newImage->height = height;
	newImage->surface = surface;
	newImage->penZeroRGB = penZero;
	newImage->penZero = Image_DDColorMatch(surface, penZero);
	newImage->pen255 = Image_DDColorMatch(surface, pen255);

	return newImage;
}

// <LegoRR.exe @0047e380>
void __cdecl Gods98::Image_AddList(void)
{
	// NOTE: This function is no longer called, imageListSet.Add already handles this.
	imageListSet.AddList();
}

// <LegoRR.exe @0047e3f0>
void __cdecl Gods98::Image_RemoveAll(void)
{
	log_firstcall();

	for (Image* image : imageListSet.EnumerateAlive()) {
		Image_Remove(image);
	}
}

// <LegoRR.exe @0047e450>
uint32 __cdecl Gods98::Image_DDColorMatch(IDirectDrawSurface4* pdds, uint32 rgb)
{
	log_firstcall();

	uint32 dw = 0;

	DDSURFACEDESC2 ddsd = { 0 };
	ddsd.dwSize = sizeof(ddsd);

	uint8 r, g, b;
	Image_CR2RGB(rgb, &r, &g, &b);

	if (pdds->Lock(nullptr, &ddsd, DDLOCK_WAIT, nullptr) == DD_OK) {
		uint32 rMask = ddsd.ddpfPixelFormat.dwRBitMask;
		uint32 gMask = ddsd.ddpfPixelFormat.dwGBitMask;
		uint32 bMask = ddsd.ddpfPixelFormat.dwBBitMask;

		uint32 rBitCount = Image_CountMaskBits(rMask);
		uint32 gBitCount = Image_CountMaskBits(gMask);
		uint32 bBitCount = Image_CountMaskBits(bMask);

		uint32 rBitShift = Image_CountMaskBitShift(rMask);
		uint32 gBitShift = Image_CountMaskBitShift(gMask);
		uint32 bBitShift = Image_CountMaskBitShift(bMask);

		dw = ((r >> (8 - rBitCount)) << rBitShift) |
			 ((g >> (8 - gBitCount)) << gBitShift) |
			 ((b >> (8 - bBitCount)) << bBitShift);

		if (ddsd.ddpfPixelFormat.dwRGBBitCount < 32) {
			dw &= (1 << ddsd.ddpfPixelFormat.dwRGBBitCount) - 1;
		}

		pdds->Unlock(nullptr);
	}

	return dw;
}

// <LegoRR.exe @0047e590>
void __cdecl Gods98::Image_CR2RGB(COLORREF cr, OPTIONAL OUT uint8* r, OPTIONAL OUT uint8* g, OPTIONAL OUT uint8* b)
{
	log_firstcall();

	uint8* ptr = (uint8*)&cr;
	if (r) *r = ptr[0];
	if (g) *g = ptr[1];
	if (b) *b = ptr[2];
}

// <LegoRR.exe @0047e5c0>
void __cdecl Gods98::Image_GetScreenshot(OUT Image* image, uint32 xsize, uint32 ysize)
{
	log_firstcall();

	HRESULT hr;
	IDirectDrawSurface4* surf;
	// Create surface
	DDSURFACEDESC2 desc;
	std::memset(&desc, 0, sizeof(DDSURFACEDESC2));
	desc.dwSize = sizeof(DDSURFACEDESC2);
	desc.dwWidth = xsize;
	desc.dwHeight = ysize;
	desc.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
	desc.dwFlags = DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS;

	if ((hr = DirectDraw()->CreateSurface(&desc, &surf, nullptr)) != DD_OK) {
		CHKDD(hr);
		return;
	}

	// Blit back buffer onto here.
	{
		RECT dest = {
			0, 0,
			(sint32)xsize,
			(sint32)ysize,
		};
		surf->Blt(&dest, DirectDraw_bSurf(), nullptr, 0, nullptr);
	}
	// Create image
	Image_InitFromSurface(image, surf, xsize, ysize, 0, 0);
}

// <LegoRR.exe @0047e6a0>
void __cdecl Gods98::Image_InitFromSurface(Image* newImage, IDirectDrawSurface4* surface,
								uint32 width, uint32 height, COLORREF penZero, COLORREF pen255)
{
	log_firstcall();

	Image_CheckInit();

	// This serves no purpose, as only existing images (or images stored outside the listSet) would be passed here.
	//if (imageGlobs.freeList == nullptr) imageListSet.AddList();
	
	/// FIXME: Figure out how best to handle re-assignment of nextFree here.
	///        Are there any checks where this could cause issues if left as is or if changed?
	///        How should items not stored in the listSet have this field assigned?
	newImage->nextFree = newImage;

	newImage->flags = ImageFlags::IMAGE_FLAG_NONE;
	newImage->width = width;
	newImage->height = height;
	newImage->surface = surface;
	newImage->penZeroRGB = penZero;
	newImage->penZero = Image_DDColorMatch(surface, penZero);
	newImage->pen255 = Image_DDColorMatch(surface, pen255);
}

// <LegoRR.exe @0047e700>
bool32 __cdecl Gods98::Image_SaveBMP(Image* image, const char* fname)
{
	log_firstcall();

	return DirectDraw_SaveBMP(image->surface, fname);
}


// <missing>
void __cdecl Gods98::Image_GetPenZero(const Image* image, OPTIONAL OUT real32* r, OPTIONAL OUT real32* g, OPTIONAL OUT real32* b)
{
	log_firstcall();

	/// FIX GODS98: Include masking for red channel
	if (r) *r = (real32)((image->penZeroRGB >> 16) & 0xff) / 255.0f;
	if (g) *g = (real32)((image->penZeroRGB >>  8) & 0xff) / 255.0f;
	if (b) *b = (real32)((image->penZeroRGB)       & 0xff) / 255.0f;
}
