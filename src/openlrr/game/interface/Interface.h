// Interface.h : 
//

#pragma once

#include "../../engine/input/Keys.h"

#include "../GameCommon.h"
#include "../object/Object.h"
#include "../object/Stats.h"


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

#define INTERFACE_MENU_MAXICONS		11

#pragma endregion

/**********************************************************************************
 ******** Enumerations
 **********************************************************************************/

#pragma region Enums

enum Interface_MenuItemFlags : uint32 // [LegoRR/Interface.c|flags:0x4|type:uint]
{
	INTERFACE_MENUITEM_FLAG_NONE  = 0,
	INTERFACE_MENUITEM_FLAG_UNK_1 = 0x1,
	INTERFACE_MENUITEM_FLAG_UNK_2 = 0x2,
	INTERFACE_MENUITEM_FLAG_FLASH = 0x4,
	INTERFACE_MENUITEM_FLAG_UNK_8 = 0x8,
};
flags_end(Interface_MenuItemFlags, 0x4);


enum Interface_GlobFlags : uint32 // [LegoRR/Interface.c|flags:0x4|type:uint]
{
	INTERFACE_GLOB_FLAG_NONE     = 0,
	INTERFACE_GLOB_FLAG_UNK_4    = 0x4,
	INTERFACE_GLOB_FLAG_UNK_20   = 0x20,
	INTERFACE_GLOB_FLAG_UNK_40   = 0x40,
	INTERFACE_GLOB_FLAG_UNK_80   = 0x80,
	INTERFACE_GLOB_FLAG_UNK_100  = 0x100,
	INTERFACE_GLOB_FLAG_UNK_200  = 0x200,
	INTERFACE_GLOB_FLAG_UNK_400  = 0x400,
	INTERFACE_GLOB_FLAG_UNK_800  = 0x800,
	INTERFACE_GLOB_FLAG_UNK_1000 = 0x1000,
	INTERFACE_GLOB_FLAG_UNK_2000 = 0x2000,
	INTERFACE_GLOB_FLAG_UNK_4000 = 0x4000,
};
flags_end(Interface_GlobFlags, 0x4);

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

struct Interface_Menu // [LegoRR/Interface.c|struct:0x8]
{
	/*0,4*/	Interface_MenuItemType* iconList;
	/*4,4*/	uint32 iconCount;
	/*8*/
};
assert_sizeof(Interface_Menu, 0x8);


