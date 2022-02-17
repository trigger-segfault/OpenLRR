// Lwt.h : 
//
/// FILEIO: Shared:Data[wad,std,cd]
/// APIS: -
/// DEPENDENCIES: Files, (Errors, Memory)
/// DEPENDENTS: Mesh

#pragma once

#include "../../common.h"


namespace Gods98
{; // !<---

/**********************************************************************************
 ******** Forward Declarations
 **********************************************************************************/

#pragma region Forward Declarations

struct File; // from `engine/core/Files.h`

#pragma endregion

/**********************************************************************************
 ******** Constants
 **********************************************************************************/

#pragma region Constants

#define LBUF_LEN 256

#pragma endregion

 /**********************************************************************************
  ******** Enumerations
  **********************************************************************************/

#pragma region Enums

// srfTexFlags bit enumeration
enum LightWave_TexBits : uint32
{
	TF_AXIS_X         = 0,
	TF_AXIS_Y         = 1,
	TF_AXIS_Z         = 2,
	TF_WORLD_COORD    = 3,
	TF_NEGATIVE_IMAGE = 4,
	TF_PIXEL_BLENDING = 5,
	TF_ANTIALIASING   = 6,
	TF_SEQUENCE       = 7,
	TF_MAX            = 8,
};
flags_end(LightWave_TexBits, 0x4);


enum LightWave_TexFlags : uint32
{
	TFM_NONE           = 0,

	TFM_AXIS_X         = (1<<TF_AXIS_X)         /*0x1*/,
	TFM_AXIS_Y         = (1<<TF_AXIS_Y)         /*0x2*/,
	TFM_AXIS_Z         = (1<<TF_AXIS_Z)         /*0x4*/,
	TFM_WORLD_COORD    = (1<<TF_WORLD_COORD)    /*0x8*/,
	TFM_NEGATIVE_IMAGE = (1<<TF_NEGATIVE_IMAGE) /*0x10*/,
	TFM_PIXEL_BLENDING = (1<<TF_PIXEL_BLENDING) /*0x20*/,
	TFM_ANTIALIASING   = (1<<TF_ANTIALIASING)   /*0x40*/,
	TFM_SEQUENCE       = (1<<TF_SEQUENCE)       /*0x80*/,
};
flags_end(LightWave_TexFlags, 0x4);


// srfFlags bit enumeration
enum LightWave_SurfBits : uint32
{
    SF_LUMINOUS        = 0,
	SF_OUTLINE         = 1,
	SF_SMOOTHING       = 2,
	SF_COLORHIGHLIGHTS = 3,
	SF_COLORFILTER     = 4,
    SF_OPAQUEEDGE      = 5, //\ These two should not be set together
	SF_TRANSPARENTEDGE = 6, ///
	SF_SHARPTERMINATOR = 7,
	SF_DOUBLESIDED     = 8,
    SF_ADDITIVE        = 9,
	SF_MAX             = 10,
};
flags_end(LightWave_SurfBits, 0x4);


enum LightWave_SurfFlags : uint32
{
	SFM_NONE            = 0,

