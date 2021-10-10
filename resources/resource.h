//{{NO_DEPENDENCIES}}
// Microsoft Visual C++ generated include file.
#pragma once

/////////////////////////////////////////////////////////////////////
// Preprocessor settings

// Include a manifest adding modern windows visual styles. (optional)
#define MANIFEST_VISUALSTYLES
// Use original language (English U.S.) for "Mode Selection" dialog resource. (optional)
#define MODEDIALOG_ORIGLANG


/////////////////////////////////////////////////////////////////////
// General use
//
#define IDM_STATIC						-1
#define IDC_STATIC						-1


/////////////////////////////////////////////////////////////////////
// Used by openlrr.rc
//
#define IDI_OPENLRR						104		// 2057 : ICON_GROUP : (OpenLRR main icon)
#define IDR_MAINFRAME					128
#define IDI_SMALL						129
#define IDS_APP_TITLE                   130
#define IDD_ABOUTBOX					131
#define IDC_DEBUGSYSMENU				132

#define IDMI_FILE						0
#define IDMI_OPTIONS					1
#define IDMI_DEBUG						2
#define IDMI_CHEATS						3
#define IDMI_HELP						4

// &File
#define IDM_TOGGLEMENU					601
#define IDM_TOGGLECONSOLE				602
// --------------
#define IDM_EXIT						603


// &Help
#define IDM_ABOUT						651


// &Options
#define IDM_MUSICON						501
#define IDM_SOUNDON						502
#define IDM_HELPWINDOW					503
#define IDM_AUTOGAMESPEED				504
#define IDM_DUALMOUSE					505
#define IDM_LOSEFOCUSANDPAUSE		    506
// --------------
#define IDM_OBJINFO						511
#define IDM_PANELS						512
#define IDM_TOOLTIPSOUND				513
// --------------
// &Options > Cursor (Radio Group)
#define IDM_CURSOR_NEVER				531
#define IDM_CURSOR_TITLEBAR				532
#define IDM_CURSOR_ALWAYS				533
// &Options > Icon (Radio Group)
#define IDM_ICON_NONE					541
#define IDM_ICON_NATIVE					542
#define IDM_ICON_OPENLRR				543
#define IDM_ICON_TEAL					544
#define IDM_ICON_GOLD					545
#define IDM_ICON_CDROM					546
// --------------
// &Options > Graphics
#define IDM_LIGHTEFFECTS				551
#define IDM_WALLPROMESHES				552
#define IDM_DYNAMICPM					553
// &Options > Graphics > Quality (Radio Group)
#define IDM_QUALITY_WIREFRAME			561
#define IDM_QUALITY_UNLITFLAT			562
#define IDM_QUALITY_FLAT				563
#define IDM_QUALITY_GOURAUD				564
// ------
#define IDM_GRAPHICS_BLEND				565
#define IDM_GRAPHICS_DITHER				566
#define IDM_GRAPHICS_FILTER				567
#define IDM_GRAPHICS_LINEARMIPMAP		568
#define IDM_GRAPHICS_MIPMAP				569
#define IDM_GRAPHICS_SORT				570


// &Debug
//#define IDM_DEBUGENABLED				301
#define IDM_CHEATSENABLED				302
#define IDM_PROGRAMMER					303
#define IDM_DEBUGMODE					304
#define IDM_DEBUGCOMPLETE				305
#define IDM_LEVELSOPEN					306
#define IDM_TESTERCALL					307
// --------------
#define IDM_ALLOWDEBUGKEYS				311
#define IDM_ALLOWEDITMODE				312
#define IDM_SHOWDEBUGTOOLTIPS			313
// --------------
// &Debug > Experimental
#define IDM_BLOCKFADE					321
#define IDM_CLEAR						322
// &Debug > Info
#define IDM_FPSMONITOR					331
#define IDM_MEMORYMONITOR				332
// --------------
#define IDM_DUMPMODE					341
#define IDM_FREEZE						342
#define IDM_ADVANCE_1FRAME				343
#define IDM_ADVANCE_1SECOND				344


