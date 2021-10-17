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

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

 // <missing>
void __cdecl Gods98::Font_Initialise(void)
{
	log_firstcall();

	for (uint32 loop = 0; loop < FONT_MAXLISTS; loop++) {
		fontGlobs.listSet[loop] = nullptr;
	}

	fontGlobs.freeList = nullptr;
	fontGlobs.listCount = 0;
	fontGlobs.flags = Font_GlobFlags::FONT_GLOB_FLAG_INITIALISED;
}

// <missing>
void __cdecl Gods98::Font_Shutdown(void)
{
	log_firstcall();

	Font_RunThroughLists(Font_RemoveCallback, nullptr);

	for (uint32 loop = 0; loop < FONT_MAXLISTS; loop++) {
		if (fontGlobs.listSet[loop]) Mem_Free(fontGlobs.listSet[loop]);
	}

	fontGlobs.freeList = nullptr;
	fontGlobs.flags = Font_GlobFlags::FONT_GLOB_FLAG_NONE;
}


// <LegoRR.exe @00401b90>
uint32 __cdecl Gods98::noinline(Font_GetStringWidth)(Font* font, const char* msg, ...)
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
void __cdecl Gods98::noinline(Font_GetStringInfo)(Font* font, OPTIONAL OUT uint32* width,
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

	if (image = Image_LoadBMP(fname)){
		if (font = Font_Create(image)){
			Image_SetPenZeroTrans(image);
			uint32 width = Image_GetWidth(image) / FONT_GRIDWIDTH;
			uint32 height = Image_GetHeight(image) / FONT_GRIDHEIGHT;

			uint32 pen255 = Image_GetPen255(image);
			uint32 mask = Image_GetPixelMask(image);

			uint32 pitch, bpp;
			uint8* buffer;
			if (buffer = (uint8*)Image_LockSurface(image, &pitch, &bpp)){
				for (uint32 y=0 ; y<FONT_GRIDHEIGHT ; y++){
					for (uint32 x=0 ; x<FONT_GRIDWIDTH ; x++){
						Area2F* pos = &font->posSet[x][y];
						pos->x = (real32) (width * x);
						pos->y = (real32) (height * y);
						pos->width = (real32) width;
						pos->height = (real32) height;

						// Pull back the width while the pixel on the end is pen255...

						for (uint32 xBack=width-1 ; xBack ; xBack--){
							uint32 loc = pitch * (uint32) pos->y;					// Get the start of the line...
							loc += (uint32) (pos->x+xBack) * (bpp/8);		// Get the end of the current character	
							uint32 dw = buffer[loc] << 24;
							dw |= buffer[loc+1] << 16;
							dw |= buffer[loc+2] << 8;
							dw |= buffer[loc+3];
							if ((dw & mask) == pen255)
								pos->width--;
							else break;
						}
					}
				}

				font->fontHeight = (uint32) font->posSet[0][0].height;
				font->tabWidth = (uint32) font->posSet[0][0].width * 8;

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

	char line[FONT_MAXSTRINGLEN], line2[FONT_MAXSTRINGLEN];
	uint32 width, loop, lines = 1;
	uint32 xPos = 0, xMax = 0, yIncrease = font->fontHeight;
	Image* image;
	const char* s;
	char* t;

	// '%b' in the string refers to an image...
	// Change '%b' to the sequence @[0x<address>]...
	for (t=line2,s=msg ; *s!='\0' ; s++,t++) {
		Error_Fatal(*s=='@'&&*(s+1)=='[', "Invalid character sequence in string");
		if (*s == '%' && *(s+1) == 'b') {
			*t++ = '@';	*t++ = '[';	*t++ = '0';	*t++ = 'x';	*t++ = '%';
			*t++ = '0';	*t++ = '.';	*t++ = '8';	*t++ = 'x';	*t = ']';
			s++;
		} else *t = *s;
	}
	*t = '\0';

	width = std::vsprintf(line, line2, args);

	for (loop=0 ; loop<width ; loop++) {
		if (line[loop] == '\n'){
			if (xPos > xMax) xMax = xPos;
			xPos = 0;
			y += yIncrease;
			yIncrease = font->fontHeight; 
			lines++;
		} else if (line[loop] == '\t') {
			xPos += font->tabWidth - (xPos % font->tabWidth);
		} else if (loop < width - 12 && line[loop] == '@' && line[loop+1] == '[' && line[loop+2] == '0' && line[loop+3] == 'x' && line[loop+12] == ']') {
			uint32 addr = 0, sub;
			Point2F pos;
			for (sub=0 ; sub<8 ; sub++) addr |= (((uchar8*)line)[loop+4+sub] - (std::isdigit(((uchar8*)line)[loop+4+sub])?'0':('a'-10))) << (28 - (sub * 4));
			if (image = (Image*) addr) {
				pos.x = (real32) (x + xPos);
				pos.y = (real32) y;

				if (((uchar8*)line)[loop] != 203)		xPos += image->width;

				if (image->height > yIncrease) yIncrease = image->height;
				Image_Display(image, &pos);
			}
			loop += 12;
		} else {
			uint32 fontWidth = Font_OutputChar(font, x + xPos, y, line[loop], render);

			if (((uchar8*)line)[loop] != 203)
			{
				xPos+=fontWidth;
			}
		}
	}
	if (lineCount) *lineCount = lines;
	return xPos>xMax?xPos:xMax;
}

// <LegoRR.exe @0047a730>
uint32 __cdecl Gods98::Font_OutputChar(const Font* font, sint32 x, sint32 y, char c, bool32 render)
{
	log_firstcall();

	Point2F pos = { (real32)x, (real32)y };

	uchar8 uc = (uchar8)c;

	uc -= 32;
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
	Error_Fatal(!dead, "NULL passed to Font_Remove()");

	Image_Remove(dead->image);

	dead->nextFree = fontGlobs.freeList;
	fontGlobs.freeList = dead;
}

// <LegoRR.exe @0047a840>
Gods98::Font* __cdecl Gods98::Font_Create(Image* image)
{
	log_firstcall();

	Font_CheckInit();

	if (fontGlobs.freeList == nullptr) Font_AddList();

	Font* newFont = fontGlobs.freeList;
	fontGlobs.freeList = newFont->nextFree;
	std::memset(newFont, 0, sizeof(Font));
	newFont->nextFree = newFont;

	newFont->image = image;

	return newFont;
}

// <LegoRR.exe @0047a880>
void __cdecl Gods98::Font_AddList(void)
{
	log_firstcall();

	Font_CheckInit();

	Error_Fatal(fontGlobs.listCount+1 >= FONT_MAXLISTS, "Run out of lists");

	uint32 count = 0x00000001 << fontGlobs.listCount;

	Font* list;
	if (list = fontGlobs.listSet[fontGlobs.listCount] = (Font*)Mem_Alloc(sizeof(Font) * count)) {

		fontGlobs.listCount++;

		for (uint32 loop=1 ; loop<count ; loop++){

			list[loop-1].nextFree = &list[loop];
		}
		list[count-1].nextFree = fontGlobs.freeList;
		fontGlobs.freeList = list;

	} else Error_Fatal(true, Error_Format("Unable to allocate %d bytes of memory for new list.\n", sizeof(Font) * count));
}




// <missing>
void __cdecl Gods98::Font_GetBackgroundColour(Font* font, OUT real32* r, OUT real32* g, OUT real32* b)
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
void __cdecl Gods98::Font_RemoveCallback(Font* font, void* data)
{
	Font_Remove(font);
}

// <missing>
void __cdecl Gods98::Font_RunThroughLists(FontRunThroughListsCallback Callback, void* data)
{
	for (uint32 list = 0; list < fontGlobs.listCount; list++) {
		if (fontGlobs.listSet[list]) {
			uint32 count = 0x00000001 << list;
			for (uint32 loop = 0; loop < count; loop++) {
				Font* tempFont;
				if (tempFont = &fontGlobs.listSet[list][loop]) {
					if (tempFont->nextFree == tempFont) {

						// This is a valid unremoved font....
						Callback(tempFont, data);

					}
				}
			}
		}
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
