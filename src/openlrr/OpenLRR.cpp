// OpenLRR.cpp : 
//

#include "platform/windows.h"
#include "../../../resources/resource.h"

#include "OpenLRR.h"
#include "hook.h"
#include "interop.h"
#include "dllmain.h"

#include "engine/drawing/Draw.h"
#include "engine/drawing/Fonts.h"
#include "engine/drawing/Images.h"
#include "engine/Graphics.h"
#include "engine/Main.h"
#include "game/Debug.h"
#include "game/Game.h"
#include "game/Shortcuts.hpp"
#include "game/effects/LightEffects.h"
#include "game/front/FrontEnd.h"
#include "game/interface/hud/Bubbles.h"
#include "game/interface/HelpWindow.h"
#include "game/interface/InfoMessages.h"
#include "game/object/Dependencies.h"
#include "game/world/Camera.h"
#include "game/world/Map3D.h"


/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

OpenLRR_Globs openlrrGlobs = { nullptr };

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

FILE* MakeConsole(void)
{
    FILE* pConsoleFile = nullptr;
    ::AllocConsole();
    ::freopen_s(&pConsoleFile, "CONOUT$", "w", stdout);
    ::SetConsoleTitleA("");
    CONSOLE_CURSOR_INFO conCursorInfo;
    ::GetConsoleCursorInfo(::GetStdHandle(STD_OUTPUT_HANDLE), &conCursorInfo);
    conCursorInfo.bVisible = false;
    ::SetConsoleCursorInfo(::GetStdHandle(STD_OUTPUT_HANDLE), &conCursorInfo);
    return pConsoleFile;
}

bool InjectOpenLRR(HINSTANCE hInstanceDll)
{
    // Store our dll instance so we can access it for loading custom OpenLRR resources later on.
    openlrrGlobs.hInstDll = hInstanceDll;
    openlrrGlobs.method = InjectMethod::DllImport;


    // Determine if openlrr-injector was used to load this dll.
    static constexpr const uint8 eipPatch[2]        = { 0xeb,0xfe }; // infinite jmp to itself

	// Determine if openlrr "Start" executable was used to load this dll.
	static constexpr const uint8 winmainStart[6]    = { 0x55,0x8b,0xec,0xff,0x74,0x24 };

	if (hook_cmp(PROCESS_EIP, eipPatch, sizeof(eipPatch)) == 0) {
		openlrrGlobs.method = InjectMethod::Injector;
	}
	else if (hook_cmp(PROCESS_WINMAIN, winmainStart, sizeof(winmainStart)) == 0) {
		openlrrGlobs.method = InjectMethod::DllStart;
		// In this scenario we do not want to touch the WinMain assembly, return here.
		return true;
	}


	// Redirect `LegoRR.exe!WinMain` to immediately jump to `openlrr.dll!StartOpenLRR`.
	bool result = interop_hook_WinMain();
	//bool result = hook_write_jmpret(PROCESS_WINMAIN, StartOpenLRRInjected);

    return result;
}


#pragma region OpenLRR_UpdateMenuItems

#define Menu_EnableButton(menuID, state) \
    ::EnableMenuItem(Gods98::Main_GetMenu(), (menuID), MF_BYCOMMAND|((state)?MF_ENABLED:(MF_GRAYED|MF_DISABLED)))

#define Menu_CheckButton(menuID, state) \
    ::CheckMenuItem(Gods98::Main_GetMenu(), (menuID), MF_BYCOMMAND|((state)?MF_CHECKED:MF_UNCHECKED))

#define Menu_EnableButtons(menuIDs, count, state) \
    for (sint32 ii = 0; ii < static_cast<sint32>(count); ii++) Menu_EnableButton((menuIDs)[ii], state)

#define Menu_CheckButtons(menuIDs, count, state) \
    for (sint32 ii = 0; ii < static_cast<sint32>(count); ii++) Menu_CheckButton((menuIDs)[ii], state)

#define Menu_CheckRadioButtons(menuIDs, count, index) \
    for (sint32 ii = 0; ii < static_cast<sint32>(count); ii++) Menu_CheckButton((menuIDs)[ii], (ii==(index)))

#define Menu_EnableButtonsArray(menuIDs, state) Menu_EnableButtons(menuIDs, menuIDs.size(), state)
#define Menu_CheckButtonsArray(menuIDs, state)  Menu_CheckButtons(menuIDs, menuIDs.size(), state)
#define Menu_CheckRadioButtonsArray(menuIDs, index) Menu_CheckRadioButtons(menuIDs, menuIDs.size(), index)


static constexpr const auto Menu_RadarMapScaleIDs = array_of<uint32>(IDM_RADARMAPSCALE_X1, IDM_RADARMAPSCALE_X2, IDM_RADARMAPSCALE_X3, IDM_RADARMAPSCALE_X4);
static constexpr const auto Menu_ScaleIDs = array_of<uint32>(IDM_WINDOWSCALE_X1, IDM_WINDOWSCALE_X2, IDM_WINDOWSCALE_X3, IDM_WINDOWSCALE_X4);
static constexpr const auto Menu_IconIDs = array_of<uint32>(IDM_ICON_NONE, IDM_ICON_NATIVE, IDM_ICON_OPENLRR, IDM_ICON_GOLD, IDM_ICON_TEAL, IDM_ICON_TEALRR);
static constexpr const auto Menu_CursorClippingIDs = array_of<uint32>(IDM_CURSORCLIPPING_OFF, IDM_CURSORCLIPPING_MENU, IDM_CURSORCLIPPING_GAMEAREA);
static constexpr const auto Menu_CursorIDs = array_of<uint32>(IDM_CURSOR_NEVER, IDM_CURSOR_TITLEBAR, IDM_CURSOR_ALWAYS);
static constexpr const auto Menu_SelectPlaceArrowIDs = array_of<uint32>(IDM_SELECTPLACEARROW_NEVER, IDM_SELECTPLACEARROW_SOLIDONLY, IDM_SELECTPLACEARROW_ALWAYS);
static constexpr const auto Menu_TopdownLODIDs = array_of<uint32>(IDM_TOPDOWNLOD_LOW, IDM_TOPDOWNLOD_MEDIUM, IDM_TOPDOWNLOD_HIGH);
static constexpr const auto Menu_QualityIDs = array_of<uint32>(IDM_QUALITY_WIREFRAME, IDM_QUALITY_UNLITFLAT, IDM_QUALITY_FLAT, IDM_QUALITY_GOURAUD, IDM_QUALITY_PHONG);
static constexpr const auto Menu_AdvanceFrameIDs = array_of<uint32>(IDM_ADVANCE_1FRAME, IDM_ADVANCE_1SECOND);
static constexpr const auto Menu_RoutingAutoIDs = array_of<uint32>(IDM_ROUTING_AUTO_NONE, IDM_ROUTING_AUTO_TRACKED, IDM_ROUTING_AUTO_ALLFRIENDLY, IDM_ROUTING_AUTO_ALL);
static constexpr const auto Menu_InitCommandLineIDs = array_of<uint32>(IDM_DUALMOUSE, IDM_PROGRAMMER, IDM_DEBUGMODE, IDM_DEBUGCOMPLETE, IDM_LEVELSOPEN, IDM_TESTERCALL, IDM_BLOCKFADE, IDM_DUMPMODE, IDM_FREEZE);

//static_assert(Menu_ScaleIDs.size() == 4, "Mismatched Menu_ScaleIDs array size");
static_assert(Menu_IconIDs.size() == openlrrGlobs.iconList.size(), "Mismatched Menu_IconIDs array size");
static_assert(Menu_CursorIDs.size() == static_cast<size_t>(Gods98::CursorVisibility::Count), "Mismatched Menu_CursorIDs array size");
static_assert(Menu_CursorClippingIDs.size() == static_cast<size_t>(Gods98::CursorClipping::Count), "Mismatched Menu_CursorClippingIDs array size");
static_assert(Menu_QualityIDs.size() == static_cast<size_t>(Gods98::Graphics_Quality::Count), "Mismatched Menu_QualityIDs array size");

