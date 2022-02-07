// Game.h : 
//

#pragma once

#include "../common.h"
#include "../engine/colour.h"
#include "../engine/geometry.h"
#include "../engine/undefined.h"

#include "../engine/core/Config.h"
#include "../engine/drawing/Fonts.h"
#include "../engine/drawing/Images.h"
#include "../engine/drawing/TextWindow.h"
#include "../engine/gfx/Containers.h"
#include "../engine/gfx/Viewports.h"
#include "../engine/Main.h"

#include "GameCommon.h"

#include "audio/SFX.h"
#include "object/Building.h"
#include "object/Creature.h"
#include "object/Object.h"
#include "object/Vehicle.h"
#include "object/Upgrade.h"
#include "world/Camera.h"
#include "world/Map3D.h"
#include "world/SelectPlace.h"


/**********************************************************************************
 ******** Forward Global Namespace Declarations
 **********************************************************************************/

#pragma region Forward Declarations

/// TODO: Remove me once Lego module is finished
#define Lego_Initialise ((bool32(__cdecl*)(void))0x0041fa80)
#define Lego_MainLoop ((bool32(__cdecl*)(real32))0x00423210)
#define Lego_Shutdown ((void(__cdecl*)(void))0x00424c20)
#define Lego_Shutdown_Debug ((void(__cdecl*)(void))0x00424c30)

#pragma endregion


/**********************************************************************************
 ******** Game Entry point
 **********************************************************************************/

namespace LegoRR
{; // !<---
#pragma region Entry point

/// CUSTOM: Basic setup performed by Lego_Gods_Go, but without calling Main_SetTitle or Main_SetState.
void __cdecl Lego_Gods_Go_Setup(const char* programName, OUT Gods98::Main_State* mainState);

/// NOTE: This is the original Gods_Go function and has been replaced by the implementation
///       in OpenLRR. The above function is still used though.
// This is the GAME entry point as called by WinMain,
//  this should hook the Main_State loop functions and only perform basic initial setup.
// (this can return bool32, but does not)
// <LegoRR.exe @0041f950>
void __cdecl Lego_Gods_Go(const char* programName);

#pragma endregion
}



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

//#define EMERGE_MAXPOINTS		5

#pragma endregion

/**********************************************************************************
 ******** Enumerations
 **********************************************************************************/

#pragma region Enums

enum GameFlags1 : uint32 // [LegoRR/Lego.c|flags:0x4|type:uint]
{
	GAME1_NONE                  = 0,
	GAME1_RADARON               = 0x2,
	GAME1_LEVELSTART            = 0x4,
	GAME1_MUSICON               = 0x8,
	GAME1_SOUNDON               = 0x10,
	GAME1_WALLPROMESHES         = 0x20,
	GAME1_FRAMERATEMONITOR      = 0x40,
	GAME1_MEMORYMONITOR         = 0x80,
	GAME1_DRAGGINGBOX_UNK       = 0x100,
	GAME1_UNK_200               = 0x200,
	GAME1_CLEAR                 = 0x400,
	GAME1_PANELS                = 0x800,
	GAME1_RADAR_MAPVIEW         = 0x1000,
	GAME1_RADAR_TRACKOBJECTVIEW = 0x2000,
	GAME1_UNK_4000              = 0x4000,
	GAME1_FOGCOLOURRGB          = 0x8000,
	GAME1_HIGHFOGCOLOURRGB      = 0x10000,
	GAME1_UNK_20000             = 0x20000,
	GAME1_UNK_40000             = 0x40000,
	GAME1_DYNAMICPM             = 0x80000,
	GAME1_CAMERADISABLED        = 0x100000,
	GAME1_UNK_200000            = 0x200000,
	GAME1_ONLYBUILDONPATHS      = 0x400000,
	GAME1_ALWAYSROCKFALL        = 0x800000,
	GAME1_DEBUG_NONERPS         = 0x1000000,
	GAME1_PAUSED                = 0x2000000,
	GAME1_STREAMNERPSSPEACH     = 0x4000000,
	GAME1_UNK_8000000           = 0x8000000,
	GAME1_LASERTRACKER          = 0x10000000,
	GAME1_UNK_20000000          = 0x20000000,
	GAME1_DEBUG_NOCLIP_FPS      = 0x40000000,
	GAME1_UNK_80000000          = 0x80000000,
};
flags_end(GameFlags1, 0x4);


