// Stats.cpp : 
//

#include "../../engine/core/Errors.h"
#include "../../engine/core/Memory.h"
#include "../../engine/core/Utils.h"

#include "../Game.h"

#include "Stats.h"


// <LegoRR.exe @00408bb0>
//uint32 __cdecl Construction_GetBuildingBase(const char* name);
#define Construction_GetBuildingBase ((uint32(__cdecl*)(const char*))0x00408bb0)

#define LiveObject_UpdatePowerConsumption ((void(__cdecl*)(LegoObject*))0x0043c830)


/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @00503bd8>
LegoRR::Stats_Globs & LegoRR::statsGlobs = *(LegoRR::Stats_Globs*)0x00503bd8;

#pragma endregion


/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

using namespace Gods98;

#define Stats_ID(name) Config_ID(gameName, "Stats", Config_GetItemName(prop), name)
#define float10 long double

// <LegoRR.exe @00466aa0>
bool32 __cdecl LegoRR::Stats_Initialise(const Gods98::Config* config, const char* gameName)
{
    const Gods98::Config* prop;
    const char* arrayID = Config_ID(gameName, "Stats");
    const char* tempStr;
    char* str;
    char* argv[32] = { nullptr }; // dummy init
    
    for (prop = Config_FindArray(config, arrayID); prop != nullptr; prop = Config_GetNextItem(prop)) {

        LegoObject_Type type = LegoObject_None;
        sint32 id = 0;
        if (!Object_GetObjectByName(Config_GetItemName(prop), &type, &id, nullptr)) {
            Error_Warn(true, "Object name in Stats not found");
            continue;
        }

        if (statsGlobs.objectStats[type] == nullptr) {
            uint32 subtypeArraySize = Object_GetTypeCount(type) * sizeof(ObjectStats*);

            statsGlobs.objectStats[type] = (ObjectStats**)Gods98::Mem_Alloc(subtypeArraySize);
            std::memset(statsGlobs.objectStats[type], 0, subtypeArraySize);
        }

        /// REFACTOR: Moved to after Object_GetObjectByName and type array allocation
        uint32 levels = (uint32)Config_GetIntValue(config, Stats_ID("Levels"));
        Error_Fatal(levels > OBJECT_MAXLEVELS, "Cannot have levels greater than maximum in Stats");
        StatsFlags1 flags1 = StatsFlags1::STATS1_NONE;
        StatsFlags2 flags2 = StatsFlags2::STATS2_NONE;
        StatsFlags3 flags3 = StatsFlags3::STATS3_NONE;

        uint32 levelsArraySize = (sint32)levels * sizeof(ObjectStats);

        statsGlobs.objectLevels[type][id] = levels;
        statsGlobs.objectStats[type][id] = (ObjectStats*)Gods98::Mem_Alloc(levelsArraySize);
        std::memset(statsGlobs.objectStats[type][id], 0, levelsArraySize);


        if ((str = Config_GetStringValue(config, Stats_ID("RouteSpeed")))) {

            uint32 argcLevels = Gods98::Util_Tokenise(str, argv, ":");
            Error_Fatal(argcLevels < levels, "Not enough levels in Stats RouteSpeed");

            for (uint32 lvl = 0; lvl < levels; lvl++) {
                statsGlobs.objectStats[type][id][lvl].RouteSpeed = (real32)std::atof(argv[lvl]);
            }
            Gods98::Mem_Free(str);
        }

        /// REFACTOR: String duplication moved inside the loop
        const char* drillTimes[Lego_SurfaceType_Count] = { nullptr };
        drillTimes[Lego_SurfaceType_Soil] = Config_GetTempStringValue(config, Stats_ID("SoilDrillTime"));
        drillTimes[Lego_SurfaceType_Loose] = Config_GetTempStringValue(config, Stats_ID("LooseDrillTime"));
        drillTimes[Lego_SurfaceType_Medium] = Config_GetTempStringValue(config, Stats_ID("MedDrillTime"));
        drillTimes[Lego_SurfaceType_Hard] = Config_GetTempStringValue(config, Stats_ID("HardDrillTime"));
        drillTimes[Lego_SurfaceType_OreSeam] = Config_GetTempStringValue(config, Stats_ID("SeamDrillTime"));
        // SeamDrillTime applies to both OreSeam and CrystalSeam
        if (drillTimes[Lego_SurfaceType_OreSeam] != nullptr)
            drillTimes[Lego_SurfaceType_CrystalSeam] = drillTimes[Lego_SurfaceType_OreSeam];
        else
            drillTimes[Lego_SurfaceType_CrystalSeam] = nullptr;

        for (uint32 i = 0; i < (uint32)Lego_SurfaceType_Count; i++) {
            if (drillTimes[i] != nullptr) {
                /// REFACTOR: String duplication moved inside the loop
                str = Gods98::Util_StrCpy(drillTimes[i]);

                uint32 argcLevels = Gods98::Util_Tokenise(str, argv, ":");
                Error_Fatal(argcLevels < levels, "Not enough levels in Stats DrillTime");

                for (uint32 lvl = 0; lvl < levels; lvl++) {
                    real32 timeVal = ((real32)std::atof(argv[lvl]) * STANDARD_FRAMERATE);
                    statsGlobs.objectStats[type][id][lvl].DrillTimes[i] = timeVal;
                }
                Gods98::Mem_Free(str);
            }
        }


        //sint32 local_144;
        sint32 buildingBase = -1;
        if ((tempStr = Config_GetTempStringValue(config, Stats_ID("BuildingBase")))) {
            // This serves no purpose in LegoRR,
            // as no BuildingBase names are ever defined in the lookup table.
            buildingBase = Construction_GetBuildingBase(tempStr);
        }
            
        //sint32 local_d8 = std::atoi(pcVar4);
        sint32 costCrystal = Config_GetIntValue(config, Stats_ID("CostCrystal"));
            
        //sint32 local_d4 = std::atoi(pcVar4);
        sint32 costOre = Config_GetIntValue(config, Stats_ID("CostOre"));
            
        //sint32 local_124 = (char*)std::atoi(pcVar4);
        sint32 costRefinedOre = Config_GetIntValue(config, Stats_ID("CostRefinedOre"));

            
        //real32 local_120 = (float)fVar14;
        real32 trackDist = Config_GetRealValue(config, Stats_ID("TrackDist"));
            
        //real32 local_114 = (float)fVar14;
        real32 alertRadius = Config_GetRealValue(config, Stats_ID("AlertRadius"));
            
        //real32 local_118 = (float)fVar14;
        real32 collHeight = Config_GetRealValue(config, Stats_ID("CollHeight"));

        //real32 local_110 = (float)fVar14;
        real32 healthDecayRate = Config_GetRealValue(config, Stats_ID("HealthDecayRate"));
            
        //real32 local_100 = (float)fVar14;
        real32 energyDecayRate = Config_GetRealValue(config, Stats_ID("EnergyDecayRate"));

        //real32 local_130 = (float)fVar14;
        real32 pickSphere = Config_GetRealValue(config, Stats_ID("PickSphere"));
            
            
        SFX_Type drillSound; //SFX_Type local_e0;
        SFX_Type drillFadeSound; //SFX_Type local_e8;
        SFX_Type engineSound; //SFX_Type local_e4;
        if (!SFX_GetType(Config_GetTempStringValue(config, Stats_ID("DrillSound")), &drillSound)) {
            drillSound = SFX_Type::SFX_Drill;
        }
        if (!SFX_GetType(Config_GetTempStringValue(config, Stats_ID("DrillFadeSound")), &drillFadeSound)) {
            drillFadeSound = SFX_Type::SFX_DrillFade;
        }
            
        if (!SFX_GetType(Config_GetTempStringValue(config, Stats_ID("EngineSound")), &engineSound)) {
            engineSound = SFX_Type::SFX_NULL;
        }

        //real32 local_10c = (float)fVar14;
        real32 restPercent = Config_GetRealValue(config, Stats_ID("RestPercent"));

        //real32 local_104 = (float)fVar14;
        real32 carryMinHealth = Config_GetRealValue(config, Stats_ID("CarryMinHealth"));

        //real32 local_fc = (float)fVar14;
        real32 stampRadius = Config_GetRealValue(config, Stats_ID("StampRadius"));

        //real32 local_f8 = (float)fVar14;
        real32 attackRadius = Config_GetRealValue(config, Stats_ID("AttackRadius"));

        //sint32 local_f4 = std::atoi(pcVar4);
        sint32 waterEntrances = Config_GetIntValue(config, Stats_ID("WaterEntrances"));

        //real32 local_11c = (float)fVar14;
        real32 rubbleCoef = Config_GetRealValue(config, Stats_ID("RubbleCoef"));

        //real32 local_138 = (float)fVar14;
        real32 pathCoef = Config_GetRealValue(config, Stats_ID("PathCoef"));

        //real32 local_108 = (float)fVar14;
        real32 oxygenCoef = Config_GetRealValue(config, Stats_ID("OxygenCoef"));

        //sint32 local_dc = std::atoi(pcVar4);
        sint32 capacity = Config_GetIntValue(config, Stats_ID("Capacity"));

        if (rubbleCoef == 0.0f) rubbleCoef = 1.0f;
        if (pathCoef   == 0.0f) pathCoef   = 1.0f;


        //real32 local_12c;
        //Size2F { local_f0, local_ec };
        Size2F collBox = { 0.0f, 0.0f }; // null init for when CollRadius != 0.0f
        real32 collRadius = Config_GetRealValue(config, Stats_ID("CollRadius"));
        if (collRadius != 0.0f) {
            flags1 |= StatsFlags1::STATS1_COLLRADIUS;
        }
        else if ((str = Config_GetStringValue(config, Stats_ID("CollBox")))) {
            // Only use CollBox if CollRadius is missing or 0.0f
            Gods98::Util_Tokenise(str, argv, ",");
            collBox.width  = (real32)std::atof(argv[0]);
            collBox.height = (real32)std::atof(argv[1]);
                
            //local_12c = std::max(local_f0, local_ec) * 0.5f;
            collRadius = std::max(collBox.width, collBox.height) * 0.5f;
            flags1 |= StatsFlags1::STATS1_COLLBOX;

            /// FIX APPLY: Missing memory cleanup
            Gods98::Mem_Free(str);
        }

        // Apply unleveled stats
        for (uint32 lvl = 0; lvl < levels; lvl++) {
            statsGlobs.objectStats[type][id][lvl].CollRadius = collRadius; // local_12c
            statsGlobs.objectStats[type][id][lvl].AlertRadius = alertRadius; // local_114
            statsGlobs.objectStats[type][id][lvl].CollBox = collBox; // { local_f0, local_ec }

            statsGlobs.objectStats[type][id][lvl].PickSphere = pickSphere; // local_130
            statsGlobs.objectStats[type][id][lvl].TrackDist = trackDist; // local_120
            statsGlobs.objectStats[type][id][lvl].CollHeight = collHeight; // local_118

            statsGlobs.objectStats[type][id][lvl].HealthDecayRate = healthDecayRate; // local_110
            statsGlobs.objectStats[type][id][lvl].EnergyDecayRate = energyDecayRate; // local_100

            statsGlobs.objectStats[type][id][lvl].BuildingBase = buildingBase; // local_144

            statsGlobs.objectStats[type][id][lvl].DrillSound = drillSound; // local_e0
            statsGlobs.objectStats[type][id][lvl].DrillFadeSound = drillFadeSound; // local_e8
            statsGlobs.objectStats[type][id][lvl].EngineSound = engineSound; // local_e4

            statsGlobs.objectStats[type][id][lvl].RestPercent = restPercent; // local_10c
            statsGlobs.objectStats[type][id][lvl].CarryMinHealth = carryMinHealth; // local_104
            statsGlobs.objectStats[type][id][lvl].StampRadius = stampRadius; // local_fc
            statsGlobs.objectStats[type][id][lvl].AttackRadius = attackRadius; // local_f8

            statsGlobs.objectStats[type][id][lvl].WaterEntrances = waterEntrances; // local_f4
            statsGlobs.objectStats[type][id][lvl].RubbleCoef = rubbleCoef; // local_11c
            statsGlobs.objectStats[type][id][lvl].PathCoef = pathCoef; // local_138
            statsGlobs.objectStats[type][id][lvl].OxygenCoef = oxygenCoef; // local_108

            statsGlobs.objectStats[type][id][lvl].Capacity = capacity; // local_dc
            statsGlobs.objectStats[type][id][lvl].CostCrystal = costCrystal; // local_d8
            statsGlobs.objectStats[type][id][lvl].CostOre = costOre; // local_d4
            statsGlobs.objectStats[type][id][lvl].CostRefinedOre = costRefinedOre; // local_124
        }


        if ((str = Config_GetStringValue(config, Stats_ID("SingleWidthDig")))) {
            uint32 argcLevels = Gods98::Util_Tokenise(str, argv, ":");
            Error_Fatal(argcLevels < levels, "Not enough levels in Stats SingleWidthDig");

            for (uint32 lvl = 0; lvl < levels; lvl++) {
                BoolTri boolVal = Gods98::Util_GetBoolFromString(argv[lvl]);
                if (boolVal == BoolTri::BOOL3_TRUE) // default: FALSE
                    statsGlobs.objectStats[type][id][lvl].flags1 |= StatsFlags1::STATS1_SINGLEWIDTHDIG;
            }
            Gods98::Mem_Free(str);
        }

        if ((str = Config_GetStringValue(config, Stats_ID("RepairValue")))) {
            uint32 argcLevels = Gods98::Util_Tokenise(str, argv, ":");
            Error_Fatal(argcLevels < levels, "Not enough levels in Stats RepairValue");

            for (uint32 lvl = 0; lvl < levels; lvl++) {
                statsGlobs.objectStats[type][id][lvl].RepairValue = (real32)std::atof(argv[lvl]);
            }
            Gods98::Mem_Free(str);
        }

        if ((str = Config_GetStringValue(config, Stats_ID("SurveyRadius")))) {
            uint32 argcLevels = Gods98::Util_Tokenise(str, argv, ":");
            Error_Fatal(argcLevels < levels, "Not enough levels in Stats SurveyRadius");

            for (uint32 lvl = 0; lvl < levels; lvl++) {
                statsGlobs.objectStats[type][id][lvl].SurveyRadius = std::atoi(argv[lvl]);
            }
            Gods98::Mem_Free(str);
        }

        if ((str = Config_GetStringValue(config, Stats_ID("MaxCarry")))) {
            uint32 argcLevels = Gods98::Util_Tokenise(str, argv, ":");
            Error_Fatal(argcLevels < levels, "Not enough levels in Stats MaxCarry");

            for (uint32 lvl = 0; lvl < levels; lvl++) {
                statsGlobs.objectStats[type][id][lvl].MaxCarry = std::atoi(argv[lvl]);
            }
            Gods98::Mem_Free(str);
        }

        if ((str = Config_GetStringValue(config, Stats_ID("CarryStart")))) {
            uint32 argcLevels = Gods98::Util_Tokenise(str, argv, ":");
            Error_Fatal(argcLevels < levels, "Not enough levels in Stats CarryStart");

            for (uint32 lvl = 0; lvl < levels; lvl++) {
                statsGlobs.objectStats[type][id][lvl].CarryStart = std::atoi(argv[lvl]);
            }
            Gods98::Mem_Free(str);
        }

        if ((str = Config_GetStringValue(config, Stats_ID("CrystalDrain")))) {
            uint32 argcLevels = Gods98::Util_Tokenise(str, argv, ":");
            Error_Fatal(argcLevels < levels, "Not enough levels in Stats CrystalDrain");

            for (uint32 lvl = 0; lvl < levels; lvl++) {
                statsGlobs.objectStats[type][id][lvl].CrystalDrain = std::atoi(argv[lvl]);
            }
            Gods98::Mem_Free(str);
        }

        if ((str = Config_GetStringValue(config, Stats_ID("NumOfToolsCanCarry")))) {
            uint32 argcLevels = Gods98::Util_Tokenise(str, argv, ":");
            Error_Fatal(argcLevels < levels, "Not enough levels in Stats NumOfToolsCanCarry");

            for (uint32 lvl = 0; lvl < levels; lvl++) {
                statsGlobs.objectStats[type][id][lvl].NumOfToolsCanCarry = std::atoi(argv[lvl]);
            }
            Gods98::Mem_Free(str);
        }

        if ((str = Config_GetStringValue(config, Stats_ID("UpgradeTime")))) {
            uint32 argcLevels = Gods98::Util_Tokenise(str, argv, ":");
            Error_Fatal(argcLevels < levels, "Not enough levels in Stats UpgradeTime");

            for (uint32 lvl = 0; lvl < levels; lvl++) {
                real32 timeVal = ((real32)std::atof(argv[lvl]) * STANDARD_FRAMERATE);
                statsGlobs.objectStats[type][id][lvl].UpgradeTime = timeVal;
            }
            Gods98::Mem_Free(str);
        }

        if ((str = Config_GetStringValue(config, Stats_ID("FunctionCoef")))) {
            uint32 argcLevels = Gods98::Util_Tokenise(str, argv, ":");
            Error_Fatal(argcLevels < levels, "Not enough levels in Stats FunctionCoef");

            for (uint32 lvl = 0; lvl < levels; lvl++) {
                statsGlobs.objectStats[type][id][lvl].FunctionCoef = (real32)std::atof(argv[lvl]);
            }
            Gods98::Mem_Free(str);
        }


		// Special cases for "UpgradeCostOre" and "UpgradeCostStuds"
		// [Carry][Scan][Speed][Drill], levels are fed into objectStats array from RIGHT to LEFT
		// OBJECT MUST HAVE AT LEAST 4 LEVELS (OR ALLOCATE A LARGE ENOUGH ARRAY)
        if ((str = Config_GetStringValue(config, Stats_ID("UpgradeCostOre")))) {
            uint32 argcLevels = Gods98::Util_Tokenise(str, argv, ":");
			// Any levels items after 4th are ignored
            Error_Fatal(argcLevels < 4, "Must have 4 levels in Stats UpgradeCostOre");
            Error_Fatal(levels < 4, "Object must have at least 4 levels when using Stats UpgradeCostOre");

			// upg[rade] index is for argc, lvl index is for objectStats
            for (sint32 upg = 3, lvl = 0; upg >= 0; upg--, lvl++) {
                statsGlobs.objectStats[type][id][lvl].UpgradeCostOre = std::atoi(argv[upg]);
            }
            Gods98::Mem_Free(str);
        }
        if ((str = Config_GetStringValue(config, Stats_ID("UpgradeCostStuds")))) {
            uint32 argcLevels = Gods98::Util_Tokenise(str, argv, ":");
			// Any levels items after 4th are ignored
            Error_Fatal(argcLevels < 4, "Must have 4 levels in Stats UpgradeCostStuds");
            Error_Fatal(levels < 4, "Object must have at least 4 levels when using Stats UpgradeCostStuds");

			// upg[rade] index is for argc, lvl index is for objectStats
            for (sint32 upg = 3, lvl = 0; upg >= 0; upg--, lvl++) {
                statsGlobs.objectStats[type][id][lvl].UpgradeCostStuds = std::atoi(argv[upg]);
            }
            Gods98::Mem_Free(str);
        }



        // Start of StatsFlags1:
        if (Config_GetBoolOrFalse(config, Stats_ID("ProcessOre"))) // Refinery
            flags1 |= StatsFlags1::STATS1_PROCESSORE;

        if (Config_GetBoolOrFalse(config, Stats_ID("ProcessCrystal"))) // PowerStation (not power generating effect)
            flags1 |= StatsFlags1::STATS1_PROCESSCRYSTAL;

        if (Config_GetBoolOrFalse(config, Stats_ID("StoreObjects"))) // Hold resources
            flags1 |= StatsFlags1::STATS1_STOREOBJECTS;

        if (Config_GetBoolOrFalse(config, Stats_ID("ManTeleporter")))
            flags1 |= StatsFlags1::STATS1_MANTELEPORTER;

        if (Config_GetBoolOrFalse(config, Stats_ID("SmallTeleporter")))
            flags1 |= StatsFlags1::STATS1_SMALLTELEPORTER;

        if (Config_GetBoolOrFalse(config, Stats_ID("BigTeleporter")))
            flags1 |= StatsFlags1::STATS1_BIGTELEPORTER;

        if (Config_GetBoolOrFalse(config, Stats_ID("WaterTeleporter")))
            flags1 |= StatsFlags1::STATS1_WATERTELEPORTER;

        if (Config_GetBoolOrFalse(config, Stats_ID("SnaxULike"))) // SupportStation/Canteen food
            flags1 |= StatsFlags1::STATS1_SNAXULIKE;

        if (Config_GetBoolOrFalse(config, Stats_ID("ToolStore"))) // Hold tools
            flags1 |= StatsFlags1::STATS1_TOOLSTORE;

        if (Config_GetBoolOrFalse(config, Stats_ID("CauseSlip")))
            flags1 |= StatsFlags1::STATS1_CAUSESLIP;

        if (Config_GetBoolOrFalse(config, Stats_ID("RandomMove")))
            flags1 |= StatsFlags1::STATS1_RANDOMMOVE;

        if (Config_GetBoolOrFalse(config, Stats_ID("CanScare")))
            flags1 |= StatsFlags1::STATS1_CANSCARE;

        if (Config_GetBoolOrFalse(config, Stats_ID("CanScareScorpion")))
            flags1 |= StatsFlags1::STATS1_CANSCARESCORPION;

        if (Config_GetBoolOrFalse(config, Stats_ID("RandomEnterWall")))
            flags1 |= StatsFlags1::STATS1_RANDOMENTERWALL;

        if (Config_GetBoolOrFalse(config, Stats_ID("ScaredByPlayer")))
            flags1 |= StatsFlags1::STATS1_SCAREDBYPLAYER;

        if (Config_GetBoolOrFalse(config, Stats_ID("GrabMinifigure")))
            flags1 |= StatsFlags1::STATS1_GRABMINIFIGURE;

        if (Config_GetBoolOrFalse(config, Stats_ID("CanBeDriven")))
            flags1 |= StatsFlags1::STATS1_CANBEDRIVEN;

        if (Config_GetBoolOrFalse(config, Stats_ID("CanSteal")))
            flags1 |= StatsFlags1::STATS1_CANSTEAL;

        if (Config_GetBoolOrFalse(config, Stats_ID("CanClearRubble")))
            flags1 |= StatsFlags1::STATS1_CANCLEARRUBBLE;

        if (Config_GetBoolOrFalse(config, Stats_ID("RouteAvoidance")))
            flags1 |= StatsFlags1::STATS1_ROUTEAVOIDANCE;

        if (Config_GetBoolOrFalse(config, Stats_ID("BumpDamage")))
            flags1 |= StatsFlags1::STATS1_BUMPDAMAGE;

        if (Config_GetBoolOrFalse(config, Stats_ID("Flocks")))
            flags1 |= StatsFlags1::STATS1_FLOCKS;

        if (Config_GetBoolOrFalse(config, Stats_ID("Flocks_Debug")))
            flags1 |= StatsFlags1::STATS1_FLOCKS_DEBUG;

        if (Config_GetBoolOrFalse(config, Stats_ID("Flocks_Smooth")))
            flags1 |= StatsFlags1::STATS1_FLOCKS_SMOOTH;

        if (Config_GetBoolOrFalse(config, Stats_ID("CrossWater")))
            flags1 |= StatsFlags1::STATS1_CROSSWATER;

        if (Config_GetBoolOrFalse(config, Stats_ID("CrossLand")))
            flags1 |= StatsFlags1::STATS1_CROSSLAND;

        if (Config_GetBoolOrFalse(config, Stats_ID("Flocks_OnGround")))
            flags1 |= StatsFlags1::STATS1_FLOCKS_ONGROUND;

        if (Config_GetBoolOrFalse(config, Stats_ID("Flocks_QuickDestroy")))
            flags1 |= StatsFlags1::STATS1_FLOCKS_QUICKDESTROY;

        if (Config_GetBoolOrFalse(config, Stats_ID("Flocks_AnimateByPitch")))
            flags1 |= StatsFlags1::STATS1_FLOCKS_ANIMATEBYPITCH;


        // Start of StatsFlags2:
        if (Config_GetBoolOrFalse(config, Stats_ID("ScaredByBigBangs")))
            flags2 |= StatsFlags2::STATS2_SCAREDBYBIGBANGS;

        if (Config_GetBoolOrFalse(config, Stats_ID("UpgradeBuilding")))
            flags2 |= StatsFlags2::STATS2_UPGRADEBUILDING;

        if (Config_GetBoolOrFalse(config, Stats_ID("CanBeShotAt")))
            flags2 |= StatsFlags2::STATS2_CANBESHOTAT;

        if (Config_GetBoolOrFalse(config, Stats_ID("UseBigTeleporter")))
            flags2 |= StatsFlags2::STATS2_USEBIGTELEPORTER;

        if (Config_GetBoolOrFalse(config, Stats_ID("UseSmallTeleporter")))
            flags2 |= StatsFlags2::STATS2_USESMALLTELEPORTER;

        if (Config_GetBoolOrFalse(config, Stats_ID("UseWaterTeleporter")))
            flags2 |= StatsFlags2::STATS2_USEWATERTELEPORTER;

        if (Config_GetBoolOrFalse(config, Stats_ID("UseLegoManTeleporter")))
            flags2 |= StatsFlags2::STATS2_USELEGOMANTELEPORTER;

        if ((str = Config_GetStringValue(config, Stats_ID("TrainPilot")))) {
            uint32 argcLevels = Gods98::Util_Tokenise(str, argv, ":");
            Error_Fatal(argcLevels < levels, "Not enough levels in Stats TrainPilot");

            for (uint32 lvl = 0; lvl < levels; lvl++) {
                BoolTri boolVal = Gods98::Util_GetBoolFromString(argv[lvl]);
                if (boolVal == BoolTri::BOOL3_TRUE) // default: FALSE
                    statsGlobs.objectStats[type][id][lvl].flags2 |= StatsFlags2::STATS2_TRAINPILOT;
            }
            Gods98::Mem_Free(str);
        }

        if ((str = Config_GetStringValue(config, Stats_ID("TrainSailor")))) {
            uint32 argcLevels = Gods98::Util_Tokenise(str, argv, ":");
            Error_Fatal(argcLevels < levels, "Not enough levels in Stats TrainSailor");

            for (uint32 lvl = 0; lvl < levels; lvl++) {
                BoolTri boolVal = Gods98::Util_GetBoolFromString(argv[lvl]);
                if (boolVal == BoolTri::BOOL3_TRUE) // default: FALSE
                    statsGlobs.objectStats[type][id][lvl].flags2 |= StatsFlags2::STATS2_TRAINSAILOR;
            }
            Gods98::Mem_Free(str);
        }

        if ((str = Config_GetStringValue(config, Stats_ID("TrainDriver")))) {
            uint32 argcLevels = Gods98::Util_Tokenise(str, argv, ":");
            Error_Fatal(argcLevels < levels, "Not enough levels in Stats TrainDriver");

            for (uint32 lvl = 0; lvl < levels; lvl++) {
                BoolTri boolVal = Gods98::Util_GetBoolFromString(argv[lvl]);
                if (boolVal == BoolTri::BOOL3_TRUE) // default: FALSE
                    statsGlobs.objectStats[type][id][lvl].flags2 |= StatsFlags2::STATS2_TRAINDRIVER;
            }
            Gods98::Mem_Free(str);
        }

        if ((str = Config_GetStringValue(config, Stats_ID("TrainDynamite")))) {
            uint32 argcLevels = Gods98::Util_Tokenise(str, argv, ":");
            Error_Fatal(argcLevels < levels, "Not enough levels in Stats TrainDynamite");

            for (uint32 lvl = 0; lvl < levels; lvl++) {
                BoolTri boolVal = Gods98::Util_GetBoolFromString(argv[lvl]);
                if (boolVal == BoolTri::BOOL3_TRUE) // default: FALSE
                    statsGlobs.objectStats[type][id][lvl].flags2 |= StatsFlags2::STATS2_TRAINDYNAMITE;
            }
            Gods98::Mem_Free(str);
        }

        if ((str = Config_GetStringValue(config, Stats_ID("TrainRepair")))) {
            uint32 argcLevels = Gods98::Util_Tokenise(str, argv, ":");
            Error_Fatal(argcLevels < levels, "Not enough levels in Stats TrainRepair");

            for (uint32 lvl = 0; lvl < levels; lvl++) {
                BoolTri boolVal = Gods98::Util_GetBoolFromString(argv[lvl]);
                if (boolVal == BoolTri::BOOL3_TRUE) // default: FALSE
                    statsGlobs.objectStats[type][id][lvl].flags2 |= StatsFlags2::STATS2_TRAINREPAIR;
            }
            Gods98::Mem_Free(str);
        }

        if ((str = Config_GetStringValue(config, Stats_ID("TrainScanner")))) {
            uint32 argcLevels = Gods98::Util_Tokenise(str, argv, ":");
            Error_Fatal(argcLevels < levels, "Not enough levels in Stats TrainScanner");

            for (uint32 lvl = 0; lvl < levels; lvl++) {
                BoolTri boolVal = Gods98::Util_GetBoolFromString(argv[lvl]);
                if (boolVal == BoolTri::BOOL3_TRUE) // default: FALSE
                    statsGlobs.objectStats[type][id][lvl].flags2 |= StatsFlags2::STATS2_TRAINSCANNER;
            }
            Gods98::Mem_Free(str);
        }

        if (Config_GetBoolOrFalse(config, Stats_ID("Tracker"))) // Laser tracker(?)
            flags2 |= StatsFlags2::STATS2_TRACKER;

        if (Config_GetBoolOrFalse(config, Stats_ID("PowerBuilding")))
            flags2 |= StatsFlags2::STATS2_POWERBUILDING; //(STATS2_GENERATEPOWER|STATS2_SELFPOWERED)

        if (Config_GetBoolOrFalse(config, Stats_ID("SelfPowered")))
            flags2 |= StatsFlags2::STATS2_SELFPOWERED;

        if (Config_GetBoolOrFalse(config, Stats_ID("AttackPaths")))
            flags2 |= StatsFlags2::STATS2_ATTACKPATHS;

        if (Config_GetBoolOrFalse(config, Stats_ID("SplitOnZeroHealth")))
            flags2 |= StatsFlags2::STATS2_SPLITONZEROHEALTH;

        if (Config_GetBoolOrFalse(config, Stats_ID("CanBeHitByFence")))
            flags2 |= StatsFlags2::STATS2_CANBEHITBYFENCE;

        if (Config_GetBoolOrFalse(config, Stats_ID("CanDoubleSelect")))
            flags2 |= StatsFlags2::STATS2_CANDOUBLESELECT;

        if (Config_GetBoolOrFalse(config, Stats_ID("DrainPower")))
            flags2 |= StatsFlags2::STATS2_DRAINPOWER;

        if (Config_GetBoolOrFalse(config, Stats_ID("UseHoles")))
            flags2 |= StatsFlags2::STATS2_USEHOLES;

        if (Config_GetBoolOrFalse(config, Stats_ID("CrossLava")))
            flags2 |= StatsFlags2::STATS2_CROSSLAVA;

        if (Config_GetBoolOrFalse(config, Stats_ID("DamageCausesCallToArms")))
            flags2 |= StatsFlags2::STATS2_DAMAGECAUSESCALLTOARMS;

        if (Config_GetBoolOrFalse(config, Stats_ID("CanFreeze")))
            flags2 |= StatsFlags2::STATS2_CANFREEZE;

        if (Config_GetBoolOrFalse(config, Stats_ID("CanLaser")))
            flags2 |= StatsFlags2::STATS2_CANLASER;

        if (Config_GetBoolOrFalse(config, Stats_ID("CanPush")))
            flags2 |= StatsFlags2::STATS2_CANPUSH;

        if (Config_GetBoolOrFalse(config, Stats_ID("DontShowDamage")))
            flags2 |= StatsFlags2::STATS2_DONTSHOWDAMAGE;

        if (Config_GetBoolOrFalse(config, Stats_ID("RemoveReinforcement")))
            flags2 |= StatsFlags2::STATS2_REMOVEREINFORCEMENT;

        if (Config_GetBoolOrFalse(config, Stats_ID("DontShowOnRadar")))
            flags2 |= StatsFlags2::STATS2_DONTSHOWONRADAR;

        if (Config_GetBoolOrFalse(config, Stats_ID("InvisibleDriver")))
            flags2 |= StatsFlags2::STATS2_INVISIBLEDRIVER;

        if (Config_GetBoolOrFalse(config, Stats_ID("Unselectable")))
            flags2 |= StatsFlags2::STATS2_UNSELECTABLE;


        // Start of StatsFlags3:
        if (Config_GetBoolOrFalse(config, Stats_ID("CarryVehicles")))
            flags3 |= StatsFlags3::STATS3_CARRYVEHICLES;

        if (Config_GetBoolOrFalse(config, Stats_ID("VehicleCanBeCarried")))
            flags3 |= StatsFlags3::STATS3_VEHICLECANBECARRIED;

        if (Config_GetBoolOrFalse(config, Stats_ID("CanStrafe")))
            flags3 |= StatsFlags3::STATS3_CANSTRAFE;

        if (Config_GetBoolOrFalse(config, Stats_ID("ClassAsLarge")))
            flags3 |= StatsFlags3::STATS3_CLASSASLARGE;

        if (Config_GetBoolOrFalse(config, Stats_ID("GetOutAtLand")))
            flags3 |= StatsFlags3::STATS3_GETOUTATLAND;

        if (Config_GetBoolOrFalse(config, Stats_ID("GetInAtLand")))
            flags3 |= StatsFlags3::STATS3_GETINATLAND;

        if (Config_GetBoolOrFalse(config, Stats_ID("TakeCarryingDrivers")))
            flags3 |= StatsFlags3::STATS3_TAKECARRYINGDRIVERS;

        if (Config_GetBoolOrFalse(config, Stats_ID("ShowHealthBar")))
            flags3 |= StatsFlags3::STATS3_SHOWHEALTHBAR;

        if (Config_GetBoolOrFalse(config, Stats_ID("NeedsPilot")))
            flags3 |= StatsFlags3::STATS3_NEEDSPILOT;

        if (Config_GetBoolOrFalse(config, Stats_ID("EnterToolStore")))
            flags3 |= StatsFlags3::STATS3_ENTERTOOLSTORE;


        //real32 local_138;
        real32 flocks_Turn = Config_GetRealValue(config, Stats_ID("Flocks_Turn"));
        if (flocks_Turn == 0.0f) flocks_Turn = 0.06f;

        //real32 local_11c;
        real32 flocks_Speed = Config_GetRealValue(config, Stats_ID("Flocks_Speed"));
        if (flocks_Speed == 0.0f) flocks_Speed = 2.0f;

        //real32 local_12c;
        real32 flocks_Tightness = Config_GetRealValue(config, Stats_ID("Flocks_Tightness"));
        if (flocks_Tightness == 0.0f) flocks_Tightness = 2.0f;

        //real32 local_108;
        real32 flocks_GoalUpdate = Config_GetRealValue(config, Stats_ID("Flocks_GoalUpdate"));
        if (flocks_GoalUpdate == 0.0f) flocks_GoalUpdate = 2.0f;

        //real32 local_f4;
        real32 flocks_Height = Config_GetRealValue(config, Stats_ID("Flocks_Height"));
        if (flocks_Height == 0.0f) flocks_Height = 30.0f;

        //real32 local_f8;
        real32 flocks_Randomness = Config_GetRealValue(config, Stats_ID("Flocks_Randomness"));
        if (flocks_Randomness == 0.0f) flocks_Randomness = 2.0f;


        //sint32 local_114; // (temp: sint32 local_d0)
        sint32 flocks_Size = Config_GetIntValue(config, Stats_ID("Flocks_Size"));
        if (flocks_Size == 0) flocks_Size = 5;


        //real32 local_fc;
        real32 flocks_AttackTime = Config_GetRealValue(config, Stats_ID("Flocks_AttackTime"));
        if (flocks_AttackTime == 0.0f) flocks_AttackTime = 300.0f;

        //real32 local_104;
        real32 awarenessRange = Config_GetRealValue(config, Stats_ID("AwarenessRange"));
        if (awarenessRange == 0.0f) awarenessRange = 0.0f;

        //real32 local_10c;
        real32 painThreshold = Config_GetRealValue(config, Stats_ID("PainThreshold"));
        if (painThreshold == 0.0f) painThreshold = 0.0f;

        //real32 local_130;
        real32 freezerTime = Config_GetRealValue(config, Stats_ID("FreezerTime"));
        if (freezerTime == 0.0f) freezerTime = 0.0f;

        //real32 local_120;
        real32 freezerDamage = Config_GetRealValue(config, Stats_ID("FreezerDamage"));
        if (freezerDamage == 0.0f) freezerDamage = 0.0f;

        //real32 local_110;
        real32 pusherDist = Config_GetRealValue(config, Stats_ID("PusherDist"));
        if (pusherDist == 0.0f) pusherDist = 0.0f;

        //real32 local_100;
        real32 pusherDamage = Config_GetRealValue(config, Stats_ID("PusherDamage"));
        if (pusherDamage == 0.0f) pusherDamage = 0.0f;

        //real32 local_118;
        real32 laserDamage = Config_GetRealValue(config, Stats_ID("LaserDamage"));
        if (laserDamage == 0.0f) laserDamage = 0.0f;

        //real32 local_144;
        real32 wakeRadius = Config_GetRealValue(config, Stats_ID("WakeRadius"));
        if (wakeRadius == 0.0f) wakeRadius = 20.0f;

        //real32 fVar14; // (register)
        real32 randomMoveTime = Config_GetRealValue(config, Stats_ID("RandomMoveTime"));
        if (randomMoveTime == 0.0f) randomMoveTime = 200.0f;


        // Apply more unleveled stats
        for (uint32 lvl = 0; lvl < levels; lvl++) {
            statsGlobs.objectStats[type][id][lvl].flags1 |= flags1;
            statsGlobs.objectStats[type][id][lvl].flags2 |= flags2;
            statsGlobs.objectStats[type][id][lvl].flags3 |= flags3;

            statsGlobs.objectStats[type][id][lvl].RandomMoveTime = randomMoveTime; // fVar14
            statsGlobs.objectStats[type][id][lvl].Flocks_Turn = flocks_Turn; // local_138
            statsGlobs.objectStats[type][id][lvl].Flocks_Speed = flocks_Speed; // local_11c
            statsGlobs.objectStats[type][id][lvl].Flocks_Tightness = flocks_Tightness; // local_12c
            statsGlobs.objectStats[type][id][lvl].Flocks_GoalUpdate = flocks_GoalUpdate; // local_108
            statsGlobs.objectStats[type][id][lvl].Flocks_Height = flocks_Height; // local_f4
            statsGlobs.objectStats[type][id][lvl].Flocks_Randomness = flocks_Randomness; // local_f8
            statsGlobs.objectStats[type][id][lvl].Flocks_Size = flocks_Size; // local_114
            statsGlobs.objectStats[type][id][lvl].Flocks_AttackTime = flocks_AttackTime; // local_fc
            statsGlobs.objectStats[type][id][lvl].AwarenessRange = awarenessRange; // local_104

            statsGlobs.objectStats[type][id][lvl].PainThreshold = painThreshold; // local_10c
            statsGlobs.objectStats[type][id][lvl].PusherDamage = pusherDamage; // local_100
            statsGlobs.objectStats[type][id][lvl].PusherDist = pusherDist; // local_110
            statsGlobs.objectStats[type][id][lvl].LaserDamage = laserDamage; // local_118
            statsGlobs.objectStats[type][id][lvl].FreezerDamage = freezerDamage; // local_120
            statsGlobs.objectStats[type][id][lvl].FreezerTime = freezerTime; // local_130
            statsGlobs.objectStats[type][id][lvl].WakeRadius = wakeRadius; // local_144
        }
    }
    
    Stats_AddToolTaskType(LegoObject_ToolType_Drill, AITask_Type_Dig);
    Stats_AddToolTaskType(LegoObject_ToolType_Spade, AITask_Type_Clear);
    Stats_AddToolTaskType(LegoObject_ToolType_Hammer, AITask_Type_Reinforce);
    Stats_AddToolTaskType(LegoObject_ToolType_Spanner, AITask_Type_Repair);

    return true;
}