static constexpr const auto Menu_LegoInitIDs = array_of<uint32>(
	IDM_MUSICON, IDM_SOUNDON, IDM_HELPWINDOW, IDM_AUTOGAMESPEED, IDM_LOSEFOCUSANDPAUSE,
	IDM_SHOWOBJINFO, IDM_RENDERPANELS, IDM_TOOLTIPSOUND, IDM_LIGHTEFFECTS, IDM_DETAILON, IDM_DYNAMICPM, IDM_ALLOWDEBUGKEYS,
	IDM_ALLOWEDITMODE, IDM_SHOWDEBUGTOOLTIPS, IDM_DDRAWCLEAR, IDM_FPSMONITOR, IDM_MEMORYMONITOR, IDM_NONERPS, IDM_UNLOCKCAMERA,
	IDM_UNLOCKBUILD, IDM_BUILDWITHOUTPATHS, IDM_FPNOCLIP, IDM_NOROCKFALL, IDM_QUICKREINFORCE,
	IDM_SELECTPLACEARROW_NEVER, IDM_SELECTPLACEARROW_SOLIDONLY, IDM_SELECTPLACEARROW_ALWAYS,

	IDM_RENDERING_DRAW, IDM_RENDERING_IMAGES, IDM_RENDERING_FONTS,
	IDM_TRANSPARENTMULTISELECT, IDM_TOPDOWNLOD_LOW, IDM_TOPDOWNLOD_MEDIUM, IDM_TOPDOWNLOD_HIGH,
	IDM_NOPOWERCONSUMPTION, IDM_NOOXYGENCONSUMPTION, IDM_SUPERTOOLSTORE, IDM_SHOWTOOLTIPS
);

static constexpr const auto Menu_InLevelIDs = array_of<uint32>(
	IDM_BUILDANYROUGHNESS, IDM_PEACEFUL, IDM_SURVEYLEVEL, IDM_ALLOWRENAME, IDM_DISABLEENDTELEPORT,
	IDM_GENERATESPIDERS, IDM_NOAUTOEAT, IDM_NOFALLINS, IDM_NOMULTISELECT, IDM_SAFECAVERNS, IDM_SEETHROUGHWALLS,
	IDM_ADDCRYSTALS, IDM_SUBCRYSTALS, IDM_ADDORE, IDM_SUBORE, IDM_ADDOXYGEN, IDM_SUBOXYGEN,

	IDM_SELECTEDLEVELUP, IDM_SELECTEDHEAL, IDM_SELECTEDTRAIN,

	IDM_ROUTING_ADDSELECTED, IDM_ROUTING_ADDALLFRIENDLY, IDM_ROUTING_ADDALL, IDM_ROUTING_REMOVESELECTED, IDM_ROUTING_REMOVEALL
);

