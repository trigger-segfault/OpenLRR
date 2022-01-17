// OpenLRR.cpp : 
//

#include "platform/windows.h"
#include "../../../resources/resource.h"

#include "OpenLRR.h"
#include "hook.h"
#include "interop.h"
#include "dllmain.h"

#include "engine/Graphics.h"
#include "engine/Main.h"
#include "game/Game.h"


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
    static const constexpr uint8 eipPatch[2]        = { 0xeb,0xfe }; // infinite jmp to itself
    static const constexpr uint32 PROCESS_EIP       = 0x0048f2c0; // LegoRR.exe!entry
    static const constexpr uint32 PROCESS_WINMAIN   = 0x00477a60; // LegoRR.exe!WinMain

	// Determine if openlrr "Start" executable was used to load this dll.
	static const constexpr uint8 winmainStart[6]    = { 0x55,0x8b,0xec,0xff,0x74,0x24 };

	if (hook_cmp(PROCESS_EIP, eipPatch, sizeof(eipPatch)) == 0) {
		openlrrGlobs.method = InjectMethod::Injector;
	}
	else if (hook_cmp(PROCESS_WINMAIN, winmainStart, sizeof(winmainStart)) == 0) {
		openlrrGlobs.method = InjectMethod::DllStart;
		// In this scenario we do not want to touch the WinMain assembly, return here.
		return true;
	}


    // Redirect `LegoRR.exe!WinMain` to immediately jump to `openlrr.dll!StartOpenLRR`.
	bool result = hook_write_jmpret(PROCESS_WINMAIN, StartOpenLRRInjected);

    return result;
}


#pragma region OpenLRR_UpdateMenuItems

#define Menu_EnableButton(menuID, state) \
    ::EnableMenuItem(Gods98::Main_GetMenu(), (menuID), MF_BYCOMMAND|((state)?MF_ENABLED:(MF_GRAYED|MF_DISABLED)))

#define Menu_CheckButton(menuID, state) \
    ::CheckMenuItem(Gods98::Main_GetMenu(), (menuID), MF_BYCOMMAND|((state)?MF_CHECKED:MF_UNCHECKED))

#define Menu_EnableButtons(menuIDs, count, state) \
    for (sint32 ii = 0; ii < (sint32)(count); ii++) Menu_EnableButton((menuIDs)[ii], state)

#define Menu_CheckButtons(menuIDs, count, state) \
    for (sint32 ii = 0; ii < (sint32)(count); ii++) Menu_CheckButton((menuIDs)[ii], state)

#define Menu_CheckRadioButtons(menuIDs, count, index) \
    for (sint32 ii = 0; ii < (sint32)(count); ii++) Menu_CheckButton((menuIDs)[ii], (ii==(index)))

#define Menu_EnableButtonsArray(menuIDs, state) Menu_EnableButtons(menuIDs, _countof(menuIDs), state)
#define Menu_CheckButtonsArray(menuIDs, state)  Menu_CheckButtons(menuIDs, _countof(menuIDs), state)
#define Menu_CheckRadioButtonsArray(menuIDs, index) Menu_CheckRadioButtons(menuIDs, _countof(menuIDs), index)


static const uint32 Menu_IconIDs[] = { IDM_ICON_NONE, IDM_ICON_NATIVE, IDM_ICON_OPENLRR, IDM_ICON_TEAL, IDM_ICON_GOLD, IDM_ICON_CDROM };
static const uint32 Menu_CursorIDs[] = { IDM_CURSOR_NEVER, IDM_CURSOR_TITLEBAR, IDM_CURSOR_ALWAYS };
static const uint32 Menu_QualityIDs[] = { IDM_QUALITY_WIREFRAME, IDM_QUALITY_UNLITFLAT, IDM_QUALITY_FLAT, IDM_QUALITY_GOURAUD, IDM_QUALITY_PHONG };
static const uint32 Menu_AdvanceFrameIDs[] = { IDM_ADVANCE_1FRAME, IDM_ADVANCE_1SECOND };
static const uint32 Menu_InitCommandLineIDs[] = { IDM_DUALMOUSE, IDM_PROGRAMMER, IDM_DEBUGMODE, IDM_DEBUGCOMPLETE, IDM_LEVELSOPEN, IDM_TESTERCALL, IDM_BLOCKFADE, IDM_DUMPMODE, IDM_FREEZE };

