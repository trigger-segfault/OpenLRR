

#include <ddraw.h>
#include <d3drm.h>			// For Viewports.h

#include "Memory.h"
#include "DirectDraw.h"
#include "Draw.h"
#include "Errors.h"
#include "Maths.h"
//#include "Mesh.h"
#include "Main.h"
//#include "Viewports.h"


/**********************************************************************************
 ******** Forward Global Namespace Declarations
 **********************************************************************************/

#pragma region Forward Declarations

/// TODO: Remove me once Maths module is finished
//#define Maths_Cos(a)						(real32)std::cos((double)(a))
//#define Maths_Sin(a)						(real32)std::sin((double)(a))

#pragma endregion


/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @005417e8>
Gods98::Draw_Globs & Gods98::drawGlobs = *(Gods98::Draw_Globs*)0x005417e8; // = { nullptr };

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

// <LegoRR.exe @00486140>
void __cdecl Gods98::Draw_Initialise(const Area2F* window)
{
	log_firstcall();

	drawGlobs.flags |= Draw_GlobFlags::DRAW_FLAG_INITIALISED;
	Draw_SetClipWindow(window);
}

// <LegoRR.exe @00486160>
void __cdecl Gods98::Draw_SetClipWindow(const Area2F* window)
{
	log_firstcall();

	IDirectDrawSurface4* surf = DirectDraw_bSurf();
	DDSURFACEDESC2 desc;

	Error_Fatal(!(drawGlobs.flags & Draw_GlobFlags::DRAW_FLAG_INITIALISED), "Draw not initialised");

	drawGlobs.clipStart.x = 0;
	drawGlobs.clipStart.y = 0;

	if (window) {
		if (window->x > 0) drawGlobs.clipStart.x = window->x;
		if (window->y > 0) drawGlobs.clipStart.y = window->y;
	}

	std::memset(&desc, 0, sizeof(DDSURFACEDESC2));
	desc.dwSize = sizeof(DDSURFACEDESC2);
	if (surf->GetSurfaceDesc(&desc) == DD_OK) {
		drawGlobs.clipEnd.x = (real32) desc.dwWidth;
		drawGlobs.clipEnd.y = (real32) desc.dwHeight;
		if (window) {
			if (window->x + window->width < desc.dwWidth) drawGlobs.clipEnd.x = window->x + window->width;
			if (window->y + window->height < desc.dwHeight) drawGlobs.clipEnd.y = window->y + window->height;
		}
	}

//	drawGlobs.lockRect.left = (uint32) drawGlobs.clipStart.x;
//	drawGlobs.lockRect.top = (uint32) drawGlobs.clipStart.y;
//	drawGlobs.lockRect.right = (uint32) drawGlobs.clipEnd.x;
//	drawGlobs.lockRect.bottom = (uint32) drawGlobs.clipEnd.y;
}

// <LegoRR.exe @00486270>
void __cdecl Gods98::Draw_GetClipWindow(OUT Area2F* window)
{
	log_firstcall();

	window->x = drawGlobs.clipStart.x;
	window->y = drawGlobs.clipStart.y;
	window->width = drawGlobs.clipEnd.x - drawGlobs.clipStart.x;
	window->height = drawGlobs.clipEnd.y - drawGlobs.clipStart.y;
}

// <unused>
void __cdecl Gods98::Draw_PixelListEx(const Point2F* pointList, uint32 count, real32 r, real32 g, real32 b, DrawEffect effect)
{
	log_firstcall();

	IDirectDrawSurface4* surf = DirectDraw_bSurf();

	Error_Fatal(!(drawGlobs.flags & Draw_GlobFlags::DRAW_FLAG_INITIALISED), "Draw not initialised");

	if (Draw_LockSurface(surf, effect)) {

		uint32 colour = Draw_GetColour(r, g, b);

		for (uint32 loop=0 ; loop<count ; loop++){
			const Point2F* point = &pointList[loop];

			if (point->x >= drawGlobs.clipStart.x && point->y >= drawGlobs.clipStart.y && point->x < drawGlobs.clipEnd.x && point->y < drawGlobs.clipEnd.y){
				drawGlobs.drawPixelFunc((sint32) point->x, (sint32) point->y, colour);
			}
		}
		Draw_UnlockSurface(surf);
	}
}

