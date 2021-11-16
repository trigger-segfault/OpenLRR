// Game.h : 
//

#pragma once

#include "../common.h"
#include "../engine/colour.h"
#include "../engine/geometry.h"
#include "../engine/undefined.h"

#include "../engine/core/Config.h"
#include "../engine/core/Memory.h"
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

#if 0
/// TODO: Remove me once Lego module is finished
#define Lego_Initialise ((bool32(__cdecl*)(void))0x0041fa80)
#define Lego_MainLoop ((bool32(__cdecl*)(real32))0x00423210)
#define Lego_Shutdown ((void(__cdecl*)(void))0x00424c20)
#define Lego_Shutdown_Debug ((void(__cdecl*)(void))0x00424c30)
#endif

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

struct BlockConstruction;

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


enum BlockFlags1 : uint32 // [LegoRR/Lego.c|flags:0x4|type:uint]
{
	BLOCK1_NONE                    = 0,
	BLOCK1_RUBBLE_LOW              = 0x1,
	BLOCK1_RUBBLE_MEDIUM           = 0x2,
	BLOCK1_RUBBLE_FULL             = 0x3,
	BLOCK1_SURVEYED                = 0x4,
	BLOCK1_FLOOR                   = 0x8,
	BLOCK1_WALL                    = 0x10,
	BLOCK1_REINFORCED              = 0x20,
	BLOCK1_CORNEROBTUSE            = 0x40,
	BLOCK1_CORNERINNER             = 0x80,
	BLOCK1_UNK_100                 = 0x100,
	BLOCK1_UNK_200                 = 0x200,
	BLOCK1_BUILDINGSOLID           = 0x400,
	BLOCK1_UNK_800                 = 0x800,
	BLOCK1_UNK_1000                = 0x1000,
	BLOCK1_GAP                     = 0x2000,
	BLOCK1_UNK_4000                = 0x4000,
	BLOCK1_BUILDINGPATH            = 0x8000,
	BLOCK1_HIDDEN                  = 0x20000,
	BLOCK1_DESTROYEDCONNECTION_UNK = 0x40000,
	BLOCK1_UNK_80000               = 0x80000,
	BLOCK1_FOUNDATION              = 0x100000,
	BLOCK1_CLEARED_UNK             = 0x200000,
	BLOCK1_UNK_400000              = 0x400000,
	BLOCK1_UNK_800000              = 0x800000,
	BLOCK1_UNK_1000000             = 0x1000000,
	BLOCK1_EXPOSED                 = 0x4000000,
	BLOCK1_BUSY                    = 0x8000000,
	BLOCK1_UNK_10000000            = 0x10000000,
	BLOCK1_PATH                    = 0x20000000,
	BLOCK1_UNK_40000000            = 0x40000000,
	BLOCK1_UNK_80000000            = 0x80000000,
};
flags_end(BlockFlags1, 0x4);


enum BlockFlags2 : uint32 // [LegoRR/Lego.c|flags:0x4|type:uint]
{
	BLOCK2_NONE             = 0,
	BLOCK2_GENERATEPOWER    = 0x1,
	BLOCK2_UNK_2            = 0x2,
	BLOCK2_TOOLSTORE_UNK    = 0x4,
	BLOCK2_FENCEREQUEST     = 0x8,
	BLOCK2_UNK_10           = 0x10,
	BLOCK2_SLUGHOLE_EXPOSED = 0x20,
	BLOCK2_EMERGE_POINT     = 0x40,
	BLOCK2_EMERGE_TRIGGER   = 0x80,
	BLOCK2_POWERED          = 0x100,
	BLOCK2_SLUGHOLE_HIDDEN  = 0x200,
	BLOCK2_UNK_400          = 0x400,
};
flags_end(BlockFlags2, 0x4);


enum SurfaceMapStruct2A8Flags : uint32 // [LegoRR/???|flags:0x4|type:uint]
{
	SURFMAP_STRUCT2A8_NONE   = 0,
	SURFMAP_STRUCT2A8_HIDDEN = 0x1,
	SURFMAP_STRUCT2A8_UNK_2  = 0x2,
	SURFMAP_STRUCT2A8_UNK_4  = 0x4,
	SURFMAP_STRUCT2A8_UNK_8  = 0x8,
	SURFMAP_STRUCT2A8_UNK_10 = 0x10,
};
flags_end(SurfaceMapStruct2A8Flags, 0x4);

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


#if 0
struct BlockConstruction // [LegoRR/Construction.c|struct:0xd4]
{
	/*00,4*/	sint32 objID;
	/*04,4*/	Direction direction;
	/*08,4*/	sint32 int_8;
	/*0c,8*/	Point2I pointi_c;
	/*14,4*/	Point2I* shapePoints;
	/*18,4*/	uint32 shapeCount;
	/*1c,8*/	undefined field_0x1c_0x23[8];
	/*24,4*/	BlockConstruction* next_24;
	/*28,4*/	undefined field_0x28_0x2b[4];
	/*2c,4*/	LegoObject* liveObjs_2c[1];
	/*30,9c*/	undefined field_0x30_0xcb[156];
	/*cc,4*/	uint32 liveObjsCount_cc;
	/*d0,4*/	uint32 flags;
	/*d4*/
};
assert_sizeof(BlockConstruction, 0xd4);
#endif


struct SurfaceMapStruct_3c // [LegoRR/Map3D.c|struct:0x3c]
{
	/*00,4*/	bool32 used_0;
	/*04,c*/	Vector3F vector_4;
	/*10,c*/	Vector3F vector_10;
	/*1c,c*/	Vector3F vector_1c;
	/*28,4*/	real32 float_28;
	/*2c,4*/	real32 float_2c;
	/*30,4*/	real32 float_30; // timer
	/*34,4*/	sint32 int_34; // (std::rand() % 30) + 40
	/*38,4*/	undefined4 field_38;
	/*3c*/
};
assert_sizeof(SurfaceMapStruct_3c, 0x3c);


// Smoke frame?
struct SurfaceMapStruct_2a8 // [LegoRR/Map3D.c|struct:0x2a8]
{
	/*000,258*/	SurfaceMapStruct_3c groupList[10];
	/*258,4*/	real32 float_258;
	/*25c,c*/	Vector3F vector_25c;
	/*268,c*/	Vector3F vector_268;
	/*274,c*/	ColourRGBF colour;
	/*280,4*/	uint32 groupCount;
	/*284,4*/	sint32 int_284;
	/*288,4*/	real32 float_288;
	/*28c,4*/	Gods98::Container* resData_28c;
	/*290,4*/	Gods98::Mesh* mesh;
	/*294,4*/	SurfaceMapStruct2A8Flags flags_294;
	/*298,4*/	SurfaceMapStruct_2a8* next;
	/*29c,4*/	SurfaceMapStruct_2a8* previous;
	/*2a0,4*/	undefined4 field_2a0;
	/*2a4,4*/	sint32 soundHandle;
	/*2a8*/
};
assert_sizeof(SurfaceMapStruct_2a8, 0x2a8);