struct Interface_Globs // [LegoRR/Interface.c|struct:0x14ac|tags:GLOBS]
{
	/*0000,4*/	Gods98::Font* font; // PTRImageFont_004ddd58
	/*0004,2c*/	Gods98::Image* iconPanelImages[INTERFACE_MENU_MAXICONS]; // (cfg: InterfaceSurroundImages[0]) g_InterfaceSurroundImages
	/*0030,58*/	Point2F iconPanelIconOffsets[INTERFACE_MENU_MAXICONS]; // (cfg: InterfaceSurroundImages[1,2])
	/*0088,2c*/	Gods98::Image* iconPanelNoBackImages[INTERFACE_MENU_MAXICONS]; // (cfg: InterfaceSurroundImages[5])
	/*00b4,58*/	Point2F iconPanelNoBackIconOffsets[INTERFACE_MENU_MAXICONS]; // (cfg: InterfaceSurroundImages[6,7])
	/*010c,4*/	Gods98::Image* backButtonImage_hl; // (cfg: InterfaceBackButton[2])
	/*0110,4*/	Gods98::Image* backButtonImage_pr; // (cfg: InterfaceBackButton[3])
	/*0114,58*/	Point2F iconPanelBackButtonOffsets[INTERFACE_MENU_MAXICONS]; // (cfg: InterfaceSurroundImages[3,4]) Point2F_ARRAY_004dde6c
	/*016c,8*/	Size2I backButtonSize; // (cfg: InterfaceBackButton[0,1])
	/*0174,4*/	char* backButtonText; // (cfg: InterfaceBackButton[4])
	/*0178,128*/	const char* menuItemName[Interface_MenuItem_Type_Count];
	/*02a0,128*/	Gods98::Image* menuItemIcons[Interface_MenuItem_Type_Count];
	/*03c8,128*/	Gods98::Image* menuItemIcons_no[Interface_MenuItem_Type_Count];
	/*04f0,128*/	Gods98::Image* menuItemIcons_pr[Interface_MenuItem_Type_Count];
	/*0618,128*/	char* langMenuItemTexts[Interface_MenuItem_Type_Count];
	/*0740,128*/	SFX_ID sfxMenuItemTexts[Interface_MenuItem_Type_Count];
	/*0868,4a*/	Gods98::Keys menuItemF2keys[Interface_MenuItem_Type_Count]; // (Keys[])
	/*08b2,2*/	undefined2 padding1;
	/*08b4,128*/	uint32 menuItemFlags[Interface_MenuItem_Type_Count]; // (0x4: flashing)
	/*09dc,4*/	Gods98::Image** vehicleItemIcons;
	/*09e0,4*/	Gods98::Image** buildingItemIcons;
	/*09e4,4*/	Gods98::Image** vehicleItemIcons_no;
	/*09e8,4*/	Gods98::Image** buildingItemIcons_no;
	/*09ec,4*/	Gods98::Image** vehicleItemIcons_pr;
	/*09f0,4*/	Gods98::Image** buildingItemIcons_pr;
	/*09f4,128*/	char* langMenuItemTexts_no[Interface_MenuItem_Type_Count];
	/*0b1c,128*/	SFX_ID sfxMenuItemTexts_no[Interface_MenuItem_Type_Count];
	/*0c44,4*/	Gods98::Keys* vehicleItemF2Keys; // (Keys*)
	/*0c48,4*/	Gods98::Keys* buildingItemF2Keys; // (Keys*)
	/*0c4c,4*/	Interface_MenuItemFlags* vehicleItemFlags;
	/*0c50,4*/	Interface_MenuItemFlags* buildingItemFlags;
	/*0c54,128*/	uint32 menuItemClicks[Interface_MenuItem_Type_Count];
	/*0d7c,4*/	uint32* vehicleItemClicks;
	/*0d80,4*/	uint32* buildingItemClicks;
	/*0d84,128*/	bool32 menuItemUnkBools[Interface_MenuItem_Type_Count]; // (default: false)
	/*0eac,c0*/	Interface_Menu menuList[24];
	/*0f6c,8*/	Point2I selBlockPos;
	/*0f74,4*/	Interface_MenuType currMenuType;
	/*0f78,8*/	Point2F currMenuPosition; // (init: 565,18) Current sliding position of menu. Point2F_004decd0
	/*0f80,4*/	LegoObject_Type objType_f80;
	/*0f84,4*/	LegoObject_ID objID_f84;
	/*0f88,8*/	Point2F slideStartPosition; // Point2F_004dece0
	/*0f90,8*/	Point2F slideEndPosition; // (init: pointf_f78) Point2F_004dece8
	/*0f98,4*/	real32 slideSpeed; // (init: 750.0f / 25.0f)
	/*0f9c,8*/	Point2I highlightBlockPos;
	/*0fa4,4*/	Interface_MenuType nextMenuType;
	/*0fa8,4*/	Interface_MenuItemType menuItemType_fa8;
	/*0fac,4*/	undefined4 field_fac;
	/*0fb0,4*/	undefined4 field_fb0;
	/*0fb4,10*/	Area2F areaf_fb4;
	/*0fc4,4*/	bool32 flashingState; // For block and icon flashing. false = off, true = on.
	/*0fc8,4*/	real32 timer_fc8;
	/*0fcc,4*/	Advisor_Type advisorType_fcc;
	/*0fd0,4*/	Interface_MenuItemType menuItemType_fd0;
	/*0fd4,4*/	LegoObject_Type objType_fd4;
	/*0fd8,4*/	LegoObject_ID objID_fd8;
	/*0fdc,4b0*/	bool32 objectBools[LegoObject_Type_Count][LegoObject_ID_Count];
	/*148c,4*/	Gods98::Image* dependenciesPlusImage;
	/*1490,4*/	Gods98::Image* dependenciesMinusImage;
	/*1494,4*/	real32 float_1494;
	/*1498,4*/	WallHighlightType origWallHighlight; // (backup for when a wall has a queued action, but we want it to show the selected colour)
	/*149c,4*/	SFX_ID sfxType_149c;
	/*14a0,4*/	Interface_GlobFlags flags;
	/*14a4,4*/	SFX_ID sfxType_14a4;
	/*14a8,4*/	bool32 sfxPlaying;
	/*14ac*/
};
assert_sizeof(Interface_Globs, 0x14ac);

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @004a3b58>
extern real32 & g_Interface_TIMER_004a3b58;

