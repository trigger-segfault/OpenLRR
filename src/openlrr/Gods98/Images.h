// Images.h : 
//
/// APIS: IDirectDraw4, IDirectDrawSurface4
/// DEPENDENCIES: Bmp, DirectDraw, Files, (Dxbug, Errors, Memory)
/// DEPENDENTS: Fonts, Bubble, FrontEnd, HelpWindow, Info, Interface, Lego, Loader,
///             NERPs, Objective, ObjInfo, Panel, Pointer, Priorities, Rewards,
///             ScrollInfo, Text, ToolTip

#pragma once

#include "../common.h"
#include "../types/geometry.h"


/**********************************************************************************
 ******** Forward Global Namespace Declarations
 **********************************************************************************/

#pragma region Forward Declarations

struct _D3DRMIMAGE;
typedef struct _D3DRMIMAGE D3DRMIMAGE;
struct IDirectDraw4;
struct IDirectDrawSurface4;
struct _DDSURFACEDESC2;
typedef struct _DDSURFACEDESC2 DDSURFACEDESC2;
struct tagPALETTEENTRY;
typedef struct tagPALETTEENTRY PALETTEENTRY;
typedef unsigned long COLORREF;
//struct PALETTEENTRY;
//enum D3DRENDERSTATETYPE : uint32;

#pragma endregion


namespace Gods98
{; // !<---

/**********************************************************************************
 ******** Forward Declarations
 **********************************************************************************/

#pragma region Forward Declarations

struct BMP_PaletteEntry;
struct BMP_Image;

#pragma endregion

/**********************************************************************************
 ******** Constants
 **********************************************************************************/

#pragma region Constants

#define IMAGE_MAXLISTS			32			// 2^32 - 1 possible images...

#pragma endregion

/**********************************************************************************
 ******** Enumerations
 **********************************************************************************/

#pragma region Enums

// used by `Image_DisplayScaled2` for
//  `IDirect3DDevice3->DrawPrimitive(D3DPT_TRIANGLESTRIP, IMAGE_VERTEXFLAGS, vertices, 4, D3DDP_DONOTLIGHT|D3DDP_WAIT)`
#define IMAGE_VERTEXFLAGS		(D3DFVF_DIFFUSE|D3DFVF_XYZRHW|D3DFVF_TEX1)


namespace _ns_Image_GlobFlags {
enum Image_GlobFlags : uint32
{
	IMAGE_FLAG_NONE        = 0,
	IMAGE_FLAG_INITIALISED = 0x1,
};
DEFINE_ENUM_FLAG_OPERATORS(Image_GlobFlags);
static_assert(sizeof(Image_GlobFlags) == 0x4, "");
} using Image_GlobFlags = _ns_Image_GlobFlags::Image_GlobFlags;


namespace _ns_ImageFlags {
enum ImageFlags : uint32
{
	IMAGE_FLAG_NONE    = 0,

	IMAGE_FLAG_TRANS   = 0x2,
	IMAGE_FLAG_TEXTURE = 0x4,
};
DEFINE_ENUM_FLAG_OPERATORS(ImageFlags);
static_assert(sizeof(ImageFlags) == 0x4, "");
} using ImageFlags = _ns_ImageFlags::ImageFlags;


enum Image_TextureMode : sint32
{
	Image_TextureMode_Normal            = 0,
	Image_TextureMode_Subtractive       = 1,
	Image_TextureMode_Additive          = 2,
	Image_TextureMode_Transparent       = 3,

	Image_TextureMode_Fixed_Subtractive = 4,	// Not affected by the global intensity value
	Image_TextureMode_Fixed_Additive    = 5,
	Image_TextureMode_Fixed_Transparent = 6,

