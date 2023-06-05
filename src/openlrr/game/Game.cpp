// Game.cpp : 
//

#include "../engine/audio/3DSound.h"
#include "../engine/core/Maths.h"
#include "../engine/core/Utils.h"
#include "../engine/drawing/DirectDraw.h"
#include "../engine/gfx/Viewports.h"
#include "../engine/input/Input.h"
#include "../engine/input/Keys.h"
#include "../engine/Main.h"

#include "audio/SFX.h"
#include "effects/DamageText.h"
#include "effects/Effects.h"
#include "effects/LightEffects.h"
#include "effects/Smoke.h"
#include "front/FrontEnd.h"
#include "front/Reward.h"
#include "interface/hud/Bubbles.h"
#include "interface/Advisor.h"
#include "interface/HelpWindow.h"
#include "interface/InfoMessages.h"
#include "interface/Interface.h"
#include "interface/Panels.h"
#include "interface/RadarMap.h"
#include "interface/TextMessages.h"
#include "interface/ToolTip.h"
#include "mission/Messages.h"
#include "mission/NERPsFile.h"
#include "mission/NERPsFunctions.h"
#include "mission/Objective.h"
#include "object/AITask.h"
#include "object/Dependencies.h"
#include "object/Object.h"
#include "object/Stats.h"
#include "world/Construction.h"
#include "world/Detail.h"
#include "world/ElectricFence.h"
#include "world/Erode.h"
#include "world/Fallin.h"
#include "world/Map3D.h"
#include "world/Roof.h"
#include "world/SpiderWeb.h"
#include "world/Teleporter.h"
#include "world/Water.h"
#include "Debug.h"
#include "Shortcuts.hpp"
#include "Game.h"


using Gods98::Keys;
using Shortcuts::ShortcutID;


/**********************************************************************************
 ******** Game Entry point
 **********************************************************************************/

#pragma region Entry point

void __cdecl LegoRR::Lego_Gods_Go_Setup(const char* programName, OUT Gods98::Main_State* mainState)
{
	std::memset(&LegoRR::legoGlobs, 0, 0xef8); //sizeof(legoGlobs);
	LegoRR::legoGlobs.gameName = programName;

	
	/*Gods98::Main_State mainState = {
		Lego_Initialise,
		Lego_MainLoop,
		Lego_Shutdown_Full, // proper shutdown, with cleanup
	};*/

	mainState->Initialise = Lego_Initialise;
	mainState->MainLoop = Lego_MainLoop;
	mainState->Shutdown = Lego_Shutdown_Full; // shutdown with cleanup

	if (Gods98::Main_ProgrammerMode() != 10) { // PROGRAMMER_MODE_10
		mainState->Shutdown = Lego_Shutdown_Quick; // immediate shutdown, no cleanup
	}
}

// This is the GAME entry point as called by WinMain,
//  this should hook the Main_State loop functions and only perform basic initial setup.
// (this can return bool32, but does not)
// <LegoRR.exe @0041f950>
void __cdecl LegoRR::Lego_Gods_Go(const char* programName)
{
	log_firstcall();

	Gods98::Main_State mainState = { nullptr }; // dummy init
	Lego_Gods_Go_Setup(programName, &mainState);

	Gods98::Main_SetTitle(programName);
	Gods98::Main_SetState(&mainState);

	/*std::memset(&LegoRR::legoGlobs, 0, 0xef8); //sizeof(legoGlobs);
	LegoRR::legoGlobs.gameName = programName;

	/// FLUFF OPENLRR: Wrap the program name in parenthesis and start with "OpenLRR"
	char buff[1024];
	if (std::strcmp(programName, "OpenLRR") != 0) {
		std::sprintf(buff, "%s (%s)", "OpenLRR", programName);
		programName = buff;
	}
	Gods98::Main_SetTitle(programName);
	
	Gods98::Main_State mainState = {
		Lego_Initialise,
		Lego_MainLoop,
		Lego_Shutdown_Full, // proper shutdown, with cleanup
	};
	if (Gods98::Main_ProgrammerMode() != 10) { // PROGRAMMER_MODE_10
		mainState.Shutdown = Lego_Shutdown_Quick; // immediate shutdown, no cleanup
	}

	Gods98::Main_SetState(&mainState);*/
}

#pragma endregion

/**********************************************************************************
 ******** Constants
 **********************************************************************************/

#pragma region Constants

#define LEGO_DRAWFILL_SELECTIONBOXES		true

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @004a4558>
LegoRR::LegoUpdate_Globs & LegoRR::updateGlobs = *(LegoRR::LegoUpdate_Globs*)0x004a4558;

// Only used by Lego_ShowBlockToolTip
// <LegoRR.exe @004df208>
Point2I & LegoRR::s_ShowBlockToolTip_MousePos = *(Point2I*)0x004df208;

// <LegoRR.exe @004df410>
LegoRR::GameControl_Globs & LegoRR::gamectrlGlobs = *(LegoRR::GameControl_Globs*)0x004df410;

// <LegoRR.exe @005570c0>
LegoRR::Lego_Globs & LegoRR::legoGlobs = *(LegoRR::Lego_Globs*)0x005570c0;

LegoRR::Lego_Globs2 LegoRR::legoGlobs2 = { 0 };

static bool _cheatBuildOnAnyRoughness = false;
static bool _cheatNoBuildCosts = false;
static bool _cheatNoConstructionBarriers = false;
static bool _cheatNoPowerConsumption = false;
static bool _cheatNoOxygenConsumption = false;
static bool _cheatSuperToolStore = false;

static bool _legoTransparentMultiSelectBox = true;
static bool _legoShowToolTips = true;
static LegoRR::LOD_PolyLevel _legoTopdownLOD = LegoRR::LOD_PolyLevel::LOD_LowPoly;

static LegoRR::LegoObject* _followUnit = nullptr;

// PowerGrid replacements for infinite grid sizes.
static std::vector<Point2I> _drainPowerBlockList;
static std::vector<Point2I> _poweredBlockList;
static std::vector<Point2I> _unpoweredBlockList;

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

bool LegoRR::Cheat_IsBuildOnAnyRoughness()
{
	return _cheatBuildOnAnyRoughness;
}

void LegoRR::Cheat_SetBuildOnAnyRoughness(bool on)
{
	_cheatBuildOnAnyRoughness = on;
}

bool LegoRR::Cheat_IsNoBuildCosts()
{
	return _cheatNoBuildCosts;
}

void LegoRR::Cheat_SetNoBuildCosts(bool on)
{
	_cheatNoBuildCosts = on;
}


bool LegoRR::Cheat_IsNoConstructionBarriers()
{
	return _cheatNoConstructionBarriers;
}

void LegoRR::Cheat_SetNoConstructionBarriers(bool on)
{
	_cheatNoConstructionBarriers = on;
}


bool LegoRR::Cheat_IsNoPowerConsumption()
{
	return _cheatNoPowerConsumption;
}

void LegoRR::Cheat_SetNoPowerConsumption(bool on)
{
	if (_cheatNoPowerConsumption != on) {
		_cheatNoPowerConsumption = on;
		// Allow toggling the cheat when outside of a level.
		if (Lego_IsInLevel()) {
			LegoObject_RequestPowerGridUpdate();
		}
	}
}


bool LegoRR::Cheat_IsNoOxygenConsumption()
{
	return _cheatNoOxygenConsumption;
}

void LegoRR::Cheat_SetNoOxygenConsumption(bool on)
{
	_cheatNoOxygenConsumption = on;
}


bool LegoRR::Cheat_IsSuperToolStore()
{
	return _cheatSuperToolStore;
}

void LegoRR::Cheat_SetSuperToolStore(bool on)
{
	_cheatSuperToolStore = on;
}


void LegoRR::Cheat_SurveyLevel()
{
	for (uint32 y = 0; y < Lego_GetLevel()->height; y++) {
		for (uint32 x = 0; x < Lego_GetLevel()->width; x++) {
			blockValue(Lego_GetLevel(), x, y).flags1 |= BLOCK1_SURVEYED;
		}
	}
}

bool LegoRR::Lego_IsLevelSurveyed()
{
	if (!Lego_IsInLevel())
		return false; // Default to false when not in level.

	for (uint32 y = 0; y < Lego_GetLevel()->height; y++) {
		for (uint32 x = 0; x < Lego_GetLevel()->width; x++) {
			// Check if a block isn't surveyed.
			if (!(blockValue(Lego_GetLevel(), x, y).flags1 & BLOCK1_SURVEYED))
				return false;
		}
	}
	return true;
}


LegoRR::LegoObject* LegoRR::Lego_GetFollowUnit()
{
	return _followUnit;
}

void LegoRR::Lego_SetFollowUnit(OPTIONAL LegoObject* liveObj)
{
	_followUnit = liveObj;
}

LegoRR::LegoObject* LegoRR::Lego_GetTopdownOrFPUnit()
{
	if (legoGlobs.viewMode == ViewMode_Top) {
		if (Message_GetNumSelectedUnits() == 1 && Lego_IsTopdownFPControlsOn()) {
			return Message_GetPrimarySelectedUnit();
		}
	}
	else if (legoGlobs.viewMode == ViewMode_FP) {
		return legoGlobs.objectFP;
	}
	return nullptr;
}

real32 LegoRR::Cheat_IsFasterUnit(LegoObject* liveObj)
{
	return (liveObj == Lego_GetTopdownOrFPUnit() && Lego_IsAllowDebugKeys() &&
			Shortcut_IsDown(ShortcutID::Cheat_FasterUnit));
}

real32 LegoRR::Cheat_GetFasterUnitCoef(LegoObject* liveObj, real32 coef)
{
	if (Cheat_IsFasterUnit(liveObj)) {
		return coef;
	}
	return 1.0f;
}


bool LegoRR::Lego_IsTransparentMultiSelectBox()
{
	return _legoTransparentMultiSelectBox;
}

void LegoRR::Lego_SetTransparentMultiSelectBox(bool on)
{
	_legoTransparentMultiSelectBox = on;
}


bool LegoRR::Lego_IsShowToolTips()
{
	return _legoShowToolTips;
}

void LegoRR::Lego_SetShowToolTips(bool on)
{
	_legoShowToolTips = on;
}


LegoRR::LOD_PolyLevel LegoRR::Lego_GetTopdownLOD()
{
	return _legoTopdownLOD;
}

void LegoRR::Lego_SetTopdownLOD(LOD_PolyLevel lod)
{
	// FP poly is probably special, and may hide parts of a unit from view.
	if (lod == LOD_FPPoly) lod = LOD_HighPoly;
	_legoTopdownLOD = lod;
}


/// CUSTOM: cfg: Main::ShowDebugToolTips
void LegoRR::Lego_SetShowDebugToolTips(bool on)
{
	if (on) legoGlobs.flags2 |= GAME2_SHOWDEBUGTOOLTIPS;
	else    legoGlobs.flags2 &= ~GAME2_SHOWDEBUGTOOLTIPS;
}

/// CUSTOM: cfg: Main::AllowDebugKeys
void LegoRR::Lego_SetAllowDebugKeys(bool on)
{
	if (on) legoGlobs.flags2 |= GAME2_ALLOWDEBUGKEYS;
	else    legoGlobs.flags2 &= ~GAME2_ALLOWDEBUGKEYS;
}

/// CUSTOM: cfg: Main::AllowEditMode
void LegoRR::Lego_SetAllowEditMode(bool on)
{
	if (on) legoGlobs.flags2 |= GAME2_ALLOWEDITMODE;
	else    legoGlobs.flags2 &= ~GAME2_ALLOWEDITMODE;
}

/// REQUIRES: Replacing `Lego_HandleKeys`, since this is always checked by CFG lookup.
/// CUSTOM: cfg: Main::LoseFocusAndPause
bool LegoRR::Lego_IsLoseFocusAndPause()
{
	return legoGlobs2.loseFocusAndPause;

	//return Config_GetBoolOrFalse(legoGlobs.config, Main_ID("LoseFocusAndPause"));
}
void LegoRR::Lego_SetLoseFocusAndPause(bool on)
{
	legoGlobs2.loseFocusAndPause = on;

	/*Gods98::Config* cfgFocus = const_cast<Gods98::Config*>(Gods98::Config_FindItem(legoGlobs.config, Main_ID("LoseFocusAndPause")));
	if (cfgFocus == nullptr) {
		// Hoooh boy, this is gonna be a pain. The game reads the cfg value AS NEEDED!!!

		// Instead of doing anything fancy with the existing tree,
		//  we'll append a new tree with the exact path to the property we need.

		// Find tail of config linked list.
		Gods98::Config* tail = legoGlobs.config;
		while (tail->linkNext != nullptr) tail = tail->linkNext;

		Gods98::Config* cfgGame = Gods98::Config_Create(tail);
		Gods98::Config* cfgMain = Gods98::Config_Create(cfgGame);
		cfgFocus = Gods98::Config_Create(cfgMain);

		// Lego*::
		cfgGame->depth = 0;
		cfgGame->itemName = legoGlobs.gameName;
		cfgGame->dataString = "{";
		// Lego*::Main::
		cfgMain->depth = cfgGame->depth + 1;
		cfgMain->itemName = "Main";
		cfgMain->dataString = "{";
		// Lego*::Main::LoseFocusAndPause
		cfgFocus->depth = cfgMain->depth + 1;
		cfgFocus->itemName = "LoseFocusAndPause";
		cfgFocus->dataString = "FALSE"; // default value when not found
	}
	cfgFocus->dataString = (on ? "TRUE" : "FALSE");*/
}

/// REQUIRES: Replacing `Weapon_LegoObject_SeeThroughWalls_FUN_00471c20`, since this is always checked by CFG lookup.
/// CUSTOM: cfg: Level::SeeThroughWalls
bool LegoRR::Lego_IsSeeThroughWalls()
{
	if (Lego_IsInLevel()) {
		return legoGlobs2.seeThroughWalls;
	}
	return true; // Default value when none exists, doesn't really matter when outside of a level.

	//return Config_GetBoolOrTrue(legoGlobs.config, Lego_ID(levelName, "SeeThroughWalls"));
}
void LegoRR::Lego_SetSeeThroughWalls(bool on)
{
	if (Lego_IsInLevel()) {
		legoGlobs2.seeThroughWalls = on;
	}

	/*/// FIXME: This is a bad temporary implementation, because it permenantly changes the level for this game session!!!
	Gods98::Config* cfgSee = const_cast<Gods98::Config*>(Gods98::Config_FindItem(legoGlobs.config, Lego_ID(levelName, "SeeThroughWalls")));
	if (cfgSee == nullptr) {

		// We know this property has to exist if this is a valid level.
		// Lego*::Levels::<levelName>
		Gods98::Config* cfgLevel = const_cast<Gods98::Config*>(Gods98::Config_FindItem(legoGlobs.config, Lego_ID(levelName)));

		if (cfgLevel == nullptr)
			return; // Failed to find level with this name.

		// Find tail of level block linked list.
		Gods98::Config* tail = cfgLevel;
		while (tail->linkNext != nullptr) tail = tail->linkNext;

		// Create as child of cfgLevel.
		cfgSee = Gods98::Config_Create(cfgLevel);

		// Lego*::Levels::<levelName>::SeeThroughWalls
		cfgSee->depth = cfgLevel->depth + 1;
		cfgSee->itemName = "SeeThroughWalls";
		cfgSee->dataString = "TRUE"; // default value when not found
	}
	cfgSee->dataString = (on ? "TRUE" : "FALSE");*/
}

/// CUSTOM: cfg: Main::DisableToolTipSound
void LegoRR::Lego_SetDisableToolTipSound(bool disabled)
{
	if (disabled) legoGlobs.flags2 |= GAME2_DISABLETOOLTIPSOUND;
	else          legoGlobs.flags2 &= ~GAME2_DISABLETOOLTIPSOUND;
}

/// CUSTOM: cfg: Main::Panels
void LegoRR::Lego_SetRenderPanels(bool on)
{
	if (on) legoGlobs.flags1 |= GAME1_RENDERPANELS;
	else    legoGlobs.flags1 &= ~GAME1_RENDERPANELS;
}

/// CUSTOM: cfg: Main::Clear
void LegoRR::Lego_SetDDrawClear(bool on)
{
	if (on) legoGlobs.flags1 |= GAME1_DDRAWCLEAR;
	else    legoGlobs.flags1 &= ~GAME1_DDRAWCLEAR;
}