// <LegoRR.exe @00469d50>
void __cdecl LegoRR::Stats_AddToolTaskType(LegoRR::LegoObject_ToolType toolType, LegoRR::AITask_Type taskType)
{
	ToolStats* toolStats = &statsGlobs.toolStats[toolType];

	toolStats->taskTypes[toolStats->taskCount++] = taskType;
}

// <LegoRR.exe @00469d80>
sint32 __cdecl LegoRR::Stats_GetCostOre(LegoRR::LegoObject_Type objType, sint32 objID, sint32 objLevel)
{
	return statsGlobs.objectStats[objType][objID][objLevel].CostOre;
}

// <LegoRR.exe @00469db0>
sint32 __cdecl LegoRR::Stats_GetCostCrystal(LegoRR::LegoObject_Type objType, sint32 objID, sint32 objLevel)
{
	return statsGlobs.objectStats[objType][objID][objLevel].CostCrystal;
}

// <LegoRR.exe @00469de0>
sint32 __cdecl LegoRR::Stats_GetCostRefinedOre(LegoRR::LegoObject_Type objType, sint32 objID, sint32 objLevel)
{
	return statsGlobs.objectStats[objType][objID][objLevel].CostRefinedOre;
}

// <LegoRR.exe @00469e10>
sint32 __cdecl LegoRR::StatsObject_GetCrystalDrain(LegoRR::LegoObject* liveObj)
{
	return statsGlobs.objectStats[liveObj->type][liveObj->id][liveObj->objLevel].CrystalDrain;
}

