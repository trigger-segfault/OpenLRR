// Interface.h : 
//

#pragma once

#include "../../common.h"
#include "../../engine/colour.h"
#include "../../engine/geometry.h"
#include "../../engine/undefined.h"

#include "../../engine/gfx/Containers.h"
#include "../../engine/drawing/Images.h"

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

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

// Interface_Globs structure is still being worked on...

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals


#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @0041a220>
#define Interface_GetFont_FUN_004ddd58 ((Font* (__cdecl* )(void))0x0041a220)

// <LegoRR.exe @0041a230>
#define Interface_Initialise ((void (__cdecl* )(uint32 x_565, uint32 y_18, Font* font))0x0041a230)

// <LegoRR.exe @0041a590>
#define Interface_AddAllMenuItems ((void (__cdecl* )(void))0x0041a590)

// <LegoRR.exe @0041a700>
#define Interface_AddMenuItem ((void (__cdecl* )(Interface_MenuType interfaceMenuType, sint32 numParams, ...))0x0041a700)

// <LegoRR.exe @0041a780>
#define Interface_FreeInterfaceIcons ((void (__cdecl* )(void))0x0041a780)

// <LegoRR.exe @0041a850>
#define Interface_LevelFree_FUN_0041a850 ((void (__cdecl* )(void))0x0041a850)

// <LegoRR.exe @0041a8c0>
#define Interface_ResetUnkValues ((void (__cdecl* )(void))0x0041a8c0)

// <LegoRR.exe @0041a8f0>
#define Interface_GetMenuItemType ((bool32 (__cdecl* )(const char* menuItemName, OUT Interface_MenuItem* menuItemType))0x0041a8f0)

// <LegoRR.exe @0041a930>
#define Interface_InitSubmenuIconTables ((void (__cdecl* )(void))0x0041a930)

// <LegoRR.exe @0041aa30>
#define Interface_LoadInterfaceBuildImages ((void (__cdecl* )(const Gods98::Config* config, const char* keyBasePath))0x0041aa30)

// <LegoRR.exe @0041acd0>
#define Interface_LoadInterfaceImages ((void (__cdecl* )(const Gods98::Config* config, const char* keyBasePath))0x0041acd0)

// <LegoRR.exe @0041af00>
#define Interface_LoadInterfaceSurroundImages ((void (__cdecl* )(const Gods98::Config* config, const char* keyBasePath))0x0041af00)

// <LegoRR.exe @0041b0e0>
#define Interface_LoadInterfaceBackButton ((void (__cdecl* )(const Gods98::Config* config, const char* keyBasePath))0x0041b0e0)

// <LegoRR.exe @0041b1a0>
#define Interface_LoadInterfaceDependencies_PlusMinus ((void (__cdecl* )(const char* plus, const char* minus))0x0041b1a0)

// <LegoRR.exe @0041b200>
#define Interface_OpenMenu_FUN_0041b200 ((void (__cdecl* )(Interface_MenuType interfaceMenuType, const Point2I* blockPos))0x0041b200)

// <LegoRR.exe @0041b230>
#define Interface_SelectBlock ((void (__cdecl* )(Interface_MenuType interfaceMenuType, const Point2I* blockPos))0x0041b230)

// <LegoRR.exe @0041b2f0>
#define Interface_FUN_0041b2f0 ((sint32 (__cdecl* )(Interface_MenuType interfaceMenuType))0x0041b2f0)

// <LegoRR.exe @0041b3a0>
#define Interface_NotMainOrFirstPersonMenu ((bool32 (__cdecl* )(Interface_MenuType interfaceMenuType))0x0041b3a0)

// <LegoRR.exe @0041b3c0>
#define Interface_FUN_0041b3c0 ((void (__cdecl* )(void))0x0041b3c0)

// <LegoRR.exe @0041b5b0>
#define Interface_FUN_0041b5b0 ((bool32 (__cdecl* )(Interface_MenuType menuIcon, undefined* callback, void* context))0x0041b5b0)