// Update menu item enabled/checked states.
void __cdecl OpenLRR_UpdateMenuItems(void)
{
    // Disable menu items that are waiting for initialisation.
    Menu_EnableButtonsArray(Menu_InitCommandLineIDs, Gods98::Main_IsCommandLineParsed());

	Menu_EnableButtonsArray(Menu_LegoInitIDs, LegoRR::Lego_IsInit());

	Menu_EnableButtonsArray(Menu_InLevelIDs, LegoRR::Lego_IsInLevel());


    ////// &File //////

    Menu_CheckButton(IDM_TOGGLEMENU, (Gods98::Main_GetMenu() != nullptr));
    // disable until we add the WinConsole module
    Menu_EnableButton(IDM_TOGGLECONSOLE, false);


    ////// &Options //////
	
	Menu_CheckButton(IDM_MUSICON,			(LegoRR::Lego_IsInit() && LegoRR::Lego_IsMusicOn()));
	Menu_CheckButton(IDM_SOUNDON,			(LegoRR::Lego_IsInit() && LegoRR::Lego_IsSoundOn()));
	Menu_CheckButton(IDM_HELPWINDOW,		(LegoRR::Lego_IsInit() && HelpWindow_IsEnabled()));
	Menu_CheckButton(IDM_AUTOGAMESPEED,		(LegoRR::Lego_IsInit() && LegoRR::Info_IsAutoGameSpeed()));
	Menu_CheckButton(IDM_DUALMOUSE,			Gods98::Main_IsDualMouse());
	Menu_CheckButton(IDM_LOSEFOCUSANDPAUSE,	(LegoRR::Lego_IsInit() && LegoRR::Lego_IsLoseFocusAndPause()));

	Menu_CheckButton(IDM_SHOWOBJINFO,	(LegoRR::Lego_IsInit() && LegoRR::Bubble_GetObjectUIsAlwaysVisible()));
	Menu_CheckButton(IDM_RENDERPANELS,	(LegoRR::Lego_IsInit() && LegoRR::Lego_IsRenderPanels()));
	Menu_CheckButton(IDM_SHOWTOOLTIPS,	(LegoRR::Lego_IsInit() && LegoRR::Lego_IsShowToolTips()));
	Menu_CheckButton(IDM_TOOLTIPSOUND,	(LegoRR::Lego_IsInit() && !LegoRR::Lego_IsDisableToolTipSound()));

	Menu_CheckButton(IDM_TRANSPARENTMULTISELECT,	(LegoRR::Lego_IsInit() && LegoRR::Lego_IsTransparentMultiSelectBox()));
	Menu_CheckButton(IDM_LIGHTEFFECTS,	(LegoRR::Lego_IsInit() && !LegoRR::gamectrlGlobs.dbgF9DisableLightEffects));
	Menu_CheckButton(IDM_DETAILON,		(LegoRR::Lego_IsInit() && LegoRR::Lego_IsDetailOn()));
	Menu_CheckButton(IDM_DYNAMICPM,		(LegoRR::Lego_IsInit() && LegoRR::Lego_IsDynamicPM()));
	Menu_CheckButton(IDM_TOPDOWNFOG,	(LegoRR::Lego_IsInit() && LegoRR::Lego_IsTopdownFogOn()));

    // These can safely be out of range, beacuse of how the Buttons macros function.
	if (LegoRR::Lego_IsInit()) {
		sint32 curArrow = static_cast<sint32>(LegoRR::SelectPlace_GetArrowVisibility(LegoRR::legoGlobs.selectPlace));
		Menu_CheckRadioButtonsArray(Menu_SelectPlaceArrowIDs, curArrow);
	}

    sint32 curCursor = static_cast<sint32>(Gods98::Main_GetCursorVisibility());
    Menu_CheckRadioButtonsArray(Menu_CursorIDs, curCursor);

    sint32 curCursorClipping = static_cast<sint32>(Gods98::Main_GetCursorClipping());
    Menu_CheckRadioButtonsArray(Menu_CursorClippingIDs, curCursorClipping);

    sint32 curRouteAuto = static_cast<sint32>(LegoRR::Debug_RouteVisual_GetAutoMode());
    Menu_CheckRadioButtonsArray(Menu_RoutingAutoIDs, curRouteAuto);

	sint32 curTopdownLOD = static_cast<sint32>(LegoRR::Lego_GetTopdownLOD());
	Menu_CheckRadioButtonsArray(Menu_TopdownLODIDs, curTopdownLOD);

	Menu_CheckRadioButtonsArray(Menu_QualityIDs,   static_cast<sint32>(Gods98::Graphics_GetRenderQuality()));
	Menu_CheckButton(IDM_GRAPHICS_BLEND,           Gods98::Graphics_IsBlendTransparency());
	Menu_CheckButton(IDM_GRAPHICS_DITHER,          Gods98::Graphics_IsDither());
	Menu_CheckButton(IDM_GRAPHICS_FILTER,          Gods98::Graphics_IsLinearFilter());
	Menu_CheckButton(IDM_GRAPHICS_LINEARMIPMAP,    Gods98::Graphics_IsMIPMapLinear());
	Menu_CheckButton(IDM_GRAPHICS_MIPMAP,          Gods98::Graphics_IsMIPMap());
	Menu_CheckButton(IDM_GRAPHICS_SORT,            Gods98::Graphics_IsSortTransparency());
	Menu_CheckButton(IDM_GRAPHICS_ALPHAMODULATION, Gods98::Graphics_IsAlphaModulation());

	Menu_CheckButton(IDM_LOGGING_DEBUG,    Gods98::Error_IsDebugVisible());
	Menu_CheckButton(IDM_LOGGING_TRACE,    Gods98::Error_IsTraceVisible());
	Menu_CheckButton(IDM_LOGGING_INFO,     Gods98::Error_IsInfoVisible());
	Menu_CheckButton(IDM_LOGGING_WARNING,  Gods98::Error_IsWarnVisible());
	Menu_CheckButton(IDM_LOGGING_FATAL,    Gods98::Error_IsFatalVisible());

    sint32 curIcon = -1;
    for (size_t i = 0; i < Menu_IconIDs.size(); i++) {
        // Disable menu items for unavailable icons.
        if (static_cast<OpenLRRIcon>(i) != OpenLRRIcon::None && !openlrrGlobs.iconList[i])
            Menu_EnableButton(Menu_IconIDs[i], false);
        else if (openlrrGlobs.iconList[i] == Gods98::Main_GetIcon() && curIcon == -1)
            curIcon = i;
    }
    Menu_CheckRadioButtonsArray(Menu_IconIDs, curIcon);

	for (size_t i = 0; i < Menu_ScaleIDs.size(); i++) {
		Menu_EnableButton((IDM_WINDOWSCALE_X1 + i), Gods98::Main_IsScaleSupported(i + 1)); // scale is "1-indexed"
	}
	Menu_CheckRadioButtonsArray(Menu_ScaleIDs, (Gods98::Main_Scale() - 1)); // scale is "1-indexed"

	// Disable changing scale in this mode, it will break things. i.e. The clipper and d3drm device, so wait until that's fixed.
	Menu_EnableButtonsArray(Menu_ScaleIDs, !Gods98::Main_IsRenderScaling());

	Menu_CheckRadioButtonsArray(Menu_RadarMapScaleIDs, (Gods98::Main_RadarMapScale() - 1)); // scale is "1-indexed"

	// Disable radar scaling for resolutions higher than window.
	for (size_t i = static_cast<size_t>(Gods98::Main_RenderScale()); i < Menu_RadarMapScaleIDs.size(); i++) {
		Menu_EnableButton(Menu_RadarMapScaleIDs[i], false);
	}

	Menu_CheckButton(IDM_ROUTING_ENABLED,			(LegoRR::Debug_RouteVisual_IsEnabled()));
	Menu_CheckButton(IDM_ROUTING_COMPLETEDPATHS,	(LegoRR::Debug_RouteVisual_IsCompletedPathsEnabled()));
	Menu_CheckButton(IDM_ROUTING_CURVEPATHS,		(LegoRR::Debug_RouteVisual_IsCurvePathsEnabled()));


    ////// &Debug //////
	Menu_CheckButton(IDM_PROGRAMMER,    (Gods98::Main_ProgrammerMode() != 0));
	Menu_CheckButton(IDM_DEBUGMODE,     Gods98::Main_IsDebugMode());
	Menu_CheckButton(IDM_DEBUGCOMPLETE, Gods98::Main_IsDebugComplete());
	Menu_CheckButton(IDM_LEVELSOPEN,    Gods98::Main_IsLevelsOpen());
	Menu_CheckButton(IDM_TESTERCALL,    Gods98::Main_IsTesterCall());

	Menu_CheckButton(IDM_ALLOWDEBUGKEYS,	(LegoRR::Lego_IsInit() && LegoRR::Lego_IsAllowDebugKeys()));
	Menu_CheckButton(IDM_ALLOWEDITMODE,		(LegoRR::Lego_IsInit() && LegoRR::Lego_IsAllowEditMode()));
	Menu_CheckButton(IDM_SHOWDEBUGTOOLTIPS,	(LegoRR::Lego_IsInit() && LegoRR::Lego_IsShowDebugToolTips()));

	Menu_CheckButton(IDM_RENDERING_DRAW,	Gods98::Draw_IsRenderEnabled());
	Menu_CheckButton(IDM_RENDERING_IMAGES,	Gods98::Image_IsRenderEnabled());
	Menu_CheckButton(IDM_RENDERING_FONTS,	Gods98::Font_IsRenderEnabled());

	Menu_CheckButton(IDM_BLOCKFADE,     Gods98::Main_IsCLBlockFade());
	Menu_CheckButton(IDM_DDRAWCLEAR,	(LegoRR::Lego_IsInit() && LegoRR::Lego_IsDDrawClear()));
	Menu_CheckButton(IDM_WOBBLYWORLD,   OpenLRR_IsWobblyWorld());
	Menu_CheckButton(IDM_RESOURCETUMBLE,LegoRR::Lego_IsResourceTumbleOn());

	Menu_CheckButton(IDM_FPSMONITOR,	(LegoRR::Lego_IsInit() && LegoRR::Lego_IsFPSMonitorOn()));
	Menu_CheckButton(IDM_MEMORYMONITOR,	(LegoRR::Lego_IsInit() && LegoRR::Lego_IsMemoryMonitorOn()));

	Menu_CheckButton(IDM_DUMPMODE,      Gods98::Main_IsDumpMode());
	Menu_CheckButton(IDM_FREEZE,        Gods98::Main_IsPaused());
	// These are only useful while paused
	Menu_EnableButtonsArray(Menu_AdvanceFrameIDs, Gods98::Main_IsPaused());


	////// &Cheats //////
	Menu_CheckButton(IDM_NONERPS,		(LegoRR::Lego_IsInit() && LegoRR::Lego_IsNoNERPs()));
	Menu_CheckButton(IDM_UNLOCKCAMERA,	(LegoRR::Lego_IsInit() && LegoRR::Camera_IsFreeMovement(LegoRR::legoGlobs.cameraMain)));
	Menu_CheckButton(IDM_UNLOCKBUILD,	(LegoRR::Lego_IsInit() && !LegoRR::Dependencies_IsEnabled()));
	Menu_CheckButton(IDM_NOBUILDCOSTS,			(LegoRR::Lego_IsInit() && LegoRR::Cheat_IsNoBuildCosts()));
	Menu_CheckButton(IDM_NOCONSTRUCTIONBARRIERS,(LegoRR::Lego_IsInit() && LegoRR::Cheat_IsNoConstructionBarriers()));
	Menu_CheckButton(IDM_BUILDWITHOUTPATHS,	(LegoRR::Lego_IsInit() && !LegoRR::Lego_IsOnlyBuildOnPaths()));
	Menu_CheckButton(IDM_BUILDANYROUGHNESS,	(LegoRR::Lego_IsInit() && LegoRR::Cheat_IsBuildOnAnyRoughness()));
	Menu_CheckButton(IDM_FPNOCLIP,		(LegoRR::Lego_IsInit() && LegoRR::Lego_IsNoclipOn()));
	Menu_CheckButton(IDM_FPCONTROLS,	(LegoRR::Lego_IsInit() && LegoRR::Lego_IsTopdownFPControlsOn()));

	Menu_CheckButton(IDM_NOPOWERCONSUMPTION,	(LegoRR::Lego_IsInit() && LegoRR::Cheat_IsNoPowerConsumption()));
	Menu_CheckButton(IDM_NOOXYGENCONSUMPTION,	(LegoRR::Lego_IsInit() && LegoRR::Cheat_IsNoOxygenConsumption()));
	Menu_CheckButton(IDM_SUPERTOOLSTORE,		(LegoRR::Lego_IsInit() && LegoRR::Cheat_IsSuperToolStore()));

	Menu_EnableButton(IDM_SURVEYLEVEL,		(LegoRR::Lego_IsInLevel() && !LegoRR::Lego_IsLevelSurveyed()));

	Menu_CheckButton(IDM_NOROCKFALL,		(LegoRR::Lego_IsInit() && !LegoRR::Lego_IsAlwaysRockFall()));
	Menu_CheckButton(IDM_ALLOWRENAME,		(LegoRR::Lego_IsInLevel() && LegoRR::Lego_IsAllowRename()));
	Menu_CheckButton(IDM_GENERATESPIDERS,	(LegoRR::Lego_IsInLevel() && LegoRR::Lego_IsGenerateSpiders()));
	Menu_CheckButton(IDM_NOAUTOEAT,			(LegoRR::Lego_IsInLevel() && LegoRR::Lego_IsNoAutoEat()));
	Menu_CheckButton(IDM_NOFALLINS,			(LegoRR::Lego_IsInLevel() && !LegoRR::Lego_IsRandomFallinsOn()));
	Menu_CheckButton(IDM_NOMULTISELECT,		(LegoRR::Lego_IsInLevel() && LegoRR::Lego_IsNoMultiSelect()));
	Menu_CheckButton(IDM_SEETHROUGHWALLS,	(LegoRR::Lego_IsInLevel() && LegoRR::Lego_IsSeeThroughWalls()));

}