// Update menu item enabled/checked states.
void __cdecl OpenLRR_UpdateMenuItems(void)
{
    // Disable menu items that are waiting for initialisation.
    Menu_EnableButtonsArray(Menu_InitCommandLineIDs, Gods98::Main_IsCommandLineParsed());


    ////// &File //////

    Menu_CheckButton(IDM_TOGGLEMENU, (Gods98::Main_GetMenu() != nullptr));
    // disable until we add the WinConsole module
    Menu_EnableButton(IDM_TOGGLECONSOLE, false);


    ////// &Options //////

    Menu_CheckButton(IDM_DUALMOUSE, (Gods98::Main_GetFlags() & Gods98::MainFlags::MAIN_FLAG_DUALMOUSE));

    // This can safely be out of range, beacuse of how the Buttons macros function
    sint32 curCursor = (sint32)Gods98::Main_GetCursorVisibility();
    Menu_CheckRadioButtonsArray(Menu_CursorIDs, curCursor);

    Menu_CheckRadioButtonsArray(Menu_QualityIDs, (sint32)Gods98::graphicsGlobs.renderQuality);
    Menu_CheckButton(IDM_GRAPHICS_BLEND, Gods98::graphicsGlobs.blendTransparency);
    Menu_CheckButton(IDM_GRAPHICS_DITHER, Gods98::graphicsGlobs.dither);
    Menu_CheckButton(IDM_GRAPHICS_FILTER, Gods98::graphicsGlobs.linearFilter);
    Menu_CheckButton(IDM_GRAPHICS_LINEARMIPMAP, Gods98::graphicsGlobs.mipMapLinear);
    Menu_CheckButton(IDM_GRAPHICS_MIPMAP, Gods98::graphicsGlobs.mipMap);
    Menu_CheckButton(IDM_GRAPHICS_SORT, Gods98::graphicsGlobs.sortTransparency);
    Menu_CheckButton(IDM_GRAPHICS_ALPHAMODULATION, Gods98::Graphics_AlphaModulation());

    sint32 curIcon = -1;
    for (uint32 i = 0; i < (uint32)OpenLRRIcon::Count; i++) {
        // Disable menu items for unavailable icons.
        if ((OpenLRRIcon)i != OpenLRRIcon::None && !openlrrGlobs.iconList[i])
            Menu_EnableButton(Menu_IconIDs[i], false);
        else if (openlrrGlobs.iconList[i] == Gods98::Main_GetIcon() && curIcon == -1)
            curIcon = i;
    }
    Menu_CheckRadioButtonsArray(Menu_IconIDs, curIcon);


    ////// &Debug //////
    Menu_CheckButton(IDM_PROGRAMMER,    (Gods98::Main_ProgrammerMode() != 0));
    Menu_CheckButton(IDM_DEBUGMODE,     (Gods98::Main_GetFlags() & Gods98::MainFlags::MAIN_FLAG_DEBUGMODE));
    Menu_CheckButton(IDM_DEBUGCOMPLETE, (Gods98::Main_GetFlags() & Gods98::MainFlags::MAIN_FLAG_DEBUGCOMPLETE));
    Menu_CheckButton(IDM_LEVELSOPEN,    (Gods98::Main_GetFlags() & Gods98::MainFlags::MAIN_FLAG_LEVELSOPEN));
    Menu_CheckButton(IDM_TESTERCALL,    (Gods98::Main_GetFlags() & Gods98::MainFlags::MAIN_FLAG_TESTERCALL));

    Menu_CheckButton(IDM_BLOCKFADE,     (Gods98::Main_GetCLFlags() & Gods98::MainCLFlags::CL_FLAG_BLOCKFADE));
    Menu_CheckButton(IDM_WOBBLYWORLD,   OpenLRR_IsWobblyWorld());

    Menu_CheckButton(IDM_DUMPMODE,      (Gods98::Main_GetFlags() & Gods98::MainFlags::MAIN_FLAG_DUMPMODE));
    Menu_CheckButton(IDM_FREEZE,        Gods98::Main_IsPaused());
    // These are only useful while paused
    Menu_EnableButtonsArray(Menu_AdvanceFrameIDs, Gods98::Main_IsPaused());

}

#pragma endregion


