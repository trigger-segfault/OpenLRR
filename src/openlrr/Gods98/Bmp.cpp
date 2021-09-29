// Bmp.cpp : 
//

#include "Memory.h"

#include "Bmp.h"


/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @00489ef0>
void __cdecl Gods98::BMP_Parse(const void* data, uint32 size, OUT BMP_Image* istruct)
{
	log_firstcall();

	const BMP_Header* header = (BMP_Header *)data;
	uint32 colourCount = 0;

	if (header->bits_per_pixel == 8) {
		if (!(colourCount = header->colours)) colourCount = 256;
		BMP_PaletteEntry* pal = (BMP_PaletteEntry*)Mem_Alloc(sizeof(BMP_PaletteEntry) * 256);		// Always create a table 256 long.
		std::memset(pal, 0, sizeof(BMP_PaletteEntry) * 256);
		std::memcpy(pal, ((uint8*)data) + sizeof(BMP_Header), colourCount * sizeof(BMP_PaletteEntry));
		istruct->rgb = false;
		istruct->red_mask   = 0xfc; // mask value unique to paletted image
		istruct->green_mask = 0xfc;
		istruct->blue_mask  = 0xfc;
		istruct->alpha_mask = 0xfc;
		istruct->palette_size = (sint32)colourCount;
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

	istruct->width = (sint32)header->wid;
	istruct->height = (sint32)header->hgt;
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

//	istruct->buffer1 = &((LPUCHAR) data)[sizeof(BMP_Header) + (colourCount * sizeof(BMP_PaletteEntry))];
	istruct->buffer1 = &((uint8*)data)[header->img_offset];

	istruct->buffer2 = nullptr;

	istruct->aspectx = istruct->aspecty = 1;
}

// <LegoRR.exe @0048a030>
void __cdecl Gods98::BMP_Cleanup(BMP_Image* istruct)
{
	log_firstcall();

	if (istruct->palette) Mem_Free(istruct->palette);
}

#pragma endregion