void LegoRR::Lego_SetMusicOn(bool on)
{
	// I dislike these two flags so much...
	if (Lego_GetLevel() == nullptr || Objective_IsShowing()) {
		// Music isn't supposed to play when in the front end or the objective is showing.
		if (on) {
			// Don't turn the music off here if it is playing for some reason. 
			// Let that be handled elsewhere by the responsible functions.
			if (!Lego_IsMusicPlaying())
				legoGlobs.flags2 |= GAME2_MUSICREADY;  // Ready, since we're not playing/not in-game.
			else
				legoGlobs.flags2 &= ~GAME2_MUSICREADY; // Not ready, since we're currently playing.
		}
		else {
			if (Lego_IsMusicPlaying())
				Lego_SetMusicPlaying(false);

			legoGlobs.flags2 &= ~GAME2_MUSICREADY; // Not ready, since we're turned off.
		}
	}
	else {
		// Music is only supposed to play when we're in a level (and an objective isn't showing).
		if (on) {
			if (!Lego_IsMusicPlaying())
				Lego_SetMusicPlaying(true);

			legoGlobs.flags2 &= ~GAME2_MUSICREADY; // Not ready, since we're currently playing.
		}
		else {
			if (Lego_IsMusicPlaying())
				Lego_SetMusicPlaying(false);

			legoGlobs.flags2 &= ~GAME2_MUSICREADY; // Not ready, since we're turned off.
		}
	}
}

void LegoRR::Lego_ChangeMusicPlaying(bool shouldPlay)
{
	if (!shouldPlay) {
		if (Lego_IsMusicPlaying()) {
			Lego_SetMusicPlaying(false);

			legoGlobs.flags2 |= GAME2_MUSICREADY;  // Ready, since we're not playing/not in-game.
		}
	}
	else {
		if (!Lego_IsMusicPlaying() && (legoGlobs.flags2 & GAME2_MUSICREADY)) {
			Lego_SetMusicPlaying(true);

			legoGlobs.flags2 &= ~GAME2_MUSICREADY; // Not ready, since we're currently playing.
		}
	}
}

// <inlined>
void LegoRR::Lego_SetDetailOn(bool on)
{
	if (on) legoGlobs.flags1 |= GAME1_USEDETAIL;
	else    legoGlobs.flags1 &= ~GAME1_USEDETAIL;
}

// <inlined>
void LegoRR::Lego_SetDynamicPM(bool on)
{
	if (on) legoGlobs.flags1 |= GAME1_DYNAMICPM;
	else    legoGlobs.flags1 &= ~GAME1_DYNAMICPM;
}

// <inlined>
void LegoRR::Lego_SetFPSMonitorOn(bool on)
{
	if (on) legoGlobs.flags1 |= GAME1_SHOWFPS;
	else    legoGlobs.flags1 &= ~GAME1_SHOWFPS;
}

// <inlined>
void LegoRR::Lego_SetMemoryMonitorOn(bool on)
{
	if (on) legoGlobs.flags1 |= GAME1_SHOWMEMORY;
	else    legoGlobs.flags1 &= ~GAME1_SHOWMEMORY;
}

// <inlined>
void LegoRR::Lego_SetNoNERPs(bool on)
{
	if (on) legoGlobs.flags1 |= GAME1_DEBUG_NONERPS;
	else    legoGlobs.flags1 &= ~GAME1_DEBUG_NONERPS;
}

// <inlined>
void LegoRR::Lego_SetOnlyBuildOnPaths(bool on)
{
	if (on) legoGlobs.flags1 |= GAME1_ONLYBUILDONPATHS;
	else    legoGlobs.flags1 &= ~GAME1_ONLYBUILDONPATHS;
}

// <inlined>
void LegoRR::Lego_SetNoclipOn(bool on)
{
	if (on) legoGlobs.flags1 |= GAME1_DEBUG_NOCLIP_FPS;
	else    legoGlobs.flags1 &= ~GAME1_DEBUG_NOCLIP_FPS;
}

// <inlined>
void LegoRR::Lego_SetAlwaysRockFall(bool on)
{
	if (on) legoGlobs.flags1 |= GAME1_ALWAYSROCKFALL;
	else    legoGlobs.flags1 &= ~GAME1_ALWAYSROCKFALL;
}

// <inlined>
void LegoRR::Lego_SetAllowRename(bool on)
{
	if (on) legoGlobs.flags2 |= GAME2_ALLOWRENAME;
	else    legoGlobs.flags2 &= ~GAME2_ALLOWRENAME;
}

// <inlined>
void LegoRR::Lego_SetGenerateSpiders(bool on)
{
	if (on) legoGlobs.flags2 |= GAME2_GENERATESPIDERS;
	else    legoGlobs.flags2 &= ~GAME2_GENERATESPIDERS;
}

// <inlined>
void LegoRR::Lego_SetNoAutoEat(bool on)
{
	if (on) legoGlobs.flags2 |= GAME2_NOAUTOEAT;
	else    legoGlobs.flags2 &= ~GAME2_NOAUTOEAT;
}

// <inlined>
void LegoRR::Lego_SetNoMultiSelect(bool on)
{
	if (on) legoGlobs.flags2 |= GAME2_NOMULTISELECT;
	else    legoGlobs.flags2 &= ~GAME2_NOMULTISELECT;
}



// <LegoRR.exe @0041f7a0>
void __cdecl LegoRR::Level_IncCrystalsStored(void)
{
	Level_AddCrystalsStored(1);
}

/// CUSTOM: Extension of Level_IncOreStored for units larger than 1.
void __cdecl LegoRR::Level_AddCrystalsStored(sint32 crystalAmount)
{
	if (legoGlobs.currLevel != nullptr) {
		legoGlobs.currLevel->crystals += crystalAmount;
	}
}

// <LegoRR.exe @0041f7b0>
void __cdecl LegoRR::Level_SubtractCrystalsStored(sint32 crystalAmount)
{
	if (legoGlobs.currLevel != nullptr) {
		legoGlobs.currLevel->crystals -= crystalAmount;
	}
}

// <LegoRR.exe @0041f7d0>
void __cdecl LegoRR::Level_AddCrystalsDrained(sint32 crystalDrainedAmount)
{
	if (legoGlobs.currLevel != nullptr) {
		legoGlobs.currLevel->crystalsDrained += crystalDrainedAmount;
	}
}

// <LegoRR.exe @0041f7f0>
void __cdecl LegoRR::Level_ResetCrystalsDrained(void)
{
	if (legoGlobs.currLevel != nullptr) {
		legoGlobs.currLevel->crystalsDrained = 0;
	}
}

// <LegoRR.exe @0041f810>
sint32 __cdecl LegoRR::Level_GetCrystalCount(bool32 includeDrained)
{
	if (legoGlobs.currLevel != nullptr) {
		sint32 crystals = legoGlobs.currLevel->crystals;
		if (!includeDrained) {
			// Exclude drained count.
			crystals -= legoGlobs.currLevel->crystalsDrained;
		}
		return crystals;
	}
	/// FIX APPLY: Non-junk return value when not in a level.
	return 0;
}

// <LegoRR.exe @0041f830>
sint32 __cdecl LegoRR::Level_GetOreCount(bool32 isProcessed)
{
	if (legoGlobs.currLevel != nullptr) {
		if (!isProcessed) {
			return legoGlobs.currLevel->ore;
		}
		else {
			return legoGlobs.currLevel->studs;
		}
	}
	/// FIX APPLY: Non-junk return value when not in a level.
	return 0;
}

// <LegoRR.exe @0041f850>
void __cdecl LegoRR::Level_AddStolenCrystals(sint32 stolenCrystalAmount)
{
	if (legoGlobs.currLevel != nullptr) {
		legoGlobs.currLevel->crystalsStolen += stolenCrystalAmount;
	}
}

// <LegoRR.exe @0041f870>
void __cdecl LegoRR::Lego_SetRadarNoTrackObject(bool32 noTrackObj)
{
	if (noTrackObj) {
		legoGlobs.flags1 |= GAME1_RADAR_NOTRACKOBJECT;
	}
	else {
		legoGlobs.flags1 &= ~GAME1_RADAR_NOTRACKOBJECT;
	}
}

// <LegoRR.exe @0041f8a0>
// inline Lego_IsNoclipOn

// <LegoRR.exe @0041f8b0>
void __cdecl LegoRR::Level_IncCrystalsPickedUp(void)
{
	if (legoGlobs.currLevel != nullptr) {
		legoGlobs.currLevel->crystalsPickedUp++;
	}
}

// <LegoRR.exe @0041f8c0>
void __cdecl LegoRR::Level_IncOrePickedUp(void)
{
	if (legoGlobs.currLevel != nullptr) {
		legoGlobs.currLevel->orePickedUp++;
	}
}

// <LegoRR.exe @0041f8d0>
void __cdecl LegoRR::Level_IncOreStored(bool32 isProcessed)
{
	Level_AddOreStored(isProcessed, 1);
}

/// CUSTOM: Extension of Level_IncOreStored for units larger than 1.
void __cdecl LegoRR::Level_AddOreStored(bool32 isProcessed, sint32 oreAmount)
{
	if (legoGlobs.currLevel != nullptr) {
		if (!isProcessed) {
			legoGlobs.currLevel->ore += oreAmount;
		}
		else {
			legoGlobs.currLevel->studs += oreAmount;
		}
	}
	if (!isProcessed) {
		Panel_CryOreSideBar_ChangeOreMeter(true, oreAmount);
	}
	else {
		Panel_CryOreSideBar_ChangeOreMeter(true, oreAmount * LEGO_STUDORECOUNT);
	}
}

// <LegoRR.exe @0041f910>
void __cdecl LegoRR::Level_SubtractOreStored(bool32 isProcessed, sint32 oreAmount)
{
	if (legoGlobs.currLevel != nullptr) {
		if (!isProcessed) {
			legoGlobs.currLevel->ore -= oreAmount;
		}
		else {
			legoGlobs.currLevel->studs -= oreAmount;
		}
	}
	if (!isProcessed) {
		Panel_CryOreSideBar_ChangeOreMeter(false, oreAmount);
	}
	else {
		/// FIX APPLY: Account for more than one stud being subtracted.
		Panel_CryOreSideBar_ChangeOreMeter(false, oreAmount * LEGO_STUDORECOUNT);
	}
}

// <LegoRR.exe @0041f950>
// Gods_Go

// <LegoRR.exe @0041f9b0>
void __cdecl LegoRR::Lego_QuitLevel(void)
{
	if (!(legoGlobs.flags1 & GAME1_LEVELENDING)) {
		legoGlobs.flags1 |= GAME1_LEVELENDING;

		rewardGlobs.current.items[Reward_Figures].numDestroyed = NERPsRuntime_GetPreviousLevelObjectsBuilt("Pilot", 0);

		Construction_DisableCryOreDrop(true); // Prevent tele'ed up objects from spawning their costs.
		LegoObject_SetLevelEnding(true);
		Teleporter_Start(OBJECT_TYPE_FLAG_VEHICLE,       0x1, 0x1);
		Teleporter_Start(OBJECT_TYPE_FLAG_MINIFIGURE,    0x1, 0x1);
		Teleporter_Start(OBJECT_TYPE_FLAG_BUILDING,      0x1, 0x1);
		Teleporter_Start(OBJECT_TYPE_FLAG_ELECTRICFENCE, 0x1, 0x1);

		Lego_SetPaused(false, false);
		Interface_BackToMain();

		if (panelGlobs.panelTable[Panel_Radar].flags & PANEL_FLAG_OPEN) {
			Panel_ToggleOpenClosed(Panel_Radar);
			Panel_ToggleOpenClosed(Panel_RadarFill);
		}
	}
}

// <LegoRR.exe @0041fa80>
// Lego_Initialise


// <LegoRR.exe @00423120>
void __cdecl LegoRR::Lego_HandleRenameInput(void)
{
	if (legoGlobs.renameInput != nullptr) {
		uint32 inputLength = std::strlen(legoGlobs.renameInput);

		if (Typing_IsKeyPressed(Keys::KEY_BACKSPACE)) {
			// Backspace the last character.
			if (inputLength > 0) {
				legoGlobs.renameInput[inputLength - 1] = '\0';
			}
		}
		else if (Typing_IsKeyReleased(Keys::KEY_RETURN) || Typing_IsKeyReleased(Keys::KEY_ESCAPE)) {
			// Accept rename input. (We cannot reasonably make this a keybind).
			legoGlobs.renameInput = nullptr;
			Lego_SetPaused(false, false);
		}
		else {
			// Type new characters.
			for (uint32 keyCode = 0; keyCode < INPUT_MAXKEYS; keyCode++) {

				if (inputLength >= OBJECT_MAXCUSTOMNAMECHARS)
					break;

				if (Typing_IsKeyPressed(keyCode)) {

					sint32 keyChar = Front_Input_GetKeyCharacter(keyCode);
					if (keyChar != 0) {
						/// TODO: Enhancement to allow lowercase letters?
						keyChar = std::toupper((sint32)keyChar);

						legoGlobs.renameInput[inputLength] = (char)keyChar;
						legoGlobs.renameInput[inputLength + 1] = '\0';
						inputLength++;
					}
				}
			}
		}

		// Required for using the Typing_ state macros.
		// Copy key states to our typing state array.
		// Then clear the real key states to prevent keybinds from triggering.
		std::memcpy(gamectrlGlobs.typingState_Map, Gods98::INPUT.Key_Map, sizeof(Gods98::INPUT.Key_Map));
		Input_ClearAllKeys();
		//std::memset(Gods98::INPUT.Key_Map, 0, sizeof(Gods98::INPUT.Key_Map));

		// Reset keybinds to prevent them from triggering during typing.
		Shortcuts::shortcutManager.Reset(false);
	}
}

// <LegoRR.exe @00423210>
// Lego_MainLoop


// <LegoRR.exe @00424660>
void __cdecl LegoRR::Lego_UpdateSceneFog(bool32 fogEnabled, real32 elapsed)
{
	if (legoGlobs.flags1 & GAME1_FOGCOLOURRGB) {
		Gods98::Container_EnableFog(fogEnabled);

		/// NOTE: Fog is already set to `FogColourRGB` during Lego_LoadLevel,
		///        so we only need to change it when we have `HighFogColourRGB`.
		if (fogEnabled && (legoGlobs.flags1 & GAME1_HIGHFOGCOLOURRGB)) {

			// (M_PI*2) float hex: 0x40c90fdb
			gamectrlGlobs.sceneFogDelta += (((M_PI*2.0f) / legoGlobs.FogRate) * elapsed);

			const real32 waveDelta = std::sin(gamectrlGlobs.sceneFogDelta); // ASM FSIN

			ColourRGBF fogRGB = legoGlobs.FogColourRGB; // Base channel values before range is added.
			for (uint32 i = 0; i < 3; i++) {
				const real32 range = (legoGlobs.HighFogColourRGB.channels[i] - legoGlobs.FogColourRGB.channels[i]) * 0.5f;
				fogRGB.channels[i] += range * waveDelta;
			}
			Gods98::Container_SetFogColour(fogRGB.red, fogRGB.green, fogRGB.blue);
		}
	}
}

// DRAW MODE: Only Draw API drawing calls can be used within this function.
// DATA: Gods98::Viewport* viewMain
// <LegoRR.exe @00424700>
bool32 __cdecl LegoRR::Lego_Callback_DrawObjectLaserTrackerBox(LegoObject* liveObj, void* pViewMain)
{
	Gods98::Viewport* viewMain = static_cast<Gods98::Viewport*>(pViewMain);

	if (liveObj->flags4 & LIVEOBJ4_LASERTRACKERMODE) {
		Lego_DrawObjectSelectionBox(liveObj, viewMain, 1.0f, 0.0f, 0.0f); // red
	}
	return false;
}

// DRAW MODE: Only Draw API drawing calls can be used within this function.
// <LegoRR.exe @00424740>
void __cdecl LegoRR::Lego_DrawAllLaserTrackerBoxes(Gods98::Viewport* viewMain)
{
	for (LegoObject* obj : objectListSet.EnumerateSkipUpgradeParts()) {
		Lego_Callback_DrawObjectLaserTrackerBox(obj, viewMain);
	}
	//LegoObject_RunThroughListsSkipUpgradeParts(Lego_Callback_DrawObjectLaserTrackerBox, viewMain);
}

/// CUSTOM: Isolate Draw API calls from Lego_DrawAllLaserTrackerBoxes.
void LegoRR::Lego_DrawAllLaserTrackerNames(Gods98::Viewport* viewMain)
{
	for (LegoObject* obj : objectListSet.EnumerateSkipUpgradeParts()) {
		if (obj->flags4 & LIVEOBJ4_LASERTRACKERMODE) {
			Lego_DrawObjectName(obj, viewMain);
		}
	}
}