#pragma pack(push, 1)
struct Lego_Block // [LegoRR/Lego.c|struct:0x48|pack:1]
{
	/*00,1*/	Lego_PredugType predug;
	/*01,1*/	SurfaceTexture texture;
	/*02,1*/	Lego_SurfaceType8 terrain;
	/*03,1*/	uint8 direction; // clockwise (does not determine corner/wall type)
	/*04,1*/	uint8 blockpointer;
	/*05,1*/	Lego_CryOreType cryOre;
	/*06,1*/	Lego_ErodeType erodeSpeed;
	/*07,1*/	uint8 field_7;
	/*08,4*/	BlockFlags1 flags1;
	/*0c,4*/	BlockFlags2 flags2;
	/*10,4*/	BlockConstruction* construct;
	/*14,4*/	real32 damage; // drill damage [0.0-1.0]
	/*18,4*/	LevelStruct_1c* struct1c_18;
	/*1c,4*/	SurfaceMapStruct_2a8* smokeptr_1c;
	/*20,2*/	sint16 randomness;
	/*22,2*/	sint16 short_22; // (some num value, observed: != 4)
	/*24,4*/	uint32 numLandSlides;
	/*28,4*/	uint32 clickCount;
	/*2c,4*/	sint32 generateCrystals;
	/*30,4*/	sint32 generateOre;
	/*34,1*/	uint8 aiNode;
	/*35,3*/	undefined field_0x35_0x37[3]; // padding?
	/*38,4*/	bool32 fallinUpper; // (fallin upper: 1 if fallin > 4)
	/*3c,4*/	sint32 fallinIntensity; // (fallin scale: 1-4)
	/*40,4*/	real32 fallinTimer; // (randomized with full fallin value)
	/*44,4*/	undefined4 field_44;
	/*48*/
};
assert_sizeof(Lego_Block, 0x48);
#pragma pack(pop)