enum GameFlags2 : uint32 // [LegoRR/Lego.c|flags:0x4|type:uint]
{
	GAME2_NONE                 = 0,
	GAME2_CALLTOARMS           = 0x1,
	GAME2_UNK_2                = 0x2,
	GAME2_ATTACKDEFER          = 0x4,
	GAME2_SHOWDEBUGTOOLTIPS    = 0x8,
	GAME2_ALLOWDEBUGKEYS       = 0x10,
	GAME2_ALLOWEDITMODE        = 0x20,
	GAME2_UNK_40               = 0x40,
	GAME2_UNK_80               = 0x80,
	GAME2_INOPTIONSMENU        = 0x100,
	GAME2_CAMERAMOVING         = 0x200,
	GAME2_MOUSE_INSIDEGAMEVIEW = 0x400,
	GAME2_MUSICON              = 0x800,
	GAME2_INMENU               = 0x1000,
	GAME2_NOMULTISELECT        = 0x2000,
	GAME2_MENU_HASNEXT         = 0x4000,
	GAME2_MENU_HASPREVIOUS     = 0x8000,
	GAME2_ALLOWRENAME          = 0x20000,
	GAME2_RECALLOLOBJECTS      = 0x40000,
	GAME2_GENERATESPIDERS      = 0x80000,
	GAME2_DISABLETOOLTIPSOUND  = 0x100000,
	GAME2_NOAUTOEAT            = 0x200000,
};
flags_end(GameFlags2, 0x4);


enum GameFlags3 : uint32 // [LegoRR/Lego.c|flags:0x4|type:uint]
{
	GAME3_NONE          = 0,
	GAME3_UNK_1         = 0x1,
	GAME3_UNK_2         = 0x2,
	GAME3_UNK_4         = 0x4,
	GAME3_PICKUPOBJECT  = 0x8,
	GAME3_LOADVEHICLE   = 0x10,
	GAME3_UNK_20        = 0x20,
	GAME3_UNK_40        = 0x40,
	GAME3_PLACEBUILDING = 0x80,
};
flags_end(GameFlags3, 0x4);


// hardcoded texture type enum, not used everywhere
enum TextureType : sint32 // [LegoRR/dummy.c|enum:0x4|type:int|tags:HELPER,UNUSED]
{
	TEXTURES__INVALID = -1,

	TEXTURES_ROCK     = 0,
	TEXTURES_LAVA     = 1,
	TEXTURES_ICE      = 2,
};
assert_sizeof(TextureType, 0x4);


enum LevelStatus : sint32 // [LegoRR/Objective.c|enum:0x4|type:int] Not sure if Objective...
{
	LEVELSTATUS_INCOMPLETE      = 0,
	LEVELSTATUS_COMPLETE        = 1,
	LEVELSTATUS_FAILED          = 2,
	LEVELSTATUS_FAILED_CRYSTALS = 3,
	LEVELSTATUS_FAILED_OTHER    = 4,
};
assert_sizeof(LevelStatus, 0x4);

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