// <LegoRR.exe @004a3b5c>
extern uint32 & g_Interface_UINT_004a3b5c;

// <LegoRR.exe @004ddd58>
extern Interface_Globs & interfaceGlobs;

#pragma endregion

/**********************************************************************************
 ******** Macros
 **********************************************************************************/

#pragma region Macros

#define Interface_RegisterMenuItemName(n)		(interfaceGlobs.menuItemName[n]=#n)

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @0041a220>
#define Interface_GetFont ((Gods98::Font* (__cdecl* )(void))0x0041a220)

// <LegoRR.exe @0041a230>
#define Interface_Initialise ((void (__cdecl* )(uint32 x_565, uint32 y_18, Gods98::Font* font))0x0041a230)

// <LegoRR.exe @0041a590>
#define Interface_AddAllMenuItems ((void (__cdecl* )(void))0x0041a590)

// <LegoRR.exe @0041a700>
#define Interface_AddMenuItems ((void (__cdecl* )(Interface_MenuType menuType, uint32 numItems, ...))0x0041a700)

// <LegoRR.exe @0041a780>
#define Interface_Shutdown ((void (__cdecl* )(void))0x0041a780)

// <LegoRR.exe @0041a850>
#define Interface_ClearStates ((void (__cdecl* )(void))0x0041a850)

// <LegoRR.exe @0041a8c0>
#define Interface_ResetMenu ((void (__cdecl* )(void))0x0041a8c0)

// <LegoRR.exe @0041a8f0>
#define Interface_GetMenuItemType ((bool32 (__cdecl* )(const char* menuItemName, OUT Interface_MenuItemType* menuItemType))0x0041a8f0)

// <LegoRR.exe @0041a930>
#define Interface_InitBuildItems ((void (__cdecl* )(void))0x0041a930)

// <LegoRR.exe @0041aa30>
#define Interface_LoadBuildItems ((void (__cdecl* )(const Gods98::Config* config, const char* gameName))0x0041aa30)

// <LegoRR.exe @0041acd0>
#define Interface_LoadMenuItems ((void (__cdecl* )(const Gods98::Config* config, const char* gameName))0x0041acd0)

// <LegoRR.exe @0041af00>
#define Interface_LoadItemPanels ((void (__cdecl* )(const Gods98::Config* config, const char* gameName))0x0041af00)

// <LegoRR.exe @0041b0e0>
#define Interface_LoadBackButton ((void (__cdecl* )(const Gods98::Config* config, const char* gameName))0x0041b0e0)

// <LegoRR.exe @0041b1a0>
#define Interface_LoadPlusMinusImages ((void (__cdecl* )(const char* plusName, const char* minusName))0x0041b1a0)

// <LegoRR.exe @0041b200>
#define Interface_OpenMenu ((void (__cdecl* )(Interface_MenuType menuType, const Point2I* blockPos))0x0041b200)

// <LegoRR.exe @0041b230>
#define _Interface_OpenMenu_SelectBlock ((void (__cdecl* )(Interface_MenuType menuType, const Point2I* blockPos))0x0041b230)

// <LegoRR.exe @0041b2f0>
#define Interface_FUN_0041b2f0 ((sint32 (__cdecl* )(Interface_MenuType interfaceMenuType))0x0041b2f0)

// <LegoRR.exe @0041b3a0>
#define Interface_MenuTypeHasBackButton ((bool32 (__cdecl* )(Interface_MenuType interfaceMenuType))0x0041b3a0)

// <LegoRR.exe @0041b3c0>
#define Interface_FUN_0041b3c0 ((void (__cdecl* )(void))0x0041b3c0)

// <LegoRR.exe @0041b5b0>
#define Interface_FUN_0041b5b0 ((bool32 (__cdecl* )(Interface_MenuType menuIcon, void* callback, void* context))0x0041b5b0)