#pragma region OpenLRR_HandleCommand

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
        Gods98::mainGlobs.exit = true;
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

    case IDM_DUALMOUSE:
        //std::printf("IDM_DUALMOUSE\n");
        Gods98::mainGlobs.flags ^= Gods98::MainFlags::MAIN_FLAG_DUALMOUSE;
        break;

    case IDM_CURSOR_NEVER:
    case IDM_CURSOR_TITLEBAR:
    case IDM_CURSOR_ALWAYS:
        /*switch (wmId) {
        case IDM_CURSOR_NEVER:    std::printf("IDM_CURSOR_NEVER\n"); break;
        case IDM_CURSOR_TITLEBAR: std::printf("IDM_CURSOR_TITLEBAR\n"); break;
        case IDM_CURSOR_ALWAYS:   std::printf("IDM_CURSOR_ALWAYS\n"); break;
        }*/
        Gods98::Main_SetCursorVisibility((Gods98::CursorVisibility)(wmId - IDM_CURSOR_NEVER));
        break;

    case IDM_ICON_NONE:
    case IDM_ICON_NATIVE:
    case IDM_ICON_OPENLRR:
    case IDM_ICON_TEAL:
    case IDM_ICON_GOLD:
    case IDM_ICON_CDROM:
        /*switch (wmId) {
        case IDM_ICON_NONE:    std::printf("IDM_ICON_NONE\n"); break;
        case IDM_ICON_NATIVE:  std::printf("IDM_ICON_NATIVE\n"); break;
        case IDM_ICON_OPENLRR: std::printf("IDM_ICON_OPENLRR\n"); break;
        case IDM_ICON_TEAL:    std::printf("IDM_ICON_TEAL\n"); break;
        case IDM_ICON_GOLD:    std::printf("IDM_ICON_GOLD\n"); break;
        case IDM_ICON_CDROM:   std::printf("IDM_ICON_CDROM\n"); break;
        }*/
        Gods98::Main_SetIcon(openlrrGlobs.iconList[(uint32)(wmId - IDM_ICON_NONE)], false);
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
        Gods98::graphicsGlobs.renderQuality = (Gods98::Graphics_Quality)((sint32)(wmId - IDM_QUALITY_WIREFRAME));
        break;

    /*case IDM_GRAPHICS_BLEND:
        //std::printf("IDM_GRAPHICS_BLEND\n");
        Gods98::graphicsGlobs.blendTransparency = !Gods98::graphicsGlobs.blendTransparency;
        break;*/
    case IDM_GRAPHICS_DITHER:
        //std::printf("IDM_GRAPHICS_DITHER\n");
        Gods98::graphicsGlobs.dither = !Gods98::graphicsGlobs.dither;
        break;
    /*case IDM_GRAPHICS_FILTER:
        //std::printf("IDM_GRAPHICS_FILTER\n");
        Gods98::graphicsGlobs.linearFilter = !Gods98::graphicsGlobs.linearFilter;
        break;*/
    case IDM_GRAPHICS_LINEARMIPMAP:
        //std::printf("IDM_GRAPHICS_LINEARMIPMAP\n");
        Gods98::graphicsGlobs.mipMapLinear = !Gods98::graphicsGlobs.mipMapLinear;
        break;
    case IDM_GRAPHICS_MIPMAP:
        //std::printf("IDM_GRAPHICS_MIPMAP\n");
        Gods98::graphicsGlobs.mipMap = !Gods98::graphicsGlobs.mipMap;
        break;
    /*case IDM_GRAPHICS_SORT:
        //std::printf("IDM_GRAPHICS_SORT\n");
        Gods98::graphicsGlobs.sortTransparency = !Gods98::graphicsGlobs.sortTransparency;
        break;*/
	case IDM_GRAPHICS_ALPHAMODULATION:
        //std::printf("IDM_GRAPHICS_ALPHAMODULATION\n");
		Gods98::Graphics_SetAlphaModulation(!Gods98::Graphics_AlphaModulation());
        break;


        ////// &Debug //////

    case IDM_PROGRAMMER:
        //std::printf("IDM_PROGRAMMER\n");
        if (Gods98::mainGlobs.programmerLevel == 0) {
            if (openlrrGlobs.orig_programmerLevel == 0)
                openlrrGlobs.orig_programmerLevel = 1; // default

            Gods98::mainGlobs.programmerLevel = openlrrGlobs.orig_programmerLevel;
        }
        else {
            openlrrGlobs.orig_programmerLevel = Gods98::mainGlobs.programmerLevel;
            Gods98::mainGlobs.programmerLevel = 0; // disabled
        }
        break;

    case IDM_DEBUGMODE:
        //std::printf("IDM_DEBUGMODE\n");
        Gods98::mainGlobs.flags ^= Gods98::MainFlags::MAIN_FLAG_DEBUGMODE;
        break;

    case IDM_DEBUGCOMPLETE:
        //std::printf("IDM_DEBUGCOMPLETE\n");
        Gods98::mainGlobs.flags ^= Gods98::MainFlags::MAIN_FLAG_DEBUGCOMPLETE;
        break;

    case IDM_LEVELSOPEN:
        //std::printf("IDM_LEVELSOPEN\n");
        Gods98::mainGlobs.flags ^= Gods98::MainFlags::MAIN_FLAG_LEVELSOPEN;
        break;

    case IDM_TESTERCALL:
        //std::printf("IDM_TESTERCALL\n");
        Gods98::mainGlobs.flags ^= Gods98::MainFlags::MAIN_FLAG_TESTERCALL;
        break;

    case IDM_BLOCKFADE:
        //std::printf("IDM_BLOCKFADE\n");
        Gods98::mainGlobs.clFlags ^= Gods98::MainCLFlags::CL_FLAG_BLOCKFADE;
        break;

    case IDM_WOBBLYWORLD:
        //std::printf("IDM_WOBBLYWORLD\n");
		OpenLRR_SetWobblyWorld(!OpenLRR_IsWobblyWorld());
        break;

    case IDM_DUMPMODE:
        //std::printf("IDM_DUMPMODE\n");
        Gods98::mainGlobs.flags ^= Gods98::MainFlags::MAIN_FLAG_DUMPMODE;
        // these modes are *basically* mutually exclusive
        if (Gods98::mainGlobs.flags & Gods98::MainFlags::MAIN_FLAG_DUMPMODE)
            Gods98::Main_PauseApp(false);
        break;

    case IDM_FREEZE:
        //std::printf("IDM_FREEZE\n");
        Gods98::Main_PauseApp(!Gods98::Main_IsPaused());
        // these modes are *basically* mutually exclusive
        if (Gods98::Main_IsPaused())
            Gods98::mainGlobs.flags &= ~Gods98::MainFlags::MAIN_FLAG_DUMPMODE;
        break;

    case IDM_ADVANCE_1FRAME:
        //std::printf("IDM_ADVANCE_1FRAME\n");
        Gods98::Main_AdvanceFrame();
        break;

    case IDM_ADVANCE_1SECOND:
        std::printf("IDM_ADVANCE_1SECOND\n");
        Gods98::Main_SetAdvanceFrames((uint32)STANDARD_FRAMERATE);
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