// <LegoRR.exe @00469e40>
sint32 __cdecl LegoRR::StatsObject_GetCapacity(LegoRR::LegoObject* liveObj)
{
	return statsGlobs.objectStats[liveObj->type][liveObj->id][liveObj->objLevel].Capacity;
}

// <LegoRR.exe @00469e70>
sint32 __cdecl LegoRR::StatsObject_GetMaxCarry(LegoRR::LegoObject* liveObj)
{
	return statsGlobs.objectStats[liveObj->type][liveObj->id][liveObj->objLevel].MaxCarry;
}

// <LegoRR.exe @00469ea0>
sint32 __cdecl LegoRR::StatsObject_GetCarryStart(LegoRR::LegoObject* liveObj)
{
	return statsGlobs.objectStats[liveObj->type][liveObj->id][liveObj->objLevel].CarryStart;
}

// <LegoRR.exe @00469ed0>
bool32 __cdecl LegoRR::StatsObject_SetObjectLevel(LegoRR::LegoObject* liveObj, uint32 newLevel)
{
	//ObjectType type = liveObj->type;
	//sint32 id = liveObj->id;
	uint32 oldLevel = liveObj->objLevel;
	if (newLevel < statsGlobs.objectLevels[liveObj->type][liveObj->id]) {
		if (newLevel != oldLevel) {
			objectGlobs.objectPrevLevels[liveObj->type][liveObj->id][oldLevel]++;
			objectGlobs.objectTotalLevels[liveObj->type][liveObj->id][newLevel]++;
		}
		liveObj->objLevel = newLevel;
		liveObj->stats = &statsGlobs.objectStats[liveObj->type][liveObj->id][newLevel];
		return true;
	}
	return false;
}

