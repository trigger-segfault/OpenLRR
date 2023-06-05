// Panels.h : 
//

#pragma once

#include "../../common.h"

#include "../GameCommon.h"


namespace LegoRR
{; // !<---

/**********************************************************************************
 ******** Forward Declarations
 **********************************************************************************/

#pragma region Forward Declarations

#pragma endregion

/**********************************************************************************
 ******** Constants
 **********************************************************************************/

#pragma region Constants

#pragma endregion

/**********************************************************************************
 ******** Enumerations
 **********************************************************************************/

#pragma region Enums

enum PanelButtonFlags : uint32
{
	PANELBUTTON_FLAG_NONE = 0,

	PANELBUTTON_FLAG_HOVER      = 0x2, // The button is being hovered over.
	PANELBUTTON_FLAG_DOWN       = 0x4, // The button is down due to mouse interaction.
	PANELBUTTON_FLAG_TOGGLED    = 0x8, // The button is down due to being toggled.
	PANELBUTTON_FLAG_HIDDEN     = 0x10, // The button is not drawn (but can still be interacted with, which is kind of jank).
	PANELBUTTON_FLAG_TOGGLEABLE = 0x20, // The button state can be toggled when pressed or toggled programmatically.
	PANELBUTTON_FLAG_UNUSED_HOVERFLASH = 0x40, // Drawing behaviour for FlashingState.
};
flags_end(PanelButtonFlags, 0x4);


enum PanelDataFlags : uint32
{
	PANEL_FLAG_NONE = 0,

	PANEL_FLAG_HASIMAGE = 0x1, // Panel successfully loaded an image or flic.
	PANEL_FLAG_OPEN     = 0x2, // Use xyOut for position during Panel_SetSlidingPositions.
	PANEL_FLAG_CLOSED   = 0x4, // Use xyIn for position during Panel_Draw.
	PANEL_FLAG_SLIDING  = 0x8, // Panel is transitioning between open or closed state.
};
flags_end(PanelDataFlags, 0x4);


enum Panel_GlobFlags : uint32
{
	PANEL_GLOB_FLAG_NONE = 0,

	PANEL_GLOB_FLAG_ROTATECONTROL_ACTIVE = 0x1,
	PANEL_GLOB_FLAG_PRIORITIES_TOP       = 0x2,
	PANEL_GLOB_FLAG_PRIORITIES_UP        = 0x4,