#pragma endregion


#pragma region OpenLRR_HandleCommand

static void updateOptionsMenuCycles(void)
{
	LegoRR::frontGlobs.cycleWallDetail		= (LegoRR::Lego_IsDetailOn()      ? 0 : 1); // cycle: [High,Low]
	LegoRR::frontGlobs.cycleMusicOn			= (LegoRR::Lego_IsMusicOn()       ? 0 : 1); // cycle: [On,Off]
	LegoRR::frontGlobs.cycleSFXOn			= (LegoRR::Lego_IsSoundOn()       ? 0 : 1); // cycle: [On,Off]
	LegoRR::frontGlobs.cycleHelpWindow		= (HelpWindow_IsEnabled()         ? 1 : 0); // cycle: [Off,On]
	LegoRR::frontGlobs.cycleAutoGameSpeed	= (LegoRR::Info_IsAutoGameSpeed() ? 0 : 1); // cycle: [On,Off]
	
	LegoRR::Front_UpdateSliderGameSpeed();

	// Not handled yet.
	//LegoRR::frontGlobs.sliderBrightness;
	//LegoRR::frontGlobs.sliderMusicVolume;
	//LegoRR::frontGlobs.sliderSFXVolume;
}

// Message handler for about box dialog.
INT_PTR CALLBACK OpenLRR_About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
            ::EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