// <LegoRR.exe @0041b730>
#define Interface_Callback_FUN_0041b730 ((bool32 (__cdecl* )(Interface_MenuItemType menuIcon, uint32 param_2, sint32 param_3, real32* param_4))0x0041b730)

// <LegoRR.exe @0041b860>
#define Interface_FUN_0041b860 ((void (__cdecl* )(real32 elapsedAbs))0x0041b860)

// This interface boolean is toggled on/off every 15-frames. Probably used for flashing.
// <LegoRR.exe @0041b8e0>
#define Interface_GetFlashingState ((bool32 (__cdecl* )(void))0x0041b8e0)

// <LegoRR.exe @0041b8f0>
#define Interface_GetPrimaryUnit_PowerIcon ((Interface_MenuItemType (__cdecl* )(IN OUT Interface_MenuItemType* menuIcon))0x0041b8f0)

// <LegoRR.exe @0041b940>
#define Interface_FUN_0041b940 ((void (__cdecl* )(real32 elapsedAbs))0x0041b940)

// <LegoRR.exe @0041b9c0>
#define Interface_SetSFX_004df1f4 ((void (__cdecl* )(SFX_ID sfxType))0x0041b9c0)

// <LegoRR.exe @0041b9d0>
#define Interface_DoSomethingWithRenameReplace ((bool32 (__cdecl* )(uint32 param_1, uint32 param_2, sint32 param_3, sint32 param_4, sint32 param_5))0x0041b9d0)

// <LegoRR.exe @0041c0f0>
#define Interface_FUN_0041c0f0 ((bool32 (__cdecl* )(uint32 param_1, uint32 param_2, OUT undefined4* param_3, OUT undefined4* param_4, OUT undefined4* param_5))0x0041c0f0)

// <LegoRR.exe @0041c240>
#define Interface_Callback_FUN_0041c240 ((bool32 (__cdecl* )(Interface_MenuItemType menuIcon, LegoObject_Type objType, LegoObject_ID objID, uint32* param_4))0x0041c240)

// <LegoRR.exe @0041c370>
//#define Interface_DoF2InterfaceKeyAction ((void (__cdecl* )(void))0x0041c370)
void __cdecl Interface_DoF2InterfaceKeyAction(void);

// <LegoRR.exe @0041c3a0>
//#define Interface_CallbackDoMenuIconKeyAction ((bool32 (__cdecl* )(Interface_MenuItemType menuIcon, LegoObject_Type objType, LegoObject_ID objID))0x0041c3a0)
bool32 __cdecl Interface_CallbackDoMenuIconKeyAction(Interface_MenuItemType menuIcon, LegoObject_Type objType, LegoObject_ID objID);

// <LegoRR.exe @0041c420>
#define Interface_FUN_0041c420 ((void (__cdecl* )(Interface_MenuItemType menuIcon, LegoObject_Type objType, LegoObject_ID objID, real32* param_4))0x0041c420)

// <LegoRR.exe @0041c610>
#define Interface_FUN_0041c610 ((bool32 (__cdecl* )(Interface_MenuItemType menuIcon, sint32 param_2, sint32 param_3, bool32 param_4, bool32 param_5))0x0041c610)

// <LegoRR.exe @0041c6e0>
#define Interface_FUN_0041c6e0 ((void (__cdecl* )(Interface_MenuItemType menuIcon))0x0041c6e0)

// <LegoRR.exe @0041c730>
#define Interface_FUN_0041c730 ((void (__cdecl* )(LegoObject_Type objType, LegoObject_ID objID))0x0041c730)

// <LegoRR.exe @0041c820>
#define Interface_FUN_0041c820 ((bool32 (__cdecl* )(Interface_MenuItemType menuIcon, bool32 param_2))0x0041c820)

// <LegoRR.exe @0041c880>
#define Interface_FUN_0041c880 ((bool32 (__cdecl* )(LegoObject_Type objType, LegoObject_ID objID, bool32 param_3))0x0041c880)

// <LegoRR.exe @0041c920>
#define Interface_FUN_0041c920 ((void (__cdecl* )(Interface_MenuItemType menuIcon, bool32 setFlag8))0x0041c920)