// DRAW MODE: Only Draw API drawing calls can be used within this function.
// <LegoRR.exe @00424760>
void __cdecl LegoRR::Lego_DrawAllSelectedUnitBoxes(Gods98::Viewport* viewMain)
{
	// Primary selected unit has a green box.
	// All remaining selected units have a yellow box.
	const ColourRGBF PRIMARY   = { 0.0f, 1.0f, 0.0f }; // green
	const ColourRGBF SECONDARY = { 1.0f, 1.0f, 0.0f }; // yellow

	const uint32 unitCount = Message_GetNumSelectedUnits();
	LegoObject** units = Message_GetSelectedUnits();
	for (uint32 i = 0; i < unitCount; i++) {
		const ColourRGBF colour = (i == 0 ? PRIMARY : SECONDARY);
		
		Lego_DrawObjectSelectionBox(units[i], viewMain, colour.red, colour.green, colour.blue);
	}
}

/// CUSTOM: Isolate Draw API calls from Lego_DrawAllSelectedUnitBoxes.
void LegoRR::Lego_DrawAllSelectedUnitNames(Gods98::Viewport* viewMain)
{
	const uint32 unitCount = Message_GetNumSelectedUnits();
	LegoObject** units = Message_GetSelectedUnits();
	for (uint32 i = 0; i < unitCount; i++) {
		Lego_DrawObjectName(units[i], viewMain);
	}
}

// DRAW MODE: Only Draw API drawing calls can be used within this function.
// <LegoRR.exe @004247e0>
void __cdecl LegoRR::Lego_DrawObjectSelectionBox(LegoObject* liveObj, Gods98::Viewport* view, real32 r, real32 g, real32 b)
{
	if (LegoObject_IsHidden(liveObj))
		return; // Don't show for hidden objects.

	// Darken lines when interface is frozen.
	if (legoGlobs.flags1 & GAME1_FREEZEINTERFACE) {
		r *= 0.3f;
		g *= 0.3f;
		b *= 0.3f;
	}

	Vector3F wPos = { 0.0f }; // dummy init
	LegoObject_GetPosition(liveObj, &wPos.x, &wPos.y);
	wPos.z = Map3D_GetWorldZ(Lego_GetMap(), wPos.x, wPos.y);
	wPos.z -= StatsObject_GetCollHeight(liveObj) / 2.0f; // Raise Z to center of collision box.

	Point2F screenPt = { 0.0f }; // dummy init
	Gods98::Viewport_WorldToScreen(view, &screenPt, &wPos);
	if (screenPt.x >= 0.0f && screenPt.x <= Gods98::appWidth() &&
		screenPt.y >= 0.0f && screenPt.y <= Gods98::appHeight())
	{
		const Vector4F transform4d = { screenPt.x, screenPt.y, 0.0f, 1.0f };
		Vector3F wFromScreenPos;
		Gods98::Viewport_InverseTransform(view, &wFromScreenPos, &transform4d);

		Vector3F wDiff;
		Gods98::Maths_Vector3DSubtract(&wDiff, &wPos, &wFromScreenPos);
		Gods98::Maths_Vector3DNormalize(&wDiff);
		const Vector3F crossProdRhs = { 0.0f, 1.0f, 0.0f };
		Vector3F crossProd;
		Gods98::Maths_Vector3DCrossProduct(&crossProd, &wDiff, &crossProdRhs);
		Gods98::Maths_Vector3DNormalize(&crossProd);

		Vector3F wPos2;
		Gods98::Maths_RayEndPoint(&wPos2, &wPos, &crossProd, StatsObject_GetPickSphere(liveObj));
		//Gods98::Maths_Vector3DScale(&crossProd, &crossProd, StatsObject_GetPickSphere(liveObj));
		//Gods98::Maths_Vector3DAdd(&wPos2, &wPos, &crossProd);

		//Vector3F local_4c;
		//Point2F local_40[2];
		Point2F screenPt2; // local_40
		Gods98::Viewport_WorldToScreen(view, &screenPt2, &wPos2);
		const real32 dist = Gods98::Maths_Vector2DDistance(&screenPt, &screenPt2);
		const real32 radius = (dist / 2.5f);
		// Minimum side length of 3 pixels, plus extra length scaled by distance.
		real32 sideLength = 3.0f + (radius / 5.0f);

		// 1 __1b  2__ 3
		//  |         |
		// 0           3b
		//       .     
		// 7b  scrPt   4
		//  |__     __|
		// 7   6   5b  5
		const Point2F start = {
			(screenPt.x - radius),
			(screenPt.y - radius),
		};
		const Point2F end = {
			(screenPt.x + radius),
			(screenPt.y + radius),
		};
		const Point2F startSide = {
			(start.x + sideLength),
			(start.y + sideLength),
		};
		const Point2F endSide = {
			(end.x - sideLength),
			(end.y - sideLength),
		};

		//#if LEGO_DRAWFILL_SELECTIONBOXES
		// If we're already locked due to drawing a transparent selection box, then we may as well use the Draw API.
		if (!Gods98::Draw_IsLocked()) {

			Area2F window;

			sideLength += 1.0f; // +1 because drawing rectagles is different from how the Draw API does line endpoints.

			window = Area2F { start.x,   start.y,  1,   sideLength };
			Gods98::DirectDraw_ClearRGBF(&window, r, g, b);
			window = Area2F { start.x+1, start.y,  sideLength-1, 1 };
			Gods98::DirectDraw_ClearRGBF(&window, r, g, b);

			window = Area2F { endSide.x, start.y,  sideLength,   1 };
			Gods98::DirectDraw_ClearRGBF(&window, r, g, b);
			window = Area2F { end.x,   start.y+1,  1, sideLength-1 };
			Gods98::DirectDraw_ClearRGBF(&window, r, g, b);

			window = Area2F { end.x,   endSide.y,  1,   sideLength };
			Gods98::DirectDraw_ClearRGBF(&window, r, g, b);
			window = Area2F { endSide.x,   end.y,  sideLength-1, 1 };
			Gods98::DirectDraw_ClearRGBF(&window, r, g, b);

			window = Area2F { start.x,     end.y,  sideLength,   1 };
			Gods98::DirectDraw_ClearRGBF(&window, r, g, b);
			window = Area2F { start.x, endSide.y,  1, sideLength-1 };
			Gods98::DirectDraw_ClearRGBF(&window, r, g, b);
		}
		else {
		//#else

			Point2F linePts1[8] = { 0.0f }; // dummy inits
			Point2F linePts2[8] = { 0.0f };

			// Top-left corner.
			linePts1[0] = Point2F { start.x, startSide.y };
			linePts2[0] = Point2F { start.x,     start.y };
			linePts1[1] = Point2F { start.x+1,   start.y }; // +1 to skip overlapping pixel.
			linePts2[1] = Point2F { startSide.x, start.y };

			// Top-right corner.
			linePts1[2] = Point2F { endSide.x,   start.y };
			linePts2[2] = Point2F { end.x,       start.y };
			linePts1[3] = Point2F { end.x,     start.y+1 }; // +1 to skip overlapping pixel.
			linePts2[3] = Point2F { end.x,   startSide.y };

			// Bottom-right corner.
			linePts1[4] = Point2F { end.x,     endSide.y };
			linePts2[4] = Point2F { end.x,         end.y };
			linePts1[5] = Point2F { end.x-1,       end.y }; // -1 to skip overlapping pixel.
			linePts2[5] = Point2F { endSide.x,     end.y };

			// Bottom-left corner.
			linePts1[6] = Point2F { startSide.x,   end.y };
			linePts2[6] = Point2F { start.x,       end.y };
			linePts1[7] = Point2F { start.x,     end.y-1 }; // -1 to skip overlapping pixel.
			linePts2[7] = Point2F { start.x,   endSide.y };

			Gods98::Draw_LineListEx(linePts1, linePts2, 8, r, g, b, Gods98::DrawEffect::None);
		}
		//#endif

		/// CHANGE: Moved to separate Lego_DrawObjectName call.
		// Draw custom name if the unit has one.
		/*if (!(legoGlobs.flags1 & GAME1_FREEZEINTERFACE) &&
			liveObj->customName != nullptr && liveObj->customName[0] != '\0')
		{
			Gods98::Font* font = legoGlobs.fontToolTip;
			const sint32 x = static_cast<sint32>(screenPt.x - radius);
			const sint32 y = static_cast<sint32>(screenPt.y - radius) - Gods98::Font_GetHeight(font);

			/// FIX APPLY: Printf safety (use %s).
			Gods98::Font_PrintF(font, x, y, "%s", liveObj->customName);
		}*/
	}
}

/// CUSTOM: Isolate Draw API calls from Lego_DrawObjectSelectionBox.
void LegoRR::Lego_DrawObjectName(LegoObject* liveObj, Gods98::Viewport* view)
{
	if (LegoObject_IsHidden(liveObj))
		return; // Don't show for hidden objects.

	if (legoGlobs.flags1 & GAME1_FREEZEINTERFACE)
		return; // Don't draw names right now.

	if (liveObj->customName == nullptr || liveObj->customName[0] == '\0')
		return; // No custom name to draw.


	Vector3F wPos = { 0.0f }; // dummy init
	LegoObject_GetPosition(liveObj, &wPos.x, &wPos.y);
	wPos.z = Map3D_GetWorldZ(Lego_GetMap(), wPos.x, wPos.y);
	wPos.z -= StatsObject_GetCollHeight(liveObj) / 2.0f; // Raise Z to center of collision box.

	Point2F screenPt = { 0.0f }; // dummy init
	Gods98::Viewport_WorldToScreen(view, &screenPt, &wPos);
	if (screenPt.x >= 0.0f && screenPt.x <= Gods98::appWidth() &&
		screenPt.y >= 0.0f && screenPt.y <= Gods98::appHeight())
	{
		const Vector4F transform4d = { screenPt.x, screenPt.y, 0.0f, 1.0f };
		Vector3F wFromScreenPos;
		Gods98::Viewport_InverseTransform(view, &wFromScreenPos, &transform4d);

		Vector3F wDiff;
		Gods98::Maths_Vector3DSubtract(&wDiff, &wPos, &wFromScreenPos);
		Gods98::Maths_Vector3DNormalize(&wDiff);
		const Vector3F crossProdRhs = { 0.0f, 1.0f, 0.0f };
		Vector3F crossProd;
		Gods98::Maths_Vector3DCrossProduct(&crossProd, &wDiff, &crossProdRhs);
		Gods98::Maths_Vector3DNormalize(&crossProd);

		Vector3F wPos2;
		Gods98::Maths_RayEndPoint(&wPos2, &wPos, &crossProd, StatsObject_GetPickSphere(liveObj));
		//Gods98::Maths_Vector3DScale(&crossProd, &crossProd, StatsObject_GetPickSphere(liveObj));
		//Gods98::Maths_Vector3DAdd(&wPos2, &wPos, &crossProd);

		//Vector3F local_4c;
		//Point2F local_40[2];
		Point2F screenPt2; // local_40
		Gods98::Viewport_WorldToScreen(view, &screenPt2, &wPos2);
		const real32 dist = Gods98::Maths_Vector2DDistance(&screenPt, &screenPt2);
		const real32 radius = (dist / 2.5f);

		// Draw custom name if the unit has one.
		Gods98::Font* font = legoGlobs.fontToolTip;
		const sint32 x = static_cast<sint32>(screenPt.x - radius);
		const sint32 y = static_cast<sint32>(screenPt.y - radius) - Gods98::Font_GetHeight(font);

		/// FIX APPLY: Printf safety (use %s).
		Gods98::Font_PrintF(font, x, y, "%s", liveObj->customName);
	}
}

/// CUSTOM:
void LegoRR::Lego_BeginDrawSelectionBoxes()
{
	// We'll need to lock the surface if drawing a transparent multi-select box,
	//  or if not using drawfill for selection boxes.
	//#if !LEGO_DRAWFILL_SELECTIONBOXES
	if (!LEGO_DRAWFILL_SELECTIONBOXES || (Lego_IsTransparentMultiSelectBox() && (legoGlobs.flags1 & GAME1_MULTISELECTING))) {
		if (!Gods98::Draw_IsLocked()) {
			Gods98::Draw_Begin();
			Error_Warn(!Gods98::Draw_IsLocked(), "Lego_BeginDrawSelectionBoxes: Draw_Begin() failed to lock the surface");
		}
	}
	//#endif
}

/// CUSTOM:
void LegoRR::Lego_EndDrawSelectionBoxes()
{
	//#if !LEGO_DRAWFILL_SELECTIONBOXES
	if (Gods98::Draw_IsLocked()) {
		Gods98::Draw_End();
	}
	//#endif
}


// DRAW MODE: Only Draw API drawing calls can be used within this function.
// <LegoRR.exe @00426180>
void __cdecl LegoRR::Lego_DrawRadarMap(void)
{
	if (Camera_GetTrackObject(legoGlobs.cameraTrack) == nullptr) {
		legoGlobs.flags1 |= GAME1_RADAR_TRACKOBJECTLOST;
	}

	Point2F radarCenterPos = { 0.0f }; // dummy init
	if (!(legoGlobs.flags1 & GAME1_RADAR_TRACKOBJECTLOST)) {
		LegoObject_GetPosition(Camera_GetTrackObject(legoGlobs.cameraTrack), &radarCenterPos.x, &radarCenterPos.y);
	}
	else {
		radarCenterPos = legoGlobs.radarCenter;
	}

	RadarMap_SetZoom(Lego_GetRadarMap(), legoGlobs.radarZoom);
	RadarMap_Draw(Lego_GetRadarMap(), &radarCenterPos);
}


