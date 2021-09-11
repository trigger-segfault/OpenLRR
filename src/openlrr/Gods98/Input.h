#pragma once

#include "../common.h"


namespace Gods98
{; // !<---

/**********************************************************************************
 ******** Constants
 **********************************************************************************/

#pragma region Constants

// looked at the win98 joystick stuff and it doesn't let you have more than 4 buttons
#define						MAX_JOYSTICK_BUTTONS		4

#define INPUT_MAXKEYS		256

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

struct joystickType
{
	/*00,4*/ bool32					found;
	/*04,4*/ IDirectInputDevice2A* dev;
	/*08,10*/ bool32					buttons[MAX_JOYSTICK_BUTTONS];
	/*18,4*/ sint32						xaxis;
	/*1c,4*/ sint32						yaxis;
	/*20*/
};// joystickType, * lpjoystickType;
static_assert(sizeof(joystickType) == 0x20, "");


struct DIcallbackData
{
	/*0,4*/ IDirectInputA* di;
	/*4,4*/ HWND hWnd;
	/*8,4*/ joystickType* joystick;
	/*c*/
};
static_assert(sizeof(DIcallbackData) == 0xc, "");


struct Input_Globs
{
	// Keyboard state
	/*000,100*/ bool prevKey_Map[INPUT_MAXKEYS];
	/*100,4*/ IDirectInputA* lpdi;
	/*104,4*/ IDirectInputDeviceA* lpdiKeyboard;
	/*108,4*/ IDirectInputDeviceA* lpdiMouse;		// (unused)
	/*10c,4*/ bool32 fKeybdAcquired;
	/*110,4*/ bool32 fMouseAcquired;							// (unused)
	/*114,10*/ RECT MouseBounds;						// Bounding box in which mouse may move
	/*124,4*/ sint32 msx;
	/*128,4*/ sint32 msy;
	/*12c,4*/ sint32 prev_msx;
	/*130,4*/ sint32 prev_msy;		// Current and previous positions of the mouse
	/*134,4*/ sint32 diffx;
	/*138,4*/ sint32 diffy;						// Relative movement of mouse cursor
	/*13c,4*/ bool32 mslb;
	/*140,4*/ bool32 msrb;						// Mouse buttons
	/*144,4*/ bool32 mslblast;
	/*148,4*/ bool32 mslbheld;
	/*14c,4*/ bool32 lClicked;
	/*150,4*/ bool32 rClicked;
	/*154,4*/ bool32 lDoubleClicked;
	/*158,4*/ bool32 rDoubleClicked;
	/*15c,4*/ bool32 caps;
	/*160,10*/ DIMOUSESTATE dims;
	/*170,c*/ DIcallbackData cbd;
	/*17c,1*/ char Input_KeyTemp;
	///*17d,3*/ unsigned char padding1[3];
	/*180,100*/ bool Key_Map[INPUT_MAXKEYS];
	/*280*/
};
static_assert(sizeof(Input_Globs) == 0x280, "");

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @0076ba00>
extern Input_Globs & INPUT;

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// External Function Prototypes

// Mouse data
// <LegoRR.exe @00410a60>
__inline sint32 __cdecl msx() { return INPUT.msx; }		// Mouse positions

// <LegoRR.exe @00410a70>
__inline sint32 __cdecl msy() { return INPUT.msy; }

// <LegoRR.exe @00410a80>
__inline bool32 __cdecl mslb() { return INPUT.mslb; }		// Left and right mouse buttons


// <inlined>
__inline bool32 __cdecl msrb() { return INPUT.msrb; }

// <inlined>
__inline sint32 __cdecl msxr() { return INPUT.diffx; }		// Relative mouse movement

// <inlined>
__inline sint32 __cdecl msyr() { return INPUT.diffy; }

// <inlined>
__inline bool32 __cdecl mslbheld() { return INPUT.mslbheld; }

// <inlined>
__inline bool32 __cdecl Input_LClicked() { return INPUT.lClicked; }



// Initialise direct input and the key board
// <LegoRR.exe @0047f050>
bool32 __cdecl Input_InitKeysAndDI(void);

// Read the key board (called from message peek)
// <LegoRR.exe @0047f1b0>
void __cdecl Input_ReadKeys(void);

// Any key pressed (returns number of keys pressed)
// <LegoRR.exe @0047f270>
uint32 __cdecl Input_AnyKeyPressed(void);

// Release the keys and direct input
// <LegoRR.exe @0047f290>
void __cdecl Input_ReleaseKeysAndDI(void);

// Read the mouse state
// <LegoRR.exe @0047f2d0>
void __cdecl Input_ReadMouse2(void);

// <LegoRR.exe @0047f390>
BOOL __cdecl Input_SetCursorPos(sint32 x, sint32 y);




// Initialize the mouse
bool32 __cdecl Input_InitMouse(bool32 GrabExclusiveAccess, sint32 sx, sint32 sy, RECT Bounds);

/*// Release the mouse
void __cdecl Input_ReleaseMouse(void);


extern	bool32			ReadJoystickInput(IN OUT joystickType*	joystick);
extern	bool32			InitJoystick(HINSTANCE hInst, HWND hWnd, lpjoystickType joystick);

extern	HRESULT			SetDIDwordProperty(LPDIRECTINPUTDEVICE pdev, REFGUID guidProperty, DWORD dwObject, DWORD dwHow, DWORD dwValue);
extern	BOOL __stdcall	InitJoystickInput(LPCDIDEVICEINSTANCE pdinst, LPVOID pvRef);

extern	bool32			ReacquireInput(joystickType* joystick);
extern	void			CleanupJoystickInput(joystickType* joystick);

extern	bool32			IsJoystickInitialised(const joystickType* joystick);
extern	void			Input_ClearClicked(void);
extern	uint8			Input_GetAsciiFromScanCode(uint32 i);


VOID Input_ReadMouse2(VOID);*/

// Static Function Prototypes


//extern UCHAR Input_KeyTemp;
//#define Input_KeyHit(k)			(Input_KeyTemp=prevKey_Map[(k)],prevKey_Map[(k)]=Key_Map[(k)],Key_Map[(k)]&&(Key_Map[(k)]^Input_KeyTemp))
//#define Input_KeyReleased(k)	(Input_KeyTemp=prevKey_Map[(k)],prevKey_Map[(k)]=Key_Map[(k)],(!Key_Map[(k)])&&(Key_Map[(k)]^Input_KeyTemp))
#define Input_KeyHit(k)			(Gods98::INPUT.Key_Map[(k)]&&(Gods98::INPUT.Key_Map[(k)]^Gods98::INPUT.prevKey_Map[(k)]))
#define Input_KeyReleased(k)	((!Gods98::INPUT.Key_Map[(k)])&&(Gods98::INPUT.Key_Map[(k)]^Gods98::INPUT.prevKey_Map[(k)]))
#define Input_KeyIsDown(k)		Gods98::INPUT.Key_Map[(k)]
#define Input_ClearAllKeys()	std::memset(Gods98::INPUT.Key_Map, 0, sizeof(Gods98::INPUT.Key_Map));

#pragma endregion

}