// &Cheats
#define IDM_NONERPS						401
#define IDM_UNLOCKCAMERA				402
#define IDM_UNLOCKBUILD					403
//#define IDM_ONLYBUILDONPATHS			404
#define IDM_BUILDWITHOUTPATHS			404
#define IDM_BUILDANYROUGHNESS			405
#define IDM_FPNOCLIP					406
#define IDM_FPCONTROLS					407
// --------------
//#define IDM_ALWAYSROCKFALL				411
#define IDM_NOROCKFALL					411
#define IDM_NOOXYGEN					412
#define IDM_QUICKREINFORCE				413
#define IDM_PEACEFUL					414
// --------------
// &Cheats > Config
#define IDM_ALLOWRENAME					421
#define IDM_DISABLEENDTELEPORT			422
#define IDM_GENERATESPIDERS				423
#define IDM_NOAUTOEAT					424
#define IDM_NOFALLINS					425
#define IDM_NOMULTISELECT				426
#define IDM_SAFECAVERNS					427
#define IDM_SEETHROUGHWALLS				428

// &Cheats > Resources
#define IDM_ADDCRYSTALS					450
#define IDM_ADDORE						451
#define IDM_ADDSTUDS					452
#define IDM_ADDOXYGEN					453
// --------------
#define IDM_SUBCRYSTALS					460
#define IDM_SUBORE						461
#define IDM_SUBSTUDS					462
#define IDM_SUBOXYGEN					463

// &Cheats > Units
#define IDM_SELECTEDLEVELUP				470
#define IDM_SELECTEDHEAL				471
#define IDM_SELECTEDTRAIN				472
#define IDM_SELECTEDEQUIP				473
#define IDM_SELECTEDMAXIMIZE			474
// --------------
#define IDM_ALLLEVELUP					480
#define IDM_ALLHEAL						481
#define IDM_ALLTRAIN					482
#define IDM_ALLEQUIP					483
#define IDM_ALLMAXIMIZE					484
// --------------
#define IDM_KILLCREATURES				491



#define ID_Menu							32771
#define ID_SUBMENUOPEN_SUBMENUITEM		32772
#define ID_SUBMENUOPEN_SUBMENUITEM32773	32773


/////////////////////////////////////////////////////////////////////
// Used by legorr.rc
//
#define IDI_LEGORR						113		// 2057 : ICON_GROUP : (LegoRR.exe main icon)

// Exclusive to beta lego*.rc builds
#define IDD_WARNDIALOG					102


/////////////////////////////////////////////////////////////////////
// Used by init.rc
//
#define IDD_MODEDIALOG					101		// 1033 : DIALOG : "Mode Selection"
#define IDC_DRIVER						1000	// IDD_MODEDIALOG : LISTBOX : "Driver"
#define IDC_DEVICE						1002	// IDD_MODEDIALOG : LISTBOX : "Device"
#define IDC_MODE						1003	// IDD_MODEDIALOG : LISTBOX : "Screen Mode" | "Window Size"
#define IDC_FULLSCREEN					1004	// IDD_MODEDIALOG : AUTORADIOBUTTON : "Full Screen"
#define IDC_WINDOW						1005	// IDD_MODEDIALOG : AUTORADIOBUTTON : "Window"
#define IDC_DEVICELISTTEXT				IDC_STATIC	// IDD_MODEDIALOG : LTEXT : "Device"
#define IDC_DEVICELISTTEXT				IDC_STATIC	// IDD_MODEDIALOG : LTEXT : "Driver"
#define IDC_MODELISTTEXT				1006	// IDD_MODEDIALOG : RTEXT : "Screen Mode" | "Window Size"


/////////////////////////////////////////////////////////////////////
// Used by clgen.rc
//
// Subtract 100 from all 200's IDs to get real CLGen values.
// (prevents conflicts with init.rc)
#define IDD_SELECTORDIALOG				201		// 2057 : DIALOG : "Video Card selector"
#define IDI_CLGEN						203		// 2057 : ICON_GROUP : (CLGen.exe main icon)
#define IDC_SELECT						1001	// IDD_SELECTORDIALOG : COMBOBOX : "Select Video Card type:"
#define IDC_SELECTTEXT					1006	// IDD_SELECTORDIALOG : LTEXT : "Select:"


/////////////////////////////////////////////////////////////////////
// Other
//
#define IDI_CDROM						204		// icon used for the CD drive


/////////////////////////////////////////////////////////////////////
// Next default values for new objects
// 
#ifdef APSTUDIO_INVOKED
#ifndef APSTUDIO_READONLY_SYMBOLS
#define _APS_NO_MFC                     1
#define _APS_NEXT_RESOURCE_VALUE        308
#define _APS_NEXT_COMMAND_VALUE         32774
#define _APS_NEXT_CONTROL_VALUE         1007
#define _APS_NEXT_SYMED_VALUE           308
#endif
#endif