	Image_TextureMode_Count = 7,
};
static_assert(sizeof(Image_TextureMode) == 0x4, "");

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

struct ImageVertex
{
	/*00,10*/ Vector4F position;
	/*10,4*/ uint32 colour;
	/*14,4*/ real32 tu;
	/*18,4*/ real32 tv;
	/*1c*/
};
static_assert(sizeof(ImageVertex) == 0x1c, "");


struct Image
{
	/*00,4*/ IDirectDrawSurface4* surface;
	//IDirect3DTexture2* texture;
	//D3DTEXTUREHANDLE textureHandle;
	/*04,4*/ uint32 width;
	/*08,4*/ uint32 height;
	/*0c,4*/ uint32 penZero;
	/*10,4*/ uint32 pen255;
	/*14,4*/ uint32 penZeroRGB;
	/*18,4*/ ImageFlags flags;
	/*1c,4*/ Image *nextFree;
	/*20*/
};
static_assert(sizeof(Image) == 0x20, "");


struct Image_Globs
{
	// [globs: start]
	/*00,80*/ Image* listSet[IMAGE_MAXLISTS];		// Images list
	/*80,4*/ Image* freeList;
	/*84,4*/ uint32 listCount;						// number of lists.
	/*88,4*/ Image_GlobFlags flags;
	// [globs: end]
	/*8c*/
};
static_assert(sizeof(Image_Globs) == 0x8c, "");

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @00534908>
extern Image_Globs & imageGlobs;

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

/*long __cdecl Image_GetWidth(const Image* p);
long __cdecl Image_GetHeight(const Image* p);*/


// <LegoRR.exe @0047d6d0>
void __cdecl Image_Initialise(void);

// <LegoRR.exe @0047d6f0>
void __cdecl Image_Shutdown(void);

// <LegoRR.exe @0047d730>
void __cdecl Image_Remove(Image* dead);

// <LegoRR.exe @0047d750>
bool32 __cdecl Image_CopyToDataToSurface(IDirectDrawSurface4* surface, BMP_Image* image);

// <LegoRR.exe @0047d7e0>
bool32 __cdecl Image_8BitSourceCopy(const DDSURFACEDESC2* desc, BMP_Image* image);

// <LegoRR.exe @0047d9c0>
uint32 __cdecl Image_CountMaskBits(uint32 mask);

// <LegoRR.exe @0047d9e0>
uint32 __cdecl Image_CountMaskBitShift(uint32 mask);

// <LegoRR.exe @0047da00>
void __cdecl Image_FlipSurface(const DDSURFACEDESC2* desc);

// <LegoRR.exe @0047dac0>
bool32 __cdecl Image_24BitSourceCopy(const DDSURFACEDESC2* desc, BMP_Image* image);

// <LegoRR.exe @0047dc90>
Image* __cdecl Image_LoadBMPScaled(const char* fileName, uint32 width, uint32 height);

// <LegoRR.exe @0047de50>
COLORREF __cdecl Image_RGB2CR(uint8 r, uint8 g, uint8 b);

// <LegoRR.exe @0047de80>
void __cdecl Image_SetPenZeroTrans(Image* image);

// <LegoRR.exe @0047deb0>
void __cdecl Image_SetupTrans(Image* image, real32 lowr, real32 lowg, real32 lowb, real32 highr, real32 highg, real32 highb);

// <LegoRR.exe @0047df70>
void __cdecl Image_DisplayScaled(Image* image, const Area2F* src, const Point2F* destPos, const Point2F* destSize);

// <LegoRR.exe @0047e120>
void* __cdecl Image_LockSurface(Image* image, OUT uint32* pitch, OUT uint32* bpp);

// <LegoRR.exe @0047e190>
void __cdecl Image_UnlockSurface(Image* image);

// <LegoRR.exe @0047e1b0>
colour32 __cdecl Image_GetPen255(Image* image);

// <LegoRR.exe @0047e210>
uint32 __cdecl Image_GetPixelMask(Image* image);

// <LegoRR.exe @0047e260>
bool32 __cdecl Image_GetPixel(Image* image, uint32 x, uint32 y, OUT colour32* colour);

// <LegoRR.exe @0047e310>
Image* __cdecl Image_Create(IDirectDrawSurface4* surface, uint32 width, uint32 height, COLORREF penZero, COLORREF pen255);

// <LegoRR.exe @0047e380>
void __cdecl Image_AddList(void);

// <LegoRR.exe @0047e3f0>
void __cdecl Image_RemoveAll(void);

// <LegoRR.exe @0047e450>
uint32 __cdecl Image_DDColorMatch(IDirectDrawSurface4* pdds, uint32 rgb);

// <LegoRR.exe @0047e590>
void __cdecl Image_CR2RGB(COLORREF cr, OUT uint8* r, OUT uint8* g, OUT uint8* b);

// <LegoRR.exe @0047e5c0>
void __cdecl Image_GetScreenshot(OUT Image* image, uint32 xsize, uint32 ysize);


// <LegoRR.exe @0047e6a0>
void __cdecl Image_InitFromSurface(Image* newImage, IDirectDrawSurface4* surface,
								uint32 width, uint32 height, COLORREF penZero, COLORREF pen255);

// <LegoRR.exe @0047e700>
bool32 __cdecl Image_SaveBMP(Image* image, const char* fname);


// <missing>
void __cdecl Image_GetPenZero(const Image* image, OUT real32* r, OUT real32* g, OUT real32* b);

/*Image* __cdecl Image_LoadBMPTexture(const char* filename);
void __cdecl Image_SetMainViewport(Viewport* vp);
void __cdecl Image_SetAlphaIntensity(real32 level);
void __cdecl Image_DisplayScaled2(Image* image, const Area2F* src, const Point2F* destPos, const Point2F* destSize, Image_TextureMode textureMode, real32 level, const Point2F* uvs);
void __cdecl Image_DisplayScaled(Image* image, const Area2F* src, const Point2F* destPos, const Point2F* destSize);
void* __cdecl Image_LockSurface(Image* image, uint32* pitch, uint32* bpp);
void __cdecl Image_UnlockSurface(Image* image);
int __cdecl Image_CopyBMP(IDirectDrawSurface4* pdds, void* hbm, uint32 x, uint32 y, uint32 dx, uint32 dy);

uint32 __cdecl Image_RGB2CR(unsigned char r, unsigned char g, unsigned char b);
void __cdecl Image_MyBlt(IDirectDrawSurface* dest, IDirectDrawSurface* src, uint32 sx, uint32 sy);
*/

#ifdef DEBUG
	#define Image_CheckInit()			if (!(imageGlobs.flags & Gods98::Image_GlobFlags::IMAGE_FLAG_INITIALISED)) Gods98::Error_Fatal(true, "Error: Image_Intitialise() Has Not Been Called");
#else
	#define Image_CheckInit()
#endif

#define Image_LoadBMP(n)					Image_LoadBMPScaled((n),0,0)
#define Image_Display(p,l)					Image_DisplayScaled((p),nullptr,(l),nullptr)


// <inlined>
__inline sint32 Image_GetWidth(const Image* p) { return p->width; }

// <inlined>
__inline sint32 Image_GetHeight(const Image* p) { return p->height; }

#pragma endregion

}
