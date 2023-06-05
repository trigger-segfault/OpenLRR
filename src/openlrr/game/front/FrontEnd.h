// FrontEnd.h : 
//

#pragma once

#include "../../common.h"

#include "../../engine/drawing/Flic.h"
#include "../../engine/video/Movie.h"

#include "../GameCommon.h"
#include "../object/ObjectRecall.h"		// For SaveStruct_18
#include "Reward.h"


namespace LegoRR
{; // !<---

/**********************************************************************************
 ******** Constants
 **********************************************************************************/

#pragma region Constants

#pragma endregion

/**********************************************************************************
 ******** Forward Declarations
 **********************************************************************************/

#pragma region Forward Declarations

struct Menu;
struct LevelLink;

#pragma endregion

/**********************************************************************************
 ******** Function Typedefs
 **********************************************************************************/

#pragma region Function Typedefs

typedef void (__cdecl* MenuItem_CycleCallback)(sint32 value);
typedef void (__cdecl* MenuItem_RealSliderCallback)(real32 value);
typedef void (__cdecl* MenuItem_SliderCallback)(sint32 value);
typedef void (__cdecl* MenuItem_TriggerCallback)(void);
typedef void (__cdecl* MenuItem_SelectCallback)(real32 elapsed, sint32 value);
typedef bool32 (__cdecl* LevelLink_RunThroughLinksCallback)(LevelLink* link, void* data);

#pragma endregion

/**********************************************************************************
 ******** Enumerations
 **********************************************************************************/

#pragma region Enums

enum Menu_ModalType : uint32 // [LegoRR/FrontEnd.c|enum:0x4|type:uint] Types of menus only shown over-top of gameplay. (Count treated as Invalid)
{
	Menu_Modal_Paused  = 0,
	Menu_Modal_Options = 1,
	Menu_Modal_Count   = 2, // Treated as invalid
};
assert_sizeof(Menu_ModalType, 0x4);


enum Menu_ScreenType : uint32 // [LegoRR/FrontEnd.c|enum:0x4|type:uint] Types of menus only shown out-of-game. (Load is purely speculataion)
{
	Menu_Screen_Title       = 0,
	Menu_Screen_Missions    = 1,
	Menu_Screen_Training    = 2,
	Menu_Screen_Load_unused = 3, // (unused, load assumed, could also be rewards in a time when it may have been considered a menu)
	Menu_Screen_Save        = 4,
	Menu_Screen_Count       = 5,
};
assert_sizeof(Menu_ScreenType, 0x4);


enum MenuItem_Type : sint32 // [LegoRR/FrontEnd.c|enum:0x4|type:int]
{
	MenuItem_Type_Invalid    = -1,

	MenuItem_Type_Cycle      = 0,
	MenuItem_Type_Trigger    = 1,
	MenuItem_Type_TextInput  = 2,
	MenuItem_Type_Slider     = 3,
	MenuItem_Type_RealSlider = 4,
	MenuItem_Type_Next       = 5,
	MenuItem_Type_Select     = 6,

	MenuItem_Type_Count,
};
assert_sizeof(MenuItem_Type, 0x4);


enum MenuItem_SelectImageType : uint32 // [LegoRR/FrontEnd.c|enum:0x4|type:uint]
{
	MenuItem_SelectImage_Light  = 0,
	MenuItem_SelectImage_Dark   = 1,
	MenuItem_SelectImage_Locked = 2,

	MenuItem_SelectImage_Count,
};
assert_sizeof(MenuItem_SelectImageType, 0x4);


enum MenuOverlay_Type : sint32 // [LegoRR/FrontEnd.c|enum:0x4|type:int]
{
	MenuOverlay_Type_Invalid   = -1,

	MenuOverlay_Type_Flic      = 0, // (".flh", default, does not check extension)
	MenuOverlay_Type_Animation = 1, // ".avi"
	MenuOverlay_Type_Image     = 2, // ".bmp"
	MenuOverlay_Type_Lws       = 3, // ".lws"