// <LegoRR.exe @0041c950>
#define Interface_FUN_0041c950 ((void (__cdecl* )(LegoObject_Type objType, LegoObject_ID objID, bool32 param_3))0x0041c950)

// <LegoRR.exe @0041c990>
#define Interface_FUN_0041c990 ((bool32 (__cdecl* )(Interface_MenuItemType menuIcon))0x0041c990)

// <LegoRR.exe @0041c9a0>
#define Interface_FUN_0041c9a0 ((bool32 (__cdecl* )(LegoObject_Type objType, LegoObject_ID objID))0x0041c9a0)

// <LegoRR.exe @0041c9e0>
#define Interface_FUN_0041c9e0 ((Gods98::Image* (__cdecl* )(Interface_MenuItemType menuIcon))0x0041c9e0)

// <LegoRR.exe @0041ca20>
#define Interface_GetBuildImageByObjectType ((Gods98::Image* (__cdecl* )(LegoObject_Type objType, LegoObject_ID objID))0x0041ca20)

// <LegoRR.exe @0041cac0>
#define Interface_GetObjectBuildImage ((Gods98::Image* (__cdecl* )(LegoObject_Type objType, LegoObject_ID objID, bool32 param_3))0x0041cac0)

// <LegoRR.exe @0041cbb0>
#define Interface_SetDat_004decd8_004decdc ((void (__cdecl* )(Interface_MenuItemType menuIcon, LegoObject_Type objType, LegoObject_ID objID))0x0041cbb0)

// <LegoRR.exe @0041cc10>
#define Interface_FUN_0041cc10 ((void (__cdecl* )(const Point2F* point, uint32 unkWidth, uint32 unkHeight))0x0041cc10)

// <LegoRR.exe @0041cc60>
//#define Interface_DrawHoverOutline ((void (__cdecl* )(const Area2F* area))0x0041cc60)
void __cdecl Interface_DrawHoverOutline(const Area2F* area);

// <LegoRR.exe @0041cdd0>
#define Interface_ChangeMenu_IfVehicleMounted_IsLiveObject ((void (__cdecl* )(LegoObject* liveObj))0x0041cdd0)

// <LegoRR.exe @0041ce50>
#define Interface_ChangeMenu_IfPrimarySelectedVehicle_IsLiveObject ((void (__cdecl* )(LegoObject* liveObj))0x0041ce50)

// <LegoRR.exe @0041ceb0>
#define Interface_BackToMain_IfSelectedWall_IsBlockPos ((void (__cdecl* )(const Point2I* blockPos))0x0041ceb0)

// <LegoRR.exe @0041cee0>
#define Interface_BackToMain_IfSelectedGroundOrConstruction_IsBlockPos ((void (__cdecl* )(const Point2I* blockPos))0x0041cee0)

// <LegoRR.exe @0041cf10>
#define Interface_BackToMain_IfSelectedRubble_IsBlockPos ((void (__cdecl* )(const Point2I* blockPos))0x0041cf10)

// <LegoRR.exe @0041cf40>
#define Interface_BackToMain_IfUnitIsSelected ((void (__cdecl* )(LegoObject* liveObj))0x0041cf40)

// <LegoRR.exe @0041cf70>
#define Interface_HandleMenuItem ((bool32 (__cdecl* )(Interface_MenuItemType menuIcon))0x0041cf70)

// <LegoRR.exe @0041dbd0>
#define Interface_DoAction_FUN_0041dbd0 ((bool32 (__cdecl* )(Interface_MenuItemType menuIcon))0x0041dbd0)

// <LegoRR.exe @0041e680>
#define Interface_BackToMain ((void (__cdecl* )(void))0x0041e680)

// <LegoRR.exe @0041e6a0>
#define Interface_SetIconFlash ((void (__cdecl* )(Interface_MenuItemType menuIcon, bool32 flash))0x0041e6a0)

// <LegoRR.exe @0041e6d0>
#define Interface_SetSubmenuIconFlash ((void (__cdecl* )(LegoObject_Type objType, LegoObject_ID objID, bool32 flash))0x0041e6d0)

// <LegoRR.exe @0041e710>
#define Interface_GetPrimarySelectedUnit ((LegoObject* (__cdecl* )(void))0x0041e710)

