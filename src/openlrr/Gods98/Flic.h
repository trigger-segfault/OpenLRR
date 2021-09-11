#pragma once

#include "../common.h"
#include "../Types/geometry.h"
#include "Files.h"
#include "Maths.h"


namespace Gods98
{; // !<---

/**********************************************************************************
 ******** Constants
 **********************************************************************************/

#pragma region Constants

#define FLICTIMERTICKSPERSECOND 70
#define MAXFLICS 30


#define FLICERRORSTART 3000

#define FLICFINISHED 1
#define FLICNOERROR  0
#define FLICERROR   -1

#define DIRECTVIDEOPORTAL (-2)

#pragma endregion

/**********************************************************************************
 ******** Enumerations
 **********************************************************************************/

#pragma region Enums

#define FLICRESIDE          (1<<0)
#define FLICDISK            (0<<0)
#define FLICMEMORY          (1<<0)
#define FLICLOOPING         (1<<1)
#define FLICLOOPINGON       (1<<1)
#define FLICLOOPINGOFF      (0<<1)
#define FLICCATCHUP         (1<<2)
#define FLICCATCHUPON       (1<<2)
#define FLICCATCHUPOFF      (0<<2)
#define FLICSOUND           (1<<3)
#define FLICSOUNDON         (0<<3)
#define FLICSOUNDOFF        (1<<3)

//#define FLICSETPALETTE      (1<<1)

enum
{
	FLICMODE_BYTEPERPIXEL,	//  EQU   0       ;BYTE PER PIXEL
	FLICMODE_MODEX,	//     EQU   1       ;MODEX
	FLICMODE_PLANAR,		//    EQU   2       ;PLANAR MODE
	FLICMODE_TRUECOLOR,		//   EQU   3       ;TRUE COLOR MODE
	FLICMODE_HICOLOR,	//   EQU   4       ;HI COLOR MODE
};


enum FlicError : sint32
{
     FLICINVALIDHANDLE=FLICERRORSTART,
     FLICNOMOREHANDLES,
     FLICNOTENOUGHMEMORY,
     FLICINVALIDFILE,
     FLICFILENOTFOUND,
     FLICFILEERROR,
     FLICOFFPORTAL,
     FLICFRAMEDOESNOTEXIST,
     FLICREPLAYNOTSUPPORTED,
     FLICNOMOREFRAMES,
     FLICSOUNDHEADERERROR,
};
static_assert(sizeof(FlicError) == 0x4, "");

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

/*
#define File_Open fopen
#define File_Seek fseek
#define File_Read fread
#define File_Close fclose
#define File_Tell ftell
*/

//int File_Length(lpFile fp);

struct PORTAL
{
	/*0,4*/ sint32 PWidth;
	/*4*/
};
static_assert(sizeof(PORTAL) == 0x4, "");


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
	/*12,2*/ uint16 padding1;
	/*14,4*/ uint32 next;
	/*18,4*/ uint32 frit;
	/*1c,52*/ uint8 expand[102-20 /*80*/];
	/*6e,2*/ uint16 padding2;
	/*70,4*/ sint32 NewPack;
	/*74,4*/ sint32 SoundID;
	/*78,4*/ sint32 SoundRate;
	/*7c,1*/ sint8 SoundChannels;
	/*7d,1*/ sint8 SoundBPS;
	/*7e,2*/ uint16 padding3;
	/*80,4*/ sint32 SoundChunkSize;
	/*84,2*/ sint16 SoundNumPreRead;
	/*86,2*/ uint16 padding4;
	/*88*/
};
static_assert(sizeof(FLICHEADERSTRUCT) == 0x88, "");


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
static_assert(sizeof(PORTAL_STRUCT) == 0x5c, "");


struct FLICLISTSTRUCT
{
    /*0,4*/ sint32 active;
	/*4,4*/ char* structhandle;
	/*8*/
};
static_assert(sizeof(FLICLISTSTRUCT) == 0x8, "");


struct FLICCHUNKSTRUCT
{
	/*0,4*/ sint32 SoundChunk;
	/*4,4*/ sint32 ImageChunk;
	/*8*/
};
static_assert(sizeof(FLICCHUNKSTRUCT) == 0x8, "");