// Perform system menu item command functions.
void __cdecl OpenLRR_HandleCommand(HWND hWnd, uint16 wmId, uint16 wmSrc)
{
    switch (wmId)
    {
        ////// &Help //////

    case IDM_ABOUT:
        //std::printf("IDM_ABOUT\n");
        ::DialogBoxParamA(OpenLRR_hInstDll(), MAKEINTRESOURCEA(IDD_ABOUTBOX), hWnd, OpenLRR_About, (LPARAM)nullptr);
        break;


        ////// &File //////

    case IDM_EXIT:
        //std::printf("IDM_EXIT\n");
		Gods98::Main_CloseApp(true);
        break;

    case IDM_TOGGLEMENU:
        //std::printf("IDM_TOGGLEMENU\n");
        Gods98::Main_SetMenu((Gods98::Main_GetMenu() ? nullptr : openlrrGlobs.menu), false);
        break;

    // Not supported yet (this module needs to be cleaned and readied first)
    /*case IDM_TOGGLECONSOLE:
        //std::printf("IDM_TOGGLEMENU\n");
        OpenLRR::Console_Hide(!OpenLRR::Console_IsHidden());
        break;*/


        ////// &Options //////

	case IDM_MUSICON:
		//std::printf("IDM_MUSICON\n");
		LegoRR::Lego_SetMusicOn(!LegoRR::Lego_IsMusicOn());
		updateOptionsMenuCycles();
		break;

	case IDM_SOUNDON:
		//std::printf("IDM_SOUNDON\n");
		LegoRR::Lego_SetSoundOn(!LegoRR::Lego_IsSoundOn());
		updateOptionsMenuCycles();
		break;

	case IDM_HELPWINDOW:
		//std::printf("IDM_HELPWINDOW\n");
		/// TODO: Come back here and figure out why this is the opposite of toggling.
		HelpWindow_SetEnabled(false, HelpWindow_IsEnabled());
		//HelpWindow_SetEnabled(false, !HelpWindow_IsEnabled());
		updateOptionsMenuCycles();
		break;

	case IDM_AUTOGAMESPEED:
		//std::printf("IDM_AUTOGAMESPEED\n");
		LegoRR::Info_SetAutoGameSpeed(!LegoRR::Info_IsAutoGameSpeed());
		updateOptionsMenuCycles();
		break;

    case IDM_DUALMOUSE:
        //std::printf("IDM_DUALMOUSE\n");
		Gods98::Main_SetDualMouse(!Gods98::Main_IsDualMouse());
        break;

	case IDM_LOSEFOCUSANDPAUSE:
		//std::printf("IDM_LOSEFOCUSANDPAUSE\n");
		LegoRR::Lego_SetLoseFocusAndPause(!LegoRR::Lego_IsLoseFocusAndPause());
		break;

	case IDM_SELECTPLACEARROW_NEVER:
	case IDM_SELECTPLACEARROW_SOLIDONLY:
	case IDM_SELECTPLACEARROW_ALWAYS:
		LegoRR::SelectPlace_SetArrowVisibility(LegoRR::legoGlobs.selectPlace, static_cast<LegoRR::SelectPlace_ArrowVisibility>(wmId - IDM_SELECTPLACEARROW_NEVER));
		break;

	case IDM_SHOWOBJINFO:
		//std::printf("IDM_SHOWOBJINFO\n");
		LegoRR::Bubble_ToggleObjectUIsAlwaysVisible();
		break;

	case IDM_RENDERPANELS:
		//std::printf("IDM_RENDERPANELS\n");
		LegoRR::Lego_SetRenderPanels(!LegoRR::Lego_IsRenderPanels());
		break;

	case IDM_SHOWTOOLTIPS:
		LegoRR::Lego_SetShowToolTips(!LegoRR::Lego_IsShowToolTips());
		break;

	case IDM_TOOLTIPSOUND:
		//std::printf("IDM_TOOLTIPSOUND\n");
		LegoRR::Lego_SetDisableToolTipSound(!LegoRR::Lego_IsDisableToolTipSound());
		break;

	case IDM_RELOAD_KEYBINDS:
		//std::printf("IDM_RELOAD_KEYBINDS\n");
		Shortcuts::shortcutManager.Load();
		break;

	case IDM_LOGGING_DEBUG:
		//std::printf("IDM_LOGGING_DEBUG\n");
		Gods98::Error_SetDebugVisible(!Gods98::Error_IsDebugVisible());
		break;
	case IDM_LOGGING_TRACE:
		//std::printf("IDM_LOGGING_TRACE\n");
		Gods98::Error_SetTraceVisible(!Gods98::Error_IsTraceVisible());
		break;
	case IDM_LOGGING_INFO:
		//std::printf("IDM_LOGGING_INFO\n");
		Gods98::Error_SetInfoVisible(!Gods98::Error_IsInfoVisible());
		break;
	case IDM_LOGGING_WARNING:
		//std::printf("IDM_LOGGING_WARNING\n");
		Gods98::Error_SetWarnVisible(!Gods98::Error_IsWarnVisible());
		break;
	case IDM_LOGGING_FATAL:
		//std::printf("IDM_LOGGING_FATAL\n");
		Gods98::Error_SetFatalVisible(!Gods98::Error_IsFatalVisible());
		break;

	case IDM_WINDOWSCALE_X1:
	case IDM_WINDOWSCALE_X2:
	case IDM_WINDOWSCALE_X3:
	case IDM_WINDOWSCALE_X4:
		if (!Gods98::Main_IsRenderScaling()) {
			Gods98::Main_SetScale((wmId - IDM_WINDOWSCALE_X1) + 1); // scale is "1-indexed"
		}
		break;

	case IDM_RADARMAPSCALE_X1:
	case IDM_RADARMAPSCALE_X2:
	case IDM_RADARMAPSCALE_X3:
	case IDM_RADARMAPSCALE_X4:
		Gods98::Main_SetRadarMapScale((wmId - IDM_RADARMAPSCALE_X1) + 1); // scale is "1-indexed"
		break;

    case IDM_CURSOR_NEVER:
    case IDM_CURSOR_TITLEBAR:
    case IDM_CURSOR_ALWAYS:
        /*switch (wmId) {
        case IDM_CURSOR_NEVER:    std::printf("IDM_CURSOR_NEVER\n"); break;
        case IDM_CURSOR_TITLEBAR: std::printf("IDM_CURSOR_TITLEBAR\n"); break;
        case IDM_CURSOR_ALWAYS:   std::printf("IDM_CURSOR_ALWAYS\n"); break;
        }*/
        Gods98::Main_SetCursorVisibility(static_cast<Gods98::CursorVisibility>(wmId - IDM_CURSOR_NEVER));
        break;

	case IDM_CURSORCLIPPING_OFF:
	case IDM_CURSORCLIPPING_MENU:
	case IDM_CURSORCLIPPING_GAMEAREA:
		Gods98::Main_SetCursorClipping(static_cast<Gods98::CursorClipping>(wmId - IDM_CURSORCLIPPING_OFF));
		break;


    case IDM_ICON_NONE:
    case IDM_ICON_NATIVE:
    case IDM_ICON_OPENLRR:
    case IDM_ICON_GOLD:
    case IDM_ICON_TEAL:
    case IDM_ICON_TEALRR:
        /*switch (wmId) {
        case IDM_ICON_NONE:    std::printf("IDM_ICON_NONE\n"); break;
        case IDM_ICON_NATIVE:  std::printf("IDM_ICON_NATIVE\n"); break;
        case IDM_ICON_OPENLRR: std::printf("IDM_ICON_OPENLRR\n"); break;
        case IDM_ICON_GOLD:    std::printf("IDM_ICON_GOLD\n"); break;
        case IDM_ICON_TEAL:    std::printf("IDM_ICON_TEAL\n"); break;
        case IDM_ICON_TEALRR:  std::printf("IDM_ICON_TEALRR\n"); break;
        }*/
        Gods98::Main_SetIcon(openlrrGlobs.iconList[static_cast<size_t>(wmId - IDM_ICON_NONE)], false);
        break;

	case IDM_TRANSPARENTMULTISELECT:
		LegoRR::Lego_SetTransparentMultiSelectBox(!LegoRR::Lego_IsTransparentMultiSelectBox());
		break;

	case IDM_LIGHTEFFECTS:
		//std::printf("IDM_LIGHTEFFECTS\n");
		LegoRR::gamectrlGlobs.dbgF9DisableLightEffects = !LegoRR::gamectrlGlobs.dbgF9DisableLightEffects;
		LegoRR::LightEffects_SetDisabled(LegoRR::gamectrlGlobs.dbgF9DisableLightEffects);
		break;

	case IDM_DETAILON:
		//std::printf("IDM_DETAILON\n");
		LegoRR::Lego_SetDetailOn(!LegoRR::Lego_IsDetailOn());
		break;

	case IDM_DYNAMICPM:
		//std::printf("IDM_DYNAMICPM\n");
		LegoRR::Lego_SetDynamicPM(!LegoRR::Lego_IsDynamicPM());
		break;

	case IDM_TOPDOWNFOG:
		//std::printf("IDM_TOPDOWNFOG\n");
		LegoRR::Lego_SetTopdownFogOn(!LegoRR::Lego_IsTopdownFogOn());
		break;

	case IDM_TOPDOWNLOD_LOW:
	case IDM_TOPDOWNLOD_MEDIUM:
	case IDM_TOPDOWNLOD_HIGH:
		LegoRR::Lego_SetTopdownLOD(static_cast<LegoRR::LOD_PolyLevel>(wmId - IDM_TOPDOWNLOD_LOW));
		break;

    case IDM_QUALITY_WIREFRAME:
    case IDM_QUALITY_UNLITFLAT:
    case IDM_QUALITY_FLAT:
    case IDM_QUALITY_GOURAUD:
    case IDM_QUALITY_PHONG:
        /*switch (wmId) {
        case IDM_QUALITY_WIREFRAME: std::printf("IDM_QUALITY_WIREFRAME\n"); break;
        case IDM_QUALITY_UNLITFLAT: std::printf("IDM_QUALITY_UNLITFLAT\n"); break;
        case IDM_QUALITY_FLAT:      std::printf("IDM_QUALITY_FLAT\n"); break;
        case IDM_QUALITY_GOURAUD:   std::printf("IDM_QUALITY_GOURAUD\n"); break;
        case IDM_QUALITY_PHONG:     std::printf("IDM_QUALITY_PHONG\n"); break;
        }*/
		Gods98::Graphics_SetRenderQuality(static_cast<Gods98::Graphics_Quality>(wmId - IDM_QUALITY_WIREFRAME));
        break;

    /*case IDM_GRAPHICS_BLEND:
        //std::printf("IDM_GRAPHICS_BLEND\n");
		Gods98::Graphics_SetBlendTransparency(!Gods98::Graphics_IsBlendTransparency());
        break;*/
    case IDM_GRAPHICS_DITHER:
        //std::printf("IDM_GRAPHICS_DITHER\n");
		Gods98::Graphics_SetDither(!Gods98::Graphics_IsDither());
        break;
    /*case IDM_GRAPHICS_FILTER:
        //std::printf("IDM_GRAPHICS_FILTER\n");
		Gods98::Graphics_SetLinearFilter(!Gods98::Graphics_IsLinearFilter());
        break;*/
    case IDM_GRAPHICS_LINEARMIPMAP:
        //std::printf("IDM_GRAPHICS_LINEARMIPMAP\n");
		Gods98::Graphics_SetMIPMapLinear(!Gods98::Graphics_IsMIPMapLinear());
        break;
    case IDM_GRAPHICS_MIPMAP:
        //std::printf("IDM_GRAPHICS_MIPMAP\n");
		Gods98::Graphics_SetMIPMap(!Gods98::Graphics_IsMIPMap());
        break;
    /*case IDM_GRAPHICS_SORT:
        //std::printf("IDM_GRAPHICS_SORT\n");
		Gods98::Graphics_SetSortTransparency(!Gods98::Graphics_IsSortTransparency());
        break;*/
	case IDM_GRAPHICS_ALPHAMODULATION:
        //std::printf("IDM_GRAPHICS_ALPHAMODULATION\n");
		Gods98::Graphics_SetAlphaModulation(!Gods98::Graphics_IsAlphaModulation());
        break;


        ////// &Debug //////

    case IDM_PROGRAMMER:
        //std::printf("IDM_PROGRAMMER\n");
		if (Gods98::Main_ProgrammerMode() == 0) {
			// Because this field is only used to toggle back on, we want to ensure its value is a non-zero mode.
            if (openlrrGlobs.orig_programmerLevel == 0)
                openlrrGlobs.orig_programmerLevel = 1; // default

			Gods98::Main_SetProgrammerMode(openlrrGlobs.orig_programmerLevel);
        }
        else {
			openlrrGlobs.orig_programmerLevel = Gods98::Main_ProgrammerMode();
			Gods98::Main_SetProgrammerMode(0); // disabled
        }
        break;

    case IDM_DEBUGMODE:
        //std::printf("IDM_DEBUGMODE\n");
		Gods98::Main_SetDebugMode(!Gods98::Main_IsDebugMode());
        break;

    case IDM_DEBUGCOMPLETE:
        //std::printf("IDM_DEBUGCOMPLETE\n");
		Gods98::Main_SetDebugComplete(!Gods98::Main_IsDebugComplete());
        break;

    case IDM_LEVELSOPEN:
        //std::printf("IDM_LEVELSOPEN\n");
		Gods98::Main_SetLevelsOpen(!Gods98::Main_IsLevelsOpen());
        break;

    case IDM_TESTERCALL:
        //std::printf("IDM_TESTERCALL\n");
		Gods98::Main_SetTesterCall(!Gods98::Main_IsTesterCall());
        break;

	case IDM_ALLOWDEBUGKEYS:
		//std::printf("IDM_ALLOWDEBUGKEYS\n");
		LegoRR::Lego_SetAllowDebugKeys(!LegoRR::Lego_IsAllowDebugKeys());
		break;

	case IDM_ALLOWEDITMODE:
		//std::printf("IDM_ALLOWEDITMODE\n");
		LegoRR::Lego_SetAllowEditMode(!LegoRR::Lego_IsAllowEditMode());
		break;

	case IDM_SHOWDEBUGTOOLTIPS:
		//std::printf("IDM_SHOWDEBUGTOOLTIPS\n");
		LegoRR::Lego_SetShowDebugToolTips(!LegoRR::Lego_IsShowDebugToolTips());
		break;

	case IDM_ROUTING_ENABLED:
		LegoRR::Debug_RouteVisual_SetEnabled(!LegoRR::Debug_RouteVisual_IsEnabled());
		break;

	case IDM_ROUTING_COMPLETEDPATHS:
		LegoRR::Debug_RouteVisual_SetCompletedPathsEnabled(!LegoRR::Debug_RouteVisual_IsCompletedPathsEnabled());
		break;

	case IDM_ROUTING_CURVEPATHS:
		LegoRR::Debug_RouteVisual_SetCurvePathsEnabled(!LegoRR::Debug_RouteVisual_IsCurvePathsEnabled());
		break;

	case IDM_ROUTING_AUTO_NONE:
	case IDM_ROUTING_AUTO_TRACKED:
	case IDM_ROUTING_AUTO_ALLFRIENDLY:
	case IDM_ROUTING_AUTO_ALL:
		LegoRR::Debug_RouteVisual_SetAutoMode(static_cast<LegoRR::Debug_RouteVisualAuto>(wmId - IDM_ROUTING_AUTO_NONE));
		LegoRR::Debug_RouteVisual_SetEnabled(true); // Automatically enable for QoL.
		break;

	case IDM_ROUTING_ADDSELECTED:
		LegoRR::Debug_RouteVisual_AddSelected();
		LegoRR::Debug_RouteVisual_SetEnabled(true); // Automatically enable for QoL.
		break;

	case IDM_ROUTING_ADDALLFRIENDLY:
		LegoRR::Debug_RouteVisual_AddAll(true);
		LegoRR::Debug_RouteVisual_SetEnabled(true); // Automatically enable for QoL.
		break;

	case IDM_ROUTING_ADDALL:
		LegoRR::Debug_RouteVisual_AddAll(false);
		LegoRR::Debug_RouteVisual_SetEnabled(true); // Automatically enable for QoL.
		break;

	case IDM_ROUTING_REMOVESELECTED:
		LegoRR::Debug_RouteVisual_RemoveSelected();
		break;

	case IDM_ROUTING_REMOVEALL:
		LegoRR::Debug_RouteVisual_RemoveAll();
		break;

	case IDM_RENDERING_DRAW:
		Gods98::Draw_SetRenderEnabled(!Gods98::Draw_IsRenderEnabled());
		break;

	case IDM_RENDERING_IMAGES:
		Gods98::Image_SetRenderEnabled(!Gods98::Image_IsRenderEnabled());
		break;

	case IDM_RENDERING_FONTS:
		Gods98::Font_SetRenderEnabled(!Gods98::Font_IsRenderEnabled());
		break;

    case IDM_BLOCKFADE:
        //std::printf("IDM_BLOCKFADE\n");
		Gods98::Main_SetCLBlockFade(!Gods98::Main_IsCLBlockFade());
        break;

	case IDM_DDRAWCLEAR:
		//std::printf("IDM_DDRAWCLEAR\n");
		LegoRR::Lego_SetDDrawClear(!LegoRR::Lego_IsDDrawClear());
		break;

    case IDM_WOBBLYWORLD:
        //std::printf("IDM_WOBBLYWORLD\n");
		OpenLRR_SetWobblyWorld(!OpenLRR_IsWobblyWorld());
        break;

    case IDM_RESOURCETUMBLE:
        //std::printf("IDM_RESOURCETUMBLE\n");
		LegoRR::Lego_SetResourceTumbleOn(!LegoRR::Lego_IsResourceTumbleOn());
        break;

	case IDM_FPSMONITOR:
		//std::printf("IDM_FPSMONITOR\n");
		LegoRR::Lego_SetFPSMonitorOn(!LegoRR::Lego_IsFPSMonitorOn());
		break;

	case IDM_MEMORYMONITOR:
		//std::printf("IDM_MEMORYMONITOR\n");
		LegoRR::Lego_SetMemoryMonitorOn(!LegoRR::Lego_IsMemoryMonitorOn());
		break;

    case IDM_DUMPMODE:
        //std::printf("IDM_DUMPMODE\n");
		Gods98::Main_SetDumpMode(!Gods98::Main_IsDumpMode());
        // these modes are *basically* mutually exclusive
		if (Gods98::Main_IsDumpMode())
            Gods98::Main_PauseApp(false);
        break;

    case IDM_FREEZE:
        //std::printf("IDM_FREEZE\n");
        Gods98::Main_PauseApp(!Gods98::Main_IsPaused());
        // these modes are *basically* mutually exclusive
		if (Gods98::Main_IsPaused())
			Gods98::Main_SetDumpMode(false);
        break;

    case IDM_ADVANCE_1FRAME:
        //std::printf("IDM_ADVANCE_1FRAME\n");
        Gods98::Main_AdvanceFrame();
        break;

    case IDM_ADVANCE_1SECOND:
        //std::printf("IDM_ADVANCE_1SECOND\n");
        Gods98::Main_SetAdvanceFrames(static_cast<sint32>(STANDARD_FRAMERATE));
        break;


		////// &Cheats //////

	case IDM_NONERPS:
		//std::printf("IDM_NONERPS\n");
		LegoRR::Lego_SetNoNERPs(!LegoRR::Lego_IsNoNERPs());
		break;

	case IDM_UNLOCKCAMERA:
		//std::printf("IDM_UNLOCKCAMERA\n");
		LegoRR::Camera_EnableFreeMovement(LegoRR::legoGlobs.cameraMain, !LegoRR::Camera_IsFreeMovement(LegoRR::legoGlobs.cameraMain));
		break;

	case IDM_UNLOCKBUILD:
		//std::printf("IDM_UNLOCKBUILD\n");
		LegoRR::Dependencies_SetEnabled(!LegoRR::Dependencies_IsEnabled());
		break;

	case IDM_NOBUILDCOSTS:
		//std::printf("IDM_NOBUILDCOSTS\n");
		LegoRR::Cheat_SetNoBuildCosts(!LegoRR::Cheat_IsNoBuildCosts());
		break;

	case IDM_NOCONSTRUCTIONBARRIERS:
		//std::printf("IDM_NOCONSTRUCTIONBARRIERS\n");
		LegoRR::Cheat_SetNoConstructionBarriers(!LegoRR::Cheat_IsNoConstructionBarriers());
		break;

	case IDM_BUILDWITHOUTPATHS:
		//std::printf("IDM_BUILDWITHOUTPATHS\n");
		LegoRR::Lego_SetOnlyBuildOnPaths(!LegoRR::Lego_IsOnlyBuildOnPaths());
		break;

	case IDM_BUILDANYROUGHNESS:
		//std::printf("IDM_BUILDANYROUGHNESS\n");
		LegoRR::Cheat_SetBuildOnAnyRoughness(!LegoRR::Cheat_IsBuildOnAnyRoughness());
		break;

	/*case IDM_BUILDANYROUGHNESS:
		//std::printf("IDM_BUILDANYROUGHNESS\n");
		openlrrGlobs.noBuildRoughness = !openlrrGlobs.noBuildRoughness;
		if (openlrrGlobs.noBuildRoughness) {
			openlrrGlobs.orig_buildingMaxVariation = LegoRR::legoGlobs.level->BuildingMaxVariation;
			openlrrGlobs.orig_buildingTolerance = LegoRR::legoGlobs.level->BuildingTolerance;
			LegoRR::legoGlobs.level->BuildingMaxVariation = 1000.0; // arbitrarily high, no clue what value is good
			LegoRR::legoGlobs.level->BuildingTolerance = 1000.0;
		}
		else { // restore values
			LegoRR::legoGlobs.level->BuildingMaxVariation = openlrrGlobs.orig_buildingMaxVariation;
			LegoRR::legoGlobs.level->BuildingTolerance = openlrrGlobs.orig_buildingTolerance;
		}
		break;*/

	case IDM_FPNOCLIP:
		//std::printf("IDM_FPNOCLIP\n");
		LegoRR::Lego_SetNoclipOn(!LegoRR::Lego_IsNoclipOn());
		break;

	case IDM_FPCONTROLS:
		//std::printf("IDM_FPCONTROLS\n");
		LegoRR::Lego_SetTopdownFPControlsOn(!LegoRR::Lego_IsTopdownFPControlsOn());
		break;


	case IDM_NOPOWERCONSUMPTION:
		//std::printf("IDM_NOPOWERCONSUMPTION\n");
		LegoRR::Cheat_SetNoPowerConsumption(!LegoRR::Cheat_IsNoPowerConsumption());
		break;

	case IDM_NOOXYGENCONSUMPTION:
		//std::printf("IDM_NOOXYGENCONSUMPTION\n");
		LegoRR::Cheat_SetNoOxygenConsumption(!LegoRR::Cheat_IsNoOxygenConsumption());
		break;

	case IDM_SUPERTOOLSTORE:
		LegoRR::Cheat_SetSuperToolStore(!LegoRR::Cheat_IsSuperToolStore());
		break;


	case IDM_SURVEYLEVEL:
		//std::printf("IDM_SURVEYLEVEL\n");
		LegoRR::Cheat_SurveyLevel();
		break;


	case IDM_NOROCKFALL:
		//std::printf("IDM_NOROCKFALL\n");
		LegoRR::Lego_SetAlwaysRockFall(!LegoRR::Lego_IsAlwaysRockFall());
		break;


	case IDM_ALLOWRENAME:
		//std::printf("IDM_ALLOWRENAME\n");
		LegoRR::Lego_SetAllowRename(!LegoRR::Lego_IsAllowRename());
		break;

	case IDM_GENERATESPIDERS:
		//std::printf("IDM_GENERATESPIDERS\n");
		LegoRR::Lego_SetGenerateSpiders(!LegoRR::Lego_IsGenerateSpiders());
		break;
	case IDM_NOAUTOEAT:
		//std::printf("IDM_NOAUTOEAT\n");
		LegoRR::Lego_SetNoAutoEat(!LegoRR::Lego_IsNoAutoEat());
		break;
	case IDM_NOFALLINS:
		//std::printf("IDM_NOFALLINS\n");
		LegoRR::Lego_SetRandomFallinsOn(!LegoRR::Lego_IsRandomFallinsOn());
		break;
	case IDM_NOMULTISELECT:
		//std::printf("IDM_NOMULTISELECT\n");
		LegoRR::Lego_SetNoMultiSelect(!LegoRR::Lego_IsNoMultiSelect());
		break;
	case IDM_SEETHROUGHWALLS:
		//std::printf("IDM_SEETHROUGHWALLS\n");
		LegoRR::Lego_SetSeeThroughWalls(!LegoRR::Lego_IsSeeThroughWalls());
		break;
    }
}

