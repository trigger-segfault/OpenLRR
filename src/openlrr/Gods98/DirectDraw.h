// DirectDraw.h : 
//
/// APIS: IDirect3DRMDevice3,
///       IDirect3D3,
///       IDirectDraw[14], IDirectDrawClipper, IDirectDrawPalette, IDirectDrawSurface4
/// DEPENDENCIES: Bmp, Files, Main, Maths, Utils, (Dxbug, Errors, Memory)
/// DEPENDENTS: Animation, Containers, Draw, Flic, Image, Main, Movie, Init,
///             Lego, Loader, NERPs, RadarMap

#pragma once

#include "../common.h"
#include "../Types/geometry.h"


/**********************************************************************************
 ******** Forward Global Namespace Declarations
 **********************************************************************************/

#pragma region Forward Declarations

struct IDirectDraw4;
struct IDirectDrawSurface4;
struct IDirectDrawClipper;
struct tagPALETTEENTRY;
typedef struct tagPALETTEENTRY PALETTEENTRY;

#pragma endregion


namespace Gods98
{; // !<---

/**********************************************************************************
 ******** Constants
 **********************************************************************************/

#pragma region Constants

#define DIRECTDRAW_MAXDRIVERS				20
#define DIRECTDRAW_MAXDEVICES				20
#define DIRECTDRAW_MAXMODES					200
#define DIRECTDRAW_DRIVERSTRINGLEN			256
#define DIRECTDRAW_DEVICESTRINGLEN			256

#pragma endregion

/**********************************************************************************
 ******** Enumerations
 **********************************************************************************/

#pragma region Enums

//#define DIRECTDRAW_FLAG_VALID					0x00000001

/*#define DIRECTDRAW_FLAG_DRIVER_PRIMARY			0x00000010
#define DIRECTDRAW_FLAG_DRIVER_WINDOWOK			0x00000020

#define DIRECTDRAW_FLAG_DEVICE_DEPTH8			0x00000010
#define DIRECTDRAW_FLAG_DEVICE_DEPTH16			0x00000020
#define DIRECTDRAW_FLAG_DEVICE_DEPTH24			0x00000040
#define DIRECTDRAW_FLAG_DEVICE_DEPTH32			0x00000080
#define DIRECTDRAW_FLAG_DEVICE_ZBUFF8			0x00000100
#define DIRECTDRAW_FLAG_DEVICE_ZBUFF16			0x00000200
#define DIRECTDRAW_FLAG_DEVICE_ZBUFF24			0x00000400
#define DIRECTDRAW_FLAG_DEVICE_ZBUFF32			0x00000800
#define DIRECTDRAW_FLAG_DEVICE_COLOUR			0x00001000
#define DIRECTDRAW_FLAG_DEVICE_HARDWARE			0x00002000
#define DIRECTDRAW_FLAG_DEVICE_VIDEOTEXTURE		0x00004000
#define DIRECTDRAW_FLAG_DEVICE_SYSTEMTEXTURE	0x00008000

/// CUSTOM:
#define DIRECTDRAW_FLAG_DRIVER_VALID			0x00000001
#define DIRECTDRAW_FLAG_DEVICE_VALID			0x00000001
#define DIRECTDRAW_FLAG_MODE_VALID				0x00000001*/

enum DirectDraw_DriverFlags : uint32
{
	DIRECTDRAW_FLAG_DRIVER_NONE				= 0,

	DIRECTDRAW_FLAG_DRIVER_VALID			= 0x1,
	DIRECTDRAW_FLAG_DRIVER_PRIMARY			= 0x10,
	DIRECTDRAW_FLAG_DRIVER_WINDOWOK			= 0x20,
};
DEFINE_ENUM_FLAG_OPERATORS(DirectDraw_DriverFlags);
static_assert(sizeof(DirectDraw_DriverFlags) == 0x4, "");


enum DirectDraw_DeviceFlags : uint32
{
	DIRECTDRAW_FLAG_DEVICE_NONE				= 0,

	DIRECTDRAW_FLAG_DEVICE_VALID			= 0x1,
	DIRECTDRAW_FLAG_DEVICE_DEPTH8			= 0x10,
	DIRECTDRAW_FLAG_DEVICE_DEPTH16			= 0x20,
	DIRECTDRAW_FLAG_DEVICE_DEPTH24			= 0x40,
	DIRECTDRAW_FLAG_DEVICE_DEPTH32			= 0x80,
	DIRECTDRAW_FLAG_DEVICE_ZBUFF8			= 0x100,
	DIRECTDRAW_FLAG_DEVICE_ZBUFF16			= 0x200,
	DIRECTDRAW_FLAG_DEVICE_ZBUFF24			= 0x400,
	DIRECTDRAW_FLAG_DEVICE_ZBUFF32			= 0x800,
	DIRECTDRAW_FLAG_DEVICE_COLOUR			= 0x1000,
	DIRECTDRAW_FLAG_DEVICE_HARDWARE			= 0x2000,
	DIRECTDRAW_FLAG_DEVICE_VIDEOTEXTURE		= 0x4000,
	DIRECTDRAW_FLAG_DEVICE_SYSTEMTEXTURE	= 0x8000,
};
DEFINE_ENUM_FLAG_OPERATORS(DirectDraw_DeviceFlags);
static_assert(sizeof(DirectDraw_DeviceFlags) == 0x4, "");


enum DirectDraw_ModeFlags : uint32
{
	DIRECTDRAW_FLAG_MODE_NONE				= 0,