struct ObjectiveData // [LegoRR/???|struct:0x54]
{
	/*00,4*/        Gods98::Image* ObjectiveImage; // bmp
	/*04,8*/        Point2F ObjectiveImagePosition;
	/*0c,4*/        Gods98::Image* ObjectiveAcheivedImage; // bmp
	/*10,8*/        Point2F ObjectiveAcheivedImagePosition;
	/*18,4*/        Gods98::Image* ObjectiveFailedImage; // bmp
	/*1c,8*/        Point2F ObjectiveFailedImagePosition;
	/*24,4*/        char* ObjectiveAcheivedAVIFilename; // filename
	/*28,8*/        Point2F ObjectiveAcheivedAVIPosition;
	/*30,4*/        undefined4 field_30;
	/*34,4*/        undefined4 field_34;
	/*38,4*/        uint32 CrystalObjective; // number of crystals needed if non-zero
	/*3c,4*/        uint32 OreObjective; // number of ore needed if non-zero
	/*40,8*/        Point2I BlockObjective;
	/*48,4*/        real32 TimerObjective; // (mult: 25.0, flags, format: "time:HitTimeFailObjective")
	/*4c,4*/        LegoObject_Type ConstructionObjectiveObjType;
	/*50,4*/        sint32 ConstructionObjectiveObjIndex;
	/*54*/
};
assert_sizeof(ObjectiveData, 0x54);

struct EmergeBlock
{
	/*0,8*/         Point2I blockPos;
	/*8,4*/         bool32 used;
	/*c*/
};
assert_sizeof(EmergeBlock, 0xc);

struct EmergeTrigger // [LegoRR/Lego.c|struct:0x4c]
{
	/*000,4*/       undefined4 field_0;
	/*004,8*/       Point2I blockPos;
	/*00c,4*/       real32 timeout;
	/*010,3c*/      EmergeBlock emergePoints[EMERGE_MAXPOINTS];
	/*4c*/
};
assert_sizeof(EmergeTrigger, 0x4c);


struct Lego_Level // [LegoRR/Lego.c|struct:0x284]
{
	/*000,4*/       char* name; // (format: "Levels::level")
	/*004,4*/       Map3D* map;
	/*008,4*/       EmergeTrigger* emergeTriggers;
	/*00c,4*/       uint32 emergeCount;
	/*010,4*/       undefined4 field_10;
	/*014,4*/       uint32 width;
	/*018,4*/       uint32 height;
	/*01c,4*/       real32 BlockSize;
	/*020,4*/       real32 DigDepth;
	/*024,4*/       real32 RoofHeight;
	/*028,4*/       real32 RoughLevel;
	/*02c,4*/       BoolTri UseRoof;
	/*030,4*/       BoolTri SafeCaverns;
	/*034,4*/       real32 SelBoxHeight;
	/*038,4*/       undefined4 field_0x38;
	/*03c,4*/       undefined4 field_0x3c;
	/*040,4*/       undefined4 field_0x40;
	/*044,4*/       undefined4 field_0x44;
	/*048,4*/       undefined4 field_0x48;
	/*04c,4*/       undefined4 field_0x4c;
	/*050,4*/       undefined4 field_0x50;
	/*054,4*/       undefined4 field_0x54;
	/*058,4*/       undefined4 field_0x58;
	/*05c,4*/       undefined4 field_0x5c;
	/*060,4*/       undefined4 field_0x60;
	/*064,4*/       undefined4 field_0x64;
	/*068,4*/       undefined4 field_0x68;
	/*06c,4*/       undefined4 field_0x6c;
	/*070,4*/       undefined4 field_0x70;
	/*074,8*/       Size2I textureSetSize; // surface texture width and height
	/*07c,4*/       Detail_TextureSet* textureSet;
	/*080,4*/       Detail_Mesh** promeshGrid;
	/*084,4*/       RadarMap* radarMap;
	/*088,4*/       LevelStruct_1c* terrain1c_88;
	/*08c,4*/       sint32 numCrystals; // (init: 0) total number of crystals
	/*090,4*/       undefined4 field_90; // (init: 0)
	/*094,4*/       undefined4 field_94; // (init: 0)
	/*098,4*/       sint32 numUnusedCrystals; // crystals available for use (not powering buildings)
	/*09c,4*/       undefined4 field_9c;
	/*0a0,4*/       sint32 numOre; // (init: 0)
	/*0a4,4*/       undefined4 field_a4; // (init: 0)
	/*0a8,4*/       undefined4 field_a8; // (init: 0)
	/*0ac,4*/       sint32 numProcessedOre;
	/*0b4,4*/       char* NextLevel;
	/*0b0,4*/       LegoObject_ID EmergeCreature; // (searches for object index by name, expects RockMonsterType)
	/*0b8,4*/       Lego_Block* blocks; // grid of blocks [y][x]
	/*0bc,54*/      ObjectiveData objective;
	/*110,4*/       bool32 hasBlockPointers;
	/*114,e0*/      BlockPointer* blockPointers[LEGO_MAXBLOCKPOINTERS];
	/*1f4,4*/       bool32 StartFP;
	/*1f8,4*/       bool32 NoDrain;
	/*1fc,4*/       real32 oxygenLevel; // (init: 100.0, Oxygen level?)
	/*200,4*/       real32 OxygenRate;
	/*204,4*/       real32 float_204; // (init: 100.0, Oxygen level?)
	/*208,4*/       real32 BuildingTolerance; // (default: 4.0)
	/*20c,4*/       real32 BuildingMaxVariation; // (default: 6.0)
	/*210,50*/      real32 UpgradeTimes[LegoObject_Type_Count]; // [objType] (mult: 25.0, 1:Vehicle, 2:MiniFigure, 3:Building)
	/*260,4*/       real32 TrainTime; // (mult: 25.0)
	/*264,4*/       real32 EmergeTimeOut; // (default: 1500.0)
	/*268,4*/       real32 SlugTime; // (default: 60.0, mult: 25.0 (applies to default))
	/*26c,4*/       LegoObject_ID Slug; // (default: 20 (invalid), searches for object index by name, expects RockMonsterType)
	/*270,4*/       char* FullName; // (replace '_' with ' ')
	/*274,4*/       TextureType BoulderAnimation; // (texture index, hardcoded: Rock, Lava, Ice)
	/*278,4*/       real32 MaxStolen;
	/*27c,4*/       LevelStatus status; // (init: 0) 2 is used for Crystal failure as well
	/*280,4*/       bool32 IsStartTeleportEnabled; // (! DisableStartTeleport)
	/*284*/
};
assert_sizeof(Lego_Level, 0x284);


