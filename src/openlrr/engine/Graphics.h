// Graphics.h : 
//
/// APIS: IDirect3DRM3, IDirect3DRMDevice3, 
///       IDirect3DDevice3
///       IDirectDraw4, IDirectDrawSurface4
/// DEPENDENCIES: Animation, Config, DirectDraw, Draw, Files, Input, Registry,
///               Sound, Utils, Init, Gods_Go, (Dxbug, Errors, Fonts, Images, Memory)
/// DEPENDENTS: ...

#pragma once

#include "../common.h"

#include "Main.h"


/**********************************************************************************
 ******** Forward Global Namespace Declarations
 **********************************************************************************/

#pragma region Forward Declarations

struct IDirectDraw;
struct IDirectDrawSurface4;
struct IDirect3DRM3;
struct IDirect3DRMDevice3;
struct IDirect3DDevice3;
enum _D3DRENDERSTATETYPE;
typedef enum _D3DRENDERSTATETYPE D3DRENDERSTATETYPE;
assert_sizeof(D3DRENDERSTATETYPE, 0x4);

#pragma endregion


namespace Gods98
{; // !<---

/**********************************************************************************
 ******** Forward Declarations
 **********************************************************************************/

#pragma region Forward Declarations

struct Graphics_Device;

#pragma endregion

/**********************************************************************************
 ******** Function Typedefs
 **********************************************************************************/

#pragma region Function Typedefs

#pragma endregion

/**********************************************************************************
 ******** Constants
 **********************************************************************************/

#pragma region Constants

//#define GRAPHICS_MAXRENDERSTATES				200

#pragma endregion

/**********************************************************************************
 ******** Enumerations
 **********************************************************************************/

#pragma region Enums

#if 0
// We can safely hardcode these values since D3DRM won't change
// same as enum Main_Quality (just different symbol names)
// (unused for now, until Graphics can be untangled from Main)
enum class Graphics_FogMethod : uint32
{
	Vertex = 0x1, // = D3DRMFOGMETHOD_VERTEX,
	Table  = 0x2, // = D3DRMFOGMETHOD_TABLE,
	Any    = 0x4, // = D3DRMFOGMETHOD_ANY,
};
assert_sizeof(Graphics_FogMethod, 0x4);


// same as enum Main_Quality (just different symbol names)
enum class Graphics_Quality : uint32
{
	Wireframe = 0, // (placeholder: D3DRMRENDER_WIREFRAME)
	UnlitFlat = 1, // (placeholder: D3DRMRENDER_UNLITFLAT) lighting not supported by immediate mode, same as Flat
	Flat      = 2, // (placeholder: D3DRMRENDER_FLAT)
	Gouraud   = 3, // (placeholder: D3DRMRENDER_GOURAUD)
	Phong     = 4, // (placeholder: D3DRMRENDER_PHONG) CUSTOM: add remaining render quality combination, does this have any use?