// <LegoRR.exe @00469f70>
real32 __cdecl LegoRR::StatsObject_GetRouteSpeed(LegoRR::LegoObject* liveObj)
{
	return liveObj->stats->RouteSpeed;
}

// <LegoRR.exe @00469f80>
real32 __cdecl LegoRR::StatsObject_GetDrillTimeType(LegoRR::LegoObject* liveObj, LegoRR::Lego_SurfaceType surfaceType)
{
	return liveObj->stats->DrillTimes[surfaceType];
}

// <LegoRR.exe @00469fa0>
real32 __cdecl LegoRR::StatsObject_GetRubbleCoef(LegoRR::LegoObject* liveObj)
{
	return liveObj->stats->RubbleCoef;
}

// <LegoRR.exe @00469fc0>
real32 __cdecl LegoRR::StatsObject_GetWakeRadius(LegoRR::LegoObject* liveObj)
{
	return liveObj->stats->WakeRadius;
}

// <LegoRR.exe @00469fe0>
real32 __cdecl LegoRR::StatsObject_GetPathCoef(LegoRR::LegoObject* liveObj)
{
	return liveObj->stats->PathCoef;
}

// <LegoRR.exe @0046a000>
real32 __cdecl LegoRR::StatsObject_GetCollRadius(LegoRR::LegoObject* liveObj)
{
	return liveObj->stats->CollRadius;
}

