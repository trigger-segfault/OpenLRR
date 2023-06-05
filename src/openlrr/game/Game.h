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
#define Lego_Initialise_orig ((bool32(__cdecl*)(void))0x0041fa80)
#define Lego_MainLoop_orig ((bool32(__cdecl*)(real32))0x00423210)
#define Lego_Shutdown_Quick_orig ((void(__cdecl*)(void))0x00424c20)
#define Lego_Shutdown_Full_orig ((void(__cdecl*)(void))0x00424c30)

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
#define LEGO_MAXPOWERGRIDBLOCKS		100

#pragma endregion

/**********************************************************************************
 ******** Enumerations
 **********************************************************************************/

#pragma region Enums

enum GameFlags1 : uint32 // [LegoRR/Lego.c|flags:0x4|type:uint]
{
	GAME1_NONE                  = 0,
	GAME1_UNUSED_1              = 0x1,
	GAME1_RADARON               = 0x2,
	GAME1_LEVELSTART            = 0x4,
	GAME1_MUSICPLAYING          = 0x8, // Music is on and playing.
	GAME1_USESFX                = 0x10,
	GAME1_USEDETAIL             = 0x20,
	GAME1_SHOWFPS               = 0x40,
	GAME1_SHOWMEMORY            = 0x80,
	GAME1_MULTISELECTING        = 0x100,
	GAME1_MOUSEBUSY             = 0x200, // Related to panel interaction and used a lot together with GAME1_MULTISELECTING.
	GAME1_DDRAWCLEAR            = 0x400,
	GAME1_RENDERPANELS          = 0x800,
	GAME1_RADAR_MAPVIEW         = 0x1000,
	GAME1_RADAR_TRACKOBJECTVIEW = 0x2000,
	GAME1_RADAR_TRACKOBJECTLOST = 0x4000, // Radar feature only found in beta.
	GAME1_FOGCOLOURRGB          = 0x8000,
	GAME1_HIGHFOGCOLOURRGB      = 0x10000,
	GAME1_RADAR_NOTRACKOBJECT   = 0x20000,
	GAME1_VERTEXMODE            = 0x40000,
	GAME1_DYNAMICPM             = 0x80000,
	GAME1_FREEZEINTERFACE       = 0x100000,
	GAME1_CAMERAGOTO            = 0x200000,
	GAME1_ONLYBUILDONPATHS      = 0x400000,
	GAME1_ALWAYSROCKFALL        = 0x800000,
	GAME1_DEBUG_NONERPS         = 0x1000000,
	GAME1_PAUSED                = 0x2000000,
	GAME1_STREAMNERPSSPEACH     = 0x4000000,
	GAME1_LEVELENDING           = 0x8000000,
	GAME1_LASERTRACKER          = 0x10000000,
	GAME1_DEBUG_SHOWVERTEXMODE  = 0x20000000,
	GAME1_DEBUG_NOCLIP_FPS      = 0x40000000,
	GAME1_VERTEXLOCKONPOINTER   = 0x80000000, // Unset by unused Vertex Mode (SingleWidthDig) after forcefully drawing a 2nd pointer over a block corner.
};
flags_end(GameFlags1, 0x4);


enum GameFlags2 : uint32 // [LegoRR/Lego.c|flags:0x4|type:uint]
{
	GAME2_NONE                 = 0,
	GAME2_CALLTOARMS           = 0x1,
	GAME2_LEVELEXITING         = 0x2, // GAME1_LEVELENDING and all units have finished teleporting up. 
	GAME2_ATTACKDEFER          = 0x4,
	GAME2_SHOWDEBUGTOOLTIPS    = 0x8,
	GAME2_ALLOWDEBUGKEYS       = 0x10,
	GAME2_ALLOWEDITMODE        = 0x20,
	GAME2_MESSAGE_HASNEXT      = 0x40,
	GAME2_MESSAGE_HASREPEAT    = 0x80,
	GAME2_INOPTIONSMENU        = 0x100,
	GAME2_CAMERAMOVING         = 0x200,
	GAME2_MOUSE_INSIDEGAMEVIEW = 0x400,
	GAME2_MUSICREADY           = 0x800, // Music is on, but not currently playing. Set by cfg: MusicOn.
	GAME2_INMENU               = 0x1000,
	GAME2_NOMULTISELECT        = 0x2000,
	GAME2_MENU_HASNEXT         = 0x4000,
	GAME2_MENU_HASPREVIOUS     = 0x8000,
	GAME2_UNUSED_10000         = 0x10000,
	GAME2_ALLOWRENAME          = 0x20000,
	GAME2_RECALLOLOBJECTS      = 0x40000,
	GAME2_GENERATESPIDERS      = 0x80000,
	GAME2_DISABLETOOLTIPSOUND  = 0x100000,
	GAME2_NOAUTOEAT            = 0x200000,
};
flags_end(GameFlags2, 0x4);


// Flags for the current user action.
enum GameFlags3 : uint32 // [LegoRR/Lego.c|flags:0x4|type:uint]
{
	GAME3_NONE          = 0,
	GAME3_ENCYCLOPEDIA  = 0x1,
	GAME3_LEGOMANGOTO   = 0x2,
	GAME3_VEHICLEGOTO   = 0x4,
	GAME3_PICKUPOBJECT  = 0x8,
	GAME3_LOADVEHICLE   = 0x10,
	GAME3_LEGOMANDIG    = 0x20,
	GAME3_VEHICLEDIG    = 0x40,
	GAME3_PLACEBUILDING = 0x80,
};
flags_end(GameFlags3, 0x4);


enum BlockFlags1 : uint32 // [LegoRR/Lego.c|flags:0x4|type:uint]
{
	BLOCK1_NONE                = 0,
	//BLOCK1_RUBBLE_VERYLOW      = 0x0,
	BLOCK1_RUBBLE_LOW          = 0x1,
	BLOCK1_RUBBLE_MEDIUM       = 0x2,
	BLOCK1_RUBBLE_FULL         = 0x3,
	//BLOCK1_RUBBLE_MASK         = 0x3,
	BLOCK1_SURVEYED            = 0x4,
	BLOCK1_FLOOR               = 0x8,
	BLOCK1_WALL                = 0x10,
	BLOCK1_REINFORCED          = 0x20,
	BLOCK1_INCORNER            = 0x40,
	BLOCK1_OUTCORNER           = 0x80,
	BLOCK1_HIGHPOLY            = 0x100,
	BLOCK1_NOTHOT              = 0x200, // Units won't take lava damage over this surface.
	BLOCK1_BUILDINGSOLID       = 0x400,
	BLOCK1_ROCKFALLFX          = 0x800,
	BLOCK1_LANDSLIDING         = 0x1000,
	BLOCK1_DIAGONAL            = 0x2000,
	BLOCK1_SMOKE               = 0x4000,
	BLOCK1_BUILDINGPATH        = 0x8000,
	BLOCK1_UNUSED_10000        = 0x10000,
	BLOCK1_HIDDEN              = 0x20000,
	BLOCK1_DESTROYEDCONNECTION = 0x40000,
	BLOCK1_BUSY_FLOOR          = 0x80000,
	BLOCK1_FOUNDATION          = 0x100000,
	BLOCK1_CLEARED             = 0x200000, // This block is NOT rubble.
	BLOCK1_EXPOSEDFLOORCHECKS  = 0x400000, // Used to handle extra checks for the first time blocks are are exposed as floors.
	                                       // Unset if checks are needed.
	BLOCK1_DIGREQUEST          = 0x800000, /// FIXME: this flag is required for the Rewards Diggable quota, but it's not set by drilling in FP.
	BLOCK1_UNUSED_PATHTYPE     = 0x1000000, // Related to reward Diggable countdown ratio.
	                                        // This flag is only checked in combination with other synthetic blocks (foundation, path, path stud).
											// It's possible it's another unused type of synthetic block.
	BLOCK1_UNUSED_2000000      = 0x2000000,
	BLOCK1_EXPOSED             = 0x4000000,
	BLOCK1_BUSY_WALL           = 0x8000000,
	BLOCK1_DOZERCLEARING       = 0x10000000,
	BLOCK1_PATH                = 0x20000000,
	BLOCK1_LAYEDPATH           = 0x40000000,
	BLOCK1_ERODEACTIVE         = 0x80000000,
};
flags_end(BlockFlags1, 0x4);


enum BlockFlags2 : uint32 // [LegoRR/Lego.c|flags:0x4|type:uint]
{
	BLOCK2_NONE              = 0,
	BLOCK2_GENERATEPOWER     = 0x1,
	BLOCK2_DRAINPOWER_TEMP   = 0x2,
	BLOCK2_TOOLSTORE         = 0x4,
	BLOCK2_FENCEREQUEST      = 0x8,
	BLOCK2_ERODELOCKED       = 0x10,
	BLOCK2_SLUGHOLE_EXPOSED  = 0x20,
	BLOCK2_EMERGE_POINT      = 0x40,
	BLOCK2_EMERGE_TRIGGER    = 0x80,
	BLOCK2_POWERED           = 0x100,
	BLOCK2_SLUGHOLE_HIDDEN   = 0x200,
	BLOCK2_PATHSBUILTCOUNTED = 0x400, // A block is flagged with this if it's currently registered as a "built object".
	                                  // This is then used to increment the number of paths built or destroyed in the level during Level_BlockUpdateSurface.
};
flags_end(BlockFlags2, 0x4);


enum BlockActivityFlags : uint8 // [LegoRR/Lego.c|flags:0x1|type:byte] 0x1 does not directly affect other flags, 0x2 is used to remove the activity after destroying has finished.
{
	BLOCKACT_FLAG_NONE       = 0,
	BLOCKACT_FLAG_STANDING   = 0x1, // "Build" activity has finished, and activity has changed to "Stand".
	BLOCKACT_FLAG_REMOVING   = 0x2, // Will be removed next UpdateAll loop.
	BLOCKACT_FLAG_DESTROYING = 0x4, // Currently playing "Destroy" activity.
};
flags_end(BlockActivityFlags, 0x1);


// hardcoded texture type enum, not used everywhere
enum TextureType : sint32 // [LegoRR/dummy.c|enum:0x4|type:int|tags:HELPER,UNUSED]
{
	TEXTURES__INVALID = -1,

	TEXTURES_ROCK     = 0,
	TEXTURES_LAVA     = 1,
	TEXTURES_ICE      = 2,
};
assert_sizeof(TextureType, 0x4);


enum PointerSFX_Type : uint32 // [LegoRR/Lego.c|enum:0x4|type:uint] Argument for Lego_SetPointerSFX (FUN_00428730)
{
	PointerSFX_Okay                  = 0,
	PointerSFX_NotOkay               = 1,
	PointerSFX_NotOkay_ImmovableRock = 2,
	PointerSFX_Okay_TopPriority      = 3,
	PointerSFX_Okay_Wall             = 4,
	PointerSFX_Okay_Floor            = 5,
};
assert_sizeof(PointerSFX_Type, 0x4);

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