	PANEL_GLOB_FLAG_ROTATECONTROL_UP     = 0x10,
	PANEL_GLOB_FLAG_ROTATECONTROL_DOWN   = 0x20,
	PANEL_GLOB_FLAG_ROTATECONTROL_LEFT   = 0x40,
	PANEL_GLOB_FLAG_ROTATECONTROL_RIGHT  = 0x80,
};
flags_end(Panel_GlobFlags, 0x4);

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

struct PanelButtonData // [LegoRR/Panels.c|struct:0x2c]
{
	/*00,10*/	Rect2F rect;
	/*10,4*/	Gods98::Image* imageHover;
	/*14,4*/	Gods98::Image* imagePressed;
	/*18,4*/	Gods98::Image* image;
	/*1c,4*/	undefined4 reserved1; // Unused and unset?
	/*20,4*/	ToolTip_Type toolTipType;
	/*24,4*/	uint32 clickCount;
	/*28,4*/	PanelButtonFlags flags;
};
assert_sizeof(PanelButtonData, 0x2c);


struct PanelData // [LegoRR/Panels.c|struct:0x30]
{
	/*00,4*/	Gods98::Image* imageOrFlic;
	/*04,4*/	bool32 isFlic;
	/*08,8*/	Point2F openPos; // (xyOut)
	/*10,8*/	Point2F closedPos; // (xyIn)
	/*18,8*/	Point2F position;
	/*20,4*/	undefined4 reserved1; // Unused, but set to 0.
	/*24,4*/	PanelButtonData* buttonList;
	/*28,4*/	uint32 buttonCount;
	/*2c,4*/	PanelDataFlags flags;
	/*30*/
};
assert_sizeof(PanelData, 0x30);


struct PanelTextWindow // [LegoRR/Panels.c|struct:0x10] A Panel-based wrapper around the TextWindow class
{
	/*00,4*/	Gods98::TextWindow* textWindow;
	/*04,8*/	Point2F position;
	/*0c,4*/	PanelData* panel;
	/*10*/
};
assert_sizeof(PanelTextWindow, 0x10);


struct Panel_Globs // [LegoRR/Panels.c|struct:0x7b8|tags:GLOBS]
{
	/*000,30*/	const char* panelName[Panel_Type_Count];
	/*030,240*/	PanelData panelTable[Panel_Type_Count];
	/*270,480*/	const char* panelButtonName[Panel_Type_Count][PanelButton_Type_Count];
	/*6f0,4*/	Panel_Type currentPanel;
	/*6f4,4*/	PanelButton_Type currentButton;
	/*6f8,4*/	PanelTextWindow* infoTextWnd;
	/*6fc,4*/	PanelTextWindow* encyclopediaTextWnd;
	/*700,4*/	Gods98::Image* crystalSmallImage; // Hard: "Interface\\RightPanel\\SmallCrystal.bmp"
	/*704,4*/	Gods98::Image* crystalNoSmallImage; // Hard: "Interface\\RightPanel\\NoSmallCrystal.bmp"
	/*708,4*/	Gods98::Image* crystalUsedImage; // Hard: "Interface\\RightPanel\\UsedCrystal.bmp"
	/*70c,4*/	uint32 crystalQuota;
	/*710,4*/	Gods98::Image* airMeterJuiceImage; // Hard: "Interface\\AirMeter\\msgpanel_air_juice.bmp"
	/*714,4*/	Gods98::Image* airMeterNoAirImage; // Hard: "Interface\\AirMeter\\msgpanel_noair.bmp"
	/*718,8*/	Point2F airMeterJuiceOffset; // Hard: (85, 6) Offset relative to top-left corner of MsgPanel
	/*720,4*/	uint32 airMeterJuiceLength; // Hard: 236
	/*724,8*/	Point2F airMeterNoAirOffset; // Hard: (21, 0) Offset relative to top-left corner of MsgPanel
	/*72c,4*/	bool32 airMeterOxygenLow;
	/*730,4*/	Gods98::Image* cryOreSideBarImage; // Hard: "Interface\\RightPanel\\crystalsidebar_ore.bmp"
	/*734,8*/	Point2F cryOreSideBarOffset; // Hard: (615, 434)
	/*73c,4*/	uint32 cryOreMeterOffset; // Hard: 423
	/*740,4*/	real32 cryOreMeterValue;
	/*744,8*/	Point2I rotateCenter;
	/*74c,4*/	sint32 rotateRadius;
	/*750,8*/	Point2F rotateUpOffset;
	/*758,8*/	Point2F rotateDownOffset;
	/*760,8*/	Point2F rotateLeftOffset;
	/*768,8*/	Point2F rotateRightOffset;
	/*770,4*/	Gods98::Image* rotateUpImage;
	/*774,4*/	Gods98::Image* rotateDownImage;
	/*778,4*/	Gods98::Image* rotateLeftImage;
	/*77c,4*/	Gods98::Image* rotateRightImage;
	/*780,4*/	Panel_GlobFlags flags;
	/*784,4*/	undefined4 reserved;
	/*788,30*/	uint32 s_crystalShifts[2][6]; // static function variable for delayed change in crystal meter
	/*7b8*/
};
assert_sizeof(Panel_Globs, 0x7b8);

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @005010e0>
extern Panel_Globs & panelGlobs;

#pragma endregion

/**********************************************************************************
 ******** Macros
 **********************************************************************************/

#pragma region Macros

#define Panel_RegisterName(n)				(panelGlobs.panelName[n]=#n)
#define Panel_RegisterButtonName(n,bn)		(panelGlobs.panelButtonName[n][bn]=#bn)

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

/// CUSTOM: Hook function to overwrite Panel maximum radarmap zoom level.
void Panel_RadarMap_ZoomIn(real32 elapsedInterface);

/// CUSTOM: Hook function to overwrite Panel minimum radarmap zoom level.
void Panel_RadarMap_ZoomOut(real32 elapsedInterface);


// <inlined>
__inline bool32 Panel_AirMeter_IsOxygenLow(void) { return panelGlobs.airMeterOxygenLow; }



// <LegoRR.exe @0045a2f0>
#define Panel_Initialise ((void (__cdecl* )(void))0x0045a2f0)

// <LegoRR.exe @0045a500>
#define Panel_LoadInterfaceButtons_ScrollInfo ((void (__cdecl* )(void))0x0045a500)

// <LegoRR.exe @0045a530>
#define Panel_ResetAll ((void (__cdecl* )(void))0x0045a530)

// <LegoRR.exe @0045a5a0>
#define Panel_LoadImage ((void (__cdecl* )(const char* filename, Panel_Type panelType, PanelDataFlags flags))0x0045a5a0)

// <LegoRR.exe @0045a630>
#define Panel_GetPanelType ((bool32 (__cdecl* )(const char* panelName, OUT Panel_Type* panelType))0x0045a630)

// <LegoRR.exe @0045a670>
#define Panel_TestPointInsideImage ((bool32 (__cdecl* )(Panel_Type panelType, sint32 screenX, sint32 screenY))0x0045a670)

// <LegoRR.exe @0045a6d0>
#define Panel_GetButtonType ((bool32 (__cdecl* )(Panel_Type panelType, const char* buttonName, OUT PanelButton_Type* buttonType))0x0045a6d0)

// <LegoRR.exe @0045a720>
#define Panel_PrintF ((uint32 (__cdecl* )(Panel_Type panelType, Gods98::Font* font, sint32 x, sint32 y, bool32 center, const char* msg, ...))0x0045a720)

// <LegoRR.exe @0045a7c0>
#define Panel_TextWindow_Create ((PanelTextWindow* (__cdecl* )(Panel_Type panelType, Gods98::Font* font, const Area2F* rect, uint32 size))0x0045a7c0)

// <LegoRR.exe @0045a850>
#define Panel_TextWindow_PrintF ((void (__cdecl* )(PanelTextWindow* panelWnd, const char* msg, ...))0x0045a850)

// <LegoRR.exe @0045a870>
#define Panel_TextWindow_Update ((void (__cdecl* )(PanelTextWindow* textWnd, uint32 posFromEnd, real32 elapsed))0x0045a870)

// <LegoRR.exe @0045a8e0>
#define Panel_TextWindow_Clear ((void (__cdecl* )(PanelTextWindow* panelWnd))0x0045a8e0)

// <LegoRR.exe @0045a8f0>
#define Panel_TextWindow_GetInfo ((void (__cdecl* )(PanelTextWindow* panelWnd, OUT uint32* linesCount, OUT uint32* linesCapacity))0x0045a8f0)

// <LegoRR.exe @0045a910>
#define Panel_SetSlidingPositions ((void (__cdecl* )(Panel_Type panelType, sint32 xOpen, sint32 yOpen, sint32 xClosed, sint32 yClosed))0x0045a910)

// <LegoRR.exe @0045a9a0>
#define Panel_SetPosition ((void (__cdecl* )(Panel_Type panelType, real32 x, real32 y))0x0045a9a0)

// <LegoRR.exe @0045a9c0>
#define Panel_GetPosition ((void (__cdecl* )(Panel_Type panelType, OUT real32* x, OUT real32* y))0x0045a9c0)

// <LegoRR.exe @0045a9f0>
#define Panel_Draw ((void (__cdecl* )(Panel_Type panelType, real32 elapsedAbs))0x0045a9f0)

// <LegoRR.exe @0045ac80>
#define Panel_DrawButtons ((void (__cdecl* )(Panel_Type panelType))0x0045ac80)

// <LegoRR.exe @0045ad80>
#define Panel_Button_Hide ((void (__cdecl* )(Panel_Type panelType, PanelButton_Type buttonType, bool32 hide))0x0045ad80)

// <LegoRR.exe @0045adc0>
#define Panel_ToggleOpenClosed ((void (__cdecl* )(Panel_Type panelType))0x0045adc0)

// The panel is in its closed state and is NOT sliding.
// <LegoRR.exe @0045adf0>
#define Panel_IsFullyClosed ((bool32 (__cdecl* )(Panel_Type panelType))0x0045adf0)

// The panel is in its opened state and is NOT sliding.
// <LegoRR.exe @0045ae20>
#define Panel_IsFullyOpen ((bool32 (__cdecl* )(Panel_Type panelType))0x0045ae20)

/// CUSTOM: The panel is in its closed or closing (sliding) state.
inline bool Panel_IsClosed(Panel_Type panelType) { return panelGlobs.panelTable[panelType].flags & PANEL_FLAG_CLOSED; };

/// CUSTOM: The panel is in its opened or opening (sliding) state.
inline bool Panel_IsOpen(Panel_Type panelType) { return panelGlobs.panelTable[panelType].flags & PANEL_FLAG_OPEN; };

// The panel is sliding in or out of an open/closed state.
// <LegoRR.exe @0045ae50>
#define Panel_IsSliding ((bool32 (__cdecl* )(Panel_Type panelType))0x0045ae50)

// Only used for TopPanel buttons Priorities and CallToArms to keep the button indented while active.
// <LegoRR.exe @0045ae70>
#define Panel_Button_SetToggleable ((void (__cdecl* )(Panel_Type panelType, PanelButton_Type buttonType, bool32 toggleable))0x0045ae70)

// Requires Panel_Button_SetToggleable to set the toggleable flag to true.
// <LegoRR.exe @0045aeb0>
#define Panel_Button_SetToggled ((void (__cdecl* )(Panel_Type panelType, PanelButton_Type buttonType, bool32 toggled))0x0045aeb0)

// <LegoRR.exe @0045aef0>
#define Panel_CreateButtons ((void (__cdecl* )(Panel_Type panelType, uint32 count, const PanelButton_Type* buttonTypes, const Area2F* areas, char** imageNames, char** imageHoverNames, char** imagePressedNames, const ToolTip_Type* toolTips))0x0045aef0)

// <LegoRR.exe @0045b070>
#define Panel_CheckCollision ((bool32 (__cdecl* )(real32 elapsedAbs, uint32 mouseX, uint32 mouseY, bool32 leftButton, bool32 leftButtonLast, OUT bool32* panelCollision))0x0045b070)

// <LegoRR.exe @0045b5d0>
#define Panel_CheckButtonCollision ((bool32 (__cdecl* )(OUT Panel_Type* panelType, OUT PanelButton_Type* panelButton, sint32 mouseX, sint32 mouseY, bool32 leftButton, bool32 leftButtonLast, OPTIONAL OUT bool32* param_7, OPTIONAL OUT bool32* panelCollision))0x0045b5d0)

// <LegoRR.exe @0045b850>
#define Panel_TestPointInsideRect ((bool32 (__cdecl* )(PanelData* panel, const Rect2F* rect, sint32 mouseX, sint32 mouseY))0x0045b850)

// <LegoRR.exe @0045b8d0>
#define Panel_PriorityList_ResetButtons ((void (__cdecl* )(void))0x0045b8d0)

// <LegoRR.exe @0045b8e0>
#define Panel_PriorityList_HandleButton ((void (__cdecl* )(PanelButton_Type buttonType))0x0045b8e0)

// <LegoRR.exe @0045ba00>
#define Panel_ScrollInfo_Initialise ((void (__cdecl* )(void))0x0045ba00)

// <LegoRR.exe @0045bb10>
#define Panel_Encyclopedia_Initialise ((void (__cdecl* )(void))0x0045bb10)

// <LegoRR.exe @0045bb60>
#define Panel_TestPointInsideCircle ((bool32 (__cdecl* )(sint32 x, sint32 y, sint32 centerX, sint32 centerY, sint32 radius))0x0045bb60)

// <LegoRR.exe @0045bbc0>
#define Panel_RotationControl_NormalizePointRadius ((void (__cdecl* )(sint32 x, sint32 y, sint32 radius, OUT real32* out_x, OUT real32* out_y))0x0045bbc0)

// <LegoRR.exe @0045bbf0>
#define Panel_RotationControl_ClampPointInsideCircle ((void (__cdecl* )(IN OUT sint32* mouseX, IN OUT sint32* mouseY, sint32 centerX, sint32 centerY, sint32 radius))0x0045bbf0)

// <LegoRR.exe @0045bc90>
#define Panel_RotationControl_Initialise ((void (__cdecl* )(const Gods98::Config* config, const char* gameName))0x0045bc90)

// <LegoRR.exe @0045bf90>
#define Panel_RotationControl_HandleRotation ((bool32 (__cdecl* )(sint32 mouseX, sint32 mouseY, real32 elapsedAbs))0x0045bf90)

// <LegoRR.exe @0045c1e0>
#define Panel_RotationControl_HandleButtons ((void (__cdecl* )(PanelButton_Type buttonType, real32 elapsedAbs))0x0045c1e0)

// <LegoRR.exe @0045c230>
#define Panel_Button_GetArea ((void (__cdecl* )(Panel_Type panelType, PanelButton_Type buttonType, OUT Area2F* area))0x0045c230)

// <LegoRR.exe @0045c270>
#define Panel_Crystals_Initialise ((void (__cdecl* )(const char* smallCrystal, const char* usedCrystal, const char* noSmallCrystal))0x0045c270)

// <LegoRR.exe @0045c300>
#define Panel_Crystals_LoadRewardQuota ((void (__cdecl* )(const Gods98::Config* config, const char* gameName, const char* levelName))0x0045c300)

// <LegoRR.exe @0045c390>
#define Panel_Crystals_Draw ((void (__cdecl* )(uint32 crystals, uint32 usedCrystals, real32 elapsedGame_unused))0x0045c390)

// <LegoRR.exe @0045c600>
#define Panel_AirMeter_Initialise ((void (__cdecl* )(const char* airJuiceName, uint32 juiceX, uint32 juiceY, uint32 juiceLength, const char* noAirName, uint32 noAirX, uint32 noAirY))0x0045c600)

// <LegoRR.exe @0045c6b0>
#define Panel_AirMeter_DrawJuice ((void (__cdecl* )(Panel_Type panelType, real32 oxygen))0x0045c6b0)

// Tells the AirMeter panel if the Oxygen level is low (10% or less).
// <LegoRR.exe @0045c760>
#define Panel_AirMeter_SetOxygenLow ((void (__cdecl* )(bool32 o2Low))0x0045c760)

// <LegoRR.exe @0045c770>
#define Panel_AirMeter_DrawOxygenLow ((void (__cdecl* )(Panel_Type panelType))0x0045c770)

// <LegoRR.exe @0045c7e0>
#define Panel_CryOreSideBar_Initialise ((void (__cdecl* )(const char* sidebarName, uint32 xPos, uint32 yPos, uint32 meterOffset))0x0045c7e0)

// <LegoRR.exe @0045c840>
#define Panel_CryOreSideBar_ChangeOreMeter ((void (__cdecl* )(bool32 increment, uint32 amount))0x0045c840)

// <LegoRR.exe @0045c8b0>
#define Panel_CryOreSideBar_Draw ((void (__cdecl* )(void))0x0045c8b0)

// <LegoRR.exe @0045c930>
#define Panel_SetCurrentAdvisorFromButton ((bool32 (__cdecl* )(Panel_Type panelType, PanelButton_Type buttonType, bool32 setFlag2))0x0045c930)

// <LegoRR.exe @0045c970>
#define Panel_GetAdvisorTypeFromButton ((bool32 (__cdecl* )(Panel_Type panelType, PanelButton_Type buttonType, OUT Advisor_Type* advisorType))0x0045c970)

#pragma endregion

}
