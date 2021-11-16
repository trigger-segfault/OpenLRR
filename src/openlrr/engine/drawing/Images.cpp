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

/*long __cdecl Gods98::Image_GetWidth(const Image* p);
long __cdecl Gods98::Image_GetHeight(const Image* p);*/


// <LegoRR.exe @0047d6d0>
void __cdecl Gods98::Image_Initialise(void)
{
	log_firstcall();

	if (imageGlobs.flags & Image_GlobFlags::IMAGE_GLOB_FLAG_INITIALISED) Error_Fatal(true, "Images already initialised");

	imageListSet.Initialise();
	/*for (uint32 loop=0 ; loop<IMAGE_MAXLISTS ; loop++){
		imageGlobs.listSet[loop] = nullptr;
	}*/

	// Global Initialisation here...

	imageGlobs.flags = Image_GlobFlags::IMAGE_GLOB_FLAG_INITIALISED;
}

// <LegoRR.exe @0047d6f0>
void __cdecl Gods98::Image_Shutdown(void)
{
	log_firstcall();

	Image_RemoveAll();

	imageListSet.Shutdown();
	/*for (uint32 loop = 0; loop < IMAGE_MAXLISTS; loop++) {
		if (imageGlobs.listSet[loop]) Mem_Free(imageGlobs.listSet[loop]);
	}

	imageGlobs.freeList = nullptr;*/
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
	/*dead->nextFree = imageGlobs.freeList;
	imageGlobs.freeList = dead;*/
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
			Error_Warn(true, "16 bit image specified - surely BMP's cant do that");
			break;
		case 24:
			Image_24BitSourceCopy(&desc, image);
			break;
		case 32:
			Error_Warn(true, "32 bit images are no fun - 8 and 24 only please");
			break;
		default:
			if (image->depth < 8) Error_Warn(true, "Please use 8/24 bit surfaces");
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

	if (desc->ddpfPixelFormat.dwRGBBitCount == 8)
	{
		Error_Warn(true, "Why have I got an 8 bit surface");
		return false;
	}
	else if (desc->ddpfPixelFormat.dwRGBBitCount == 16)
	{
		/// HARDCODED 16-BIT IMAGE HANDLING
		uint32 bmpLineLength = image->bytes_per_line;
		uint32 surfaceLineLength = ((uint32) desc->lPitch) / 2;
		const BMP_PaletteEntry* pal = image->palette; // D3DRMPALETTEENTRY

		const uint8* bmpPtr =  (uint8*) image->buffer1;
		uint16* surfPtr = (uint16*) desc->lpSurface;

		uint32 rMask = desc->ddpfPixelFormat.dwRBitMask;
		uint32 gMask = desc->ddpfPixelFormat.dwGBitMask;
		uint32 bMask = desc->ddpfPixelFormat.dwBBitMask;

		uint32 rBitCount = Image_CountMaskBits(rMask);
		uint32 gBitCount = Image_CountMaskBits(gMask);
		uint32 bBitCount = Image_CountMaskBits(bMask);

		uint32 rBitShift = Image_CountMaskBitShift(rMask);
		uint32 gBitShift = Image_CountMaskBitShift(gMask);
		uint32 bBitShift = Image_CountMaskBitShift(bMask);

		for (sint32 h=0; h<image->height; h++)
		{
			for (sint32 w=0; w<image->width; w++)
			{
				uint8 r = pal[*bmpPtr].red;
				uint8 g = pal[*bmpPtr].green;
				uint8 b = pal[*(bmpPtr++)].blue;

				// Mix the colours
				uint16 surfaceValue =
					((r >> (8 - rBitCount)) << rBitShift) |
					((g >> (8 - gBitCount)) << gBitShift) |
					((b >> (8 - bBitCount)) << bBitShift) ;
				*(surfPtr++) = surfaceValue;
			}
			// Do the pitch thing
			bmpPtr += bmpLineLength - image->width;
			surfPtr += surfaceLineLength - image->width;
		}
		Image_FlipSurface(desc);
	}
	else if (desc->ddpfPixelFormat.dwRGBBitCount == 24)
	{
		Error_Warn(true, "Why have I got an 24 bit surface");
		return false;
	}
	else if (desc->ddpfPixelFormat.dwRGBBitCount == 32)
	{
		Error_Warn(true, "Why have I got a 32 bit surface");
		return false;
	}

	return true;
}