struct ObjectiveData // [LegoRR/???|struct:0x54]
{
	/*00,4*/        Gods98::Image* panelImage; // (cfg: ObjectiveImage<W>x<H>[0])
	/*04,8*/        Point2F panelImagePosition; // (cfg: ObjectiveImage<W>x<H>[1,2])
	/*0c,4*/        Gods98::Image* achievedImage; // (cfg: ObjectiveAcheivedImage<W>x<H>[0]) unused
	/*10,8*/        Point2F achievedImagePosition; // (cfg: ObjectiveAcheivedImage<W>x<H>[1,2]) unused
	/*18,4*/        Gods98::Image* failedImage; // (cfg: ObjectiveFailedImage<W>x<H>[0]) unused
	/*1c,8*/        Point2F failedImagePosition; // (cfg: ObjectiveFailedImage<W>x<H>[1,2]) unused
	/*24,4*/        char* achievedVideoName; // (cfg: ObjectiveAcheivedAVI[0])
	/*28,8*/        Point2F achievedVideoPosition; // (cfg: ObjectiveAcheivedAVI[1,2]) 
	/*30,4*/        bool32 noAchievedVideo; // True if cfg:ObjectiveAcheivedAVI was not parsed correctly (or does not eixst?)
	/*34,4*/        bool32 achievedVideoPlayed;
	/*38,4*/        uint32 crystals; // (cfg: CrystalObjective) number of crystals needed if non-zero
	/*3c,4*/        uint32 ore; // (cfg: OreObjective) number of ore needed if non-zero
	/*40,8*/        Point2I blockPos; // (cfg: BlockObjective)
	/*48,4*/        real32 timer; // (cfg: TimerObjective, mult: 25.0, flags, format: "time:HitTimeFailObjective")
	/*4c,4*/        LegoObject_Type constructionType; // (cfg: ConstructionObjective)
	/*50,4*/        LegoObject_ID constructionID; // (cfg: ConstructionObjective)
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


#pragma pack(push, 1)
struct Lego_Block // [LegoRR/Lego.c|struct:0x48|pack:1]
{
	/*00,1*/	Lego_PredugType predug;
	/*01,1*/	SurfaceTexture texture;
	/*02,1*/	uint8 terrain; // as Lego_SurfaceType, but 1 byte
	/*03,1*/	uint8 direction; // clockwise (does not determine corner/wall type)
	/*04,1*/	uint8 blockpointer;
	/*05,1*/	Lego_CryOreType cryOre;
	/*06,1*/	uint8 erodeSpeed; // (Lego_ErodeType >> 1, 0 = v.slow, 1 = slow, 2 = med, 3 = fast, 4 = v.fast)
	/*07,1*/	uint8 erodeLevel; // 0 = low, 1 = med, 2 = high, 3 = max, 4 = lava
	/*08,4*/	BlockFlags1 flags1;
	/*0c,4*/	BlockFlags2 flags2;
	/*10,4*/	Construction_Zone* construct;
	/*14,4*/	real32 damage; // drill damage [0.0-1.0]
	/*18,4*/	Lego_BlockActivity* activity;
	/*1c,4*/	Smoke* smoke;
	/*20,2*/	sint16 randomness;
	/*22,2*/	uint16 seamDigCount; // Number of times a crystal/ore seam has been dug (destroyed at 4).
	/*24,4*/	uint32 numLandSlides;
	/*28,4*/	uint32 clickCount;
	/*2c,4*/	sint32 generateCrystals;
	/*30,4*/	sint32 generateOre;
	/*34,1*/	uint8 aiNode;
	/*35,3*/	undefined field_0x35_0x37[3];
	/*38,4*/	bool32 fallinUpper; // (fallin upper: 1 if fallin > 4)
	/*3c,4*/	sint32 fallinIntensity; // (fallin scale: 1-4)
	/*40,4*/	real32 fallinTimer; // (randomized with full fallin value)
	/*44,4*/	bool32 tutoHighlightState; // Tutorial block highlight color (false = tutorial color, true = normal).
	/*48*/
};
assert_sizeof(Lego_Block, 0x48);
#pragma pack(pop)


struct Lego_BlockActivity // [LegoRR/Lego.c|struct:0x1c] An effect or activity container attached to an individual block (only one per block max).
{
	/*00,4*/	Gods98::Container* cont;
	/*04,8*/	Point2I blockPos;
	/*0c,1*/	BlockActivityFlags flags;
	///*0d,3*/    uint8 padding1[3];
	/*10,4*/	Lego_BlockActivity* next;
	/*14,4*/	Lego_BlockActivity* previous;
	/*18,4*/	bool32 staticEffect; // When false, effect is treated as an activity with "Build","Stand","Destroy" animatinos.
	/*1c*/
};
assert_sizeof(Lego_BlockActivity, 0x1c);


struct Lego_Level // [LegoRR/Lego.c|struct:0x284]
{
	/*000,4*/       char* name; // (format: "Levels::level")
	/*004,4*/       Map3D* map;
	/*008,4*/       EmergeTrigger* emergeTriggers;
	/*00c,4*/       uint32 emergeCount;
	/*010,4*/       undefined4 unused_rewardID; // Presumably used to lookup level rewards by some constant ID value.
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
	/*088,4*/       Lego_BlockActivity* blockActLast; // Tail of doubly-linked list of block effects.
	/*08c,4*/       sint32 crystals; // (init: 0) Total number of Crystals stored.
	/*090,4*/       undefined4 unused_crystals_90; // (init: 0, unused) Presumably once for Crystals.
	/*094,4*/       sint32 crystalsPickedUp; // (init: 0) Used by NERPs:GetCrystalsPickedUp.
	/*098,4*/       sint32 crystalsDrained; // Crystals stored, but in-use powering buildings.
	/*09c,4*/       sint32 crystalsStolen; // For checking MaxStolen failure condition.
	/*0a0,4*/       sint32 ore; // (init: 0) Total number of normal Ore stored. (EXCLUDES STUDS)
	/*0a4,4*/       undefined4 unused_ore_a4; // (init: 0, unused) Presumably once for Ore.
	/*0a8,4*/       sint32 orePickedUp; // (init: 0) Used by NERPs:GetOrePickedUp.
	/*0ac,4*/       sint32 studs; // (init: 0) Total number of Studs stored. (1 stud == 5 ore)
	/*0b0,4*/       LegoObject_ID EmergeCreature; // (searches for object index by name, expects RockMonsterType)
	/*0b4,4*/       char* nextLevelID; // (cfg: NextLevel)
	/*0b8,4*/       Lego_Block* blocks; // grid of blocks [y][x]
	/*0bc,54*/      ObjectiveData objective;
	/*110,4*/       bool32 hasBlockPointers;
	/*114,e0*/      BlockPointer* blockPointers[LEGO_MAXBLOCKPOINTERS];
	/*1f4,4*/       bool32 StartFP;
	/*1f8,4*/       bool32 NoDrain;
	/*1fc,4*/       real32 oxygenLevel; // (init: 100.0, Oxygen level?)
	/*200,4*/       real32 OxygenRate;
	/*204,4*/       real32 oxygenLevelMessage; // Tracks change in oxygen since the last message (new message after change of 5%).
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
	/*010,4*/       Gods98::Graphics_Quality quality; // (cfg: Main::Quality)
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
	/*04c,4*/       Gods98::Container* dirLightCallToArms;
	/*050,4*/       real32 FPClipBlocks; // (cfg: Main::FPClipBlocks)
	/*054,4*/       real32 TVClipDist; // (cfg: Main::TVClipDist)
	/*058,4*/       undefined4 field_58;
	/*05c,4*/       undefined4 field_5c;
	/*060,4*/       undefined4 field_60;
	/*064,4*/       Gods98::Font* fontStandard; // Loader, Debug text, Level Select window, Pull-out info icon window, (unused) Info, Encyclopedia(?).
	/*068,4*/       Gods98::Font* fontToolTip; // ToolTip, CryOre count, Unit custom name, Teleporter queued, Interface UPG(???), Info icon count.
	/*06c,4*/       Gods98::Font* fontTallYellow; // (unused) Block / Vertex mode for unimplemented SingleWidthDig feature.
	/*070,4*/       Gods98::Font* fontTextWindow; // TextMessage window (same as fontStandard).
	/*074,4*/       Gods98::Font* fontBriefingLo; // Dark text color used by most menus that share the briefing menu background.
	/*078,4*/       Gods98::Font* fontBriefingHi; // Bright text color used by most menus that share the briefing menu background.
	/*07c,4*/       Gods98::Font* fontCredits; // Credits roll only.
	/*080,4*/       Gods98::TextWindow* textOnlyWindow;
	/*084,4*/       Gods98::TextWindow* textImageWindow; // Same as textOnlyWindow, but used instead when there's an image.
	/*088,8*/       Point2F gotoNewPos; // Upcoming/current 2D world position used when programatically moving the camera.
	/*090,8*/       Point2F gotoTargetPos; // Desired 2D world position used when programatically moving the camera (using Smooth option)
	/*098,4*/       bool32 gotoSmooth; // When false, gotoNewPos is the target position (aka, camera smoothing).
	///*088,10*/      Point2F gotoPositions_88[2];
	///*098,4*/       bool32 gotoBool_98;
	/*09c,4*/       real32 gameSpeed;
	/*0a0,4*/       LegoObject* objectFP;
	/*0a4,c*/       Vector3F vectorDragStartUnk_a4;
	/*0b0,4*/       real32 float_b0;
	/*0b4,4*/       real32 float_b4;
	/*0b8,4*/       real32 float_b8;
	/*0bc,4*/       real32 float_bc;
	/*0c0,8*/       Point2I digVertexBlockPos;   // Drilling pointer block position for unused Vertex Mode (SingleWidthDig).
	/*0c8,4*/       bool32 digVertexShowPointer; // Used with: digVertexBlockPos
	/*0cc,4*/       char* langPowerCrystal_name; // (cfg: ObjectNames)
	/*0d0,4*/       char* langOre_name; // (cfg: ObjectNames)
	/*0d4,4*/       char* langProcessedOre_name; // (cfg: ObjectNames)
	/*0d8,4*/       char* langDynamite_name; // (cfg: ObjectNames)
	/*0dc,4*/       char* langBarrier_name; // (cfg: ObjectNames)
	/*0e0,4*/       char* langElectricFence_name; // (cfg: ObjectNames)
	/*0e4,4*/       char* langSpiderWeb_name; // (cfg: ObjectNames)
	/*0e8,4*/       char* langOohScary_name; // (cfg: ObjectNames)
	/*0ec,4*/       char* langPath_name; // (cfg: ObjectNames)
	/*0f0,4*/       char* langPowerCrystal_theName; // (cfg: ObjectTheNames)
	/*0f4,4*/       char* langOre_theName; // (cfg: ObjectTheNames)
	/*0f8,4*/       char* langProcessedOre_theName; // (cfg: ObjectTheNames)
	/*0fc,4*/       char* langDynamite_theName; // (cfg: ObjectTheNames)
	/*100,4*/       char* langBarrier_theName; // (cfg: ObjectTheNames)
	/*104,4*/       char* langElectricFence_theName; // (cfg: ObjectTheNames)
	/*108,4*/       char* langSpiderWeb_theName; // (cfg: ObjectTheNames)
	/*10c,4*/       char* langOohScary_theName; // (cfg: ObjectTheNames)
	/*110,4*/       char* langPath_theName; // (cfg: ObjectTheNames)
	/*114,4*/       VehicleModel* vehicleData; // (cfg: VehicleTypes)
	/*118,4*/       CreatureModel* miniFigureData; // (cfg: MiniFigureTypes)
	/*11c,4*/       CreatureModel* rockMonsterData; // (cfg: RockMonsterTypes)
	/*120,4*/       BuildingModel* buildingData; // (cfg: BuildingTypes)
	/*124,4*/       Upgrade_PartModel* upgradeData; // (cfg: UpgradeTypes)
	/*128,4*/       char** vehicleName; // (cfg: VehicleTypes)
	/*12c,4*/       char** miniFigureName; // (cfg: MiniFigureTypes)
	/*130,4*/       char** rockMonsterName; // (cfg: RockMonsterTypes)
	/*134,4*/       char** buildingName; // (cfg: BuildingTypes)
	/*138,4*/       char** upgradeName; // (cfg: UpgradeTypes)
	/*13c,2c*/      const char* toolName[LegoObject_ToolType_Count];
	/*168,2c*/      char* langTool_name[LegoObject_ToolType_Count]; // (cfg: ToolNames)
	/*194,4*/       char** langVehicle_name; // (cfg: ObjectNames)
	/*198,4*/       char** langMiniFigure_name; // (cfg: ObjectNames)
	/*19c,4*/       char** langRockMonster_name; // (cfg: ObjectNames)
	/*1a0,4*/       char** langBuilding_name; // (cfg: ObjectNames)
	/*1a4,4*/       char** langUpgrade_name; // (cfg: ObjectNames) never specified in Lego.cfg
	/*1a8,4*/       char** langVehicle_theName; // (cfg: ObjectTheNames)
	/*1ac,4*/       char** langMiniFigure_theName; // (cfg: ObjectTheNames)
	/*1b0,4*/       char** langRockMonster_theName; // (cfg: ObjectTheNames)
	/*1b4,4*/       char** langBuilding_theName; // (cfg: ObjectTheNames)
	/*1b8,4*/       char** langUpgrade_theName; // (cfg: ObjectTheNames) never specified in Lego.cfg
	/*1bc,4*/       uint32 vehicleCount; // (cfg: VehicleTypes)
	/*1c0,4*/       uint32 miniFigureCount; // (cfg: MiniFigureTypes)
	/*1c4,4*/       uint32 rockMonsterCount; // (cfg: RockMonsterTypes)
	/*1c8,4*/       uint32 buildingCount; // (cfg: BuildingTypes)
	/*1cc,4*/       uint32 upgradeCount; // (cfg: UpgradeTypes)
	/*1d0,48*/      const char* surfaceName[Lego_SurfaceType_Count];
	/*218,48*/      char* langSurface_name[Lego_SurfaceType_Count]; // (cfg: SurfaceTypeDescriptions)
	/*260,48*/      SFX_ID langSurface_sound[Lego_SurfaceType_Count]; // (cfg: SurfaceTypeDescriptions)
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
	/*2f8,28*/      LegoObject* recordObjs[OBJECT_MAXRECORDOBJECTS];
	/*320,4*/       uint32 recordObjsCount;
	/*324,10*/      Area2F radarScreenRect;
	/*334,4*/       real32 radarZoom;
	/*338,8*/       Point2F radarCenter;
	/*340,4*/       real32 MedPolyRange; // (cfg: Main::MedPolyRange)
	/*344,4*/       real32 HighPolyRange; // (cfg: Main::HighPolyRange)
	/*348,4*/       sint32 HPBlocks; // (cfg: Main::HPBlocks)
	/*34c,c*/       ColourRGBF FogColourRGB; // (cfg: Level::FogColourRGB)
	/*358,c*/       ColourRGBF HighFogColourRGB; // (cfg: Level::HighFogColourRGB)
	/*364,4*/       real32 float_364; // level-related?
	/*368,c*/       ColourRGBF PowerCrystalRGB; // (cfg: Main::PowerCrystalRGB)
	/*374,c*/       ColourRGBF UnpoweredCrystalRGB; // (cfg: Main::UnpoweredCrystalRGB)
	/*380,4*/       LegoObject_Type placeObjType;
	/*384,4*/       LegoObject_ID placeObjID;
	/*388,4*/       Direction placeObjDirection;
	/*38c,4*/       LegoObject* placeDestSmallTeleporter;
	/*390,4*/       LegoObject* placeDestBigTeleporter;
	/*394,4*/       LegoObject* placeDestWaterTeleporter;
	/*398,4*/       real32 MinEnergyForEat; // (cfg: Main::MinEnergyForEat)
	/*39c,4*/       Gods98::Image* TutorialIcon; // (cfg: Main::TutorialIcon)
	/*3a0,4*/       undefined4 field_3a0;
	/*3a4,4*/       real32 DynamiteDamageRadius; // (cfg: Main::DynamiteDamageRadius)
	/*3a8,4*/       real32 DynamiteMaxDamage; // (cfg: Main::DynamiteMaxDamage)
	/*3ac,4*/       real32 DynamiteWakeRadius; // (cfg: Main::DynamiteWakeRadius)
	/*3b0,4*/       real32 BirdScarerRadius; // (cfg: Main::BirdScarerRadius)
	/*3b4,50*/      LegoObject_Type objTeleportQueueTypes_TABLE[LegoObject_Type_Count];
	/*404,50*/      LegoObject_ID objTeleportQueueIDs_TABLE[LegoObject_Type_Count];
	/*454,4*/       uint32 objTeleportQueue_COUNT;
	/*458,4*/       real32 MiniFigureRunAway; // (cfg: Main::MiniFigureRunAway)
	/*45c,c*/       Vector3F mouseWorldPos;
	/*468,320*/     Point2I powerDrainBlocks[LEGO_MAXPOWERGRIDBLOCKS]; // Temporary list used during powergrid calculation.
	/*788,4*/       uint32 powerDrainCount;
	/*78c,320*/     Point2I poweredBlocks[LEGO_MAXPOWERGRIDBLOCKS]; // Related to power grid calculation.
	/*aac,320*/     Point2I unpoweredBlocks[LEGO_MAXPOWERGRIDBLOCKS];
	/*dcc,4*/       uint32 poweredBlockCount;
	/*dd0,4*/       uint32 unpoweredBlockCount;
	/*dd4,4*/       uint32 MaxReturnedCrystals; // (cfg: Main::MaxReturnedCrystals)
	/*dd8,4*/       sint32 MouseScrollBorder; // (cfg: Main::MouseScrollBorder)
	/*ddc,4*/       char* langHealth_toolTip; // (cfg: ToolTipInfo::HealthText)
	/*de0,4*/       char* langEnergy_toolTip; // (cfg: ToolTipInfo::EnergyText)
	/*de4,4*/       char* langCrystals_toolTip; // (cfg: ToolTipInfo::CrystalsText)
	/*de8,4*/       char* langOre_toolTip; // (cfg: ToolTipInfo::OreText)
	/*dec,4*/       char* langStuds_toolTip; // (cfg: ToolTipInfo::StudsText)
	/*df0,4*/       char* langTools_toolTip; // (cfg: ToolTipInfo::ToolsText)
	/*df4,4*/       char* langCarryObject_toolTip; // (cfg: ToolTipInfo::CarryObjectText)
	/*df8,4*/       char* langDrivenBy_toolTip; // (cfg: ToolTipInfo::DrivenByText)
	/*dfc,4*/       char* langOreRequired_toolTip; // (cfg: ToolTipInfo::OreRequiredText)
	/*e00,4*/       bool32 IsFallinsEnabled; // (cfg not: Level::NoFallins)
	/*e04,4*/       real32 MinDistFor3DSoundsOnTopView; // (cfg: Main::MinDistFor3DSoundsOnTopView)
	/*e08,4*/       ViewMode viewMode; // FirstPerson or TopDown
	/*e0c,4*/       GameFlags1 flags1;
	/*e10,4*/       GameFlags2 flags2;
	/*e14,4*/       GameFlags3 flags3; // only first byte is used(?)
	/*e18,4*/       real32 InitialSlugTime; // (cfg: Level::InitialSlugTime)
	/*e1c,8*/       Point2F NextButtonPos; // (cfg: Main::NextButtonPos<WxH>)
	/*e24,8*/       Point2F RepeatButtonPos; // (cfg: Main::BackButtonPos<WxH>)
	/*e2c,4*/       Gods98::Image* NextButtonImage; // (cfg: Main::NextButton<WxH>)
	/*e30,4*/       Gods98::Image* RepeatButtonImage; // (cfg: Main::BackButton<WxH>)
	/*e34,4*/       Gods98::Image* BackArrowImage; // (cfg: Main::BackArrow<WxH>)
	/*e38,4*/       real32 FogRate; // (cfg: Level::FogRate)
	/*e3c,4*/       real32 timerGame_e3c;
	/*e40,4*/       real32 elapsedAbs;
	/*e44,4*/       real32 DrainTime; // (cfg: Main::DrainTime)
	/*e48,4*/       uint32 ReinforceHits; // (cfg: Main::ReinforceHits)
	/*e4c,4*/       uint32 CDStartTrack; // (cfg: Main::CDStartTrack)
	/*e50,4*/       uint32 CDTracks; // (cfg: Main::CDTracks)
	/*e54,4*/       uint32 FallinMultiplier; // (cfg: Level::FallinMultiplier)
	/*e58,4*/       bool32 hasFallins;
	/*e5c,8*/       Point2F menuNextPoint;
	/*e64,8*/       Point2F menuPrevPoint;
	/*e6c,c*/       ColourRGBF DragBoxRGB; // (cfg: Level::DragBoxRGB)
	/*e78,4*/       Gods98::Image* DialogImage; // (cfg: Dialog::Image)
	/*e7c,4*/       Gods98::Image* DialogContrastOverlay; // (cfg: Dialog::ContrastOverlay)
	/*e80,4*/       Gods98::TextWindow* DialogTextWndTitle; // (cfg: Dialog::TitleWindow)
	/*e84,4*/       Gods98::TextWindow* DialogTextWndMessage; // (cfg: Dialog::TextWindow)
	/*e88,4*/       Gods98::TextWindow* DialogTextWndOK; // (cfg: Dialog::OKWindow)
	/*e8c,4*/       Gods98::TextWindow* DialogTextWndCancel; // (cfg: Dialog::CancelWindow)
	/*e90,4*/       char* CreditsTextFile; // (cfg: Main::CreditsTextFile)
	/*e94,4*/       char* CreditsBackAVI; // (cfg: Main::CreditsBackAVI)
	/*e98,40*/      char* langUpgradeLevel_name[OBJECT_MAXLEVELS]; // (cfg: UpgradeNames)
	/*ed8,4*/       sint32 BuildingUpgradeCostOre; // (cfg: Main::BuildingUpgradeCostOre)
	/*edc,4*/       sint32 BuildingUpgradeCostStuds; // (cfg: Main::BuildingUpgradeCostStuds)
	/*ee0,4*/       char* renameInput;
	/*ee4,8*/       Point2F renamePosition;
	/*eec,4*/       char* RenameReplace; // (cfg: Main::RenameReplace)
	/*ef0,4*/       char* EndGameAVI1; // (cfg: Level::EndGameAVI1)
	/*ef4,4*/       char* EndGameAVI2; // (cfg: Level::EndGameAVI2)
	// [globs: end]
	/*ef8,8*/       Point2I mouseBlockPos; // (static, Game_unkGameLoop_FUN_00426450)
	/*f00*/
};
assert_sizeof(Lego_Globs, 0xf00);


struct GameControl_Globs // [LegoRR/???|struct:0x180|tags:GLOBS]
{
	/*000,4*/	sint32 msy_Last_1;
	/*004,4*/	sint32 msx_Last_1;
	/*008,8*/	Point2F pointf_8;
	/*010,4*/	undefined4 reserved1;
	/*014,4*/	bool32 mslb_Last_1;
	/*018,4*/	bool32 renameUseQuotes;
	/*01c,4*/	undefined4 reserved2;
	/*020,100*/	bool8 typingState_Map[256];
	/*120,4*/	real32 dbgUpgradeChangeTimer;
	/*124,4*/	real32 dbgSpeedChangeTimer;
	/*128,4*/	real32 dbgRollOffChangeTimer;
	/*12c,4*/	real32 sceneFogDelta; // Delta value used in Lego_UpdateSceneFog ((M_PI*2)/fogRate * elapsed).
	/*130,4*/	bool32 handleKeysLastLeftMouseButtonDown; // Tracks mouse released state in Lego_HandleKeys
	/*134,4*/	real32 dbgCursorLightLevel;
	/*138,4*/	bool32 isGameSpeedLocked; // When this is TRUE, game speed can only be lowered when calling `Game_SetGameSpeed`.
	/*13c,4*/	bool32 mslb_Last_3;
	/*140,4*/	bool32 mslb_Last_4;
	/*144,8*/	Point2F pointf_144;
	/*14c,4*/	bool32 mslb_Last_5;
	/*150,4*/	bool32 mslb_Last_6;
	/*154,4*/	bool32 mslb_Last_0;
	/*158,4*/	bool32 mslr_Last_0;
	/*15c,4*/	real32 handleWorldNoMouseButtonsElapsed; // Duration that neither left or right mouse buttons have been down for (in game time).
	/*160,4*/	bool32 bool_160;
	/*164,4*/	LegoObject* toolTipObject;
	/*168,4*/	bool32 dbgF10InvertLighting;
	/*16c,4*/	bool32 dbgF9DisableLightEffects;
	/*170,4*/	undefined4 reserved3;
	/*174,4*/	LegoObject* dbgGetInVehicle; // (K debug key, assigned to selected vehicle, pressing K with a rock raider will tell them to get in this registered vehicle)
	/*178,4*/	Direction direction_178;
	/*17c,4*/	real32 timerTutorialBlockFlash;
	/*180*/
};
assert_sizeof(GameControl_Globs, 0x180);


struct LegoUpdate_Globs // [LegoRR/Lego.c|struct:0x14|tags:GLOBS] (miscellaneous static function variables used during game update loop)
{
	/*00,4*/	real32 renameInputTimer; // (ram: 12.5)
	/*04,4*/	real32 dripSFXTimer; // (ram: 75.0)
	/*08,4*/	real32 ambientSFXTimer; // (ram: 250.0)
	/*0c,4*/	real32 dbgRollOffFactorValue; // (ram: 1.0)
	/*10,4*/	real32 currentBaseLightLevel; // (ram: 0.7)
	/*14*/
};
assert_sizeof(LegoUpdate_Globs, 0x14);


/// CUSTOM: Extension of `Lego_Globs` for OpenLRR-exclusive globals
struct Lego_Globs2
{
	bool legoInit; // Lego_Initialise has finished (up to the FrontEnd UI loop).
	bool topdownFogOn; // Render the same fog used in FP view while in Topdown view.
	bool topdownFPControlsOn; // Allow controling the primary unit with FP controls while in topdown view.