struct Lego_Globs // [LegoRR/Lego.c|struct:0xf00|tags:GLOBS]
{
	// [globs: start]
	/*000,4*/       Gods98::Config* config;
	/*004,4*/       const char* gameName;
	/*008,4*/       undefined4 field_8;
	/*00c,4*/       undefined4 field_c;
	/*010,4*/       Gods98::Graphics_Quality quality;
	/*014,4*/       Lego_Level* currLevel;
	/*018,4*/       Gods98::Container* rootCont; // containerGlobs.root
	/*01c,4*/       Gods98::Viewport* viewMain; // Viewport area: (0.0,0.0 - 1.0x1.0)
	/*020,4*/       Gods98::Viewport* viewTrack; // Viewport area: (16,14 - 151x151)
	/*024,4*/       LegoCamera* cameraMain;
	/*028,4*/       LegoCamera* cameraTrack;
	/*02c,4*/       LegoCamera* cameraFP;
	/*030,4*/       Gods98::Container* spotlightTop;
	/*034,4*/       Gods98::Container* spotlightTrack;
	/*038,4*/       Gods98::Container* pointLightFP;
	/*03c,4*/       Gods98::Container* dirLightFP;
	/*040,4*/       Gods98::Container* ambientLight;
	/*044,4*/       Gods98::Container* rootSpotlight;
	/*048,4*/       Gods98::Container* rootLight;
	/*04c,4*/       Gods98::Container* dirLightFPDefault;
	/*050,4*/       real32 FPClipBlocks;
	/*054,4*/       real32 TVClipDist;
	/*058,4*/       undefined4 field_58;
	/*05c,4*/       undefined4 field_5c;
	/*060,4*/       undefined4 field_60;
	/*064,4*/       Gods98::Font* bmpFONT5_HI;
	/*068,4*/       Gods98::Font* bmpToolTipFont;
	/*06c,4*/       Gods98::Font* bmpDeskTopFont;
	/*070,4*/       Gods98::Font* bmpfont5_HI;
	/*074,4*/       Gods98::Font* bmpMbriefFONT;
	/*078,4*/       Gods98::Font* bmpMbriefFONT2;
	/*07c,4*/       Gods98::Font* bmpRSFont;
	/*080,4*/       Gods98::TextWindow* textWnd_80;
	/*084,4*/       Gods98::TextWindow* textWnd_84;
	/*088,10*/      Point2F gotoPositions_88[2];
	/*098,4*/       bool32 gotoBool_98;
	/*09c,4*/       real32 gameSpeed;
	/*0a0,4*/       LegoObject* objectFP;
	/*0a4,c*/       Vector3F vectorDragStartUnk_a4;
	/*0b0,4*/       real32 float_b0;
	/*0b4,4*/       real32 float_b4;
	/*0b8,4*/       real32 float_b8;
	/*0bc,4*/       real32 float_bc;
	/*0c0,8*/       Point2I pointi_c0;
	/*0c8,4*/       bool32 bool_c8;
	/*0cc,4*/       char* langPowerCrystal_name;
	/*0d0,4*/       char* langOre_name;
	/*0d4,4*/       char* langProcessedOre_name;
	/*0d8,4*/       char* langDynamite_name;
	/*0dc,4*/       char* langBarrier_name;
	/*0e0,4*/       char* langElectricFence_name;
	/*0e4,4*/       char* langSpiderWeb_name;
	/*0e8,4*/       char* langOohScary_name;
	/*0ec,4*/       char* langPath_name;
	/*0f0,4*/       char* langPowerCrystal_theName;
	/*0f4,4*/       char* langOre_theName;
	/*0f8,4*/       char* langProcessedOre_theName;
	/*0fc,4*/       char* langDynamite_theName;
	/*100,4*/       char* langBarrier_theName;
	/*104,4*/       char* langElectricFence_theName;
	/*108,4*/       char* langSpiderWeb_theName;
	/*10c,4*/       char* langOohScary_theName;
	/*110,4*/       char* langPath_theName;
	/*114,4*/       VehicleData* vehicleData;
	/*118,4*/       CreatureData* miniFigureData;
	/*11c,4*/       CreatureData* rockMonsterData;
	/*120,4*/       BuildingData* buildingData;
	/*124,4*/       UpgradeData* upgradeData;
	/*128,4*/       char** vehicleName;
	/*12c,4*/       char** miniFigureName;
	/*130,4*/       char** rockMonsterName;
	/*134,4*/       char** buildingName;
	/*138,4*/       char** upgradeName;
	/*13c,2c*/      char* toolName[LegoObject_ToolType_Count];
	/*168,2c*/      char* langTool_name[LegoObject_ToolType_Count];
	/*194,4*/       char** langVehicle_name;
	/*198,4*/       char** langMiniFigure_name;
	/*19c,4*/       char** langRockMonster_name;
	/*1a0,4*/       char** langBuilding_name;
	/*1a4,4*/       char** langUpgrade_name; // (never specified in Lego.cfg)
	/*1a8,4*/       char** langVehicle_theName;
	/*1ac,4*/       char** langMiniFigure_theName;
	/*1b0,4*/       char** langRockMonster_theName;
	/*1b4,4*/       char** langBuilding_theName;
	/*1b8,4*/       char** langUpgrade_theName;
	/*1bc,4*/       uint32 vehicleCount;
	/*1c0,4*/       uint32 miniFigureCount;
	/*1c4,4*/       uint32 rockMonsterCount;
	/*1c8,4*/       uint32 buildingCount;
	/*1cc,4*/       uint32 upgradeCount;
	/*1d0,48*/      char* surfaceName[18];
	/*218,48*/      char* langSurface_name[18];
	/*260,48*/      SFX_ID langSurface_sound[Lego_SurfaceType_Count];
	/*2a8,4*/       Gods98::Container* contBoulder;
	/*2ac,4*/       Gods98::Container* contBoulderExplode;
	/*2b0,4*/       Gods98::Container* contBoulderExplodeIce;
	/*2b4,4*/       Gods98::Container* contCrystal;
	/*2b8,4*/       Gods98::Container* contDynamite;
	/*2bc,8*/       Gods98::Container* contOresTable[2];
	/*2c4,4*/       Gods98::Container* contOohScary;
	/*2c8,4*/       Gods98::Container* contBarrier;
	/*2cc,4*/       Gods98::Container* contElectricFence;
	/*2d0,4*/       Gods98::Container* contSpiderWeb;
	/*2d4,4*/       Gods98::Container* contRechargeSparkle;
	/*2d8,4*/       Gods98::Container* contMiniTeleportUp;
	/*2dc,4*/       Gods98::Container* contElectricFenceStud;
	/*2e0,4*/       Gods98::Container* contPusher;
	/*2e4,4*/       Gods98::Container* contFreezer;
	/*2e8,4*/       Gods98::Container* contIceCube;
	/*2ec,4*/       Gods98::Container* contSmashPath;
	/*2f0,4*/       Gods98::Container* contLaserShot;
	/*2f4,4*/       SelectPlace* selectPlace;
	/*2f8,28*/      LegoObject* recordObjs[10];
	/*320,4*/       uint32 recordObjsCount;
	/*324,10*/      Area2F radarScreenRect;
	/*334,4*/       real32 tvTiltOrZoom_334;
	/*338,8*/       Point2F tvFaceDirection_338;
	/*340,4*/       real32 MedPolyRange;
	/*344,4*/       real32 HighPolyRange;
	/*348,4*/       sint32 HPBlocks;
	/*34c,c*/       ColourRGBF FogColourRGB;
	/*358,c*/       ColourRGBF HighFogColourRGB;
	/*364,4*/       real32 float_364; // level-related?
	/*368,c*/       ColourRGBF PowerCrystalRGB;
	/*374,c*/       ColourRGBF UnpoweredCrystalRGB;
	/*380,4*/       LegoObject_Type placeObjType;
	/*384,4*/       sint32 placeObjIndex;
	/*388,4*/       Direction placeObjDirection;
	/*38c,4*/       LegoObject* placeDestSmallTeleporter;
	/*390,4*/       LegoObject* placeDestBigTeleporter;
	/*394,4*/       LegoObject* placeDestWaterTeleporter;
	/*398,4*/       real32 MinEnergyForEat;
	/*39c,4*/       Gods98::Image* TutorialIcon;
	/*3a0,4*/       undefined4 field_3a0;
	/*3a4,4*/       real32 DynamiteDamageRadius;
	/*3a8,4*/       real32 DynamiteMaxDamage;
	/*3ac,4*/       real32 DynamiteWakeRadius;
	/*3b0,4*/       real32 BirdScarerRadius;
	/*3b4,50*/      LegoObject_Type objTeleportQueueTypes_TABLE[LegoObject_Type_Count];
	/*404,50*/      sint32 objTeleportQueueIndexes_TABLE[LegoObject_Type_Count];
	/*454,4*/       uint32 objTeleportQueue_COUNT;
	/*458,4*/       real32 MiniFigureRunAway;
	/*45c,c*/       Vector3F vector_45c;
	/*468,320*/     Point2I blockPts_468[100];
	/*788,4*/       uint32 blockPtsCount_788;
	/*78c,640*/     Point2I points2x100_78c[2][100];
	/*dcc,8*/       uint32 pointsCount2_dcc[2];
	/*dd4,4*/       uint32 MaxReturnedCrystals;
	/*dd8,4*/       sint32 MouseScrollBorder;
	/*ddc,4*/       char* langHealth_toolTip;
	/*de0,4*/       char* langEnergy_toolTip;
	/*de4,4*/       char* langCrystals_toolTip;
	/*de8,4*/       char* langOre_toolTip;
	/*dec,4*/       char* langStuds_toolTip;
	/*df0,4*/       char* langTools_toolTip;
	/*df4,4*/       char* langCarryObject_toolTip;
	/*df8,4*/       char* langDrivenBy_toolTip;
	/*dfc,4*/       char* langOreRequired_toolTip;
	/*e00,4*/       bool32 IsFallinsEnabled; // (not "NoFallins")
	/*e04,4*/       real32 MinDistFor3DSoundsOnTopView;
	/*e08,4*/       ViewMode viewMode; // FirstPerson or TopDown
	/*e0c,4*/       GameFlags1 flags1;
	/*e10,4*/       GameFlags2 flags2;
	/*e14,4*/       GameFlags3 flags3; // only first byte is used(?)
	/*e18,4*/       real32 InitialSlugTime;
	/*e1c,8*/       Point2F NextButtonPos;
	/*e24,8*/       Point2F BackButtonPos;
	/*e2c,4*/       Gods98::Image* NextButtonImage;
	/*e30,4*/       Gods98::Image* BackButtonImage;
	/*e34,4*/       Gods98::Image* BackArrowImage;
	/*e38,4*/       real32 FogRate;
	/*e3c,4*/       real32 timerGame_e3c;
	/*e40,4*/       real32 elapsedAbs;
	/*e44,4*/       real32 DrainTime;
	/*e48,4*/       uint32 ReinforceHits;
	/*e4c,4*/       uint32 CDStartTrack;
	/*e50,4*/       uint32 CDTracks;
	/*e54,4*/       uint32 FallinMultiplier;
	/*e58,4*/       bool32 hasFallins;
	/*e5c,8*/       Point2F menuNextPoint;
	/*e64,8*/       Point2F menuPrevPoint;
	/*e6c,c*/       ColourRGBF DragBoxRGB;
	/*e78,4*/       Gods98::Image* DialogImage;
	/*e7c,4*/       Gods98::Image* DialogContrastOverlay;
	/*e80,4*/       Gods98::TextWindow* DialogTextWndTitle;
	/*e84,4*/       Gods98::TextWindow* DialogTextWndMessage;
	/*e88,4*/       Gods98::TextWindow* DialogTextWndOK;
	/*e8c,4*/       Gods98::TextWindow* DialogTextWndCancel;
	/*e90,4*/       char* CreditsTextFile;
	/*e94,4*/       char* CreditsBackAVI;
	/*e98,40*/      char* langUpgradeLevel_name[OBJECT_MAXLEVELS];
	/*ed8,4*/       sint32 BuildingUpgradeCostOre;
	/*edc,4*/       sint32 BuildingUpgradeCostStuds;
	/*ee0,4*/       char* renameInput;
	/*ee4,8*/       Point2F renamePosition;
	/*eec,4*/       char* RenameReplace;
	/*ef0,4*/       char* EndGameAVI1;
	/*ef4,4*/       char* EndGameAVI2;
	// [globs: end]
	/*ef8,8*/       Point2I s_mouseBlockPos_ef8; // (static, Game_unkGameLoop_FUN_00426450)
	/*f00*/
};
assert_sizeof(Lego_Globs, 0xf00);

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @005570c0>
extern Lego_Globs & legoGlobs;

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <inlined>
__inline const Gods98::Config* Lego_Config(void) { return legoGlobs.config; }