// <LegoRR.exe @004862b0>
void __cdecl Gods98::Draw_LineListEx(const Point2F* fromList, const Point2F* toList, uint32 count, real32 r, real32 g, real32 b, DrawEffect effect)
{
	log_firstcall();

	IDirectDrawSurface4* surf = DirectDraw_bSurf();

	Error_Fatal(!(drawGlobs.flags & Draw_GlobFlags::DRAW_FLAG_INITIALISED), "Draw not initialised");

	if (Draw_LockSurface(surf, effect)) {
		
		uint32 colour = Draw_GetColour(r, g, b);
		
		for (uint32 loop=0 ; loop<count ; loop++){
			const Point2F* from = &fromList[loop];
			const Point2F* to = &toList[loop];
			Draw_LineActual((sint32) from->x, (sint32) from->y, (sint32) to->x, (sint32) to->y, colour);
		}
		Draw_UnlockSurface(surf);
	}
}

// <unused>
//void __cdecl Gods98::Draw_WorldLineListEx(Viewport* vp, const Vector3F* fromList, const Vector3F* toList, uint32 count, real32 r, real32 g, real32 b, real32 a, DrawEffect effect)
//{
//}

// <LegoRR.exe @00486350>
void __cdecl Gods98::Draw_RectListEx(const Area2F* rectList, uint32 count, real32 r, real32 g, real32 b, DrawEffect effect)
{
	log_firstcall();

	IDirectDrawSurface4* surf = DirectDraw_bSurf();

	Error_Fatal(!(drawGlobs.flags & Draw_GlobFlags::DRAW_FLAG_INITIALISED), "Draw not initialised");

	if (Draw_LockSurface(surf, effect)) {

		uint32 colour = Draw_GetColour(r, g, b);

		for (uint32 loop=0 ; loop<count ; loop++){
			Area2F rect = rectList[loop];
			Point2F end = {
				rect.x + rect.width,
				rect.y + rect.height
			};

			if (rect.x < drawGlobs.clipStart.x) rect.x = (real32) drawGlobs.clipStart.x;
			if (rect.y < drawGlobs.clipStart.y) rect.y = (real32) drawGlobs.clipStart.y;
			if (end.x >= drawGlobs.clipEnd.x) end.x = (real32) (drawGlobs.clipEnd.x-1);
			if (end.y >= drawGlobs.clipEnd.y) end.y = (real32) (drawGlobs.clipEnd.y-1);
			sint32 ex = (sint32) end.x;
			sint32 ey = (sint32) end.y;
			for (sint32 y=(sint32)rect.y ; y<ey ; y++){
				for (sint32 x=(sint32)rect.x ; x<ex ; x++){
					drawGlobs.drawPixelFunc(x, y, colour);
				}
			}
		}
		Draw_UnlockSurface(surf);
	}
}

// <LegoRR.exe @004864d0>
void __cdecl Gods98::Draw_RectList2Ex(const Draw_Rect* rectList, uint32 count, DrawEffect effect)
{
	log_firstcall();

	IDirectDrawSurface4* surf = DirectDraw_bSurf();
	/*uint32 loop, colour;
	sint32 x, y, ex, ey;
	Area2F rect;
	Point2F end;*/

	Error_Fatal(!(drawGlobs.flags & Draw_GlobFlags::DRAW_FLAG_INITIALISED), "Draw not initialised");

	if (Draw_LockSurface(surf, effect)) {

		for (uint32 loop=0 ; loop<count ; loop++){

			Area2F rect = rectList[loop].rect;
			uint32 colour = Draw_GetColour(rectList[loop].r, rectList[loop].g, rectList[loop].b);

			Point2F end = {
				rect.x + rect.width,
				rect.y + rect.height
			};
			if (rect.x < drawGlobs.clipStart.x) rect.x = (real32) drawGlobs.clipStart.x;
			if (rect.y < drawGlobs.clipStart.y) rect.y = (real32) drawGlobs.clipStart.y;
			if (end.x >= drawGlobs.clipEnd.x) end.x = (real32) (drawGlobs.clipEnd.x-1);
			if (end.y >= drawGlobs.clipEnd.y) end.y = (real32) (drawGlobs.clipEnd.y-1);
			sint32 ex = (sint32) end.x;
			sint32 ey = (sint32) end.y;
			for (sint32 y=(sint32)rect.y ; y<ey ; y++){
				for (sint32 x=(sint32)rect.x ; x<ex ; x++){
					drawGlobs.drawPixelFunc(x, y, colour);

				}
			}
		}
		Draw_UnlockSurface(surf);
	}
}