struct FLICSTRUCT
{
	/*000,4*/ sint32 userflags;
	/*004,4*/ sint32 fsXc;
	/*008,4*/ sint32 fsYc;
	/*00c,4*/ sint32 fsXsize;
	/*010,4*/ sint32 fsYsize;
	/*014,4*/ char* rambufferhandle;
	/*018,4*/ char* destportalhandle;
	/*01c,100*/ char filename[256];
	/*11c,300*/ uint8 fsPalette256[(256*3)];
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
	/*6c8,4*/ File_Dummy* filehandle;
	/*6cc,4*/ void* fsSPtr;
	/*6d0,4*/ void* fsSource;
	/*6d4,4*/ sint32 fsDisplayMode;
	/*6d8,4*/ sint32 fsBitPlanes;
	/*6dc,4*/ sint32 fsLoadBufferSize;
	/*6e0,4*/ void* fsLoadBuffer;
	/*6e4,4*/ bool32 is15bit;
	/*6e8*/
};
static_assert(sizeof(FLICSTRUCT) == 0x6e8, "");


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
static_assert(sizeof(SOUNDARRAY) == 0x11c, "");

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @00483f40>
bool32 __cdecl Flic_Setup(const char* filename, OUT FLICSTRUCT** fsp, sint32 flags);

// <LegoRR.exe @004841c0>
bool32 __cdecl Flic_Close(FLICSTRUCT* fsp);

// <LegoRR.exe @00484220>
bool32 __cdecl Flic_LoadHeader(const char* filename, FLICSTRUCT** fsp);

// <LegoRR.exe @00484330>
bool32 __cdecl Flic_Animate(FLICSTRUCT* fsp, const Area2F* destArea, bool32 advance, bool32 trans);

// <LegoRR.exe @00484490>
FlicError __cdecl Flic_Memory(FLICSTRUCT* fsp);

// <LegoRR.exe @00484520>
FlicError __cdecl Flic_Load(FLICSTRUCT* fsp);

// <LegoRR.exe @004845e0>
FlicError __cdecl Flic_FindChunk(FLICSTRUCT* fsp);

// <LegoRR.exe @00484770>
bool32 __cdecl Flic_FrameChunk(FLICSTRUCT* fsp);

// <LegoRR.exe @004848d0>
FlicError __cdecl Flic_DoChunk(FLICSTRUCT* fsp);

// Function to load pointers for each frame of flic
// <LegoRR.exe @004849e0>
FlicError __cdecl Flic_LoadPointers(FLICSTRUCT* fsp);

// Function to load an 8 bit palette
// <LegoRR.exe @00484a90>
FlicError __cdecl Flic_LoadPalette64(FLICSTRUCT* fsp);

// <LegoRR.exe @00484b40>
bool32 __cdecl Flic_Copy(FLICSTRUCT* fsp);

// <LegoRR.exe @00484b90>
bool32 __cdecl FlicBRunDepackHiColor(FLICSTRUCT* fsp);

// <LegoRR.exe @00484c90>
bool32 __cdecl FlicBRunDepackHiColorFlic32k(FLICSTRUCT* fsp);

// <LegoRR.exe @00484de0>
bool32 __cdecl Flic_BrunDepack(FLICSTRUCT* fsp);

// (shared)
// <LegoRR.exe @00484e50>
bool32 __cdecl Flic_NERPFunc__True(int* stack); // return 1;

// <LegoRR.exe @00484e60>
void __cdecl FlicCreateHiColorTable(FLICSTRUCT* fsp);

// <LegoRR.exe @00484ec0>
bool32 __cdecl Flic_Palette256(FLICSTRUCT* fsp);

// (shared)
// <LegoRR.exe @00484f50>
void __cdecl Flic_logf_removed(const char* msg, ...); // return;

// <LegoRR.exe @00484f60>
void __cdecl FlicDeltaWordHiColor(FLICSTRUCT* fsp);

// <LegoRR.exe @00485110>
void __cdecl FlicDeltaWordHiColorFlic32k(FLICSTRUCT* fsp);

// <LegoRR.exe @004852f0>
bool32 __cdecl Flic_DeltaWord(FLICSTRUCT* fsp);

// <LegoRR.exe @00485380>
uint16 __cdecl getFlicCol(uint8 n, FLICSTRUCT* fsp);

// <LegoRR.exe @004853a0>
uint32 __cdecl Flic_GetHeight(FLICSTRUCT* fsp);


// (shared)
// <LegoRR.exe @00489a90>
uint32 __cdecl Flic_GetWidth(FLICSTRUCT* fsp);


#pragma endregion

}