#pragma endregion


#pragma region OpenLRR_WindowCallback

// Handle system menu commands.
void __cdecl OpenLRR_WindowCallback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_ENTERIDLE:
		::ClipCursor(nullptr); // Unlock cursor when pressing alt.
		break;

    case WM_INITMENUPOPUP:
        OpenLRR_UpdateMenuItems();
        break;

    case WM_COMMAND: {
            uint16 wmId  = LOWORD(wParam);
            uint16 wmSrc = HIWORD(wParam);
            OpenLRR_HandleCommand(hWnd, wmId, wmSrc);
        }
        break;
    }
}

#pragma endregion


#pragma region OpenLRR Wobbly World effect

// Sets if the wobbly world, fun effects are enabled. This needs some hardcoded handling to turn off mid-game.
void OpenLRR_SetWobblyWorld(bool on)
{
	openlrrGlobs.wobblyWorld = on;

	if (!openlrrGlobs.wobblyWorld) {
		auto level = LegoRR::Lego_GetLevel();
		if (level != nullptr) {
			// Turn off wobbly world by relying on hardcoded behaviour of what blocks normally wobble.
			auto map = LegoRR::Lego_GetMap();
			for (uint32 by = 0; by < map->blockHeight; by++) {
				for (uint32 bx = 0; bx < map->blockWidth; bx++) {
					// Lazy method by turning off wobbles, then having `Level_BlockUpdateSurface` them turn back on as needed.
					/// WARNING: There could be unintended consequences for calling `Level_BlockUpdateSurface`, when not expected.
					///          But this mode isn't exactly important enough to worry about that yet...
					Map3D_SetBlockUVWobbles(map, bx, by, false);
					Level_BlockUpdateSurface(level, bx, by, 0); // 0 = reserved field
				}
			}
		}

	}
	else {
		OpenLRR_UpdateWobblyWorld();
	}
}

