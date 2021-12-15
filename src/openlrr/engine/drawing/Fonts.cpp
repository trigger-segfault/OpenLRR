// Fonts.cpp : 
//

#include "../core/Errors.h"
#include "../core/Memory.h"
#include "Images.h"

#include "Fonts.h"


/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @00507528>
Gods98::Font_Globs & Gods98::fontGlobs = *(Gods98::Font_Globs*)0x00507528; // (no init)

Gods98::Font_ListSet Gods98::fontListSet = Gods98::Font_ListSet(Gods98::fontGlobs);

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <missing>
void __cdecl Gods98::Font_Initialise(void)
{
	log_firstcall();

	fontListSet.Initialise();
	fontGlobs.flags = Font_GlobFlags::FONT_GLOB_FLAG_INITIALISED;
}

// <missing>
void __cdecl Gods98::Font_Shutdown(void)
{
	log_firstcall();

	Font_RemoveAll();

	fontListSet.Shutdown();
	fontGlobs.flags = Font_GlobFlags::FONT_GLOB_FLAG_NONE;
}


// <LegoRR.exe @00401b90>
uint32 __cdecl Gods98::noinline(Font_GetStringWidth)(const Font* font, const char* msg, ...)
{
	log_firstcall();

	uint32 width;
	std::va_list args;
	va_start(args, msg);
	Font_VGetStringInfo(font, &width, nullptr, msg, args);
	va_end(args);
	return width;
}

// <LegoRR.exe @00401bc0>
void __cdecl Gods98::noinline(Font_GetStringInfo)(const Font* font, OPTIONAL OUT uint32* width,
								OPTIONAL OUT uint32* lineCount, const char* msg, ...)
{
	log_firstcall();

	std::va_list args;
	va_start(args, msg);
	Font_VGetStringInfo(font, width, lineCount, msg, args);
	va_end(args);
}

// <LegoRR.exe @0047a1a0>
Gods98::Font* __cdecl Gods98::Font_Load(const char* fname)
{
	log_firstcall();

	Font* font;
	Image* image;

	if (image = Image_LoadBMP(fname)) {
		if (font = Font_Create(image)) {
			Image_SetPenZeroTrans(image);
			uint32 width = Image_GetWidth(image) / FONT_GRIDWIDTH;
			uint32 height = Image_GetHeight(image) / FONT_GRIDHEIGHT;

			uint32 pen255 = Image_GetPen255(image);
			uint32 mask = Image_GetPixelMask(image);

			uint32 pitch, bpp;
			uint8* buffer;
			if (buffer = (uint8*)Image_LockSurface(image, &pitch, &bpp)) {
				for (uint32 y = 0; y < FONT_GRIDHEIGHT; y++) {
					for (uint32 x = 0; x < FONT_GRIDWIDTH; x++) {
						Area2F* pos = &font->posSet[x][y];
						pos->x = (real32)(width  * x);
						pos->y = (real32)(height * y);
						pos->width  = (real32)width;
						pos->height = (real32)height;

						// Pull back the width while the pixel on the end is pen255...

						for (uint32 xBack = width-1; xBack != 0; xBack--) {
							uint32 loc = pitch * (uint32)pos->y;			// Get the start of the line...
							loc += (uint32) (pos->x+xBack) * (bpp/8);		// Get the end of the current character	
							uint32 dw = buffer[loc] << 24;
							dw |= buffer[loc+1] << 16;
							dw |= buffer[loc+2] << 8;
							dw |= buffer[loc+3];
							if ((dw & mask) == pen255)
								pos->width--;
							else
								break;
						}
					}
				}

				font->fontHeight = (uint32)font->posSet[0][0].height;
				font->tabWidth = (uint32)font->posSet[0][0].width * 8; // 8-character tab width?

				// Clean up an return...
				Image_UnlockSurface(image);
				return font;

			}
			Font_Remove(font);
		}
		Image_Remove(image);
	}

	return nullptr;
}

// <LegoRR.exe @0047a410>
void __cdecl Gods98::Font_VGetStringInfo(const Font* font, OPTIONAL OUT uint32* width,
								OPTIONAL OUT uint32* lineCount, const char* msg, std::va_list args)
{
	log_firstcall();

	uint32 w = Font_VPrintF2(font, 0, 0, false, lineCount, msg, args);
	if (width) *width = w;
}


// <LegoRR.exe @0047a440>
uint32 __cdecl Gods98::Font_PrintF(const Font* font, sint32 x, sint32 y, const char* msg, ...)
{
	log_firstcall();

	std::va_list args;
	uint32 width;

	va_start(args, msg);
	width = Font_VPrintF2(font, x, y, true, nullptr, msg, args);
	va_end(args);

	return width;
}

// <LegoRR.exe @0047a470>
uint32 __cdecl Gods98::Font_VPrintF(const Font* font, sint32 x, sint32 y, const char* msg, std::va_list args)
{
	log_firstcall();

	return Font_VPrintF2(font, x, y, true, nullptr, msg, args);
}