// <LegoRR.exe @0041e720>
#define Interface_SetIconClicked ((sint32 (__cdecl* )(Interface_MenuItemType menuIcon, sint32 clickedCount))0x0041e720)

// <LegoRR.exe @0041e740>
#define Interface_GetIconClicked ((sint32 (__cdecl* )(Interface_MenuItemType menuIcon))0x0041e740)

// <LegoRR.exe @0041e750>
#define Interface_SetSubmenuIconClicked ((sint32 (__cdecl* )(LegoObject_Type objType, LegoObject_ID objID, sint32 clickedCount))0x0041e750)

// <LegoRR.exe @0041e790>
#define Interface_GetSubmenuIconClicked ((sint32 (__cdecl* )(LegoObject_Type objType, LegoObject_ID objID))0x0041e790)

// <LegoRR.exe @0041e7c0>
#define Interface_SetAdvisorPointToFashingIcon ((bool32 (__cdecl* )(Interface_MenuItemType menuIcon, bool32 setFlag40))0x0041e7c0)

// <LegoRR.exe @0041e800>
#define Interface_GetAdvisorType_FromIcon ((Advisor_Type (__cdecl* )(Interface_MenuItemType menuIcon))0x0041e800)

// <LegoRR.exe @0041e8c0>
#define Interface_GetObjectBool_FUN_0041e8c0 ((bool32 (__cdecl* )(LegoObject_Type objType, LegoObject_ID objID))0x0041e8c0)

// <LegoRR.exe @0041e900>
#define Interface_SetScrollParameters ((void (__cdecl* )(uint32 xEnd, uint32 yEnd, uint32 xStart, uint32 yStart, real32 timerFloat_750))0x0041e900)

// <LegoRR.exe @0041e980>
#define _Interface_OpenMenu_SetNext ((void (__cdecl* )(Interface_MenuType menuType, OPTIONAL const Point2I* blockPos))0x0041e980)

// <LegoRR.exe @0041e9f0>
#define Interface_FUN_0041e9f0 ((void (__cdecl* )(real32 elapsedAbs))0x0041e9f0)

// <LegoRR.exe @0041eb60>
#define Interface_DrawTeleportQueueNumber ((void (__cdecl* )(LegoObject_Type objType, LegoObject_ID objID, const Point2F* screenPt))0x0041eb60)

// <LegoRR.exe @0041ebd0>
#define Interface_FUN_0041ebd0 ((void (__cdecl* )(real32 xScreen, real32 yScreen))0x0041ebd0)

// <LegoRR.exe @0041ed90>
#define Interface_SetFloatTo25_004df1ec_AndUnsetFlags800_004df1f8 ((void (__cdecl* )(void))0x0041ed90)

// <LegoRR.exe @0041edb0>
#define Interface_FUN_0041edb0 ((bool32 (__cdecl* )(uint32 param_1, uint32 param_2, real32 x, real32 y, OUT LegoObject_Type* objType, OUT LegoObject_ID* objID, OUT uint32* objLevel))0x0041edb0)

// <LegoRR.exe @0041f030>
#define Interface_HasTeleporterForObject ((bool32 (__cdecl* )(LegoObject_Type objType, LegoObject_ID objID))0x0041f030)

// DATA: SearchInterfaceFindObject*
// <LegoRR.exe @0041f0c0>
#define Interface_Callback_HasObject ((bool32 (__cdecl* )(LegoObject* liveObj, void* search))0x0041f0c0)

// <LegoRR.exe @0041f160>
#define Interface_HasUpgradeStation ((bool32 (__cdecl* )(void))0x0041f160)

// <LegoRR.exe @0041f1a0>
#define Interface_HasStatsFlags2 ((bool32 (__cdecl* )(StatsFlags2 statsFlags2))0x0041f1a0)

// <LegoRR.exe @0041f1e0>
#define Interface_HasToolStore ((bool32 (__cdecl* )(void))0x0041f1e0)

// <LegoRR.exe @0041f220>
#define Interface_HasObjectOfTypeID ((bool32 (__cdecl* )(LegoObject_Type objType, LegoObject_ID objID))0x0041f220)