// <LegoRR.exe @0046a010>
real32 __cdecl LegoRR::StatsObject_GetCollHeight(LegoRR::LegoObject* liveObj)
{
	return liveObj->stats->CollHeight;
}

// <LegoRR.exe @0046a020>
real32 __cdecl LegoRR::StatsObject_GetPickSphere(LegoRR::LegoObject* liveObj)
{
	return liveObj->stats->PickSphere;
}

// <LegoRR.exe @0046a030>
real32 __cdecl LegoRR::StatsObject_GetPainThreshold(LegoRR::LegoObject* liveObj)
{
	return liveObj->stats->PainThreshold;
}

// <LegoRR.exe @0046a050>
real32 __cdecl LegoRR::StatsObject_GetAlertRadius(LegoRR::LegoObject* liveObj)
{
	return liveObj->stats->AlertRadius;
}

// <LegoRR.exe @0046a060>
const Size2F* __cdecl LegoRR::StatsObject_GetCollBox(LegoRR::LegoObject* liveObj)
{
	return &liveObj->stats->CollBox;
}

// <LegoRR.exe @0046a070>
real32 __cdecl LegoRR::StatsObject_GetTrackDist(LegoRR::LegoObject* liveObj)
{
	return liveObj->stats->TrackDist;
}

// <LegoRR.exe @0046a080>
real32 __cdecl LegoRR::StatsObject_GetHealthDecayRate(LegoRR::LegoObject* liveObj)
{
	return liveObj->stats->HealthDecayRate;
}