// <inlined>
__inline const char* Lego_GameName(void) { return legoGlobs.gameName; }

// <inlined>
__inline ViewMode Lego_GetViewMode(void) { return legoGlobs.viewMode; }

// <LegoRR.exe @004294d0>
__inline Gods98::Container* __cdecl Lego_GetCurrentViewLight(void)
{
	return ((legoGlobs.viewMode==ViewMode_FP) ? legoGlobs.pointLightFP : legoGlobs.rootSpotlight);
	//if (legoGlobs.viewMode == ViewMode_FP)
	//	return legoGlobs.pointLightFP;
	//else
	//	return legoGlobs.rootSpotlight;
}

// <LegoRR.exe @00431490>
//Lego_Level* __cdecl noinline(Lego_GetLevel)(void);
__inline Lego_Level* Lego_GetLevel(void) { return legoGlobs.currLevel; }

// <LegoRR.exe @004314a0>
//Map3D* __cdecl noinline(Lego_GetMap)(void);
__inline Map3D* Lego_GetMap(void) { return Lego_GetLevel()->map; }


// <LegoRR.exe @0041f870>
#define Lego_SetRadarNoTrackObject ((void (__cdecl* )(bool32 noTrackObj))0x0041f870)


// <LegoRR.exe @0042e7e0>
#define Lego_GetObjectByName ((bool32 (__cdecl* )(const char* objName, OUT LegoObject_Type* objType, OUT LegoObject_ID* objID, OUT void** model))0x0042e7e0)
//__inline bool32 __cdecl Lego_GetObjectByName(const char* objName, OUT LegoObject_Type* objType, OUT LegoObject_ID* objID, OPTIONAL OUT void** model);

