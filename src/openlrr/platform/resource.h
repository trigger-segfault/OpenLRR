//{{NO_DEPENDENCIES}}
// Microsoft Visual C++ generated include file.


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
#define IDI_SMALL						302
#define IDS_APP_TITLE                   303
#define IDD_ABOUTBOX					304
#define IDM_ABOUT						305
#define IDM_EXIT						306
#define IDC_DEBUGSYSMENU				307
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
