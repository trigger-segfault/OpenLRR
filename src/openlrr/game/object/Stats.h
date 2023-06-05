// Stats.h : 
//

#pragma once

#include "../../engine/core/Config.h"

#include "../GameCommon.h"


namespace LegoRR
{; // !<---

/**********************************************************************************
 ******** Forward Declarations
 **********************************************************************************/

#pragma region Forward Declarations

struct LegoObject;

#pragma endregion

/**********************************************************************************
 ******** Constants
 **********************************************************************************/

#pragma region Constants

#define STATS_MAXTOOLAITASKS		3

#pragma endregion

/**********************************************************************************
 ******** Enumerations
 **********************************************************************************/

#pragma region Enums

enum StatsFlags1 : uint32 // [LegoRR/Stats.c|flags:0x4|type:uint]
{
	STATS1_NONE                  = 0,
	STATS1_SINGLEWIDTHDIG        = 0x1,
	STATS1_PROCESSORE            = 0x2,
	STATS1_PROCESSCRYSTAL        = 0x4,
	STATS1_STOREOBJECTS          = 0x8,
	STATS1_SMALLTELEPORTER       = 0x10,
	STATS1_BIGTELEPORTER         = 0x20,
	STATS1_WATERTELEPORTER       = 0x40,
	STATS1_COLLRADIUS            = 0x80,
	STATS1_COLLBOX               = 0x100,
	STATS1_CAUSESLIP             = 0x200,
	STATS1_RANDOMMOVE            = 0x400,
	STATS1_CANSCARE              = 0x800,
	STATS1_RANDOMENTERWALL       = 0x1000,
	STATS1_SCAREDBYPLAYER        = 0x2000,
	STATS1_SNAXULIKE             = 0x4000,
	STATS1_GRABMINIFIGURE        = 0x8000,
	STATS1_CANCLEARRUBBLE        = 0x10000,
	STATS1_CANBEDRIVEN           = 0x20000,
	STATS1_CANSCARESCORPION      = 0x40000,
	STATS1_CANSTEAL              = 0x80000,
	STATS1_TOOLSTORE             = 0x100000,
	STATS1_FLOCKS                = 0x200000,
	STATS1_FLOCKS_DEBUG          = 0x400000,
	STATS1_FLOCKS_SMOOTH         = 0x800000,
	STATS1_CROSSWATER            = 0x1000000,
	STATS1_CROSSLAND             = 0x2000000,
	STATS1_FLOCKS_ONGROUND       = 0x4000000,
	STATS1_FLOCKS_QUICKDESTROY   = 0x8000000,
	STATS1_FLOCKS_ANIMATEBYPITCH = 0x10000000,
	STATS1_ROUTEAVOIDANCE        = 0x20000000,
	STATS1_BUMPDAMAGE            = 0x40000000,
	STATS1_MANTELEPORTER         = 0x80000000,
};
flags_end(StatsFlags1, 0x4);


enum StatsFlags2 : uint32 // [LegoRR/Stats.c|flags:0x4|type:uint]
{
	STATS2_NONE                   = 0,
	STATS2_SCAREDBYBIGBANGS       = 0x1,
	STATS2_UPGRADEBUILDING        = 0x2,
	STATS2_TRAINPILOT             = 0x4,
	STATS2_TRAINSAILOR            = 0x8,
	STATS2_TRAINDRIVER            = 0x10,
	STATS2_TRAINDYNAMITE          = 0x20,
	STATS2_TRAINREPAIR            = 0x40,
	STATS2_TRAINSCANNER           = 0x80,
	STATS2_TRACKER                = 0x100,
	STATS2_GENERATEPOWER          = 0x200,
	STATS2_SELFPOWERED            = 0x400,
	STATS2_POWERBUILDING          = 0x600,
	STATS2_USEBIGTELEPORTER       = 0x800,
	STATS2_USESMALLTELEPORTER     = 0x1000,
	STATS2_USEWATERTELEPORTER     = 0x2000,
	STATS2_ATTACKPATHS            = 0x4000,
	STATS2_SPLITONZEROHEALTH      = 0x8000,
	STATS2_CANBEHITBYFENCE        = 0x10000,
	STATS2_CANDOUBLESELECT        = 0x20000,
	STATS2_CANBESHOTAT            = 0x40000,
	STATS2_DRAINPOWER             = 0x80000,
	STATS2_USEHOLES               = 0x100000,
	STATS2_CROSSLAVA              = 0x200000,
	STATS2_USELEGOMANTELEPORTER   = 0x400000,
	STATS2_DAMAGECAUSESCALLTOARMS = 0x800000,
	STATS2_CANFREEZE              = 0x1000000,
	STATS2_CANLASER               = 0x2000000,
	STATS2_CANPUSH                = 0x4000000,
	STATS2_DONTSHOWDAMAGE         = 0x8000000,
	STATS2_REMOVEREINFORCEMENT    = 0x10000000,
	STATS2_DONTSHOWONRADAR        = 0x20000000,
	STATS2_INVISIBLEDRIVER        = 0x40000000,
	STATS2_UNSELECTABLE           = 0x80000000,

