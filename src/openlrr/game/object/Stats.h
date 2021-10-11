// Stats.h : 
//

#pragma once

#include "../../common.h"
#include "../../engine/colour.h"
#include "../../engine/geometry.h"
#include "../../engine/undefined.h"

#include "../../engine/gfx/Containers.h"
#include "../../engine/drawing/Images.h"

#include "../audio/SFX.h"


namespace LegoRR
{; // !<---

/**********************************************************************************
 ******** Forward Declarations
 **********************************************************************************/

#pragma region Forward Declarations

enum AITaskType : sint32;
struct LegoObject;

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
    /*000,4*/       undefined4 field_0;
    /*004,4*/       undefined4 field_4;
    /*008,4*/       undefined4 field_8;
    /*00c,4*/       undefined4 field_c;
    /*010,4*/       undefined4 field_10;
    /*014,4*/       undefined4 field_14;
    /*018,4*/       undefined4 field_18;
    /*01c,4*/       undefined4 field_1c;
    /*020,4*/       real32 RouteSpeed; // (leveled)
    /*024,48*/      real32 DrillTimes[18]; // (leveled, table of surface type times)
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
    /*0c0,4*/       SFXType DrillSound; // (from: FUN_00464f30, invalid: 2 "SFX_Drill")
    /*0c4,4*/       SFXType DrillFadeSound; // (from: FUN_00464f30, invalid: 3 "SFX_DrillFade")
    /*0c8,4*/       SFXType EngineSound; // (from: FUN_00464f30, invalid: 0 "SFX_Bodge")
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
    /*104,4*/       undefined4 field_104;
    /*108,4*/       undefined4 field_108;
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
    /*00,c*/        AITaskType taskTypes[3]; // Up to 3 AITaskTypes can be associated with a tool
    /*0c,4*/        uint32 taskCount;
    /*10*/
};
assert_sizeof(ToolStats, 0x10);


struct Stats_Globs // [LegoRR/Stats.c|struct:0x5b0|tags:GLOBS]
{
    /*000,50*/      ObjectStats** objectStats[20]; // [objType:20][*objIndex][*objLevel]
    /*050,4b0*/     uint32 objectLevels[20][15]; // [objType:20][objIndex:15]
    /*500,b0*/      ToolStats toolStats[11]; // [toolType:11]
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

#pragma endregion

}