// <LegoRR.exe @00486650>
void __cdecl Gods98::Draw_DotCircle(const Point2F* pos, uint32 radius, uint32 dots, real32 r, real32 g, real32 b, DrawEffect effect)
{
	log_firstcall();

	IDirectDrawSurface4* surf = DirectDraw_bSurf();
	real32 step = (2.0f * M_PI) / dots;

	Error_Fatal(!(drawGlobs.flags & Draw_GlobFlags::DRAW_FLAG_INITIALISED), "Draw not initialised");

	if (Draw_LockSurface(surf, effect)) {

		uint32 colour = Draw_GetColour(r, g, b);

		for (uint32 loop=0 ; loop<dots ; loop++){
			real32 angle = step * loop;
			uint32 x = (uint32) (pos->x + (Maths_Sin(angle) * radius));
			uint32 y = (uint32) (pos->y + (Maths_Cos(angle) * radius));
			if (x >= drawGlobs.clipStart.x && y >= drawGlobs.clipStart.y && x < drawGlobs.clipEnd.x && y < drawGlobs.clipEnd.y){
				drawGlobs.drawPixelFunc(x, y, colour);
			}
		}
		Draw_UnlockSurface(surf);
	}
}

// <LegoRR.exe @00486790>
uint32 __cdecl Gods98::Draw_GetColour(real32 r, real32 g, real32 b)
{
	log_firstcall();

	uint32 colour = 0;

	Error_Fatal(drawGlobs.buffer==nullptr, "Must be called after Draw_LockSurface()");
	
	if (drawGlobs.bpp == 8) {
		/// FIXME: not implemented by GODS98 or LegoRR
	} else {
		colour |= (uint32) (r * 255.0) >> (8 - drawGlobs.redBits) << (drawGlobs.greenBits + drawGlobs.blueBits);
		colour |= (uint32) (g * 255.0) >> (8 - drawGlobs.greenBits) << drawGlobs.blueBits;
		colour |= (uint32) (b * 255.0) >> (8 - drawGlobs.blueBits);
	}

	return colour;
}

// <LegoRR.exe @00486810>
bool32 __cdecl Gods98::Draw_LockSurface(IDirectDrawSurface4* surf, DrawEffect effect)
{
	log_firstcall();

	DDSURFACEDESC2 desc;
	HRESULT r;

	std::memset(&desc, 0, sizeof(DDSURFACEDESC2));
	desc.dwSize = sizeof(DDSURFACEDESC2);
//	if ((r = surf->Lock(&drawGlobs.lockRect, &desc, DDLOCK_WAIT, nullptr)) == DD_OK){
	if ((r = surf->Lock(nullptr, &desc, DDLOCK_WAIT, nullptr)) == DD_OK){


		drawGlobs.buffer = desc.lpSurface;
		drawGlobs.pitch = desc.lPitch;
		drawGlobs.redMask = desc.ddpfPixelFormat.dwRBitMask;
		drawGlobs.greenMask = desc.ddpfPixelFormat.dwGBitMask;
		drawGlobs.blueMask = desc.ddpfPixelFormat.dwBBitMask;
		drawGlobs.bpp = desc.ddpfPixelFormat.dwRGBBitCount;

		drawGlobs.redBits = drawGlobs.greenBits = drawGlobs.blueBits = 0;
		for (uint32 loop=0 ; loop<drawGlobs.bpp ; loop++) {
			if (drawGlobs.redMask & (1 << loop)) drawGlobs.redBits++;
			if (drawGlobs.greenMask & (1 << loop)) drawGlobs.greenBits++;
			if (drawGlobs.blueMask & (1 << loop)) drawGlobs.blueBits++;
		}

		if (Draw_SetDrawPixelFunc(effect)) {
			return true;
		}
		Draw_UnlockSurface(surf);
	}

	return false;
}

