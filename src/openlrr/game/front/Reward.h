// Reward.h : 
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

enum RewardUIState : uint32 // [LegoRR/Rewards.c|enum:0x4|type:uint]
{
	REWARDSTATE_0      = 0,
	REWARDSTATE_1      = 1,
	REWARDSTATE_2      = 2,
	REWARDSTATE_3      = 3,
	REWARDSTATE__COUNT = 4,
};
assert_sizeof(RewardUIState, 0x4);


enum RewardScrollLabelMode : uint32 // [LegoRR/Rewards.c|enum:0x4|type:uint] Inclusive mode, None does nothing, WINDOW also performs IMAGE and TEXT, IMAGE also performs TEXT, TEXT does only that
{
	REWARDSCROLL_MODE_NONE   = 0,
	REWARDSCROLL_MODE_WINDOW = 1,
	REWARDSCROLL_MODE_IMAGE  = 2,
	REWARDSCROLL_MODE_TEXT   = 3,
};
assert_sizeof(RewardScrollLabelMode, 0x4);


enum RewardItemFlags : uint32 // [LegoRR/Rewards.c|flags:0x4|type:uint]
{
	REWARDITEM_FLAG_NONE               = 0,
	REWARDITEM_FLAG_VALUETEXT          = 0x1,
	REWARDITEM_FLAG_IMAGES             = 0x2,
	REWARDITEM_FLAG_FLIC               = 0x4,
	REWARDITEM_FLAG_NAMETEXT           = 0x10,
	REWARDITEM_FLAG_NAMETEXT_HALFTRANS = 0x20,
	REWARDITEM_FLAG_BOXIMAGES          = 0x40,
};
flags_end(RewardItemFlags, 0x4);


enum RewardScrollFlags : uint32 // [LegoRR/Rewards.c|flags:0x4|type:uint]
{
	REWARDSCROLL_NONE  = 0,
	REWARDSCROLL_UNK_1 = 0x1,
	REWARDSCROLL_UNK_2 = 0x2,
};
flags_end(RewardScrollFlags, 0x4);


enum RewardScrollLabelFlags : uint32 // [LegoRR/Rewards.c|flags:0x4|type:uint]
{
	REWARDSCROLL_LABEL_NONE     = 0,
	REWARDSCROLL_LABEL_CENTERED = 0x1,
	REWARDSCROLL_LABEL_NOSCROLL = 0x2,
};
flags_end(RewardScrollLabelFlags, 0x4);

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

struct RewardScrollLabel // [LegoRR/Rewards.c|struct:0x20]
{
	/*00,4*/	RewardScrollLabelMode mode; // (0=none, 1=txtWnd+, 2=image+, 3=font+)
	/*04,4*/	real32 xPos;
	/*08,4*/	real32 yPos; // (vertSpacing?)
	/*0c,4*/	Gods98::TextWindow* textWnd;
	/*10,4*/	Gods98::Image* image;
	/*14,4*/	char* text;
	/*18,4*/	Gods98::Font* font;
	/*1c,4*/	RewardScrollLabelFlags flags; // ( 1 = centered??, 2 = ? )
	/*20*/
};
assert_sizeof(RewardScrollLabel, 0x20);


struct RewardScroll // [LegoRR/Rewards.c|struct:0x28]
{
	/*00,4*/	real32 yInitial; // (init: appHeight() / 20.0f)
	/*04,4*/	real32 xPos; // (init: 0.0f)
	/*08,4*/	real32 yPos; // (init: appHeight() / 20.0f) scrollCountdown??
	/*0c,4*/	real32 width; // (init: appWidth())
	/*10,4*/	real32 height; // (init: appHeight() - appHeight() / 3.5f)
	/*14,4*/	real32 scrollSpeed;
	/*18,4*/	real32 curScrollY; // (init: set to 400.0 after Create function)
	/*1c,4*/	uint32 labelCount;
	/*20,4*/	RewardScrollLabel** labels;
	/*24,4*/	RewardScrollFlags flags;
	/*28*/
};
assert_sizeof(RewardScroll, 0x28);


struct RewardBuildingCounts // [LegoRR/Rewards.c|struct:0x198]
{
	/*000,190*/	char* nameTable[100];
	/*190,4*/	uint32 count;
	/*194,4*/	uint32 numPrevLevels_unk;
	/*198*/
};
assert_sizeof(RewardBuildingCounts, 0x198);


