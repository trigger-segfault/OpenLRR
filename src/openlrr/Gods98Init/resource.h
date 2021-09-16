// Init/resource.h
//

#pragma once

// Used by Init.rc
//
#define IDD_MODEDIALOG                  101   // 1033 : DIALOG : "Mode Selection"
#define IDI_LEGORR						113   // 2057 : ICON_GROUP : (main icon)

#define IDC_DRIVER                      1000  // IDD_MODEDIALOG : LISTBOX : "Driver"
#define IDC_DEVICE                      1002  // IDD_MODEDIALOG : LISTBOX : "Device"
#define IDC_MODE                        1003  // IDD_MODEDIALOG : LISTBOX : "Screen Mode" | "Window Size"
#define IDC_FULLSCREEN                  1004  // IDD_MODEDIALOG : RADIO BUTTON : "Full Screen"
#define IDC_WINDOW                      1005  // IDD_MODEDIALOG : RADIO BUTTON : "Window"
#define IDC_MODELISTTEXT                1006  // IDD_MODEDIALOG : STATIC : "Screen Mode" | "Window Size"


#if 0
// Used by CLGen.rc
//
#define IDD_VIDEOCARDDIALOG				101   // 2057 : DIALOG : "Video Card selector"
#define IDI_CLGEN						103   // 2057 : ICON_GROUP : (main icon)

#define IDC_VIDEOCARD					1001  // IDD_VIDEOCARDDIALOG : COMBOBOX : "Select Video Card type:"
#define IDC_SELECTTEXT					1006  // IDD_VIDEOCARDDIALOG : STATIC : "Select:"

#endif
