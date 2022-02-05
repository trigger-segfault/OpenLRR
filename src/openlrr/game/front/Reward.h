// Reward.h : 
//

#pragma once

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
	/*0000,4*/	bool32 display; // (cfg: Display, default: false) Is the rewards screen shown on mission end?
	/*0004,4*/	bool32 centerText; // (cfg: CentreText, default: false)
	/*0008,4*/	undefined4 reserved1;
	/*000c,4*/	real32 timer; // (cfg: Timer, default: 76.9375f)
	/*0010,4*/	RewardLevel* base; // base reward state template
	/*0014,4*/	RewardLevel* level; // level requirements reward state
	/*0018,4*/	Gods98::Image* wallpaper; // (cfg: Wallpaper)
	/*001c,318c*/	RewardLevel current; // current reward state, modified during gameplay
	/*31a8,4*/	real32 scrollSpeed; // (cfg: ScrollSpeed)
	/*31ac,4*/	real32 vertSpacing; // (cfg: VertSpacing)
	/*31b0,c*/	undefined4 reserved2[3];
	/*31bc,4*/	char* fontName; // (cfg: Font)
	/*31c0,4*/	Gods98::Font* font; // (cfg: Font)
	/*31c4,4*/	char* titleFontName; // (cfg: TitleFont)
	/*31c8,4*/	Gods98::Font* titleFont; // (cfg: TitleFont)
	/*31cc,4*/	char* backFontName; // (cfg: BackFont)
	/*31d0,4*/	Gods98::Font* backFont; // (cfg: BackFont)
	/*31d4,4*/	Gods98::Config* config;
	/*31d8,4*/	const char* gameName;
	/*31dc,4*/	RewardScroll* scroll;
	/*31e0,8*/	Point2F saveButtonPosition; // (cfg: SaveButton[4,5])
	/*31e8,4*/	Gods98::Image* saveButton; // (cfg: SaveButton[0])
	/*31ec,4*/	Gods98::Image* saveButton_hi; // (cfg: SaveButton[1])
	/*31f0,4*/	Gods98::Image* saveButton_in; // (cfg: SaveButton[2])
	/*31f4,4*/	Gods98::Image* saveButton_dim; // (cfg: SaveButton[3])
	/*31f8,8*/	Point2F advanceButtonPosition; // (cfg: AdvanceButton[4,5])
	/*3200,4*/	Gods98::Image* advanceButton; // (cfg: AdvanceButton[0])
	/*3204,4*/	Gods98::Image* advanceButton_hi; // (cfg: AdvanceButton[1])
	/*3208,4*/	Gods98::Image* advanceButton_in; // (cfg: AdvanceButton[2])
	/*320c,4*/	Gods98::Image* advanceButton_dim; // (cfg: AdvanceButton[3])
	/*3210,4*/	char* completeText; // (cfg: CompleteText)
	/*3214,4*/	char* failedText; // (cfg: FailedText)
	/*3218,4*/	char* quitText; // (cfg: QuitText)
	/*321c,8*/	Point2I textPos; // (cfg: TextPos)
	/*3224,20*/	char statusMessage[32];
	/*3244,4*/	bool32 displayText; // (cfg: DisplayText, default: true)
	/*3248,4*/	bool32 displayImages; // (cfg: DisplayImages, default: true)
	/*324c,4*/	bool32 displayFlics; // (cfg: DisplayFlics, default: true)
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
 ******** Macros
 **********************************************************************************/

#pragma region Macros

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @0045f2f0>
#define Reward_Initialise ((bool32 (__cdecl* )(void))0x0045f2f0)

// <LegoRR.exe @0045f4b0>
#define Reward_LoadItemSounds ((void (__cdecl* )(RewardLevelItem* rewardItem))0x0045f4b0)

// <LegoRR.exe @0045f4f0>
#define Reward_LoadItemFonts ((void (__cdecl* )(RewardLevelItem* rewardItem))0x0045f4f0)

// <LegoRR.exe @0045f550>
#define Reward_LoadItemBoxImages ((void (__cdecl* )(RewardLevelItem* rewardItem))0x0045f550)