// Called during `OpenLRR_MainLoop_Wrapper` to forcefully make all block textures wobble.
void OpenLRR_UpdateWobblyWorld(void)
{
	auto level = LegoRR::Lego_GetLevel();
	if (level != nullptr) {
		// All the wobbles! All the time!
		auto map = LegoRR::Lego_GetMap();
		for (uint32 by = 0; by < map->blockHeight; by++) {
			for (uint32 bx = 0; bx < map->blockWidth; bx++) {
				Map3D_SetBlockUVWobbles(map, bx, by, true);
			}
		}
	}
}

#pragma endregion


#pragma region Main State wrappers

bool32 __cdecl OpenLRR_Initialise_Wrapper(void)
{
	// pre-Initialise code here...

	bool32 result = openlrrGlobs.legoState.Initialise();

	// NOTE: This call does not finish until the MainMenu has been left and a game started!!!
	// post-Initialise code...

	return result;
}

bool32 __cdecl OpenLRR_MainLoop_Wrapper(real32 elapsed)
{
	// pre-MainLoop code here...

	bool32 result = openlrrGlobs.legoState.MainLoop(elapsed);

	// NOTE: Similar to Initialise, the main loop does not run when in the FrontEnd!!!
	// post-MainLoop code here...


	if (OpenLRR_IsWobblyWorld()) {
		OpenLRR_UpdateWobblyWorld();
	}

	return result;
}