// <LegoRR.exe @0041f270>
#define Interface_HasObjectWithAbilities ((bool32 (__cdecl* )(LegoObject_AbilityFlags abilityFlags))0x0041f270)

// <LegoRR.exe @0041f2c0>
#define Interface_Callback_ReqestDigBlock ((bool32 (__cdecl* )(LegoObject* liveObj, const Point2I* blockPos))0x0041f2c0)

// <LegoRR.exe @0041f2f0>
#define Interface_ReqestDigBlock ((bool32 (__cdecl* )(const Point2I* blockPos))0x0041f2f0)

// <LegoRR.exe @0041f310>
#define Interface_Callback_RequestReinforceBlock ((bool32 (__cdecl* )(LegoObject* liveObj, const Point2I* blockPos))0x0041f310)

// <LegoRR.exe @0041f330>
#define Interface_RequestReinforceBlock ((bool32 (__cdecl* )(const Point2I* blockPos))0x0041f330)

// <LegoRR.exe @0041f350>
#define Interface_DoSelectedUnits_Callback ((bool32 (__cdecl* )(LegoObject_RunThroughListsCallback callback, void* data))0x0041f350)

// <LegoRR.exe @0041f3a0>
#define Interface_ObjectCallback_IsCarryingButNotStoring ((bool32 (__cdecl* )(LegoObject* liveObj, void* unused))0x0041f3a0)

// <LegoRR.exe @0041f3c0>
#define Interface_ObjectCallback_IsEnergyBelowMax ((bool32 (__cdecl* )(LegoObject* liveObj, void* unused))0x0041f3c0)

// <LegoRR.exe @0041f3e0>
#define Interface_ObjectCallback_IsHealthBelowMax ((bool32 (__cdecl* )(LegoObject* liveObj, void* unused))0x0041f3e0)

// <LegoRR.exe @0041f400>
#define Interface_ObjectCallback_FUN_0041f400 ((bool32 (__cdecl* )(LegoObject* liveObj, void* unused))0x0041f400)

// <LegoRR.exe @0041f520>
#define Interface_ObjectCallback_SetFlags4_8_HealthM1 ((bool32 (__cdecl* )(LegoObject* liveObj, void* unused))0x0041f520)

// <LegoRR.exe @0041f540>
#define Interface_ObjectCallback_GoEatIfEnergyBelowMax ((bool32 (__cdecl* )(LegoObject* liveObj, void* unused))0x0041f540)

// <LegoRR.exe @0041f570>
#define Interface_ObjectCallback_RequestRepairIfHealthBelowMax ((bool32 (__cdecl* )(LegoObject* liveObj, void* unused))0x0041f570)

// <LegoRR.exe @0041f5a0>
#define Interface_ObjectCallback_DoesNotHaveToolEquipped ((bool32 (__cdecl* )(LegoObject* liveObj, LegoObject_ToolType toolType))0x0041f5a0)

// <LegoRR.exe @0041f5c0>
#define Interface_ObjectCallback_GoGetToolIfNotEquipped ((bool32 (__cdecl* )(LegoObject* liveObj, LegoObject_ToolType toolType))0x0041f5c0)

// <LegoRR.exe @0041f5f0>
#define Interface_ObjectCallback_FUN_0041f5f0 ((bool32 (__cdecl* )(LegoObject* liveObj, void* unused))0x0041f5f0)

// <LegoRR.exe @0041f650>
#define Interface_CheckPrimaryUnitHasAbility ((bool32 (__cdecl* )(LegoObject_AbilityFlags abilityFlag))0x0041f650)

// <LegoRR.exe @0041f670>
#define Interface_Block_FUN_0041f670 ((bool32 (__cdecl* )(const Point2I* blockPos))0x0041f670)

// <LegoRR.exe @0041f750>
#define Interface_ObjectCallback_HasToolEquipped_2 ((bool32 (__cdecl* )(LegoObject* liveObj, LegoObject_ToolType toolType))0x0041f750)

// <LegoRR.exe @0041f770>
#define Interface_ObjectCallback_PlaceBirdScarerIfEquipped ((bool32 (__cdecl* )(LegoObject* liveObj, void* unused))0x0041f770)

#pragma endregion

}