// <LegoRR.exe @0046a0a0>
real32 __cdecl LegoRR::StatsObject_GetEnergyDecayRate(LegoRR::LegoObject* liveObj)
{
	return liveObj->stats->EnergyDecayRate;
}

// <LegoRR.exe @0046a0c0>
real32 __cdecl LegoRR::Stats_GetOxygenCoef(LegoRR::LegoObject_Type objType, sint32 objID)
{
	return statsGlobs.objectStats[objType][objID]->OxygenCoef;
}

// <LegoRR.exe @0046a0e0>
real32 __cdecl LegoRR::StatsObject_GetOxygenCoef(LegoRR::LegoObject* liveObj)
{
	return liveObj->stats->OxygenCoef;
}

// <LegoRR.exe @0046a100>
sint32 __cdecl LegoRR::StatsObject_GetSurveyRadius(LegoRR::LegoObject* liveObj)
{
	return liveObj->stats->SurveyRadius;
}

// <LegoRR.exe @0046a120>
LegoRR::StatsFlags1 __cdecl LegoRR::StatsObject_GetStatsFlags1(LegoRR::LegoObject* liveObj)
{
	return liveObj->stats->flags1;
}

// <LegoRR.exe @0046a140>
LegoRR::StatsFlags2 __cdecl LegoRR::StatsObject_GetStatsFlags2(LegoRR::LegoObject* liveObj)
{
	return liveObj->stats->flags2;
}