struct RewardLevelItem // [LegoRR/Rewards.c|struct:0x210]
{
	/*000,4*/	RewardItemFlags flags; // (0x4 = flic)
	/*004,20*/	char name[32];
	/*024,4*/	real32 Importance;
	/*028,4*/	sint32 Quota;
	/*02c,4*/	undefined field_0x2c_0x2f[4];
	/*030,4*/	real32 countdownRatio; // Elapsed / Countdown Timers
	/*034,4*/	real32 countdown; // Countdown timer
	/*038,4*/	real32 percentFloat;
	/*03c,4*/	uint32 numGenerated;
	/*040,4*/	uint32 numDestroyed;
	/*044,4*/	uint32 numAttacked; // For every weapon attack
	/*048,4*/	real32 damageTaken;
	/*04c,4*/	undefined field_0x4c_0x4f[4];
	/*050,4*/	sint32 percentInt;
	/*054,80*/	char Text[128]; // (TODO: size me)
	/*0d4,80*/	char valueText[128];
	/*154,8*/	Point2I TextPosition;
	/*15c,8*/	Point2I ImagePosition;
	/*164,8*/	Point2I BoxImagePosition;
	/*16c,80*/	char FlicName[128]; // (TODO: size me)
	/*1ec,8*/	Size2I FlicSize;
	/*1f4,8*/	Point2I FlicPosition;
	/*1fc,4*/	Gods98::Flic* Flic;
	/*200,4*/	Gods98::Image* Image;
	/*204,4*/	Gods98::Font* Font;
	/*208,4*/	char* SoundName;
	/*20c,4*/	Gods98::Image* BoxImage;
	/*210*/
};
assert_sizeof(RewardLevelItem, 0x210);


struct RewardLevel // [LegoRR/Rewards.c|struct:0x318c]
{
	/*0000,4*/	bool32 Enabled;
	/*0004,4*/	bool32 saveHasCapture;
	/*0008,20*/	Gods98::Image saveCaptureImage;
	/*0028,198*/	RewardBuildingCounts buildingCounts;
	/*01c0,4*/	sint32 Modifier;
	/*01c4,4*/	undefined field_0x1c4_0x1c7[4];
	/*01c8,4*/	undefined4 field_1c8;
	/*01cc,4*/	uint32 uintConstructCount_1cc;
	/*01d0,4*/	uint32 uintConstruct_1d0;
	/*01d4,1900*/	char buildingNameBuffers[100][64];
	/*1ad4,30*/	undefined field_0x1ad4_0x1b03[48];
	/*1b04,4*/	real32 float_1b04;
	/*1b08,4*/	real32 float_1b08;
	/*1b0c,4*/	real32 float_1b0c;
	/*1b10,1d4*/	undefined field_0x1b10_0x1ce3[468];
	/*1ce4,4*/	sint32 itemCount; // (init: 10)
	/*1ce8,4*/	RewardLevelItem* itemPtr; // (init: &items_TABLE)
	/*1cec,14a0*/	RewardLevelItem items[10];
	/*318c*/
};
assert_sizeof(RewardLevel, 0x318c);


struct Reward_Globs // [LegoRR/Rewards.c|struct:0x3250|tags:GLOBS]
{
	/*0000,4*/	bool32 Display; // is the rewards screen shown on mission end?
	/*0004,4*/	bool32 CenterText;
	/*0008,4*/	undefined4 reserved1;
	/*000c,4*/	real32 Timer;
	/*0010,4*/	RewardLevel* base; // base reward state template
	/*0014,4*/	RewardLevel* level; // level requirements reward state
	/*0018,4*/	Gods98::Image* Wallpaper;
	/*001c,318c*/	RewardLevel current; // current reward state, modified during gameplay
	/*31a8,4*/	real32 ScrollSpeed;
	/*31ac,4*/	real32 VertSpacing;
	/*31b0,c*/	undefined4 padding2[3];
	/*31bc,4*/	char* FontName;
	/*31c0,4*/	Gods98::Font* Font;
	/*31c4,4*/	char* TitleFontName;
	/*31c8,4*/	Gods98::Font* TitleFont;
	/*31cc,4*/	char* BackFontName;
	/*31d0,4*/	Gods98::Font* BackFont;
	/*31d4,4*/	Gods98::Config* config;
	/*31d8,4*/	char* gameName;
	/*31dc,4*/	RewardScroll* scroll;
	/*31e0,8*/	Point2F SaveButtonPosition;
	/*31e8,4*/	Gods98::Image* SaveButton;
	/*31ec,4*/	Gods98::Image* SaveButton_hi;
	/*31f0,4*/	Gods98::Image* SaveButton_in;
	/*31f4,4*/	Gods98::Image* SaveButton_dim;
	/*31f8,8*/	Point2F AdvanceButtonPosition;
	/*3200,4*/	Gods98::Image* AdvanceButton;
	/*3204,4*/	Gods98::Image* AdvanceButton_hi;
	/*3208,4*/	Gods98::Image* AdvanceButton_in;
	/*320c,4*/	Gods98::Image* AdvanceButton_dim;
	/*3210,4*/	char* CompleteText;
	/*3214,4*/	char* FailedText;
	/*3218,4*/	char* QuitText;
	/*321c,8*/	Point2I TextPos;
	/*3224,20*/	char StatusMessage[32];
	/*3244,4*/	bool32 DisplayText;
	/*3248,4*/	bool32 DisplayImages;
	/*324c,4*/	bool32 DisplayFlics;
	/*3250*/
};
assert_sizeof(Reward_Globs, 0x3250);

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @00553980>
extern Reward_Globs & rewardGlobs;

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions


#pragma endregion

}