// <LegoRR.exe @00450c20>
#define Map3D_SetBlockUVWobbles ((void (__cdecl* )(LegoRR::Map3D* map, uint32 bx, uint32 by, bool32 on))0x00450c20)

// Reserved field is always 0, and never used.
// <LegoRR.exe @0042f620>
#define Level_BlockUpdateSurface ((void (__cdecl* )(LegoRR::Lego_Level* level, sint32 bx, sint32 by, bool32 reserved))0x0042f620)


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

#undef Map3D_SetBlockUVWobbles
#undef Level_BlockUpdateSurface

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
	if (::_stricmp(programName, "OpenLRR") == 0 ||
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
    openlrrGlobs.hInstMain = hInstance;
    openlrrGlobs.conout = MakeConsole();

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
    openlrrGlobs.iconList[(uint32)OpenLRRIcon::Teal]    = ::LoadIconA(OpenLRR_hInstDll(), MAKEINTRESOURCEA(IDI_CLGEN));
    openlrrGlobs.iconList[(uint32)OpenLRRIcon::Gold]    = ::LoadIconA(OpenLRR_hInstDll(), MAKEINTRESOURCEA(IDI_LEGORR));
    openlrrGlobs.iconList[(uint32)OpenLRRIcon::CDROM]   = ::LoadIconA(OpenLRR_hInstDll(), MAKEINTRESOURCEA(IDI_CDROM));

    openlrrGlobs.menu   = ::LoadMenuA(OpenLRR_hInstDll(), MAKEINTRESOURCEA(IDC_DEBUGSYSMENU));
    openlrrGlobs.accels = ::LoadAcceleratorsA(OpenLRR_hInstDll(), MAKEINTRESOURCEA(IDC_DEBUGSYSMENU));

    if (openlrrGlobs.iconList[(uint32)OpenLRRIcon::OpenLRR])
        Gods98::Main_SetIcon(openlrrGlobs.iconList[(uint32)OpenLRRIcon::OpenLRR], false);
    else if (openlrrGlobs.iconList[(uint32)OpenLRRIcon::Native])
        Gods98::Main_SetIcon(openlrrGlobs.iconList[(uint32)OpenLRRIcon::Native], false);

    Gods98::Main_SetAccel(openlrrGlobs.accels, false);
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