// <LegoRR.exe @0046a160>
LegoRR::StatsFlags3 __cdecl LegoRR::StatsObject_GetStatsFlags3(LegoRR::LegoObject* liveObj)
{
	return liveObj->stats->flags3;
}

// <LegoRR.exe @0046a180>
LegoRR::StatsFlags1 __cdecl LegoRR::Stats_GetStatsFlags1(LegoRR::LegoObject_Type objType, sint32 objID)
{
	return statsGlobs.objectStats[objType][objID]->flags1;
}

// <LegoRR.exe @0046a1a0>
LegoRR::StatsFlags2 __cdecl LegoRR::Stats_GetStatsFlags2(LegoRR::LegoObject_Type objType, sint32 objID)
{
	return statsGlobs.objectStats[objType][objID]->flags2;
}

// <LegoRR.exe @0046a1c0>
LegoRR::StatsFlags3 __cdecl LegoRR::Stats_GetStatsFlags3(LegoRR::LegoObject_Type objType, sint32 objID)
{
	return statsGlobs.objectStats[objType][objID]->flags3;
}

// <LegoRR.exe @0046a1e0>
real32 __cdecl LegoRR::StatsObject_GetRepairValue(LegoRR::LegoObject* liveObj)
{
	return liveObj->stats->RepairValue;
}

// <LegoRR.exe @0046a200>
uint32 __cdecl LegoRR::Stats_GetLevels(LegoRR::LegoObject_Type objType, sint32 objID)
{
	return statsGlobs.objectLevels[objType][objID];
}

// <LegoRR.exe @0046a220>
sint32 __cdecl LegoRR::Stats_GetWaterEntrances(LegoRR::LegoObject_Type objType, sint32 objID, sint32 objLevel)
{
	return statsGlobs.objectStats[objType][objID][objLevel].WaterEntrances;
}