LegoRR::ToolTip_Type LegoRR::Lego_PrepareObjectToolTip(LegoObject* liveObj)
{
	/// FIX APPLY: Increase horribly small buffer sizes
	char buffVal[TOOLTIP_BUFFERSIZE]; //[128];
	char buffText[TOOLTIP_BUFFERSIZE * 4] = { '\0' }; //[256]; // x4 so that we can safely cap it at TOOLTIP_BUFFERSIZE before calling ToolTip_SetContent


	const bool debugToolTips = (legoGlobs.flags2 & GAME2_SHOWDEBUGTOOLTIPS);


	const char* langName = LegoObject_GetLangName(liveObj);
	if (debugToolTips && (langName == nullptr || langName[0] == '\0')) {
		// (Debug only) If no language name is defined (or name is empty),
		//               then get the internal object Type/ID name.
		langName = Debug_GetObjectIDName(liveObj->type, liveObj->id);
	}

	// Only show a tooltip if we have a name to display for this object.
	if (langName != nullptr && langName[0] != '\0') {

		// Object name:
		if (liveObj->customName != nullptr && liveObj->customName[0] == '\0') {
			// Remove empty (but defined) custom names ahead of time.
			LegoObject_SetCustomName(liveObj, nullptr);
		}
		// Use custom name instead of langName, if we have one.
		const char* objName = ((liveObj->customName != nullptr) ? liveObj->customName : langName);
		std::strcpy(buffText, objName);


		// Upgrade level:
		const char* upgradeName = legoGlobs.langUpgradeLevel_name[liveObj->objLevel];
		if (debugToolTips || (liveObj->type == LegoObject_Building && upgradeName != nullptr)) {
			if (liveObj->type == LegoObject_Vehicle) {
				// (Debug only) Show level as bits for vehicles (1 bit per upgrade type).
				//              But don't show level 0.
				if (liveObj->objLevel != 0) {
					std::sprintf(buffVal, " (UPG%i%i%i%i)",
								 ((liveObj->objLevel & UPGRADE_FLAG_CARRY) ? 1 : 0),
								 ((liveObj->objLevel & UPGRADE_FLAG_SCAN)  ? 1 : 0),
								 ((liveObj->objLevel & UPGRADE_FLAG_SPEED) ? 1 : 0),
								 ((liveObj->objLevel & UPGRADE_FLAG_DRILL) ? 1 : 0));
					std::strcat(buffText, buffVal);
				}
			}
			else if (upgradeName == nullptr) {
				// (Debug only) Show generic upgrade text, when none is defined.
				//              But don't show level 0.
				if (liveObj->objLevel != 0) {
					std::sprintf(buffVal, " (UP%i)", liveObj->objLevel);
					std::strcat(buffText, buffVal);
				}
			}
			else {
				// Only buildings openly display their upgrade level: (UP1), (UP2), etc...
				std::sprintf(buffVal, " (%s)", upgradeName);
				std::strcat(buffText, buffVal);
			}
		}

		// World position:
		if (debugToolTips) {
			real32 xPos, yPos, zPos;
			LegoObject_GetPosition(liveObj, &xPos, &yPos);
			if (liveObj->type == LegoObject_MiniFigure) {
				zPos = LegoObject_GetWorldZCallback_Lake(xPos, yPos, Lego_GetMap());
			}
			else {
				zPos = LegoObject_GetWorldZCallback(xPos, yPos, Lego_GetMap());
			}
			std::sprintf(buffVal, " (%0.1f,%0.1f,%0.1f)", xPos, yPos, zPos);
			std::strcat(buffText, buffVal);
		}

		// Health and Energy:
		if (liveObj->flags3 & LIVEOBJ3_CANDAMAGE) {
			const uint32 health = ((liveObj->health >= 0.0f) ? (uint32)liveObj->health : 0);
			const uint32 energy = ((liveObj->energy >= 0.0f) ? (uint32)liveObj->energy : 0);

			if (debugToolTips) {
				// Under non-debug circumstances, the game would run fine without this text being defined.
				const char* healthToolTip = legoGlobs.langHealth_toolTip;
				if (healthToolTip == nullptr) healthToolTip = "Health";

				// (Debug only) Always show health and energy.
				std::sprintf(buffVal, "\n%s: %i", healthToolTip, health);
				std::strcat(buffText, buffVal);

				std::sprintf(buffVal, "\n%s: %i", legoGlobs.langEnergy_toolTip, energy);
				std::strcat(buffText, buffVal);
			}
			else if (DamageFont_LiveObject_CheckCanShowDamage_Unk(liveObj) &&
					 (liveObj->type != LegoObject_MiniFigure && liveObj->type != LegoObject_RockMonster))
			{
				// Vehicles, buildings, etc. show "Energy" as their health.
				std::sprintf(buffVal, "\n%s: %i", legoGlobs.langEnergy_toolTip, health);
				std::strcat(buffText, buffVal);
			}
		}

		// Cargo count:
		if (debugToolTips && liveObj->numCarriedObjects != 0) {
			std::sprintf(buffVal, "\nCargo: %i", liveObj->numCarriedObjects);
			std::strcat(buffText, buffVal);
		}

		// Driver custom name:
		if (liveObj->type == LegoObject_Vehicle && liveObj->driveObject != nullptr &&
			(liveObj->driveObject->customName != nullptr && liveObj->driveObject->customName[0] != '\0') &&
			legoGlobs.langDrivenBy_toolTip != nullptr)
		{
			// Only show "Driven by:" text for drivers with custom names.
			std::sprintf(buffVal, "\n%s: %s", legoGlobs.langDrivenBy_toolTip, liveObj->driveObject->customName);
			std::strcat(buffText, buffVal);
		}

		// Task and Activity:
		if (debugToolTips) {
			if (liveObj->aiTask != nullptr) {
				//const char* taskName = AITask_GetTaskName(liveObj->aiTask);
				const char* taskName = aiGlobs.taskName[liveObj->aiTask->taskType];
				const char* prioName = aiGlobs.priorityName[liveObj->aiTask->priorityType];
				
				std::sprintf(buffVal, "\nTask: %s", taskName + std::strlen("AITask_Type_"));
				std::strcat(buffText, buffVal);
				
				std::sprintf(buffVal, "\nPriority: %s", prioName + std::strlen("AI_Priority_"));
				std::strcat(buffText, buffVal);
			}
			if (liveObj->activityName1 != nullptr) {
				std::sprintf(buffVal, "\nActivity: %s", liveObj->activityName1 + std::strlen("Activity_"));
				std::strcat(buffText, buffVal);
			}
		}

		// Routing blocks:
		if (debugToolTips && liveObj->routeBlocks != nullptr && liveObj->routeBlocksTotal != 0) {
			std::sprintf(buffVal, "\nRouting: %i/%i", liveObj->routeBlocksCurrent, liveObj->routeBlocksTotal);
			std::strcat(buffText, buffVal);
		}

		// Record object ID:
		if (debugToolTips) {
			for (uint32 i = 0; i < legoGlobs.recordObjsCount; i++) {
				LegoObject* recordObj = legoGlobs.recordObjs[i];
				if (recordObj == liveObj) {
					std::sprintf(buffVal, "\nRecord: %i", (uint32)i);
					std::strcat(buffText, buffVal);
					break;
				}
			}
		}

		// Object flags: (consider another setting to remove this verbose info)
		if (debugToolTips) {
			std::sprintf(buffVal, "\nFlags1: %08x\nFlags2: %08x\nFlags3: %08x\nFlags4: %08x",
						 (uint32)liveObj->flags1,
						 (uint32)liveObj->flags2,
						 (uint32)liveObj->flags3,
						 (uint32)liveObj->flags4);
			std::strcat(buffText, buffVal);
		}


		/// SANITY: Until we increase the tooltip buffer size, ensure we don't exceed 511 characters.
		if (std::strlen(buffText) >= TOOLTIP_BUFFERSIZE) {
			std::strcpy(&buffText[TOOLTIP_BUFFERSIZE-4], "...");
		}
		ToolTip_SetContent(ToolTip_UnitSelect, buffText);

		const SFX_ID objTtSFX = LegoObject_GetObjTtSFX(liveObj);
		ToolTip_SetSFX(ToolTip_UnitSelect, objTtSFX);


		// Carried tools and Trained abilities icons:
		if (liveObj->type == LegoObject_MiniFigure) {
			// Only minifigures have tools and abilities to display.

			// Carried tools:
			{
				uint32 i = 0;
				// Draw equipped tool icons:
				for (; i < liveObj->numCarriedTools; i++) {
					const LegoObject_ToolType toolType = liveObj->carriedTools[i];
					if (objectGlobs.ToolTipIcons_Tools[toolType] != nullptr) {
						ToolTip_AddIcon(ToolTip_UnitSelect, objectGlobs.ToolTipIcons_Tools[toolType]);
					}
				}
				// Draw blank icons for the remaining space the unit has available.
				for (; i < StatsObject_GetNumOfToolsCanCarry(liveObj); i++) {
					if (objectGlobs.ToolTipIcon_Blank != nullptr) {
						ToolTip_AddIcon(ToolTip_UnitSelect, objectGlobs.ToolTipIcon_Blank);
					}
				}
			}

			// Trained abilities:
			if (liveObj->abilityFlags != ABILITY_FLAG_NONE) {
				// Show ability icons, only when unit has been trained in at least one.

				// I think this nullptr is used here to add a new row (to show abilities below tool icons).
				ToolTip_AddIcon(ToolTip_UnitSelect, nullptr);

				for (uint32 i = 0; i < LegoObject_AbilityType_Count; i++) {
					Gods98::Image* abilityIcon;
					if (liveObj->abilityFlags & (0x1<<i))
						abilityIcon = objectGlobs.ToolTipIcons_Abilities[i];
					else
						abilityIcon = objectGlobs.ToolTipIcon_Blank;

					if (abilityIcon != nullptr) {
						ToolTip_AddIcon(ToolTip_UnitSelect, abilityIcon);
					}
				}
			}
		}

		// Something like flushing the tooltip buffer, now that its ready(?)
		ToolTip_Activate(ToolTip_UnitSelect);

		return ToolTip_UnitSelect;
	}
	return ToolTip_Type_Count; // invalid
}


// <LegoRR.exe @00427f50>
void __cdecl LegoRR::Lego_ShowObjectToolTip(LegoObject* liveObj)
{
	const bool debugToolTips = (legoGlobs.flags2 & GAME2_SHOWDEBUGTOOLTIPS);


	const char* langName = LegoObject_GetLangName(liveObj);
	if (debugToolTips && (langName == nullptr || langName[0] == '\0')) {
		// (Debug only) If no language name is defined (or name is empty),
		//               then get the internal object Type/ID name.
		langName = Debug_GetObjectIDName(liveObj->type, liveObj->id);
	}

	// Only show a tooltip if we have a name to display for this object.
	if (langName != nullptr && langName[0] != '\0') {

		// Not the current toolTip object, abort, and let the next update loop handle this object.
		if (gamectrlGlobs.toolTipObject != liveObj) {
			gamectrlGlobs.toolTipObject = liveObj;
			return;
		}

		Lego_PrepareObjectToolTip(liveObj);
	}
}

LegoRR::ToolTip_Type LegoRR::Lego_PrepareConstructionToolTip(const Point2I* blockPos, bool32 showConstruction)
{
	/// FIX APPLY: Increase horribly small buffer sizes
	char buffVal[TOOLTIP_BUFFERSIZE]; //[128];
	char buffText[TOOLTIP_BUFFERSIZE * 4] = { '\0' }; //[128]; // x4 so that we can safely cap it at TOOLTIP_BUFFERSIZE before calling ToolTip_SetContent


	const bool debugToolTips = (legoGlobs.flags2 & GAME2_SHOWDEBUGTOOLTIPS);


	const Lego_Block* block = &blockValue(Lego_GetLevel(), blockPos->x, blockPos->y);
	Construction_Zone* construct = block->construct;

	// Building name:
	const char* objName = Object_GetLangName(LegoObject_Building, construct->objID);
	std::strcat(buffText, objName);


	// Resource progress:
	if (showConstruction) {
		/// CHANGE: Use Construction_Zone functions to get resource costs,
		///          which is important now that we have the 'No Build Costs' cheat.

		bool useStuds;
		uint32 crystals, oreType;
		const uint32 crystalsCost = Construction_Zone_GetCostCrystal(construct, &crystals);
		const uint32 oreTypeCost = Construction_Zone_GetCostOreType(construct, &oreType, &useStuds);

		// Crystals progress:
		if (crystalsCost > 0) {
			std::sprintf(buffVal, "\n%s: %i/%i", legoGlobs.langCrystals_toolTip, crystals, crystalsCost);
			std::strcat(buffText, buffVal);
		}

		const char* oreTypeToolTip = (useStuds ? legoGlobs.langStuds_toolTip : legoGlobs.langOre_toolTip);
		
		// Ore / Studs progress:
		if (oreTypeCost > 0) {
			std::sprintf(buffVal, "\n%s: %i/%i", oreTypeToolTip, oreType, oreTypeCost);
			std::strcat(buffText, buffVal);
		}

		// (Debug only) Barriers progress:
		if (debugToolTips) {
			uint32 barriers;
			const uint32 barriersCost = Construction_Zone_GetCostBarriers(construct, &barriers);

			if (barriersCost > 0) {
				const char* barrierName = Object_GetLangName(LegoObject_Barrier, (LegoObject_ID)0);
				if (barrierName == nullptr || barrierName[0] == '\0') {
					barrierName = Debug_GetObjectIDName(LegoObject_Barrier, (LegoObject_ID)0);
				}

				if (barrierName != nullptr && barrierName[0] != '\0') {
					std::sprintf(buffVal, "\n%s: %i/%i", barrierName, barriers, barriersCost);
					std::strcat(buffText, buffVal);
				}
			}
		}

		// (Debug only) Foreign objects obstructing:
		if (debugToolTips) {
			bool obstructed = !Construction_Zone_NoForeignObjectsInside(construct);
			std::sprintf(buffVal, "\nObstructed: %s", (obstructed ? "Yes": "No"));
			std::strcat(buffText, buffVal);
		}
	}


	/// SANITY: Until we increase the tooltip buffer size, ensure we don't exceed 511 characters.
	if (std::strlen(buffText) >= TOOLTIP_BUFFERSIZE) {
		std::strcpy(&buffText[TOOLTIP_BUFFERSIZE-4], "...");
	}
	ToolTip_SetContent(ToolTip_Construction, buffText);

	ToolTip_SetSFX(ToolTip_Construction, SFX_NULL);
	ToolTip_Activate(ToolTip_Construction);
	return ToolTip_Construction;
}

LegoRR::ToolTip_Type LegoRR::Lego_PrepareMapBlockToolTip(const Point2I* blockPos, bool32 playSound, bool32 showCavern)
{
	/// FIX APPLY: Increase horribly small buffer sizes
	// Originally these buffers were only used for Construction.
	char buffVal[TOOLTIP_BUFFERSIZE]; //[128];
	char buffText[TOOLTIP_BUFFERSIZE * 4] = { '\0' }; //[128]; // x4 so that we can safely cap it at TOOLTIP_BUFFERSIZE before calling ToolTip_SetContent


	const bool debugToolTips = (legoGlobs.flags2 & GAME2_SHOWDEBUGTOOLTIPS);


	const Lego_Block* block = &blockValue(Lego_GetLevel(), blockPos->x, blockPos->y);

	Lego_SurfaceType surfType; // = Lego_SurfaceType_Tunnel; // dummy init (0)
	bool showToolTip = true; // default behaviour (only changed for solid building tiles)


	if (!(block->flags1 & BLOCK1_FLOOR)) {
		// Wall types:
		if (!(block->flags1 & BLOCK1_WALL) && (block->flags1 & BLOCK1_HIDDEN)) {
			surfType = Lego_SurfaceType_Undiscovered; // Undiscovered cavern (not necessarily a wall type)
		}
		else if (block->flags1 & BLOCK1_REINFORCED) {
			/// UNINLINED: Level_Block_IsReinforced(blockPos->x, blockPos->y)
			surfType = Lego_SurfaceType_Reinforcement; // Reinforced wall (of any type).
		}
		else {
			surfType = (Lego_SurfaceType)block->terrain; // All other wall types
		}
	}
	else {
		// Floor types:
		if (block->terrain == Lego_SurfaceType_Lake || block->terrain == Lego_SurfaceType_Lava) {
			surfType = (Lego_SurfaceType)block->terrain; // Lake ("Water") and Lava (overrides all other floor type flags).
		}
		else if (block->flags1 & BLOCK1_ERODEACTIVE) {
			// Tunnel: (what exactly is this flag? Considering we have a second case for Tunnel below)
			//         Although possibly just an optimization. Flow for the second case jumps to here.
			surfType = Lego_SurfaceType_Tunnel;
		}
		else if (!(block->flags1 & BLOCK1_CLEARED)) {
			/// UNINLINED: Level_Block_GetRubbleLayers(blockPos) != 0
			surfType = Lego_SurfaceType_Rubble; // Uncleared rubble
		}
		else if (block->flags2 & BLOCK2_SLUGHOLE_EXPOSED) {
			surfType = Lego_SurfaceType_SlugHole; // Slimy slug hole
		}
		else if (Level_Block_IsPath(blockPos) || Level_Block_IsPathBuilding(blockPos)) {
			/// INLINED: (block->flags1 & (BLOCK1_PATH|BLOCK1_BUILDINGPATH))
			surfType = Lego_SurfaceType_Path; // Power path
		}
		else if (Level_Block_IsSolidBuilding(blockPos->x, blockPos->y, true) || (block->flags1 & BLOCK1_FOUNDATION)) {
			/// INLINED: (block->flags1 & (BLOCK1_BUILDINGSOLID|BLOCK1_FOUNDATION)) || (block->flags2 & BLOCK2_TOOLSTORE)
			showToolTip = false; // Core building tile: DON'T SHOW TOOLTIP!
		}
		else if (showCavern) {
			surfType = Lego_SurfaceType_Cavern; // Catch all for Ground
		}
		else {
			surfType = Lego_SurfaceType_Tunnel; // Catch all for Roof(?)
		}
	}
	//if (showCavern == 0) goto LAB_004285a8;


	if (showToolTip) {
		const char* surfName = Lego_GetSurfaceTypeDescription(surfType);
		SFX_ID surfSFX = Lego_GetSurfaceTypeSFX(surfType);

		// Block surface name:
		std::strcat(buffText, surfName);

		if (debugToolTips) {
			// Block coordinates:
			std::sprintf(buffVal, " (%i,%i)", blockPos->x, blockPos->y);
			std::strcat(buffText, buffVal);


			// Texture hex ID (and direction):
			static constexpr const std::array<char, 4> directionChars = { 'U', 'R', 'D', 'L' };
			std::sprintf(buffVal, "\nTex: %02x, Dir: %c", (uint32)block->texture, directionChars[block->direction % directionChars.size()]);
			std::strcat(buffText, buffVal);
			
			// Wall damage:
			if (Level_Block_IsWall(blockPos->x, blockPos->y)) {
				const uint32 damage = ((block->damage >= 0.0f) ? (uint32)(block->damage*100) : 0);

				std::sprintf(buffVal, "\nDamage: %i%%", damage); // Percentage version
				//std::sprintf(buffVal, "\nnDamage: %0.2f", block->damage); // Ratio version
				std::strcat(buffText, buffVal);
			}

			// Rubble layers:
			const uint32 rubbleLayers = Level_Block_GetRubbleLayers(blockPos);
			if (!Level_Block_IsWall(blockPos->x, blockPos->y) && rubbleLayers != 0) {
				std::sprintf(buffVal, "\nLayers: %i/%i", rubbleLayers, 4); // 4 == Max rubble layers
				std::strcat(buffText, buffVal);
			}

			// Clicks:
			if (block->clickCount != 0) {
				std::sprintf(buffVal, "\nClicks: %i", (uint32)block->clickCount);
				std::strcat(buffText, buffVal);
			}

			// Landslides:
			if (block->numLandSlides != 0) {
				std::sprintf(buffVal, "\nLandslides: %i", (uint32)block->numLandSlides);
				std::strcat(buffText, buffVal);
			}

			// CryOre:
			// Sanity check for valid cryore count.
			// It's entirely possible to "safely" have this value out of range,
			//  but never have the count accessed because of not being in a wall block).
			uint32 cryOre[2][2] = { { 0 } }; // [cry,ore][lv0,lv1] dummy init
			if (block->cryOre != Lego_CryOreType_None && block->cryOre <= Lego_CryOreType_Ore_Lv1_25 &&
				Lego_GetBlockCryOre(blockPos, &cryOre[0][0], &cryOre[0][1], &cryOre[1][0], &cryOre[1][1]))
			{
				for (uint32 i = 0; i < 2; i++) { // Type
					const LegoObject_Type cryOreType = (i == 1 ? LegoObject_Ore : LegoObject_PowerCrystal);

					const char* cryOreToolTip = (cryOreType == LegoObject_Ore ? legoGlobs.langOre_toolTip : legoGlobs.langCrystals_toolTip);

					if (cryOreToolTip == nullptr || cryOreToolTip[0] == '\0') {
						cryOreToolTip = Object_GetLangName(cryOreType, (LegoObject_ID)0);
						if (cryOreToolTip == nullptr || cryOreToolTip[0] == '\0') {
							cryOreToolTip = Debug_GetObjectIDName(cryOreType, (LegoObject_ID)0);
						}
					}

					for (uint32 j = 0; j < 2; j++) { // Level
						const uint32 cryOreLevel = j;
						const uint32 cryOreCount = cryOre[i][j];
						
						if (cryOreCount != 0 && cryOreToolTip != nullptr && cryOreToolTip[0] != '\0') {
							std::sprintf(buffVal, "\n%s: %i", cryOreToolTip, cryOreCount);
							std::strcat(buffText, buffVal);
							if (cryOreLevel != 0) {
								std::sprintf(buffVal, " (L%i)", cryOreLevel);
								std::strcat(buffText, buffVal);
							}
						}
					}
				}
			}
			
			// Erosion:
			if (block->erodeSpeed != Lego_ErodeType_None) {// && block->terrain != Lego_SurfaceType_Lava) {
				bool erodeActive = false;
				for (uint32 i = 0; i < _countof(erodeGlobs.activeBlocks); i++) {
					const Point2I& activePos = erodeGlobs.activeBlocks[i];
					if (erodeGlobs.activeStates[i] && activePos.x == blockPos->x && activePos.y == blockPos->y) {
						erodeActive = true;
						break;
					}
				}
				// Include "None" as dummy to keep array index lookup the same as erodeSpeed.
				static constexpr const std::array<const char*, 6> erodeSpeedNames = { "None", "VSlow", "Slow", "Med", "Fast", "VFast" };
				const char* erodeActiveName = (erodeActive ? "Active" : "Dormant");
				// Casts are vital here, since values are stored in `uint8`.
				if (block->erodeSpeed >= erodeSpeedNames.size()) {
					std::sprintf(buffVal, "\nErode: %i", (uint32)block->erodeSpeed);
					std::strcat(buffText, buffVal);
				}
				else {
					std::sprintf(buffVal, "\nErode: %s", erodeSpeedNames[block->erodeSpeed]);
					std::strcat(buffText, buffVal);
				}
				std::sprintf(buffVal, ", %s, %i/%i", erodeActiveName, (uint32)block->erodeLevel, 4); // Progress 4 is lava
				std::strcat(buffText, buffVal);
			}

			// Emerge:
			if (Lego_GetLevel()->emergeTriggers != nullptr) {
				for (uint32 i = 0; i < Lego_GetLevel()->emergeCount; i++) {
					auto& emergeTrigger = Lego_GetLevel()->emergeTriggers[i];
					if (emergeTrigger.blockPos.x == blockPos->x && emergeTrigger.blockPos.y == blockPos->y) {
						std::sprintf(buffVal, "\nEmerge: %i, Trigger", i);
						std::strcat(buffText, buffVal);
						break;
					}
					else {
						bool found = false;
						for (uint32 j = 0; j < _countof(emergeTrigger.emergePoints); j++) {
							auto& emergeBlock = emergeTrigger.emergePoints[j];
							if (emergeBlock.used && emergeBlock.blockPos.x == blockPos->x && emergeBlock.blockPos.y == blockPos->y) {
								found = true;
								std::sprintf(buffVal, "\nEmerge: %i, Point %i", i, j+1);
								std::strcat(buffText, buffVal);
								break;
							}
						}
						if (found) {
							break;
						}
					}
				}
			}

			// Block pointer ID:
			if (block->blockpointer != 0) {
				std::sprintf(buffVal, "\nPointer: %i", (uint32)block->blockpointer);
				std::strcat(buffText, buffVal);
			}

			// Block flags: (consider another setting to remove this verbose info)
			{
				if (block->tutoHighlightState != 0 || block->seamDigCount != 0) {
					std::sprintf(buffVal, "\nTuto Hilite: %i\nSeam Digs: %i",
									 (uint32)block->tutoHighlightState,
									 (uint32)block->seamDigCount);
					std::strcat(buffText, buffVal);
				}

				std::sprintf(buffVal, "\nFlags1: %08x\nFlags2: %08x",
								(uint32)block->flags1,
								(uint32)block->flags2);
				std::strcat(buffText, buffVal);
			}
		}


		/// SANITY: Until we increase the tooltip buffer size, ensure we don't exceed 511 characters.
		if (std::strlen(buffText) >= TOOLTIP_BUFFERSIZE) {
			std::strcpy(&buffText[TOOLTIP_BUFFERSIZE-4], "...");
		}
		ToolTip_SetContent(ToolTip_MapBlock, "%s", buffText);

		if (!playSound) surfSFX = SFX_NULL;
		ToolTip_SetSFX(ToolTip_MapBlock, surfSFX);
		ToolTip_Activate(ToolTip_MapBlock);
		return ToolTip_MapBlock;
	}
	return ToolTip_Type_Count; // invalid
}