	// CFG on-demand lookup replacements:
	// Game properties:
	bool loseFocusAndPause; // (cfg: Main::LoseFocusAndPause)

	// Level properties:
	bool seeThroughWalls; // (cfg: Level::SeeThroughWalls)
};

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @004a4558>
extern LegoUpdate_Globs & updateGlobs;

// Only used by Lego_ShowBlockToolTip
// <LegoRR.exe @004df208>
extern Point2I & s_ShowBlockToolTip_MousePos;

// <LegoRR.exe @004df410>
extern GameControl_Globs & gamectrlGlobs;

// <LegoRR.exe @005570c0>
extern Lego_Globs & legoGlobs;

/// CUSTOM: Extension of `Lego_Globs` for OpenLRR-exclusive globals
extern Lego_Globs2 legoGlobs2;

#pragma endregion

/**********************************************************************************
 ******** Macros
 **********************************************************************************/

#pragma region Macros

/**
 * @brief Gets the 1D index of a level's block at the specified 2D coordinates.
 */
#define blockIndex(l, bx, by)			(((l)->width*(by))+(bx))
/**
 * @brief Gets the level's block at the specified 2D coordinates.
 */
#define blockValue(l, bx, by)			((l)->blocks[blockIndex((l),(bx),(by))])

#define blockInBounds(l, bx, by)		((static_cast<sint32>((bx)) >= 0 && static_cast<uint32>((bx)) < (l)->width) && \
										 (static_cast<sint32>((by)) >= 0 && static_cast<uint32>((by)) < (l)->height))


