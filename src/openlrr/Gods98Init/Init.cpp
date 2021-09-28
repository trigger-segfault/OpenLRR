// Init/Init.cpp : 
//

#include "../platform/windows.h"

#include "../Gods98/DirectDraw.h"
#include "../Gods98/Errors.h"
#include "../Gods98/Main.h"

#include "Init.h"


/**********************************************************************************
 ******** Resource IDs
 **********************************************************************************/

#pragma region Resource IDs

/// NOTE: These are the original resource IDs present in LegoRR.exe.
///       There may be no relation to resource IDs in OpenLRR unless otherwise stated.

#define IDD_MODEDIALOG                  101   // 1033 : DIALOG : "Mode Selection"
#define IDI_LEGORR						113   // 2057 : ICON_GROUP : (main icon)

#define IDC_DRIVER                      1000  // IDD_MODEDIALOG : LISTBOX : "Driver"
#define IDC_DEVICE                      1002  // IDD_MODEDIALOG : LISTBOX : "Device"
#define IDC_MODE                        1003  // IDD_MODEDIALOG : LISTBOX : "Screen Mode" | "Window Size"
#define IDC_FULLSCREEN                  1004  // IDD_MODEDIALOG : RADIO BUTTON : "Full Screen"
#define IDC_WINDOW                      1005  // IDD_MODEDIALOG : RADIO BUTTON : "Window"
#define IDC_MODELISTTEXT                1006  // IDD_MODEDIALOG : STATIC : "Screen Mode" | "Window Size"

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @005590a0>
Gods98::Init_Globs & Gods98::initGlobs = *(Gods98::Init_Globs*)0x005590a0; // (no init)

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// Entry point for Init "Mode Selection" dialog.
//  Calls the appropriate DirectDraw_Setup function after a mode has been selected.
// <LegoRR.exe @0049d2f0>
bool32 __cdecl Gods98::Init_Initialise(bool32 setup, bool32 debug, bool32 best, bool32 window, const char* noHALMsg)
{
	log_firstcall();

	sint32 rval = IDOK;
	bool32 ok;

	initGlobs.selDriver = nullptr;
	initGlobs.selDevice = nullptr;
	initGlobs.selMode = nullptr;
	initGlobs.selFullScreen = initGlobs.wasFullScreen = false;
	initGlobs.validModeCount = 0;

//	Init_AddValidMode(0, 0, 8);
//	Init_AddValidMode(0, 0, 16);			// Any 16bit mode
//	Init_AddValidMode(0, 0, 24);
//	Init_AddValidMode(0, 0, 32);

//	Init_AddValidMode(640, 400, 16);
//	Init_AddValidMode(640, 400, 32);
//	if (debug) Init_AddValidMode(640, 480, 8);
	Init_AddValidMode(640, 480, 16);
//	Init_AddValidMode(640, 480, 24);
	//Init_AddValidMode(800, 600, 16);
	//Init_AddValidMode(1024, 768, 16);

	if (debug) {
		Init_AddValidMode(800, 600, 16);
		Init_AddValidMode(1024, 768, 16);
	}
//	Init_AddValidMode(640, 480, 32);

	DirectDraw_EnumDrivers(initGlobs.drivers, &initGlobs.driverCount);
	if (initGlobs.driverCount) {

		initGlobs.selDriver = &initGlobs.drivers[0];

		if (best) {

			sint32 loop, sub;
			uint32 mode;
			bool32 found = false;

			initGlobs.selFullScreen = !window;
			rval = IDOK;

			for (loop=initGlobs.driverCount-1 ; loop>=0 ; loop--) {
				if (DirectDraw_EnumDevices(&initGlobs.drivers[loop], initGlobs.devices, &initGlobs.deviceCount)) {
					DirectDraw_EnumModes(&initGlobs.drivers[loop], initGlobs.selFullScreen, initGlobs.modes, &initGlobs.modeCount);
					for (mode=0 ; mode<initGlobs.modeCount ; mode++) {
						if (Init_IsValidMode(mode)) {
							for (sub=initGlobs.deviceCount-1 ; sub>=0 ; sub--) {
//								if ((initGlobs.devices[sub].flags & DIRECTDRAW_FLAG_DEVICE_HARDWARE) || 0 == loop) {
								if (initGlobs.devices[sub].flags & DIRECTDRAW_FLAG_DEVICE_HARDWARE) {
									initGlobs.selDriver = &initGlobs.drivers[loop];
									initGlobs.selDevice = &initGlobs.devices[sub];
									initGlobs.selMode = &initGlobs.modes[mode];
									found = true;
									break;
								}
							}
							break;
						}
					}
				} else {
					::MessageBoxA(nullptr, "Please install DirectX version 6 or later", "Error", MB_OK);
					return false;
				}
				if (found == true) break; // yes, this really checked for `== TRUE`
			}

			if (!found) {
				::MessageBoxA(nullptr, noHALMsg, "Error", MB_OK);
				return false;
			}

		} else {
			if (setup) rval = ::DialogBoxParamA(Main_hInst(), MAKEINTRESOURCEA(IDD_MODEDIALOG), Main_hWnd(), (DLGPROC)Init_DialogProc, 0);
		}

		if (rval == IDOK){

			if (initGlobs.selFullScreen) ok = DirectDraw_SetupFullScreen(initGlobs.selDriver, initGlobs.selDevice, initGlobs.selMode);
			else if (initGlobs.selMode) ok = DirectDraw_SetupWindowed(initGlobs.selDevice, 100, 100, initGlobs.selMode->width, initGlobs.selMode->height);
			else ok = DirectDraw_SetupWindowed(initGlobs.selDevice, 40, 40, 640, 480);

			return ok;

		} else if (rval == -1) Error_Warn(true, "Cannot create dialog box");

	} else Error_Warn(true, "Cannot Enumerate Drivers");
	
	return false;
}