LegoRR::ToolTip_Type LegoRR::Lego_PrepareBlockToolTip(const Point2I* blockPos, bool32 showConstruction, bool32 playSound, bool32 showCavern)
{
	const Lego_Block* block = &blockValue(Lego_GetLevel(), blockPos->x, blockPos->y);

	if ((block->flags1 & BLOCK1_FOUNDATION) && !Level_Block_IsSolidBuilding(blockPos->x, blockPos->y, true) &&
		!Level_Block_IsPathBuilding(blockPos))
	{
		/// INLINED: (block->flags1 & BLOCK1_FOUNDATION) && !(block->flags1 & (BLOCK1_BUILDINGSOLID|BLOCK1_BUILDINGPATH)) &&
		///          !(block->flags2 & BLOCK2_TOOLSTORE)

		// Construction barrier-ed area:
		return Lego_PrepareConstructionToolTip(blockPos, showConstruction);
	}
	else {
		// Map block:
		return Lego_PrepareMapBlockToolTip(blockPos, playSound, showCavern);
	}
}

// <LegoRR.exe @00428260>
void __cdecl LegoRR::Lego_ShowBlockToolTip(const Point2I* blockPos, bool32 showConstruction, bool32 playSound, bool32 showCavern)
{
	// Not the current toolTip block, abort, and let the next update loop handle this block.
	if (s_ShowBlockToolTip_MousePos.x != blockPos->x || s_ShowBlockToolTip_MousePos.y != blockPos->y) {
		s_ShowBlockToolTip_MousePos = *blockPos;
		return;
	}

	Lego_PrepareBlockToolTip(blockPos, showConstruction, playSound, showCavern);
}