#define SurfaceType_RegisterName(n)		(legoGlobs.surfaceName[n]=#n)
#define ToolType_RegisterName(n)		(legoGlobs.toolName[n]=#n)

#define Lego_ID(...) Config_ID(LegoRR::legoGlobs.gameName, __VA_ARGS__ )
#define Main_ID(...) Config_ID(LegoRR::legoGlobs.gameName, "Main", __VA_ARGS__ )


#define Typing_IsKeyPressed(k)	(Input_IsKeyDown((k)) && !gamectrlGlobs.typingState_Map[(k)])
#define Typing_IsKeyReleased(k)	(Input_IsKeyUp((k)) && gamectrlGlobs.typingState_Map[(k)])

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

/// CUSTOM: Check if Lego_Initialise has finished (up to the FrontEnd UI loop).
inline bool Lego_IsInit() { return legoGlobs2.legoInit; }

/// CUSTOM: Check if we're currently in a level that has already started, and is not ending.
inline bool Lego_IsInLevel() { return Lego_IsInit() && legoGlobs.currLevel != nullptr && !(legoGlobs.flags1 & (GAME1_LEVELSTART|GAME1_LEVELENDING)); }


// Allows constructing buildings on terrain of any roughness.
bool Cheat_IsBuildOnAnyRoughness();
void Cheat_SetBuildOnAnyRoughness(bool on);

// Disables building and vehicle costs. Does not change upgrade costs.
bool Cheat_IsNoBuildCosts();
void Cheat_SetNoBuildCosts(bool on);

// Buildings won't request construction barriers when building.
bool Cheat_IsNoConstructionBarriers();
void Cheat_SetNoConstructionBarriers(bool on);

// Buildings will not consume power (however a power source is still needed to provide power).
bool Cheat_IsNoPowerConsumption();
void Cheat_SetNoPowerConsumption(bool on);

// Units with negative OxygenCoefs will not deplete Oxygen level.
bool Cheat_IsNoOxygenConsumption();
void Cheat_SetNoOxygenConsumption(bool on);

// Tool Stores will be able to teleport down any land vehicles, generates power, and can train any ability.
bool Cheat_IsSuperToolStore();
void Cheat_SetSuperToolStore(bool on);

// Marks all blocks in the level as scanned, so that they'll show up on the radar.
void Cheat_SurveyLevel();

// Returns true if all blocks in the level have been scanned. Only really useful with Cheat_SurveyLevel.
bool Lego_IsLevelSurveyed();

LegoObject* Lego_GetFollowUnit();

void Lego_SetFollowUnit(OPTIONAL LegoObject* liveObj);

LegoObject* Lego_GetTopdownOrFPUnit();

real32 Cheat_IsFasterUnit(LegoObject* liveObj);

real32 Cheat_GetFasterUnitCoef(LegoObject* liveObj, real32 coef);

/// CUSTOM: Change whether the multiselect drag box draws with half-transparency or no transparency.
bool Lego_IsTransparentMultiSelectBox();
void Lego_SetTransparentMultiSelectBox(bool on);

/// CUSTOM: Change whether tooltips are shown at all.
bool Lego_IsShowToolTips();
void Lego_SetShowToolTips(bool on);

LOD_PolyLevel Lego_GetTopdownLOD();
void Lego_SetTopdownLOD(LOD_PolyLevel lod);

/// <inline>
inline bool Lego_IsFreezeInterface() { return (legoGlobs.flags1 & GAME1_FREEZEINTERFACE); }

/// CUSTOM: SceneFog in topdown view
inline bool Lego_IsTopdownFogOn() { return legoGlobs2.topdownFogOn; }
/// CUSTOM: SceneFog in topdown view
inline void Lego_SetTopdownFogOn(bool on) { legoGlobs2.topdownFogOn = on; }

/// CUSTOM: FP controls for units all the time.
inline bool Lego_IsTopdownFPControlsOn() { return legoGlobs2.topdownFPControlsOn; }
/// CUSTOM: FP controls for units all the time.
inline void Lego_SetTopdownFPControlsOn(bool on) { legoGlobs2.topdownFPControlsOn = on; }

/// CUSTOM: cfg: Main::ShowDebugToolTips
inline bool Lego_IsShowDebugToolTips() { return (legoGlobs.flags2 & GAME2_SHOWDEBUGTOOLTIPS); }
void Lego_SetShowDebugToolTips(bool on);

/// CUSTOM: cfg: Main::AllowDebugKeys
inline bool Lego_IsAllowDebugKeys() { return (legoGlobs.flags2 & GAME2_ALLOWDEBUGKEYS); }
void Lego_SetAllowDebugKeys(bool on);

/// CUSTOM: cfg: Main::AllowEditMode
inline bool Lego_IsAllowEditMode() { return (legoGlobs.flags2 & GAME2_ALLOWEDITMODE); }
void Lego_SetAllowEditMode(bool on);

/// REQUIRES: Replacing `Lego_HandleKeys`, since this is always checked by CFG lookup.
/// CUSTOM: cfg: Main::LoseFocusAndPause
bool Lego_IsLoseFocusAndPause();
void Lego_SetLoseFocusAndPause(bool on);

/// REQUIRES: Replacing `Weapon_LegoObject_SeeThroughWalls_FUN_00471c20`, since this is always checked by CFG lookup.
/// CUSTOM: cfg: Level::SeeThroughWalls
bool Lego_IsSeeThroughWalls();
void Lego_SetSeeThroughWalls(bool on);

/// CUSTOM: cfg: Main::DisableToolTipSound
inline bool Lego_IsDisableToolTipSound() { return (legoGlobs.flags2 & GAME2_DISABLETOOLTIPSOUND); }
void Lego_SetDisableToolTipSound(bool disabled);

/// CUSTOM: cfg: Main::Panels
inline bool Lego_IsRenderPanels() { return (legoGlobs.flags1 & GAME1_RENDERPANELS); }
void Lego_SetRenderPanels(bool on);

/// CUSTOM: cfg: Main::Clear
inline bool Lego_IsDDrawClear() { return (legoGlobs.flags1 & GAME1_DDRAWCLEAR); }
void Lego_SetDDrawClear(bool on);