// <LegoRR.exe @0042eca0>
#define Lego_GetObjectTypeModel ((bool32 (__cdecl* )(LegoObject_Type objType, LegoObject_ID objID, OUT void** model))0x0042eca0)
//__inline bool32 __cdecl Lego_GetObjectTypeModel(LegoObject_Type objType, LegoObject_ID objID, OUT void** model);

// <LegoRR.exe @0042ee70>
#define Lego_GetObjectTypeIDCount ((uint32 (__cdecl* )(LegoObject_Type objType))0x0042ee70)

// <LegoRR.exe @0042c260>
bool32 __cdecl Level_HandleEmergeTriggers(Lego_Level* level, const Point2I* blockPos, OUT Point2I* emergeBlockPos);

#if 0
// (first function in module)
// <LegoRR.exe @0041f7a0>
void __cdecl Level_IncCrystals(void);


// [...]


// (LRR_Go)
// <LegoRR.exe @0041f950>
//void __cdecl Gods_Go(const char* programName);

// (LRR_SetLoadFlag_StartTeleporter)
// <LegoRR.exe @0041f9b0>
void __cdecl Lego_SetLoadFlag_StartTeleporter(void);

// <LegoRR.exe @0041fa70>
real32 __cdecl Game_GetGameSpeed(void);