	DIRECTDRAW_FLAG_MODE_VALID				= 0x1,
};
DEFINE_ENUM_FLAG_OPERATORS(DirectDraw_ModeFlags);
static_assert(sizeof(DirectDraw_ModeFlags) == 0x4, "");

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

struct DirectDraw_Driver
{
	/*000,10*/ GUID guid;
	/*010,100*/ char desc[DIRECTDRAW_DRIVERSTRINGLEN];
	/*110,4*/ DirectDraw_DriverFlags flags;
	/*114*/
};
static_assert(sizeof(DirectDraw_Driver) == 0x114, "");


struct DirectDraw_Device
{
	/*000,10*/ GUID guid;
	/*010,100*/ char desc[DIRECTDRAW_DEVICESTRINGLEN];
	/*110,4*/ DirectDraw_DeviceFlags flags;
	/*114*/
};
static_assert(sizeof(DirectDraw_Device) == 0x114, "");


struct DirectDraw_Mode
{
	/*000,4*/ uint32 width;
	/*004,4*/ uint32 height;
	/*008,4*/ uint32 bitDepth;
	/*00c,100*/ char desc[DIRECTDRAW_DEVICESTRINGLEN];
	/*10c,4*/ DirectDraw_ModeFlags flags;
	/*110*/
};
static_assert(sizeof(DirectDraw_Mode) == 0x110, "");


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
	/*1c,4*/ DirectDraw_Driver* driverList;
	/*20,4*/ DirectDraw_Driver* selectedDriver;		// (unused)
	/*24,4*/ DirectDraw_Device* deviceList;
	/*28,4*/ DirectDraw_Device* selectedDevice;		// (unused)
	/*2c,4*/ DirectDraw_Mode* modeList;
	/*30,4*/ uint32 driverCount;
	/*34,4*/ uint32 deviceCount;
	/*38,4*/ uint32 modeCount;
	/*3c,4*/ bool32 fullScreen;
	/*40,4*/ uint32 width;
	/*44,4*/ uint32 height;
	// [globs: end]
	/*48*/
};
static_assert(sizeof(DirectDraw_Globs) == 0x48, "");

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


//__inline bool32 DirectDraw_SetupFullScreen(lpDirectDraw_Driver driver, lpDirectDraw_Device device, lpDirectDraw_Mode mode)	{ return DirectDraw_Setup(TRUE, driver, device, mode, 0, 0, 320, 200); }
//__inline bool32 DirectDraw_SetupWindowed(lpDirectDraw_Device device, ULONG xPos, ULONG yPos, ULONG width, ULONG height)		{ return DirectDraw_Setup(FALSE, NULL, device, NULL, xPos, yPos, width, height); }


// <LegoRR.exe @0047c430>
void __cdecl DirectDraw_Initialise(HWND hWnd);

// <LegoRR.exe @0047c480>
bool32 __cdecl DirectDraw_EnumDrivers(OUT DirectDraw_Driver* list, OUT uint32* count);

// <LegoRR.exe @0047c4b0>
BOOL __stdcall DirectDraw_EnumDriverCallback(GUID FAR* lpGUID, LPSTR lpDriverDescription, LPSTR lpDriverName, LPVOID lpContext);

// <LegoRR.exe @0047c5a0>
bool32 __cdecl DirectDraw_EnumDevices(const DirectDraw_Driver* driver, OUT DirectDraw_Device* list, OUT uint32* count);

// <LegoRR.exe @0047c640>
HRESULT __stdcall DirectDraw_EnumDeviceCallback(LPGUID lpGuid, LPSTR lpDeviceDescription,
											LPSTR lpDeviceName, LPD3DDEVICEDESC lpHWDesc,
											LPD3DDEVICEDESC lpHELDesc, LPVOID lpContext);

// <LegoRR.exe @0047c770>
bool32 __cdecl DirectDraw_EnumModes(const DirectDraw_Driver* driver, bool32 fullScreen, OUT DirectDraw_Mode* list, OUT uint32* count);

// <LegoRR.exe @0047c810>
HRESULT __stdcall DirectDraw_EnumModeCallback(LPDDSURFACEDESC2 lpDDSurfaceDesc, LPVOID lpContext);

// <LegoRR.exe @0047c8d0>
bool32 __cdecl DirectDraw_Setup(bool32 fullscreen, const DirectDraw_Driver* driver, const DirectDraw_Device* device,
								const DirectDraw_Mode* mode, uint32 xPos, uint32 yPos, uint32 width, uint32 height);

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
void __cdecl DirectDraw_Blt8To16(IDirectDrawSurface4* target, IDirectDrawSurface4* source, PALETTEENTRY* palette);

// <LegoRR.exe @0047d590>
uint32 __cdecl DirectDraw_GetColour(IDirectDrawSurface4* surf, uint32 colour);

// <LegoRR.exe @0047d6b0>
uint32 __cdecl DirectDraw_GetNumberOfBits(uint32 mask);


// <inlined>
__inline bool32 DirectDraw_SetupFullScreen(const DirectDraw_Driver* driver, const DirectDraw_Device* device, const DirectDraw_Mode* mode) { return DirectDraw_Setup(true, driver, device, mode, 0, 0, 320, 200); }

// <inlined>
__inline bool32 DirectDraw_SetupWindowed(const DirectDraw_Device* device, uint32 xPos, uint32 yPos, uint32 width, uint32 height) { return DirectDraw_Setup(false, nullptr, device, nullptr, xPos, yPos, width, height); }

#pragma endregion

}