/// CUSTOM: Gets if music is enabled (either playing or ready to play).
inline bool Lego_IsMusicOn() { return (legoGlobs.flags1 & GAME1_MUSICPLAYING) || (legoGlobs.flags2 & GAME2_MUSICREADY); }

/// CUSTOM: Sets if music is enabled, and handles changing the playing state if required.
void Lego_SetMusicOn(bool on);

/// CUSTOM: Gets if music is actively playing.
inline bool Lego_IsMusicPlaying() { return (legoGlobs.flags1 & GAME1_MUSICPLAYING); }

/// CUSTOM: Updates the music playing state based on if we're in a context where music should play (specified by shouldPlay).
///         This is an extension of the Lego_SetMusicPlaying function.
void Lego_ChangeMusicPlaying(bool shouldPlay);

// <inlined>
inline bool Lego_IsSoundOn() { return (legoGlobs.flags1 & GAME1_USESFX); }

// <inlined>
inline bool Lego_IsDetailOn() { return (legoGlobs.flags1 & GAME1_USEDETAIL); }
void Lego_SetDetailOn(bool on);

// <inlined>
inline bool Lego_IsDynamicPM() { return (legoGlobs.flags1 & GAME1_DYNAMICPM); }
void Lego_SetDynamicPM(bool on);

// <inlined>
inline bool Lego_IsFPSMonitorOn() { return (legoGlobs.flags1 & GAME1_SHOWFPS); }
void Lego_SetFPSMonitorOn(bool on);

// <inlined>
inline bool Lego_IsMemoryMonitorOn() { return (legoGlobs.flags1 & GAME1_SHOWMEMORY); }
void Lego_SetMemoryMonitorOn(bool on);

// <inlined>
inline bool Lego_IsNoNERPs() { return (legoGlobs.flags1 & GAME1_DEBUG_NONERPS); }
void Lego_SetNoNERPs(bool on);

// <inlined>
inline bool Lego_IsOnlyBuildOnPaths() { return (legoGlobs.flags1 & GAME1_ONLYBUILDONPATHS); }
void Lego_SetOnlyBuildOnPaths(bool on);

// <inlined>
void Lego_SetNoclipOn(bool on);

// <inlined>
inline bool Lego_IsAlwaysRockFall() { return (legoGlobs.flags1 & GAME1_ALWAYSROCKFALL); }
void Lego_SetAlwaysRockFall(bool on);

// <inlined>
inline bool Lego_IsAllowRename() { return (legoGlobs.flags2 & GAME2_ALLOWRENAME); }
void Lego_SetAllowRename(bool on);

// <inlined>
inline bool Lego_IsGenerateSpiders() { return (legoGlobs.flags2 & GAME2_GENERATESPIDERS); }
void Lego_SetGenerateSpiders(bool on);

// <inlined>
inline bool Lego_IsNoAutoEat() { return (legoGlobs.flags2 & GAME2_NOAUTOEAT); }
void Lego_SetNoAutoEat(bool on);

// <inlined>
inline bool Lego_IsNoMultiSelect() { return (legoGlobs.flags2 & GAME2_NOMULTISELECT); }
void Lego_SetNoMultiSelect(bool on);

// <inlined>
inline bool Lego_IsFallinsOn() { return legoGlobs.IsFallinsEnabled; }
inline void Lego_SetFallinsOn(bool on) { legoGlobs.IsFallinsEnabled = on; }



// <inlined>
__inline const Gods98::Config* Lego_Config(void) { return legoGlobs.config; }

// <inlined>
__inline const char* Lego_GameName(void) { return legoGlobs.gameName; }

// <inlined>
__inline ViewMode Lego_GetViewMode(void) { return legoGlobs.viewMode; }




// <LegoRR.exe @0041f7a0>
//#define Level_IncCrystalsStored ((void (__cdecl* )(void))0x0041f7a0)
void __cdecl Level_IncCrystalsStored(void);

/// CUSTOM: Extension of Level_IncCrystalsStored for units larger than 1.
void __cdecl Level_AddCrystalsStored(sint32 crystalAmount);

// <LegoRR.exe @0041f7b0>
//#define Level_SubtractCrystalsStored ((void (__cdecl* )(sint32 crystalAmount))0x0041f7b0)
void __cdecl Level_SubtractCrystalsStored(sint32 crystalAmount);

// <LegoRR.exe @0041f7d0>
//#define Level_AddCrystalsDrained ((void (__cdecl* )(sint32 crystalDrainedAmount))0x0041f7d0)
void __cdecl Level_AddCrystalsDrained(sint32 crystalDrainedAmount);

// Previously tail of LegoObject_RequestPowerGridUpdate function.
// <LegoRR.exe @0041f7f0>
void __cdecl Level_ResetCrystalsDrained(void);

// false - GetCrystalsFree   (not powering buildings)
// true  - GetCrystalsStored (total)
// <LegoRR.exe @0041f810>
//#define Level_GetCrystalCount ((sint32 (__cdecl* )(bool32 includeDrained))0x0041f810)
sint32 __cdecl Level_GetCrystalCount(bool32 includeDrained);

// <LegoRR.exe @0041f830>
//#define Level_GetOreCount ((sint32 (__cdecl* )(bool32 isProcessed))0x0041f830)
sint32 __cdecl Level_GetOreCount(bool32 isProcessed);

// <LegoRR.exe @0041f850>
//#define Level_AddStolenCrystals ((void (__cdecl* )(sint32 stolenCrystalAmount))0x0041f850)
void __cdecl Level_AddStolenCrystals(sint32 stolenCrystalAmount);

// <LegoRR.exe @0041f870>
//#define Lego_SetRadarNoTrackObject ((void (__cdecl* )(bool32 noTrackObj))0x0041f870)
void __cdecl Lego_SetRadarNoTrackObject(bool32 noTrackObj);

// <LegoRR.exe @0041f8a0>
//#define Lego_IsNoclipOn ((bool32 (__cdecl* )(void))0x0041f8a0)
__inline bool32 __cdecl Lego_IsNoclipOn(void) { return (legoGlobs.flags1 & GAME1_DEBUG_NOCLIP_FPS); }

// <LegoRR.exe @0041f8b0>
//#define Level_IncCrystalsPickedUp ((void (__cdecl* )(void))0x0041f8b0)
void __cdecl Level_IncCrystalsPickedUp(void);

// <LegoRR.exe @0041f8c0>
//#define Level_IncOrePickedUp ((void (__cdecl* )(void))0x0041f8c0)
void __cdecl Level_IncOrePickedUp(void);

// <LegoRR.exe @0041f8d0>
//#define Level_IncOreStored ((void (__cdecl* )(bool32 isProcessed))0x0041f8d0)
void __cdecl Level_IncOreStored(bool32 isProcessed);

/// CUSTOM: Extension of Level_IncOreStored for units larger than 1.
void __cdecl Level_AddOreStored(bool32 isProcessed, sint32 oreAmount);

// <LegoRR.exe @0041f910>
//#define Level_SubtractOreStored ((void (__cdecl* )(bool32 isProcessed, sint32 oreAmount))0x0041f910)
void __cdecl Level_SubtractOreStored(bool32 isProcessed, sint32 oreAmount);

// <LegoRR.exe @0041f950>
// Gods_Go

// <LegoRR.exe @0041f9b0>
//#define Lego_QuitLevel ((void (__cdecl* )(void))0x0041f9b0)
void __cdecl Lego_QuitLevel(void);

// <LegoRR.exe @0041fa70>
//#define Lego_GetGameSpeed ((real32 (__cdecl* )(void))0x0041fa70)
__inline real32 __cdecl Lego_GetGameSpeed(void) { return legoGlobs.gameSpeed; }


// <LegoRR.exe @0041fa80>
//#define Lego_Initialise ((bool32 (__cdecl* )(void))0x0041fa80)
bool32 __cdecl Lego_Initialise(void);

// <LegoRR.exe @00422780>
#define Lego_LoadMiscObjects ((void (__cdecl* )(const Gods98::Config* config))0x00422780)

// <LegoRR.exe @00422fb0>
#define Lego_GetCurrentCamera_Container ((Gods98::Container* (__cdecl* )(void))0x00422fb0)

// <LegoRR.exe @00422fe0>
//#define Lego_GetElapsedAbs ((real32 (__cdecl* )(void))0x00422fe0)
__inline real32 __cdecl Lego_GetElapsedAbs(void) { return legoGlobs.elapsedAbs; }

// <LegoRR.exe @00422ff0>
#define Lego_DrawRenameInput ((void (__cdecl* )(real32 elapsedAbs))0x00422ff0)

// This function also has the essential behaviour of clearing INPUT.Key_Map.
// Which prevents any functions called after this during the main loop from
// triggering debug keys.
// <LegoRR.exe @00423120>
//#define Lego_HandleRenameInput ((void (__cdecl* )(void))0x00423120)
void __cdecl Lego_HandleRenameInput(void);

// <LegoRR.exe @00423210>
//#define Lego_MainLoop ((bool32 (__cdecl* )(real32 elapsed))0x00423210)
bool32 __cdecl Lego_MainLoop(real32 elapsed);

// <LegoRR.exe @00424490>
#define Level_ConsumeObjectOxygen ((void (__cdecl* )(LegoObject* liveObj, real32 elapsed))0x00424490)

// <LegoRR.exe @00424530>
#define Level_UpdateEffects ((void (__cdecl* )(Lego_Level* level, real32 elapsedGame))0x00424530)

// <LegoRR.exe @00424660>
//#define Lego_UpdateSceneFog ((void (__cdecl* )(bool32 fogEnabled, real32 elapsed))0x00424660)
void __cdecl Lego_UpdateSceneFog(bool32 fogEnabled, real32 elapsed);

// DRAW MODE: Only Draw API drawing calls can be used within this function.
// DATA: Gods98::Viewport* viewMain
// <LegoRR.exe @00424700>
//#define Lego_Callback_DrawObjectLaserTrackerBox ((bool32 (__cdecl* )(LegoObject* liveObj, void* pViewMain))0x00424700)
bool32 __cdecl Lego_Callback_DrawObjectLaserTrackerBox(LegoObject* liveObj, void* pViewMain);

// DRAW MODE: Only Draw API drawing calls can be used within this function.
// <LegoRR.exe @00424740>
//#define Lego_DrawAllLaserTrackerBoxes ((void (__cdecl* )(Gods98::Viewport* viewMain))0x00424740)
void __cdecl Lego_DrawAllLaserTrackerBoxes(Gods98::Viewport* viewMain);

/// CUSTOM: Isolate Draw API calls from Lego_DrawAllLaserTrackerBoxes.
void Lego_DrawAllLaserTrackerNames(Gods98::Viewport* viewMain);


// DRAW MODE: Only Draw API drawing calls can be used within this function.
// <LegoRR.exe @00424760>
//#define Lego_DrawAllSelectedUnitBoxes ((void (__cdecl* )(Gods98::Viewport* viewMain))0x00424760)
void __cdecl Lego_DrawAllSelectedUnitBoxes(Gods98::Viewport* viewMain);

/// CUSTOM: Isolate Draw API calls from Lego_DrawAllSelectedUnitBoxes.
void Lego_DrawAllSelectedUnitNames(Gods98::Viewport* viewMain);


