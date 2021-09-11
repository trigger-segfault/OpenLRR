
#define DIRECTINPUT_VERSION 0x500
//#include "../DirectX/D3DRM/d3drmwin.h"
#include <d3drmwin.h>
#include <dinput.h>

#include "Input.h"
#include "Main.h"
#include "Errors.h"
#include "Dxbug.h"


/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @0076ba00>
Gods98::Input_Globs & Gods98::INPUT = *(Gods98::Input_Globs*)0x0076ba00; // (no init)

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// External Function Prototypes

// Mouse data

// Mouse positions
// <LegoRR.exe @00410a60>
__inline sint32 __cdecl Gods98::msx()
{
	return INPUT.msx;
}

// <LegoRR.exe @00410a70>
__inline sint32 __cdecl Gods98::msy()
{
	return INPUT.msy;
}

// Left and right mouse buttons
// <LegoRR.exe @00410a80>
__inline bool32 __cdecl Gods98::mslb()
{
	return INPUT.mslb;
}


// <inlined>
__inline bool32 __cdecl Gods98::msrb()
{
	return INPUT.msrb;
}

// Relative mouse movement
// <inlined>
__inline sint32 __cdecl Gods98::msxr()
{
	return INPUT.diffx;
}

// <inlined>
__inline sint32 __cdecl Gods98::msyr()
{
	return INPUT.diffy;
}

// <inlined>
__inline bool32 __cdecl Gods98::mslbheld()
{
	return INPUT.mslbheld;
}

// <inlined>
__inline bool32 __cdecl Gods98::Input_LClicked()
{
	return INPUT.lClicked;
}



// Initialise direct input and the key board
// <LegoRR.exe @0047f050>
bool32 __cdecl Gods98::Input_InitKeysAndDI(void)
{
	GUID guid = GUID_SysKeyboard;

	// CHKDI_F if an Errors.h macro

	// Try to create DI object
	CHKDI_F(::DirectInputCreateA(Main_hInst(), DIRECTINPUT_VERSION, &INPUT.lpdi, nullptr));

	// Try to create keyboard device
	CHKDI_F(INPUT.lpdi->CreateDevice(guid, &INPUT.lpdiKeyboard, nullptr));

	// Tell DirectInput that we want to receive data in keyboard format
	CHKDI_F(INPUT.lpdiKeyboard->SetDataFormat(&c_dfDIKeyboard));

	// set cooperative level for keyboard
	CHKDI_F(INPUT.lpdiKeyboard->SetCooperativeLevel(Main_hWnd(), DISCL_NONEXCLUSIVE | DISCL_FOREGROUND));

	// Try to acquire the keyboard
	HRESULT hRes = INPUT.lpdiKeyboard->Acquire();
	if (hRes == DI_OK) INPUT.fKeybdAcquired = true;
	else INPUT.fKeybdAcquired = false;

	// if we get here, all objects were created successfully
	return true;
}

// Read the key board (called from message peek)
// <LegoRR.exe @0047f1b0>
void __cdecl Gods98::Input_ReadKeys(void)
{
	//uint16 us = 0;

	if (!(Main_FullScreen() ? true : Main_AppActive()))
	{
		// Zero out the keymaps.
		std::memset(INPUT.Key_Map, 0, sizeof(INPUT.Key_Map));
		std::memset(INPUT.prevKey_Map, 0, sizeof(INPUT.prevKey_Map));
		INPUT.lpdiKeyboard->Unacquire();
		return;
	}

	// Is the key device set up?
	if (!INPUT.lpdiKeyboard) return;

	HRESULT hRes;
	if (INPUT.fKeybdAcquired)
	{
		// Zero key map entries
		std::memcpy(INPUT.prevKey_Map, INPUT.Key_Map, sizeof(INPUT.Key_Map));
		std::memset(INPUT.Key_Map, 0, sizeof(INPUT.Key_Map));
		hRes = INPUT.lpdiKeyboard->GetDeviceState(sizeof(INPUT.Key_Map), INPUT.Key_Map);
	}
	else hRes = DIERR_INPUTLOST;

	if (hRes != DI_OK)
	{
		// Opps, we lost control of the keyboard, reacquire
		INPUT.fKeybdAcquired = false;
		if (INPUT.lpdiKeyboard->Acquire() == DI_OK) INPUT.fKeybdAcquired = true;
	}

	// failed to read the keyboard, so just return and hope it comes back!
	return;
}

// Any key pressed (returns number of keys pressed)
// <LegoRR.exe @0047f270>
uint32 __cdecl Gods98::Input_AnyKeyPressed(void)
{
	uint32 ret = 0;
	const bool* lp = INPUT.Key_Map;
	for (uint32 i=0; i<INPUT_MAXKEYS; i++)
	{
		if(*lp)
			ret++;
		lp++;
	}
	return ret;
}

// Release the keys and direct input
// <LegoRR.exe @0047f290>
void __cdecl Gods98::Input_ReleaseKeysAndDI(void)
{
	// If we have control over the keyboard then release it first
	if(INPUT.fKeybdAcquired)
	{
		INPUT.lpdiKeyboard->Unacquire();
		INPUT.fKeybdAcquired = false;
	}
	
	// If the keyboard object exsits then release it
	if(INPUT.lpdiKeyboard != nullptr) INPUT.lpdiKeyboard->Release();

	// If the direct input object exsits then release it
	if(INPUT.lpdi != nullptr) INPUT.lpdi->Release();
}

// Read the mouse state
// <LegoRR.exe @0047f2d0>
void __cdecl Gods98::Input_ReadMouse2(void)
{
	RECT rect;
	if (::GetClientRect(Main_hWnd(), &rect)){

		POINT cursorPos;
		POINT clientPos = { 0, 0 };
		::ClientToScreen(Main_hWnd(), &clientPos);
		::GetCursorPos(&cursorPos);

		cursorPos.x -= clientPos.x;
		cursorPos.y -= clientPos.y;
	
		if (cursorPos.x >= appWidth()) cursorPos.x = appWidth() - 1;
		if (cursorPos.y >= appHeight()) cursorPos.y = appHeight() - 1;
		if (cursorPos.x < 0) cursorPos.x = 0;
		if (cursorPos.y < 0) cursorPos.y = 0;

		INPUT.diffx = cursorPos.x - INPUT.msx;
		INPUT.diffy = cursorPos.y - INPUT.msy;
		INPUT.msx = cursorPos.x;
		INPUT.msy = cursorPos.y;

	}
}

// <LegoRR.exe @0047f390>
BOOL __cdecl Gods98::Input_SetCursorPos(sint32 x, sint32 y)
{
	POINT clientPos = { 0, 0 };
	::ClientToScreen(Main_hWnd(), &clientPos );

	INPUT.msx = x;
	INPUT.msy = y;
	INPUT.diffx = 0;
	INPUT.diffy = 0;

	x += clientPos.x;
	y += clientPos.y;

	return ::SetCursorPos( x, y );
}

#pragma endregion
