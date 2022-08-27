// Fonts.cpp : 
//

#include "../../platform/windows.h"
#include "../../platform/ddraw.h"

#include "../core/Errors.h"
#include "../core/Memory.h"
#include "DirectDraw.h"
#include "Images.h"

#include "Fonts.h"


/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @00507528>
Gods98::Font_Globs & Gods98::fontGlobs = *(Gods98::Font_Globs*)0x00507528; // (no init)

Gods98::Font_ListSet Gods98::fontListSet = Gods98::Font_ListSet(Gods98::fontGlobs);

Gods98::Font_TT_Globs Gods98::fontTTGlobs = { 0 };

Gods98::Font_TT_ListSet Gods98::fontTTListSet = Gods98::Font_TT_ListSet(Gods98::fontTTGlobs);
//Gods98::Font_TT_ListSet Gods98::fontTTListSet = Gods98::Font_TT_ListSet();

static std::map<std::string, Gods98::Font_TT*> _fontNameLookup;
static std::map<uintptr_t, Gods98::Font_TT*> _fontTTLookup;

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

namespace Gods98
{

static Gods98::Font_TT* Font_TT_Load(const char* fname, Gods98::Font* bitmapFont)
{
	std::string name = fname;
	for (size_t i = 0; i < name.length(); i++) {
		name[i] = std::tolower(name[i]);
	}
	if (_fontNameLookup.find(name) == _fontNameLookup.end()) {
		_fontNameLookup[name] = fontTTListSet.Add();
	}
	if (fontListSet.EnumerateAlive().Count() >= 24) {
		uint32 i = 0;
		Error_InfoF("[LOADED FONTS]: %i", (uint32)_fontNameLookup.size());
		for (auto& it : _fontNameLookup) {
			Error_InfoF("[%i] = \"%s\"", i++, it.first.c_str());
		}
	}

	Font_TT* fontTT = fontTTListSet.Add();
	ListSet::MemZero(fontTT); // Clear memory before returning.

	Font_GDI_Init(fontTT, bitmapFont);
	fontTT->flags = (Font_TTFlags::FONT_TT_FLAG_TRUETYPE);// |Font_TTFlags::FONT_TT_FLAG_BITMAPFALLBACK);
	//fontTT->flags = (Font_TTFlags::FONT_TT_FLAG_TRUETYPE|Font_TTFlags::FONT_TT_FLAG_BITMAPFALLBACK);
	fontTT->flags |= (Font_TTFlags::FONT_TT_FLAG_BITMAPFALLBACK);
	//fontTT->flags |= (Font_TTFlags::FONT_TT_FLAG_VARIABLEWIDTH);

	if (name == "interface\\fonts\\desktopfont.bmp") { // Tall narrow yellow (shaded)

		fontTTListSet.Remove(fontTT);
		fontTT = nullptr;

	}
	else if (name == "interface\\fonts\\font5_hi.bmp") { // Small faded bright lime bold
		fontTT->weight = FW_NORMAL;
		fontTT->height = -13;
		fontTT->fixedWidth = 12;
		fontTT->foreColour = 0x00d50c; // bright green
	}
	else if (name == "interface\\fonts\\fsfont.bmp") {       // Tall thin wide black/white(?)

		fontTTListSet.Remove(fontTT);
		fontTT = nullptr;

	}
	else if (name == "interface\\fonts\\mbrieffont.bmp") {   // Tall thin wide dark green
		fontTT->weight = FW_NORMAL;
		fontTT->height = -13;
		fontTT->fixedWidth = 12;
		fontTT->foreColour = 0x007500; // dark green
	}
	else if (name == "interface\\fonts\\mbrieffont2.bmp") {  // Tall bold wide medium-bright green
		fontTT->weight = FW_NORMAL;
		fontTT->height = -13;
		fontTT->fixedWidth = 12;
		fontTT->foreColour = 0x00d50c; // bright green
	}
	else if (name == "interface\\fonts\\rsfont.bmp") {       // Bold green (shaded)
		
		fontTTListSet.Remove(fontTT);
		fontTT = nullptr;

	}
	else if (name == "interface\\fonts\\rswritten.bmp") {    // Black large (0-9 and %)

		fontTTListSet.Remove(fontTT);
		fontTT = nullptr;

	}
	else if (name == "interface\\fonts\\tooltipfont.bmp") {  // Small bold white
		fontTT->weight = FW_NORMAL;
		fontTT->height = -13;
		fontTT->fixedWidth = 12;
		fontTT->foreColour = 0xffffff;
	}
	else if (name == "interface\\frontend\\menu_font_hi.bmp") { // Gigantic bold yellow (shaded)
		fontTT->weight = FW_BOLD;
		fontTT->height = -32;
		fontTT->fixedWidth = 26;
		fontTT->useBackColour = true;
		fontTT->backOffset = Point2I { 1, 1 };
		fontTT->foreColour = 0x20ffff; // bright yellow
		fontTT->backColour = 0x404040; // dark gray
	}
	else if (name == "interface\\frontend\\menu_font_lo.bmp") { // Gigantic bold dark cyan (shaded)
		fontTT->weight = FW_BOLD;
		fontTT->height = -32;
		fontTT->fixedWidth = 26;
		fontTT->useBackColour = true;
		fontTT->backOffset = Point2I { 1, 1 };
		fontTT->foreColour = 0x738200; // dark cyan
		fontTT->backColour = 0xc4c4c4; // bright gray
	}
	else {
		fontTTListSet.Remove(fontTT);
		fontTT = nullptr;
	}

	_fontTTLookup[(uintptr_t)bitmapFont] = fontTT;
	return fontTT;
	//Font_TT* newFontTT = fontTTListSet.Add();
	//ListSet::MemZero(newFontTT); // Clear memory before returning.

	//Font_GDI_Init(newFontTT, bitmapFont);
	//return newFontTT;
}

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

