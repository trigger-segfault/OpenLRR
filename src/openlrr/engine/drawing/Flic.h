// Flic.h : 
//
/// APIS: IDirectDraw4, IDirectDrawSurface4
/// DEPENDENCIES: DirectDraw, Files, (Errors, Memory)
/// DEPENDENTS: Main, TextWindow, Credits, FrontEnd, Panel, Pointer, Rewards

#pragma once

#include "../../common.h"
#include "../colour.h"
#include "../geometry.h"


/**********************************************************************************
 ******** Forward Global Namespace Declarations
 **********************************************************************************/

#pragma region Forward Declarations

struct IDirectDrawSurface4;
struct _DDSURFACEDESC2;
typedef struct _DDSURFACEDESC2			DDSURFACEDESC2, FAR* LPDDSURFACEDESC2;

#pragma endregion


namespace Gods98
{; // !<---

/**********************************************************************************
 ******** Forward Declarations
 **********************************************************************************/

#pragma region Forward Declarations

struct File; // from `engine/core/Files.h`
struct Image; // from `engine/drawing/Images.h`

#pragma endregion

/**********************************************************************************
 ******** Constants
 **********************************************************************************/

#pragma region Constants

#define FLICTIMERTICKSPERSECOND	70
#define MAXFLICS				30

#define FLICERRORSTART			3000

#define DIRECTVIDEOPORTAL		(-2)

// flic variables
#define FLIC_FIRSTBUFFER		0
#define FLIC_SECONDBUFFER		1

#pragma endregion

/**********************************************************************************
 ******** Enumerations
 **********************************************************************************/

#pragma region Enums

flags_scoped(FlicUserFlags) : sint32
{
	FLICUSER_FLAG_NONE = 0,

	FLICRESIDE     = (1<<0) /*0x1*/, // reside:mask
	FLICDISK       = (0<<0) /*0x0*/, // reside:state disk
	FLICMEMORY     = (1<<0) /*0x1*/, // reside:state memory
	FLICLOOPING    = (1<<1) /*0x2*/, // looping:mask
	FLICLOOPINGON  = (1<<1) /*0x2*/, // looping:state on
	FLICLOOPINGOFF = (0<<1) /*0x0*/, // looping:state off
	FLICCATCHUP    = (1<<2) /*0x4*/, // catchup:mask
	FLICCATCHUPON  = (1<<2) /*0x4*/, // catchup:state on
	FLICCATCHUPOFF = (0<<2) /*0x0*/, // catchup:state off
	FLICSOUND      = (1<<3) /*0x8*/, // sound:mask
	FLICSOUNDON    = (0<<3) /*0x0*/, // sound:state on
	FLICSOUNDOFF   = (1<<3) /*0x8*/, // sound:state off
};
flags_scoped_end(FlicUserFlags, 0x4);


enum_scoped(FlicMode) : sint32
{
	FLICMODE_BYTEPERPIXEL = 0, // BYTE PER PIXEL
	FLICMODE_MODEX        = 1, // MODEX
	FLICMODE_PLANAR       = 2, // PLANAR MODE
	FLICMODE_TRUECOLOR    = 3, // TRUE COLOR MODE
	FLICMODE_HICOLOR      = 4, // HI COLOR MODE
};
enum_scoped_end(FlicMode, 0x4);


enum_scoped(FlicError) : sint32
{
	FLICERROR              = -1,
	FLICNOERROR            = 0,
	FLICFINISHED           = 1,

	FLICINVALIDHANDLE      = FLICERRORSTART    /*3000*/,
	FLICNOMOREHANDLES      = FLICERRORSTART+1  /*3001*/,
	FLICNOTENOUGHMEMORY    = FLICERRORSTART+2  /*3002*/,
	FLICINVALIDFILE        = FLICERRORSTART+3  /*3003*/,
	FLICFILENOTFOUND       = FLICERRORSTART+4  /*3004*/,
	FLICFILEERROR          = FLICERRORSTART+5  /*3005*/,
	FLICOFFPORTAL          = FLICERRORSTART+6  /*3006*/,
	FLICFRAMEDOESNOTEXIST  = FLICERRORSTART+7  /*3007*/,
	FLICREPLAYNOTSUPPORTED = FLICERRORSTART+8  /*3008*/,
	FLICNOMOREFRAMES       = FLICERRORSTART+9  /*3009*/,
	FLICSOUNDHEADERERROR   = FLICERRORSTART+10 /*3010*/,
};
enum_scoped_end(FlicError, 0x4);

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

struct PORTAL
{
	/*0,4*/ sint32 PWidth;
	/*4*/
};
assert_sizeof(PORTAL, 0x4);


struct FLICHEADERSTRUCT
{
	/*00,4*/ sint32 size;
	/*04,2*/ uint16 magic;
	/*06,2*/ uint16 frames;
	/*08,2*/ uint16 width;
	/*0a,2*/ uint16 height;
	/*0c,2*/ uint16 depth;
	/*0e,2*/ uint16 flags;
	/*10,2*/ uint16 speed;
	///*12,2*/ uint16 padding1;
	/*14,4*/ uint32 next;
	/*18,4*/ uint32 frit;
	/*1c,52*/ uint8 expand[102-20 /*80*/];
	///*6e,2*/ uint16 padding2;
	/*70,4*/ sint32 NewPack;
	/*74,4*/ sint32 SoundID;
	/*78,4*/ sint32 SoundRate;
	/*7c,1*/ sint8 SoundChannels;
	/*7d,1*/ sint8 SoundBPS;
	///*7e,2*/ uint16 padding3;
	/*80,4*/ sint32 SoundChunkSize;
	/*84,2*/ sint16 SoundNumPreRead;
	///*86,2*/ uint16 padding4;
	/*88*/
};
assert_sizeof(FLICHEADERSTRUCT, 0x88);


struct PORTAL_STRUCT
{
	/*00,4*/ sint32 Active;
	/*04,4*/ sint32 Handle;
	/*08,4*/ sint32 Priority;
	/*0c,4*/ sint32 Mode;
	/*10,4*/ sint32 Flag;
	/*14,4*/ sint32 BytesPerPixel;
	/*18,4*/ sint32 PWidth;
	/*1c,4*/ sint32 PHeight;
	/*20,4*/ sint32 PWidthBytes;
	/*24,4*/ sint32 VWidth;
	/*28,4*/ sint32 VHeight;
	/*2c,4*/ sint32 VWidthBytes;
	/*30,4*/ sint32 Offset;
	/*34,4*/ sint32 SpriteHandle;                   /* Mem_Alloc handle */
	/*38,4*/ sint32 Palette;                        /* Mem_Alloc handle */
	/*3c,4*/ sint32 X;
	/*40,4*/ sint32 Y;
	/*44,4*/ sint32 MaxAnim8;
	/*48,4*/ sint32 NextAnim8;
	/*4c,4*/ sint32 MaxSprite;
	/*50,4*/ sint32 NextSprite;
	/*54,4*/ sint32 XOffset;
	/*58,4*/ sint32 YOffset;
	/*5c*/
};
assert_sizeof(PORTAL_STRUCT, 0x5c);


struct FLICLISTSTRUCT
{
    /*0,4*/ sint32 active;
	/*4,4*/ sint8* structhandle;
	/*8*/
};
assert_sizeof(FLICLISTSTRUCT, 0x8);


struct FLICCHUNKSTRUCT
{
	/*0,4*/ sint32 SoundChunk;
	/*4,4*/ sint32 ImageChunk;
	/*8*/
};
assert_sizeof(FLICCHUNKSTRUCT, 0x8);


/// NOTE: It's safe to expand the flic structure, because its guaranteed to be allocated by Flic_Setup.
struct Flic
{
	/*000,4*/ FlicUserFlags userflags;
	/*004,4*/ sint32 fsXc;
	/*008,4*/ sint32 fsYc;
	/*00c,4*/ sint32 fsXsize;
	/*010,4*/ sint32 fsYsize;
	/*014,4*/ sint8* rambufferhandle;
	/*018,4*/ sint8* destportalhandle;
	/*01c,100*/ char filename[256];
	/*11c,300*/ ColourRGBPacked fsPalette256[256];
	/*41c,200*/ uint16 fsPalette64k[256];
	/*61c,4*/ sint32 framerate;
	/*620,4*/ sint32 lastticks;
	/*624,4*/ sint32 currentframe;
	/*628,4*/ sint32 overallframe;
	/*62c,4*/ sint32 mode;
	/*630,4*/ sint32 ringframe;
	/*634,4*/ sint32 pointerposition;
	/*638,4*/ uint32 fsPitch;
	/*63c,4*/ IDirectDrawSurface4* fsSurface;
	/*640,88*/ FLICHEADERSTRUCT fsHeader;
	/*6c8,4*/ File* filehandle;
	/*6cc,4*/ void* fsSPtr;
	/*6d0,4*/ void* fsSource;
	/*6d4,4*/ FlicMode fsDisplayMode;
	/*6d8,4*/ sint32 fsBitPlanes;
	/*6dc,4*/ sint32 fsLoadBufferSize;
	/*6e0,4*/ void* fsLoadBuffer;
	/*6e4,4*/ bool32 is15bit;
	/*6e8*/
	/// EXPANSION: Extra information for playing flics in real-time in Flic_AnimateMainDeltaTime.
	real32 frameTimer;
	/// EXPANSION: Extra info for writing pixels, used while the surface is locked during Flic_Animate.
	uint32 bitDepth;
	uint32 byteDepth;
	uint32 rBits;
	uint32 gBits;
	uint32 bBits;
	uint32 aBits;
	uint32 rShift;
	uint32 gShift;
	uint32 bShift;
	uint32 aShift;
	uint32 aValue; // Pre-shifted alpha, since we use the same value for all pixels.
};
//assert_sizeof(Flic, 0x6e8);

//typedef struct Flic FLICSTRUCT; // old name


struct SOUNDARRAY
{
	/*000,100*/ void* pBuffer[64];
	/*100,8*/ void* pSample[2];
	/*108,4*/ sint32  CurrentReadBuffer;
	/*10c,4*/ sint32  CurrentWriteBuffer;
	/*110,4*/ sint32  ChunkSize;
	/*114,4*/ sint32  NumBuffers;
	/*118,4*/ sint32  SampleNum;
	/*11c*/
};
assert_sizeof(SOUNDARRAY, 0x11c);


// Union used when an image can be either static or animated.
union Image_Flic
{
	/*0,4*/ Image* image;
	/*0,4*/ Flic* flic;
	/*0,4*/ void* data; // For clearer null checking.
	/*4*/
};
assert_sizeof(Image_Flic, 0x4);

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @00483f40>
bool32 __cdecl Flic_Setup(const char* filename, OUT Flic** fsp, FlicUserFlags flags);

// <LegoRR.exe @004841c0>
bool32 __cdecl Flic_Close(Flic* fsp);

/// CUSTOM:
uint32 Flic_GetFrameCount(const Flic* fsp);

/// CUSTOM:
uint32 Flic_GetCurrentFrame(const Flic* fsp);

/// CUSTOM:
bool Flic_IsStarted(const Flic* fsp);

// <unused>
bool32 __cdecl Flic_SetFrameRate(Flic* fsp, sint32 rate);

// <unused>
sint32 __cdecl Flic_GetFrameRate(const Flic* fsp);


// <LegoRR.exe @00484220>
bool32 __cdecl Flic_LoadHeader(const char* filename, Flic** fsp);


// (debug)
// <unused>
void __cdecl flicTest(Flic* fsp);


// <LegoRR.exe @00484330>
bool32 __cdecl Flic_Animate(Flic* fsp, const Area2F* destArea, bool32 advance, bool32 trans);

/// REPLACEMENT FOR: Flic_Animate
// Animates the flic using the framerate from Main_GetDeltaTime()
// <LegoRR.exe @00484330>
bool32 __cdecl Flic_AnimateMainDeltaTime(Flic* fsp, const Area2F* destArea, bool32 advance, bool32 trans);

/// CUSTOM:
bool Flic_AnimateDeltaTime(Flic* fsp, const Area2F* destArea, bool advance, bool trans, real32 elapsed);

/// CUSTOM: destArea and trans are optional (unused) when render is false.
bool Flic_Animate2(Flic* fsp, OPTIONAL const Area2F* destArea, uint32 advanceCount, OPTIONAL bool trans, bool render);

// <LegoRR.exe @00484490>
FlicError __cdecl Flic_Memory(Flic* fsp);

// <LegoRR.exe @00484520>
FlicError __cdecl Flic_Load(Flic* fsp);

// <LegoRR.exe @004845e0>
FlicError __cdecl Flic_FindChunk(Flic* fsp);

// <LegoRR.exe @00484770>
bool32 __cdecl Flic_FrameChunk(Flic* fsp);

// <LegoRR.exe @004848d0>
FlicError __cdecl Flic_DoChunk(Flic* fsp);


// (unsused and commented out in Flic.prot)
// <unused>
sint32 __cdecl Flic_FindMaxChunk(Flic* fsp);


// Function to load pointers for each frame of flic
// <LegoRR.exe @004849e0>
FlicError __cdecl Flic_LoadPointers(Flic* fsp);

// Function to load an 8 bit palette
// <LegoRR.exe @00484a90>
FlicError __cdecl Flic_LoadPalette64(Flic* fsp);

// <LegoRR.exe @00484b40>
bool32 __cdecl Flic_Copy(Flic* fsp);


// Simply returns 100
// <unused>
sint32 __cdecl Flic_GetEventRate(void);


// return true;
// <shared internal> (see: <LegoRR.exe @00484e50>)
__inline bool32 FlicBRunDepackBytePerPixel(Flic* fsp) { return true; }


// <LegoRR.exe @00484b90>
bool32 __cdecl FlicBRunDepackHiColor(Flic* fsp);

// <LegoRR.exe @00484c90>
bool32 __cdecl FlicBRunDepackHiColorFlic32k(Flic* fsp);


// return true;
// <shared internal> (see: <LegoRR.exe @00484e50>)
__inline bool32 FlicBRunDepackHiColorFlic(Flic* fsp) { return true; }

// return 0;
// <shared internal> (see: <LegoRR.exe @00484f50>)
__inline sint32 UnpackM2(void* input, void* output) { return 0; }


// <LegoRR.exe @00484de0>
bool32 __cdecl Flic_BrunDepack(Flic* fsp);


// return true;
// <shared internal> (see: <LegoRR.exe @00484e50>)
__inline bool32 Flic_Black(Flic* fsp) { return true; }

// return true;
// <shared internal> (see: <LegoRR.exe @00484e50>)
__inline bool32 Flic_DeltaByte(Flic* fsp) { return true; }

// return true;
// <shared internal> (see: <LegoRR.exe @00484e50>)
__inline bool32 Flic_Palette64(Flic* fsp) { return true; }

// (shared, internal)
// DON'T HOOK THIS, it's internal only(?)
// <LegoRR.exe @00484e50>
//bool32 __cdecl Flic_NERPFunc__True(sint32* stack); // return 1;


// <LegoRR.exe @00484e60>
void __cdecl FlicCreateHiColorTable(Flic* fsp);

// <LegoRR.exe @00484ec0>
bool32 __cdecl Flic_Palette256(Flic* fsp);


// return 0;
// <shared internal> (see: <LegoRR.exe @00484f50>)
__inline sint32 Flic_Unpack(Flic* fsp) { return 0; }

// return;
// <shared internal> (see: <LegoRR.exe @00484f50>)
__inline void FlicCopyHiColor(Flic* fsp) { return; }

// return;
// <shared internal> (see: <LegoRR.exe @00484f50>)
__inline void FlicCopyBytePerPixel(Flic* fsp) { return; }

// return;
// <shared internal> (see: <LegoRR.exe @00484f50>)
__inline void FlicCopyHiColorFlic(Flic* fsp) { return; }

// return;
// <shared internal> (see: <LegoRR.exe @00484f50>)
__inline void FlicDeltaWordBytePerPixel(Flic* fsp) { return; }

// (shared, internal)
// DON'T HOOK THIS, it's internal only(?)
// <LegoRR.exe @00484f50>
//void __cdecl Flic_logf_removed(const char* msg, ...); // return;


// <LegoRR.exe @00484f60>
void __cdecl FlicDeltaWordHiColor(Flic* fsp);


// return;
// <shared internal> (see: <LegoRR.exe @00484f50>)
__inline void FlicDeltaWordHiColorDZ(Flic* fsp) { return; }


// <LegoRR.exe @00485110>
void __cdecl FlicDeltaWordHiColorFlic32k(Flic* fsp);


// return;
// <shared internal> (see: <LegoRR.exe @00484f50>)
__inline void FlicDeltaWordHiColorFlic(Flic* fsp) { return; }


// <LegoRR.exe @004852f0>
bool32 __cdecl Flic_DeltaWord(Flic* fsp);

// <LegoRR.exe @00485380>
uint16 __cdecl getFlicCol(uint8 n, const Flic* fsp);

/// CUSTOM: Writes a paletted pixel to the target surface (of 16-bit, 24-bit, or 32-bit).
///         Returns the pointer to the next pixel in the target surface.
uint8* _Flic_WritePixel8(const Flic* fsp, uint8* dst, uint8 index);

/// CUSTOM: Writes a 16-bit pixel to the target surface (of 16-bit, 24-bit, or 32-bit).
///         Returns the pointer to the next pixel in the target surface.
uint8* _Flic_WritePixel16(const Flic* fsp, uint8* dst, uint16 value);

/// CUSTOM: Writes RGB values to the target surface (of 16-bit, 24-bit, or 32-bit).
///         Returns the pointer to the next pixel in the target surface.
uint8* _Flic_WritePixelRGB(const Flic* fsp, uint8* dst, uint8 r, uint8 g, uint8 b);

// This function performs the same accessor,
// shared between 3 different structure types.
//  uint32 __cdecl Flic_GetWidth(Flic* fsp);
//  bool32 __cdecl AnimClone_IsLws(AnimClone* clone);
//  uint32 __cdecl Flocks_GetNumSubdata(Flocks* flocksData);
// (shared) "AnimClone_IsLws__Flic_GetWidth"
// THIS FUNCTION MUST BE HOOKED ON AN INDIVIDUAL BASIS
// There are 5 calls made to this:
//  type:Flic (Flic_GetWidth)  -> FUN_004120e0  <@004120f7>
//                                      Panel_Draw  <@0045ab17>
//                                      Pointer_DrawPointer  <@0045cfc8>
//  type:FlocksData (Flocks_GetNumSubdata) -> LegoObject_Flocks_FUN_0044bef0  <@0044bfc3>
//  type:AnimClone (AnimClone_IsLws) -> Container_FormatPartName  <@00473f60>
// <called @004120f7, 0045ab17, 0045cfc8>
// <LegoRR.exe @00489a90>
uint32 __cdecl Flic_GetWidth(const Flic* fsp);


// <LegoRR.exe @004853a0>
uint32 __cdecl Flic_GetHeight(const Flic* fsp);

#pragma endregion

}