// mbx,mby : mouse-over block position.
// mouseOverObj: mouse-over object.
// <LegoRR.exe @00428810>
void __cdecl LegoRR::Lego_HandleWorldDebugKeys(sint32 mbx, sint32 mby, LegoObject* mouseOverObj, real32 noMouseButtonsElapsed)
{
	const Point2I mouseBlockPos = { mbx, mby };

	/// DEBUG KEYBIND: [A]  "Creates a landslide at mousepoint."
	// The DIRECTION_FLAG_N here is probably why the landslide debug key is so finicky.
	if (Shortcut_IsPressed(ShortcutID::Debug_CreateLandslide)) {
		Fallin_Block_FUN_0040f260(&mouseBlockPos, DIRECTION_FLAG_N, true);
	}
	/// DEBUG KEYBIND: [A]  "Causes unit at mousepoint to slip."
	if (mouseOverObj != nullptr && Shortcut_IsPressed(ShortcutID::Debug_TripUnit)) {
		LegoObject_DoSlip(mouseOverObj);
	}

	/// DEBUG KEYBIND: [End]  "Toggles power Off/On for currently selected building."
	if (Message_AnyUnitSelected() && Shortcut_IsPressed(ShortcutID::Debug_ToggleSelfPowered)) {
		StatsObject_Debug_ToggleSelfPowered(Message_GetPrimarySelectedUnit());
	}

	/// DEBUG KEYBIND: [E]  "Makes a monster or slug emerge from a valid spawn wall/hole at mousepoint."
	if (Shortcut_IsPressed(ShortcutID::Debug_EmergeMonster)) {
		// First see if a slughole is at mousepoint, and try to emerge a slug.
		LegoObject* emergeObj = nullptr;
		LegoObject_Type slugType = LegoObject_None; // dummy inits
		LegoObject_ID slugID = LegoObject_ID_Invalid;
		ObjectModel* slugModel = nullptr;
		// Oh joy, only one legacy level even uses this property, everywhere else, Slug is hardcoded...
		if (legoGlobs.currLevel->Slug != LegoObject_ID_Invalid) {
			slugType = LegoObject_RockMonster;
			slugID = legoGlobs.currLevel->Slug;
			slugModel = &legoGlobs.rockMonsterData[legoGlobs.currLevel->Slug];
		}
		else {
			if (!Lego_GetObjectByName("Slug", &slugType, &slugID, &slugModel))
				slugID = LegoObject_ID_Invalid;
		}
		if (slugID != LegoObject_ID_Invalid) {
			emergeObj = LegoObject_TryGenerateSlugAtBlock(slugModel,
														  slugType, slugID,
														  mbx, mby, 0.0f, false);
		}

		if (emergeObj == nullptr && legoGlobs.currLevel->EmergeCreature != LegoObject_ID_Invalid) {
			// This wasn't a slughole, try to emerge a monster.
			LegoObject_TryGenerateRMonster(&legoGlobs.rockMonsterData[legoGlobs.currLevel->EmergeCreature],
										   LegoObject_RockMonster, legoGlobs.currLevel->EmergeCreature, mbx, mby);
		}
	}

	/// DEBUG KEYBIND: [W]  "Performs unknown behaviour with the unfinished 'flood water' surface."
	if (Shortcut_IsDown(ShortcutID::Debug_Unknown_Water)) {
		Level_Debug_WKey_NeedsBlockFlags1_8_FUN_004303a0(legoGlobs.currLevel, 0, mbx, mby); // 0 = unused parameter
	}

	/// DEBUG KEYBIND: [C]  "Tell selected unit carrying dynamite drop it where they are and set it off."
	if (Message_AnyUnitSelected() && Shortcut_IsPressed(ShortcutID::Debug_CommandDropDynamite)) {
		LegoObject_Debug_DropActivateDynamite(Message_GetPrimarySelectedUnit());

		// Simply calling the same debug [C] key function again... no idea why.
		if (Message_AnyUnitSelected()) {
			LegoObject_Debug_DropActivateDynamite(Message_GetPrimarySelectedUnit());
		}
	}

	/// DEBUG KEYBIND: [F12]  "Disables all NERPs functions (toggle)."
	if (Shortcut_IsPressed(ShortcutID::Debug_ToggleNoNERPs)) {
		if (!(legoGlobs.flags1 & GAME1_DEBUG_NONERPS)) {
			legoGlobs.flags1 |= GAME1_DEBUG_NONERPS;
			// Clear tutorial flags to remove all tutorial restrictions.
			NERPs_SetTutorialFlags(TUTORIAL_FLAG_NONE);
			//TutorialFlags tutFlags = TUTORIAL_FLAG_NONE;
			//NERPFunc__SetTutorialFlags((sint32*)&tutFlags);
		}
		else {
			legoGlobs.flags1 &= ~GAME1_DEBUG_NONERPS;
		}
	}

	/// DEBUG KEYBIND: [F11]  "Disables all building and vehicle prerequisites."
	if (Shortcut_IsPressed(ShortcutID::Debug_ToggleBuildDependencies)) {
		Dependencies_SetEnabled(!Dependencies_IsEnabled());
	}

	/// DEBUG KEYBIND: [F10]  "Inverts the direction of lighting."
	if (Shortcut_IsPressed(ShortcutID::Debug_ToggleInvertLighting)) {
		gamectrlGlobs.dbgF10InvertLighting = !gamectrlGlobs.dbgF10InvertLighting;
		real32 dirZ;
		if (!gamectrlGlobs.dbgF10InvertLighting) {
			Gods98::Container_SetPosition(legoGlobs.spotlightTop, legoGlobs.cameraMain->cont3, 200.0f, 140.0f, -130.0f);
			dirZ = 0.75f;
		}
		else {
			Gods98::Container_SetPosition(legoGlobs.spotlightTop, legoGlobs.cameraMain->cont3, 250.0f, 190.0f, 20.0f);
			dirZ = 0.0f;
		}
		Gods98::Container_SetOrientation(legoGlobs.spotlightTop, legoGlobs.cameraMain->cont3, -1.0f, -0.8f, dirZ, 0.0f, 1.0f, 0.0f);
		LightEffects_InvalidatePosition();
	}

	/// DEBUG KEYBIND: [F9]  "Toggle spotlight effects."
	if (Shortcut_IsPressed(ShortcutID::Debug_ToggleSpotlightEffects)) {
		gamectrlGlobs.dbgF9DisableLightEffects = !gamectrlGlobs.dbgF9DisableLightEffects;
		LightEffects_SetDisabled(gamectrlGlobs.dbgF9DisableLightEffects);
	}

	/// DEBUG KEYBIND: [5]  "Change selected unit visual upgrade parts (Carry level bit)"
	/// DEBUG KEYBIND: [6]  "Change selected unit visual upgrade parts (Scan  level bit)"
	/// DEBUG KEYBIND: [7]  "Change selected unit visual upgrade parts (Speed level bit)"
	/// DEBUG KEYBIND: [8]  "Change selected unit visual upgrade parts (Drill level bit)"
	// Evaluate if we can perform an upgrade before checking keys (for future hotkey implementation).
	if (Message_AnyUnitSelected() && 
		(Message_GetPrimarySelectedUnit()->type == LegoObject_Building ||
		 Message_GetPrimarySelectedUnit()->type == LegoObject_Vehicle))
	{
		bool hotkeyUpgCarry = Shortcut_IsPressed(ShortcutID::Debug_ToggleUpgradeCarry);
		bool hotkeyUpgScan  = Shortcut_IsPressed(ShortcutID::Debug_ToggleUpgradeSpeed);
		bool hotkeyUpgSpeed = Shortcut_IsPressed(ShortcutID::Debug_ToggleUpgradeScan);
		bool hotkeyUpgDrill = Shortcut_IsPressed(ShortcutID::Debug_ToggleUpgradeDrill);
		if (hotkeyUpgCarry || hotkeyUpgScan || hotkeyUpgSpeed || hotkeyUpgDrill) {

			bool canUpgCarry = false, canUpgScan = false, canUpgSpeed = false, canUpgDrill = false; // dummy inits
			UpgradesModel* upgrades = nullptr; // Can't perform upgrades unless a check below for object type passes.

			LegoObject* primaryObj = Message_GetPrimarySelectedUnit();
			switch (primaryObj->type) {
			case LegoObject_Building:
				upgrades = &primaryObj->building->upgrades;

				canUpgCarry = Building_CanUpgradeType(primaryObj->building, LegoObject_UpgradeType_Carry, false);
				canUpgScan  = Building_CanUpgradeType(primaryObj->building, LegoObject_UpgradeType_Scan,  false);
				canUpgSpeed = Building_CanUpgradeType(primaryObj->building, LegoObject_UpgradeType_Speed, false);
				canUpgDrill = Building_CanUpgradeType(primaryObj->building, LegoObject_UpgradeType_Drill, false);
				break;

			case LegoObject_Vehicle:
				upgrades = &primaryObj->vehicle->upgrades;

				canUpgCarry = Vehicle_CanUpgradeType(primaryObj->vehicle, LegoObject_UpgradeType_Carry, false);
				canUpgScan  = Vehicle_CanUpgradeType(primaryObj->vehicle, LegoObject_UpgradeType_Scan,  false);
				canUpgSpeed = Vehicle_CanUpgradeType(primaryObj->vehicle, LegoObject_UpgradeType_Speed, false);
				canUpgDrill = Vehicle_CanUpgradeType(primaryObj->vehicle, LegoObject_UpgradeType_Drill, false);
				break;
			}

			if (upgrades != nullptr) {
				const LegoObject_UpgradeFlags oldUpgradeLvl = (LegoObject_UpgradeFlags)upgrades->currentLevel;
				LegoObject_UpgradeFlags newUpgradeLvl = oldUpgradeLvl;

				/// FIX APPLY: Upgrade hotkeys now check the correct 'CanUpgradeType'.
				///            Original order was reversed: canUpgDrill, canUpgSpeed, canUpgScan, canUpgCarry
				if (hotkeyUpgCarry && canUpgCarry) newUpgradeLvl ^= UPGRADE_FLAG_CARRY;
				if (hotkeyUpgScan  && canUpgScan)  newUpgradeLvl ^= UPGRADE_FLAG_SCAN;
				if (hotkeyUpgSpeed && canUpgSpeed) newUpgradeLvl ^= UPGRADE_FLAG_SPEED;
				if (hotkeyUpgDrill && canUpgDrill) newUpgradeLvl ^= UPGRADE_FLAG_DRILL;

				if (newUpgradeLvl != oldUpgradeLvl) {
					switch (primaryObj->type) {
					case LegoObject_Building:
						Building_SetUpgradeLevel(primaryObj->building, newUpgradeLvl);
						break;
					case LegoObject_Vehicle:
						Vehicle_SetUpgradeLevel(primaryObj->vehicle, newUpgradeLvl);
						break;
					}
				}
			}
		}
	}

	/// DEBUG KEYBIND: [Backspace]  ???
	if (Shortcut_IsPressed(ShortcutID::Debug_Unknown_Gather)) {
		AITask_DoGather_Count(5);
	}

	/// DEBUG KEYBIND: [Numpad Del]  "Destroys any walls at mousepoint, except border rock"
	if (Shortcut_IsPressed(ShortcutID::Debug_DestroyWall)) {
		Level_DestroyWall(legoGlobs.currLevel, mbx, mby, false);
	}

	/// DEBUG KEYBIND: [Numpad 3]  "Destroys connections between any walls at mousepoint, except border rock."
	if (Shortcut_IsDown(ShortcutID::Debug_DestroyWallConnection)) {
		Level_DestroyWallConnection(legoGlobs.currLevel, mbx, mby);
	}

	/// DEBUG KEYBIND: [W]  "Tell selected monster to immediately carry a created boulder."
	if (Message_AnyUnitSelected() && Message_GetPrimarySelectedUnit()->type == LegoObject_RockMonster &&
		Shortcut_IsPressed(ShortcutID::Debug_CommandGatherBoulder))
	{
		LegoObject* primaryObj = Message_GetPrimarySelectedUnit();
		sint32 nearest_bx = 0, nearest_by = 0; // dummy inits
		LegoObject_FindNearestWall(primaryObj, &nearest_bx, &nearest_by, true, false, false);
		LegoObject* boulderObj = LegoObject_Create(legoGlobs.contBoulder, LegoObject_Boulder, (LegoObject_ID)0);
		LegoObject_Hide(boulderObj, true);
		LegoObject_RoutingNoCarry_FUN_00447470(primaryObj, nearest_bx, nearest_by, boulderObj);
		Gods98::Container_Texture* contTexture = Detail_GetTexture(legoGlobs.currLevel->textureSet, blockValue(legoGlobs.currLevel, nearest_bx, nearest_by).texture);
		LegoObject_InitBoulderMesh_FUN_00440eb0(boulderObj, contTexture);
	}

	/// DEBUG KEYBIND: [N]  "Stops current routing for the primary selected unit."
	if (Message_AnyUnitSelected() && Shortcut_IsPressed(ShortcutID::Debug_StopRouting)) {
		LegoObject_Route_End(Message_GetPrimarySelectedUnit(), false);
	}

	/// DEBUG KEYBIND: [LShift]+[A]  "Tells any Rock Raider to get a Sonic Blaster from Tool Store and place at mousepoint."
	if (Shortcut_IsPressed(ShortcutID::Debug_CommandPlaceSonicBlaster)) {
		AITask_DoBirdScarer_AtPosition(&mouseBlockPos);
	}

	/// DEBUG KEYBIND: [B]  "Pushes (bumps) any Rock Raider or Monster at mousepoint east-northeast."
	if (mouseOverObj != nullptr && Shortcut_IsPressed(ShortcutID::Debug_BumpUnit)) {
		const Point2F pushVec2D = { 2.0f, 1.0f };
		LegoObject_Push(mouseOverObj, &pushVec2D, 40.0f);
	}

	/// EDIT KEYBIND: [H]  "Creates a spider web at mousepoint."
	if (Lego_IsAllowEditMode() && Shortcut_IsPressed(ShortcutID::Edit_PlaceSpiderWeb)) {
		SpiderWeb_SpawnAt(mbx, mby);
	}

	/// DEBUG KEYBIND: [F]  "Take 40 health points off all units selected."
	if (Message_AnyUnitSelected() && Shortcut_IsPressed(ShortcutID::Debug_DamageUnit)) {
		const uint32 numSelected = Message_GetNumSelectedUnits();
		LegoObject** selectedUnits = Message_GetSelectedUnits();
		for (uint32 i = 0; i < numSelected; i++) {
			LegoObject_AddDamage2(selectedUnits[i], 40.0f, true, noMouseButtonsElapsed);
		}
	}
	/// DEBUG KEYBIND: NULL  "Heal to max HP and energy for all units selected."
	if (Message_AnyUnitSelected() && Shortcut_IsDown(ShortcutID::Cheat_HealUnit)) {
		const uint32 numSelected = Message_GetNumSelectedUnits();
		LegoObject** selectedUnits = Message_GetSelectedUnits();
		// The DamageNumbers only want to work with negative values, so turn this off if its weird.
		const bool showVisual = false;

		for (uint32 i = 0; i < numSelected; i++) {
			LegoObject* unit = selectedUnits[i];

			const real32 origHealth = unit->health;
			if (origHealth >= 0.0f && origHealth < 100.0f) {
				// Not dead, and not an object that doesn't use health/energy.
				const real32 healAmount = (100.0f - origHealth);
				unit->health = 100.0f;
				if (showVisual) {
					// Show damage text, is one of these for the minus flag?
					if (Input_IsKeyDown(Keys::KEY_RIGHTCTRL))
						unit->flags2 |= (LIVEOBJ2_SHOWDAMAGENUMBERS);
					else
						unit->flags2 |= (LIVEOBJ2_DAMAGE_UNK_1000|LIVEOBJ2_SHOWDAMAGENUMBERS);
					unit->damageNumbers += healAmount; // += will show a negative number, but that's because positive numbers are blocked.
				}
				Bubble_ShowHealthBar(unit); // Show the healthbar as feedback that the unit was healed.
			}
			const real32 origEnergy = unit->energy;
			if (origEnergy >= 0.0f && origEnergy < 100.0f) {
				// Not dead, and not an object that doesn't use health/energy.
				unit->energy = 100.0f;
			}
		}
	}
	/// DEBUG KEYBIND: NULL  "Levels up selected units to max level, and gives selected mini-figures all abilities."
	if (Shortcut_IsPressed(ShortcutID::Cheat_MaxOutUnit)) {
		const uint32 numSelected = Message_GetNumSelectedUnits();
		LegoObject** selectedUnits = Message_GetSelectedUnits();

		for (uint32 i = 0; i < numSelected; i++) {
			LegoObject* unit = selectedUnits[i];

			uint32 maxLevel = Stats_GetLevels(unit->type, unit->id) - 1; // Levels are 0-indexed, -1 for max level.
			bool safeToUpgrade = true;
			bool safeToTrain = true;

			// It's probably best to avoid changing units that are in the process of training/upgrading to avoid crashes.
			// NOTE: Extra checks are needed for vehicles and buildings because of upgrade bitfields.
			if (unit->objLevel >= maxLevel ||
				(unit->activityName1 == objectGlobs.activityName[Activity_Upgrade]))
			{
				safeToUpgrade = false; // No need to upgrade, or busing upgrading.
			}
			if (unit->type != LegoObject_MiniFigure || (unit->abilityFlags & ABILITY_FLAGS_ALL) == ABILITY_FLAGS_ALL ||
				(unit->activityName1 == objectGlobs.activityName[Activity_Train]))
			{
				safeToTrain = false; // No abilities to train, or busy training.
			}

			// If not currently training/upgrading, make sure there isn't a queued task to do so.
			for (auto task : aiListSet.EnumerateAlive()) {
				if (!safeToUpgrade && !safeToTrain)
					break;

				if (task->targetObject == unit) {
					if (task->taskType == AITask_Type_Upgrade)
						safeToUpgrade = false;
					if (task->taskType == AITask_Type_Train)
						safeToTrain = false;
				}
			}
			
			if (safeToUpgrade) {
				if (unit->type == LegoObject_Vehicle) {
					// A vehicles level count may not reflect its supported max level due to upgrade bitfields.
					// There should be a function made to consolidate this.
					LegoObject_UpgradeFlags upgFlags = UPGRADE_FLAG_NONE;
					if (Vehicle_CanUpgradeType(unit->vehicle, LegoObject_UpgradeType_Carry, false))
						upgFlags |= UPGRADE_FLAG_CARRY;
					if (Vehicle_CanUpgradeType(unit->vehicle, LegoObject_UpgradeType_Scan, false))
						upgFlags |= UPGRADE_FLAG_SCAN;
					if (Vehicle_CanUpgradeType(unit->vehicle, LegoObject_UpgradeType_Speed, false))
						upgFlags |= UPGRADE_FLAG_SPEED;
					if (Vehicle_CanUpgradeType(unit->vehicle, LegoObject_UpgradeType_Drill, false))
						upgFlags |= UPGRADE_FLAG_DRILL;

					maxLevel = (uint32)upgFlags;
					Vehicle_SetUpgradeLevel(unit->vehicle, maxLevel);
					StatsObject_SetObjectLevel(unit, maxLevel);
				}
				else if (unit->type == LegoObject_Building) {
					Building_SetUpgradeLevel(unit->building, maxLevel);
					StatsObject_SetObjectLevel(unit, maxLevel);
				}
				else {
					StatsObject_SetObjectLevel(unit, maxLevel);
				}

				// Update dependencies and disable help window.
				HelpWindow_RecallDependencies(unit->type, unit->id, unit->objLevel, true);
			}
			if (safeToTrain) {
				LegoObject_TrainMiniFigure_instantunk(unit, ABILITY_FLAGS_ALL);
			}
		}
	}

	/// DEBUG KEYBIND: [H]  ???
	if (Message_AnyUnitSelected() && Message_GetPrimarySelectedUnit()->type == LegoObject_Building &&
		Shortcut_IsPressed(ShortcutID::Debug_Unknown_ElectricFence))
	{
		ElectricFence_FUN_0040d420(Message_GetPrimarySelectedUnit(), 0, 0);
	}

	/// EDIT KEYBIND: [J]  "Place an electric fence at mousepoint."
	if (Lego_IsAllowEditMode() && Shortcut_IsPressed(ShortcutID::Edit_PlaceElectricFence)) {
		if (!ElectricFence_Block_IsFence(mbx, mby)) {
			ElectricFence_Debug_PlaceFence(mbx, mby);
		}
		else {
			ElectricFence_Debug_RemoveFence(mbx, mby);
		}
	}

	/// EDIT KEYBIND: NULL  "Places or removes a power path at mousepoint."
	if (Lego_IsAllowEditMode() && Shortcut_IsPressed(ShortcutID::Edit_PlacePath) &&
		Level_Block_IsGround(mbx, mby) && !Level_Block_IsPathBuilding(&mouseBlockPos) &&
		!Construction_Zone_ExistsAtBlock(&mouseBlockPos) &&
		(blockValue(legoGlobs.currLevel, mbx, mby).terrain != Lego_SurfaceType_Lava) &&
		(blockValue(legoGlobs.currLevel, mbx, mby).terrain != Lego_SurfaceType_Lake))
	{
		if (!Level_Block_IsPath(&mouseBlockPos)) {
			Level_Block_SetPath(&mouseBlockPos);
			Interface_BackToMain_IfSelectedGroundOrConstruction_IsBlockPos(&mouseBlockPos);
			Dependencies_Object_FUN_0040add0(LegoObject_Path, (LegoObject_ID)0, 0);
		}
		else {
			blockValue(legoGlobs.currLevel, mbx, mby).flags1 &= ~BLOCK1_PATH;
			Level_BlockUpdateSurface(legoGlobs.currLevel, mbx, mby, 0);
			Interface_BackToMain_IfSelectedGroundOrConstruction_IsBlockPos(&mouseBlockPos);
			LegoObject_RequestPowerGridUpdate();
		}
	}

	const bool cryOreExactMousePoint = true;

	/// EDIT KEYBIND: NULL  "Places one crystal at mousepoint."
	if (Lego_IsAllowEditMode() && Shortcut_IsPressed(ShortcutID::Edit_PlaceCrystal) && Level_Block_IsGround(mbx, mby)) {
		Point2F wPos2D = { 0.0f }; // dummy init
		if (cryOreExactMousePoint) {
			Vector3F wPos3D = { 0.0f }; // dummy init
			Lego_GetMouseWorldPosition(&wPos3D);
			Gods98::Maths_Vector3DMake2D(&wPos2D, &wPos3D);
		}
		Level_GenerateCrystal(&mouseBlockPos, 0, (cryOreExactMousePoint ? &wPos2D : nullptr), false);
	}
	/// EDIT KEYBIND: NULL  "Places one ore piece at mousepoint."
	if (Lego_IsAllowEditMode() && Shortcut_IsPressed(ShortcutID::Edit_PlaceOre) && Level_Block_IsGround(mbx, mby)) {
		Point2F wPos2D = { 0.0f }; // dummy init
		if (cryOreExactMousePoint) {
			Vector3F wPos3D = { 0.0f }; // dummy init
			Lego_GetMouseWorldPosition(&wPos3D);
			Gods98::Maths_Vector3DMake2D(&wPos2D, &wPos3D);
		}
		Level_GenerateOre(&mouseBlockPos, 0, (cryOreExactMousePoint ? &wPos2D : nullptr), false);
	}
	/// CHEAT KEYBIND: NULL  "Increases the number of stored crystals by 1."
	if (Shortcut_IsPressed(ShortcutID::Cheat_IncreaseCrystalsStored)) {
		Level_IncCrystalsStored();
		LegoObject_RequestPowerGridUpdate();
	}
	/// CHEAT KEYBIND: NULL  "Increases the number of stored ore by 5."
	if (Shortcut_IsPressed(ShortcutID::Cheat_IncreaseOreStored)) {
		Level_AddOreStored(false, 5);
	}

	/// DEBUG KEYBIND: NULL  "Toggles the power of a building, or the sleeping state of a monster."
	if (Lego_IsAllowEditMode() && Shortcut_IsPressed(ShortcutID::Edit_TogglePower) && Message_AnyUnitSelected()) {
		LegoObject_SetPowerOn(Message_GetPrimarySelectedUnit(), (Message_GetPrimarySelectedUnit()->flags3 & LIVEOBJ3_POWEROFF));
	}
	/// DEBUG KEYBIND: NULL  "Freezes unit at mousepoint in a block of ice for 10 seconds."
	if (mouseOverObj != nullptr && Shortcut_IsPressed(ShortcutID::Cheat_FreezeUnit)) {
		LegoObject_Freeze(mouseOverObj, 10.0f); // Freeze for 10 seconds.
	}

	const bool placeDynBirdExactMousePoint = true;

	/// DEBUG KEYBIND: NULL  "Causes an explosion at the all selected units' positions and kills them in the process."
	if (Shortcut_IsPressed(ShortcutID::Cheat_KamikazeUnit) && Message_AnyUnitSelected()) {
		const uint32 numSelected = Message_GetNumSelectedUnits();
		LegoObject** selectedUnits = Message_GetSelectedUnits();

		for (uint32 i = 0; i < numSelected; i++) {
			LegoObject* unit = selectedUnits[i];

			const real32 heading = LegoObject_GetHeading(unit);
			Point2F wPos2D = { 0.0f }; // dummy init
			LegoObject_GetPosition(unit, &wPos2D.x, &wPos2D.y);
			LegoObject* dynamiteObj = LegoObject_CreateInWorld(legoGlobs.contDynamite, LegoObject_Dynamite, (LegoObject_ID)0, 0, wPos2D.x, wPos2D.y, heading);
			// Set the dynamite explosion block to invalid coordinates (strangely not -1, -1).
			dynamiteObj->targetBlockPos = Point2F { 0.0f, 0.0f };
			LegoObject_StartTickDown(dynamiteObj, true);

			// No tickdown, explode immediately.
			dynamiteObj->health = -1.0f;
			
			if (unit->health >= 0.0f) {
				unit->health = -1.0f;
				switch (unit->type) {
				case LegoObject_Building:
				case LegoObject_Vehicle:
				case LegoObject_MiniFigure:
					// Destroy these object types normally.
					unit->health = -1.0f;
					break;
				case LegoObject_RockMonster:
					unit->health = -1.0f;
					if (!(StatsObject_GetStatsFlags3(unit) & STATS3_SHOWHEALTHBAR) ||
						(StatsObject_GetStatsFlags2(unit) & STATS2_USEHOLES))
					{
						// Remove unit by force. Either its a unit that doesn't "die",
						//  or Slimy Slug burrowing is too slow for a kamikaze visual.
						unit->flags3 |= LIVEOBJ3_REMOVING;
					}
					break;
				default:
					unit->health = -1.0f;
					unit->flags3 |= LIVEOBJ3_REMOVING;
					break;
				}
			}
		}
	}
	/// DEBUG KEYBIND: NULL  "Spawns dynamite and starts the tickdown."
	/// DEBUG KEYBIND: NULL  "Spawns dynamite at mousepoint that immediately explodes."
	if ((Shortcut_IsPressed(ShortcutID::Cheat_PlaceDynamite) || Shortcut_IsPressed(ShortcutID::Cheat_PlaceDynamiteInstant)) &&
		(Level_Block_IsGround(mbx, mby) || Level_Block_IsWall(mbx, mby)))
	{
		const real32 heading = Gods98::Maths_RandRange(0.0f, M_PI*2.0f);
		Point2F wPos2D = { 0.0f }; // dummy init
		if (placeDynBirdExactMousePoint) {
			Vector3F wPos3D = { 0.0f }; // dummy init
			Lego_GetMouseWorldPosition(&wPos3D);
			Gods98::Maths_Vector3DMake2D(&wPos2D, &wPos3D);
		}
		else {
			Map3D_BlockToWorldPos(Lego_GetMap(), mbx, mby, &wPos2D.x, &wPos2D.x);
		}
		LegoObject* dynamiteObj = LegoObject_CreateInWorld(legoGlobs.contDynamite, LegoObject_Dynamite, (LegoObject_ID)0, 0, wPos2D.x, wPos2D.y, heading);
		// Set the block this is on, which will be demolished if its a wall.
		dynamiteObj->targetBlockPos = Point2F { (real32)mbx, (real32)mby };
		LegoObject_StartTickDown(dynamiteObj, true);
		if (Shortcut_IsPressed(ShortcutID::Cheat_PlaceDynamiteInstant)) {
			// No tickdown, explode immediately.
			dynamiteObj->health = -1.0f;
		}
	}
	/// DEBUG KEYBIND: NULL  "Spawns Sonic Blaster and starts the tickdown."
	/// DEBUG KEYBIND: NULL  "Spawns a sonic blaster at mousepoint that immediately goes off."
	if ((Shortcut_IsPressed(ShortcutID::Cheat_PlaceSonicBlaster) || Shortcut_IsPressed(ShortcutID::Cheat_PlaceSonicBlasterInstant)) &&
		(Level_Block_IsGround(mbx, mby) || Level_Block_IsWall(mbx, mby)))
	{
		const real32 heading = Gods98::Maths_RandRange(0.0f, M_PI*2.0f);
		Point2F wPos2D = { 0.0f }; // dummy init
		if (placeDynBirdExactMousePoint) {
			Vector3F wPos3D = { 0.0f }; // dummy init
			Lego_GetMouseWorldPosition(&wPos3D);
			Gods98::Maths_Vector3DMake2D(&wPos2D, &wPos3D);
		}
		else {
			Map3D_BlockToWorldPos(Lego_GetMap(), mbx, mby, &wPos2D.x, &wPos2D.x);
		}
		LegoObject* oohScaryObj = LegoObject_CreateInWorld(legoGlobs.contOohScary, LegoObject_OohScary, (LegoObject_ID)0, 0, wPos2D.x, wPos2D.y, heading);
		LegoObject_StartTickDown(oohScaryObj, true);
		if (Shortcut_IsPressed(ShortcutID::Cheat_PlaceSonicBlasterInstant)) {
			// No tickdown, go off immediately.
			oohScaryObj->health = -1.0f;
		}
	}


	/// DEBUG KEYBIND: [Y]  "Triggers the CrystalFound InfoMessage."
	if (Shortcut_IsPressed(ShortcutID::Debug_InterfaceCrystalFoundMessage)) {
		const Point2I infoBlockPos = { 1, 1 };
		Info_Send(Info_CrystalFound, nullptr, nullptr, &infoBlockPos);
	}


	/// DEBUG KEYBIND: (no LShift)+[U]  "Ends Advisor_Anim_Point_N."
	if (Shortcut_IsPressed(ShortcutID::Debug_EndAdvisorAnim)) {
		Advisor_End();
	}
	/// DEBUG KEYBIND: [LShift]+[U]  "Begins Advisor_Anim_Point_N."
	if (Shortcut_IsPressed(ShortcutID::Debug_BeginAdvisorAnim)) {
		Panel_SetCurrentAdvisorFromButton(Panel_Radar, PanelButton_Radar_Toggle, true);
	}

	/// DEBUG KEYBIND: [K]  "Pt.1 Registers a selected vehicle as a get-in target."
	if (Message_AnyUnitSelected() && Message_GetPrimarySelectedUnit()->type == LegoObject_Vehicle &&
		Shortcut_IsPressed(ShortcutID::Debug_CommandRegisterVehicle))
	{
		// Register vehicle for Get-in.
		gamectrlGlobs.dbgGetInVehicle = Message_GetPrimarySelectedUnit();
	}
	/// DEBUG KEYBIND: [K]  "Pt.2 Tells a selected minifigure to get in a registered vehicle (training not required)."
	if (Message_AnyUnitSelected() && Message_GetPrimarySelectedUnit()->type == LegoObject_MiniFigure &&
		Shortcut_IsPressed(ShortcutID::Debug_CommandGetInRegisteredVehicle))
	{
		// Tell unit to Get-in registered vehicle.
		/// FIX APPLY: Ensure our registered vehicle is valid/hasn't been teleported up/etc.
		///            Note that this is also ensured in LegoObject_Remove.
		if (!ListSet::IsNullOrDead(gamectrlGlobs.dbgGetInVehicle) &&
			gamectrlGlobs.dbgGetInVehicle->type == LegoObject_Vehicle)
		{
			LegoObject* registeredVehicle = gamectrlGlobs.dbgGetInVehicle;
			LegoObject_TryFindDriver_FUN_00440690(Message_GetPrimarySelectedUnit(), registeredVehicle);
		}
	}
}