// <LegoRR.exe @0047d9c0>
uint32 __cdecl Gods98::Image_CountMaskBits(uint32 mask)
{
	log_firstcall();

	uint32 count = 0;

	for (uint32 i = 0; i < (sizeof(mask) * 8); i++)
	{
		if (0 != (mask & (1 << i))) count++;
	}

	return count;
}

// <LegoRR.exe @0047d9e0>
uint32 __cdecl Gods98::Image_CountMaskBitShift(uint32 mask)
{
	log_firstcall();

	for (uint32 i = 0; i < (sizeof(mask) * 8); i++)
	{
		if (0 != (mask & (1 << i))) return i;
	}
	return 0xffffffffU;
}

// <LegoRR.exe @0047da00>
void __cdecl Gods98::Image_FlipSurface(const DDSURFACEDESC2* desc)
{
	log_firstcall();

	/// HARDCODED 16-BIT IMAGE HANDLING
	uint32 shortsPerLine = desc->lPitch / 2;
	uint32 height = desc->dwHeight;
	uint16* start = (uint16*)desc->lpSurface;
	uint16* end = (uint16*)desc->lpSurface + ((height - 1) * shortsPerLine);
	void* temp = Mem_Alloc(sizeof(uint16) * shortsPerLine);

	for (uint32 h = 0; h < desc->dwHeight / 2; h++)
	{
		std::memcpy(temp, start, sizeof(uint16) * shortsPerLine);
		std::memcpy(start, end, sizeof(uint16) * shortsPerLine);
		std::memcpy(end, temp, sizeof(uint16) * shortsPerLine);
		start += shortsPerLine;
		end -= shortsPerLine;
	}
	Mem_Free(temp);
}

// <LegoRR.exe @0047dac0>
bool32 __cdecl Gods98::Image_24BitSourceCopy(const DDSURFACEDESC2* desc, BMP_Image* image)
{
	log_firstcall();

	if (desc->ddpfPixelFormat.dwRGBBitCount == 8)
	{
		Error_Warn(true, "Why have I got an 8 bit surface");
		return false;
	}
	else if (desc->ddpfPixelFormat.dwRGBBitCount == 16)
	{
		// Not convinced that this is correct as it does 
		// not round up properly unless we have even sizes.
		uint32 bmpLineLength = image->bytes_per_line;
		uint32 surfaceLineLength = ((uint32) desc->lPitch) / 2;

		const uint8* bmpPtr =  (uint8*)  image->buffer1;
		uint16* surfPtr = (uint16*) desc->lpSurface;

		uint32 rMask = desc->ddpfPixelFormat.dwRBitMask;
		uint32 gMask = desc->ddpfPixelFormat.dwGBitMask;
		uint32 bMask = desc->ddpfPixelFormat.dwBBitMask;

		uint32 rBitCount = Image_CountMaskBits(rMask);
		uint32 gBitCount = Image_CountMaskBits(gMask);
		uint32 bBitCount = Image_CountMaskBits(bMask);

		uint32 rBitShift = Image_CountMaskBitShift(rMask);
		uint32 gBitShift = Image_CountMaskBitShift(gMask);
		uint32 bBitShift = Image_CountMaskBitShift(bMask);

		for (sint32 h=0; h<image->height; h++)
		{
			for (sint32 w=0; w<image->width; w++)
			{
				uint8 b = *bmpPtr++;
				uint8 g = *bmpPtr++;
				uint8 r = *bmpPtr++;

				// Mix the colours
				uint16 surfaceValue =
					((r >> (8 - rBitCount)) << rBitShift) |
					((g >> (8 - gBitCount)) << gBitShift) |
					((b >> (8 - bBitCount)) << bBitShift) ;
				*(surfPtr++) = surfaceValue;
			}
			// Do the pitch thing
			bmpPtr += bmpLineLength - (image->width*3);
			surfPtr += surfaceLineLength - image->width;
		}
		Image_FlipSurface(desc);
	}
	else if (desc->ddpfPixelFormat.dwRGBBitCount == 24)
	{
		Error_Warn(true, "Why have I got an 24 bit surface");
		return false;
	}
	else if (desc->ddpfPixelFormat.dwRGBBitCount == 32)
	{
		Error_Warn(true, "Why have I got a 32 bit surface");
		return false;
	}

	return true;
}