// <LegoRR.exe @0047a4a0>
uint32 __cdecl Gods98::Font_VPrintF2(const Font* font, sint32 x, sint32 y, bool32 render,
									OPTIONAL OUT uint32* lineCount, const char* msg, std::va_list args)
{
	log_firstcall();

	char line[FONT_MAXSTRINGLEN], fmtLine[FONT_MAXSTRINGLEN];
	const char* s;
	char* t;

	// '%b' in the string refers to an image...
	// Change '%b' to the sequence @[0x<address>]...
	for (t = fmtLine, s = msg; *s != '\0'; s++, t++) {
		Error_Fatal(*s == '@' && s[1] == '[', "Invalid character sequence in string");
		if (*s == '%' && s[1] == 'b') {
			// t.insert("@[0x%0.8x]")
			*t++ = '@';	*t++ = '[';	*t++ = '0';	*t++ = 'x';	*t++ = '%';
			*t++ = '0';	*t++ = '.';	*t++ = '8';	*t++ = 'x';	*t = ']';
			s++;
		}
		else *t = *s;
	}
	*t = '\0';

	uint32 xPos = 0, xMax = 0, yIncrease = font->fontHeight;
	uint32 lines = 1;
	uint32 width = std::vsprintf(line, fmtLine, args);

	for (uint32 i = 0; i < width; i++) {
		if (line[i] == '\n') {
			if (xPos > xMax) xMax = xPos;
			xPos = 0;
			y += yIncrease;
			yIncrease = font->fontHeight; 
			lines++;

		}
		else if (line[i] == '\t') {
			xPos += font->tabWidth - (xPos % font->tabWidth);

		}
		else if (i < width - 12 && line[i] == '@' && line[i+1] == '[' && line[i+2] == '0' && line[i+3] == 'x' && line[i+12] == ']') {
			// Parse hex address @[0x<address>]
			uint32 addr = 0;
			for (uint32 j = 0; j < 8; j++) {
				addr |= ((uchar8)line[i+4+j] - (std::isdigit((uchar8)line[i+4+j]) ? '0' : ('a'-10))) << (28 - (j * 4));
			}
			
			Point2F pos;
			Image* image;
			if (image = (Image*)addr) {
				pos.x = (real32)(x + xPos);
				pos.y = (real32)y;

				if ((uchar8)line[i] != 203) xPos += image->width;

				if (image->height > yIncrease) yIncrease = image->height;
				Image_Display(image, &pos);
			}
			i += 12;

		}
		else {
			uint32 fontWidth = Font_OutputChar(font, x + xPos, y, line[i], render);

			if ((uchar8)line[i] != 203) {
				xPos += fontWidth;
			}
		}
	}

	if (lineCount) *lineCount = lines;
	return ((xPos > xMax) ? xPos : xMax);
}

// <LegoRR.exe @0047a730>
uint32 __cdecl Gods98::Font_OutputChar(const Font* font, sint32 x, sint32 y, char c, bool32 render)
{
	log_firstcall();

	Point2F pos = { (real32)x, (real32)y };

	uchar8 uc = (uchar8)c;

	uc -= 32; // start font characters at ' ' (skip control characters)
	uint32 gy = uc / FONT_GRIDWIDTH;
	uint32 gx = uc % FONT_GRIDWIDTH;

	if (gy < FONT_GRIDHEIGHT) {
		if (render) Image_DisplayScaled(font->image, &font->posSet[gx][gy], &pos, nullptr);
		return (uint32)font->posSet[gx][gy].width;
	}

	return 0;
}

// <LegoRR.exe @0047a7e0>
uint32 __cdecl Gods98::Font_GetCharWidth(const Font* font, char c)
{
	log_firstcall();

	return Font_OutputChar(font, 0, 0, c, false);
}

// <LegoRR.exe @0047a800>
uint32 __cdecl Gods98::Font_GetHeight(const Font* font)
{
	log_firstcall();

	return font->fontHeight;
}

// <LegoRR.exe @0047a810>
void __cdecl Gods98::Font_Remove(Font* dead)
{
	log_firstcall();

	Font_CheckInit();
	//Error_Fatal(!dead, "NULL passed to Font_Remove()");

	Image_Remove(dead->image);

	fontListSet.Remove(dead);
}

// <LegoRR.exe @0047a840>
Gods98::Font* __cdecl Gods98::Font_Create(Image* image)
{
	log_firstcall();

	Font_CheckInit();

	Font* newFont = fontListSet.Add();
	ListSet::MemZero(newFont); // Clear memory before returning.

	newFont->image = image;

	return newFont;
}

// <LegoRR.exe @0047a880>
void __cdecl Gods98::Font_AddList(void)
{
	// NOTE: This function is no longer called, fontListSet.Add already handles this.
	fontListSet.AddList();
}

/// CUSTOM:
void __cdecl Gods98::Font_RemoveAll(void)
{
	for (Font* font : fontListSet.EnumerateAlive()) {
		Font_Remove(font);
	}
}



// <missing>
void __cdecl Gods98::Font_GetBackgroundColour(const Font* font, OUT real32* r, OUT real32* g, OUT real32* b)
{
	Image_GetPenZero(font->image, r, g, b);
}

// <missing>
void __cdecl Gods98::Font_SetTabWidth(Font* font, uint32 width, bool32 spaces)
{
	if (spaces) font->tabWidth = (uint32)font->posSet[0][0].width * width;
	else font->tabWidth = width;
}

// <missing>
void __cdecl Gods98::Font_RunThroughLists(Font_RunThroughListsCallback callback, void* data)
{
	for (Font* font : fontListSet.EnumerateAlive()) {
		callback(font, data);
	}
}

/*
// <missing or inlined>
__inline uint32 __cdecl Gods98::Font_GetLineCount(Font* font, const char* msg, ...)
{
	uint32 lineCount;
	std::va_list args;
	va_start(args, msg);
	Font_VGetStringInfo(font, nullptr, &lineCount, msg, args);
	va_end(args);
	return lineCount;
}
*/

#pragma endregion