				Font_TT_Load(fname, font);
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
	const uint32 width = std::vsprintf(line, fmtLine, args);

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
			uintptr_t addr = 0;
			for (uint32 j = 0; j < 8; j++) {
				addr |= ((uchar8)line[i+4+j] - (std::isdigit((uchar8)line[i+4+j]) ? '0' : ('a'-10))) << (28 - (j * 4));
			}
			
			Image* image;
			if (image = (Image*)addr) {
				const Point2F pos = {
					(real32)(x + xPos),
					(real32)y,
				};

				if (line[i] != FONT_SPECIALCHARACTER) {
					xPos += image->width;
				}

				if (image->height > yIncrease) yIncrease = image->height;
				Image_Display(image, &pos);
			}
			i += 12;

		}
		else {
			const uint32 fontWidth = Font_OutputChar(font, x + xPos, y, line[i], render);

			if (line[i] != FONT_SPECIALCHARACTER) {
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
		if (render && c != ' ') Image_DisplayScaled(font->image, &font->posSet[gx][gy], &pos, nullptr);
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

static const char g_GDI_fontFaceName[] = { '\xb1', '\xbc', '\xb8', '\xb2', '\0' };

static const bool g_GDI_specialCharCheck = true;

/*namespace Gods98
{



HFONT __cdecl Font_GDI_CreateHFont(const char* faceName, sint32 height, sint32 width, sint32 escapement, sint32 orientation, sint32 weight,
								   bool italic, bool underline, bool strikeOut,
								   uint8 charSet, uint8 outPrecision, uint8 clipPrecision, uint8 quality, uint8 pitchAndFamily);

void __cdecl Font_GDI_Init(OUT Font_TT* fontTT, Font* font);
bool32 __cdecl Font_GDI_CreateFont(Font_TT* fontTT);
bool32 __cdecl Font_GDI_DestroyFont(Font_TT* fontTT);

uint32 __cdecl Font_GDI_VPrintF2(Font_TT* fontTT, sint32 x, sint32 y, bool32 render,
								 OPTIONAL OUT uint32* lineCount, const char* msg, std::va_list args);

uint32 __cdecl Font_GDI_VPrintF2_White(Font* font, sint32 x, sint32 y, bool32 render,
									   OPTIONAL OUT uint32* lineCount, const char* msg, std::va_list args);

uint32 __cdecl Font_GDI_VPrintF2_Green(Font* font, sint32 x, sint32 y, bool32 render, bool32 darkOrBright,
									   OPTIONAL OUT uint32* lineCount, const char* msg, std::va_list args);

uint32 __cdecl Font_GDI_VPrintF2_Large(Font* font, sint32 x, sint32 y, bool32 render, bool32 cyanOrYellow,
									   OPTIONAL OUT uint32* lineCount, const char* msg, std::va_list args);

//bool32 __cdecl Font_OutputChar_GDI(HFONT hGDIFont, const char* codePoint, sint32 x, sint32 y, colour32 foreColour,
//								  OPTIONAL const colour32* backColour, OPTIONAL const Point2I* backOffset);

uint32 __cdecl Font_GDI_OutputCodePoint(Font_TT* fontTT, sint32 x, sint32 y, const char* codePoint, bool32 render);
}*/

/*HFONT __cdecl Gods98::Font_GDI_CreateHFont(const char* faceName, sint32 height, sint32 width, sint32 escapement, sint32 orientation, sint32 weight,
										   bool italic, bool underline, bool strikeOut,
										   uint8 charSet, uint8 outPrecision, uint8 clipPrecision, uint8 quality, uint8 pitchAndFamily)
{
	return ::CreateFontA(height, width, escapement, orientation, weight, italic, underline, strikeOut,
						 charSet, outPrecision, clipPrecision, quality, pitchAndFamily, faceName);
}*/



bool Gods98::Font_GDI_HasUnsupportedChars(const Font_TT* fontTT, const char* text)
{
	for (const char* s = text; *s != '\0'; s++) {
		if (!Font_GDI_IsSupportedChar(fontTT, *s) || Font_GDI_IsLeadByteChar(fontTT, *s))
			return true;
	}
	return false;
}
bool Gods98::Font_GDI_HasUnsupportedChars(const Font_TT* fontTT, const char* text, size_t size)
{
	uint32 i = 0;
	for (const char* s = text; i < size && *s != '\0'; i++, s++) {
		if (!Font_GDI_IsSupportedChar(fontTT, *s) || Font_GDI_IsLeadByteChar(fontTT, *s))
			return true;
	}
	return true;// false;
}

void Gods98::Font_GDI_Init(OUT Font_TT* fontTT, const Font* bitmapFont)
{
	ListSet::MemZero(fontTT);

	fontTT->bitmapFont = bitmapFont;
	fontTT->flags |= (Font_TTFlags::FONT_TT_FLAG_TRUETYPE|Font_TTFlags::FONT_TT_FLAG_BITMAPFALLBACK);
	fontTT->faceName = g_GDI_fontFaceName;
	fontTT->weight = FW_NORMAL;
	// Surprisingly not JOHAB_CHARSET.
	fontTT->charset = HANGUL_CHARSET;
	fontTT->codePage = 949;
	fontTT->outPrecision = OUT_STROKE_PRECIS;
	//fontTT->outPrecision = OUT_TT_PRECIS;
	fontTT->clipPrecision = CLIP_STROKE_PRECIS;
	fontTT->quality = DRAFT_QUALITY;
	fontTT->pitchAndFamily = (FF_MODERN|VARIABLE_PITCH);
}

bool Gods98::Font_GDI_IsLeadByteChar(const Font_TT* fontTT, char c)
{
	return ((fontTT->flags & Font_TTFlags::FONT_TT_FLAG_TRUETYPE) &&
			fontTT->codePage != 0 && ::IsDBCSLeadByteEx(fontTT->codePage, (uchar8)c));
			//fontTT->codePage != 0 && (((uchar8)c & 0x80) || ::IsDBCSLeadByteEx(fontTT->codePage, (uchar8)c)));
}

bool Gods98::Font_GDI_IsSupportedChar(const Font_TT* fontTT, char c)
{
	return (!(fontTT->flags & Font_TTFlags::FONT_TT_FLAG_TRUETYPE) ||
			(!((uchar8)c & 0x80) && (fontTT->flags & Font_TTFlags::FONT_TT_FLAG_BITMAPFALLBACK)));
}

bool Gods98::Font_GDI_CreateFont(Font_TT* fontTT)
{
	log_firstcall();

	fontTT->fontRefCount++;

	if (fontTT->hFont == nullptr) {
		fontTT->fontRefCount = 1;
		fontTT->hFont = ::CreateFontA(fontTT->height, fontTT->width, fontTT->escapement, fontTT->orientation, fontTT->weight,
									   fontTT->italic, fontTT->underline, fontTT->strikethrough,
									   fontTT->charset, fontTT->outPrecision, fontTT->clipPrecision, fontTT->quality, fontTT->pitchAndFamily,
									   fontTT->faceName);

		Error_Fatal((fontTT->hFont == nullptr), "Failed to create GDI font.");
		return true;
	}
	return false;
}

bool Gods98::Font_GDI_DestroyFont(Font_TT* fontTT)
{
	log_firstcall();

	Error_Fatal((fontTT->fontRefCount <= 0), "Attempting to release Font_TT hFont with zero or less references.");
	fontTT->fontRefCount--;

	if (fontTT->hFont != nullptr && fontTT->fontRefCount <= 0) {
		fontTT->fontRefCount = 0;
		::DeleteObject((HFONT)fontTT->hFont);

		fontTT->hFont = nullptr;
		return true;
	}
	return false;
}

bool Gods98::Font_GDI_IsFontReady(const Font_TT* fontTT)
{
	log_firstcall();

	return fontTT->hFont != nullptr;
}

bool Gods98::Font_GDI_LockSurface(Font_TT* fontTT)
{
	log_firstcall();

	fontTT->dcRefCount++;

	if (fontTT->hDC == nullptr) {
		fontTT->dcRefCount = 1;

		HRESULT hr = DirectDraw_bSurf()->GetDC((HDC*)&fontTT->hDC);
		Error_Fatal((hr != DD_OK), "Failed to lock GDI DC surface.");
		fontTT->hSelect = ::SelectObject((HDC)fontTT->hDC, fontTT->hFont);
		//Error_Fatal((fontTT->hSelect == nullptr), "Failed to select GDI DC font.");

		::SetBkMode((HDC)fontTT->hDC, TRANSPARENT);
		
		if (!fontTT->useBackColour || (fontTT->foreColour == fontTT->backColour)) {
			// Set this ahead of time since it'll only be needed once.
			::SetTextColor((HDC)fontTT->hDC, fontTT->foreColour);
		}
		return true;
	}
	return false;
}

bool Gods98::Font_GDI_UnlockSurface(Font_TT* fontTT)
{
	log_firstcall();

	Error_Fatal((fontTT->dcRefCount <= 0), "Attempting to release Font_TT DC with zero or less references.");
	fontTT->dcRefCount--;

	if (fontTT->hDC != nullptr && fontTT->dcRefCount <= 0) {
		fontTT->dcRefCount = 0;

		::SelectObject((HDC)fontTT->hDC, fontTT->hSelect);
		DirectDraw_bSurf()->ReleaseDC((HDC)fontTT->hDC);

		fontTT->hDC = nullptr;
		fontTT->hSelect = nullptr;
		return true;
	}
	return false;
}

bool Gods98::Font_GDI_IsLocked(const Font_TT* fontTT)
{
	log_firstcall();

	return fontTT->hDC != nullptr;
}
Gods98::Font_TT* Gods98::Font_TT_Find(const Font* bitmapFont)
{
	auto it = _fontTTLookup.find((uintptr_t)const_cast<Font*>(bitmapFont));
	if (it != _fontTTLookup.end()) {
		return it->second;
	}
	return nullptr;
	/*if (!= _fontTTLookup.end()) {
		return _fontTTLookup[(uintptr_t)bitmapFont];
	}
	return nullptr;*/
}

uint32 __cdecl Gods98::Font_GDI_OutputCodePoint(Font_TT* fontTT, sint32 x, sint32 y, const char* codePoint, bool32 render)
{
	log_firstcall();

	const uint32 length = (uint32)std::strlen(codePoint);
	if (render) {
		Error_Fatal(!Font_GDI_IsFontReady(fontTT), "Attempting to output codepoint for Font_TT without created font.");
		Error_Fatal(!Font_GDI_IsLocked(fontTT), "Attempting to output codepoint for Font_TT without locked surface.");

		// Add references to these if the user hasn't handled it.
		Font_GDI_CreateFont(fontTT);
		Font_GDI_LockSurface(fontTT);

		const bool multipleColours = (fontTT->useBackColour && (fontTT->foreColour != fontTT->backColour));

		if (fontTT->useBackColour) {
			const Point2I off = fontTT->backOffset;

			if (multipleColours) ::SetTextColor((HDC)fontTT->hDC, fontTT->backColour);
			RECT backRect = { x + off.x, y + off.y, 0, 0 };
			//::DrawTextA((HDC)fontTT->hDC, codePoint, -1, &backRect, DT_LEFT | DT_NOCLIP | DT_SINGLELINE);
			::TextOutA((HDC)fontTT->hDC, x + off.x, y + off.y, codePoint, length);
		}
		// Only need to set this when using fore and back colours.
		if (multipleColours) ::SetTextColor((HDC)fontTT->hDC, fontTT->foreColour);
		RECT foreRect = { x, y, 0, 0 };
		//::DrawTextA((HDC)fontTT->hDC, codePoint, -1, &foreRect, DT_LEFT | DT_NOCLIP | DT_SINGLELINE);
		::TextOutA((HDC)fontTT->hDC, x, y, codePoint, length);

		Font_GDI_UnlockSurface(fontTT);
		Font_GDI_DestroyFont(fontTT);

		/*HDC hdc;
		if (DirectDraw_bSurf()->GetDC(&hdc) == DD_OK) {
			HGDIOBJ hGDISel = ::SelectObject(hdc, fontTT->hFont);
			::SetBkMode(hdc, TRANSPARENT);

			if (fontTT->useBackColour) {
				const Point2I off = fontTT->backOffset;

				::SetTextColor(hdc, fontTT->backColour);
				::TextOutA(hdc, x + off.x, y + off.y, codePoint, length);
			}

			::SetTextColor(hdc, fontTT->foreColour);
			::TextOutA(hdc, x, y, codePoint, length);

			::SelectObject(hdc, hGDISel);
			DirectDraw_bSurf()->ReleaseDC(hdc);
		}*/
	}
	/// TODO: divide by two for single-byte characters?
	if (fontTT->flags & Font_TTFlags::FONT_TT_FLAG_VARIABLEWIDTH) {
#if false
		Font_GDI_CreateFont(fontTT);
		//while (Font_GDI_IsLocked(fontTT)) Font_GDI_UnlockSurface(fontTT);
		Font_GDI_LockSurface(fontTT);
		/*uint32 totalWidth = 0;
		for (uint32 i = 0; i < length; i++) {
			uint32 ch = (uint8)codePoint[i];
			ABC abc = { 0 }; // dummy inits
			int width = 0;
			if (::GetCharABCWidthsA((HDC)fontTT->hDC, ch, ch, &abc))
				width = abc.abcA + abc.abcB + abc.abcC;
			else if (!::GetCharWidth32A((HDC)fontTT->hDC, ch, ch, &width))
				Error_FatalF(true, "GetCharABCWidthsA and GetCharWidth32A failed for codepoint length %i, chars: %02x %02x (idx=%i)", length, (uint32)(uchar8)codePoint[0], (uint32)(uchar8)codePoint[1], i);
			totalWidth += width;
		}*/
		
		wchar_t wch[4] = { L'\0' };
		/*uint32 ch = 0;
		if (length == 1) {
			ch = (uchar8)codePoint[0];
		}
		else if (length == 2) {
			//ch = (uchar8)codePoint[0];
			ch = ((uchar8)codePoint[0] << 8) | (uchar8)codePoint[1];
			//ch = ((uchar8)codePoint[1] << 8) | (uchar8)codePoint[0];
			
		}*/
		if (!::MultiByteToWideChar(fontTT->codePage, 0, codePoint, length, wch, _countof(wch)))
			Error_FatalF(true, "MultiByteToWideChar failed for codepoint length %i, chars: %02x %02x", length, (uint32)(uchar8)codePoint[0], (uint32)(uchar8)codePoint[1]);

		/*ABC abc = { 0 }; // dummy inits
		int totalWidth = 0;
		if (::GetCharABCWidthsW((HDC)fontTT->hDC, wch[0], wch[0], &abc)) {
			totalWidth = abc.abcA + abc.abcB + abc.abcC;
			//totalWidth =            abc.abcB + abc.abcC;
			//totalWidth = abc.abcA + abc.abcB;
			//totalWidth =            abc.abcB;
		}
		else if (!::GetCharWidth32W((HDC)fontTT->hDC, wch[0], wch[0], &totalWidth))
			Error_FatalF(true, "GetCharABCWidthsA and GetCharWidth32A failed for codepoint length %i, chars: %02x %02x", length, (uint32)(uchar8)codePoint[0], (uint32)(uchar8)codePoint[1]);
		totalWidth += GetTextCharacterExtra((HDC)fontTT->hDC);*/
		/*ABC abc = {0}; // dummy inits
		int totalWidth = 0;
		if (::GetCharABCWidthsA((HDC)fontTT->hDC, ch, ch, &abc))
			totalWidth = abc.abcA + abc.abcB + abc.abcC;
		else if (!::GetCharWidth32A((HDC)fontTT->hDC, ch, ch, &totalWidth))
			Error_FatalF(true, "GetCharABCWidthsA and GetCharWidth32A failed for codepoint length %i, chars: %02x %02x", length, (uint32)(uchar8)codePoint[0], (uint32)(uchar8)codePoint[1]);
		*/
		SIZE totalWidth = { 0 }; // dummy init
		if (!::GetTextExtentPoint32W((HDC)fontTT->hDC, wch, ::wcslen(wch), &totalWidth))
			Error_FatalF(true, "GetTextExtentPoint32W failed for codepoint length %i, chars: %02x %02x", length, (uint32)(uchar8)codePoint[0], (uint32)(uchar8)codePoint[1]);
		
		totalWidth.cx += GetTextCharacterExtra((HDC)fontTT->hDC);
		Font_GDI_UnlockSurface(fontTT);
		Font_GDI_DestroyFont(fontTT);
		return totalWidth.cx;
#endif
	}
	return fontTT->fixedWidth; // * length;
}

uint32 __cdecl Gods98::Font_GDI_VPrintF2_White(const Font* font, sint32 x, sint32 y, bool32 render,
											   OPTIONAL OUT uint32* lineCount, const char* msg, std::va_list args)
{
	Font_TT* fontTT = Font_TT_Find(font);
	if (fontTT == nullptr) {
		return Font_VPrintF2(font, x, y, render, lineCount, msg, args);
	}
	return Font_GDI_VPrintF2(fontTT, x, y, render, lineCount, msg, args);

	/*return Font_VPrintF2(font, x, y, render, lineCount, msg, args);
	Font_TT* fontTT = fontTTListSet.Add();
	Font_GDI_Init(fontTT, font);
	fontTT->flags = (Font_TTFlags::FONT_TT_FLAG_TRUETYPE|Font_TTFlags::FONT_TT_FLAG_BITMAPFALLBACK);
	fontTT->weight = FW_NORMAL;
	fontTT->height = -13;
	fontTT->fixedWidth = 12;
	fontTT->foreColour = 0xffffff;

	uint32 r = Font_GDI_VPrintF2(fontTT, x, y, render, lineCount, msg, args);
	fontTTListSet.Remove(fontTT);
	return r;*/
}

uint32 __cdecl Gods98::Font_GDI_VPrintF2_Green(const Font* font, sint32 x, sint32 y, bool32 render, bool32 darkOrBright,
											   OPTIONAL OUT uint32* lineCount, const char* msg, std::va_list args)
{
	Font_TT* fontTT = fontTTListSet.Add();
	Font_GDI_Init(fontTT, font);
	fontTT->flags = (Font_TTFlags::FONT_TT_FLAG_TRUETYPE|Font_TTFlags::FONT_TT_FLAG_BITMAPFALLBACK);
	fontTT->weight = FW_NORMAL;
	fontTT->height = -13;
	fontTT->fixedWidth = 12;
	if (!darkOrBright) {
		fontTT->foreColour = 0x007500; // dark green
	}
	else {
		fontTT->foreColour = 0x0cd500; // bright green
	}

	uint32 r = Font_GDI_VPrintF2(fontTT, x, y, render, lineCount, msg, args);
	fontTTListSet.Remove(fontTT);
	return r;
}

uint32 __cdecl Gods98::Font_GDI_VPrintF2_Large(const Font* font, sint32 x, sint32 y, bool32 render, bool32 cyanOrYellow,
											   OPTIONAL OUT uint32* lineCount, const char* msg, std::va_list args)
{
	Font_TT* fontTT = fontTTListSet.Add();
	Font_GDI_Init(fontTT, font);
	fontTT->flags = (Font_TTFlags::FONT_TT_FLAG_TRUETYPE|Font_TTFlags::FONT_TT_FLAG_BITMAPFALLBACK);
	fontTT->weight = FW_BOLD;
	fontTT->height = -32;
	fontTT->fixedWidth = 26;
	fontTT->useBackColour = true;
	fontTT->backOffset = Point2I { 1, 1 };
	if (!cyanOrYellow) {
		fontTT->foreColour = 0x008273; // dark cyan
		fontTT->backColour = 0xc4c4c4; // bright gray
	}
	else {
		fontTT->foreColour = 0xffff20; // bright yellow
		fontTT->backColour = 0x404040; // dark gray
	}

	uint32 r = Font_GDI_VPrintF2(fontTT, x, y, render, lineCount, msg, args);
	fontTTListSet.Remove(fontTT);
	return r;
}

uint32 __cdecl Gods98::Font_GDI_VPrintF2(Font_TT* fontTT, sint32 x, sint32 y, bool32 render,
										 OPTIONAL OUT uint32* lineCount, const char* msg, std::va_list args)
{
	log_firstcall();

	char line[FONT_MAXSTRINGLEN], fmtLine[FONT_MAXSTRINGLEN];
	const char* s;
	char* t;

	const Font* bitmapFont = fontTT->bitmapFont;
	const bool trueType = (fontTT->flags & Font_TTFlags::FONT_TT_FLAG_TRUETYPE);
	const bool variableWidth = (fontTT->flags & Font_TTFlags::FONT_TT_FLAG_VARIABLEWIDTH);
	//const bool fallback = !trueType || (fontTT->flags & Font_TTFlags::FONT_TT_FLAG_BITMAPFALLBACK);

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

	uint32 xPos = 0, xMax = 0, yIncrease = fontTT->bitmapFont->fontHeight;
	uint32 lines = 1;
	const uint32 width = std::vsprintf(line, fmtLine, args);

	bool useGDI = false; // For multi-byte and non-ASCII characters.
	char codePoint[4] = { '\0' }; // dummy init
	const bool useTT = Font_GDI_HasUnsupportedChars(fontTT, line) || variableWidth;

	if ((render || variableWidth) && useTT) {
		Font_GDI_CreateFont(fontTT);
		//Font_GDI_LockSurface(fontTT);
	}

	for (uint32 i = 0; i < width; i++) {
		const char c = line[i];

		if (c == '\n') {

			if (xPos > xMax) xMax = xPos;
			xPos = 0;
			y += yIncrease;
			yIncrease = bitmapFont->fontHeight;
			lines++;
		}
		else if (c == '\t') {

			xPos += bitmapFont->tabWidth - (xPos % bitmapFont->tabWidth);
		}
		else if (i < width - 12 && line[i] == '@' && line[i+1] == '[' && line[i+2] == '0' && line[i+3] == 'x' && line[i+12] == ']') {
			if (Font_GDI_IsLocked(fontTT)) Font_GDI_UnlockSurface(fontTT);

			// Parse hex address @[0x<address>]
			uintptr_t addr = 0;
			for (uint32 j = 0; j < (sizeof(uintptr_t)*2); j++) {
				addr |= ((uchar8)line[i+4+j] - (std::isdigit((uchar8)line[i+4+j]) ? '0' : ('a'-10))) << (28 - (j * 4));
			}
			
			Image* image;
			if (image = (Image*)addr) {
				const Point2F pos = {
					(real32)(x + xPos),
					(real32)y,
				};

				/// DIFFERENCE: No special character check in Korean version.
				if (trueType || c != FONT_SPECIALCHARACTER) {
					xPos += image->width;
				}

				if (image->height > yIncrease) yIncrease = image->height;
				Image_Display(image, &pos);
			}
			i += 12;
		}
		else if (!useGDI && Font_GDI_IsLeadByteChar(fontTT, c) /*&& c != FONT_SPECIALCHARACTER*/) {
			// Multibyte / non-ASCII character, use GDI for drawing.
			useGDI = true;
			codePoint[0] = c;
			codePoint[1] = '\0'; // Null-terminate in-case this is the last character.
		}
		else if ((useGDI || !Font_GDI_IsSupportedChar(fontTT, c)) /*&& c != FONT_SPECIALCHARACTER*/) {
			// Flush multi-byte / non-ASCII characters.
			const uint32 idx = (useGDI ? 1 : 0);
			useGDI = false;
			codePoint[idx+0] = c;
			codePoint[idx+1] = '\0';
			// Don't lock the surface if we're just measuring.
			if ((render || variableWidth) && !Font_GDI_IsLocked(fontTT)) Font_GDI_LockSurface(fontTT);

			xPos += Font_GDI_OutputCodePoint(fontTT, x + xPos, y, codePoint, render);
		}
		/*else if (((uchar8)c & 0x80)) {
			// Multibyte / non-ASCII character, use GDI for drawing.
			useGDI = true;
			codePoint[0] = c;
			codePoint[1] = '\0'; // Null-terminate in-case this is the last character.
		}*/
		else {
			if (Font_GDI_IsLocked(fontTT)) Font_GDI_UnlockSurface(fontTT); // Release our hold on the surface.

			const uint32 fontWidth = Font_OutputChar(bitmapFont, x + xPos, y, c, render);

			/// DIFFERENCE: No special character check in Korean version.
			if (trueType || c != FONT_SPECIALCHARACTER) {
				xPos += fontWidth;
			}
		}
	}
	if (Font_GDI_IsLocked(fontTT)) Font_GDI_UnlockSurface(fontTT); // Release our hold on the surface.

	if ((render || variableWidth) && useTT) {
		//Font_GDI_UnlockSurface(fontTT);
		Error_Fatal(Font_GDI_IsLocked(fontTT), "Font_TT finished with selected reference count greater than zero.");
		Font_GDI_DestroyFont(fontTT);
		Error_Fatal(Font_GDI_IsFontReady(fontTT), "Font_TT font not destroyed.");
	}

	if (lineCount) *lineCount = lines;
	return ((xPos > xMax) ? xPos : xMax);
}

// <LegoRR.exe @00401b90>
uint32 __cdecl Gods98::Font_GDI_GetStringWidth_White(const Font* font, const char* msg, ...)
{
	log_firstcall();

	uint32 width;
	std::va_list args;
	va_start(args, msg);
	Font_GDI_VGetStringInfo_White(font, &width, nullptr, msg, args);
	va_end(args);
	return width;
}

// <LegoRR.exe @00401bc0>
void __cdecl Gods98::Font_GDI_GetStringInfo_White(const Font* font, OPTIONAL OUT uint32* width,
												  OPTIONAL OUT uint32* lineCount, const char* msg, ...)
{
	log_firstcall();

	std::va_list args;
	va_start(args, msg);
	Font_GDI_VGetStringInfo_White(font, width, lineCount, msg, args);
	va_end(args);
}

void __cdecl Gods98::Font_GDI_VGetStringInfo_White(const Font* font, OPTIONAL OUT uint32* width,
												   OPTIONAL OUT uint32* lineCount, const char* msg, std::va_list args)
{
	log_firstcall();

	uint32 w = Font_GDI_VPrintF2_White(font, 0, 0, false, lineCount, msg, args);
	if (width) *width = w;
}


uint32 __cdecl Gods98::Font_GDI_PrintF_White(const Font* font, sint32 x, sint32 y, const char* msg, ...)
{
	log_firstcall();

	std::va_list args;
	uint32 width;

	va_start(args, msg);
	width = Font_GDI_VPrintF2_White(font, x, y, true, nullptr, msg, args);
	va_end(args);

	return width;
}

uint32 __cdecl Gods98::Font_GDI_VPrintF_White(const Font* font, sint32 x, sint32 y, const char* msg, std::va_list args)
{
	log_firstcall();

	return Font_GDI_VPrintF2_White(font, x, y, true, nullptr, msg, args);
}


#pragma endregion