// <LegoRR.exe @0041b730>
#define Interface_Callback_FUN_0041b730 ((bool32 (__cdecl* )(Interface_MenuItem menuIcon, uint32 param_2, sint32 param_3, real32* param_4))0x0041b730)

// <LegoRR.exe @0041b860>
#define Interface_FUN_0041b860 ((void (__cdecl* )(real32 elapsedAbs))0x0041b860)

// <LegoRR.exe @0041b8e0>
#define Interface_GetBool_004ded1c ((bool32 (__cdecl* )(void))0x0041b8e0)

// <LegoRR.exe @0041b8f0>
#define Interface_GetPrimaryUnit_PowerIcon ((Interface_MenuItem (__cdecl* )(IN OUT Interface_MenuItem* menuIcon))0x0041b8f0)

// <LegoRR.exe @0041b940>
#define Interface_FUN_0041b940 ((void (__cdecl* )(real32 elapsedAbs))0x0041b940)

// <LegoRR.exe @0041b9c0>
#define Interface_SetDat_004df1f4 ((void (__cdecl* )(SFX_Type param_1))0x0041b9c0)

// <LegoRR.exe @0041b9d0>
#define Interface_DoSomethingWithRenameReplace ((bool32 (__cdecl* )(uint32 param_1, uint32 param_2, sint32 param_3, sint32 param_4, sint32 param_5))0x0041b9d0)

// <LegoRR.exe @0041c0f0>
#define Interface_FUN_0041c0f0 ((bool32 (__cdecl* )(uint32 param_1, uint32 param_2, undefined4* param_3, undefined4* param_4, undefined4* param_5))0x0041c0f0)

// <LegoRR.exe @0041c240>
#define Interface_Callback_FUN_0041c240 ((bool32 (__cdecl* )(Interface_MenuItem menuIcon, LegoObject_Type objType, sint32 objID, uint32* param_4))0x0041c240)

// <LegoRR.exe @0041c370>
#define Interface_DoF2InterfaceKeyAction ((void (__cdecl* )(void))0x0041c370)

// <LegoRR.exe @0041c3a0>
#define Interface_CallbackDoMenuIconKeyAction ((bool32 (__cdecl* )(Interface_MenuItem menuIcon, LegoObject_Type objType, sint32 objID))0x0041c3a0)

// <LegoRR.exe @0041c420>
#define Interface_FUN_0041c420 ((void (__cdecl* )(Interface_MenuItem menuIcon, LegoObject_Type objType, sint32 objID, real32* param_4))0x0041c420)

// <LegoRR.exe @0041c610>
#define Interface_FUN_0041c610 ((bool32 (__cdecl* )(Interface_MenuItem menuIcon, sint32 param_2, sint32 param_3, sint32 param_4, sint32 param_5))0x0041c610)

// <LegoRR.exe @0041c6e0>
#define Interface_FUN_0041c6e0 ((void (__cdecl* )(Interface_MenuItem menuIcon))0x0041c6e0)

// <LegoRR.exe @0041c730>
#define Interface_FUN_0041c730 ((void (__cdecl* )(LegoObject_Type objType, sint32 objID))0x0041c730)

// <LegoRR.exe @0041c820>
#define Interface_FUN_0041c820 ((bool32 (__cdecl* )(Interface_MenuItem menuIcon, bool32 param_2))0x0041c820)

// <LegoRR.exe @0041c880>
#define Interface_FUN_0041c880 ((bool32 (__cdecl* )(LegoObject_Type objType, sint32 objID, bool32 param_3))0x0041c880)

// <LegoRR.exe @0041c920>
#define Interface_FUN_0041c920 ((void (__cdecl* )(Interface_MenuItem menuIcon, bool32 setFlag8))0x0041c920)

// <LegoRR.exe @0041c950>
#define Interface_FUN_0041c950 ((void (__cdecl* )(LegoObject_Type objType, sint32 objID, bool32 param_3))0x0041c950)

// <LegoRR.exe @0041c990>
#define Interface_FUN_0041c990 ((bool32 (__cdecl* )(Interface_MenuItem menuIcon))0x0041c990)

