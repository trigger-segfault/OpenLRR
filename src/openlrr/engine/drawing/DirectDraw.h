// DirectDraw.h : 
//
/// APIS: IDirect3DRMDevice3,
///       IDirect3D3,
///       IDirectDraw[14], IDirectDrawClipper, IDirectDrawPalette, IDirectDrawSurface4
/// DEPENDENCIES: Bmp, Files, Main, Maths, Utils, (Dxbug, Errors, Memory)
/// DEPENDENTS: Animation, Containers, Draw, Flic, Image, Main, Movie, Init,
///             Lego, Loader, NERPs, RadarMap

#pragma once

#include "../../platform/windows.h"

#include "../../common.h"
#include "../geometry.h"


/**********************************************************************************
 ******** Forward Global Namespace Declarations
 **********************************************************************************/

#pragma region Forward Declarations

struct IDirectDraw;
struct IDirectDraw4;
struct IDirectDrawSurface4;
struct IDirectDrawClipper;
struct _D3DDeviceDesc;
typedef struct _D3DDeviceDesc			D3DDEVICEDESC, * LPD3DDEVICEDESC;
struct _DDSURFACEDESC2;
typedef struct _DDSURFACEDESC2			DDSURFACEDESC2, FAR* LPDDSURFACEDESC2;

#pragma endregion


namespace Gods98
{; // !<---

/**********************************************************************************
 ******** Forward Declarations
 **********************************************************************************/

#pragma region Forward Declarations

struct BMP_PaletteEntry; // from `engine/drawing/Bmp.h`

#pragma endregion

/**********************************************************************************
 ******** Constants
 **********************************************************************************/

#pragma region Constants

#define GRAPHICS_MAXDRIVERS				20
#define GRAPHICS_MAXDEVICES				20
#define GRAPHICS_MAXMODES				200
#define GRAPHICS_DRIVERSTRINGLEN		256
#define GRAPHICS_DEVICESTRINGLEN		256
#define GRAPHICS_MODESTRINGLEN			256

#pragma endregion

/**********************************************************************************
 ******** Enumerations
 **********************************************************************************/

#pragma region Enums

enum Graphics_DriverFlags : uint32
{
	GRAPHICS_DRIVER_FLAG_NONE			= 0,

	GRAPHICS_DRIVER_FLAG_VALID			= 0x1,
	GRAPHICS_DRIVER_FLAG_PRIMARY		= 0x10,
	GRAPHICS_DRIVER_FLAG_WINDOWOK		= 0x20,
};
flags_end(Graphics_DriverFlags, 0x4);


enum Graphics_DeviceFlags : uint32
{
	GRAPHICS_DEVICE_FLAG_NONE			= 0,

	GRAPHICS_DEVICE_FLAG_VALID			= 0x1,
	GRAPHICS_DEVICE_FLAG_DEPTH8			= 0x10,
	GRAPHICS_DEVICE_FLAG_DEPTH16		= 0x20,
	GRAPHICS_DEVICE_FLAG_DEPTH24		= 0x40,
	GRAPHICS_DEVICE_FLAG_DEPTH32		= 0x80,
	GRAPHICS_DEVICE_FLAG_ZBUFF8			= 0x100,
	GRAPHICS_DEVICE_FLAG_ZBUFF16		= 0x200,
	GRAPHICS_DEVICE_FLAG_ZBUFF24		= 0x400,
	GRAPHICS_DEVICE_FLAG_ZBUFF32		= 0x800,
	GRAPHICS_DEVICE_FLAG_COLOUR			= 0x1000,
	GRAPHICS_DEVICE_FLAG_HARDWARE		= 0x2000,
	GRAPHICS_DEVICE_FLAG_VIDEOTEXTURE	= 0x4000,
	GRAPHICS_DEVICE_FLAG_SYSTEMTEXTURE	= 0x8000,
};
flags_end(Graphics_DeviceFlags, 0x4);


enum Graphics_ModeFlags : uint32
{
	GRAPHICS_MODE_FLAG_NONE				= 0,

