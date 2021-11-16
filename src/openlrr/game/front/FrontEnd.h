// FrontEnd.h : 
//

#pragma once

#include "../../common.h"
#include "../../engine/colour.h"
#include "../../engine/geometry.h"
#include "../../engine/undefined.h"

#include "../../engine/gfx/Containers.h"
#include "../../engine/drawing/Flic.h"
#include "../../engine/drawing/Fonts.h"
#include "../../engine/drawing/Images.h"
#include "../../engine/drawing/TextWindow.h"

#include "../../engine/video/Movie.h"

#include "../GameCommon.h"

#include "../audio/SFX.h"

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

#pragma endregion

/**********************************************************************************
 ******** Function Typedefs
 **********************************************************************************/

#pragma region Function Typedefs

typedef void (__cdecl* MenuItemCycleCallback)(sint32);
typedef void (__cdecl* MenuItemRealSliderCallback)(real32);
typedef void (__cdecl* MenuItemSliderCallback)(sint32);
typedef void (__cdecl* MenuItemTriggerCallback)(void);

#pragma endregion

/**********************************************************************************
 ******** Enumerations
 **********************************************************************************/

#pragma region Enums

enum MenuItem_Type : sint32 // [LegoRR/FrontEnd.c|enum:0x4|type:int]
{
	MenuItem_Type_Invalid = -1,

	MenuItem_Cycle        = 0,
	MenuItem_Trigger      = 1,
	MenuItem_TextInput    = 2,
	MenuItem_Slider       = 3,
	MenuItem_RealSlider   = 4,
	MenuItem_Next         = 5,
	MenuItem_LevelSelect  = 6,

	MenuItem_Type_Count   = 7,
};
assert_sizeof(MenuItem_Type, 0x4);


enum Front_RockWipeFlags : uint8 // [LegoRR/Front.c|flags:0x1|type:byte]
{
	ROCKWIPE_FLAG_NONE  = 0,
	ROCKWIPE_FLAG_UNK_1 = 0x1,
	ROCKWIPE_FLAG_UNK_2 = 0x2,
};
flags_end(Front_RockWipeFlags, 0x1);