// <LegoRR.exe @0041c9a0>
#define Interface_FUN_0041c9a0 ((bool32 (__cdecl* )(LegoObject_Type objType, sint32 objID))0x0041c9a0)

// <LegoRR.exe @0041c9e0>
#define Interface_FUN_0041c9e0 ((Image* (__cdecl* )(Interface_MenuItem menuIcon))0x0041c9e0)

// <LegoRR.exe @0041ca20>
#define Interface_GetBuildImageByObjectType ((Image* (__cdecl* )(LegoObject_Type objType, sint32 objID))0x0041ca20)

// <LegoRR.exe @0041cac0>
#define Interface_GetObjectBuildImage ((Image* (__cdecl* )(LegoObject_Type objType, sint32 objID, bool32 param_3))0x0041cac0)

// <LegoRR.exe @0041cbb0>
#define Interface_SetDat_004decd8_004decdc ((void (__cdecl* )(Interface_MenuItem menuIcon, LegoObject_Type objType, sint32 objID))0x0041cbb0)

// <LegoRR.exe @0041cc10>
#define Interface_FUN_0041cc10 ((void (__cdecl* )(const Point2F* point, uint32 unkWidth, uint32 unkHeight))0x0041cc10)

// <LegoRR.exe @0041cc60>
#define Interface_Front_DrawLineListUnk_FUN_0041cc60 ((void (__cdecl* )(const Area2F* rect))0x0041cc60)

// <LegoRR.exe @0041cdd0>
#define Interface_ChangeMenu_IfVehicleMounted_IsLiveObject ((void (__cdecl* )(LegoObject* liveObj))0x0041cdd0)

// <LegoRR.exe @0041ce50>
#define Interface_ChangeMenu_IfPrimarySelectedVehicle_IsLiveObject ((void (__cdecl* )(LegoObject* liveObj))0x0041ce50)

// <LegoRR.exe @0041ceb0>
#define Interface_BackToMain_IfSelectedWall_IsBlockPos ((void (__cdecl* )(const Point2I* blockPos))0x0041ceb0)

// <LegoRR.exe @0041cee0>
#define Interface_BackToMain_IfSelectedGroundOrConstruction_IsBlockPos ((void (__cdecl* )(const Point2I* blockPos))0x0041cee0)

// <LegoRR.exe @0041cf10>
#define Interface_IfSelectedRubble_IsBlockPos ((void (__cdecl* )(const Point2I* blockPos))0x0041cf10)

// <LegoRR.exe @0041cf40>
#define Interface_BackToMain_IfLiveObject_IsSelected_OrFlags3_200000 ((void (__cdecl* )(LegoObject* liveObj))0x0041cf40)

// <LegoRR.exe @0041cf70>
#define Interface_HandleIcon_FUN_0041cf70 ((bool32 (__cdecl* )(Interface_MenuItem menuIcon))0x0041cf70)

// <LegoRR.exe @0041dbd0>
#define Interface_DoAction_FUN_0041dbd0 ((bool32 (__cdecl* )(Interface_MenuItem menuIcon))0x0041dbd0)

// <LegoRR.exe @0041e680>
#define Interface_BackToMain ((void (__cdecl* )(void))0x0041e680)

// <LegoRR.exe @0041e6a0>
#define Interface_SetIconFlash ((void (__cdecl* )(Interface_MenuItem menuIcon, bool32 flash))0x0041e6a0)

// <LegoRR.exe @0041e6d0>
#define Interface_SetSubmenuIconFlash ((void (__cdecl* )(LegoObject_Type objType, sint32 objID, bool32 flash))0x0041e6d0)

// <LegoRR.exe @0041e710>
#define Interface_GetPrimarySelectedUnit ((LegoObject* (__cdecl* )(void))0x0041e710)

// <LegoRR.exe @0041e720>
#define Interface_SetIconClicked ((sint32 (__cdecl* )(Interface_MenuItem menuIcon, sint32 clickedCount))0x0041e720)