// <LegoRR.exe @0049d5b0>
BOOL __stdcall Gods98::Init_DialogProc(HWND hWndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	log_firstcall();

	if (WM_INITDIALOG == uMsg){

		RECT deskRect, dlgRect;

		HWND hWndDesktop = ::GetDesktopWindow();
		::GetWindowRect(hWndDesktop, &deskRect);
		::GetWindowRect(hWndDlg, &dlgRect);
		uint32 xPos = (deskRect.right / 2) - ((dlgRect.right - dlgRect.left) / 2);
		uint32 yPos = (deskRect.bottom / 2) - ((dlgRect.bottom - dlgRect.top) / 2);
		::MoveWindow(hWndDlg, xPos, yPos, dlgRect.right - dlgRect.left, dlgRect.bottom - dlgRect.top, true);

		HWND hWndButton;
		if (initGlobs.selFullScreen) hWndButton = ::GetDlgItem(hWndDlg, IDC_FULLSCREEN);
		else hWndButton = ::GetDlgItem(hWndDlg, IDC_WINDOW);
		::SendMessageA(hWndButton, BM_SETCHECK, BST_CHECKED, 0);

		HWND hWndList = ::GetDlgItem(hWndDlg, IDC_DRIVER);
		for (uint32 loop=0 ; loop<initGlobs.driverCount ; loop++) ::SendMessageA(hWndList, LB_ADDSTRING, 0, (LPARAM) initGlobs.drivers[loop].desc);
		::SendMessageA(hWndList, LB_SETCURSEL, 0, 0);
		::SetFocus(hWndList);

		Init_HandleWindowButton(hWndDlg);
		Init_SetDeviceList(hWndDlg);
		Init_SetModeList(hWndDlg);

		return false;

	} else if (uMsg == WM_COMMAND){

		if (HIWORD(wParam) == LBN_SELCHANGE){
//			Error_Debug(Error_Format("Selection changed on list box #%i (hWnd == 0x%x)\n", LOWORD(wParam), lParam));
			
			if (LOWORD(wParam) == IDC_DEVICE){

				initGlobs.selDevice = &initGlobs.devices[::SendMessageA((HWND)lParam, LB_GETCURSEL, 0, 0)];
	
			} else if (LOWORD(wParam) == IDC_MODE){

				// buffer should be filled by SendMessage LB_GETTEXT, unless it fails
				char desc[1024] = { 0 }; // dummy init
				uint32 mode;

				uint32 num = ::SendMessageA((HWND)lParam, LB_GETCURSEL, 0, 0);
				::SendMessageA((HWND)lParam, LB_GETTEXT, num, (LPARAM) desc);
				if (Init_GetMode(desc, &mode)) initGlobs.selMode = &initGlobs.modes[mode];

			} else if (LOWORD(wParam) == IDC_DRIVER){

				initGlobs.selDriver = &initGlobs.drivers[::SendMessageA((HWND)lParam, LB_GETCURSEL, 0, 0)];

				Init_HandleWindowButton(hWndDlg);
				Init_SetModeList(hWndDlg);
				Init_SetDeviceList(hWndDlg);
				
			}

		} else if (HIWORD(wParam) == BN_CLICKED){

			if (LOWORD(wParam) == IDC_WINDOW) Init_SetFullScreen(hWndDlg, false);
			if (LOWORD(wParam) == IDC_FULLSCREEN) Init_SetFullScreen(hWndDlg, true);
			else if (LOWORD(wParam) == IDOK) ::EndDialog(hWndDlg, wParam);
			else if (LOWORD(wParam) == IDCANCEL) ::EndDialog(hWndDlg, wParam);
		}

		return true;
	}

	return false;
}