// <LegoRR.exe @00486910>
void __cdecl Gods98::Draw_UnlockSurface(IDirectDrawSurface4* surf)
{
	log_firstcall();

//	surf->Unlock(&drawGlobs.lockRect);
	surf->Unlock(nullptr);
	drawGlobs.buffer = nullptr;
	drawGlobs.pitch = 0;
	drawGlobs.redMask = 0;
	drawGlobs.greenMask = 0;
	drawGlobs.blueMask = 0;
	drawGlobs.drawPixelFunc = nullptr;
	drawGlobs.bpp = 0;
}

// <LegoRR.exe @00486950>
bool32 __cdecl Gods98::Draw_SetDrawPixelFunc(DrawEffect effect)
{
	log_firstcall();

	switch (drawGlobs.bpp) {
	case 8:
		drawGlobs.drawPixelFunc = Draw_Pixel8;
		break;
	case 16:
		if (effect == DrawEffect::DrawEffect_XOR) drawGlobs.drawPixelFunc = Draw_Pixel16XOR;
		else if (effect == DrawEffect::DrawEffect_HalfTrans) drawGlobs.drawPixelFunc = Draw_Pixel16HalfTrans;
		else drawGlobs.drawPixelFunc = Draw_Pixel16;
		break;
	case 24:
		drawGlobs.drawPixelFunc = Draw_Pixel24;
		break;
	case 32:
		drawGlobs.drawPixelFunc = Draw_Pixel32;
		break;

	default:
		drawGlobs.drawPixelFunc = nullptr;
		return false;
	}

	return true;

	/*if (drawGlobs.bpp == 8) {
		drawGlobs.drawPixelFunc = Draw_Pixel8;
	}
	else if (drawGlobs.bpp == 16) {
		if (DrawEffect_XOR == effect) drawGlobs.drawPixelFunc = Draw_Pixel16XOR;
		else if (DrawEffect_HalfTrans == effect) drawGlobs.drawPixelFunc = Draw_Pixel16HalfTrans;
		else drawGlobs.drawPixelFunc = Draw_Pixel16;
	}
	else if (drawGlobs.bpp == 24) {
		drawGlobs.drawPixelFunc = Draw_Pixel24;
	}
	else if (drawGlobs.bpp == 32) {
		drawGlobs.drawPixelFunc = Draw_Pixel32;
	}
	else {
		drawGlobs.drawPixelFunc = nullptr;
		return false;
	}

	return true;*/
}