void __cdecl OpenLRR_Shutdown_Wrapper(void)
{
	// pre-Shutdown code here...

	openlrrGlobs.legoState.Shutdown();

	// post-Shutdown code here...
}

#pragma endregion

/**********************************************************************************
 ******** OpenLRR Game Entry point
 **********************************************************************************/

#pragma region Entry point

// This is the GAME entry point as called by WinMain,
//  this should hook the Main_State loop functions and only perform basic initial setup.
// (this can return bool32, but does not)
// We're moving this to OpenLRR module to allow easy hooking and setup for the game loop
// <LegoRR.exe @0041f950>
void __cdecl Gods98::Gods_Go(const char* programName)
{
	log_firstcall();

	// The program name MUST start with Lego* if we want all cfg,ae,ptl,ol assets to function.
	// NOTE: This does not change WAD filename access, that has to be handled in Main_WinMain.
	if (Gods98::mainOptions.gameName) {
		std::strcpy(openlrrGlobs.legoProgramName, Gods98::mainOptions.gameName->c_str());
	}
	else if (::_stricmp(programName, "OpenLRR") == 0 ||
		::_stricmp(programName, "OpenLRR-d") == 0)
	{
		// Forcefully default to the standard "LegoRR" name. This is a terrible solution...
		std::strcpy(openlrrGlobs.legoProgramName, "LegoRR");
	}
	else if (::_strnicmp(programName, "Lego", 4) != 0) {
		// Maybe add an option if the user REALLY wants to redefine everything under something other
		// than Lego*, but for now, this is just 
		std::sprintf(openlrrGlobs.legoProgramName, "%s%s", "Lego", programName);
	}
	else {
		std::strcpy(openlrrGlobs.legoProgramName, programName);
	}
	LegoRR::Lego_Gods_Go_Setup(openlrrGlobs.legoProgramName, &openlrrGlobs.legoState);


	/// FLUFF OPENLRR: Wrap the program name in parenthesis and start with "OpenLRR"
	char buff[1024];
	if (::_stricmp(programName, "OpenLRR") == 0 ||
		::_stricmp(programName, "OpenLRR-d") == 0)
	{
		#ifdef NDEBUG
		std::sprintf(buff, "%s", "OpenLRR"); // Release build
		#else
		std::sprintf(buff, "%s (%s)", "OpenLRR", "Debug"); // Debug build
		#endif
		programName = buff;
	}
	else {
		std::sprintf(buff, "%s (%s)", "OpenLRR", programName); // List the startup program name
		programName = buff;
	}
	Main_SetTitle(programName);


	// These will call the appropriate openlrrGlobs.legoState functions.
	Gods98::Main_State mainState = {
		OpenLRR_Initialise_Wrapper,
		OpenLRR_MainLoop_Wrapper,
		OpenLRR_Shutdown_Wrapper,
	};

	Main_SetState(&mainState);
}

#pragma endregion

sint32 __stdcall LaunchOpenLRR(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, sint32 nCmdShow)
{
	if (!Gods98::Main_Initialise(OpenLRR_hInstDll(), hInstance, hPrevInstance, lpCmdLine, nCmdShow)) {
		return 0;
	}

	openlrrGlobs.hInstMain = hInstance;
	if (Gods98::mainOptions.log.value_or(true))
		openlrrGlobs.conout = MakeConsole();


	//std::printf("lpCmdline: %s\n", lpCmdLine);
	//for (size_t i = 0; i < Gods98::mainOptions.arguments.size(); i++) {
	//	std::printf("arguments[%i]: %s\n", (sint32)i, Gods98::mainOptions.arguments[i].c_str());
	//}

    const char* methodName;
    switch (openlrrGlobs.method) {
    case InjectMethod::Unknown:     methodName = "Unknown";     break;
    case InjectMethod::NotAttached: methodName = "NotAttached"; break;
    case InjectMethod::Injector:    methodName = "Injector";    break;
    case InjectMethod::DllImport:   methodName = "DllImport";   break;
    case InjectMethod::DllStart:    methodName = "DllStart";    break;
    default:                        methodName = "<???>";       break;
    }
    std::printf("InjectMethod::%s\n", methodName);
    //::system("pause");

    // Hook all functions that aren't being called through OpenLRR code.
    interop_hook_all();


    // System menu and window customization setup:

    openlrrGlobs.iconList[(uint32)OpenLRRIcon::None]    = nullptr;
    openlrrGlobs.iconList[(uint32)OpenLRRIcon::Native]  = ::LoadIconA(OpenLRR_hInstMain(), MAKEINTRESOURCEA(IDI_LEGORR));
    openlrrGlobs.iconList[(uint32)OpenLRRIcon::OpenLRR] = ::LoadIconA(OpenLRR_hInstDll(), MAKEINTRESOURCEA(IDI_OPENLRR));
    // The following icons may not be included in OpenLRR:
    openlrrGlobs.iconList[(uint32)OpenLRRIcon::Gold]    = ::LoadIconA(OpenLRR_hInstDll(), MAKEINTRESOURCEA(IDI_LEGORR));
    openlrrGlobs.iconList[(uint32)OpenLRRIcon::Teal]    = ::LoadIconA(OpenLRR_hInstDll(), MAKEINTRESOURCEA(IDI_CLGEN));
    openlrrGlobs.iconList[(uint32)OpenLRRIcon::TealRR]  = ::LoadIconA(OpenLRR_hInstDll(), MAKEINTRESOURCEA(IDI_CDROM));

    openlrrGlobs.menu   = ::LoadMenuA(OpenLRR_hInstDll(), MAKEINTRESOURCEA(IDC_DEBUGSYSMENU));
    openlrrGlobs.accels = ::LoadAcceleratorsA(OpenLRR_hInstDll(), MAKEINTRESOURCEA(IDC_DEBUGSYSMENU));

    if (openlrrGlobs.iconList[(uint32)OpenLRRIcon::OpenLRR])
        Gods98::Main_SetIcon(openlrrGlobs.iconList[(uint32)OpenLRRIcon::OpenLRR], false);
    else if (openlrrGlobs.iconList[(uint32)OpenLRRIcon::Native])
        Gods98::Main_SetIcon(openlrrGlobs.iconList[(uint32)OpenLRRIcon::Native], false);

    Gods98::Main_SetAccel(openlrrGlobs.accels, false);
	if (Gods98::mainOptions.menu.value_or(true))
		Gods98::Main_SetMenu(openlrrGlobs.menu, false);
    Gods98::Main_SetWindowCallback(OpenLRR_WindowCallback);

    Gods98::Main_SetCursorVisibility(Gods98::CursorVisibility::TitleBar);

    
    return Gods98::Main_WinMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}

sint32 __stdcall StartOpenLRRInjected(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, sint32 nCmdShow)
{
	return LaunchOpenLRR(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}

extern "C" __declspec(dllexport) sint32 __cdecl StartOpenLRR(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, sint32 nCmdShow)
{
	return LaunchOpenLRR(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}

/*extern "C" __declspec(dllexport) void __cdecl Dummy(void)
{
}*/

#pragma endregion