// DRAW MODE: Only Draw API drawing calls can be used within this function.
// <LegoRR.exe @004247e0>
//#define Lego_DrawObjectSelectionBox ((void (__cdecl* )(LegoObject* liveObj, Gods98::Viewport* view, real32 r, real32 g, real32 b))0x004247e0)
void __cdecl Lego_DrawObjectSelectionBox(LegoObject* liveObj, Gods98::Viewport* view, real32 r, real32 g, real32 b);

/// CUSTOM: Isolate Draw API calls from Lego_DrawObjectSelectionBox.
void Lego_DrawObjectName(LegoObject* liveObj, Gods98::Viewport* view);


/// CUSTOM:
void Lego_BeginDrawSelectionBoxes();

/// CUSTOM:
void Lego_EndDrawSelectionBoxes();


// Main_State.Shutdown function
// Simply halts music playback, then calls std::exit(0); (by calling Lego_Exit();)
// (LRR_Shutdown)
// <LegoRR.exe @00424c20>
//#define Lego_Shutdown_Quick ((void (__cdecl* )(void))0x00424c20)
void __cdecl Lego_Shutdown_Quick(void);

// Main_State.Shutdown function  (ProgrammerMode_10)
// This is only called for ProgrammerMode_10. Proper game cleanup is performed here.
// (LRR_Shutdown_Debug)
// <LegoRR.exe @00424c30>
//#define Lego_Shutdown_Full ((void (__cdecl* )(void))0x00424c30)
void __cdecl Lego_Shutdown_Full(void);

// Simply halts music playback, then calls std::exit(0);
// (LRR_Exit)
// <LegoRR.exe @00424fd0>
//#define Lego_Exit ((void (__cdecl* )(void))0x00424fd0)
__declspec(noreturn) void __cdecl Lego_Exit(void);

// Returning FALSE will naturally exit the program (as handled by Lego_MainLoop).
// 
// keyDownT is unused, and no keybinds exist.
// keyDownR is unused, but debug keybinds exist for the rewards screen.
// keyDownAddSelection is only needed by Lego_HandleWorld().
// <LegoRR.exe @00424ff0>
//#define Lego_HandleKeys ((bool32 (__cdecl* )(real32 elapsedGame, real32 elapsedInterface, OUT bool32* keyDownT, OUT bool32* keyDownR, OUT bool32* keyDownAddSelection))0x00424ff0)
bool32 __cdecl Lego_HandleKeys(real32 elapsedGame, real32 elapsedInterface, OUT bool32* keyDownT, OUT bool32* keyDownR, OUT bool32* keyDownAddSelection);

// <LegoRR.exe @00425a70>
#define Lego_UpdateAll3DSounds ((bool32 (__cdecl* )(bool32 stopAll))0x00425a70)
//bool32 __cdecl Lego_UpdateAll3DSounds(bool32 stopAll);

// <LegoRR.exe @00425a90>
#define Lego_UpdateObject3DSounds ((bool32 (__cdecl* )(LegoObject* liveObj, bool32* pStopAll))0x00425a90)
//bool32 __cdecl Lego_UpdateObject3DSounds(LegoObject* liveObj, bool32* pStopAll);

// <LegoRR.exe @00425b60>
#define Lego_SetPaused ((void (__cdecl* )(bool32 checkCamDisableFlag, bool32 paused))0x00425b60)
//void __cdecl Lego_SetPaused(bool32 checkCamDisableFlag, bool32 paused);

// When set to TRUE, game speed can only be lowered, not raised.
// <LegoRR.exe @00425c00>
//#define Lego_LockGameSpeed ((void (__cdecl* )(bool32 locked))0x00425c00)
__inline void __cdecl Lego_LockGameSpeed(bool32 locked) { gamectrlGlobs.isGameSpeedLocked = locked; }

// Set new game speed and update front end speed slider.
//  If GameSpeedLocked global is true, game speed can only be lowered, not raised.
// <LegoRR.exe @00425c10>
#define Lego_SetGameSpeed ((void (__cdecl* )(real32 newGameSpeed))0x00425c10)
//void __cdecl Lego_SetGameSpeed(real32 newGameSpeed);

// <LegoRR.exe @00425c80>
#define Lego_TrackObjectInRadar ((void (__cdecl* )(LegoObject* liveObj))0x00425c80)

// <LegoRR.exe @00425cb0>
//#define Lego_IsFirstPersonView ((bool32 (__cdecl* )(void))0x00425cb0)
__inline bool32 __cdecl Lego_IsFirstPersonView() { return (legoGlobs.viewMode == ViewMode_FP); }

// <LegoRR.exe @00425cc0>
#define Lego_HandleRadarInput ((void (__cdecl* )(void))0x00425cc0)
//void __cdecl Lego_HandleRadarInput(void);

// <LegoRR.exe @004260f0>
#define Lego_UpdateSlug_FUN_004260f0 ((void (__cdecl* )(real32 elapsedGame))0x004260f0)

// <LegoRR.exe @00426160>
//#define Lego_GetMouseWorldPosition ((void (__cdecl* )(OUT Vector3F* vector))0x00426160)
__inline void __cdecl Lego_GetMouseWorldPosition(OUT Vector3F* vector) { *vector = legoGlobs.mouseWorldPos; }

// DRAW MODE: Only Draw API drawing calls can be used within this function.
// <LegoRR.exe @00426180>
//#define Lego_DrawRadarMap ((void (__cdecl* )(void))0x00426180)
void __cdecl Lego_DrawRadarMap(void);

// <LegoRR.exe @00426210>
#define Lego_SetMenuNextPosition ((void (__cdecl* )(OPTIONAL const Point2F* position))0x00426210)

// <LegoRR.exe @00426250>
#define Lego_SetMenuPreviousPosition ((void (__cdecl* )(OPTIONAL const Point2F* position))0x00426250)

// <LegoRR.exe @00426290>
#define Lego_SetFlags2_40_And_2_unkCamera ((void (__cdecl* )(bool32 onFlag40, bool32 onFlag2))0x00426290)

// <LegoRR.exe @004262d0>
#define Lego_SetFlags2_80 ((void (__cdecl* )(bool32 state))0x004262d0)

// <LegoRR.exe @004262f0>
#define Lego_UnkObjective_CompleteSub_FUN_004262f0 ((void (__cdecl* )(void))0x004262f0)

// <LegoRR.exe @00426350>
#define Lego_UpdateTopdownCamera ((void (__cdecl* )(real32 elapsedAbs))0x00426350)

// <LegoRR.exe @00426450>
#define Lego_HandleWorld ((void (__cdecl* )(real32 elapsedGame, real32 elapsedAbs, bool32 keyDownT, bool32 keyDownR, bool32 keyDownAddSelection))0x00426450)
//void __cdecl Lego_HandleWorld(real32 elapsedGame, real32 elapsedAbs, bool32 keyDownT, bool32 keyDownR, bool32 keyDownAddSelection);

// <LegoRR.exe @00427d30>
#define Lego_LoadToolTipInfos ((void (__cdecl* )(const Gods98::Config* config, const char* gameName))0x00427d30)

// <LegoRR.exe @00427eb0>
#define Lego_LoadUpgradeNames ((void (__cdecl* )(const Gods98::Config* config))0x00427eb0)


/// CUSTOM: Subfunction of Lego_ShowBlockToolTip.
ToolTip_Type Lego_PrepareObjectToolTip(LegoObject* liveObj);


// <LegoRR.exe @00427f50>
//#define Lego_ShowObjectToolTip ((void (__cdecl* )(LegoObject* liveObj))0x00427f50)
void __cdecl Lego_ShowObjectToolTip(LegoObject* liveObj);


/// CUSTOM: Subfunction of Lego_PrepareBlockToolTip.
ToolTip_Type Lego_PrepareConstructionToolTip(const Point2I* blockPos, bool32 showConstruction);

/// CUSTOM: Subfunction of Lego_PrepareBlockToolTip.
ToolTip_Type Lego_PrepareMapBlockToolTip(const Point2I* blockPos, bool32 playSound, bool32 showCavern);

/// CUSTOM: Subfunction of Lego_ShowBlockToolTip.
ToolTip_Type Lego_PrepareBlockToolTip(const Point2I* blockPos, bool32 showConstruction, bool32 playSound, bool32 showCavern);

// <LegoRR.exe @00428260>
//#define Lego_ShowBlockToolTip ((void (__cdecl* )(const Point2I* blockPos, bool32 showConstruction, bool32 playSound, bool32 showCavern))0x00428260)
void __cdecl Lego_ShowBlockToolTip(const Point2I* blockPos, bool32 showConstruction, bool32 playSound, bool32 showCavern);


// <LegoRR.exe @004286b0>
#define Level_BlockPointerCheck ((bool32 (__cdecl* )(const Point2I* blockPos))0x004286b0)

// <LegoRR.exe @00428730>
#define Lego_SetPointerSFX ((void (__cdecl* )(PointerSFX_Type pointerSFXType))0x00428730)

// bx,by : mouse-over block position.
// mouseOverObj: mouse-over object.
// <LegoRR.exe @00428810>
//#define Lego_HandleWorldDebugKeys ((void (__cdecl* )(sint32 bx, sint32 by, LegoObject* mouseOverObj, real32 noMouseButtonsElapsed))0x00428810)
void __cdecl Lego_HandleWorldDebugKeys(sint32 bx, sint32 by, LegoObject* mouseOverObj, real32 noMouseButtonsElapsed);

// <LegoRR.exe @00429040>
#define Lego_XYCallback_AddVisibleSmoke ((void (__cdecl* )(sint32 bx, sint32 by))0x00429040)

// <LegoRR.exe @00429090>
#define Lego_GetBlockDetail_ContainerTexture ((Container_Texture* (__cdecl* )(const Point2I* blockPos))0x00429090)

// <LegoRR.exe @004290d0>
#define Lego_UnkUpdateMapsWorldUnk_FUN_004290d0 ((void (__cdecl* )(real32 elapsedAbs, bool32 pass2))0x004290d0)
//void __cdecl Lego_UnkUpdateMapsWorldUnk_FUN_004290d0(real32 elapsedAbs, bool32 pass2);

// DRAW MODE: Only Draw API drawing calls can be used within this function.
// <LegoRR.exe @004292e0>
//#define Lego_DrawDragSelectionBox ((void (__cdecl* )(Lego_Level* level))0x004292e0)
void __cdecl Lego_DrawDragSelectionBox(Lego_Level* level);

// <LegoRR.exe @004293a0>
#define Lego_MainView_MouseTransform ((void (__cdecl* )(uint32 mouseX, uint32 mouseY, OUT real32* xPos, OUT real32* yPos))0x004293a0)

// <LegoRR.exe @004294d0>
//#define Lego_GetCurrentViewLight ((Gods98::Container* (__cdecl* )(void))0x004294d0)
__inline Gods98::Container* __cdecl Lego_GetCurrentViewLight(void)
{
	return ((legoGlobs.viewMode==ViewMode_FP) ? legoGlobs.pointLightFP : legoGlobs.rootSpotlight);
}

// Returns TRUE if liveObj (or its drivenObj) is the first-person unit.
// BUG: When in topdown view, returns TRUE if the objectFP is not NULL and matches the unit's drivenObj.
// <LegoRR.exe @004294f0>
//#define Lego_IsFPObject ((bool32 (__cdecl* )(LegoObject* liveObj))0x004294f0)
bool32 __cdecl Lego_IsFPObject(LegoObject* liveObj);