	SFM_LUMINOUS        = (1<<SF_LUMINOUS)        /*0x1*/,
	SFM_OUTLINE         = (1<<SF_OUTLINE)         /*0x2*/,
	SFM_SMOOTHING       = (1<<SF_SMOOTHING)       /*0x4*/,
	SFM_COLORHIGHLIGHTS = (1<<SF_COLORHIGHLIGHTS) /*0x8*/,
	SFM_COLORFILTER     = (1<<SF_COLORFILTER)     /*0x10*/,
	SFM_OPAQUEEDGE      = (1<<SF_OPAQUEEDGE)      /*0x20*/,
	SFM_TRANSPARENTEDGE = (1<<SF_TRANSPARENTEDGE) /*0x40*/,
	SFM_SHARPTERMINATOR = (1<<SF_SHARPTERMINATOR) /*0x80*/,
	SFM_DOUBLESIDED     = (1<<SF_DOUBLESIDED)     /*0x100*/,
	SFM_ADDITIVE        = (1<<SF_ADDITIVE)        /*0x200*/,
};
flags_end(LightWave_SurfFlags, 0x4);


enum LightWave_TexType : uint32
{
	MT_PLANAR      = 0,
	MT_CYLINDRICAL = 1,
	MT_SPHERICAL   = 2,
	MT_MAX         = 3,
};
assert_sizeof(LightWave_TexType, 0x4);

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

struct LWSURFLIST
{
	/*0,4*/ uint32 srflCount;
	/*4,4*/ char** srflName;
	/*8*/
};
assert_sizeof(LWSURFLIST, 0x8);


struct LWSIZE
{
	/*0,4*/ uint32 lwVertCount;
	/*4,4*/ uint32 lwPolyCount;
	/*8,4*/ uint32 lwSurfaceCount;
	/*c*/
};
assert_sizeof(LWSIZE, 0xc);


struct LWPOLY
{
	/*0,4*/ uint32 plyCount;
	/*4,4*/ uint32 plySurface;
	/*8,4*/ uint16* plyData;
	/*c*/
};
assert_sizeof(LWPOLY, 0xc);


// ColourRGBAPacked with differently-named fields
struct LWRGB
{
	/*0,1*/ uint8 colRed;
	/*1,1*/ uint8 colGreen;
	/*2,1*/ uint8 colBlue;
	/*3,1*/ uint8 colAlpha;
	/*4*/
};
assert_sizeof(LWRGB, 0x4);

// Vector3F with differently-named fields
struct TEXDATA
{
	/*0,4*/ real32 tdX;
	/*4,4*/ real32 tdY;
	/*8,4*/ real32 tdZ;
	/*c*/
};
assert_sizeof(TEXDATA, 0xc);


struct LWSURFACE
{
	/*00,4*/ char*	srfName;
	/*04,4*/ LWSURFACE* srfNextSurf;
	/*08,4*/ char*	srfPath;
	/*0c,4*/ LWRGB	srfCol;
	/*10,4*/ char	srfTCLR[4];
	/*14,4*/ LightWave_TexFlags srfTexFlags;
	/*18,4*/ LightWave_SurfFlags srfFlags;
	/*0c,4*/ LightWave_TexType srfTexType;
	/*20,4*/ uint32	srfTexWrap;
	/*24,c*/ TEXDATA srfTexSize;
	/*30,c*/ TEXDATA srfTexCentre; // british spelling "Centre"
	/*3c,4*/ real32 srfLuminous;
	/*40,4*/ real32 srfTransparent;
	/*44,4*/ real32 srfDiffuse;
	/*48,4*/ real32 srfReflect;
	/*4c,4*/ real32 srfSpecular;
	/*50,4*/ real32 srfSpecPower;
	/*54*/
};
assert_sizeof(LWSURFACE, 0x54);


struct APPOBJ
{
	/*00,4*/ char*		aoPath;
	/*04,c*/ LWSIZE		aoSize;
	/*10,4*/ real32*	aoVerts;
	/*14,4*/ LWPOLY*	aoPoly;
	/*18,4*/ LWSURFACE*	aoSurface;
	/*1c,4*/ File* aoFileUV;
	/*20*/
};
assert_sizeof(APPOBJ, 0x20);

#pragma endregion

/**********************************************************************************
 ******** Macros
 **********************************************************************************/

#pragma region Macros

#define LWVALUE(n) (((n>>24)&0xff) + ((n>>8)&0xff00) + ((n<<8)&0xff0000) + ((n<<24)&0xff000000))
#define LWSVALUE(n) (((n>>8)&0xff) + ((n<<8)&0xff00))
#define LWSPVALUE(n) ((((*n)>>8)&0xff) + (((*n)<<8)&0xff00))

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @0048c300>
const char* __cdecl lwExtractString(const char* str1, const char* str2, OPTIONAL OUT uint32* pos, OPTIONAL OUT uint32* len);

// "YASD" Yet another `char* ::_strdup(const char*)`
// <LegoRR.exe @0048c380>
char* __cdecl stringAlloc(const char* str);

// <LegoRR.exe @0048c3e0>
LightWave_TexType __cdecl texMapType(const char* str);

// if (flag) Mem_Free(srf);
// <LegoRR.exe @0048c440>
void __cdecl surfFree(LWSURFACE* srf, bool32 flag);

// <LegoRR.exe @0048c490>
bool32 __cdecl LWD3D(uint8** p, real32* f);

// <LegoRR.exe @0048c4d0>
bool32 __cdecl PNTSprc(File* file, LWSIZE* sizeData, sint32 csize, real32** verts, bool32 dflag);

// <LegoRR.exe @0048c620>
bool32 __cdecl CRVSprc(File* file, sint32 csize, bool32 dflag);

// <LegoRR.exe @0048c6a0>
bool32 __cdecl POLSprc(File* file, LWSIZE* sizeData, LWPOLY** polys, sint32 csize, bool32 dflag);

// if (dflag) ... Error_Debug(...);
// <LegoRR.exe @0048c950>
bool32 __cdecl SRFSprc(File* file, LWSIZE* sizeData, LWSURFLIST** srfl, sint32 csize, OUT uint32* srflCount, bool32 dflag);

// if (dflag) ... Error_Debug(...);
// <LegoRR.exe @0048cae0>
bool32 __cdecl SURFprc(File* file, LWSIZE* sizeData, LWSURFACE** surf, sint32 csize, bool32 dflag);

// if (dflag) ... Error_Debug(...);
// <LegoRR.exe @0048d580>
bool32 __cdecl LoadLWOB(const char* fn, LWSIZE* sd, real32** verts, LWPOLY** polys, LWSURFACE** surfs, File** fileUV, bool32 dflag);

// if (dflag) ... Error_Debug(...);
// <LegoRR.exe @0048da80>
bool32 __cdecl LoadAppObj(const char* fn, APPOBJ** ao, bool32 flag);

// flag -> if (dFlag) ... Error_Debug(...);
// <LegoRR.exe @0048db30>
bool32 __cdecl FreeLWOB(APPOBJ* ao);

#pragma endregion

}
