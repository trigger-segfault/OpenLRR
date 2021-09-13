
//#include "../DirectX/D3DRM/d3drmwin.h"
#include <d3drmwin.h>

#include "Bmp.h"
#include "Memory.h"


// <LegoRR.exe @00489ef0>
void __cdecl Gods98::BMP_Parse(const void* data, uint32 size, OUT D3DRMIMAGE* istruct)
{
	log_firstcall();

	BMP_Header* header = (BMP_Header *)data;
	uint32 colourCount = 0;

	if (header->bits_per_pixel == 8) {
		if (!(colourCount = header->colours)) colourCount = 256;
		D3DRMPALETTEENTRY* pal = (D3DRMPALETTEENTRY*)Mem_Alloc(sizeof(D3DRMPALETTEENTRY) * 256);		// Always create a table 256 long.
		std::memset(pal, 0, sizeof(D3DRMPALETTEENTRY) * 256);
		std::memcpy(pal, ((char*)data) + sizeof(BMP_Header), colourCount * sizeof(D3DRMPALETTEENTRY));
		istruct->rgb = false;
		istruct->red_mask   = 0xfc;
		istruct->green_mask = 0xfc;
		istruct->blue_mask  = 0xfc;
		istruct->alpha_mask = 0xfc;
		istruct->palette_size = (int)colourCount;
		for (sint32 i = 0; i < (sint32)colourCount; i++) {
			uint8 swap = pal[i].red;								// convert palette to RGB from BGR
			pal[i].red = pal[i].blue;
			pal[i].blue = swap;
		}
		istruct->palette = pal;								// place in image structure
	}
	else {
		istruct->rgb = true;
		istruct->palette_size = 0;
		istruct->palette = nullptr;
	}

	istruct->width = (int)header->wid;
	istruct->height = (int)header->hgt;
	istruct->depth = header->bits_per_pixel;

	uint32 bpp;
	switch (istruct->depth)
	{
	case 8:  bpp = 1; break;
	case 15:
	case 16: bpp = 2; break;
	case 24: bpp = 3; break;
	case 32: bpp = 4; break;
	default: bpp = 0;
	}
	istruct->bytes_per_line = ((istruct->width * bpp) + 3) & ~3;

//	istruct->buffer1 = &((LPUCHAR) data)[sizeof(BMP_Header) + (colourCount * sizeof(D3DRMPALETTEENTRY))];
	istruct->buffer1 = &((uint8*)data)[header->img_offset];

	istruct->buffer2 = nullptr;

	istruct->aspectx = istruct->aspecty = 1;
}

// <LegoRR.exe @0048a030>
void __cdecl Gods98::BMP_Cleanup(D3DRMIMAGE* istruct)
{
	log_firstcall();

	if (istruct->palette) Mem_Free(istruct->palette);
}