enum SaveRewardFlags : uint32
{
	SAVEREWARD_NONE  = 0,
	SAVEREWARD_UNK_1 = 1,
	SAVEREWARD_UNK_2 = 2,
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


struct SaveStruct_18 // [LegoRR/save.c|struct:0x18]
{
	/*00,18*/	undefined field_0x0_0x17[24];
	/*18*/
};
assert_sizeof(SaveStruct_18, 0x18);


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


struct LevelInfo // [LegoRR/FrontEnd.c|struct:0x14]
{
	/*00,4*/	sint32 index; // Index in LevelCollection
	/*04,4*/	LevelInfo** linkList;
	/*08,4*/	uint32 linkCount;
	/*0c,4*/	undefined4 field_c;
	/*10,4*/	undefined4 field_10; // (is locked/unlocked?)
	/*14*/
};
assert_sizeof(LevelInfo, 0x14);


struct LevelSet // [LegoRR/FrontEnd.c|struct:0x14]
{
	/*00,4*/	sint32 count;
	/*04,4*/	char** levelNames;
	/*08,4*/	char** fullNames;
	/*0c,4*/	LevelInfo** levelList;
	/*10,4*/	bool32* isLinked; // True if this level has been loaded from level links (probably makes them require unlock)
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
	/*04,4*/	sint32 overlayType; // (bmp, avi, lws, flh?)
	/*08,4*/	undefined4 field_8;
	/*0c,8*/	Point2I position;
	/*14,4*/	SFX_Type sfxType;
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
	/*0c,4*/	sint32* value;
	/*10,4*/	sint32 x2; // (cfg: Cycle:[3])
	/*14,4*/	sint32 y2; // (cfg: Cycle:[4])
	/*18,4*/	MenuItemCycleCallback callback;
	/*1c*/
};
assert_sizeof(MenuItem_CycleData, 0x1c);


struct MenuItem_RealSliderData // [LegoRR/FrontEnd.c|struct:0x1c]
{
	/*00,4*/	real32* value;
	/*04,4*/	real32 valueMin; // (cfg: RealSlider:[6])
	/*08,4*/	real32 valueMax; // (cfg: RealSlider:[7])
	/*0c,4*/	real32 valueStep; // (cfg: RealSlider:[8])
	/*10,4*/	sint32 x2; // (cfg: RealSlider:[3])
	/*14,4*/	sint32 y2; // (cfg: RealSlider:[4])
	/*18,4*/	MenuItemRealSliderCallback callback;
	/*1c*/
};
assert_sizeof(MenuItem_RealSliderData, 0x1c);


struct MenuItem_SliderData // [LegoRR/FrontEnd.c|struct:0x38]
{
	/*00,4*/	sint32* value;
	/*04,4*/	sint32 valueMin; // (cfg: Slider:[6])
	/*08,4*/	sint32 valueMax; // (cfg: Slider:[7])
	/*0c,4*/	sint32 x2; // (cfg: Slider:[3])
	/*10,4*/	sint32 y2; // (cfg: Slider:[4])
	/*14,4*/	MenuItemSliderCallback callback;
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
	/*0,4*/	sint32* value;
	/*4,4*/	bool32 end; // (cfg: Trigger:[4]) End/close the current MenuSet
	/*8,4*/	MenuItemTriggerCallback callback;
	/*c*/
};
assert_sizeof(MenuItem_TriggerData, 0xc);


struct MenuItem_LevelData // [LegoRR/FrontEnd.c|struct:0x20]
{
	/*00,4*/	uint32 flags; // [0x1 = print name over image, 0x2, 0x4]
	/*04,4*/	char* strImageNames; // memory allocation for cfg: Level::MenuBMP (needed for ImageCache)
	/*08,4*/	Gods98::Image* imageHi; // (cfg: Level::MenuBMP[0])
	/*0c,4*/	Gods98::Image* imageLo; // (cfg: Level::MenuBMP[1])
	/*10,4*/	Gods98::Image* imageLockedOverlay; // (cfg: Level::MenuBMP[2])
	/*14,4*/	sint32 frontEndX; // (cfg: Level::FrontEndX)
	/*18,4*/	sint32 frontEndY; // (cfg: Level::FrontEndY)
	/*1c,4*/	bool32 frontEndOpen; // (cfg: Level::FrontEndOpen)
	/*20*/
};
assert_sizeof(MenuItem_LevelData, 0x20);


struct MenuItem_LevelSelectData // [LegoRR/FrontEnd.c|struct:0x5c]
{
	/*00,4*/	MenuItem_LevelData* levelsList;
	/*04,c*/	sint32* widths[3];
	/*10,c*/	sint32* heights[3];
	/*1c,4*/	sint32 levelCount;
	/*20,4*/	char* string1;
	/*24,4*/	char* string2;
	/*28,4*/	sint32* value;
	/*2c,10*/	Area2I rect1;
	/*3c,10*/	Area2I rect2;
	/*4c,4*/	undefined4 field_4c;
	/*50,4*/	undefined4 field_50;
	/*54,4*/	undefined* callback;
	/*58,4*/	undefined4 field_58;
	/*5c*/
};
assert_sizeof(MenuItem_LevelSelectData, 0x5c);


union MenuItem_Data_union // [LegoRR/FrontEnd.c|union:0x4]
{
	/*0,4*/	MenuItem_CycleData* cycle;
	/*0,4*/	MenuItem_TriggerData* trigger;
	/*0,4*/	MenuItem_SliderData* slider;
	/*0,4*/	MenuItem_RealSliderData* realSlider;
	/*0,4*/	MenuItem_LevelSelectData* levelSelect;
	/*0,4*/	Menu* next;
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
	/*28,4*/	bool32 isImageItem; // (1 = hasImages?)
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
	/*18,4*/	MenuItem* items; // (cfg: Menu::Item1, Item2...)
	/*1c,4*/	sint32 itemCount; // (cfg: Menu::ItemCount) Number of used slots in items
	/*20,4*/	sint32 itemCapacity; // Total number of slots allocated in items
	/*24,4*/	sint32 itemFocus; // Index of item with keyboard focus
	/*28,4*/	bool32 bool_28;
	/*2c,8*/	Point2I position; // (cfg: Menu::Position)
	/*34,4*/	uint32 flags_34; // flags [0x1, 0x2, ...]
	/*38,8*/	Point2F currPosition;
	/*40,4*/	MenuOverlay* overlays; // (cfg: Menu::Overlay1, Overlay2...)
	/*44,4*/	bool32 autoCenter; // (cfg: Menu::AutoCenter)
	/*48,4*/	bool32 displayTitle; // (cfg: Menu::DisplayTitle)
	/*4c,4*/	bool32 anchored; // (cfg: Menu::Anchored)
	/*50,8*/	Point2I anchoredPosition; // (cfg: Menu::Anchored)
	/*58,4*/	sint32 flags_58; // (0x2 = CanScroll)
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
	/*0,4*/	void* value;
	/*4,4*/	void* callback;
	/*8*/
};
assert_sizeof(MenuItemCallbackPair, 0x8);
;

struct Front_Globs // [LegoRR/FrontEnd.c|struct:0x884|tags:GLOBS]
{
	/*000,4*/	MenuSet* pausedMenuSet;
	/*004,4*/	MenuSet* mainMenuSet;
	/*008,4*/	MenuSet* optionsMenuSet;
	/*00c,4*/	MenuSet* saveMenuSet;
	/*010,14*/	LevelSet tutorialLevels;
	/*024,14*/	LevelSet missionLevels;
	/*038,4*/	LevelInfo* startMissionInfo;
	/*03c,4*/	LevelInfo* startTutorialInfo;
	/*040,10*/	undefined4 reserved1[4];
	/*050,4*/	sint32 triggerCredits; // [trigger: 1=play credits]
	/*054,8*/	undefined4 reserved2[2];
	/*05c,4*/	sint32 triggerYesQuit; // [trigger: 1=yes, quit game]
	/*060,4*/	sint32 unusedMissionNumber; // (always -1)
	/*064,4*/	sint32 unusedTutorialNumber; // (always -1)
	/*068,4*/	sint32 currLevelSel; // [levelselect: index=hoverLevel?]
	/*06c,4*/	sint32 triggerContinueMission; // [trigger: 1=close pause menu]
	/*070,4*/	sint32 sliderGameSpeed; // [slider: 0-5]
	/*074,4*/	sint32 sliderSFXVolume; // [slider: 0-10]
	/*078,4*/	sint32 sliderMusicVolume; // [slider: 0-10]
	/*07c,4*/	sint32 sliderBrightness; // [slider: 0-10]
	/*080,4*/	sint32 cycleHelpWindow; // [cycle: Off,On]
	/*084,4*/	sint32 triggerReplayObjective; // [trigger: 1=replay]
	/*088,4*/	sint32 triggerQuitMission; // [trigger: 1=quit level]
	/*08c,4*/	sint32 triggerRestartMission; // [trigger: 1=restart level]
	/*090,4*/	sint32 cycleWallDetail; // [cycle: High,Low]
	/*094,4*/	sint32 cycleMusicOn; // [cycle: On,Off]
	/*098,4*/	sint32 cycleSFXOn; // [cycle: On,Off]
	/*09c,4*/	sint32 cycleAutoGameSpeed; // [cycle: On,Off]
	/*0a0,4*/	sint32 triggerBackSave; // [trigger: 1=leave save menu]
	/*0a4,8*/	Point2I overlayPosition;
	/*0ac,4*/	Gods98::Flic* overlayImageOrFlic; // Image* or Flic* type
	/*0b0,4*/	uint32 overlayStartTime; // starting Main_GetTime (millliseconds)
	/*0b4,4*/	uint32 overlayCurrTime; // current Main_GetTime (millliseconds)
	/*0b8,4*/	Gods98::Font* versionFont; // (file: bmpMbriefFONT2)
	/*0bc,4*/	char* versionString; // (cfg: Main::Version)
	/*0c0,450*/	SaveData saveData[6];
	/*510,4*/	sint32 saveNumber;
	/*514,4*/	char* strDefaultLevelBMPS; // (cfg: Menu::DefaultLevelBMPS) memory storage
	/*518,4*/	undefined4 reserved3;
	/*51c,8*/	Point2F scrollOffset; // Offset position in level select (or anywhere else with a larger menu image)
	/*524,8*/	undefined4 reserved4[2];
	/*52c,4*/	Gods98::Container* rockWipeAnim;
	/*530,1*/	Front_RockWipeFlags rockWipeFlags;
	/*531,3*/	undefined padding1[3];
	/*534,4*/	real32 rockWipeSFXTimer;
	/*538,4*/	real32 rockWipeSFXStartTime;
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
	/*87c,4*/	bool32 levelSelectSFXPlaying;
	/*880,4*/	real32 levelSelectSFXTimer;
	/*884*/
};
assert_sizeof(Front_Globs, 0x884);

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @00557fc0>
extern Front_Globs & frontGlobs;

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @00414c10>
bool32 __cdecl Front_IsIntrosEnabled(void);

/*// <LegoRR.exe @00415080>
void __cdecl Front_UpdateOptionsSliders(void);*/

// Plays an alread-loaded Movie_t from the G98CMovie C wrapper API.
// Allows terminating the movie playback during runtime with isSkippable.
// <LegoRR.exe @00415630>
void __cdecl Front_PlayMovie(Gods98::Movie_t* mov, bool32 skippable);

// <LegoRR.exe @004156f0>
void __cdecl Front_ShowIntroSplash(const char* imageKey, bool32 skippable, const char* timeKey);

// <LegoRR.exe @00415840>
void __cdecl Front_ShowIntroMovie(const char* movieKey, bool32 skippable);

#pragma endregion

}