/// CUSTOM:
void Lego_SetSceneFogParams(ViewMode viewMode);

// <LegoRR.exe @00429520>
//#define Lego_SetViewMode ((void (__cdecl* )(ViewMode viewMode, LegoObject* liveObj, uint32 fpCameraFrame))0x00429520)
void __cdecl Lego_SetViewMode(ViewMode viewMode, LegoObject* liveObj, uint32 fpCameraFrame);

// <LegoRR.exe @004296d0>
//#define Lego_CDTrackPlayNextCallback ((void (__cdecl* )(void))0x004296d0)
void __cdecl Lego_CDTrackPlayNextCallback(void);

/// CHANGE: Only used internally by music handling functions, use Lego_ChangeMusicPlaying instead.
// <LegoRR.exe @004296e0>
//#define Lego_SetMusicPlaying ((void (__cdecl* )(bool32 on))0x004296e0)
void __cdecl Lego_SetMusicPlaying(bool32 on);

// <LegoRR.exe @00429740>
//#define Lego_SetSoundOn ((void (__cdecl* )(bool32 on))0x00429740)
void __cdecl Lego_SetSoundOn(bool32 on);

// <LegoRR.exe @00429780>
//#define Lego_GetEmergeCreatureID ((LegoObject_ID (__cdecl* )(void))0x00429780)
__inline LegoObject_ID __cdecl Lego_GetEmergeCreatureID(void) { return legoGlobs.currLevel->EmergeCreature; }

// <LegoRR.exe @00429790>
//#define Lego_GetObjectUpgradeTime ((real32 (__cdecl* )(LegoObject_Type objType))0x00429790)
__inline real32 __cdecl Lego_GetObjectUpgradeTime(LegoObject_Type objType) { return legoGlobs.currLevel->UpgradeTimes[objType]; }

// <LegoRR.exe @004297b0>
//#define Lego_GetTrainTime ((real32 (__cdecl* )(void))0x004297b0)
__inline real32 __cdecl Lego_GetTrainTime(void) { return legoGlobs.currLevel->TrainTime; }

// <LegoRR.exe @004297c0>
#define Lego_LoadLevel ((bool32 (__cdecl* )(char* levelName))0x004297c0)

// Move functionality from here into Lego_LoadLevel once that's implemented.
/// CUSTOM: Extended version of `Lego_LoadLevel` that also handles storing the SeeThroughWalls property.
/// CHANGE: Always allocate new memory for the level name, because its taken from various places that we can't rely on.
bool32 __cdecl Lego_LoadLevel2(const char* tempLevelName);

// <LegoRR.exe @0042b220>
#define Level_AddCryOreToToolStore ((bool32 (__cdecl* )(LegoObject* liveObj, SearchAddCryOre_c* search))0x0042b220)

// <LegoRR.exe @0042b260>
#define Lego_LoadDetailMeshes ((bool32 (__cdecl* )(Lego_Level* level, const char* meshBaseName))0x0042b260)

// <LegoRR.exe @0042b3b0>
#define Lego_FreeDetailMeshes ((void (__cdecl* )(Lego_Level* level))0x0042b3b0)

// <LegoRR.exe @0042b430>
//#define Lego_GetRadarMap ((RadarMap* (__cdecl* )(void))0x0042b430)
__inline RadarMap* __cdecl Lego_GetRadarMap(void) { return legoGlobs.currLevel->radarMap; }

// <LegoRR.exe @0042b440>
#define Lego_LoadMapSet ((bool32 (__cdecl* )(Lego_Level* level, const char* surfaceMap, const char* predugMap, sint32 predugParam, const char* terrainMap, sint32 terrainParam, const char* blockPointersMap, sint32 blockPointersParam, const char* cryOreMap, sint8 cryOreParam, const char* erodeMap, const char* pathMap, sint32 pathParam, const char* textureSet, const char* emergeMap, const char* aiMap, const char* fallinMap))0x0042b440)

// <LegoRR.exe @0042b780>
#define Lego_InitTextureMappings ((void (__cdecl* )(Map3D* map))0x0042b780)
//void __cdecl Lego_InitTextureMappings(Map3D* map);

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

// <LegoRR.exe @0042c260>
//#define Level_HandleEmergeTriggers ((bool32 (__cdecl* )(Lego_Level* level, const Point2I* blockPos, OUT Point2I* emergeBlockPos))0x0042c260)
bool32 __cdecl Level_HandleEmergeTriggers(Lego_Level* level, const Point2I* blockPos, OUT Point2I* emergeBlockPos);

// <LegoRR.exe @0042c370>
#define Level_Emerge_FUN_0042c370 ((void (__cdecl* )(Lego_Level* level, real32 elapsedAbs))0x0042c370)

// <LegoRR.exe @0042c3b0>
#define Lego_LoadTerrainMap ((bool32 (__cdecl* )(Lego_Level* level, const char* filename, sint32 modifier))0x0042c3b0)

// <LegoRR.exe @0042c4e0>
#define Lego_GetBlockCryOre ((bool32 (__cdecl* )(const Point2I* blockPos, OUT uint32* crystalLv0, OUT uint32* crystalLv1, OUT uint32* oreLv0, OUT uint32* oreLv1))0x0042c4e0)

// <LegoRR.exe @0042c5d0>
#define Lego_LoadCryOreMap ((bool32 (__cdecl* )(Lego_Level* level, const char* filename, sint8 modifier))0x0042c5d0)

// <LegoRR.exe @0042c690>
#define Lego_LoadPathMap ((bool32 (__cdecl* )(Lego_Level* level, const char* filename, sint32 modifier))0x0042c690)

// <LegoRR.exe @0042c900>
#define Lego_LoadFallinMap ((bool32 (__cdecl* )(Lego_Level* level, const char* filename))0x0042c900)

// <LegoRR.exe @0042caa0>
#define Lego_UpdateFallins ((void (__cdecl* )(real32 elapsedGame))0x0042caa0)

// <LegoRR.exe @0042cbc0>
#define Lego_LoadBlockPointersMap ((bool32 (__cdecl* )(Lego_Level* level, const char* filename, sint32 modifier))0x0042cbc0)

// <LegoRR.exe @0042cc80>
#define Lego_GetUpgradePartModel ((Upgrade_PartModel* (__cdecl* )(const char* upgradeName))0x0042cc80)

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

// liveObj or blockPos must be non-NULL.
// <LegoRR.exe @0042dd70>
#define Lego_Goto ((void (__cdecl* )(OPTIONAL LegoObject* liveObj, OPTIONAL const Point2I* blockPos, bool32 smooth))0x0042dd70)

// <LegoRR.exe @0042def0>
#define Lego_RemoveRecordObject ((void (__cdecl* )(LegoObject* liveObj))0x0042def0)

// <LegoRR.exe @0042df20>
#define Lego_GetRecordObject ((bool32 (__cdecl* )(uint32 recordObjPtr, OUT LegoObject** liveObj))0x0042df20)

// <LegoRR.exe @0042df50>
#define Lego_LoadOLObjectList ((bool32 (__cdecl* )(Lego_Level* level, const char* filename))0x0042df50)

// <LegoRR.exe @0042e7e0>
#define Lego_GetObjectByName ((bool32 (__cdecl* )(const char* objName, OUT LegoObject_Type* objType, OUT LegoObject_ID* objID, OUT ObjectModel** objModel))0x0042e7e0)
//__inline bool32 __cdecl Lego_GetObjectByName(const char* objName, OUT LegoObject_Type* objType, OUT LegoObject_ID* objID, OPTIONAL OUT ObjectModel** objModel);

// <LegoRR.exe @0042eca0>
#define Lego_GetObjectTypeModel ((bool32 (__cdecl* )(LegoObject_Type objType, LegoObject_ID objID, OUT ObjectModel** objModel))0x0042eca0)
//__inline bool32 __cdecl Lego_GetObjectTypeModel(LegoObject_Type objType, LegoObject_ID objID, OUT ObjectModel** objModel);

// <LegoRR.exe @0042ee70>
#define Lego_GetObjectTypeIDCount ((uint32 (__cdecl* )(LegoObject_Type objType))0x0042ee70)

// This is an old method for playing movies.
//  It supports playing a movie that isn't just in the center of the screen
//  (which can be seen by setting certain Level CFG AVI properties).
// All movie playback seen in LegoRR is done by Front_PlayMovie (which supports
//  skipping, and scales to screen, but not playing at a specified position).
// <LegoRR.exe @0042eef0>
#define Lego_PlayMovie_old ((void (__cdecl* )(const char* fName, OPTIONAL const Point2F* screenPt))0x0042eef0)

// <LegoRR.exe @0042eff0>
//#define Level_Free ((char* (__cdecl* )(void))0x0042eff0)
const char* __cdecl Level_Free(void);

// <LegoRR.exe @0042f210>
#define Level_Block_SetNotHot ((void (__cdecl* )(Lego_Level* level, uint32 bx, uint32 by, bool32 notHot))0x0042f210)

// <LegoRR.exe @0042f280>
#define Level_Block_ChoosePathTexture ((SurfaceTexture (__cdecl* )(sint32 bx, sint32 by, IN OUT uint8* direction, bool32 powered))0x0042f280)

// Reserved field is always 0, and never used.
// <LegoRR.exe @0042f620>
#define Level_BlockUpdateSurface ((void (__cdecl* )(LegoRR::Lego_Level* level, sint32 bx, sint32 by, bool32 reserved))0x0042f620)

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

// staticEffect states that the block does not perform specific activities, but simply exists with
//  the block. When false, the block will use Build/Stand/Destroy activities.
// <LegoRR.exe @00431100>
#define Level_BlockActivity_Create ((void (__cdecl* )(Lego_Level* level, const Point2I* blockPos, bool32 staticEffect))0x00431100)
//void __cdecl Level_BlockActivity_Create(Lego_Level* level, const Point2I* blockPos, bool32 staticEffect);

// <LegoRR.exe @004312e0>
#define Level_BlockActivity_UpdateAll ((void (__cdecl* )(Lego_Level* level, real32 elapsedGame))0x004312e0)
//void __cdecl Level_BlockActivity_UpdateAll(Lego_Level* level, real32 elapsedGame);

// NOTE: This does not DIRECTLY remove the block activity.
//       But it will subsequently mark it for removal in the next UpdateAll loop.
#define Level_BlockActivity_Destroy ((void (__cdecl* )(Lego_Level* level, const Point2I* blockPos, bool32 wallDestroyed))0x00431380)
//void __cdecl Level_BlockActivity_Destroy(Lego_Level* level, const Point2I* blockPos, bool32 wallDestroyed);

// <LegoRR.exe @004313f0>
#define Level_BlockActivity_Remove ((void (__cdecl* )(Lego_BlockActivity* blockAct))0x004313f0)
//void __cdecl Level_BlockActivity_Remove(Lego_BlockActivity* blockAct);

// <LegoRR.exe @00431460>
#define Level_BlockActivity_RemoveAll ((void (__cdecl* )(Lego_Level* level))0x00431460)
//void __cdecl Level_BlockActivity_RemoveAll(Lego_Level* level);