	STATS2_FLAGS_ALL              = 0xffffffff,
};
flags_end(StatsFlags2, 0x4);


enum StatsFlags3 : uint32 // [LegoRR/Stats.c|flags:0x4|type:uint]
{
	STATS3_NONE                = 0,
	STATS3_CARRYVEHICLES       = 0x1,
	STATS3_VEHICLECANBECARRIED = 0x2,
	STATS3_CANSTRAFE           = 0x4,
	STATS3_CLASSASLARGE        = 0x8,
	STATS3_GETOUTATLAND        = 0x10,
	STATS3_GETINATLAND         = 0x20,
	STATS3_TAKECARRYINGDRIVERS = 0x40,
	STATS3_SHOWHEALTHBAR       = 0x80,
	STATS3_NEEDSPILOT          = 0x100,
	STATS3_ENTERTOOLSTORE      = 0x200,
};
flags_end(StatsFlags3, 0x4);

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

struct ObjectStats // [LegoRR/Stats.c|struct:0x150]
{
	/*000,1e*/      char unused_typeName[30]; // field set to 0x30 ('0') for all constant ObjectStats (Barrier, Dynamite, Upgrade, Other)
	/*01e,2*/       undefined2 padding;
	/*020,4*/       real32 RouteSpeed; // (leveled)
	/*024,48*/      real32 DrillTimes[Lego_SurfaceType_Count]; // (leveled, table of surface type times)
	/*06c,4*/       real32 CollRadius; // (effect: flags1 | 0x80)
	/*070,4*/       real32 AlertRadius;
	/*074,4*/       real32 CollHeight;
	/*078,4*/       real32 PickSphere;
	/*07c,4*/       real32 TrackDist;
	/*080,4*/       real32 HealthDecayRate;
	/*084,4*/       real32 EnergyDecayRate;
	/*088,4*/       real32 RepairValue; // (leveled)
	/*08c,4*/       real32 RestPercent;
	/*090,4*/       real32 CarryMinHealth;
	/*094,4*/       real32 StampRadius;
	/*098,4*/       real32 AttackRadius;
	/*09c,4*/       real32 WakeRadius; // (default: 20.0)
	/*0a0,8*/       Size2F CollBox; // (requires: invalid/no CollRadius, effect: flag 0x100, CollRadius = min(w,h) * 0.5)
	/*0a8,4*/       sint32 BuildingBase; // (from: FUN_00408bb0, invalid: -1, either ptr or basenumber)
	/*0ac,4*/       sint32 SurveyRadius; // (leveled)
	/*0b0,4*/       sint32 CostOre;
	/*0b4,4*/       sint32 CostCrystal;
	/*0b8,4*/       sint32 CostRefinedOre;
	/*0bc,4*/       sint32 CrystalDrain; // (leveled)
	/*0c0,4*/       SFX_ID DrillSound; // (from: FUN_00464f30, invalid: 2 SFX_Drill)
	/*0c4,4*/       SFX_ID DrillFadeSound; // (from: FUN_00464f30, invalid: SFX_DrillFade)
	/*0c8,4*/       SFX_ID EngineSound; // (from: FUN_00464f30, invalid: SFX_Null)
	/*0cc,4*/       sint32 NumOfToolsCanCarry; // (leveled)
	/*0d0,4*/       sint32 WaterEntrances;
	/*0d4,4*/       real32 RubbleCoef; // (default: 1.0)
	/*0d8,4*/       real32 PathCoef; // (default: 1.0)
	/*0dc,4*/       real32 OxygenCoef; // (negative consumes oxygen)
	/*0e0,4*/       sint32 Capacity;
	/*0e4,4*/       real32 AwarenessRange; // (default: 0.0)
	/*0e8,4*/       real32 PainThreshold; // (default: 0.0)
	/*0ec,4*/       sint32 MaxCarry; // (leveled)
	/*0f0,4*/       sint32 CarryStart; // (leveled)
	/*0f4,4*/       real32 UpgradeTime; // (leveled, mult: 25.0)
	/*0f8,4*/       real32 FunctionCoef; // (leveled)
	/*0fc,4*/       sint32 UpgradeCostOre; // Carry:Scan:Speed:Drill (requires all 4)
	/*100,4*/       sint32 UpgradeCostStuds; // Carry:Scan:Speed:Drill (requires all 4)
	/*104,4*/       real32 unused_viewAngle;
	/*108,4*/       real32 unused_viewDist;
	/*10c,4*/       real32 FreezerTime; // (default: 0.0)
	/*110,4*/       real32 FreezerDamage; // (default: 0.0)
	/*114,4*/       real32 PusherDist; // (default: 0.0)
	/*118,4*/       real32 PusherDamage; // (default: 0.0)
	/*11c,4*/       real32 LaserDamage; // (default: 0.0)
	/*120,4*/       real32 Flocks_Turn; // (default: 0.06)
	/*124,4*/       real32 Flocks_Speed; // (default: 2.0)
	/*128,4*/       real32 Flocks_Tightness; // (default: 2.0)
	/*12c,4*/       real32 Flocks_GoalUpdate; // (default: 2.0)
	/*130,4*/       real32 Flocks_Height; // (default: 30.0)
	/*134,4*/       real32 Flocks_Randomness; // (default: 2.0)
	/*138,4*/       real32 Flocks_AttackTime; // (default: 300.0)
	/*13c,4*/       sint32 Flocks_Size; // (default: 5)
	/*140,4*/       real32 RandomMoveTime; // (default: 200.0)
	/*144,4*/       StatsFlags1 flags1;
	/*148,4*/       StatsFlags2 flags2;
	/*14c,4*/       StatsFlags3 flags3;
	/*150*/
};
assert_sizeof(ObjectStats, 0x150);


struct ToolStats // [LegoRR/Stats.c|struct:0x10]
{
	/*00,c*/        AITask_Type taskTypes[STATS_MAXTOOLAITASKS]; // Up to 3 AITaskTypes can be associated with a tool
	/*0c,4*/        uint32 taskCount;
	/*10*/
};
assert_sizeof(ToolStats, 0x10);


struct Stats_Globs // [LegoRR/Stats.c|struct:0x5b0|tags:GLOBS]
{
	/*000,50*/      ObjectStats** objectStats[LegoObject_Type_Count]; // [objType:20][*objID][*objLevel]
	/*050,4b0*/     uint32 objectLevels[LegoObject_Type_Count][LegoObject_ID_Count]; // [objType:20][objID:15]
	/*500,b0*/      ToolStats toolStats[LegoObject_ToolType_Count]; // [toolType:11]
	/*5b0*/
};
assert_sizeof(Stats_Globs, 0x5b0);

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @00503bd8>
extern Stats_Globs & statsGlobs;

// All identical, with field_0x0 being assigned to 0x30, and all other fields zero.
// Used by: UpgradePart
// <LegoRR.exe @004aa7a0>
extern ObjectStats & c_ObjectStats_Upgrade;
// Used by: Dynamite
// <LegoRR.exe @004aa8f0>
extern ObjectStats & c_ObjectStats_Dynamite;
// Used by: Barrier
// <LegoRR.exe @004aaa40>
extern ObjectStats & c_ObjectStats_Barrier;
// Used by: SpiderWeb, OohScary, ElectricFenceStud, Pusher, Freezer, IceCube, LaserShot
// <LegoRR.exe @004aab90>
extern ObjectStats & c_ObjectStats_Other;

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

// Removes all allocated ObjectStats_Modified's.
void Stats_RemoveAllModified();

// Returns true if liveObj stats are a disposable type that can be safely modified.
bool StatsObject_IsModified(LegoObject* liveObj);

// Returns the original stats of this object, if StatsObject_MakeModified has not been called, then liveObj stats is returned.
ObjectStats* StatsObject_GetOriginal(LegoObject* liveObj);

// Converts liveObj stats to a disposable type that can be safely modified (does nothing if already modified stats).
ObjectStats* StatsObject_MakeModified(LegoObject* liveObj);

// Restores the liveObj's modified stats to their default values.
bool StatsObject_RestoreModified(LegoObject* liveObj);


// <LegoRR.exe @00466aa0>
bool32 __cdecl Stats_Initialise(const Gods98::Config* config, const char* gameName);

// <LegoRR.exe @00469d50>
void __cdecl Stats_AddToolTaskType(LegoObject_ToolType toolType, AITask_Type taskType);

// <LegoRR.exe @00469d80>
sint32 __cdecl Stats_GetCostOre(LegoObject_Type objType, LegoObject_ID objID, uint32 objLevel);

// <LegoRR.exe @00469db0>
sint32 __cdecl Stats_GetCostCrystal(LegoObject_Type objType, LegoObject_ID objID, uint32 objLevel);

// <LegoRR.exe @00469de0>
sint32 __cdecl Stats_GetCostRefinedOre(LegoObject_Type objType, LegoObject_ID objID, uint32 objLevel);

/// CUSTOM: Functions to get real resource costs without the Cheat override.
sint32 Stats_GetRealCostOre(LegoObject_Type objType, LegoObject_ID objID, uint32 objLevel);

sint32 Stats_GetRealCostCrystal(LegoObject_Type objType, LegoObject_ID objID, uint32 objLevel);

sint32 Stats_GetRealCostRefinedOre(LegoObject_Type objType, LegoObject_ID objID, uint32 objLevel);

/// CUSTOM:
bool Stats_GetRequiresConstructionBarriers(LegoObject_Type objType, LegoObject_ID objID);

// <LegoRR.exe @00469e10>
sint32 __cdecl StatsObject_GetCrystalDrain(LegoObject* liveObj);

// <LegoRR.exe @00469e40>
sint32 __cdecl StatsObject_GetCapacity(LegoObject* liveObj);

// <LegoRR.exe @00469e70>
sint32 __cdecl StatsObject_GetMaxCarry(LegoObject* liveObj);

// <LegoRR.exe @00469ea0>
sint32 __cdecl StatsObject_GetCarryStart(LegoObject* liveObj);

// <LegoRR.exe @00469ed0>
bool32 __cdecl StatsObject_SetObjectLevel(LegoObject* liveObj, uint32 newLevel);

// <LegoRR.exe @00469f70>
real32 __cdecl StatsObject_GetRouteSpeed(LegoObject* liveObj);

// <LegoRR.exe @00469f80>
real32 __cdecl StatsObject_GetDrillTimeType(LegoObject* liveObj, Lego_SurfaceType surfaceType);

// <LegoRR.exe @00469fa0>
real32 __cdecl StatsObject_GetRubbleCoef(LegoObject* liveObj);

// <LegoRR.exe @00469fc0>
real32 __cdecl StatsObject_GetWakeRadius(LegoObject* liveObj);

// <LegoRR.exe @00469fe0>
real32 __cdecl StatsObject_GetPathCoef(LegoObject* liveObj);

// <LegoRR.exe @0046a000>
real32 __cdecl StatsObject_GetCollRadius(LegoObject* liveObj);

// <LegoRR.exe @0046a010>
real32 __cdecl StatsObject_GetCollHeight(LegoObject* liveObj);

// <LegoRR.exe @0046a020>
real32 __cdecl StatsObject_GetPickSphere(LegoObject* liveObj);

// <LegoRR.exe @0046a030>
real32 __cdecl StatsObject_GetPainThreshold(LegoObject* liveObj);

// <LegoRR.exe @0046a050>
real32 __cdecl StatsObject_GetAlertRadius(LegoObject* liveObj);

// <LegoRR.exe @0046a060>
const Size2F* __cdecl StatsObject_GetCollBox(LegoObject* liveObj);

// <LegoRR.exe @0046a070>
real32 __cdecl StatsObject_GetTrackDist(LegoObject* liveObj);

// <LegoRR.exe @0046a080>
real32 __cdecl StatsObject_GetHealthDecayRate(LegoObject* liveObj);

// <LegoRR.exe @0046a0a0>
real32 __cdecl StatsObject_GetEnergyDecayRate(LegoObject* liveObj);

// <LegoRR.exe @0046a0c0>
real32 __cdecl Stats_GetOxygenCoef(LegoObject_Type objType, LegoObject_ID objID);

// <LegoRR.exe @0046a0e0>
real32 __cdecl StatsObject_GetOxygenCoef(LegoObject* liveObj);

// <LegoRR.exe @0046a100>
sint32 __cdecl StatsObject_GetSurveyRadius(LegoObject* liveObj);

// <LegoRR.exe @0046a120>
StatsFlags1 __cdecl StatsObject_GetStatsFlags1(LegoObject* liveObj);

// <LegoRR.exe @0046a140>
StatsFlags2 __cdecl StatsObject_GetStatsFlags2(LegoObject* liveObj);

// <LegoRR.exe @0046a160>
StatsFlags3 __cdecl StatsObject_GetStatsFlags3(LegoObject* liveObj);

// <LegoRR.exe @0046a180>
StatsFlags1 __cdecl Stats_GetStatsFlags1(LegoObject_Type objType, LegoObject_ID objID);

// <LegoRR.exe @0046a1a0>
StatsFlags2 __cdecl Stats_GetStatsFlags2(LegoObject_Type objType, LegoObject_ID objID);

// <LegoRR.exe @0046a1c0>
StatsFlags3 __cdecl Stats_GetStatsFlags3(LegoObject_Type objType, LegoObject_ID objID);

// <LegoRR.exe @0046a1e0>
real32 __cdecl StatsObject_GetRepairValue(LegoObject* liveObj);

// <LegoRR.exe @0046a200>
uint32 __cdecl Stats_GetLevels(LegoObject_Type objType, LegoObject_ID objID);

// <LegoRR.exe @0046a220>
sint32 __cdecl Stats_GetWaterEntrances(LegoObject_Type objType, LegoObject_ID objID, uint32 objLevel);

// <LegoRR.exe @0046a250>
SFX_ID __cdecl StatsObject_GetDrillSoundType(LegoObject* liveObj, bool32 fade);

// <LegoRR.exe @0046a280>
SFX_ID __cdecl StatsObject_GetEngineSound(LegoObject* liveObj);

// <LegoRR.exe @0046a2a0>
real32 __cdecl StatsObject_GetRestPercent(LegoObject* liveObj);

// <LegoRR.exe @0046a2c0>
real32 __cdecl StatsObject_GetCarryMinHealth(LegoObject* liveObj);

// <LegoRR.exe @0046a2e0>
real32 __cdecl StatsObject_GetAttackRadius(LegoObject* liveObj);

// <LegoRR.exe @0046a300>
real32 __cdecl StatsObject_GetStampRadius(LegoObject* liveObj);

// <LegoRR.exe @0046a320>
uint32 __cdecl StatsObject_GetNumOfToolsCanCarry(LegoObject* liveObj);

// <LegoRR.exe @0046a340>
real32 __cdecl StatsObject_GetUpgradeTime(LegoObject* liveObj);

// <LegoRR.exe @0046a360>
real32 __cdecl StatsObject_GetFunctionCoef(LegoObject* liveObj);

// <LegoRR.exe @0046a380>
sint32 __cdecl Stats_GetUpgradeCostOre(LegoObject_Type objType, LegoObject_ID objID, LegoObject_UpgradeType upgradeType);

// <LegoRR.exe @0046a3b0>
sint32 __cdecl Stats_GetUpgradeCostStuds(LegoObject_Type objType, LegoObject_ID objID, LegoObject_UpgradeType upgradeType);

// <LegoRR.exe @0046a3e0>
bool32 __cdecl Stats_FindToolFromTaskType(AITask_Type taskType, OUT LegoObject_ToolType* toolType);

// <LegoRR.exe @0046a430>
real32 __cdecl StatsObject_GetFlocks_Height(LegoObject* liveObj);

// <LegoRR.exe @0046a450>
real32 __cdecl StatsObject_GetFlocks_Randomness(LegoObject* liveObj);

// <LegoRR.exe @0046a470>
real32 __cdecl StatsObject_GetFlocks_Turn(LegoObject* liveObj);

// <LegoRR.exe @0046a490>
real32 __cdecl StatsObject_GetFlocks_Tightness(LegoObject* liveObj);

// <LegoRR.exe @0046a4b0>
real32 __cdecl StatsObject_GetFlocks_Speed(LegoObject* liveObj);

// <LegoRR.exe @0046a4d0>
sint32 __cdecl StatsObject_GetFlocks_Size(LegoObject* liveObj);

// <LegoRR.exe @0046a4f0>
real32 __cdecl StatsObject_GetFlocks_GoalUpdate(LegoObject* liveObj);

// <LegoRR.exe @0046a510>
real32 __cdecl StatsObject_GetFlocks_AttackTime(LegoObject* liveObj);

// <LegoRR.exe @0046a530>
real32 __cdecl StatsObject_GetAwarenessRange(LegoObject* liveObj);

// <LegoRR.exe @0046a550>
real32 __cdecl StatsObject_GetPusherDist(LegoObject* liveObj);

// <LegoRR.exe @0046a570>
real32 __cdecl StatsObject_GetPusherDamage(LegoObject* liveObj);

// <LegoRR.exe @0046a590>
real32 __cdecl StatsObject_GetLaserDamage(LegoObject* liveObj);

// <LegoRR.exe @0046a5b0>
real32 __cdecl StatsObject_GetFreezerDamage(LegoObject* liveObj);

// <LegoRR.exe @0046a5d0>
real32 __cdecl StatsObject_GetFreezerTime(LegoObject* liveObj);

// <LegoRR.exe @0046a5f0>
void __cdecl StatsObject_Debug_ToggleSelfPowered(LegoObject* liveObj);

#pragma endregion

}