// <LegoRR.exe @0041e740>
#define Interface_GetIconClicked ((sint32 (__cdecl* )(Interface_MenuItem menuIcon))0x0041e740)

// <LegoRR.exe @0041e750>
#define Interface_SetSubmenuIconClicked ((sint32 (__cdecl* )(LegoObject_Type objType, sint32 objID, sint32 clickedCount))0x0041e750)

// <LegoRR.exe @0041e790>
#define Interface_GetSubmenuIconClicked ((sint32 (__cdecl* )(LegoObject_Type objType, sint32 objID))0x0041e790)

// <LegoRR.exe @0041e7c0>
#define Interface_SetAdvisorPointToFashingIcon ((bool32 (__cdecl* )(Interface_MenuItem menuIcon, bool32 setFlag40))0x0041e7c0)

// <LegoRR.exe @0041e800>
#define Interface_GetAdvisorType_FromIcon ((Advisor_Type (__cdecl* )(Interface_MenuItem menuIcon))0x0041e800)

// <LegoRR.exe @0041e8c0>
#define Interface_FUN_0041e8c0 ((bool32 (__cdecl* )(LegoObject_Type objType, sint32 objID))0x0041e8c0)

// <LegoRR.exe @0041e900>
#define Interface_SetUnkPositions1 ((void (__cdecl* )(uint32 x1_565, uint32 y1_18, uint32 x2_appWidthPlus10, uint32 y2_18, real32 param_5))0x0041e900)

// <LegoRR.exe @0041e980>
#define Interface_UnkSlideOffScreen_FUN_0041e980_internal ((void (__cdecl* )(Interface_MenuType interfaceMenuType, OPTIONAL const Point2I* blockPos))0x0041e980)

// <LegoRR.exe @0041e9f0>
#define Interface_FUN_0041e9f0 ((void (__cdecl* )(real32 elapsedAbs))0x0041e9f0)

// <LegoRR.exe @0041eb60>
#define Interface_DrawTeleportQueueNumber ((void (__cdecl* )(sint32 param_1, sint32 param_2, const Point2F* screenPt))0x0041eb60)

// <LegoRR.exe @0041ebd0>
#define Interface_FUN_0041ebd0 ((void (__cdecl* )(real32 xScreen, real32 yScreen))0x0041ebd0)

// <LegoRR.exe @0041ed90>
#define Interface_SetFloatTo25_004df1ec_AndUnsetFlags800_004df1f8 ((void (__cdecl* )(void))0x0041ed90)

// <LegoRR.exe @0041edb0>
#define Interface_FUN_0041edb0 ((bool32 (__cdecl* )(uint32 param_1, uint32 param_2, real32 param_3, real32 param_4, undefined4* param_5, undefined4* param_6, undefined4* param_7))0x0041edb0)

// <LegoRR.exe @0041f030>
#define Interface_Object_FindTeleporter_FUN_0041f030 ((bool32 (__cdecl* )(LegoObject_Type objType, sint32 objID))0x0041f030)

// Search type: LegoObject_Type*
// <LegoRR.exe @0041f0c0>
#define Interface_LiveObject_CallbackCheck_FUN_0041f0c0 ((bool32 (__cdecl* )(LegoObject* liveObj, LegoObject_Type* search))0x0041f0c0)

// <LegoRR.exe @0041f160>
#define Interface_FUN_0041f160 ((bool32 (__cdecl* )(void))0x0041f160)

// <LegoRR.exe @0041f1a0>
#define Interface_FUN_0041f1a0 ((bool32 (__cdecl* )(undefined4 param_1))0x0041f1a0)

// <LegoRR.exe @0041f1e0>
#define Interface_FUN_0041f1e0 ((bool32 (__cdecl* )(void))0x0041f1e0)

// <LegoRR.exe @0041f220>
#define Interface_FUN_0041f220 ((bool32 (__cdecl* )(undefined4 param_1, undefined4 param_2))0x0041f220)