// Bresenham's algorithm (line drawing)
// see: <http://www.brackeen.com/vga/shapes.html#shapes3.0>
// also see: <https://github.com/trigger-segfault/AsciiArtist/blob/75c75467d4f5fa2da02c1ba9712deb16529bbbde/PowerConsoleLib/PowerConsole/Drawing/Graphics.cpp#L459-L499>
// <LegoRR.exe @004869e0>
void __cdecl Gods98::Draw_LineActual(sint32 x1, sint32 y1, sint32 x2, sint32 y2, uint32 colour)
{
	log_firstcall();

	sint32 numpixels;
	sint32 d, dinc1, dinc2;

	// Sign of the distance
	sint32 xinc1, xinc2;
	sint32 yinc1, yinc2;

	// Absolute distance
	sint32 deltax = std::abs(x2 - x1);
	sint32 deltay = std::abs(y2 - y1);

	// different drawing behavior depending on the larger coordinate plane distance
	if (deltax >= deltay) {
		numpixels = deltax + 1;
		d = 2 * deltay - deltax;
		dinc1 = deltay << 1;
		dinc2 = (deltay - deltax) << 1;
		xinc1 = 1;
		xinc2 = 1;
		yinc1 = 0;
		yinc2 = 1;
	} else {
		numpixels = deltay + 1;
		d = (2 * deltax) - deltay;
		dinc1 = deltax << 1;
		dinc2 = (deltax - deltay) << 1;
		xinc1 = 0;
		xinc2 = 1;
		yinc1 = 1;
		yinc2 = 1;
	}
	
	if (x1>x2) {
		xinc1 = -xinc1;
		xinc2 = -xinc2;
	}
	if (y1>y2) {
		yinc1 = -yinc1;
		yinc2 = -yinc2;
	}
	sint32 x = x1;
	sint32 y = y1;
	
	for (sint32 loop=1 ; loop<=numpixels ; loop++) {
		
		if (x >= drawGlobs.clipStart.x && y >= drawGlobs.clipStart.y && x < drawGlobs.clipEnd.x && y < drawGlobs.clipEnd.y){
			drawGlobs.drawPixelFunc(x, y, colour);
		}
		
		if (d < 0) {
			d = d + dinc1;
			x = x + xinc1;
			y = y + yinc1;
		} else {
			d = d + dinc2;
			x = x + xinc2;
			y = y + yinc2;
		}
	}
}

// <LegoRR.exe @00486b40>
void __cdecl Gods98::Draw_Pixel8(sint32 x, sint32 y, uint32 value)
{
	log_firstcall();

	uint8* addr = &((uint8*)drawGlobs.buffer)[(y*drawGlobs.pitch)+x];
	*addr = (uint8) value;
}

// <LegoRR.exe @00486b60>
void __cdecl Gods98::Draw_Pixel16(sint32 x, sint32 y, uint32 value)
{
	log_firstcall();

	uint16* addr = (uint16*) &((uint8*)drawGlobs.buffer)[(y*drawGlobs.pitch)+(x*2)];
	*addr = (uint16) value;
}

// <LegoRR.exe @00486b90>
void __cdecl Gods98::Draw_Pixel16XOR(sint32 x, sint32 y, uint32 value)
{
	log_firstcall();

	uint16* addr = (uint16*) &((uint8*)drawGlobs.buffer)[(y*drawGlobs.pitch)+(x*2)];
	*addr = *addr ^((uint16) value);
}

// <LegoRR.exe @00486bc0>
void __cdecl Gods98::Draw_Pixel16HalfTrans(sint32 x, sint32 y, uint32 value)
{
	log_firstcall();

	uint16* addr = (uint16*) &((uint8*)drawGlobs.buffer)[(y*drawGlobs.pitch)+(x*2)];
	uint16 r, g, b;

	r = (((*addr >> 12) + ((uint16) value >> 12)) << 11) & (uint16) drawGlobs.redMask;
	g = ((((*addr & (uint16) drawGlobs.greenMask) >> 6) + (((uint16) value & (uint16) drawGlobs.greenMask) >> 6)) << 5) & (uint16) drawGlobs.greenMask;
	b = (((*addr & (uint16) drawGlobs.blueMask) >> 1) + (((uint16) value & (uint16) drawGlobs.blueMask) >> 1)) & (uint16) drawGlobs.blueMask;

	*addr = (r|g|b);
}

// <LegoRR.exe @00486c60>
void __cdecl Gods98::Draw_Pixel24(sint32 x, sint32 y, uint32 value)
{
	log_firstcall();

	uint32* addr = (uint32*) & ((uint8*)drawGlobs.buffer)[(y * drawGlobs.pitch) + (x * 3)];

	*addr &= 0x000000ff;
	*addr |= value << 8;
}

// <LegoRR.exe @00486c90>
void __cdecl Gods98::Draw_Pixel32(sint32 x, sint32 y, uint32 value)
{
	log_firstcall();

	uint32* addr = (uint32*) &((uint8*)drawGlobs.buffer)[(y*drawGlobs.pitch)+(x*4)];
	*addr = (uint32) value;
}

#pragma endregion