// DRAW MODE: Only Draw API drawing calls can be used within this function.
// <LegoRR.exe @004292e0>
void __cdecl LegoRR::Lego_DrawDragSelectionBox(Lego_Level* level)
{
	if (legoGlobs.flags1 & GAME1_MULTISELECTING) {
		Point2F end = {
			static_cast<real32>(Gods98::msx()),
			static_cast<real32>(Gods98::msy()),
		};
		Point2F start;
		Gods98::Viewport_WorldToScreen(legoGlobs.viewMain, &start, &legoGlobs.vectorDragStartUnk_a4);

		// 0 ____ 1
		//  |    |
		//  |____|
		// 3      2

		// If the drawing surface is already locked, then we may as well use the Draw API.
		if (!Gods98::Draw_IsLocked() && LEGO_DRAWFILL_SELECTIONBOXES && !Lego_IsTransparentMultiSelectBox()) {
			Area2F window;

			const real32 r = legoGlobs.DragBoxRGB.red;
			const real32 g = legoGlobs.DragBoxRGB.green;
			const real32 b = legoGlobs.DragBoxRGB.blue;

			// We need to draw boxes with positive dimensions, so convert start/end to min/max coords.
			const Point2F min = {
				std::min(start.x, end.x),
				std::min(start.y, end.y),
			};
			const Point2F max = {
				std::max(start.x, end.x),
				std::max(start.y, end.y),
			};

			start = min;
			end   = max;

			// +1 because drawing rectagles is different from how the Draw API does line endpoints.
			// Just ignore the fact that each use of length has -1 after it,
			//  which is because we're skipping the pixel of the overlapping line.
			const Point2F length = {
				end.x - start.x + 1.0f,
				end.y - start.y + 1.0f,
			};

			// 3000
			// 3  1
			// 2221

			window = Area2F { start.x+1, start.y,  length.x-1, 1 }; // +1/-1 to skip overlapping pixel.
			Gods98::DirectDraw_ClearRGBF(&window, r, g, b);
			window = Area2F { end.x,   start.y+1,  1, length.y-1 }; // +1/-1 to skip overlapping pixel.
			Gods98::DirectDraw_ClearRGBF(&window, r, g, b);

			window = Area2F { start.x,     end.y,  length.x-1, 1 }; // -1 to skip overlapping pixel.
			Gods98::DirectDraw_ClearRGBF(&window, r, g, b);
			window = Area2F { start.x,   start.y,  1, length.y-1 }; // -1 to skip overlapping pixel.
			Gods98::DirectDraw_ClearRGBF(&window, r, g, b);
		}
		else {

			Point2F lineList[5] = { 0.0f }; // dummy init
			lineList[0] = Point2F { start.x, start.y };
			lineList[1] = Point2F { end.x,   start.y };
			lineList[2] = Point2F { end.x,     end.y };
			lineList[3] = Point2F { start.x,   end.y };
			lineList[4] = lineList[0];

			Gods98::DrawEffect effect = Gods98::DrawEffect::HalfTrans;
			if (!Lego_IsTransparentMultiSelectBox())
				effect = Gods98::DrawEffect::None;

			Gods98::Draw_LineListEx(lineList, lineList + 1, 4, legoGlobs.DragBoxRGB.red,
									legoGlobs.DragBoxRGB.green, legoGlobs.DragBoxRGB.blue,
									effect);
		}
	}
}


// Returns TRUE if liveObj (or its drivenObj) is the first-person unit.
// BUG: When in topdown view, returns TRUE if the objectFP is not NULL and matches the unit's drivenObj.
// <LegoRR.exe @004294f0>
bool32 __cdecl LegoRR::Lego_IsFPObject(LegoObject* liveObj)
{
	/// FIX APPLY: Only return true if we're in FP mode, beforehand the mode didn't matter if the drivenObject matched.
	if (legoGlobs.viewMode == ViewMode_FP) {
		return (liveObj == legoGlobs.objectFP ||
				(liveObj->driveObject != nullptr && liveObj->driveObject == legoGlobs.objectFP));
	}
	return false;

	/// OLD BUGGED LOGIC:
	//if ((legoGlobs.viewMode == ViewMode_FP && liveObj == legoGlobs.objectFP) ||
	//	(liveObj->driveObject != nullptr && liveObj->driveObject == legoGlobs.objectFP))
	//{
	//	return true;
	//}
	//return false;
}


/// CUSTOM:
void LegoRR::Lego_SetSceneFogParams(ViewMode viewMode)
{
	if (legoGlobs.flags1 & GAME1_FOGCOLOURRGB) {
		real32 start;
		real32 end;
		real32 density;
		if (viewMode == ViewMode_Top) {
			start   = legoGlobs.TVClipDist * (3.0f / 4.0f); // Clearer area around the center (helps when cam is zoomed out).
			end     = legoGlobs.TVClipDist;
			density = 0.0032f * 0.8f; // Recuce density in topdown.
		}
		else if (viewMode == ViewMode_FP) {
			start   = 0.0f;
			end     = legoGlobs.FPClipBlocks * Lego_GetLevel()->BlockSize;
			density = 0.0032f; // Assuming this was from division: 4.0f / 1250.0f
		}
		else {
			return;
		}
		Gods98::Container_SetFogParams(start, end, density);
	}
}

// <LegoRR.exe @00429520>
void __cdecl LegoRR::Lego_SetViewMode(ViewMode viewMode, LegoObject* liveObj, uint32 fpCameraFrame)
{
	if (viewMode == ViewMode_FP) {
		const real32 smoothFOV = ((fpCameraFrame==0) ? 0.9f : 0.6f);

		Camera_SetFPObject(legoGlobs.cameraFP, liveObj, fpCameraFrame);
		Water_ChangeViewMode_removed(true);
		Map3D_SetEmissive(legoGlobs.currLevel->map, true);

		legoGlobs.objectFP = liveObj;

		if (liveObj->type == LegoObject_MiniFigure) {
			LegoObject_DropCarriedObject(liveObj, false);
		}

		Gods98::Viewport_SetCamera(legoGlobs.viewMain, legoGlobs.cameraFP->contCam);
		Gods98::Sound3D_MakeContainerListener(legoGlobs.cameraFP->contCam);
		Gods98::Sound3D_SetMinDistForAtten(50.0f);
		Gods98::Viewport_SmoothSetField(legoGlobs.viewMain, smoothFOV);
		Gods98::Viewport_SetBackClip(legoGlobs.viewMain, legoGlobs.currLevel->BlockSize * legoGlobs.FPClipBlocks);
	}
	else if (viewMode == ViewMode_Top) {
		if (legoGlobs.objectFP != nullptr) {
			Lego_Goto(legoGlobs.objectFP, nullptr, false);
		}
		legoGlobs.cameraFP->trackObj = nullptr;
		legoGlobs.objectFP = nullptr;

		Water_ChangeViewMode_removed(false);
		Map3D_SetEmissive(legoGlobs.currLevel->map, false);
		Gods98::Viewport_SetCamera(legoGlobs.viewMain, legoGlobs.cameraMain->contCam);
		Gods98::Sound3D_MakeContainerListener(legoGlobs.cameraMain->contListener);
		Gods98::Sound3D_SetMinDistForAtten(legoGlobs.MinDistFor3DSoundsOnTopView);
		Gods98::Viewport_SetField(legoGlobs.viewMain, 0.5f);
		Gods98::Viewport_SetBackClip(legoGlobs.viewMain, legoGlobs.TVClipDist);
	}

	Lego_SetSceneFogParams(viewMode);

	legoGlobs.viewMode = viewMode;
}

// <LegoRR.exe @004296d0>
void __cdecl LegoRR::Lego_CDTrackPlayNextCallback(void)
{
	/// NOTE: This callback is only hit from Sound_Update when passing cdtrack = true.
	///       Meaning this function will only be reached when music is enabled+actively playing.
	Gods98::Sound_StopCD();
	Lego_SetMusicPlaying(true);
}

// <LegoRR.exe @004296e0>
void __cdecl LegoRR::Lego_SetMusicPlaying(bool32 on)
{
	if (on) {
		legoGlobs.flags1 |= GAME1_MUSICPLAYING; // Playing state.
		// ABOUT MUSIC FIX WINMM.DLL:
		//  The Music Fix dll changes up most behaviour, and basically disables normal track randomization.
		//  It's always randomized by the dll itself, and it will never report being stopped,
		//   meaning Lego_CDTrackPlayNextCallback will never be called.

		// StartTrack is likely what track to skip (it's 2 in Lego.cfg, so track 1 is likely reserved for non-game music???).
		const uint32 track = legoGlobs.CDStartTrack - 1 + ((uint32)Gods98::Maths_Rand() % legoGlobs.CDTracks);

		Gods98::Sound_PlayCDTrack(track, Gods98::SoundMode::Once, Lego_CDTrackPlayNextCallback);
	}
	else {
		legoGlobs.flags1 &= ~GAME1_MUSICPLAYING; // Not playing state.
		Gods98::Sound_StopCD();
		Gods98::Sound_Update(false);
	}
}

// <LegoRR.exe @00429740>
void __cdecl LegoRR::Lego_SetSoundOn(bool32 on)
{
	if (on) {
		legoGlobs.flags1 |= GAME1_USESFX;
		SFX_SetSoundOn_AndStopAll(true);
		return;
	}
	else {
		legoGlobs.flags1 &= ~GAME1_USESFX;
		SFX_SetSoundOn_AndStopAll(false);
	}
}


// Move functionality from here into Lego_LoadLevel once that's implemented.
/// CUSTOM: Extended version of `Lego_LoadLevel` that also handles storing the SeeThroughWalls property.
/// CHANGE: Always allocate new memory for the level name, because its taken from various places that we can't rely on.
bool32 __cdecl LegoRR::Lego_LoadLevel2(const char* tempLevelName)
{
	/// NEW: Allocate memory for level name (because name was being taken from inconsistent sources).
	char* levelName = Gods98::Util_StrCpy(tempLevelName);

	/// NEW: Store SeeThroughWalls property so that we don't need to look it up on-demand.
	legoGlobs2.seeThroughWalls = Config_GetBoolOrTrue(legoGlobs.config, Lego_ID(levelName, "SeeThroughWalls"));

	if (!Lego_LoadLevel(levelName)) {
		/// NEW: Failed to load level, so cleanup allocated levelName string.
		Gods98::Mem_Free(levelName);
		return false;
	}

	return true;
}