// Main_State.Initialise function
// (LRR_Initialise)
// <LegoRR.exe @0041fa80>
bool32 __cdecl Lego_Initialise(void);

// (LRR_LoadMiscObjectsnitialise)
// <LegoRR.exe @00422780>
void __cdecl Lego_LoadMiscObjects(const Gods98::Config* root);

// (LRR_GetCurrentCamera_Container)
// <LegoRR.exe @00422fb0>
Gods98::Container* __cdecl Lego_GetCurrentCamera_Container(void);

// (LRR_GetElapsedAbs)
// <LegoRR.exe @00422fe0>
real32 __cdecl Lego_GetElapsedAbs(void);

// (LRR_RenameInputUnk)
// <LegoRR.exe @00422ff0>
void __cdecl Lego_RenameInputUnk(void);

// (LRR_Input_UpdateTyping)
// <LegoRR.exe @00423120>
void __cdecl Lego_Input_UpdateTyping(void);

// Main_State.MainLoop function
// (LRR_MainLoop)
// <LegoRR.exe @00423210>
bool32 __cdecl Lego_MainLoop(real32 elapsed);


// [...]


// Main_State.Shutdown function
// Simply halts music playback, then calls std::exit(0); (by calling Lego_Exit();)
// (LRR_Shutdown)
// <LegoRR.exe @00424c20>
void __cdecl Lego_Shutdown(void);

// Main_State.Shutdown function  (ProgrammerMode_10)
// This is only called for ProgrammerMode_10. Proper game cleanup is performed here.
// (LRR_Shutdown_Debug)
// <LegoRR.exe @00424c30>
void __cdecl Lego_Shutdown_Debug(void);

// Simply halts music playback, then calls std::exit(0);
// (LRR_Exit)
// <LegoRR.exe @00424fd0>
void __cdecl Lego_Exit(void);


// [... ... ...]


// (last function of Lego module)
// <LegoRR.exe @004359d0>
void __cdecl LRRGame_SetCallToArmsOn(bool32 callToArms);


#pragma endregion
#endif

}