// <LegoRR.exe @0047dc90>
Gods98::Image* __cdecl Gods98::Image_LoadBMPScaled(const char* fileName, uint32 width, uint32 height)
{
	log_firstcall();

	void* buffer = 0;
	uint32 size = 0;
	BMP_Image image = { 0 };  // D3DRMIMAGE
	DDSURFACEDESC2 ddsd = { 0 };
	IDirectDrawSurface4* surface = 0;
	COLORREF penZero, pen255;
	char name[1024];

	penZero = pen255 = Image_RGB2CR(0, 0, 0);

	if (buffer = File_LoadBinary(fileName, &size))
	{
/*		{
#include <stdio.h>
			FILE *dumpFile = fopen("ImageLoad.txt", "a");
			fprintf(dumpFile, "Data\\%s\n", fileName);
			fclose(dumpFile);
		}*/


		BMP_Parse(buffer, size, &image);
		if (image.rgb == false) 
		{
			penZero = Image_RGB2CR(image.palette[0].red, image.palette[0].green, image.palette[0].blue);
			pen255 = Image_RGB2CR(image.palette[255].red, image.palette[255].green, image.palette[255].blue);
		}
	}
	else return nullptr;

	std::sprintf(name, "Data\\%s", fileName);

	std::memset(&ddsd, 0, sizeof(ddsd));
	//::ZeroMemory(&ddsd, sizeof(ddsd));

	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
	ddsd.dwWidth = image.width;
	ddsd.dwHeight = image.height;

	if (DirectDraw()->CreateSurface(&ddsd, &surface, nullptr) == DD_OK)
	{
		Image* newImage;

		if (Image_CopyToDataToSurface(surface, &image))
		{

			if (newImage = Image_Create(surface, image.width, image.height, penZero, pen255))
			{
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
	return 0;
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
	log_firstcall();

	// taken from: `if (!(image->flags & ImageFlags::IMAGE_FLAG_TEXTURE))` of `Image_DisplayScaled2`

	RECT r_src, r_dst;

	Error_Fatal(!image, "NULL passed as image to Image_DisplayScaled()");

	if (src){
		r_src.left = (sint32) src->x;
		r_src.top = (sint32) src->y;
		r_src.right = (sint32) (src->x + src->width);
		r_src.bottom = (sint32) (src->y + src->height);
	}

	if (destPos){
/*
		if (destPos->x < 0) destPos->x = appWidth() + destPos->x;
		if (destPos->y < 0) destPos->y = appHeight() + destPos->y;
*/
		//r_dst.left = (uint32) destPos->x;
		//r_dst.top = (uint32) destPos->y;
		r_dst.left = (sint32) destPos->x;
		r_dst.top = (sint32) destPos->y;
		if (destSize){
			r_dst.right = (sint32) (destPos->x + destSize->x);
			r_dst.bottom = (sint32) (destPos->y + destSize->y);
		} else if (src) {
			r_dst.right = (sint32) (destPos->x + src->width);
			r_dst.bottom = (sint32) (destPos->y + src->height);
		} else {
			r_dst.right = (sint32) (destPos->x + image->width);
			r_dst.bottom = (sint32) (destPos->y + image->height);
		}
	}

	if (image->flags & ImageFlags::IMAGE_FLAG_TRANS) DirectDraw_bSurf()->Blt((destPos)?&r_dst: nullptr, image->surface, (src)?&r_src: nullptr, DDBLT_KEYSRC | DDBLT_WAIT, nullptr);
	else DirectDraw_bSurf()->Blt((destPos)?&r_dst:nullptr, image->surface, (src)?&r_src:nullptr, DDBLT_WAIT, nullptr);
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
//		if (8 == pf.dwRGBBitCount) return 0xff000000;
		uint8* s = (uint8*) &pen;
		uint8* t = (uint8*) &image->pen255;
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
		return 0xffffffff << (32 - pf.dwRGBBitCount);
	}
	return 0;
}

// <LegoRR.exe @0047e260>
bool32 __cdecl Gods98::Image_GetPixel(Image* image, uint32 x, uint32 y, OUT colour32* colour)
{
	log_firstcall();

	uint32 col, bpp;
	DDSURFACEDESC2 desc;
	std::memset(&desc, 0, sizeof(DDSURFACEDESC2));
	desc.dwSize = sizeof(DDSURFACEDESC2);

	if (x < image->width && y < image->height) {
		if (image->surface->Lock(nullptr, &desc, DDLOCK_WAIT|DDLOCK_READONLY, nullptr) == DD_OK) {
			bpp = desc.ddpfPixelFormat.dwRGBBitCount;
			col = *((uint32*) &((uint8*) desc.lpSurface)[(y * desc.lPitch) + (x * (bpp / 8))]);
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

	Image* newImage = imageListSet.Add(false); // No need to memzero, all fields are assigned.
	/*if (imageGlobs.freeList == nullptr) Image_AddList();
	
	Image* newImage = imageGlobs.freeList;
	imageGlobs.freeList = newImage->nextFree;
	newImage->nextFree = newImage;*/

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
	// NOTE: This function is no longer called, imageListSet.Add already does so.
	log_firstcall();

	Image_CheckInit();

	imageListSet.AddList();
	/*Error_Fatal(imageGlobs.listCount+1 >= IMAGE_MAXLISTS, "Run out of lists");

	uint32 count = 0x00000001 << imageGlobs.listCount;

	Image* list;
	if (list = imageGlobs.listSet[imageGlobs.listCount] = (Image*)Mem_Alloc(sizeof(Image) * count)){

		imageGlobs.listCount++;

		for (uint32 loop=1 ; loop<count ; loop++){

			list[loop-1].nextFree = &list[loop];
		}
		list[count-1].nextFree = imageGlobs.freeList;
		imageGlobs.freeList = list;

	} else Error_Fatal(true, Error_Format("Unable to allocate %d bytes of memory for new list.\n", sizeof(Image) * count));*/
}

// <LegoRR.exe @0047e3f0>
void __cdecl Gods98::Image_RemoveAll(void)
{
	log_firstcall();

	for (Image* image : imageListSet.EnumerateAlive()) {
		Image_Remove(image);
	}
	/*for (uint32 list=0 ; list<imageGlobs.listCount ; list++){
		if (imageGlobs.listSet[list]){
			uint32 count = 0x00000001 << list;
			for (uint32 loop=0 ; loop<count ; loop++){
				Image* tempImage;
				if (tempImage = &imageGlobs.listSet[list][loop]){
					if (tempImage->nextFree == tempImage){

						Image_Remove(tempImage);

					}
				}
			}
		}
	}*/
}

// <LegoRR.exe @0047e450>
uint32 __cdecl Gods98::Image_DDColorMatch(IDirectDrawSurface4* pdds, uint32 rgb)
{
	log_firstcall();

	DDSURFACEDESC2 ddsd = { 0 };
	uint32 dw = 0;

	ddsd.dwSize = sizeof(ddsd);

	uint8 r, g, b;
	Image_CR2RGB(rgb, &r, &g, &b);

	if (pdds->Lock(nullptr, &ddsd, DDLOCK_WAIT, nullptr) == DD_OK)
	{
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

        if (ddsd.ddpfPixelFormat.dwRGBBitCount < 32) 
			dw &= (1 << ddsd.ddpfPixelFormat.dwRGBBitCount)-1;

		pdds->Unlock(nullptr);
	}

	return dw;
}

// <LegoRR.exe @0047e590>
void __cdecl Gods98::Image_CR2RGB(COLORREF cr, OUT uint8* r, OUT uint8* g, OUT uint8* b)
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
	desc.dwSize=sizeof(DDSURFACEDESC2);
	desc.dwWidth = xsize;
	desc.dwHeight = ysize;
	desc.ddsCaps.dwCaps=DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
	desc.dwFlags=DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS;
	if ((hr = DirectDraw()->CreateSurface(&desc, &surf, nullptr)) != DD_OK)
	{
		CHKDD(hr);
		return;
	}
	// Blit back buffer onto here.
	{
		RECT dest = {
			0, 0,
			(sint32) xsize,
			(sint32) ysize,
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

	if (imageGlobs.freeList == nullptr) Image_AddList();
	
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
void __cdecl Gods98::Image_GetPenZero(const Image* image, OUT real32* r, OUT real32* g, OUT real32* b)
{
	log_firstcall();

	/// FIXME GODS98: This should really be masking the red channel too, in-case of alpha
	if (r) *r = (real32)(image->penZeroRGB >> 16) / 255.0f;
	if (g) *g = (real32)((image->penZeroRGB >> 8) & 0xff) / 255.0f;
	if (b) *b = (real32)(image->penZeroRGB & 0xff) / 255.0f;
}