// Change the FullScreen mode setting, and update the "Screen Mode" / "Window Size" label text.
// <LegoRR.exe @0049d8a0>
void __cdecl Gods98::Init_SetFullScreen(HWND hWndDlg, bool32 on)
{
	log_firstcall();

	initGlobs.selFullScreen = on;
	Init_SetModeList(hWndDlg);
	HWND hWndText = ::GetDlgItem(hWndDlg, IDC_MODELISTTEXT);
	if (on) ::SendMessageA(hWndText, WM_SETTEXT, 0, (LPARAM)"Screen Mode");
	else ::SendMessageA(hWndText, WM_SETTEXT, 0, (LPARAM)"Window Size");
}

// Rebuild (enumerate through) and repopulate the listbox of modes.
// Only modes registered as supported by the game will appear in the listbox.
// <LegoRR.exe @0049d8f0>
void __cdecl Gods98::Init_SetModeList(HWND hWndDlg)
{
	log_firstcall();

	bool32 first = true;

	HWND hWndList = ::GetDlgItem(hWndDlg, IDC_MODE);
	
	// Clear the existing list (if any)...
	while (::SendMessageA(hWndList, LB_DELETESTRING, 0, 0) != -1) ;

	DirectDraw_EnumModes(initGlobs.selDriver, initGlobs.selFullScreen, initGlobs.modes, &initGlobs.modeCount);
	for (uint32 loop=0 ; loop<initGlobs.modeCount ; loop++) {
		if (Init_IsValidMode(loop)) {
			::SendMessageA(hWndList, LB_ADDSTRING, 0, (LPARAM) initGlobs.modes[loop].desc);
			if (first) {
				initGlobs.selMode = &initGlobs.modes[loop];
				first = false;
			}
		}
	}

	HWND hWndButton = ::GetDlgItem(hWndDlg, IDOK);

	if (first) {
		char msg[256];
		if (!initGlobs.selFullScreen) std::sprintf(msg, "No supported %i bit modes found", Main_GetWindowsBitDepth());
		else std::sprintf(msg, "No supported screen modes found");
		::SendMessageA(hWndList, LB_ADDSTRING, 0, (LPARAM) msg);
		::EnableWindow(hWndButton, false);
	} else ::EnableWindow(hWndButton, true);

	::SendMessageA(hWndList, LB_SETCURSEL, 0, 0);
}

// Rebuild (enumerate through) and repopulate the listbox of devices.
// <LegoRR.exe @0049da40>
void __cdecl Gods98::Init_SetDeviceList(HWND hWndDlg)
{
	log_firstcall();

	uint32 best = 0;

	HWND hWndList = ::GetDlgItem(hWndDlg, IDC_DEVICE);
	
	// Clear the existing list (if any)...
	while (::SendMessageA(hWndList, LB_DELETESTRING, 0, 0) != -1) ;

	if (DirectDraw_EnumDevices(initGlobs.selDriver, initGlobs.devices, &initGlobs.deviceCount)) {

		for (uint32 loop=0 ; loop<initGlobs.deviceCount ; loop++) {
			if (initGlobs.devices[loop].flags & DIRECTDRAW_FLAG_DEVICE_HARDWARE) best = loop;
			::SendMessageA(hWndList, LB_ADDSTRING, 0, (LPARAM) initGlobs.devices[loop].desc);
		}
		initGlobs.selDevice = &initGlobs.devices[best];

	} else {
		::SendMessageA(hWndList, LB_ADDSTRING, 0, (LPARAM) "Error: DirectX6 not installed.");
	}

	::SendMessageA(hWndList, LB_SETCURSEL, best, 0);
}