	GRAPHICS_MODE_FLAG_VALID			= 0x1,
};
flags_end(Graphics_ModeFlags, 0x4);

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

struct Graphics_Driver
{
	/*000,10*/ GUID guid;
	/*010,100*/ char desc[GRAPHICS_DRIVERSTRINGLEN];
	/*110,4*/ Graphics_DriverFlags flags;
	/*114*/
};
assert_sizeof(Graphics_Driver, 0x114);


struct Graphics_Device
{
	/*000,10*/ GUID guid;
	/*010,100*/ char desc[GRAPHICS_DEVICESTRINGLEN];
	/*110,4*/ Graphics_DeviceFlags flags;
	/*114*/
};
assert_sizeof(Graphics_Device, 0x114);


struct Graphics_Mode
{
	/*000,4*/ uint32 width;
	/*004,4*/ uint32 height;
	/*008,4*/ uint32 bitDepth;
	/*00c,100*/ char desc[GRAPHICS_MODESTRINGLEN];
	/*10c,4*/ Graphics_ModeFlags flags;
	/*110*/
};
assert_sizeof(Graphics_Mode, 0x110);


struct DirectDraw_Globs
{
	// [globs: start]
	/*00,4*/ HWND hWnd;
	/*04,4*/ IDirectDraw4* lpDirectDraw;
	/*08,4*/ IDirectDrawSurface4* fSurf;
	/*0c,4*/ IDirectDrawSurface4* bSurf;
	/*10,4*/ IDirectDrawSurface4* zSurf;			// (unused)
	/*14,4*/ IDirectDrawClipper* lpFrontClipper;
	/*18,4*/ IDirectDrawClipper* lpBackClipper;
	/*1c,4*/ Graphics_Driver* driverList;
	/*20,4*/ Graphics_Driver* selectedDriver;		// (unused)
	/*24,4*/ Graphics_Device* deviceList;
	/*28,4*/ Graphics_Device* selectedDevice;		// (unused)
	/*2c,4*/ Graphics_Mode* modeList;
	/*30,4*/ uint32 driverCount;
	/*34,4*/ uint32 deviceCount;
	/*38,4*/ uint32 modeCount;
	/*3c,4*/ bool32 fullScreen;
	/*40,4*/ uint32 width;
	/*44,4*/ uint32 height;
	// [globs: end]
	/*48*/
};
assert_sizeof(DirectDraw_Globs, 0x48);

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @0076bc80>
extern DirectDraw_Globs & directDrawGlobs;

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @00406500>
IDirectDraw4* __cdecl noinline(DirectDraw)(void);
__inline IDirectDraw4* DirectDraw(void) { return directDrawGlobs.lpDirectDraw; }

// <LegoRR.exe @00406510>
IDirectDrawSurface4* __cdecl noinline(DirectDraw_bSurf)(void);
__inline IDirectDrawSurface4* DirectDraw_bSurf(void) { return directDrawGlobs.bSurf; }


// <unused>
__inline IDirectDrawSurface4* DirectDraw_fSurf(void) { return directDrawGlobs.fSurf; }

// <unused>
__inline bool32 DirectDraw_FullScreen(void) { return directDrawGlobs.fullScreen; }


// <LegoRR.exe @0047c430>
void __cdecl DirectDraw_Initialise(HWND hWnd);

// <LegoRR.exe @0047c480>
bool32 __cdecl DirectDraw_EnumDrivers(OUT Graphics_Driver* list, OUT uint32* count);

// <LegoRR.exe @0047c4b0>
BOOL __stdcall DirectDraw_EnumDriverCallback(GUID* lpGUID, char* lpDriverDescription, char* lpDriverName, void* lpContext);

// <LegoRR.exe @0047c5a0>
bool32 __cdecl DirectDraw_EnumDevices(const Graphics_Driver* driver, OUT Graphics_Device* list, OUT uint32* count);

// <LegoRR.exe @0047c640>
HRESULT __stdcall DirectDraw_EnumDeviceCallback(GUID* lpGuid, char* lpDeviceDescription,
												char* lpDeviceName, D3DDEVICEDESC* lpHWDesc,
												D3DDEVICEDESC* lpHELDesc, void* lpContext);

// <LegoRR.exe @0047c770>
bool32 __cdecl DirectDraw_EnumModes(const Graphics_Driver* driver, bool32 fullScreen, OUT Graphics_Mode* list, OUT uint32* count);

// <LegoRR.exe @0047c810>
HRESULT __stdcall DirectDraw_EnumModeCallback(DDSURFACEDESC2* lpDDSurfaceDesc, void* lpContext);

// <LegoRR.exe @0047c8d0>
bool32 __cdecl DirectDraw_Setup(bool32 fullscreen, const Graphics_Driver* driver, const Graphics_Device* device,
								const Graphics_Mode* mode, uint32 xPos, uint32 yPos, uint32 width, uint32 height);

// <LegoRR.exe @0047cb90>
void __cdecl DirectDraw_Flip(void);

// <LegoRR.exe @0047cbb0>
bool32 __cdecl DirectDraw_SaveBMP(IDirectDrawSurface4* surface, const char* fname);

// <LegoRR.exe @0047cee0>
void __cdecl DirectDraw_ReturnFrontBuffer(void);

// <LegoRR.exe @0047cf10>
void __cdecl DirectDraw_BlitBuffers(void);

// <LegoRR.exe @0047cfb0>
void __cdecl DirectDraw_Shutdown(void);

// Adjust the texture usage for cards that don't like 8 bit textures...
// <LegoRR.exe @0047d010>
void __cdecl DirectDraw_AdjustTextureUsage(IN OUT uint32* textureUsage);

// <LegoRR.exe @0047d090>
bool32 __cdecl DirectDraw_GetAvailTextureMem(OUT uint32* total, OUT uint32* avail);

// <LegoRR.exe @0047d0e0>
void __cdecl DirectDraw_Clear(const Area2F* window, uint32 colour);

// <LegoRR.exe @0047d1a0>
bool32 __cdecl DirectDraw_CreateClipper(bool32 fullscreen, uint32 width, uint32 height);

// <LegoRR.exe @0047d2c0>
void __cdecl DirectDraw_Blt8To16(IDirectDrawSurface4* target, IDirectDrawSurface4* source, BMP_PaletteEntry* palette);

// <LegoRR.exe @0047d590>
uint32 __cdecl DirectDraw_GetColour(IDirectDrawSurface4* surf, uint32 colour);

// <LegoRR.exe @0047d6b0>
uint32 __cdecl DirectDraw_GetNumberOfBits(uint32 mask);


// <inlined>
__inline bool32 DirectDraw_SetupFullScreen(const Graphics_Driver* driver, const Graphics_Device* device, const Graphics_Mode* mode) { return DirectDraw_Setup(true, driver, device, mode, 0, 0, 320, 200); }

// <inlined>
__inline bool32 DirectDraw_SetupWindowed(const Graphics_Device* device, uint32 xPos, uint32 yPos, uint32 width, uint32 height) { return DirectDraw_Setup(false, nullptr, device, nullptr, xPos, yPos, width, height); }

#pragma endregion

}