	/*/// OLD:
	WIREFRAME      = 0,
	UNLITFLATSHADE = 1,
	FLATSHADE      = 2,
	GOURAUDSHADE   = 3,
	PHONGSHADE     = 4,*/
};
assert_sizeof(Graphics_Quality, 0x4);
#endif

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

#if 0
struct Graphics_StateChangeData
{
	/*0,4*/ uint32 origValue;
	/*4,4*/ bool32 changed;
	/*8*/
};
assert_sizeof(Graphics_StateChangeData, 0x8);
#endif


/// CUSTOM: Extension of `Main_Globs` for OpenLRR-exclusive graphics-management globals
struct Graphics_Globs
{
	/// RENDER:
	// All options passed to Main_Setup3D
	//  (most of these can't really be used as the Mesh module currently is)
	bool32				dither;
	bool32				linearFilter;
	bool32				mipMap;
	bool32				mipMapLinear;
	bool32				blendTransparency; // (placeholder: D3DRMRENDERMODE_BLENDEDTRANSPARENCY)
	bool32				sortTransparency;  // (placeholder: D3DRMRENDERMODE_SORTEDTRANSPARENCY)
	Graphics_Quality	renderQuality; // don't put at the top to avoid C 0-initialisation headaches
};

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

/// CUSTOM: Extension of `Main_Globs` for OpenLRR-exclusive graphics-management globals
extern Graphics_Globs graphicsGlobs;

#pragma endregion


/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <inlined>
__inline IDirect3DRM3* lpD3DRM(void) { return mainGlobs.lpD3DRM; }

// <inlined>
__inline IDirect3DRMDevice3* lpDevice(void) { return mainGlobs.device; }

// <inlined>
__inline IDirect3DDevice3* lpIMDevice(void) { return mainGlobs.imDevice; }

// <inlined>
__inline bool32 Graphics_VideoTexture(void) { return (mainGlobs.flags & MainFlags::MAIN_FLAG_VIDEOTEXTURE); }

// <inlined>
__inline bool32 Graphics_MIPMapEnabled(void) { return (mainGlobs.flags & MainFlags::MAIN_FLAG_MIPMAPENABLED); }

// <inlined>
__inline bool32 Graphics_FullScreen(void) { return (mainGlobs.flags & MainFlags::MAIN_FLAG_FULLSCREEN); }

// Gets the fog method used with `IDirect3DRMFrame3::SetSceneFogMethod(uint32)`.
// <inlined>
__inline uint32 Graphics_GetFogMethod(void) { return mainGlobs.fogMethod; }

// Sets the fog method used with `IDirect3DRMFrame3::SetSceneFogMethod(uint32)`.
// <inlined>
__inline void Graphics_SetFogMethod(uint32 m) { mainGlobs.fogMethod = m; }



/// CUSTOM: Gets if the `MAIN_FLAG_DONTMANAGETEXTURES` flag is unset.
__inline bool32 Graphics_ManageTextures(void) { return !(mainGlobs.flags & MainFlags::MAIN_FLAG_DONTMANAGETEXTURES); }

/// CUSTOM: Gets if the `MAIN_FLAG_REDUCEANIMATION` flag is set.
__inline bool32 Graphics_ReduceAnimation(void) { return (mainGlobs.flags & MainFlags::MAIN_FLAG_REDUCEANIMATION); }

/// CUSTOM: Gets if the `MAIN_FLAG_REDUCEFLICS` flag is set.
__inline bool32 Graphics_ReduceFlics(void) { return (mainGlobs.flags & MainFlags::MAIN_FLAG_REDUCEFLICS); }

/// CUSTOM: Gets if the `MAIN_FLAG_REDUCEIMAGES` flag is set.
__inline bool32 Graphics_ReduceImages(void) { return (mainGlobs.flags & MainFlags::MAIN_FLAG_REDUCEIMAGES); }

/// CUSTOM: Gets if the `MAIN_FLAG_REDUCEPROMESHES` flag is set.
__inline bool32 Graphics_ReducePromeshes(void) { return (mainGlobs.flags & MainFlags::MAIN_FLAG_REDUCEPROMESHES); }

/// CUSTOM: Gets if the `MAIN_FLAG_REDUCESAMPLES` flag is set.
__inline bool32 Graphics_ReduceSamples(void) { return (mainGlobs.flags & MainFlags::MAIN_FLAG_REDUCESAMPLES); }



// Turns off Texture Management if the `-ftm` commandline option was NOT used.
//<LegoRR.exe @00477e90>
void __cdecl Graphics_DisableTextureManagement(void);

// <LegoRR.exe @00478240>
uint32 __cdecl Graphics_GetWindowsBitDepth(void);

// <LegoRR.exe @00478260>
void __cdecl Graphics_Finalise3D(void);

// <LegoRR.exe @00478490>
bool32 __cdecl Graphics_SetupDirect3D(const Graphics_Device* dev, IDirectDraw* ddraw1, IDirectDrawSurface4* backSurf, bool32 doubleBuffered);

// <LegoRR.exe @004785f0>
void __cdecl Graphics_Setup3D(Graphics_Quality renderQuality, bool32 dither, bool32 linearFilter, bool32 mipMap,
							  bool32 mipMapLinear, bool32 blendTransparency, bool32 sortTransparency);


// <missing>
uint32 __cdecl Graphics_GetTrianglesDrawn(bool32 total);


// These render state functions are only used by Mesh, maybe move them there?
// <LegoRR.exe @00478b90>
void __cdecl Graphics_ChangeRenderState(D3DRENDERSTATETYPE dwRenderStateType, uint32 dwRenderState);

/// NOTE: newer engine version has argument: BOOL force, but LegoRR DOES NOT have this argument.
///        (it's possible this argument has been inlined, as it negates calling the entire function body.)
// <LegoRR.exe @00478c00>
void __cdecl Graphics_RestoreStates(void);

#pragma endregion

}

