// Init.h : 
//
/// APIS: -
/// DEPENDENCIES: Main, DirectDraw, (Errors)
/// DEPENDENTS: Main

#pragma once

#include "../platform/windows.h"

#include "../common.h"

#include "drawing/DirectDraw.h"


namespace Gods98
{; // !<---

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

struct Init_Globs
{
	/*00000,4*/ uint32 driverCount;
	/*00004,4*/ uint32 deviceCount;
	/*00008,4*/ uint32 modeCount;
	/*0000c,1590*/ DirectDraw_Driver drivers[DIRECTDRAW_MAXDRIVERS];
	/*0159c,1590*/ DirectDraw_Device devices[DIRECTDRAW_MAXDEVICES];
	/*02b2c,d480*/ DirectDraw_Mode modes[DIRECTDRAW_MAXMODES];
	/*0ffac,4*/ const DirectDraw_Driver* selDriver;
	/*0ffb0,4*/ const DirectDraw_Device* selDevice;
	/*0ffb4,4*/ const DirectDraw_Mode* selMode;
	/*0ffb8,4*/ bool32 selFullScreen;
	/*0ffbc,4*/ bool32 wasFullScreen;
	/*0ffc0,d480*/ DirectDraw_Mode validModes[DIRECTDRAW_MAXMODES];
	/*1d440,4*/ uint32 validModeCount;
	/*1d444*/
};
static_assert(sizeof(Init_Globs) == 0x1d444, "");

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @005590a0>
extern Init_Globs & initGlobs;

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// Entry point for Init "Mode Selection" dialog.
//  Calls the appropriate DirectDraw_Setup function after a mode has been selected.
// <LegoRR.exe @0049d2f0>
bool32 __cdecl Init_Initialise(bool32 setup, bool32 debug, bool32 best, bool32 window, const char* noHALMsg);

// <LegoRR.exe @0049d5b0>
BOOL __stdcall Init_DialogProc(HWND hWndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Change the FullScreen mode setting, and update the "Screen Mode" / "Window Size" label text.
// <LegoRR.exe @0049d8a0>
void __cdecl Init_SetFullScreen(HWND hWndDlg, bool32 on);

// Rebuild (enumerate through) and repopulate the listbox of modes.
// Only modes registered as supported by the game will appear in the listbox.
// <LegoRR.exe @0049d8f0>
void __cdecl Init_SetModeList(HWND hWndDlg);

// Rebuild (enumerate through) and repopulate the listbox of devices.
// <LegoRR.exe @0049da40>
void __cdecl Init_SetDeviceList(HWND hWndDlg);

// Register a mode that the game supports.
// Although this assigns to a table using the same ScreenMode structure,
//  only the width, height, and depth fields are assigned.
// <LegoRR.exe @0049db30>
void __cdecl Init_AddValidMode(uint32 width, uint32 height, uint32 depth);

// Check if the selected system-available screen mode is supported by the game.
// <LegoRR.exe @0049db90>
bool32 __cdecl Init_IsValidMode(uint32 mode);

// Get the index of a system-available mode from the display name used in the listbox.
// <LegoRR.exe @0049dc10>
bool32 __cdecl Init_GetMode(const char* name, OUT uint32* mode);

// Update FullScreen/Window radio buttons depending on whether
// the currently selected driver allows windowed mode.
// <LegoRR.exe @0049dc90>
void __cdecl Init_HandleWindowButton(HWND hWndDlg);

#pragma endregion

}