// <LegoRR.exe @0041f270>
#define Interface_FUN_0041f270 ((bool32 (__cdecl* )(undefined4 param_1))0x0041f270)

// <LegoRR.exe @0041f2c0>
#define Interface_LiveObject_Callback_FUN_0041f2c0 ((bool32 (__cdecl* )(LegoObject* liveObj, const Point2I* blockPos))0x0041f2c0)

// <LegoRR.exe @0041f2f0>
#define Interface_ReqestDigBlock ((bool32 (__cdecl* )(const Point2I* blockPos))0x0041f2f0)

// <LegoRR.exe @0041f310>
#define Interface_LiveObjectCallback_RequestReinforceBlock ((bool32 (__cdecl* )(LegoObject* liveObj, const Point2I* blockPos))0x0041f310)

// <LegoRR.exe @0041f330>
#define Interface_RequestReinforceBlock ((bool32 (__cdecl* )(const Point2I* blockPos))0x0041f330)

// <LegoRR.exe @0041f350>
#define Interface_DoSelectedUnits_Callback ((bool32 (__cdecl* )(undefined* callback, sint32 lpContext))0x0041f350)

// <LegoRR.exe @0041f3a0>
#define Interface_LiveObjectCallback_FUN_0041f3a0 ((bool32 (__cdecl* )(LegoObject* liveObj))0x0041f3a0)

// <LegoRR.exe @0041f3c0>
#define Interface_LiveObjectCallback_IsEnergyLessThan100 ((bool32 (__cdecl* )(LegoObject* liveObj))0x0041f3c0)

// <LegoRR.exe @0041f3e0>
#define Interface_LiveObjectCallback_IsHealthLessThan100 ((bool32 (__cdecl* )(LegoObject* liveObj))0x0041f3e0)

// <LegoRR.exe @0041f400>
#define Interface_LiveObjectCallback_FUN_0041f400 ((bool32 (__cdecl* )(LegoObject* liveObj))0x0041f400)

// <LegoRR.exe @0041f520>
#define Interface_LiveObjectCallback_FUN_0041f520 ((bool32 (__cdecl* )(LegoObject* liveObj))0x0041f520)

// <LegoRR.exe @0041f540>
#define Interface_LiveObjectCallback_GoEatIfEnergyIsLessThan100 ((bool32 (__cdecl* )(LegoObject* liveObj))0x0041f540)

// <LegoRR.exe @0041f570>
#define Interface_LiveObjectCallback_GoRepairIfHealthLessThan100 ((bool32 (__cdecl* )(LegoObject* liveObj))0x0041f570)

// <LegoRR.exe @0041f5a0>
#define Interface_LiveObjectCallback_DoesNotHaveToolEquipped ((bool32 (__cdecl* )(LegoObject* liveObj, LegoObject_ToolType toolType))0x0041f5a0)

// <LegoRR.exe @0041f5c0>
#define Interface_LiveObjectCallback_GoGetToolIfNotEquipped ((bool32 (__cdecl* )(LegoObject* liveObj, LegoObject_ToolType toolType))0x0041f5c0)

// <LegoRR.exe @0041f5f0>
#define Interface_LiveObjectCallback_FUN_0041f5f0 ((bool32 (__cdecl* )(LegoObject* liveObj))0x0041f5f0)

// <LegoRR.exe @0041f650>
#define Interface_FUN_0041f650 ((bool32 (__cdecl* )(uint32 param_1))0x0041f650)

// <LegoRR.exe @0041f670>
#define Interface_Block_FUN_0041f670 ((bool32 (__cdecl* )(const Point2I* blockPos))0x0041f670)

// <LegoRR.exe @0041f750>
#define Interface_LiveObjectCallback_HasToolEquipped_2 ((bool32 (__cdecl* )(LegoObject* liveObj, LegoObject_ToolType toolType))0x0041f750)

// <LegoRR.exe @0041f770>
#define Interface_LiveObjectCallback_FUN_0041f770 ((bool32 (__cdecl* )(LegoObject* liveObj))0x0041f770)

#pragma endregion

}