// <LegoRR.exe @0042c260>
bool32 __cdecl LegoRR::Level_HandleEmergeTriggers(Lego_Level* level, const Point2I* blockPos, OUT Point2I* emergeBlockPos)
{
    /// DEBUG: (testing) Enable multiple emerges at once :)
//#define DEBUG_MULTIPLE_EMERGE


#ifdef DEBUG_MULTIPLE_EMERGE
    uint32 emergedCount = 0;
#endif

    for (uint32 i = 0; i < level->emergeCount; i++) {
        EmergeTrigger* trigger = &level->emergeTriggers[i];
        const Point2I* triggerPos = &trigger->blockPos;
        if ((trigger->blockPos.x == blockPos->x && trigger->blockPos.y == blockPos->y) &&
            trigger->timeout == 0.0f)
        {
            for (uint32 j = 0; j < EMERGE_MAXPOINTS; j++) {
                const EmergeBlock* emergePt = &trigger->emergePoints[j];
                if (emergePt->used) {

                    LegoObject* legoObj = LegoObject_TryGenerateRMonster(
                        &legoGlobs.rockMonsterData[level->EmergeCreature],
                        LegoObject_RockMonster, level->EmergeCreature,
                        emergePt->blockPos.x, emergePt->blockPos.y);
                    
                    if (legoObj != nullptr) {

                        #ifdef DEBUG_MULTIPLE_EMERGE
                        // Everything after is just setting the output parameter and returning,
                        //  which we don't need with this preprocessor after the first emerge.
                        if (emergedCount++ > 0)
                            continue;  
                        #endif

                        trigger->timeout = level->EmergeTimeOut;
                        if (emergeBlockPos != nullptr) {
                            *emergeBlockPos = emergePt->blockPos;
                        }

                        #ifndef DEBUG_MULTIPLE_EMERGE
                        return true;
                        #endif
                    }
                }
            }
        }
    }

    #ifdef DEBUG_MULTIPLE_EMERGE
    /// CUSTOM: Spawn all the things!
    return (emergedCount != 0);

    #else
    return false;

    #endif
}



// <LegoRR.exe @0042eff0>
const char* __cdecl LegoRR::Level_Free(void)
{
	Lego_Level* level = legoGlobs.currLevel;
	const char* nextLevelName = nullptr;

	if (legoGlobs.currLevel != nullptr) {
		Gods98::Mem_Free((legoGlobs.currLevel)->emergeTriggers);
		if (legoGlobs.EndGameAVI1 != nullptr) {
			Gods98::Mem_Free(legoGlobs.EndGameAVI1);
		}
		if (legoGlobs.EndGameAVI2 != nullptr) {
			Gods98::Mem_Free(legoGlobs.EndGameAVI2);
		}
		Gods98::Mem_Free(level->FullName);

		legoGlobs.flags1 &= ~(GAME1_CAMERAGOTO|GAME1_LEVELENDING);
		legoGlobs.flags2 &= ~(GAME2_LEVELEXITING|GAME2_NOMULTISELECT);

		objectGlobs.flags &= ~LegoObject_GlobFlags::OBJECT_GLOB_FLAG_CYCLEUNITS;

		legoGlobs.gotoSmooth = false;
		legoGlobs.objTeleportQueue_COUNT = 0;
		objectGlobs.minifigureObj_9cb8 = nullptr;
		objectGlobs.cycleUnitCount = 0;
		objectGlobs.cycleBuildingCount = 0;

		Lego_SetCallToArmsOn(false);

		legoGlobs.flags1 &= ~GAME1_LASERTRACKER;
		legoGlobs.flags2 &= ~(GAME2_ATTACKDEFER|GAME2_MESSAGE_HASNEXT|GAME2_MESSAGE_HASREPEAT|GAME2_MENU_HASNEXT);

		_drainPowerBlockList.clear();
		_poweredBlockList.clear();
		_unpoweredBlockList.clear();
		// These fields have been replaced by the above vectors, but clear anyway for sanity.
		legoGlobs.powerDrainCount = 0;
		legoGlobs.poweredBlockCount = 0;
		legoGlobs.unpoweredBlockCount = 0;

		Camera_Shake(legoGlobs.cameraMain, 0.0f, 0.0f);
		Camera_SetZoom(legoGlobs.cameraMain, 200.0f);

		/// CUSTOM: Remove all created disposable ObjectStats.
		Stats_RemoveAllModified();

		/// CUSTOM: Remove all routing path lines.
		Debug_RouteVisual_RemoveAll();

		/// CUSTOM: Handle water cleanup so that it doesn't persist between levels.
		Water_RemoveAll();

		Smoke_RemoveAll();
		Lego_StopUserAction();

		nextLevelName = level->nextLevelID;

		Text_Clear();
		HelpWindow_ClearFlag1();
		Dependencies_Reset_ClearAllLevelFlags_10c();
		NERPs_FreeBlockPointers();

		AITask_CleanupLevel(false);
		LegoObject_CleanupLevel();
		Construction_RemoveAll();
		Effect_StopAll();
		AITask_CleanupLevel(true);

		Info_SetFlag4(false);
		Roof_Shutdown();
		Camera_TrackObject(legoGlobs.cameraTrack, nullptr, 0.0f, 0.0f, 0.0f, 0.0f);
		Camera_SetFPObject(legoGlobs.cameraFP, nullptr, 0);
		Message_CleanupSelectedUnitsCount();
		NERPsFile_Free();
		RadarMap_Free(level->radarMap);
		Map3D_Remove(level->map);
		Lego_FreeDetailMeshes(level);
		Detail_FreeTextureSet(level->textureSet);
		Level_BlockActivity_RemoveAll(level);
		Effect_RemoveAll_BoulderExplode();

		/// NEW: Free memory allocated for level name (because name was being taken from inconsistent sources).
		Gods98::Mem_Free(level->name);

		Gods98::Mem_Free(level->blocks);
		Gods98::Mem_Free(level);

		Info_ClearAllMessages();
		Interface_ClearStates();
		Lego_SetGameSpeed(1.0f);
		Panel_ResetAll();
		Advisor_Stop();
		Gods98::Sound3D_StopAllSounds();
		DamageFont_Cleanup();
	}
	legoGlobs.currLevel = nullptr;
	return nextLevelName;
}


// <LegoRR.exe @00431a50>
bool32 __cdecl LegoRR::Level_CanBuildOnBlock(sint32 bx, sint32 by, bool32 isPath, bool32 isWaterEntrance)
{
	const Point2I DIRECTIONS[DIRECTION__COUNT] = {
		{  0, -1 },
		{  1,  0 },
		{  0,  1 },
		{ -1,  0 },
	};

	const Point2I blockPos = { bx, by };
	const Lego_Block* block = &blockValue(Lego_GetLevel(), bx, by);

	/// SANITY: Bounds check.
	if (!blockInBounds(Lego_GetLevel(), bx, by))
		return false;


	// TERRAIN CHECKS:
	if (!Level_Block_IsGround(bx, by)) //if (!(block->flags1 & BLOCK1_FLOOR))
		return false; // Must build on floor.

	if (Level_Block_GetRubbleLayers(&blockPos)) //if (!(block->flags1 & BLOCK1_CLEARED))
		return false; // Can't build over rubble.

	if (block->flags2 & BLOCK2_SLUGHOLE_EXPOSED)
		return false; // Can't build over Slimy Slug holes.

	for (uint32 dir = 0; dir < DIRECTION__COUNT; dir++) {
		const Lego_Block* adjacentBlock = &blockValue(Lego_GetLevel(), bx + DIRECTIONS[dir].x, by + DIRECTIONS[dir].y);
		if (adjacentBlock->terrain == Lego_SurfaceType_RechargeSeam)
			return false; // Can't build adjacent to recharge seam walls.
	}

	if (Level_Block_IsLava(&blockPos)) //if (block->terrain == Lego_SurfaceType_Lava)
		return false; // Can't build over lava.


	// WATER TERRAIN CHECKS:
	// isPath is always true when isWaterEntrance is true.
	if (isPath && isWaterEntrance && block->terrain != Lego_SurfaceType_Lake)
		return false; // Must build over water for water entrances.

	if (!isWaterEntrance && block->terrain == Lego_SurfaceType_Lake)
		return false; // Can't build over water for non-water entrances.
	

	/// BUILDINGS CHECKS:
	// This check serves no purpose, due to the BLOCK1_BUILDINGPATH flag check below.
	if (isPath && Level_Block_IsPathBuilding(&blockPos)) //if (isPath && (block->flags1 & BLOCK1_BUILDINGPATH))
		return false; // Can't overlap building paths.

	if (block->flags1 & (BLOCK1_BUILDINGSOLID|BLOCK1_BUILDINGPATH|BLOCK1_FOUNDATION))
		return false; // Can't build over existing building tiles.

	if (Level_Block_IsPath(&blockPos)) //if (block->flags1 & BLOCK1_PATH)
		return false; // Can't build over Power Paths.

	if (Construction_Zone_ExistsAtBlock(&blockPos))
		return false; // Can't build over construction zones (including layed paths).

	if (ElectricFence_Block_IsFence(bx, by))
		return false; // Can't build over Electric Fences.


	return true;

	// There's a lot of compacted logic in this decompiled if statement,
	//  so I'm leaving the cleaned-up version here for error checking.

	//if ((!isPath || !isWaterEntrance || block->terrain == Lego_SurfaceType_Lake) &&
	//	(!isPath || !(block->flags1 & BLOCK1_BUILDINGPATH)) &&
	//	(isWaterEntrance || block->terrain != Lego_SurfaceType_Lake) &&
	//	block->terrain != Lego_SurfaceType_Lava &&
	//	(block->flags1 & BLOCK1_FLOOR) && (block->flags1 & BLOCK1_CLEARED) &&
	//	!(block->flags1 & (BLOCK1_BUILDINGSOLID|BLOCK1_BUILDINGPATH|BLOCK1_FOUNDATION)))
}


// <LegoRR.exe @00432030>
void __cdecl LegoRR::Level_PowerGrid_AddPoweredBlock(const Point2I* blockPos)
{
	Lego_Block* block = &blockValue(Lego_GetLevel(), blockPos->x, blockPos->y);
	block->flags2 |= BLOCK2_POWERED;

	_poweredBlockList.push_back(*blockPos);
	//legoGlobs.poweredBlocks[legoGlobs.poweredBlockCount] = *blockPos;
	//legoGlobs.poweredBlockCount++;
	Level_BlockUpdateSurface(Lego_GetLevel(), blockPos->x, blockPos->y, 0); // 0 = reserved
}

// <LegoRR.exe @004320a0>
bool32 __cdecl LegoRR::Level_Block_IsPowered(const Point2I* blockPos)
{
	const Lego_Block* block = &blockValue(Lego_GetLevel(), blockPos->x, blockPos->y);
	return (block->flags2 & BLOCK2_POWERED);
}

// <LegoRR.exe @004320d0>
void __cdecl LegoRR::Level_PowerGrid_UpdateUnpoweredBlockSurfaces(void)
{
	for (size_t i = 0; i < _unpoweredBlockList.size(); i++) {
		const Point2I blockPos = _unpoweredBlockList[i];

		if (!Level_Block_IsPowered(&blockPos)) {
			Level_BlockUpdateSurface(Lego_GetLevel(), blockPos.x, blockPos.y, 0); // 0 = reserved
		}
	}
	_unpoweredBlockList.clear();

	/*for (uint32 i = 0; i < legoGlobs.unpoweredBlockCount; i++) {
		const Point2I blockPos = legoGlobs.unpoweredBlocks[i];

		if (!Level_Block_IsPowered(&blockPos)) {
			Level_BlockUpdateSurface(Lego_GetLevel(), blockPos.x, blockPos.y, 0); // 0 = reserved
		}
	}
	legoGlobs.unpoweredBlockCount = 0;*/
}

// <LegoRR.exe @00432130>
void __cdecl LegoRR::Level_PowerGrid_UnpowerPoweredBlocks(void)
{
	// Move powered blocks to unpowered blocks list and unset powered flag.
	_unpoweredBlockList = _poweredBlockList;
	_poweredBlockList.clear();

	for (size_t i = 0; i < _unpoweredBlockList.size(); i++) {
		const Point2I blockPos = _unpoweredBlockList[i];

		Lego_Block* block = &blockValue(Lego_GetLevel(), blockPos.x, blockPos.y);
		block->flags2 &= ~BLOCK2_POWERED;
	}

	/*for (uint32 i = 0; i < legoGlobs.poweredBlockCount; i++) {
		const Point2I blockPos = legoGlobs.poweredBlocks[i];

		Lego_Block* block = &blockValue(Lego_GetLevel(), blockPos.x, blockPos.y);
		block->flags2 &= ~BLOCK2_POWERED;

		legoGlobs.unpoweredBlocks[i] = legoGlobs.poweredBlocks[i];
	}
	legoGlobs.unpoweredBlockCount = legoGlobs.poweredBlockCount;
	legoGlobs.poweredBlockCount = 0;*/
}

// <LegoRR.exe @004321a0>
void __cdecl LegoRR::Level_PowerGrid_AddDrainPowerBlock(const Point2I* blockPos)
{
	Lego_Block* block = &blockValue(Lego_GetLevel(), blockPos->x, blockPos->y);
	block->flags2 |= BLOCK2_DRAINPOWER_TEMP;
	_drainPowerBlockList.push_back(*blockPos);
	//legoGlobs.powerDrainBlocks[legoGlobs.powerDrainCount] = *blockPos;
	//legoGlobs.powerDrainCount++;
}

// <LegoRR.exe @00432200>
bool32 __cdecl LegoRR::Level_PowerGrid_IsDrainPowerBlock(const Point2I* blockPos)
{
	const Lego_Block* block = &blockValue(Lego_GetLevel(), blockPos->x, blockPos->y);
	return (block->flags2 & BLOCK2_DRAINPOWER_TEMP);
}

// <LegoRR.exe @00432230>
void __cdecl LegoRR::Level_PowerGrid_ClearDrainPowerBlocks(void)
{
	for (size_t i = 0; i < _drainPowerBlockList.size(); i++) {
		const Point2I blockPos = _drainPowerBlockList[i];

		Lego_Block* block = &blockValue(Lego_GetLevel(), blockPos.x, blockPos.y);
		block->flags2 &= ~BLOCK2_DRAINPOWER_TEMP;
	}

	_drainPowerBlockList.clear();

	/*for (uint32 i = 0; i < legoGlobs.powerDrainCount; i++) {
		const Point2I blockPos = legoGlobs.powerDrainBlocks[i];

		Lego_Block* block = &blockValue(Lego_GetLevel(), blockPos.x, blockPos.y);
		block->flags2 &= ~BLOCK2_DRAINPOWER_TEMP;
	}

	legoGlobs.powerDrainCount = 0;*/
}


// <LegoRR.exe @00435870>
bool32 __cdecl LegoRR::Lego_EndLevel(void)
{
	const bool32 isMissionLevelSel = Front_IsMissionSelected();
	const bool32 isTutorialLevelSel = Front_IsTutorialSelected();

	Gods98::Sound3D_StopAllSounds();

	/// FIX APPLY: Turn off music when instantly exiting a level.
	Lego_ChangeMusicPlaying(false); // End music.

	if (Front_IsFrontEndEnabled()) {
		Reward_CreateLevel();
		Reward_Prepare();
		Reward_Show();
		Reward_FreeLevel();
	}
	Lego_SetViewMode(ViewMode_Top, nullptr, 0);

	Gods98::TextWindow_Clear(legoGlobs.textOnlyWindow);

	// Default to using the next level specified in the level links. Otherwise ask the FrontEnd.
	const char* nextLevelID = Level_Free();

	if (Front_IsFrontEndEnabled()) {
		// Start FrontEnd loop to select the next level (this runs all FrontEnd menus).
		if (isMissionLevelSel) {
			Front_RunScreenMenuType(Menu_Screen_Missions);
			if (Front_IsTriggerAppQuit()) {
				return false;
			}
			nextLevelID = Front_GetSelectedLevel();
		}
		else if (isTutorialLevelSel) {
			Front_RunScreenMenuType(Menu_Screen_Training);
			if (Front_IsTriggerAppQuit()) {
				return false;
			}
			nextLevelID = Front_GetSelectedLevel();
		}
	}

	Front_LoadOptionParameters(true, false);
	if (nextLevelID == nullptr) {
		return false; // No next level selected, must have quit or something...
	}

	// Override Lego_LoadLevel to store SeeThroughWalls property.
	if (!Lego_LoadLevel2(nextLevelID)) {
		return false; // Failed to load level.
	}

	/// FIX APPLY: Don't free memory that wasn't allocated for this. BAD! NO TOUCH!
	// Another memory leak? Only freed on LoadLevel success...
	// I'm not even sure how this bit of memory is supposed to be managed.
	//Gods98::Mem_Free(nextLevelID);

	return true;
}

#pragma endregion