	MenuOverlay_Type_Count,
};
assert_sizeof(MenuOverlay_Type, 0x4);


enum MenuFlags : uint32 // [LegoRR/Front.c|flags:0x4|type:uint]
{
	MENU_FLAG_NONE        = 0,
	MENU_FLAG_HASPOSITION = 0x1,
	MENU_FLAG_CANSCROLL   = 0x2,
};
flags_end(MenuFlags, 0x4);


enum MenuItem_SelectItemFlags : uint32 // [LegoRR/Front.c|flags:0x4|type:uint]
{
	SELECTITEM_FLAG_NONE      = 0,
	SELECTITEM_FLAG_HASBANNER = 0x1,
	SELECTITEM_FLAG_HASIMAGE  = 0x2,
	SELECTITEM_FLAG_ENABLED   = 0x4,
};
flags_end(MenuItem_SelectItemFlags, 0x4);


enum Front_RockWipeFlags : uint32 // [LegoRR/Front.c|flags:0x1|type:byte]
{
	ROCKWIPE_FLAG_NONE      = 0,
	ROCKWIPE_FLAG_ANIMATING = 0x1,
	ROCKWIPE_FLAG_NOINPUT   = 0x2,
};
flags_end(Front_RockWipeFlags, 0x4);


enum SaveRewardFlags : uint32
{
	SAVEREWARD_FLAG_NONE      = 0,
	SAVEREWARD_FLAG_COMPLETED = 0x1,
	SAVEREWARD_FLAG_TUTORIAL  = 0x2,
};
flags_end(SaveRewardFlags, 0x4);

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

struct SaveReward
{
	/*0000,4*/	SaveRewardFlags flags;
	/*0004,318c*/	RewardLevel reward;
	/*3190*/
};
assert_sizeof(SaveReward, 0x3190);


// Moved to ObjectRecall.h, as a temporary location that both FrontEnd.h and Object.h can rely on without needing to include these heavy modules.
#if false
struct SaveStruct_18 // [LegoRR/save.c|struct:0x18]
{
	/*00,18*/	undefined field_0x0_0x17[24];
	/*18*/
};
assert_sizeof(SaveStruct_18, 0x18);
#endif


struct SaveData // [LegoRR/save.c|struct:0xb8]
{
	/*00,4*/	undefined4 field_0x0;
	/*04,4*/	sint32 SliderMusicVolume;
	/*08,4*/	sint32 SliderSoundVolume;
	/*0c,4*/	sint32 SliderGameSpeed;
	/*10,4*/	sint32 SliderBrightness;
	/*14,4*/	undefined4 field_0x14;
	/*18,4*/	uint32 missionsCount;
	/*1c,18*/	SaveStruct_18 saveStruct18_1c;
	/*34,4*/	SaveReward* missionsTable; // std::malloc(count_18 * 0x3190)
	/*38,4*/	undefined4 field_0x38;
	/*3c,4*/	undefined4 field_0x3c;
	/*40,4*/	undefined4 field_0x40;
	/*44,4*/	undefined4 field_0x44;
	/*48,4*/	undefined4 field_0x48;
	/*4c,4*/	undefined4 field_0x4c;
	/*50,4*/	undefined4 field_0x50;
	/*54,4*/	undefined4 field_0x54;
	/*58,4*/	undefined4 field_0x58;
	/*5c,4*/	undefined4 field_0x5c;
	/*60,4*/	undefined4 field_0x60;
	/*64,4*/	undefined4 field_0x64;
	/*68,4*/	undefined4 field_0x68;
	/*6c,4*/	undefined4 field_0x6c;
	/*70,4*/	undefined4 field_0x70;
	/*74,4*/	undefined4 field_0x74;
	/*78,4*/	undefined4 field_0x78;
	/*7c,4*/	undefined4 field_0x7c;
	/*80,4*/	undefined4 field_0x80;
	/*84,4*/	undefined4 field_0x84;
	/*88,4*/	undefined4 field_0x88;
	/*8c,4*/	undefined4 field_0x8c;
	/*90,4*/	undefined4 field_0x90;
	/*94,4*/	undefined4 field_0x94;
	/*98,4*/	undefined4 field_0x98;
	/*9c,4*/	undefined4 field_0x9c;
	/*a0,4*/	undefined4 field_0xa0;
	/*a4,4*/	undefined4 field_0xa4;
	/*a8,4*/	undefined4 field_0xa8;
	/*ac,4*/	undefined4 field_0xac;
	/*b0,4*/	undefined4 field_0xb0;
	/*b4,4*/	undefined4 field_0xb4;
	/*b8*/
};
assert_sizeof(SaveData, 0xb8);


struct LevelLink // [LegoRR/FrontEnd.c|struct:0x14]
{
	/*00,4*/	sint32 setIndex; // Index in LevelSet
	/*04,4*/	LevelLink** linkLevels;
	/*08,4*/	uint32 linkCount;
	/*0c,4*/	undefined4 field_c;
	/*10,4*/	bool32 visited; // True if reached in RunThroughRecurse callback.
	/*14*/
};
assert_sizeof(LevelLink, 0x14);


struct LevelSet // [LegoRR/FrontEnd.c|struct:0x14]
{
	/*00,4*/	sint32 count;
	/*04,4*/	char** idNames;
	/*08,4*/	char** langNames;
	/*0c,4*/	LevelLink** levels;
	/*10,4*/	bool32* visitedList; // True if this level has been loaded from level links (probably makes them require unlock)
	/*14*/
};
assert_sizeof(LevelSet, 0x14);



// Only used by HelpWindow(?)
//struct MenuButton // [LegoRR/FrontEnd.c|struct:0x1c]
//{
//	/*00,10*/	Area2F window;
//	/*10,4*/	Gods98::Image* HiImage;
//	/*14,4*/	Gods98::Image* LoImage;
//	/*18,4*/	char* Text;
//	/*1c*/
//};
//assert_sizeof(MenuButton, 0x1c);


struct MenuOverlay // [LegoRR/FrontEnd.c|struct:0x20]
{
	/*00,4*/	char* filename;
	/*04,4*/	MenuOverlay_Type overlayType; // (.flh, .avi, .bmp, .lws)
	/*08,4*/	undefined4 field_8;
	/*0c,8*/	Point2I position;
	/*14,4*/	SFX_ID sfxType;
	/*18,4*/	MenuOverlay* previous;
	/*1c,4*/	undefined4 field_1c;
	/*20*/
};
assert_sizeof(MenuOverlay, 0x20);


struct MenuItem_CycleData // [LegoRR/FrontEnd.c|struct:0x1c]
{
	/*00,4*/	char** nameList; // (cfg: Cycle:[7...])
	/*04,4*/	uint32 cycleCount;
	/*08,4*/	sint32 nameCount; // (cfg: Cycle:[6])
	/*0c,4*/	sint32* valuePtr;
	/*10,4*/	sint32 x2; // (cfg: Cycle:[3])
	/*14,4*/	sint32 y2; // (cfg: Cycle:[4])
	/*18,4*/	MenuItem_CycleCallback callback;
	/*1c*/
};
assert_sizeof(MenuItem_CycleData, 0x1c);


struct MenuItem_RealSliderData // [LegoRR/FrontEnd.c|struct:0x1c]
{
	/*00,4*/	real32* valuePtr;
	/*04,4*/	real32 valueMin; // (cfg: RealSlider:[6])
	/*08,4*/	real32 valueMax; // (cfg: RealSlider:[7])
	/*0c,4*/	real32 valueStep; // (cfg: RealSlider:[8])
	/*10,4*/	sint32 x2; // (cfg: RealSlider:[3])
	/*14,4*/	sint32 y2; // (cfg: RealSlider:[4])
	/*18,4*/	MenuItem_RealSliderCallback callback;
	/*1c*/
};
assert_sizeof(MenuItem_RealSliderData, 0x1c);


struct MenuItem_SliderData // [LegoRR/FrontEnd.c|struct:0x38]
{
	/*00,4*/	sint32* valuePtr;
	/*04,4*/	sint32 valueMin; // (cfg: Slider:[6])
	/*08,4*/	sint32 valueMax; // (cfg: Slider:[7])
	/*0c,4*/	sint32 x2; // (cfg: Slider:[3])
	/*10,4*/	sint32 y2; // (cfg: Slider:[4])
	/*14,4*/	MenuItem_SliderCallback callback;
	/*18,4*/	Gods98::Image* imageBarOff; // (cfg: Slider:[8] "OffBar")
	/*1c,4*/	Gods98::Image* imageBarOn; // (cfg: Slider:[9] "OnBar")
	/*20,4*/	Gods98::Image* imageCapLeft; // (cfg: Slider:[10] "Leftcap")
	/*24,4*/	Gods98::Image* imageCapRight; // (cfg: Slider:[11] "Rightcap")
	/*28,4*/	Gods98::Image* imagePlusLo; // (cfg: Slider:[12] "Plus")
	/*2c,4*/	Gods98::Image* imageMinusLo; // (cfg: Slider:[13] "Minus")
	/*30,4*/	Gods98::Image* imagePlusHi; // (cfg: Slider:[14] "PlusHi")
	/*34,4*/	Gods98::Image* imageMinusHi; // (cfg: Slider:[15] "MinusHi")
	/*38*/
};
assert_sizeof(MenuItem_SliderData, 0x38);


struct MenuItem_TriggerData // [LegoRR/FrontEnd.c|struct:0xc]
{
	/*0,4*/	bool32* valuePtr;
	/*4,4*/	bool32 end; // (cfg: Trigger:[4]) End/close the current MenuSet
	/*8,4*/	MenuItem_TriggerCallback callback;
	/*c*/
};
assert_sizeof(MenuItem_TriggerData, 0xc);


struct MenuItem_SelectItem // [LegoRR/FrontEnd.c|struct:0x20]
{
	/*00,4*/	MenuItem_SelectItemFlags flags; // [0x1 = print name over image, 0x2, 0x4]
	/*04,4*/	char* banner;
	/*08,c*/	Gods98::Image* images[MenuItem_SelectImage_Count]; // (cfg: Level::MenuBMP[0,1,2] -> Hi,Lo,Locked)
	/*14,4*/	sint32 frontEndX; // (cfg: Level::FrontEndX)
	/*18,4*/	sint32 frontEndY; // (cfg: Level::FrontEndY)
	/*1c,4*/	bool32 frontEndOpen; // (cfg: Level::FrontEndOpen)
	/*20*/
};
assert_sizeof(MenuItem_SelectItem, 0x20);


struct MenuItem_SelectData // [LegoRR/FrontEnd.c|struct:0x5c]
{
	/*00,4*/	MenuItem_SelectItem* selItemList;
	/*04,c*/	uint32* widths[MenuItem_SelectImage_Count]; // (image Hi,Lo,Locked widths, 3 identical for font string width)
	/*10,c*/	uint32* heights[MenuItem_SelectImage_Count]; // (image Hi,Lo,Locked heights, 3 identical for font height)
	/*1c,4*/	sint32 selItemCount;
	/*20,4*/	char* string1;
	/*24,4*/	char* string2;
	/*28,4*/	sint32* valuePtr;
	/*2c,4*/	sint32 x2;
	/*30,4*/	sint32 y2;
	/*34,4*/	sint32 selItemHeight;
	/*38,4*/	sint32 scrollCount; // Max number of select items visible in scroll area.
	/*3c,4*/	sint32 xString1;
	/*40,4*/	sint32 yString1;
	/*44,4*/	sint32 xString2;
	/*48,4*/	sint32 yString2;
	/*4c,4*/	sint32 scrollStart; // Index offset of visible scroll area.
	/*50,4*/	undefined4 field_50;
	/*54,4*/	MenuItem_SelectCallback callback;
	/*58,4*/	Menu* nextMenu; // Optional menu to transition to after making a selection.
	/*5c*/
};
assert_sizeof(MenuItem_SelectData, 0x5c);


// No information on this structure's allocation.
//typedef void MenuItem_TextInputData;
struct MenuItem_TextInputData // [LegoRR/FrontEnd.c|struct:0x18]
{
	/*00,4*/	char* valuePtr;
	/*04,4*/	sint32 length;
	/*08,4*/	sint32 caretPos;  // Character index of cursor.
	/*0c,4*/	sint32 maxLength; // Maximum length of value buffer.
	/*10,4*/	sint32 x2;
	/*14,4*/	sint32 y2;
	/*18*/
};
assert_sizeof(MenuItem_TextInputData, 0x18);


union MenuItem_Data_union // [LegoRR/FrontEnd.c|union:0x4]
{
	/*0,4*/	MenuItem_CycleData* cycle;
	/*0,4*/	MenuItem_TriggerData* trigger;
	/*0,4*/ MenuItem_TextInputData* textInput;
	/*0,4*/	MenuItem_SliderData* slider;
	/*0,4*/	MenuItem_RealSliderData* realSlider;
	/*0,4*/	MenuItem_SelectData* select;
	/*0,4*/	Menu* next;
	/*0,4*/ void* data; // Typeless union member for assignment in C++
	/*4*/
};
assert_sizeof(MenuItem_Data_union, 0x4);


struct MenuItem // [LegoRR/FrontEnd.c|struct:0x3c]
{
	/*00,4*/	char* banner; // (cfg: type:[3|5]) Label text
	/*04,4*/	uint32 length;
	/*08,4*/	Gods98::Font* fontHi; // (cfg: Menu::HiFont)
	/*0c,4*/	Gods98::Font* fontLo; // (cfg: Menu::LoFont)
	/*10,4*/	MenuItem_Data_union itemData; // (cfg: type:[0,*])
	/*14,4*/	MenuItem_Type itemType; // (cfg: type:[0])
	/*18,4*/	sint32 x1; // (cfg: type:[1])
	/*1c,4*/	sint32 y1; // (cfg: type:[2])
	/*20,4*/	sint32 centerOffLo;
	/*24,4*/	sint32 centerOffHi;
	/*28,4*/	bool32 isImageItem; // Only true if imageHi is loaded (should be renamed)
	/*2c,4*/	Gods98::Image* imageLo;
	/*30,4*/	Gods98::Image* imageHi;
	/*34,4*/	ToolTip_Type toolTipType;
	/*38,4*/	bool32 notInTuto; // (string value is unchecked, but "NotInTuto" is the only seen usage)
	/*3c*/
};
assert_sizeof(MenuItem, 0x3c);


struct Menu // [LegoRR/FrontEnd.c|struct:0xa0] A singular menu screen contained within a MenuSet structure.
{
	/*00,4*/	char* title; // (cfg: Menu::Title)
	/*04,4*/	char* fullName; // (cfg: Menu::FullName)
	/*08,4*/	uint32 titleLength; // (init: strlen(title))
	/*0c,4*/	Gods98::Font* menuFont; // (cfg: Menu::MenuFont)
	/*10,4*/	Gods98::Image* menuImage; // (cfg: Menu::MenuImage)
	/*14,4*/	Gods98::Image* menuImageDark; // (cfg: Menu::MenuImageDark)
	/*18,4*/	MenuItem** items; // (cfg: Menu::Item1, Item2...)
	/*1c,4*/	sint32 itemCount; // (cfg: Menu::ItemCount) Number of used slots in items
	/*20,4*/	sint32 itemCapacity; // Total number of slots allocated in items
	/*24,4*/	sint32 itemFocus; // Index of item with keyboard focus
	/*28,4*/	bool32 closed; // Signals the menu(screen?) has been closed?
	/*2c,8*/	Point2I position; // (cfg: Menu::Position)
	/*34,4*/	MenuFlags flags; // flags [0x1, 0x2, ...]
	/*38,8*/	Point2F currPosition;
	/*40,4*/	MenuOverlay* overlays; // (cfg: Menu::Overlay1, Overlay2...)
	/*44,4*/	bool32 autoCenter; // (cfg: Menu::AutoCenter)
	/*48,4*/	bool32 displayTitle; // (cfg: Menu::DisplayTitle)
	/*4c,4*/	bool32 anchored; // (cfg: Menu::Anchored)
	/*50,8*/	Point2I anchoredPosition; // (cfg: Menu::Anchored)
	/*58,4*/	sint32 centerX;
	/*5c,40*/	char name[64]; // Name of menu in Lego.cfg
	/*9c,4*/	BoolTri playRandom; // (cfg: Menu::PlayRandom)
	/*a0*/
};
assert_sizeof(Menu, 0xa0);


struct MenuSet // [LegoRR/FrontEnd.c|struct:0x8] A collection of menus that may interact with one another (i.e. MainMenuFull, PausedMenu)
{
	/*0,4*/	Menu** menus; // (cfg: MenuSet::Menu1, Menu2...)
	/*4,4*/	uint32 menuCount; // (cfg: MenuSet::MenuCount)
	/*8*/
};
assert_sizeof(MenuSet, 0x8);


struct MenuTextWindow // [LegoRR/FrontEnd.c|struct:0x728] Menu/FrontEnd text structure, see CFG blocks: SaveText, LevelText
{
	/*000,4*/	Gods98::TextWindow* textWindow;
	/*004,4*/	Gods98::Image* PanelImage;
	/*008,10*/	Area2F WindowArea;
	/*018,10*/	Area2F PanelArea;
	/*028,100*/	char LoadText[256];
	/*128,100*/	char SaveText[256];
	/*228,100*/	char SlotText[256]; // format: %d (save number)
	/*328,100*/	char LoadSelText[256]; // format: %d (save number)
	/*428,100*/	char SaveSelText[256]; // format: %d (save number)
	/*528,100*/	char LevelText[256];
	/*628,100*/	char TutorialText[256];
	/*728*/
};
assert_sizeof(MenuTextWindow, 0x728);


struct MenuItemCallbackPair // [LegoRR/dummy.c|struct:0x8] Helper struct
{
	/*0,4*/	void* value; // may be sint32, real32, or nullptr
	/*4,4*/	void* callback;
	/*8*/
};
assert_sizeof(MenuItemCallbackPair, 0x8);


struct SearchLevelSelectAdd
{
	LevelSet* levelSet;
	Menu* menu_4;
	MenuItem_SelectData* itemData;
	SaveData* saveData;
};
assert_sizeof(SearchLevelSelectAdd, 0x10);


struct SearchLevelIdentifier_10 // [LegoRR/search.c|struct:0x10]
{
	/*00,4*/	LevelLink* resultLink; // output level identifier (not modified on failure)
	/*04,4*/	sint32 searchIndex; // (target index to search for)
	/*08,4*/	sint32 currentIndex; // (counter index of individual linked levels)
	/*0c,4*/	sint32 resultIndex; // (equal to searchIndex on success)
	/*10*/
};
assert_sizeof(SearchLevelIdentifier_10, 0x10);


struct SearchLevelSelectInfo_14 // [LegoRR/search.c|struct:0x14]
{
	/*00,4*/	SaveReward* saveReward; // (optional)
	/*04,4*/	LevelSet* levelSet;
	/*08,4*/	MenuItem_SelectData* selectData;
	/*0c,4*/	uint32 index; // LevelLink linked list index/selectData selItem index
	/*10,4*/	bool32 keepLocked; // When saveReward is null, this specifies if the level will remain in their default locked state.
	/*14*/
};
assert_sizeof(SearchLevelSelectInfo_14, 0x14);


struct Front_Cache // [LegoRR/FrontEnd.c|struct:0x10] Cache for image/fonts loaded from menus
{
	/*00,4*/	char* path;
	/*04,4*/	Gods98::Image* image;
	/*08,4*/	Gods98::Font* font;
	/*0c,4*/	Front_Cache* next;
	/*10*/
};
assert_sizeof(Front_Cache, 0x10);


struct Front_Globs // [LegoRR/FrontEnd.c|struct:0x884|tags:GLOBS]
{
	/*000,4*/	MenuSet* pausedMenuSet;  // (cfg: Menu::PausedMenu)
	/*004,4*/	MenuSet* mainMenuSet;    // (cfg: Menu::MainMenuFull)
	/*008,4*/	MenuSet* optionsMenuSet; // (cfg: Menu::OptionsMenu)
	/*00c,4*/	MenuSet* saveMenuSet;    // (cfg: Menu::SaveMenu)
	/*010,14*/	LevelSet tutorialLevels;      // (cfg: Main::TutorialStartLevel)
	/*024,14*/	LevelSet missionLevels;       // (cfg: Main::StartLevel)
	/*038,4*/	LevelLink* startMissionLink;  // (cfg: Main::StartLevel)
	/*03c,4*/	LevelLink* startTutorialLink; // (cfg: Main::TutorialStartLevel)
	/*040,10*/	undefined4 reserved1[4];
	/*050,4*/	sint32 triggerCredits;         // [trigger: 1=play credits]
	/*054,8*/	undefined4 reserved2[2];
	/*05c,4*/	sint32 triggerQuitApp;         // [trigger: 1=yes quit game]
	/*060,4*/	sint32 selectMissionIndex;     // [select: -1, or pressed mission item index]
	/*064,4*/	sint32 selectTutorialIndex;    // [select: -1, or pressed tutorial item index]
	/*068,4*/	sint32 selectLoadSaveIndex;    // [select: -1, or pressed save item index]
	/*06c,4*/	sint32 triggerContinueMission; // [trigger: 1=close pause menu]
	/*070,4*/	sint32 sliderGameSpeed;        // [slider: 0-5]
	/*074,4*/	sint32 sliderSFXVolume;        // [slider: 0-10]
	/*078,4*/	sint32 sliderMusicVolume;      // [slider: 0-10]
	/*07c,4*/	sint32 sliderBrightness;       // [slider: 0-10]
	/*080,4*/	sint32 cycleHelpWindow;        // [cycle: Off,On]
	/*084,4*/	sint32 triggerReplayObjective; // [trigger: 1=replay]
	/*088,4*/	sint32 triggerQuitMission;     // [trigger: 1=quit level]
	/*08c,4*/	sint32 triggerRestartMission;  // [trigger: 1=restart level]
	/*090,4*/	sint32 cycleWallDetail;        // [cycle: High,Low]
	/*094,4*/	sint32 cycleMusicOn;           // [cycle: On,Off]
	/*098,4*/	sint32 cycleSFXOn;             // [cycle: On,Off]
	/*09c,4*/	sint32 cycleAutoGameSpeed;     // [cycle: On,Off]
	/*0a0,4*/	sint32 triggerBackSave;        // [trigger: 1=leave save menu]
	/*0a4,8*/	Point2I overlayPosition;
	/*0ac,4*/	Gods98::Image_Flic overlayImageOrFlic; // Image* or Flic* type
	/*0b0,4*/	uint32 overlayStartTime; // starting Main_GetTime (milliseconds)
	/*0b4,4*/	uint32 overlayCurrTime; // current Main_GetTime (milliseconds)
	/*0b8,4*/	Gods98::Font* versionFont; // (file: fontBriefingHi)
	/*0bc,4*/	char* versionString; // (cfg: Main::Version)
	/*0c0,450*/	SaveData saveData[6];
	/*510,4*/	sint32 saveNumber;
	/*514,4*/	char* strDefaultLevelBMPS; // (cfg: Menu::DefaultLevelBMPS) memory storage
	/*518,4*/	undefined4 reserved3;
	/*51c,8*/	Point2I scrollOffset; // Offset position in level select (or anywhere else with a larger menu image)
	/*524,8*/	undefined4 reserved4[2];
	/*52c,4*/	Gods98::Container* rockWipeAnim;
	/*530,4*/	Front_RockWipeFlags rockWipeFlags;
	/*534,4*/	real32 rockWipeTimer; // RockWipe animation timer in animation frames units.
	/*538,4*/	real32 rockWipeLastUpdateTime;
	/*53c,4*/	Gods98::Container* rockWipeLight;
	/*540,4*/	bool32 saveBool_540;
	/*544,4*/	bool32 isLoadModeBool_544;
	/*548,4*/	bool32 saveBool_548;
	/*54c,8*/	Size2I saveImageBigSize;
	/*554,40*/	char langLoadGame[64];
	/*594,40*/	char langSaveGame[64];
	/*5d4,4*/	MenuTextWindow* saveTextWnd;
	/*5d8,4*/	MenuTextWindow* saveLevelWnd;
	/*5dc,80*/	char langOverwriteTitle[128];
	/*65c,100*/	char langOverwriteMessage[256];
	/*75c,80*/	char langOverwriteOK[128];
	/*7dc,80*/	char langOverwriteCancel[128];
	/*85c,4*/	bool32 saveBool_85c;
	/*860,4*/	undefined4 reserved5;
	/*864,4*/	undefined4 unused_zero_864; // (init: 0) Set to 0 and never touched?
	/*868,4*/	sint32 maxLevelScreens;
	/*86c,8*/	undefined4 reserved6[2];
	/*874,4*/	uint32 levelSelectHoverNumber;
	/*878,4*/	uint32 levelSelectLastNumber;
	/*87c,4*/	bool32 levelSelectSFXStopped;
	/*880,4*/	real32 levelSelectSFXTimer;
	/*884*/
};
assert_sizeof(Front_Globs, 0x884);

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @004a2f48>
extern sint32 & g_SaveMenu_INDEX_004a2f48; // = -1;

// <LegoRR.exe @004a2f4c>
extern sint32 & g_SaveMenu_INT_004a2f4c; // = -1;

// <LegoRR.exe @004a2f50>
extern sint32 & g_SaveMenu_INDEX_004a2f50; // = -1;

// <LegoRR.exe @004a2f54>
extern sint32 & g_SaveMenu_INDEX_004a2f54; // = -1;

// <LegoRR.exe @004a2f58>
extern sint32 & g_SaveMenu_INT_004a2f58; // = -1;

// <LegoRR.exe @004a2f5c>
extern sint32 & g_SaveMenu_OutNumber; // = -1;

// As a heads up, this IS NOT the array used in GODS98, and seems to exist solely for Rename Input in LRR.
// <LegoRR.exe @004a2f60>
extern const uint32 (& c_keyCharMap)[256];


// <LegoRR.exe @004d88c8>
extern sint32 (& s_LevelSelect_NameXs)[16];


// <LegoRR.exe @004d8b08>
extern Gods98::Font* (& s_LevelSelectNameFonts)[16];

// <LegoRR.exe @004d8b48>
extern char (& s_FrontLevelSelectNames)[15][1024];

// <LegoRR.exe @004dc748>
extern char (& s_FrontReplaceSpacesBuff)[256];


// <LegoRR.exe @004dc850>
extern sint32 (& s_LevelSelect_NameYs)[16];

// <LegoRR.exe @004dc890>
extern Gods98::Image* (& g_SaveSlotImages_TABLE)[5];

// <LegoRR.exe @004dc8a4>
extern undefined4 & DAT_004dc8a4;

// <LegoRR.exe @004dc8a8>
extern uint32 (& g_SaveSlotPtrs_TABLE)[5];

// <LegoRR.exe @004dc8bc>
extern bool32 & g_FrontBool_004dc8bc;

// <LegoRR.exe @004dc8c0>
extern bool32 & g_FrontBool_004dc8c0;

// <LegoRR.exe @004dc8c4>
extern bool32 & g_FrontBool_004dc8c4;

// <LegoRR.exe @004dc8c8>
extern bool32 & g_FrontBool_004dc8c8;

// Linked list for commonly-loaded image files. Never cleaned up.
// <LegoRR.exe @004dc8cc>
extern Front_Cache* (& g_ImageCache_NEXT);

// <LegoRR.exe @004dc8d0>
extern uint32 & s_LevelSelectNameCount;

// <LegoRR.exe @004dc8d4>
extern bool32 & g_FrontBool_004dc8d4;


// <LegoRR.exe @004dc8dc>
extern sint32 & s_frontTextInputCaretBlinker;

// <LegoRR.exe @004dc8e0>
extern Menu* (& s_FrontOptionsMenu);

// <LegoRR.exe @004dc8e4>
extern MenuSet* (& s_FrontOptionsMenuSet);


// <LegoRR.exe @00557fc0>
extern Front_Globs & frontGlobs;

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// Returns a temporary string buffer. Or str if the temporary buffer is too small.
// <LegoRR.exe @004101e0>
const char* __cdecl Front_Util_ReplaceTextSpaces(const char* str);

// <LegoRR.exe @00410250>
void __cdecl Front_LevelSelect_LevelNamePrintF(Gods98::Font* font, sint32 x, sint32 y, const char* msg, ...);

// <LegoRR.exe @00410300>
void __cdecl Front_Callback_TriggerPlayCredits(void);

// <LegoRR.exe @00410320>
void __cdecl Front_RockWipe_Play(void);

// <LegoRR.exe @00410370>
void __cdecl Front_RockWipe_Stop(void);

// <LegoRR.exe @00410380>
Front_Cache* __cdecl Front_Cache_FindByName(const char* filename);

// <LegoRR.exe @004103c0>
Front_Cache* __cdecl Front_Cache_Create(const char* filename);

// <LegoRR.exe @00410450>
Gods98::Image* __cdecl Front_Cache_LoadImage(const char* filename);

// <LegoRR.exe @00410490>
Gods98::Font* __cdecl Front_Cache_LoadFont(const char* filename);

// Same as `Gods98::Util_StrCpy`, but with null checks.
// <LegoRR.exe @004104d0>
char* __cdecl Front_Util_StrCpy(const char* str);

// <LegoRR.exe @00410520>
MenuItem_SelectData* __cdecl Front_MenuItem_CreateSelect(IN sint32* valuePtr, const char* string1, const char* string2, sint32 x2, sint32 y2,
														 sint32 selItemHeight, sint32 scrollCount, sint32 xString1, sint32 yString1, sint32 xString2,
														 sint32 yString2, sint32 field50, MenuItem_SelectCallback callback,
														 OPTIONAL Menu* nextMenu);

// <LegoRR.exe @004105c0>
void __cdecl Front_MenuItem_FreeSelect(MenuItem_SelectData* selectData);

// <LegoRR.exe @00410670>
void __cdecl Front_MenuItem_AddSelectItem(MenuItem_SelectData* selectData, char* bannerOrBMPName, bool32 enabled, Gods98::Font* hiFont,
										  sint32 frontEndX, sint32 frontEndY, bool32 frontEndOpen);

// <LegoRR.exe @00410940>
MenuItem_CycleData* __cdecl Front_MenuItem_CreateCycle(uint32 count, IN sint32* valuePtr, sint32 x2, sint32 y2, MenuItem_CycleCallback callback);

// <LegoRR.exe @004109d0>
void __cdecl Front_MenuItem_AddCycleName(MenuItem_CycleData* cycleData, const char* name);

// <LegoRR.exe @00410a10>
void __cdecl Front_MenuItem_FreeCycle(MenuItem_CycleData* cycleData);

// <LegoRR.exe @00410a90>
MenuItem_TriggerData* __cdecl Front_MenuItem_CreateTrigger(IN bool32* valuePtr, bool32 end, MenuItem_TriggerCallback callback);

// <merged with freeNonNull @ 0040f6e0>
void __cdecl Front_MenuItem_FreeTrigger(MenuItem_TriggerData* triggerData);

// <merged with freeNonNull @ 0040f6e0>
void __cdecl Front_MenuItem_FreeTextInput(MenuItem_TextInputData* textInputData);

// <LegoRR.exe @00410ac0>
MenuItem_RealSliderData* __cdecl Front_MenuItem_CreateRealSlider(IN real32* valuePtr, real32 valueMin, real32 valueMax, real32 step,
																 sint32 x2, sint32 y2, MenuItem_RealSliderCallback callback);

// <merged with freeNonNull @ 0040f6e0>
void __cdecl Front_MenuItem_FreeRealSlider(MenuItem_RealSliderData* realSliderData);

// <LegoRR.exe @00410b20>
MenuItem_SliderData* __cdecl Front_MenuItem_CreateSlider(IN sint32* valuePtr, sint32 valueMin, sint32 valueMax, sint32 x2, sint32 y2,
														 MenuItem_SliderCallback callback, Gods98::Image* offBarImage, Gods98::Image* onBarImage,
														 Gods98::Image* leftCapImage, Gods98::Image* rightCapImage, Gods98::Image* loPlusImage,
														 Gods98::Image* loMinusImage, Gods98::Image* hiPlusImage, Gods98::Image* hiMinusImage);

// <merged with freeNonNull @ 0040f6e0>
void __cdecl Front_MenuItem_FreeSlider(MenuItem_SliderData* sliderData);

// <LegoRR.exe @00410ba0>
void __cdecl Front_MenuItem_FreeMenuItem(MenuItem* menuItem);

// <LegoRR.exe @00410c80>
MenuItem* __cdecl Front_MenuItem_CreateBannerItem(const char* banner, Gods98::Font* loFont, Gods98::Font* hiFont, sint32 x1, sint32 y1,
												  MenuItem_Type itemType, bool32 centered, void* itemData, bool32 notInTuto);

// <LegoRR.exe @00410d50>
MenuItem* __cdecl Front_MenuItem_CreateImageItem(const char* banner, Gods98::Font* loFont, Gods98::Font* hiFont, char* loImageName, char* hiImageName,
												 sint32 x1, sint32 y1, MenuItem_Type itemType, bool32 centered, char* toolTipName, void* itemData);

// <LegoRR.exe @00410e60>
void __cdecl Front_Menu_FreeMenu(Menu* menu);

// <LegoRR.exe @00410ee0>
bool32 __cdecl Front_Menu_LoadMenuImage(Menu* menu, char* filename, bool32 light);

// <LegoRR.exe @00411030>
#define Front_Menu_CreateMenu ((Menu* (__cdecl* )(char* title, char* fullName, Gods98::Font* menuFont, sint32 positionX, sint32 positionY, bool32 autoCenter, bool32 displayTitle, sint32 centerX, bool32 canScroll, char* anchored_str))0x00411030)
//Menu* __cdecl Front_Menu_CreateMenu(char* title, char* fullName, Gods98::Font* menuFont, sint32 positionX, sint32 positionY, bool32 autoCenter, bool32 displayTitle, sint32 centerX, bool32 canScroll, char* anchored_str);

// <LegoRR.exe @00411190>
bool32 __cdecl Front_Menu_AddMenuItem(Menu* menu, MenuItem* menuItem);

// <LegoRR.exe @00411210>
#define Front_Maths_IsPointInsideRect ((bool32 (__cdecl* )(sint32 ptX, sint32 ptY, sint32 rcX, sint32 rcY, sint32 rcWidth, sint32 rcHeight))0x00411210)
//bool32 __cdecl Front_Maths_IsPointInsideRect(sint32 ptX, sint32 ptY, sint32 rcX, sint32 rcY, sint32 rcWidth, sint32 rcHeight);

// <LegoRR.exe @00411250>
#define Front_Maths_IsPointInsideRect_OptCenterX ((bool32 (__cdecl* )(sint32 ptX, sint32 ptY, sint32 rcX, sint32 rcY, sint32 rcWidth, sint32 rcHeight, bool32 shouldCenterX))0x00411250)
//bool32 __cdecl Front_Maths_IsPointInsideRect_OptCenterX(sint32 ptX, sint32 ptY, sint32 rcX, sint32 rcY, sint32 rcWidth, sint32 rcHeight, bool32 shouldCenterX);

// <LegoRR.exe @00411290>
#define Front_MenuItem_Select_CollisionCheck_FUN_00411290 ((sint32 (__cdecl* )(Menu* menu, MenuItem* menuItem, MenuItem_SelectData* selectData))0x00411290)
//sint32 __cdecl Front_MenuItem_Select_CollisionCheck_FUN_00411290(Menu* menu, MenuItem* menuItem, MenuItem_SelectData* selectData);

// <LegoRR.exe @00411420>
#define Front_Menu_IsLevelItemUnderMouse ((bool32 (__cdecl* )(Menu* menu, sint32 itemIndex))0x00411420)
//bool32 __cdecl Front_Menu_IsLevelItemUnderMouse(Menu* menu, sint32 itemIndex);

// <LegoRR.exe @00411460>
#define Front_Menu_GetItemBounds ((bool32 (__cdecl* )(Menu* menu, sint32 itemIndex, OUT sint32* rcX, OUT sint32* rcY, OUT sint32* rcWidth, OUT sint32* rcHeight))0x00411460)
//bool32 __cdecl Front_Menu_GetItemBounds(Menu* menu, sint32 itemIndex, OUT sint32* rcX, OUT sint32* rcY, OUT sint32* rcWidth, OUT sint32* rcHeight);

// itemIndex is not assigned on failure.
// <LegoRR.exe @004116c0>
#define Front_Menu_FindItemUnderMouse ((bool32 (__cdecl* )(Menu* menu, OUT sint32* itemIndex))0x004116c0)
//bool32 __cdecl Front_Menu_FindItemUnderMouse(Menu* menu, OUT sint32* itemIndex);

// <LegoRR.exe @00411770>
#define Front_GetMousePressedState ((bool32 (__cdecl* )(void))0x00411770)
//bool32 __cdecl Front_GetMousePressedState(void);

// <LegoRR.exe @004117a0>
#define Front_MenuItem_SliderHandleInput ((bool32 (__cdecl* )(Menu* menu, MenuItem* menuItem, MenuItem_SliderData* sliderData))0x004117a0)
//bool32 __cdecl Front_MenuItem_SliderHandleInput(Menu* menu, MenuItem* menuItem, MenuItem_SliderData* sliderData);

// <LegoRR.exe @00411900>
#define Front_MenuItem_CheckNotInTutoAnyTutorialFlags ((bool32 (__cdecl* )(MenuItem* menuItem))0x00411900)
//bool32 __cdecl Front_MenuItem_CheckNotInTutoAnyTutorialFlags(MenuItem* menuItem);

// <LegoRR.exe @00411930>
#define Front_Menu_UpdateMenuItemsInput ((Menu* (__cdecl* )(real32 elapsed, Menu* menu))0x00411930)
//Menu* __cdecl Front_Menu_UpdateMenuItemsInput(real32 elapsed, Menu* menu);

// See Keys enum
// (can't use Keys enum as argument for define, Keys is uint8, so undefined data may be passed in higher bytes)
// <LegoRR.exe @00411e30>
#define Front_Input_GetKeyCharacter ((uint32 (__cdecl* )(uint32 diKey))0x00411e30)
//uint32 __cdecl Front_Input_GetKeyCharacter(Keys diKey);

// valueIndex == (value - valueMin);
// valueRange == (valueMax - valueMin);
// <LegoRR.exe @00411e40>
#define Front_MenuItem_DrawSlider ((void (__cdecl* )(MenuItem_SliderData* sliderData, uint32 x, uint32 y, sint32 valueIndex, uint32 valueRange))0x00411e40)
//void __cdecl Front_MenuItem_DrawSlider(MenuItem_SliderData* sliderData, uint32 x, uint32 y, sint32 valueIndex, uint32 valueRange);

// <LegoRR.exe @004120a0>
#define Front_Menu_GetOverlayCount ((uint32 (__cdecl* )(Menu* menu))0x004120a0)
//uint32 __cdecl Front_Menu_GetOverlayCount(Menu* menu);

// <LegoRR.exe @004120c0>
//#define Front_Menu_ShouldRandomPlay ((bool32 (__cdecl* )(void))0x004120c0)
bool32 __cdecl Front_Menu_ShouldRandomPlay(void);

// <LegoRR.exe @004120e0>
#define Front_Menu_UpdateOverlays ((void (__cdecl* )(Menu* menu))0x004120e0)
//void __cdecl Front_Menu_UpdateOverlays(Menu* menu);

// <LegoRR.exe @00412380>
void __cdecl Front_MenuItem_DrawSelectItem(sint32 x, sint32 y, Gods98::Font* font, MenuItem_SelectData* selectData, uint32 selIndex, MenuItem_SelectImageType imageType);

// <LegoRR.exe @00412420>
#define Front_MenuItem_DrawSaveImage ((void (__cdecl* )(Menu* menu, sint32 selIndex, MenuItem_SelectData* selectData, bool32 bigSize))0x00412420)
//void __cdecl Front_MenuItem_DrawSaveImage(Menu* menu, sint32 selIndex, MenuItem_SelectData* selectData, bool32 bigSize);

// <LegoRR.exe @00412680>
#define Front_Menu_DrawLoadSaveText ((void (__cdecl* )(Menu** pMenu, IN OUT bool32* currBool, IN OUT bool32* nextBool))0x00412680)
//void __cdecl Front_Menu_DrawLoadSaveText(Menu** pMenu, IN OUT bool32* currBool, IN OUT bool32* nextBool);

// <LegoRR.exe @00412900>
#define Front_MenuItem_DrawSelectTextWindow ((void (__cdecl* )(Menu** pMenu))0x00412900)
//void __cdecl Front_MenuItem_DrawSelectTextWindow(Menu** pMenu);

// <LegoRR.exe @00412a20>
#define Front_Menu_DrawMenuImage ((void (__cdecl* )(Menu* menu, bool32 light))0x00412a20)
//void __cdecl Front_Menu_DrawMenuImage(Menu* menu, bool32 light);

// <LegoRR.exe @00412b30>
//#define Front_Menu_Update ((Menu* (__cdecl* )(real32 elapsed, Menu* menu, bool32* optout_bool))0x00412b30)
Menu* __cdecl Front_Menu_Update(real32 elapsed, Menu* menu, OUT bool32* optout_bool);

// <LegoRR.exe @004138a0>
void __cdecl Front_Menu_UpdateMousePosition(Menu* menu);

// <LegoRR.exe @00413990>
void __cdecl Front_LoadSaveSlotImages(void);

// <LegoRR.exe @00413a80>
void __cdecl Front_FreeSaveSlotImages(void);

// <LegoRR.exe @00413ab0>
#define Front_ScreenMenuLoop ((void (__cdecl* )(Menu* menu))0x00413ab0)
//void __cdecl Front_ScreenMenuLoop(Menu* menu);

// <LegoRR.exe @00413d50>
#define Front_RunScreenMenu ((void (__cdecl* )(MenuSet* menuSet, uint32 menuIndex))0x00413d50)
//void __cdecl Front_RunScreenMenu(MenuSet* menuSet, uint32 menuIndex);

// <LegoRR.exe @00413d90>
MenuItem_Type __cdecl Front_MenuItem_ParseTypeString(const char* itemTypeName);

// Returns str.
// <LegoRR.exe @00413e30>
//#define Front_Util_StringReplaceChar ((char* (__cdecl* )(IN OUT char* str, char origChar, char newChar))0x00413e30)
char* __cdecl Front_Util_StringReplaceChar(IN OUT char* str, char origChar, char newChar);

// <LegoRR.exe @00413e60>
MenuOverlay_Type __cdecl Front_Menu_GetOverlayType(MenuOverlay* menuOverlay);

// <LegoRR.exe @00413ec0>
MenuOverlay* __cdecl Front_Menu_CreateOverlay(const char* filename, IN OUT MenuOverlay** linkedOverlay, sint32 positionX, sint32 positionY, SFX_ID sfxType);

// <LegoRR.exe @00413f40>
void __cdecl Front_Menu_LoadSliderImages(sint32 numParts, IN char** stringParts, OUT Gods98::Image** images);

// <LegoRR.exe @00413fa0>
MenuSet* __cdecl Front_CreateMenuSet(uint32 menuCount);

// <LegoRR.exe @00413ff0>
#define Front_LoadMenuSet ((MenuSet* (__cdecl* )(const Gods98::Config* unused_config, const char* menuName, void* dst, void* callback, ...))0x00413ff0)
//MenuSet* __cdecl Front_LoadMenuSet(const Gods98::Config* unused_config, const char* menuName, void* dst, void* callback, ...);

// <LegoRR.exe @00414bc0>
sint32 __cdecl Front_GetMenuIDByName(MenuSet* menuSet, const char* name);

// <LegoRR.exe @00414c10>
//#define Front_IsIntrosEnabled ((bool32 (__cdecl* )(void))0x00414c10)
bool32 __cdecl Front_IsIntrosEnabled(void);

// <LegoRR.exe @00414c60>
void __cdecl Front_Callback_SliderBrightness(sint32 slider_0_10);

// <LegoRR.exe @00414d10>
void __cdecl Front_Callback_SliderSoundVolume(sint32 slider_0_10);

// <LegoRR.exe @00414d20>
void __cdecl Front_Callback_SliderMusicVolume(sint32 slider_0_10);

// <LegoRR.exe @00414d40>
sint32 __cdecl Front_CalcSliderCDVolume(void);

// <LegoRR.exe @00414d80>
//#define Front_Callback_CycleWallDetail ((void (__cdecl* )(sint32 cycle_High_Low))0x00414d80)
void __cdecl Front_Callback_CycleWallDetail(sint32 cycle_High_Low);

// <LegoRR.exe @00414db0>
void __cdecl Front_Callback_CycleAutoGameSpeed(sint32 cycle_On_Off);

// <LegoRR.exe @00414dd0>
//#define Front_Callback_CycleMusic ((void (__cdecl* )(sint32 cycle_On_Off))0x00414dd0)
void __cdecl Front_Callback_CycleMusic(sint32 cycle_On_Off);

// <LegoRR.exe @00414df0>
//#define Front_Callback_CycleSound ((void (__cdecl* )(sint32 cycle_On_Off))0x00414df0)
void __cdecl Front_Callback_CycleSound(sint32 cycle_On_Off);

// <LegoRR.exe @00414e10>
void __cdecl Front_Callback_CycleHelpWindow(sint32 cycle_Off_On);

// <LegoRR.exe @00414e40>
void __cdecl Front_Callback_TriggerReplayObjective(void);

// <LegoRR.exe @00414e50>
void __cdecl Front_Callback_SliderGameSpeed(sint32 slider_0_5);

// <LegoRR.exe @00414ec0>
void __cdecl Front_UpdateSliderGameSpeed(void);

// <LegoRR.exe @00414f60>
sint32 __cdecl Front_CalcSliderGameSpeed(void);

// <LegoRR.exe @00414fe0>
void __cdecl Front_Callback_SelectLoadSave(real32 elapsedAbs, sint32 selectIndex);

// <LegoRR.exe @00415080>
void __cdecl Front_UpdateOptionsSliders(void);

// <LegoRR.exe @004150c0>
void __cdecl Front_Callback_TriggerBackSave(void);

// <LegoRR.exe @00415150>
//#define Front_RestartLevel ((void (__cdecl* )(void))0x00415150)
void __cdecl Front_RestartLevel(void);

// <LegoRR.exe @004151f0>
void __cdecl Front_Save_GetLevelCompleteWithPoints(OPTIONAL SaveData* saveData, OUT char* buffer);

// <LegoRR.exe @00415290>
void __cdecl Front_UpdateGameSpeedSliderLevel(void);

// param isOptions: 0 = PausedMenu, 1 = OptionsMenu, 2 = not supported
// <LegoRR.exe @004152a0>
//#define Front_Options_Update ((bool32 (__cdecl* )(real32 elapsed, Menu_ModalType modalType))0x004152a0)
bool32 __cdecl Front_Options_Update(real32 elapsed, Menu_ModalType modalType);

// levelKey is either "StartLevel" or "TutorialStartLevel".
// <LegoRR.exe @004153e0>
#define Front_LoadLevelSet ((bool32 (__cdecl* )(const Gods98::Config* config, IN OUT LevelSet* levelSet, const char* levelKey))0x004153e0)
//bool32 __cdecl Front_LoadLevelSet(const Gods98::Config* config, IN OUT LevelSet* levelSet, const char* levelKey);

// Plays an alread-loaded Movie_t from the G98CMovie C wrapper API.
// Allows terminating the movie playback during runtime with isSkippable.
// <LegoRR.exe @00415630>
void __cdecl Front_PlayMovie(Gods98::Movie_t* mov, bool32 skippable);

// <LegoRR.exe @004156f0>
void __cdecl Front_PlayIntroSplash(const char* imageKey, bool32 skippable, const char* timeKey);

// <LegoRR.exe @00415840>
void __cdecl Front_PlayIntroMovie(const char* movieKey, bool32 skippable);

// <LegoRR.exe @004158c0>
//#define Front_PlayLevelMovie ((void (__cdecl* )(const char* levelName, bool32 skippable))0x004158c0)
void __cdecl Front_PlayLevelMovie(const char* levelName, bool32 skippable);


// <LegoRR.exe @00415940>
//#define Front_LoadLevels ((void (__cdecl* )(MenuSet* unused_mainMenuFull))0x00415940)
void __cdecl Front_LoadLevels(MenuSet* unused_mainMenuFull);

// Only called during Lego_Initialise.
// <LegoRR.exe @00415c20>
//#define Front_ResetSaveNumber ((void (__cdecl* )(void))0x00415c20)
void __cdecl Front_ResetSaveNumber(void);

// <LegoRR.exe @00415c30>
#define Front_LoadMenuTextWindow ((void (__cdecl* )(const Gods98::Config* config, const char* gameName, MenuTextWindow* menuWnd))0x00415c30)
//void __cdecl Front_LoadMenuTextWindow(const Gods98::Config* config, const char* gameName, MenuTextWindow* menuWnd);

// <LegoRR.exe @00416080>
//#define Front_LevelSelect_PlayLevelNameSFX ((bool32 (__cdecl* )(sint32 levelNumber))0x00416080)
bool32 __cdecl Front_LevelSelect_PlayLevelNameSFX(sint32 levelNumber);

// <LegoRR.exe @004160d0>
//#define Front_LevelSelect_PlayTutoLevelNameSFX ((bool32 (__cdecl* )(sint32 levelNumber))0x004160d0)
bool32 __cdecl Front_LevelSelect_PlayTutoLevelNameSFX(sint32 levelNumber);

// Load MenuSets and LevelSets
// <LegoRR.exe @00416120>
#define Front_Initialise ((void (__cdecl* )(const Gods98::Config* config))0x00416120)
//void __cdecl Front_Initialise(const Gods98::Config* config);

// <LegoRR.exe @00416840>
//#define Front_SaveOptionParameters ((void (__cdecl* )(void))0x00416840)
void __cdecl Front_SaveOptionParameters(void);

// <LegoRR.exe @00416870>
//#define Front_LoadOptionParameters ((void (__cdecl* )(bool32 loadOptions, bool32 resetFront))0x00416870)
void __cdecl Front_LoadOptionParameters(bool32 loadOptions, bool32 resetFront);

// <LegoRR.exe @004168f0>
#define Front_PrepareScreenMenuType ((void (__cdecl* )(Menu_ScreenType screenType))0x004168f0)
//void __cdecl Front_PrepareScreenMenuType(Menu_ScreenType screenType);

// <LegoRR.exe @00416bb0>
//#define Front_RunScreenMenuType ((void (__cdecl* )(Menu_ScreenType screenType))0x00416bb0)
bool32 __cdecl Front_RunScreenMenuType(Menu_ScreenType screenType);

// <LegoRR.exe @00416c30>
//#define Front_IsFrontEndEnabled ((bool32 (__cdecl* )(void))0x00416c30)
bool32 __cdecl Front_IsFrontEndEnabled(void);

// <LegoRR.exe @00416c80>
//#define Front_IsMissionSelected ((bool32 (__cdecl* )(void))0x00416c80)
bool32 __cdecl Front_IsMissionSelected(void);

// <LegoRR.exe @00416c90>
//#define Front_IsTutorialSelected ((bool32 (__cdecl* )(void))0x00416c90)
bool32 __cdecl Front_IsTutorialSelected(void);

// <LegoRR.exe @00416ca0>
//#define Front_GetSelectedLevel ((const char* (__cdecl* )(void))0x00416ca0)
const char* __cdecl Front_GetSelectedLevel(void);

// <LegoRR.exe @00416d00>
//#define Front_IsTriggerAppQuit ((bool32 (__cdecl* )(void))0x00416d00)
bool32 __cdecl Front_IsTriggerAppQuit(void);

// <LegoRR.exe @00416d10>
//#define Front_IsTriggerMissionQuit ((bool32 (__cdecl* )(void))0x00416d10)
bool32 __cdecl Front_IsTriggerMissionQuit(void);

// <LegoRR.exe @00416d20>
//#define Front_IsTriggerMissionRestart ((bool32 (__cdecl* )(void))0x00416d20)
bool32 __cdecl Front_IsTriggerMissionRestart(void);

// <LegoRR.exe @00416d30>
LevelSet* __cdecl Front_Levels_GetTutoOrMissions(void);

// <LegoRR.exe @00416d50>
sint32 __cdecl Front_LevelSet_IndexOf(LevelSet* levelSet, const char* levelName);

// <LegoRR.exe @00416da0>
void __cdecl Front_Levels_ResetVisited(void);

// <LegoRR.exe @00416e00>
void __cdecl Front_LevelSet_SetLinkVisited(LevelSet* levelSet, const char* levelName, bool32 visited);

// <LegoRR.exe @00416e70>
bool32 __cdecl Front_LevelSet_IsLinkVisited(LevelSet* levelSet, const char* levelName);

// <LegoRR.exe @00416ee0>
void __cdecl Front_LevelSet_SetLevelLink(LevelSet* levelSet, const char* levelName, LevelLink* link);

// <LegoRR.exe @00416f50>
LevelLink* __cdecl Front_LevelSet_GetLevelLink(LevelSet* levelSet, const char* levelName);


// <LegoRR.exe @00416fc0>
LevelLink* __cdecl Front_LevelSet_LoadLevelLinks(LevelSet* levelSet, OPTIONAL const char* levelName);

// <LegoRR.exe @004170f0>
bool32 __cdecl Front_LevelLink_RunThroughLinks(LevelLink* startLink, LevelLink_RunThroughLinksCallback callback, void* data);

// DATA: sint32* pCount;
// <LegoRR.exe @00417170>
bool32 __cdecl Front_LevelLink_Callback_IncCount(LevelLink* link, void* data);

// DATA: SearchLevelIdentifier_10* search;
// <LegoRR.exe @00417180>
bool32 __cdecl Front_LevelLink_Callback_FindByLinkIndex(LevelLink* link, void* data);

// <LegoRR.exe @004171b0>
LevelLink* __cdecl Front_LevelLink_FindByLinkIndex(LevelLink* startLink, sint32 linkIndex);

// <LegoRR.exe @00417200>
sint32 __cdecl Front_LevelLink_FindSetIndexOf(LevelLink* startLink, sint32 linkIndex);

// <LegoRR.exe @00417220>
void __cdecl Front_Levels_UpdateAvailable_Recursive(LevelLink* link, SearchLevelSelectInfo_14* search, bool32 unlocked);

// When opt_saveReward is null, keepLocked states if all appropriate levels will be set to their default locked state if true, or unlocked if false.
// <LegoRR.exe @00417310>
void __cdecl Front_Levels_UpdateAvailable(LevelLink* startLink, OPTIONAL SaveReward* saveReward, LevelSet* levelSet, MenuItem_SelectData* selectData, bool32 keepLocked);

// <LegoRR.exe @00417360>
sint32 __cdecl Front_Save_GetLevelScore(uint32 index, const SaveData* saveData);

// <LegoRR.exe @00417390>
void __cdecl Front_Callback_SelectMissionItem(real32 elapsedAbs, sint32 selectIndex);

// <LegoRR.exe @00417630>
#define Front_Callback_SelectTutorialItem ((void (__cdecl* )(real32 elapsedAbs, sint32 selectIndex))0x00417630)
//void __cdecl Front_Callback_SelectTutorialItem(real32 elapsedAbs, sint32 selectIndex);

// DATA: SearchLevelSelectAdd* search;
// <LegoRR.exe @00417710>
bool32 __cdecl Front_LevelInfo_Callback_AddItem(LevelLink* link, void* data);

// <LegoRR.exe @004178e0>
#define MainMenuFull_AddMissionsDisplay ((void (__cdecl* )(sint32 valueOffset, LevelLink* startLink, LevelSet* levelSet, Menu* menu, SaveData* saveData, OPTIONAL Menu* menu58, void* callback))0x004178e0)
//void __cdecl MainMenuFull_AddMissionsDisplay(sint32 valueOffset, LevelLink* startLink, LevelSet* levelSet, Menu* menu, SaveData* saveData,
//											 OPTIONAL Menu* menu58, void* callback);

// <LegoRR.exe @004179c0>
#define Front_Save_ReadSaveFile ((bool32 (__cdecl* )(uint32 saveIndex, OUT SaveData* saveData, bool32 readOnly))0x004179c0)
//bool32 __cdecl Front_Save_ReadSaveFile(uint32 saveIndex, OUT SaveData* saveData, bool32 readOnly);

// <LegoRR.exe @00417b00>
#define Front_Save_WriteSaveFiles ((bool32 (__cdecl* )(uint32 saveNumber, OPTIONAL SaveData* saveData))0x00417b00)
//bool32 __cdecl Front_Save_WriteSaveFiles(uint32 saveNumber, OPTIONAL const SaveData* saveData);

// <LegoRR.exe @00417d20>
#define Front_Save_LoadAllSaveFiles ((void (__cdecl* )(void))0x00417d20)
//void __cdecl Front_Save_LoadAllSaveFiles(void);

// <LegoRR.exe @00417d80>
SaveData* __cdecl Front_Save_GetSaveDataAt(sint32 saveIndex);

// <LegoRR.exe @00417da0>
SaveData* __cdecl Front_Save_GetCurrentSaveData(void);

// <LegoRR.exe @00417dc0>
sint32 __cdecl Front_Save_GetSaveNumber(void);

// <LegoRR.exe @00417dd0>
void __cdecl Front_Save_SetSaveNumber(sint32 saveNumber);

// <LegoRR.exe @00417de0>
#define Front_Save_SetLevelCompleted ((void (__cdecl* )(uint32 levelIndex))0x00417de0)
//void __cdecl Front_Save_SetLevelCompleted(uint32 levelIndex);

// <LegoRR.exe @00417e50>
#define Front_Save_SetSaveStruct18 ((void (__cdecl* )(SaveStruct_18* savestruct18))0x00417e50)
//void __cdecl Front_Save_SetSaveStruct18(const SaveStruct_18* savestruct18);

// <LegoRR.exe @00417e70>
#define Front_Save_SetRewardLevel ((bool32 (__cdecl* )(sint32 levelIndex, RewardLevel* rewardLevel))0x00417e70)
//bool32 __cdecl Front_Save_SetRewardLevel(sint32 levelIndex, const RewardLevel* rewardLevel);

// <LegoRR.exe @00417ec0>
RewardLevel* __cdecl Front_Save_GetRewardLevel(sint32 levelIndex);

// <LegoRR.exe @00417ef0>
#define Front_Save_WriteCurrentSaveFiles ((bool32 (__cdecl* )(void))0x00417ef0)
//bool32 __cdecl Front_Save_WriteCurrentSaveFiles(void);

// <LegoRR.exe @00417f10>
bool32 __cdecl Front_Save_GetBool_540(void);

// <LegoRR.exe @00417f20>
void __cdecl Front_Save_SetBool_540(bool32 state);

// <LegoRR.exe @00417f30>
#define Front_Save_Write_FUN_00417f30 ((void (__cdecl* )(void))0x00417f30)
//void __cdecl Front_Save_Write_FUN_00417f30(void);

// <LegoRR.exe @00417f70>
#define Front_Save_CopySaveData ((void (__cdecl* )(SaveData* out_saveData))0x00417f70)
//void __cdecl Front_Save_CopySaveData(OUT SaveData* saveData);

// <LegoRR.exe @00417ff0>
#define Front_Save_SetSaveData ((void (__cdecl* )(SaveData* saveData))0x00417ff0)
//void __cdecl Front_Save_SetSaveData(const SaveData* saveData);

// <LegoRR.exe @00418040>
void __cdecl Front_Save_SetBool_85c(bool32 state);

// <LegoRR.exe @00418050>
#define Front_Save_FUN_00418050 ((bool32 (__cdecl* )(void))0x00418050)
//bool32 __cdecl Front_Save_FUN_00418050(void);

#pragma endregion

}