// Register a mode that the game supports.
// Although this assigns to a table using the same ScreenMode structure,
//  only the width, height, and depth fields are assigned.
// <LegoRR.exe @0049db30>
void __cdecl Gods98::Init_AddValidMode(uint32 width, uint32 height, uint32 depth)
{
	log_firstcall();

	initGlobs.validModes[initGlobs.validModeCount].width = width;
	initGlobs.validModes[initGlobs.validModeCount].height = height;
	initGlobs.validModes[initGlobs.validModeCount].bitDepth = depth;

	initGlobs.validModeCount++;
}

// Check if the selected system-available screen mode is supported by the game.
// <LegoRR.exe @0049db90>
bool32 __cdecl Gods98::Init_IsValidMode(uint32 mode)
{
	log_firstcall();

	if (initGlobs.validModeCount) {

		for (uint32 loop = 0; loop < initGlobs.validModeCount; loop++) {
			if ((initGlobs.validModes[loop].width == 0 || initGlobs.validModes[loop].width == initGlobs.modes[mode].width) &&
				(initGlobs.validModes[loop].height == 0 || initGlobs.validModes[loop].height == initGlobs.modes[mode].height) &&
				(initGlobs.validModes[loop].bitDepth == 0 || initGlobs.validModes[loop].bitDepth == initGlobs.modes[mode].bitDepth)) {

				return true;
			}
		}

		return false;

	}
	else return true;
}

// Get the index of a system-available mode from the display name used in the listbox.
// <LegoRR.exe @0049dc10>
bool32 __cdecl Gods98::Init_GetMode(const char* name, OUT uint32* mode)
{
	log_firstcall();

	for (uint32 loop = 0; loop < initGlobs.modeCount; loop++) {
		if (std::strcmp(initGlobs.modes[loop].desc, name) == 0) {

			*mode = loop;
			return true;
		}
	}

	return false;
}

// Update FullScreen/Window radio buttons depending on whether
// the currently selected driver allows windowed mode.
// <LegoRR.exe @0049dc90>
void __cdecl Gods98::Init_HandleWindowButton(HWND hWndDlg)
{
	log_firstcall();

	HWND hWndButton;

	if (initGlobs.selDriver->flags & DIRECTDRAW_FLAG_DRIVER_WINDOWOK) {

		hWndButton = ::GetDlgItem(hWndDlg, IDC_WINDOW);
		::EnableWindow(hWndButton, true);

		// Was previous mode Windowed before DIRECTDRAW_FLAG_DRIVER_WINDOWOK was false?
		//  If so, change it back.
		if (!initGlobs.wasFullScreen){
			Init_SetFullScreen(hWndDlg, false);
			hWndButton = ::GetDlgItem(hWndDlg, IDC_FULLSCREEN);
			::SendMessageA(hWndButton, BM_SETCHECK, BST_UNCHECKED, 0);
			hWndButton = ::GetDlgItem(hWndDlg, IDC_WINDOW);
			::SendMessageA(hWndButton, BM_SETCHECK, BST_CHECKED, 0);
		}

	} else {
		// Store the original FullScreen mode, we can revert
		//  this when Windowed mode is supported again.
		initGlobs.wasFullScreen = initGlobs.selFullScreen;

		Init_SetFullScreen(hWndDlg, true);
		hWndButton = ::GetDlgItem(hWndDlg, IDC_WINDOW);
		::SendMessageA(hWndButton, BM_SETCHECK, BST_UNCHECKED, 0);
		::EnableWindow(hWndButton, false);
		hWndButton = ::GetDlgItem(hWndDlg, IDC_FULLSCREEN);
		::SendMessageA(hWndButton, BM_SETCHECK, BST_CHECKED, 0);

	}
}

#pragma endregion