// <LegoRR.exe @0046a250>
LegoRR::SFX_Type __cdecl LegoRR::StatsObject_GetDrillSoundType(LegoRR::LegoObject* liveObj, bool32 fade)
{
	return (fade ? liveObj->stats->DrillFadeSound : liveObj->stats->DrillSound);
}

// <LegoRR.exe @0046a280>
LegoRR::SFX_Type __cdecl LegoRR::StatsObject_GetEngineSound(LegoRR::LegoObject* liveObj)
{
	return liveObj->stats->EngineSound;
}

// <LegoRR.exe @0046a2a0>
real32 __cdecl LegoRR::StatsObject_GetRestPercent(LegoRR::LegoObject* liveObj)
{
	return liveObj->stats->RestPercent;
}

// <LegoRR.exe @0046a2c0>
real32 __cdecl LegoRR::StatsObject_GetCarryMinHealth(LegoRR::LegoObject* liveObj)
{
	return liveObj->stats->CarryMinHealth;
}

// <LegoRR.exe @0046a2e0>
real32 __cdecl LegoRR::StatsObject_GetAttackRadius(LegoRR::LegoObject* liveObj)
{
	return liveObj->stats->AttackRadius;
}

// <LegoRR.exe @0046a300>
real32 __cdecl LegoRR::StatsObject_GetStampRadius(LegoRR::LegoObject* liveObj)
{
	return liveObj->stats->StampRadius;
}

// <LegoRR.exe @0046a320>
sint32 __cdecl LegoRR::StatsObject_GetNumOfToolsCanCarry(LegoRR::LegoObject* liveObj)
{
	return liveObj->stats->NumOfToolsCanCarry;
}

// <LegoRR.exe @0046a340>
real32 __cdecl LegoRR::StatsObject_GetUpgradeTime(LegoRR::LegoObject* liveObj)
{
	return liveObj->stats->UpgradeTime;
}

// <LegoRR.exe @0046a360>
real32 __cdecl LegoRR::StatsObject_GetFunctionCoef(LegoRR::LegoObject* liveObj)
{
	return liveObj->stats->FunctionCoef;
}

// <LegoRR.exe @0046a380>
sint32 __cdecl LegoRR::Stats_GetUpgradeCostOre(LegoRR::LegoObject_Type objType, sint32 objID, sint32 objLevel)
{
	return statsGlobs.objectStats[objType][objID][objLevel].UpgradeCostOre;
}

// <LegoRR.exe @0046a3b0>
sint32 __cdecl LegoRR::Stats_GetUpgradeCostStuds(LegoRR::LegoObject_Type objType, sint32 objID, sint32 objLevel)
{
	return statsGlobs.objectStats[objType][objID][objLevel].UpgradeCostStuds;
}

// <LegoRR.exe @0046a3e0>
bool32 __cdecl LegoRR::Stats_FindToolFromTaskType(LegoRR::AITask_Type taskType, OUT LegoRR::LegoObject_ToolType* toolType)
{
	for (uint32 i = 0; i < (uint32)LegoObject_ToolType_Count; i++) {
		const ToolStats* toolStats = &statsGlobs.toolStats[i];
		for (uint32 j = 0; j < toolStats->taskCount; j++) {
			if (taskType == toolStats->taskTypes[j]) {
				*toolType = (LegoObject_ToolType)i;
				return true;
			}
		}
	}
	return false;
}

// <LegoRR.exe @0046a430>
real32 __cdecl LegoRR::StatsObject_GetFlocks_Height(LegoRR::LegoObject* liveObj)
{
	return liveObj->stats->Flocks_Height;
}

// <LegoRR.exe @0046a450>
real32 __cdecl LegoRR::StatsObject_GetFlocks_Randomness(LegoRR::LegoObject* liveObj)
{
	return liveObj->stats->Flocks_Randomness;
}

// <LegoRR.exe @0046a470>
real32 __cdecl LegoRR::StatsObject_GetFlocks_Turn(LegoRR::LegoObject* liveObj)
{
	return liveObj->stats->Flocks_Turn;
}

// <LegoRR.exe @0046a490>
real32 __cdecl LegoRR::StatsObject_GetFlocks_Tightness(LegoRR::LegoObject* liveObj)
{
	return liveObj->stats->Flocks_Tightness;
}

// <LegoRR.exe @0046a4b0>
real32 __cdecl LegoRR::StatsObject_GetFlocks_Speed(LegoRR::LegoObject* liveObj)
{
	return liveObj->stats->Flocks_Speed;
}

// <LegoRR.exe @0046a4d0>
sint32 __cdecl LegoRR::StatsObject_GetFlocks_Size(LegoRR::LegoObject* liveObj)
{
	return liveObj->stats->Flocks_Size;
}

// <LegoRR.exe @0046a4f0>
real32 __cdecl LegoRR::StatsObject_GetFlocks_GoalUpdate(LegoRR::LegoObject* liveObj)
{
	return liveObj->stats->Flocks_GoalUpdate;
}

// <LegoRR.exe @0046a510>
real32 __cdecl LegoRR::StatsObject_GetFlocks_AttackTime(LegoRR::LegoObject* liveObj)
{
	return liveObj->stats->Flocks_AttackTime;
}

// <LegoRR.exe @0046a530>
real32 __cdecl LegoRR::StatsObject_GetAwarenessRange(LegoRR::LegoObject* liveObj)
{
	return liveObj->stats->AwarenessRange;
}

// <LegoRR.exe @0046a550>
real32 __cdecl LegoRR::StatsObject_GetPusherDist(LegoRR::LegoObject* liveObj)
{
	return liveObj->stats->PusherDist;
}

// <LegoRR.exe @0046a570>
real32 __cdecl LegoRR::StatsObject_GetPusherDamage(LegoRR::LegoObject* liveObj)
{
	return liveObj->stats->PusherDamage;
}

// <LegoRR.exe @0046a590>
real32 __cdecl LegoRR::StatsObject_GetLaserDamage(LegoRR::LegoObject* liveObj)
{
	return liveObj->stats->LaserDamage;
}

// <LegoRR.exe @0046a5b0>
real32 __cdecl LegoRR::StatsObject_GetFreezerDamage(LegoRR::LegoObject* liveObj)
{
	return liveObj->stats->FreezerDamage;
}

// <LegoRR.exe @0046a5d0>
real32 __cdecl LegoRR::StatsObject_GetObjectFreezerTime(LegoRR::LegoObject* liveObj)
{
	return liveObj->stats->FreezerTime;
}

// <LegoRR.exe @0046a5f0>
void __cdecl LegoRR::StatsObject_Debug_ToggleObjectPower(LegoRR::LegoObject* liveObj)
{
	/// WARNING: This actually modifies the stats for all objects of this type:index:level!
	if (!(liveObj->stats->flags2 & STATS2_SELFPOWERED))
		liveObj->stats->flags2 |= STATS2_SELFPOWERED;
	else
		liveObj->stats->flags2 &= ~STATS2_SELFPOWERED;

	LiveObject_UpdatePowerConsumption(liveObj);
}


#pragma endregion