// <LegoRR.exe @0045f6a0>
#define Reward_LoadItemImages ((void (__cdecl* )(RewardLevelItem* rewardItem))0x0045f6a0)

// <LegoRR.exe @0045f7f0>
#define Reward_LoadItemText ((void (__cdecl* )(RewardLevelItem* rewardItem))0x0045f7f0)

// <LegoRR.exe @0045f8b0>
#define Reward_LoadItemFlics ((void (__cdecl* )(RewardLevelItem* rewardItem))0x0045f8b0)

// <LegoRR.exe @0045fa10>
#define Reward_Shutdown ((void (__cdecl* )(void))0x0045fa10)

// <LegoRR.exe @0045fa70>
#define Reward_FreeItemImage ((void (__cdecl* )(RewardLevelItem* rewardItem))0x0045fa70)

// <LegoRR.exe @0045fa90>
#define Reward_FreeItemFont ((void (__cdecl* )(RewardLevelItem* rewardItem))0x0045fa90)

// <LegoRR.exe @0045fab0>
#define Reward_LoadGraphics ((bool32 (__cdecl* )(const Gods98::Config* config, const char* gameName))0x0045fab0)

// <LegoRR.exe @0045fdb0>
#define Reward_LoadButtons ((bool32 (__cdecl* )(const Gods98::Config* config, const char* gameName))0x0045fdb0)

// <LegoRR.exe @00460060>
#define Reward_CreateLevel ((bool32 (__cdecl* )(void))0x00460060)

// <LegoRR.exe @00460360>
#define Reward_LoadLevelItemImportance ((void (__cdecl* )(RewardLevelItem* rewardItem))0x00460360)

// <LegoRR.exe @00460400>
#define Reward_LoadLevelItemQuota ((void (__cdecl* )(RewardLevelItem* rewardItem))0x00460400)

// <LegoRR.exe @004604a0>
#define Reward_FreeLevel ((void (__cdecl* )(void))0x004604a0)

// <LegoRR.exe @004604b0>
#define GetRewardLevel2 ((RewardLevel* (__cdecl* )(undefined4 unused_rewardID))0x004604b0)

// <LegoRR.exe @004604c0>
#define GetRewardLevel ((RewardLevel* (__cdecl* )(void))0x004604c0)

// <LegoRR.exe @004604e0>
#define RewardQuota_CountUnits ((void (__cdecl* )(void))0x004604e0)

// <LegoRR.exe @00460550>
#define RewardQuota_CountBuildings ((RewardBuildingCounts* (__cdecl* )(OUT RewardBuildingCounts* buildingCounts))0x00460550)

// DATA: RewardBuildingCounts*
// <LegoRR.exe @004605d0>
#define RewardQuota_LiveObjectCallback_CountBuildings ((bool32 (__cdecl* )(LegoObject* liveObj, void* search))0x004605d0)

// <LegoRR.exe @00460620>
#define Reward_Prepare ((bool32 (__cdecl* )(void))0x00460620)

// <LegoRR.exe @00460bd0>
#define Reward_GetLevelObjectsBuilt ((uint32 (__cdecl* )(const char* objName, sint32 objLevel, bool32 currentLevel))0x00460bd0)

// <LegoRR.exe @00460c10>
#define Reward_PrepareCalculate ((bool32 (__cdecl* )(void))0x00460c10)

// <LegoRR.exe @004611c0>
#define Reward_PrepareScroll ((bool32 (__cdecl* )(void))0x004611c0)

// <LegoRR.exe @00461330>
#define Reward_PrepareValueText ((bool32 (__cdecl* )(void))0x00461330)

// <LegoRR.exe @004616d0>
#define Reward_Show ((bool32 (__cdecl* )(void))0x004616d0)

// <LegoRR.exe @00461a50>
#define Reward_DrawItem ((void (__cdecl* )(RewardLevelItem* rewardItem, RewardItemFlags flags, Reward_Type rewardType))0x00461a50)

// <LegoRR.exe @00461f50>
#define Reward_DrawScore ((void (__cdecl* )(Reward_Type rewardType))0x00461f50)