struct Lego_Level // [LegoRR/Lego.c|struct:0x284]
{
	/*000,4*/       char* name; // (format: "Levels::level")
	/*004,4*/       Map3D* map;
	/*008,4*/       EmergeTrigger* emergeTriggers;
	/*00c,4*/       uint32 emergeCount;
	/*010,4*/       undefined4 field_10;
	/*014,4*/       uint32 width;
	/*018,4*/       uint32 height;
	/*01c,4*/       real32 blockSize;
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
	/*0b0,4*/       sint32 EmergeCreature; // (searches for object index by name, expects RockMonsterType)
	/*0b4,4*/       char* NextLevel;
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
	/*26c,4*/       sint32 Slug; // (default: 20 (invalid), searches for object index by name, expects RockMonsterType)
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
	/*114,4*/       VehicleModel* vehicleData;
	/*118,4*/       CreatureModel* miniFigureData;
	/*11c,4*/       CreatureModel* rockMonsterData;
	/*120,4*/       BuildingModel* buildingData;
	/*124,4*/       Upgrade_PartModel* upgradeData;
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
	/*260,48*/      SFX_Type langSurface_sound[Lego_SurfaceType_Count];
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

// <LegoRR.exe @00431490>
//Lego_Level* __cdecl noinline(Lego_GetLevel)(void);
__inline Lego_Level* Lego_GetLevel(void) { return legoGlobs.currLevel; }

// <LegoRR.exe @004314a0>
//Map3D* __cdecl noinline(Lego_GetMap)(void);
__inline Map3D* Lego_GetMap(void) { return Lego_GetLevel()->map; }


#if 0
// <LegoRR.exe @0041f870>
#define Game_SetFlag1_20000_unkCameraRadarHasTrackObj ((bool32(__cdecl*)(bool32 state))0x0041f870)


// <LegoRR.exe @0042e7e0>
#define Object_GetObjectByName ((bool32(__cdecl*)(const char*,LegoObject_Type*,sint32*,void**))0x0042e7e0)
//__inline bool32 __cdecl Object_GetObjectByName(const char* objName, OUT LegoObject_Type* objType, OUT sint32* objID, OPTIONAL OUT void** objContainer);

// <LegoRR.exe @0042eca0>
#define Object_GetTypeResource ((bool32(__cdecl*)(LegoObject_Type,sint32,void**))0x0042eca0)
//__inline bool32 __cdecl Object_GetTypeResource(LegoObject_Type objType, sint32 objID, OUT void** objContainer);

// <LegoRR.exe @0042ee70>
#define Object_GetTypeCount ((uint32(__cdecl*)(LegoObject_Type))0x0042ee70)
/*__inline uint32 __cdecl Object_GetTypeCount(LegoObject_Type objType)
{
    return ((uint32(__cdecl*)(LegoObject_Type))0x0042ee70)(objType);
}*/
#endif


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


#endif


// <LegoRR.exe @0041f7a0>
#define Level_IncCrystals ((void (__cdecl* )(void))0x0041f7a0)

// <LegoRR.exe @0041f7b0>
#define Level_SubtractCrystals ((void (__cdecl* )(sint32 crystalCount))0x0041f7b0)

// <LegoRR.exe @0041f7d0>
#define Level_AddDrainedCrystals ((void (__cdecl* )(sint32 drainedCrystalCount))0x0041f7d0)

// <LegoRR.exe @0041f810>
#define Level_GetCrystalCount ((sint32 (__cdecl* )(bool32 asTotalCount))0x0041f810)

// <LegoRR.exe @0041f830>
#define Level_GetOreCount ((sint32 (__cdecl* )(bool32 isProcessed))0x0041f830)

// <LegoRR.exe @0041f850>
#define Level_AddToField9c ((void (__cdecl* )(sint32 value))0x0041f850)

// <LegoRR.exe @0041f870>
#define Game_SetFlag1_20000_unkCameraRadarHasTrackObj ((void (__cdecl* )(bool32 state))0x0041f870)

// <LegoRR.exe @0041f8a0>
#define Game_IsNoclipOn ((bool32 (__cdecl* )(void))0x0041f8a0)

// <LegoRR.exe @0041f8b0>
#define Level_IncField94 ((void (__cdecl* )(void))0x0041f8b0)

// <LegoRR.exe @0041f8c0>
#define Level_IncFieldA8 ((void (__cdecl* )(void))0x0041f8c0)

// <LegoRR.exe @0041f8d0>
#define Level_IncOre ((void (__cdecl* )(LegoObject_ID isProcessed))0x0041f8d0)

// <LegoRR.exe @0041f910>
#define Level_SubtractOre ((void (__cdecl* )(bool32 isProcessed, sint32 oreCount))0x0041f910)



// <LegoRR.exe @0041f9b0>
#define Lego_SetLoadFlag_StartTeleporter ((void (__cdecl* )(void))0x0041f9b0)

// <LegoRR.exe @0041fa70>
#define Game_GetGameSpeed ((real32 (__cdecl* )(void))0x0041fa70)

// <LegoRR.exe @0041fa80>
#define Lego_Initialise ((bool32 (__cdecl* )(void))0x0041fa80)

// <LegoRR.exe @00422780>
#define Lego_LoadMiscObjects ((void (__cdecl* )(const Gods98::Config* config))0x00422780)

// <LegoRR.exe @00422fb0>
#define Lego_GetCurrentCamera_Container ((Gods98::Container* (__cdecl* )(void))0x00422fb0)

// <LegoRR.exe @00422fe0>
#define Lego_GetElapsedAbs ((real32 (__cdecl* )(void))0x00422fe0)

// <LegoRR.exe @00422ff0>
#define Lego_RenameInputUnk ((void (__cdecl* )(real32 elapsedAbs))0x00422ff0)

// <LegoRR.exe @00423120>
#define Lego_Input_UpdateTyping ((void (__cdecl* )(void))0x00423120)

// <LegoRR.exe @00423210>
#define Lego_MainLoop ((bool32 (__cdecl* )(real32 elapsed))0x00423210)

// <LegoRR.exe @00424490>
#define LiveObject_ConsumeOxygen ((void (__cdecl* )(LegoObject* liveObj, real32 elapsed))0x00424490)

// <LegoRR.exe @00424530>
#define Level_FUN_00424530 ((void (__cdecl* )(Lego_Level* level, real32 elapsedGame))0x00424530)

// <LegoRR.exe @00424660>
#define Game_UpdateSceneFog ((void (__cdecl* )(bool32 isFogEnabled, real32 elapsed))0x00424660)

// <LegoRR.exe @00424700>
#define LiveObject_Callback_DrawRedBox_IfFlags4_1 ((bool32 (__cdecl* )(LegoObject* liveObj, Gods98::Viewport* viewMain))0x00424700)

// <LegoRR.exe @00424740>
#define Game_DrawRedBoxes_AroundUnitsWithFlags4_1 ((bool32 (__cdecl* )(Gods98::Viewport* viewMain))0x00424740)

// <LegoRR.exe @00424760>
#define Game_DrawSelectedUnitBoxes ((void (__cdecl* )(Gods98::Viewport* viewMain))0x00424760)

// <LegoRR.exe @004247e0>
#define LiveObject_DrawSelectedBox ((void (__cdecl* )(LegoObject* liveObj, Gods98::Viewport* param_2, real32 r, real32 g, real32 b))0x004247e0)

// <LegoRR.exe @00424c20>
#define Lego_Shutdown ((void (__cdecl* )(void))0x00424c20)

// <LegoRR.exe @00424c30>
#define Lego_Shutdown_Debug ((void (__cdecl* )(void))0x00424c30)

// <LegoRR.exe @00424fd0>
#define Lego_Exit ((void (__cdecl* )(void))0x00424fd0)

// <LegoRR.exe @00424ff0>
#define Lego_HandleKeys ((bool32 (__cdecl* )(real32 elapsedGame, real32 param_2, OUT bool32* keyDownT, OUT bool32* keyDownR, OUT bool32* keyDownShift))0x00424ff0)

// <LegoRR.exe @00425a70>
#define Lego_UpdateAll3DSounds ((bool32 (__cdecl* )(bool32 stopAll))0x00425a70)

// <LegoRR.exe @00425a90>
#define Lego_LiveObjectCallback_UpdateAll3DSounds ((bool32 (__cdecl* )(LegoObject* liveObj, bool32* pStopAll))0x00425a90)

// <LegoRR.exe @00425b60>
#define Game_SetPaused ((void (__cdecl* )(bool32 checkCamDisableFlag, bool32 paused))0x00425b60)

// <LegoRR.exe @00425c00>
#define Game_LockGameSpeed ((void (__cdecl* )(bool32 locked))0x00425c00)

// <LegoRR.exe @00425c10>
#define Game_SetGameSpeed ((void (__cdecl* )(real32 newGameSpeed))0x00425c10)

// <LegoRR.exe @00425c80>
#define Game_TrackObjectInRadar ((void (__cdecl* )(LegoObject* liveObj))0x00425c80)

// <LegoRR.exe @00425cb0>
#define Game_IsFirstPersonView ((bool32 (__cdecl* )(void))0x00425cb0)

// <LegoRR.exe @00425cc0>
#define Lego_Unk_HasRadarCtrl_FUN_00425cc0 ((void (__cdecl* )(void))0x00425cc0)

// <LegoRR.exe @004260f0>
#define Lego_UpdateSlug_FUN_004260f0 ((void (__cdecl* )(real32 elapsedGame))0x004260f0)

// <LegoRR.exe @00426160>
#define Game_GetVector_45c ((void (__cdecl* )(OUT Vector3F* vector))0x00426160)

// <LegoRR.exe @00426180>
#define Lego_UnkCameraTrack_InRadar_FUN_00426180 ((void (__cdecl* )(void))0x00426180)

// <LegoRR.exe @00426210>
#define Game_SetMenuNextPosition ((void (__cdecl* )(OPTIONAL const Point2F* position))0x00426210)

// <LegoRR.exe @00426250>
#define Game_SetMenuPreviousPosition ((void (__cdecl* )(OPTIONAL const Point2F* position))0x00426250)

// <LegoRR.exe @00426290>
#define Game_SetFlags2_40_And_2_unkCamera ((void (__cdecl* )(bool32 onFlag40, bool32 onFlag2))0x00426290)

// <LegoRR.exe @004262d0>
#define Game_SetFlags2_80 ((void (__cdecl* )(bool32 state))0x004262d0)

// <LegoRR.exe @004262f0>
#define Lego_UnkObjective_CompleteSub_FUN_004262f0 ((void (__cdecl* )(void))0x004262f0)

// <LegoRR.exe @00426350>
#define Game_UpdateTopdownCamera ((void (__cdecl* )(real32 elapsedAbs))0x00426350)

// <LegoRR.exe @00426450>
#define Game_unkGameLoop_FUN_00426450 ((void (__cdecl* )(real32 elapsedGame, real32 elapsedAbs, undefined4 param_3, undefined4 param_4, sint32 param_5))0x00426450)

// <LegoRR.exe @00427d30>
#define Lego_LoadToolTipInfos ((void (__cdecl* )(const Gods98::Config* config, const char* gameName_unused))0x00427d30)

// <LegoRR.exe @00427eb0>
#define Lego_LoadUpgradeNames ((void (__cdecl* )(const Gods98::Config* config))0x00427eb0)

// <LegoRR.exe @00427f50>
#define Lego_ShowObjectToolTip ((void (__cdecl* )(LegoObject* liveObj))0x00427f50)

// <LegoRR.exe @00428260>
#define Lego_ShowBlockToolTip ((void (__cdecl* )(const Point2I* mouseBlockPos, bool32 showConstruction, bool32 silent, bool32 showCavern))0x00428260)

// <LegoRR.exe @004286b0>
#define Level_BlockPointerCheck ((bool32 (__cdecl* )(const Point2I* blockPos))0x004286b0)

// <LegoRR.exe @00428730>
#define Game_SetPointer_AndPlayEnumSFX ((void (__cdecl* )(sint32 unkEnum))0x00428730)

// <LegoRR.exe @00428810>
#define Lego_HandleDebugKeys ((void (__cdecl* )(sint32 bx, sint32 by, LegoObject* liveObj))0x00428810)

// <LegoRR.exe @00429040>
#define Lego_XYCallback_AddVisibleSmoke ((void (__cdecl* )(sint32 bx, sint32 by))0x00429040)

// <LegoRR.exe @00429090>
#define Lego_GetBlockDetail_ContainerTexture ((Gods98::Container_Texture* (__cdecl* )(const Point2I* blockPos))0x00429090)

// <LegoRR.exe @004290d0>
#define Lego_UnkUpdateMapsWorldUnk_FUN_004290d0 ((void (__cdecl* )(real32 elapsedAbs, bool32 pass2))0x004290d0)

// <LegoRR.exe @004292e0>
#define Level_DrawDragSelectionBox ((void (__cdecl* )(Lego_Level* level))0x004292e0)

// <LegoRR.exe @004293a0>
#define Game_MainView_MouseTransform ((void (__cdecl* )(uint32 mouseX, uint32 mouseY, OUT real32* xPos, OUT real32* yPos))0x004293a0)

// <LegoRR.exe @004294d0>
#define Lego_GetCurrentViewLight ((Gods98::Container* (__cdecl* )(void))0x004294d0)

// <LegoRR.exe @004294f0>
#define LiveObject_Check_FUN_004294f0 ((bool32 (__cdecl* )(LegoObject* liveObj))0x004294f0)

// <LegoRR.exe @00429520>
#define Lego_SetViewMode ((void (__cdecl* )(ViewMode viewMode, LegoObject* liveObj, sint32 fpCameraFrame))0x00429520)

// <LegoRR.exe @004296d0>
#define Music_CDAudio_PlayNextCallback ((void (__cdecl* )(void))0x004296d0)

// <LegoRR.exe @004296e0>
#define Lego_SetMusicOn ((void (__cdecl* )(bool32 isMusicOn))0x004296e0)

// <LegoRR.exe @00429740>
#define Lego_SetSoundOn ((void (__cdecl* )(bool32 isSoundOn))0x00429740)

// <LegoRR.exe @00429780>
#define Lego_GetEmergeCreature ((sint32 (__cdecl* )(void))0x00429780)

// <LegoRR.exe @00429790>
#define Lego_GetObjectUpgradeTime ((real32 (__cdecl* )(LegoObject_Type objType))0x00429790)

// <LegoRR.exe @004297b0>
#define Lego_GetTrainTime ((real32 (__cdecl* )(void))0x004297b0)

// <LegoRR.exe @004297c0>
#define Lego_LoadLevel ((bool32 (__cdecl* )(const char* levelName))0x004297c0)

// <LegoRR.exe @0042b220>
#define LiveObject_Callback_AddCryOre ((bool32 (__cdecl* )(LegoObject* liveObj, SearchAddCryOre_c* search))0x0042b220)

// <LegoRR.exe @0042b260>
#define WorldMesh_LoadPromeshGrid ((bool32 (__cdecl* )(Lego_Level* level, const char* meshBaseName))0x0042b260)

// <LegoRR.exe @0042b3b0>
#define Level_Free_ProMeshGrid ((void (__cdecl* )(Lego_Level* level))0x0042b3b0)

// <LegoRR.exe @0042b430>
#define Lego_GetRadarMap ((RadarMap* (__cdecl* )(void))0x0042b430)

// <LegoRR.exe @0042b440>
#define Lego_LoadMapSet ((bool32 (__cdecl* )(Lego_Level* level, const char* surfaceMap, const char* predugMap, sint32 predugParam, const char* terrainMap, sint32 terrainParam, const char* blockPointersMap, sint32 blockPointersParam, const char* cryOreMap, sint8 cryOreParam, OPTIONAL const char* erodeMap, OPTIONAL const char* pathMap, sint32 pathParam, const char* textureSet, OPTIONAL const char* emergeMap, OPTIONAL const char* aiMap, OPTIONAL const char* fallinMap))0x0042b440)

// <LegoRR.exe @0042b780>
#define Lego_InitTextureMappings ((void (__cdecl* )(Map3D* map))0x0042b780)

// <LegoRR.exe @0042ba90>
#define Lego_LoadTextureSet ((bool32 (__cdecl* )(Lego_Level* level, const char* keyTexturePath))0x0042ba90)

// <LegoRR.exe @0042bc50>
#define Lego_LoadPreDugMap ((bool32 (__cdecl* )(Lego_Level* level, const char* filename, sint32 modifier))0x0042bc50)

// <LegoRR.exe @0042be70>
#define Lego_LoadErodeMap ((bool32 (__cdecl* )(Lego_Level* level, const char* filename))0x0042be70)

// <LegoRR.exe @0042bf90>
#define Lego_LoadAIMap ((bool32 (__cdecl* )(Lego_Level* level, const char* filename))0x0042bf90)

// <LegoRR.exe @0042c050>
#define Lego_LoadEmergeMap ((bool32 (__cdecl* )(Lego_Level* level, const char* filename))0x0042c050)

// Implemented
// <LegoRR.exe @0042c260>
//#define Level_HandleEmergeTriggers ((bool32 (__cdecl* )(Lego_Level* level, Point2I* blockPos, Point2I* out_emergeBlockPos))0x0042c260)

// <LegoRR.exe @0042c370>
#define Level_FUN_0042c370 ((void (__cdecl* )(Lego_Level* level, real32 elapsedAbs))0x0042c370)

// <LegoRR.exe @0042c3b0>
#define Lego_LoadTerrainMap ((bool32 (__cdecl* )(Lego_Level* level, const char* filename, sint32 modifier))0x0042c3b0)

// <LegoRR.exe @0042c4e0>
#define Lego_GetBlockCryOre ((bool32 (__cdecl* )(const Point2I* blockPos, OUT sint32* crystalLv0, OUT sint32* oreLv0, OUT sint32* crystalLv1, OUT sint32* oreLv1))0x0042c4e0)

// <LegoRR.exe @0042c5d0>
#define Lego_LoadCryOreMap ((bool32 (__cdecl* )(Lego_Level* level, const char* filename, sint8 modifier))0x0042c5d0)

// <LegoRR.exe @0042c690>
#define Lego_LoadPathMap ((bool32 (__cdecl* )(Lego_Level* level, const char* filename, sint32 modifier))0x0042c690)

// <LegoRR.exe @0042c900>
#define Lego_LoadFallinMap ((bool32 (__cdecl* )(Lego_Level* level, const char* filename))0x0042c900)

// <LegoRR.exe @0042caa0>
#define Game_UpdateFallins ((void (__cdecl* )(real32 elapsedGame))0x0042caa0)

// <LegoRR.exe @0042cbc0>
#define Lego_LoadBlockPointersMap ((bool32 (__cdecl* )(Lego_Level* level, const char* filename, sint32 modifier))0x0042cbc0)

// <LegoRR.exe @0042cc80>
#define Game_GetUpgradeData ((Upgrade_PartModel* (__cdecl* )(const char* upgradeName))0x0042cc80)

// <LegoRR.exe @0042ccd0>
#define Lego_LoadVehicleTypes ((bool32 (__cdecl* )(void))0x0042ccd0)

// <LegoRR.exe @0042ce80>
#define Lego_LoadMiniFigureTypes ((bool32 (__cdecl* )(void))0x0042ce80)

// <LegoRR.exe @0042d030>
#define Lego_LoadRockMonsterTypes ((bool32 (__cdecl* )(void))0x0042d030)

// <LegoRR.exe @0042d1e0>
#define Lego_LoadBuildingTypes ((bool32 (__cdecl* )(void))0x0042d1e0)

// <LegoRR.exe @0042d390>
#define Lego_LoadUpgradeTypes ((bool32 (__cdecl* )(void))0x0042d390)

// <LegoRR.exe @0042d530>
#define Lego_LoadObjectNames ((void (__cdecl* )(const Gods98::Config* config))0x0042d530)

// <LegoRR.exe @0042d950>
#define Lego_LoadObjectTheNames ((void (__cdecl* )(const Gods98::Config* config))0x0042d950)

// Must pass a non-null LegoObject* -OR- Point2I* in order to get a block position. (LegoObject* has precedence)
// <LegoRR.exe @0042dd70>
#define Lego_GotoInfo ((void (__cdecl* )(OPTIONAL LegoObject* liveObj, OPTIONAL const Point2I* blockPos, bool32 gotoBool))0x0042dd70)

// <LegoRR.exe @0042def0>
#define Game_RemoveRecordObject ((void (__cdecl* )(LegoObject* liveObj))0x0042def0)

// <LegoRR.exe @0042df20>
#define Game_GetRecordObject ((bool32 (__cdecl* )(uint32 recordObjPtr, OUT LegoObject** liveObj))0x0042df20)

// <LegoRR.exe @0042df50>
#define Game_LoadOLObjectList ((bool32 (__cdecl* )(Lego_Level* level, const char* filename))0x0042df50)

// <LegoRR.exe @0042e7e0>
#define Object_GetObjectByName ((bool32 (__cdecl* )(const char* objName, OUT LegoObject_Type* objType, OUT LegoObject_ID* objID, OUT OPTIONAL Gods98::Container** cont))0x0042e7e0)

// <LegoRR.exe @0042eca0>
#define Object_GetTypeActivityContainer ((bool32 (__cdecl* )(LegoObject_Type objType, LegoObject_ID objID, OUT Gods98::Container** cont))0x0042eca0)

// <LegoRR.exe @0042ee70>
#define Object_GetTypeCount ((sint32 (__cdecl* )(LegoObject_Type objType))0x0042ee70)

// <LegoRR.exe @0042eef0>
#define Lego_PlayMovie_old ((void (__cdecl* )(const char* fName, OPTIONAL const Point2F* screenPt))0x0042eef0)

// <LegoRR.exe @0042eff0>
#define Level_Free ((char* (__cdecl* )(void))0x0042eff0)

// <LegoRR.exe @0042f210>
#define Level_Block_SetFlags1_200_AndUpdateSurface_LevelStruct428 ((void (__cdecl* )(Lego_Level* level, uint32 bx, uint32 by, bool32 setFlag200))0x0042f210)

// <LegoRR.exe @0042f280>
#define Level_Block_ChoosePathTexture ((SurfaceTexture (__cdecl* )(sint32 bx, sint32 by, IN OUT uint8* direction, bool32 powered))0x0042f280)

// <LegoRR.exe @0042f620>
#define Level_BlockUpdateSurface ((void (__cdecl* )(Lego_Level* level, sint32 bx, sint32 by, bool32 reserved))0x0042f620)

// <LegoRR.exe @004301e0>
#define Level_Block_Proc_FUN_004301e0 ((void (__cdecl* )(const Point2I* blockPos))0x004301e0)

// <LegoRR.exe @00430250>
#define AITask_DoClearTypeAction ((void (__cdecl* )(const Point2I* blockPos, Message_Type completeAction))0x00430250)

// <LegoRR.exe @004303a0>
#define Level_Debug_WKey_NeedsBlockFlags1_8_FUN_004303a0 ((void (__cdecl* )(Lego_Level* level, bool32 unused, uint32 bx, uint32 by))0x004303a0)

// <LegoRR.exe @00430460>
#define Level_DestroyWall ((bool32 (__cdecl* )(Lego_Level* level, uint32 bx, uint32 by, bool32 isHiddenCavern))0x00430460)

// <LegoRR.exe @00430d20>
#define Level_Block_FUN_00430d20 ((void (__cdecl* )(const Point2I* blockPos))0x00430d20)

// <LegoRR.exe @00430e10>
#define Level_DestroyWallConnection ((bool32 (__cdecl* )(Lego_Level* level, uint32 bx, uint32 by))0x00430e10)

// <LegoRR.exe @00431020>
#define Level_Block_RemoveReinforcement ((void (__cdecl* )(const Point2I* blockPos))0x00431020)

// <LegoRR.exe @00431070>
#define Level_Block_Reinforce ((void (__cdecl* )(sint32 bx, sint32 by))0x00431070)

// <LegoRR.exe @00431100>
#define Level_LevelStruct1C_Create_Res_Unk_RechargeSparkle ((void (__cdecl* )(Lego_Level* level, Point2I* blockPos, bool32 bool18))0x00431100)

// <LegoRR.exe @004312e0>
#define Level_LevelStruct1C_FUN_004312e0 ((void (__cdecl* )(Lego_Level* level, real32 elapsedGame))0x004312e0)

// <LegoRR.exe @00431380>
#define Level_Block_LevelStruct1C_DoActivityDestroy ((void (__cdecl* )(Lego_Level* level, const Point2I* blockPos, bool32 requireNotBool18))0x00431380)

// <LegoRR.exe @004313f0>
#define Level_Remove_LevelStruct1C_AndFree ((void (__cdecl* )(LevelStruct_1c* param_1))0x004313f0)

// <LegoRR.exe @00431460>
#define Level_FreeAll_LevelStruct1Cs_FUN_00431460 ((void (__cdecl* )(Lego_Level* level))0x00431460)

// <LegoRR.exe @00431490>
#define Lego_GetLevel ((Lego_Level* (__cdecl* )(void))0x00431490)

// <LegoRR.exe @004314a0>
#define Lego_GetMap ((Map3D* (__cdecl* )(void))0x004314a0)

// <LegoRR.exe @004314b0>
#define Level_UncoverHiddenCavern ((void (__cdecl* )(uint32 bx, uint32 by))0x004314b0)

// <LegoRR.exe @004316b0>
#define Game_PTL_RockFall ((void (__cdecl* )(uint32 bx, uint32 by, sint32 param_3, bool32 param_4))0x004316b0)

// <LegoRR.exe @004318e0>
#define Lego_GetBlockTerrain ((Lego_SurfaceType (__cdecl* )(sint32 bx, sint32 by))0x004318e0)

// <LegoRR.exe @00431910>
#define MapShared_GetBlock ((uint32 (__cdecl* )(uint32 memHandle, sint32 bx, sint32 by))0x00431910)

// <LegoRR.exe @00431960>
#define Level_FindSelectedUnit_BlockCheck_FUN_00431960 ((bool32 (__cdecl* )(uint32 bx, uint32 by, bool32 param_3))0x00431960)

// <LegoRR.exe @004319e0>
#define Level_FindSelectedLiveObject_BlockReinforce_FUN_004319e0 ((bool32 (__cdecl* )(uint32 bx, uint32 by))0x004319e0)

// <LegoRR.exe @00431a50>
#define Level_BlockCheck_SelectPlace_FUN_00431a50 ((bool32 (__cdecl* )(sint32 bx, sint32 by, bool32 param_3, bool32 param_4))0x00431a50)

// <LegoRR.exe @00431ba0>
#define LiveObject_FUN_00431ba0 ((bool32 (__cdecl* )(LegoObject* liveObj, const Point2I* param_2, OUT Point2I* result, bool32 param_4))0x00431ba0)

// <LegoRR.exe @00431cd0>
#define LiveObject_Routing_GetCrossTerrainType ((sint32 (__cdecl* )(LegoObject* liveObj, sint32 bx1, sint32 by1, sint32 bx2, sint32 by2, bool32 param_6))0x00431cd0)

// <LegoRR.exe @00432030>
#define Level_Block_SetPowered_AddToTable ((void (__cdecl* )(const Point2I* blockPos))0x00432030)

// <LegoRR.exe @004320a0>
#define Level_Block_IsPowered ((bool32 (__cdecl* )(const Point2I* blockPos))0x004320a0)

// <LegoRR.exe @004320d0>
#define Game_UpdateLevelBlocks_PointsAAC ((void (__cdecl* )(void))0x004320d0)

// <LegoRR.exe @00432130>
#define Game_LevelClearBlockFlag2_100_Points28C ((void (__cdecl* )(void))0x00432130)

// <LegoRR.exe @004321a0>
#define Level_Block_AddPowerPathDrainTemp_ToTable_Unk ((void (__cdecl* )(const Point2I* blockPos))0x004321a0)

// <LegoRR.exe @00432200>
#define Level_Block_IsPowerPathDrainTemp_Unk ((bool32 (__cdecl* )(const Point2I* blockPos))0x00432200)

// <LegoRR.exe @00432230>
#define Game_UnkProcessBlockPtsTable_UnsetsFlag2_2 ((void (__cdecl* )(void))0x00432230)

// <LegoRR.exe @00432290>
#define Level_Block_UnsetFlags1_108400PwrPath_AndUnsetFlags2_4_UpdateSurface ((void (__cdecl* )(const Point2I* blockPos))0x00432290)

// <LegoRR.exe @004322f0>
#define Level_Block_UnsetGeneratePower ((void (__cdecl* )(const Point2I* blockPos))0x004322f0)

// <LegoRR.exe @00432320>
#define Level_Block_SetToolStoreBuilding ((void (__cdecl* )(const Point2I* blockPos))0x00432320)

// <LegoRR.exe @00432380>
#define Level_Block_SetSolidBuilding ((void (__cdecl* )(sint32 bx, sint32 by))0x00432380)

// <LegoRR.exe @004323c0>
#define Level_Block_SetPathBuilding ((void (__cdecl* )(sint32 bx, sint32 by))0x004323c0)

// <LegoRR.exe @00432400>
#define Level_Block_SetFenceRequest ((void (__cdecl* )(sint32 bx, sint32 by, bool32 state))0x00432400)

// <LegoRR.exe @00432450>
#define Level_Block_IsFenceRequest ((bool32 (__cdecl* )(sint32 bx, sint32 by))0x00432450)

// Some removed functionality, always returns TRUE.
// <LegoRR.exe @00432480>
#define Level_IsBuildPathBoolUnk_true ((bool32 (__cdecl* )(const Point2I* blockPos_unused))0x00432480)

// <LegoRR.exe @00432490>
#define Level_Block_SetLayedPath ((void (__cdecl* )(const Point2I* blockPos, bool32 state))0x00432490)

// <LegoRR.exe @00432500>
#define Level_Block_SetGeneratePower ((void (__cdecl* )(const Point2I* blockPos))0x00432500)

// <LegoRR.exe @00432530>
#define Level_Block_SetPath ((bool32 (__cdecl* )(const Point2I* blockPos))0x00432530)

// <LegoRR.exe @00432640>
#define Level_Block_SetFlags1_80000 ((void (__cdecl* )(const Point2I* blockPos, bool32 condition))0x00432640)

// <LegoRR.exe @004326a0>
#define LiveObject_BlockCheck_FUN_004326a0 ((bool32 (__cdecl* )(LegoObject* liveObj, uint32 bx, uint32 by, bool32 param_4, bool32 param_5))0x004326a0)

// <LegoRR.exe @00432880>
#define LiveObject_CanDynamiteBlockPos ((bool32 (__cdecl* )(LegoObject* liveObj, uint32 bx, uint32 by))0x00432880)

// <LegoRR.exe @00432900>
#define Level_Block_IsGround_alt ((bool32 (__cdecl* )(LegoObject* liveObj, uint32 bx, uint32 by))0x00432900)

// <LegoRR.exe @00432950>
#define LiveObject_CanReinforceBlock ((bool32 (__cdecl* )(LegoObject* liveObj, uint32 bx, uint32 by))0x00432950)

// <LegoRR.exe @004329d0>
#define Level_Block_IsSolidBuilding ((bool32 (__cdecl* )(uint32 bx, uint32 by, bool32 allowToolStore))0x004329d0)

// <LegoRR.exe @00432a30>
#define Level_Block_IsFlags1_800 ((bool32 (__cdecl* )(uint32 bx, uint32 by))0x00432a30)

// <LegoRR.exe @00432a80>
#define Level_Block_IsGround ((bool32 (__cdecl* )(uint32 bx, uint32 by))0x00432a80)

// <LegoRR.exe @00432ac0>
#define Level_Block_IsSeamWall ((bool32 (__cdecl* )(uint32 bx, uint32 by))0x00432ac0)

// <LegoRR.exe @00432b00>
#define Level_Block_IsWall ((bool32 (__cdecl* )(uint32 bx, uint32 by))0x00432b00)

// <LegoRR.exe @00432b50>
#define Level_Block_IsDestroyedConnection ((bool32 (__cdecl* )(uint32 bx, uint32 by))0x00432b50)

// <LegoRR.exe @00432b80>
#define Level_Block_GetRubbleLayers ((uint32 (__cdecl* )(const Point2I* blockPos))0x00432b80)

// <LegoRR.exe @00432bc0>
#define Level_Block_ClearRubbleLayer ((bool32 (__cdecl* )(const Point2I* blockPos))0x00432bc0)

// <LegoRR.exe @00432cc0>
#define Level_Block_IsReinforced ((bool32 (__cdecl* )(uint32 bx, uint32 by))0x00432cc0)

// <LegoRR.exe @00432d00>
#define Level_Block_IsBusy ((bool32 (__cdecl* )(const Point2I* blockPos))0x00432d00)

// <LegoRR.exe @00432d30>
#define Level_Block_SetBusy ((void (__cdecl* )(const Point2I* blockPos, bool32 state))0x00432d30)

// <LegoRR.exe @00432d90>
#define Level_Block_IsCorner ((bool32 (__cdecl* )(uint32 bx, uint32 by))0x00432d90)

// <LegoRR.exe @00432dc0>
#define Level_Block_IsExposed ((bool32 (__cdecl* )(const Point2I* blockPos))0x00432dc0)

// <LegoRR.exe @00432df0>
#define Level_Block_IsImmovable ((bool32 (__cdecl* )(const Point2I* blockPos))0x00432df0)

// <LegoRR.exe @00432e30>
#define Level_Block_IsLava ((bool32 (__cdecl* )(const Point2I* blockPos))0x00432e30)

// <LegoRR.exe @00432e70>
#define Level_Block_IsNotWallOrGround ((bool32 (__cdecl* )(uint32 bx, uint32 by))0x00432e70)

// <LegoRR.exe @00432ec0>
#define Level_Block_IsSurveyed ((bool32 (__cdecl* )(uint32 bx, uint32 by))0x00432ec0)

// Returns if a wall has diagonal connection between two wall tiles and two ground tiles.
// <LegoRR.exe @00432f00>
#define Level_Block_IsGap ((bool32 (__cdecl* )(uint32 bx, uint32 by))0x00432f00)

// <LegoRR.exe @00432f30>
#define Level_Block_IsCornerInner ((bool32 (__cdecl* )(uint32 bx, uint32 by))0x00432f30)

// <LegoRR.exe @00432f60>
#define Level_Block_IsPathBuilding ((bool32 (__cdecl* )(const Point2I* blockPos))0x00432f60)

// <LegoRR.exe @00432f90>
#define Level_Block_IsGeneratePower ((bool32 (__cdecl* )(const Point2I* blockPos))0x00432f90)

// <LegoRR.exe @00432fc0>
#define Level_Block_IsPath ((bool32 (__cdecl* )(const Point2I* blockPos))0x00432fc0)

// <LegoRR.exe @00433010>
#define Level_Block_IsAnyFlags1_80000_Foundation ((bool32 (__cdecl* )(const Point2I* blockPos))0x00433010)

// <LegoRR.exe @00433050>
#define Level_Block_SetFlags1_10000000 ((void (__cdecl* )(const Point2I* blockPos, bool32 state))0x00433050)

// <LegoRR.exe @004330b0>
#define LiveObject_GetRockFallInDamage_Unk ((bool32 (__cdecl* )(LegoObject* liveObj, sint32 bx, sint32 by, real32 elapsed, OPTIONAL OUT real32* value))0x004330b0)

// <LegoRR.exe @004331f0>
#define Level_Block_GetField3__routingRelated ((uint32 (__cdecl* )(uint32 bx, uint32 by))0x004331f0)

// <LegoRR.exe @00433220>
#define Level_Block_SetSurveyed ((void (__cdecl* )(uint32 bx, uint32 by))0x00433220)

// <LegoRR.exe @00433260>
#define Level_Block_GetSurfaceType ((bool32 (__cdecl* )(uint32 bx, uint32 by, OUT Lego_SurfaceType* surfaceType))0x00433260)

// <LegoRR.exe @004332b0>
#define Level_Block_LowerRoofVertices ((void (__cdecl* )(Lego_Level* level, uint32 bx, uint32 by))0x004332b0)

// <LegoRR.exe @004333f0>
#define MapShared_GetDimensions ((void (__cdecl* )(uint32 memHandle, OUT uint32* width, OUT uint32* height))0x004333f0)

// <LegoRR.exe @00433420>
#define Lego_LoadGraphicsSettings ((bool32 (__cdecl* )(void))0x00433420)

// <LegoRR.exe @004336a0>
#define Lego_LoadLighting ((bool32 (__cdecl* )(void))0x004336a0)

// <LegoRR.exe @00433b10>
#define Lego_WorldToBlockPos_NoZ ((bool32 (__cdecl* )(real32 xPos, real32 yPos, OUT sint32* bx, OUT sint32* by))0x00433b10)

// <LegoRR.exe @00433b40>
#define LiveObject_FUN_00433b40 ((bool32 (__cdecl* )(LegoObject* liveObj, real32 param_2, bool32 param_3))0x00433b40)

// <LegoRR.exe @00433d60>
#define Level_Block_IsMeshHidden ((bool32 (__cdecl* )(uint32 bx, uint32 by))0x00433d60)

// <LegoRR.exe @00433db0>
#define Lego_FPHighPolyBlocks_FUN_00433db0 ((void (__cdecl* )(Gods98::Container* contCamera, Gods98::Viewport* view, real32 fpClipBlocksMult, real32 highPolyBlocksMult))0x00433db0)

// <LegoRR.exe @00434380>
#define Lego_QsortCompareUnk_FUN_00434380 ((sint32 (__cdecl* )(sint32 param_1, sint32 param_2))0x00434380)

// <LegoRR.exe @004343b0>
#define Level_Block_Detail_FUN_004343b0 ((bool32 (__cdecl* )(Lego_Level* level, uint32 bx, uint32 by, undefined4 param_4, uint32 param_5))0x004343b0)

// <LegoRR.exe @00434460>
#define Level_RemoveAll_ProMeshes ((void (__cdecl* )(void))0x00434460)

// <LegoRR.exe @004344a0>
#define Level_Block_GetUnkField14Test ((bool32 (__cdecl* )(uint32 bx, uint32 by, real32 param_3, real32 param_4))0x004344a0)

// <LegoRR.exe @00434520>
#define Lego_LoadPanels ((void (__cdecl* )(const Gods98::Config* config, uint32 screenWidth, uint32 screenHeight))0x00434520)

// <LegoRR.exe @00434640>
#define Lego_LoadPanelButtons ((void (__cdecl* )(const Gods98::Config* config, uint32 screenWidth, uint32 screenHeight))0x00434640)

// <LegoRR.exe @00434930>
#define Lego_LoadTutorialIcon ((void (__cdecl* )(const Gods98::Config* config))0x00434930)

// <LegoRR.exe @00434980>
#define Lego_LoadSamples ((void (__cdecl* )(const Gods98::Config* config, bool32 noReduceSamples))0x00434980)

// <LegoRR.exe @00434a20>
#define Lego_LoadTextMessages ((void (__cdecl* )(const Gods98::Config* config))0x00434a20)

// <LegoRR.exe @00434b40>
#define Lego_LoadInfoMessages ((void (__cdecl* )(const Gods98::Config* config))0x00434b40)

// <LegoRR.exe @00434cd0>
#define Lego_LoadToolTips ((void (__cdecl* )(const Gods98::Config* config))0x00434cd0)

// <LegoRR.exe @00434db0>
#define Game_TryTeleportObject ((bool32 (__cdecl* )(LegoObject_Type objType, LegoObject_ID objID))0x00434db0)

// <LegoRR.exe @00434f40>
#define Level_Block_UpdateSurveyRadius_FUN_00434f40 ((void (__cdecl* )(const Point2I* blockPos, sint32 surveyRadius))0x00434f40)

// <LegoRR.exe @00434fd0>
#define Lego_LoadSurfaceTypeDescriptions_sound ((void (__cdecl* )(const Gods98::Config* config, const char* gameName))0x00434fd0)

// <LegoRR.exe @004350a0>
#define Game_GetSurfaceTypeDescription ((char* (__cdecl* )(Lego_SurfaceType surfaceType))0x004350a0)

// <LegoRR.exe @004350c0>
#define Game_GetSurfaceTypeSFX ((SFX_Type (__cdecl* )(Lego_SurfaceType surfaceType))0x004350c0)

// <LegoRR.exe @004350d0>
#define Level_SetPointer_FromSurfaceType ((void (__cdecl* )(Lego_SurfaceType surfaceType))0x004350d0)

// <LegoRR.exe @00435160>
#define Level_GenerateFallin_InRadius ((void (__cdecl* )(const Point2I* blockPos, sint32 radius, bool32 param_3))0x00435160)

// <LegoRR.exe @00435230>
#define Level_UpdateTutorialBlockFlashing ((void (__cdecl* )(Lego_Level* level, Gods98::Viewport* viewMain, real32 elapsedGame, real32 elapsedAbs))0x00435230)

// <LegoRR.exe @00435480>
#define Front_LeftButtonInputUnk_FUN_00435480 ((bool32 (__cdecl* )(void))0x00435480)

// <LegoRR.exe @004354b0>
#define Front_DrawDialogContrastOverlay ((bool32 (__cdecl* )(void))0x004354b0)

// <LegoRR.exe @004354f0>
#define SaveMenu_ConfirmMessage_FUN_004354f0 ((sint32 (__cdecl* )(const char* titleText, const char* message, const char* okText, const char* cancelText))0x004354f0)

// <LegoRR.exe @00435870>
#define Lego_EndLevel ((bool32 (__cdecl* )(void))0x00435870)

// <LegoRR.exe @00435950>
#define LegoGame_ClearSomeFlags3_FUN_00435950 ((void (__cdecl* )(void))0x00435950)

// <LegoRR.exe @00435980>
#define LegoGame_UnkTeleporterInit_FUN_00435980 ((void (__cdecl* )(void))0x00435980)

// <LegoRR.exe @004359b0>
#define LegoGame_SetAttackDefer ((void (__cdecl* )(bool32 defer))0x004359b0)

// <LegoRR.exe @004359d0>
#define LegoGame_SetCallToArmsOn ((void (__cdecl* )(bool32 callToArms))0x004359d0)


#pragma endregion

}