// <LegoRR.exe @00431490>
//Lego_Level* __cdecl noinline(Lego_GetLevel)(void);
__inline Lego_Level* Lego_GetLevel(void) { return legoGlobs.currLevel; }

// <LegoRR.exe @004314a0>
//Map3D* __cdecl noinline(Lego_GetMap)(void);
__inline Map3D* Lego_GetMap(void) { return Lego_GetLevel()->map; }

// <LegoRR.exe @004314b0>
#define Level_UncoverHiddenCavern ((void (__cdecl* )(uint32 bx, uint32 by))0x004314b0)

// <LegoRR.exe @004316b0>
#define Lego_PTL_RockFall ((void (__cdecl* )(uint32 bx, uint32 by, Direction direction, bool32 isBlockVertexPos))0x004316b0)

// <LegoRR.exe @004318e0>
#define Lego_GetBlockTerrain ((Lego_SurfaceType (__cdecl* )(sint32 bx, sint32 by))0x004318e0)

// <LegoRR.exe @00431910>
#define MapShared_GetBlock ((uint32 (__cdecl* )(uint32 memHandle, sint32 bx, sint32 by))0x00431910)

// <LegoRR.exe @00431960>
#define Level_FindSelectedUnit_BlockCheck_FUN_00431960 ((bool32 (__cdecl* )(uint32 bx, uint32 by, bool32 param_3))0x00431960)

// <LegoRR.exe @004319e0>
#define Level_FindSelectedLiveObject_BlockReinforce_FUN_004319e0 ((bool32 (__cdecl* )(uint32 bx, uint32 by))0x004319e0)

// <LegoRR.exe @00431a50>
//#define Level_CanBuildOnBlock ((bool32 (__cdecl* )(sint32 bx, sint32 by, bool32 param_3, bool32 param_4))0x00431a50)
bool32 __cdecl Level_CanBuildOnBlock(sint32 bx, sint32 by, bool32 param_3, bool32 param_4);

// <LegoRR.exe @00431ba0>
#define LiveObject_FUN_00431ba0 ((bool32 (__cdecl* )(LegoObject* liveObj, const Point2I* blockPos, OUT Point2I* blockOffPos, bool32 param_4))0x00431ba0)

// <LegoRR.exe @00431cd0>
#define Lego_GetCrossTerrainType ((sint32 (__cdecl* )(LegoObject* liveObj, sint32 bx1, sint32 by1, sint32 bx2, sint32 by2, bool32 param_6))0x00431cd0)

// <LegoRR.exe @00432030>
//#define Level_PowerGrid_AddPoweredBlock ((void (__cdecl* )(const Point2I* blockPos))0x00432030)
void __cdecl Level_PowerGrid_AddPoweredBlock(const Point2I* blockPos);

// <LegoRR.exe @004320a0>
//#define Level_Block_IsPowered ((bool32 (__cdecl* )(const Point2I* blockPos))0x004320a0)
bool32 __cdecl Level_Block_IsPowered(const Point2I* blockPos);

// <LegoRR.exe @004320d0>
//#define Level_PowerGrid_UpdateUnpoweredBlockSurfaces ((void (__cdecl* )(void))0x004320d0)
void __cdecl Level_PowerGrid_UpdateUnpoweredBlockSurfaces(void);

// <LegoRR.exe @00432130>
//#define Level_PowerGrid_UnpowerPoweredBlocks ((void (__cdecl* )(void))0x00432130)
void __cdecl Level_PowerGrid_UnpowerPoweredBlocks(void);

// <LegoRR.exe @004321a0>
//#define Level_PowerGrid_AddDrainPowerBlock ((void (__cdecl* )(const Point2I* blockPos))0x004321a0)
void __cdecl Level_PowerGrid_AddDrainPowerBlock(const Point2I* blockPos);

// <LegoRR.exe @00432200>
//#define Level_PowerGrid_IsDrainPowerBlock ((bool32 (__cdecl* )(const Point2I* blockPos))0x00432200)
bool32 __cdecl Level_PowerGrid_IsDrainPowerBlock(const Point2I* blockPos);

// <LegoRR.exe @00432230>
//#define Level_PowerGrid_ClearDrainPowerBlocks ((void (__cdecl* )(void))0x00432230)
void __cdecl Level_PowerGrid_ClearDrainPowerBlocks(void);

// <LegoRR.exe @00432290>
#define Level_Block_UnsetBuildingTile ((void (__cdecl* )(const Point2I* blockPos))0x00432290)

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

// <LegoRR.exe @00432480>
#define Level_IsBuildPathBoolUnk_true ((bool32 (__cdecl* )(const Point2I* blockPos))0x00432480)

// <LegoRR.exe @00432490>
#define Level_Block_SetLayedPath ((void (__cdecl* )(const Point2I* blockPos, bool32 state))0x00432490)

// <LegoRR.exe @00432500>
#define Level_Block_SetGeneratePower ((void (__cdecl* )(const Point2I* blockPos))0x00432500)

// <LegoRR.exe @00432530>
#define Level_Block_SetPath ((bool32 (__cdecl* )(const Point2I* blockPos))0x00432530)

// <LegoRR.exe @00432640>
#define Level_Block_SetBusyFloor ((void (__cdecl* )(const Point2I* blockPos, bool32 busyFloor))0x00432640)

// <LegoRR.exe @004326a0>
#define LiveObject_BlockCheck_FUN_004326a0 ((bool32 (__cdecl* )(LegoObject* liveObj, uint32 bx, uint32 by, bool32 param_4, bool32 param_5))0x004326a0)

// <LegoRR.exe @00432880>
#define LiveObject_CanDynamiteBlockPos ((bool32 (__cdecl* )(LegoObject* liveObj, uint32 bx, uint32 by))0x00432880)

// <LegoRR.exe @00432900>
#define Level_Block_IsGround_alt ((bool32 (__cdecl* )(LegoObject* liveObj, uint32 bx, uint32 by))0x00432900)

// <LegoRR.exe @00432950>
#define LiveObject_CanReinforceBlock ((bool32 (__cdecl* )(OPTIONAL LegoObject* liveObj, uint32 bx, uint32 by))0x00432950)

// <LegoRR.exe @004329d0>
#define Level_Block_IsSolidBuilding ((bool32 (__cdecl* )(uint32 bx, uint32 by, bool32 includeToolStore))0x004329d0)

// <LegoRR.exe @00432a30>
#define Level_Block_IsRockFallFX ((bool32 (__cdecl* )(uint32 bx, uint32 by))0x00432a30)

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
#define Level_Block_IsFoundationOrBusyFloor ((bool32 (__cdecl* )(const Point2I* blockPos))0x00433010)

// <LegoRR.exe @00433050>
#define Level_Block_SetDozerClearing ((void (__cdecl* )(const Point2I* blockPos, bool32 state))0x00433050)

// <LegoRR.exe @004330b0>
#define Level_GetObjectDamageFromSurface ((bool32 (__cdecl* )(LegoObject* liveObj, sint32 bx, sint32 by, real32 elapsedGame, OPTIONAL OUT real32* damage))0x004330b0)

// <LegoRR.exe @004331f0>
#define Level_Block_GetDirection ((uint32 (__cdecl* )(uint32 bx, uint32 by))0x004331f0)

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
#define Level_Block_Detail_FUN_004343b0 ((bool32 (__cdecl* )(Lego_Level* level, uint32 bx, uint32 by, real32 scaleZ, real32 brightness))0x004343b0)
//bool32 __cdecl Level_Block_Detail_FUN_004343b0(Lego_Level* level, uint32 bx, uint32 by, real32 scaleZ, real32 brightness);

// <LegoRR.exe @00434460>
#define Level_RemoveAll_ProMeshes ((void (__cdecl* )(void))0x00434460)

// destroyTime can also be drillTime.
// <LegoRR.exe @004344a0>
#define Level_Block_Damage ((bool32 (__cdecl* )(uint32 bx, uint32 by, real32 destroyTime, real32 elapsed))0x004344a0)

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
#define Lego_TryTeleportObject ((bool32 (__cdecl* )(LegoObject_Type objType, LegoObject_ID objID))0x00434db0)

// <LegoRR.exe @00434f40>
#define Level_Block_UpdateSurveyRadius_FUN_00434f40 ((void (__cdecl* )(const Point2I* blockPos, sint32 surveyRadius))0x00434f40)

// <LegoRR.exe @00434fd0>
#define Lego_LoadSurfaceTypeDescriptions_sound ((void (__cdecl* )(const Gods98::Config* config, const char* gameName))0x00434fd0)

// <LegoRR.exe @004350a0>
//#define Lego_GetSurfaceTypeDescription ((const char* (__cdecl* )(Lego_SurfaceType surfaceType))0x004350a0)
__inline const char* __cdecl Lego_GetSurfaceTypeDescription(Lego_SurfaceType surfaceType) { return ((legoGlobs.langSurface_name[surfaceType] != nullptr) ? legoGlobs.langSurface_name[surfaceType] : "Need config for surface type."); }

// <LegoRR.exe @004350c0>
//#define Lego_GetSurfaceTypeSFX ((SFX_ID (__cdecl* )(Lego_SurfaceType surfaceType))0x004350c0)
__inline SFX_ID __cdecl Lego_GetSurfaceTypeSFX(Lego_SurfaceType surfaceType) { return legoGlobs.langSurface_sound[surfaceType]; }

// <LegoRR.exe @004350d0>
#define Level_SetPointer_FromSurfaceType ((void (__cdecl* )(Lego_SurfaceType surfaceType))0x004350d0)

// <LegoRR.exe @00435160>
#define Level_GenerateFallin_InRadius ((void (__cdecl* )(const Point2I* blockPos, sint32 radius, bool32 param_3))0x00435160)

// <LegoRR.exe @00435230>
#define Level_UpdateTutorialBlockFlashing ((void (__cdecl* )(Lego_Level* level, Gods98::Viewport* viewMain, real32 elapsedGame, real32 elapsedAbs))0x00435230)

// <LegoRR.exe @00435480>
#define Lego_UpdateGameCtrlLeftButtonLast ((bool32 (__cdecl* )(void))0x00435480)

// <LegoRR.exe @004354b0>
#define Lego_DrawDialogContrastOverlay ((bool32 (__cdecl* )(void))0x004354b0)

// <LegoRR.exe @004354f0>
#define Lego_SaveMenu_ConfirmMessage_FUN_004354f0 ((sint32 (__cdecl* )(const char* titleText, const char* message, const char* okText, const char* cancelText))0x004354f0)

// <LegoRR.exe @00435870>
//#define Lego_EndLevel ((bool32 (__cdecl* )(void))0x00435870)
bool32 __cdecl Lego_EndLevel(void);

// <LegoRR.exe @00435950>
#define Lego_StopUserAction ((void (__cdecl* )(void))0x00435950)

// <LegoRR.exe @00435980>
#define Lego_UnkTeleporterInit_FUN_00435980 ((void (__cdecl* )(void))0x00435980)

// <LegoRR.exe @004359b0>
#define Lego_SetAttackDefer ((void (__cdecl* )(bool32 defer))0x004359b0)

// <LegoRR.exe @004359d0>
#define Lego_SetCallToArmsOn ((void (__cdecl* )(bool32 callToArms))0x004359d0)

#pragma endregion

}