// <LegoRR.exe @00462090>
#define Reward_HandleButtons ((void (__cdecl* )(OUT RewardUIState* state, OUT bool32* saved))0x00462090)

// <LegoRR.exe @004622f0>
#define Reward_GotoSaveMenu ((void (__cdecl* )(void))0x004622f0)

// <LegoRR.exe @00462530>
#define Reward_GotoAdvance ((void (__cdecl* )(void))0x00462530)

// <LegoRR.exe @00462650>
#define Reward_HandleDebugKeys ((void (__cdecl* )(IN OUT Reward_Type* rewardType, IN OUT RewardUIState* state, IN OUT bool32* finished, IN OUT real32* timer))0x00462650)

// <LegoRR.exe @00462720>
#define Reward_PlayFlic ((void (__cdecl* )(RewardLevelItem* rewardItem))0x00462720)

// <LegoRR.exe @00462760>
#define Reward_UpdateState ((void (__cdecl* )(IN OUT Reward_Type* rewardType, IN OUT RewardUIState* state, IN OUT real32* timer))0x00462760)

// <LegoRR.exe @004628c0>
#define Reward_DrawAllValues ((void (__cdecl* )(IN Reward_Type* rewardType, OUT bool32* finished))0x004628c0)

// <LegoRR.exe @004629c0>
#define Reward_LoopUpdate ((void (__cdecl* )(real32 elapsed))0x004629c0)

// <LegoRR.exe @00462a40>
#define Reward_LoopBegin ((bool32 (__cdecl* )(void))0x00462a40)

// <LegoRR.exe @00462ac0>
#define RewardQuota_UpdateTimers ((void (__cdecl* )(real32 elapsedGame))0x00462ac0)

// <LegoRR.exe @00462af0>
#define RewardQuota_WallDestroyed ((void (__cdecl* )(void))0x00462af0)

// <LegoRR.exe @00462b10>
#define RewardQuota_CavernDiscovered ((void (__cdecl* )(void))0x00462b10)

// <LegoRR.exe @00462b30>
#define RewardQuota_RockMonsterGenerated ((void (__cdecl* )(void))0x00462b30)

// <LegoRR.exe @00462b40>
#define RewardQuota_RockMonsterDestroyed ((void (__cdecl* )(void))0x00462b40)

// <LegoRR.exe @00462b50>
#define RewardQuota_RockMonsterAttacked ((void (__cdecl* )(void))0x00462b50)

// <LegoRR.exe @00462b60>
#define RewardQuota_RockMonsterDamageDealt ((void (__cdecl* )(real32 damage))0x00462b60)

// <LegoRR.exe @00462b80>
#define RewardQuota_MiniFigureDamageTaken ((void (__cdecl* )(real32 damage))0x00462b80)

// <LegoRR.exe @00462ba0>
#define RewardScroll_Create ((RewardScroll* (__cdecl* )(OUT RewardScroll** scroll, real32 zero, real32 heightDiv20, real32 width, real32 bottomSubDiv3pt5, real32 scrollSpeed))0x00462ba0)

// <LegoRR.exe @00462c20>
#define RewardScroll_Free ((bool32 (__cdecl* )(IN OUT RewardScroll** scroll))0x00462c20)

// <LegoRR.exe @00462c90>
#define RewardScroll_AddLabel ((RewardScrollLabel* (__cdecl* )(RewardScroll* scroll, const char* text, Gods98::Font* font, real32 xPos, real32 yPos, RewardScrollLabelFlags labelFlags))0x00462c90)

// <LegoRR.exe @00462d70>
#define RewardScroll_SetDelay_Unk ((void (__cdecl* )(RewardScroll* scroll, real32 curScrollY))0x00462d70)

// <LegoRR.exe @00462d80>
#define RewardScroll_AddFlags ((void (__cdecl* )(RewardScroll* scroll, RewardScrollFlags flags))0x00462d80)

// <LegoRR.exe @00462d90>
#define RewardScroll_DrawLabels ((bool32 (__cdecl* )(RewardScroll* scroll))0x00462d90)

#pragma endregion

}
