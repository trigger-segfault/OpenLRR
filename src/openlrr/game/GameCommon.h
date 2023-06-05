// GameCommon.h : 
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
//#include "../engine/Main.h"


namespace LegoRR
{; // !<---

/**********************************************************************************
 ******** Forward Declarations
 **********************************************************************************/

#pragma region Forward Declarations

struct AITask;
struct BlockPointer;
struct Construction_Zone;
struct Detail_Mesh;
struct Detail_TextureSet;
struct Flocks;
struct FlocksItem;
struct Lego_Block;
struct Lego_BlockActivity;
struct Lego_Level;
struct LegoObject;
struct Map3D;
struct Map3D_Block;
struct Map3D_TransitionBlock;
struct ObjectStats;
struct RadarMap;
struct RoutingBlock;
struct SelectPlace;
struct Smoke;
struct TeleporterService;

struct CreatureModel;
struct BuildingModel;
struct VehicleModel;
struct Upgrade_PartInfo;
struct Upgrade_PartModel;
struct UpgradesModel;
struct WeaponsModel;

// ObjectModel of various types: Gods98::Container, CreatureModel, BuildingModel, VehicleModel, Upgrade_PartModel
typedef void ObjectModel;

typedef uint8 DirectionByte;

#pragma endregion

/**********************************************************************************
 ******** Constants
 **********************************************************************************/

#pragma region Constants

#define LEGO_CONFIGFILENAME				"Lego.cfg"


#define OBJECT_MAXTYPES			20
#define OBJECT_MAXIDS			15
#define OBJECT_MAXLEVELS		16
#define TOOL_MAXTYPES			11
#define TOOL_MAXAITASKS			3
#define ABILITY_MAXTYPES		6
#define ACTIVITY_MAXTYPES		79

#define SURFACE_MAXTYPES		18

#define AITASK_MAXTYPES			31
#define AI_MAXPRIORITIES		27


#define EMERGE_MAXPOINTS		5

#define ADVISOR_MAXPOSITIONS	21
#define ADVISOR_MAXANIMS		11

#define BUBBLE_MAXTYPES			39

#define MESSAGE_MAXTYPES		65

#define REWARD_MAXTYPES			10

#define INFO_MAXTYPES			40
#define INTERFACE_MAXMENUITEMS	74
#define PANEL_MAXTYPES			12
#define PANEL_MAXBUTTONS		24
#define POINTER_MAXTYPES		54
#define TEXT_MAXTYPES			26
#define TOOLTIP_MAXTYPES		39

#define NERPS_MAXREGISTERS		8
#define NERPS_MAXTIMERS			4

#define ERODE_MAXUNK1			2000
#define ERODE_MAXACTIVE			2000
#define ERODE_MAXLOCKED			1000

#define ROCKFALL_MAXSTYLES		4
#define CONSTRUCTION_MAXBUILDINGBASES	5


#define LEGO_MAXMULTISELECT		100
#define LEGO_MAXBLOCKPOINTERS	56
//#define MAX_SMOKE_TYPES			4

#define LEGO_STUDORECOUNT		5

#define LEGO_SEAMCRYORECOUNT	4	// Number of crystals/ore generated from a seam before being destroyed.
#define LEGO_ERODESTAGECOUNT	4
#define LEGO_RUBBLELAYERCOUNT	4

#define OBJECT_MAXRECORDOBJECTS	10

#define OBJECT_CUSTOMNAMELENGTH		12
#define OBJECT_MAXCUSTOMNAMECHARS	(OBJECT_CUSTOMNAMELENGTH - 1)

#define OBJECT_MAXCYCLEUNITS	256


// Built-in objects: MiniFigure names
#define OBJECT_NAME_PILOT				"Pilot"			// Rock Raider


// Built-in objects: RockMonster names
#define OBJECT_NAME_ROCKMONSTER			"RockMonster"
#define OBJECT_NAME_ICEMONSTER			"IceMonster"
#define OBJECT_NAME_LAVAMONSTER			"LavaMonster"
#define OBJECT_NAME_ROCKMONSTER_TINY	"TinyRM"
#define OBJECT_NAME_ICEMONSTER_TINY		"TinyIM"
// NOTE: Tiny Lava Monsters use Tiny Rock Monster models.
#define OBJECT_NAME_LAVAMONSTER_TINY	OBJECT_NAME_ROCKMONSTER_TINY
#define OBJECT_NAME_BAT					"Bat"
#define OBJECT_NAME_SLUG				"Slug"
#define OBJECT_NAME_SMALLSPIDER			"SmallSpider"
// Unused monsters:
#define OBJECT_NAME_SPIDER				"Spider"
#define OBJECT_NAME_SNAKE				"Snake"
#define OBJECT_NAME_SCORPION			"Scorpion"


// Built-in objects: Small Vehicle names
#define OBJECT_NAME_HOVERSCOUT			"Hoverboard"
#define OBJECT_NAME_SMALLDIGGER			"SmallDigger"
#define OBJECT_NAME_SMALLTRUCK			"SmallTruck"	// Small Transport Truck
#define OBJECT_NAME_RAPIDRIDER			"SmallCat"
#define OBJECT_NAME_SMALLMLP			"SmallMLP"		// Small Mobile Laser Cutter
#define OBJECT_NAME_TUNNELSCOUT			"SmallHeli"

//#define OBJECT_NAME_HOVERBOARD			"Hoverboard"	// Hover Scout
//#define OBJECT_NAME_SMALLDIGGER			"SmallDigger"	// Small Digger
//#define OBJECT_NAME_SMALLTRUCK			"SmallTruck"	// Small Transport Truck
//#define OBJECT_NAME_SMALLCAT			"SmallCat"		// Rapid Rider
//#define OBJECT_NAME_SMALLMLP			"SmallMLP"		// Small Mobile Laser Cutter
//#define OBJECT_NAME_SMALLHELICOPTER		"SmallHeli"		// Tunnel Scout

// Built-in objects: Large Vehicle names
#define OBJECT_NAME_LOADERDOZER			"BullDozer"
#define OBJECT_NAME_GRANITEGRINDER		"WalkerDigger"
#define OBJECT_NAME_LARGEMLP			"LargeMLP"		// Large Mobile Laser Cutter
#define OBJECT_NAME_CHROMECRUSHER		"LargeDigger"
#define OBJECT_NAME_CARGOCARRIER		"LargeCat"
#define OBJECT_NAME_TUNNELTRANSPORT		"LargeHeli"

//#define OBJECT_NAME_BULLDOZER			"BullDozer"		// Loader Dozer
//#define OBJECT_NAME_WALKERDIGGER		"WalkerDigger"	// Granite Grinder
//#define OBJECT_NAME_LARGEMLP			"LargeMLP"		// Large Mobile Laser Cutter
//#define OBJECT_NAME_LARGEDIGGER			"LargeDigger"	// Chrome Crusher
//#define OBJECT_NAME_LARGECAT			"LargeCat"		// Cargo Carrier
//#define OBJECT_NAME_LARGEHELICOPTER		"LargeHeli"		// Tunnel Transport


// Built-in objects: Building names
#define OBJECT_NAME_TOOLSTORE			"Toolstation"
#define OBJECT_NAME_TELEPORTPAD			"TeleportPad"
#define OBJECT_NAME_DOCKS				"Docks"
#define OBJECT_NAME_POWERSTATION		"Powerstation"
#define OBJECT_NAME_BARRACKS			"Barracks"		// Support Station
#define OBJECT_NAME_UPGRADESTATION		"Upgrade"
#define OBJECT_NAME_GEODOME				"Geo-dome"		// Geological Center
#define OBJECT_NAME_OREREFINERY			"OreRefinery"
#define OBJECT_NAME_GUNSTATION			"Gunstation"	// Mining Laser
#define OBJECT_NAME_SUPERTELEPORT		"TeleportBIG"


// Built-in objects: Misc names
#define OBJECT_NAME_PATH				"Path"			// Fake object for counting paths built.
#define OBJECT_NAME_ELECTRICFENCE		"ElectricFence"
#define OBJECT_NAME_SPIDERWEB			"SpiderWeb"
#define OBJECT_NAME_BARRIER				"Barrier"
#define OBJECT_NAME_DYNAMITE			"Dynamite"
#define OBJECT_NAME_OOHSCARY			"OohScary"		// Sonic Blaster
#define OBJECT_NAME_ORE					"Ore"
#define OBJECT_NAME_STUD				"ProcessedOre"
#define OBJECT_NAME_POWERCRYSTAL		"PowerCrystal"


// Built-in objects: Special names for fake objects.
#define OBJECT_SPECIAL_NAME_TVCAMERA		"tvcamera"

// Built-in objects: Names that are referenced, but don't exist.
#define OBJECT_LEGACY_NAME_OREREFINERY		"Refinery"
#define OBJECT_LEGACY_NAME_CRYSTALREFINERY	"CrystalRefinery"

#pragma endregion

/**********************************************************************************
 ******** Function Typedefs
 **********************************************************************************/

#pragma region Function Typedefs

typedef void (__cdecl* XYCallback)(sint32 x, sint32 y);

typedef real32 (__cdecl* GetWorldZCallback)(real32 xPos, real32 yPos, Map3D* map);

#pragma endregion

/**********************************************************************************
 ******** Enumerations
 **********************************************************************************/

#pragma region Enums

enum AI_Priority : sint32
{
	AI_Priority_Crystal           = 0,
	AI_Priority_Ore               = 1,
	AI_Priority_DefaultCollect    = 2,
	AI_Priority_Destruction       = 3,
	AI_Priority_Construction      = 4,
	AI_Priority_Request           = 5,
	AI_Priority_Reinforce         = 6,
	AI_Priority_Repair            = 7,
	AI_Priority_Clearing          = 8,
	AI_Priority_Storage           = 9,
	AI_Priority_Refining          = 10,
	AI_Priority_HealthLow         = 11,
	AI_Priority_GetIn             = 12,
	AI_Priority_Upgrade           = 13,
	AI_Priority_BuildPath         = 14,
	AI_Priority_AttackRockMonster = 15,
	AI_Priority_Barrier           = 16,
	AI_Priority_FindLoad          = 17,
	AI_Priority_Recharge          = 18,
	AI_Priority_UpgradeBuilding   = 19,
	AI_Priority_Gather            = 20,
	AI_Priority_Steal             = 21,
	AI_Priority_Punch             = 22,
	AI_Priority_Depart            = 23,
	AI_Priority_AttackPath        = 24,
	AI_Priority_AttackObject      = 25,
	AI_Priority_Congregate        = 26,

	AI_Priority_Count,
};
assert_sizeof(AI_Priority, 0x4);


enum AITask_Type : sint32
{
	AITask_Type_Goto              = 0,
	AITask_Type_Follow            = 1,
	AITask_Type_FollowAttack      = 2,
	AITask_Type_Collect           = 3,  // see: Message_CollectCrystal
	AITask_Type_Gather            = 4,
	AITask_Type_Deposit           = 5,
	AITask_Type_Dump              = 6,
	AITask_Type_Request           = 7,
	AITask_Type_Dig               = 8,  // see: Message_Dig
	AITask_Type_Dynamite          = 9,
	AITask_Type_Repair            = 10, // see: Message_Repair
	AITask_Type_Reinforce         = 11, // see: Message_Reinforce
	AITask_Type_Clear             = 12, // see: Message_Clear
	AITask_Type_Wait              = 13,
	AITask_Type_AnimationWait     = 14,
	AITask_Type_ElecFence         = 15,
	AITask_Type_Eat               = 16,
	AITask_Type_GotoEat           = 17,
	AITask_Type_FindDriver        = 18,
	AITask_Type_GetTool           = 19, // see: Message_CollectTool
	AITask_Type_BirdScarer        = 20,
	AITask_Type_Upgrade           = 21, // see: Message_Upgrade
	AITask_Type_BuildPath         = 22, // see: Message_BuildPath
	AITask_Type_Train             = 23, // see: Message_Train
	AITask_Type_Depart            = 24,
	AITask_Type_AttackPath        = 25,
	AITask_Type_AttackRockMonster = 26,
	AITask_Type_Recharge          = 27,
	AITask_Type_Dock              = 28,
	AITask_Type_AttackObject      = 29,
	AITask_Type_FindLoad          = 30,

	AITask_Type_Count,
};
assert_sizeof(AITask_Type, 0x4);


enum Advisor_Type : sint32
{
	Advisor_Objective                        = 0,
	Advisor_ObjectiveAcheived                = 1,
	Advisor_ObjectiveFailed                  = 2,
	Advisor_IconPoint_Normal                 = 3,
	Advisor_IconPoint_BackButton             = 4,
	Advisor_IconPoint_TopButtons             = 5,
	Advisor_PanelPoint_RadarToggle           = 6,
	Advisor_PanelPoint_RadarTaggedObjectView = 7,
	Advisor_PanelPoint_RadarZoomIn           = 8,
	Advisor_PanelPoint_RadarZoomOut          = 9,
	Advisor_PanelPoint_RadarMapView          = 10,
	Advisor_PanelPoint_InfoDockGoto          = 11,
	Advisor_PanelPoint_InfoDockClose         = 12,
	Advisor_PanelPoint_TopPanelInfo          = 13,
	Advisor_PanelPoint_TopPanelOptions       = 14,
	Advisor_PanelPoint_TopPanelPriorities    = 15,
	Advisor_PanelPoint_TopPanelCallToArms    = 16,
	Advisor_PanelPoint_TopPanelGoBack        = 17,
	Advisor_PanelPoint_ControlZoomIn         = 18,
	Advisor_PanelPoint_ControlZoomOut        = 19,
	Advisor_TalkInGame                       = 20,

	Advisor_Type_Count,
};
assert_sizeof(Advisor_Type, 0x4);


enum Advisor_Anim : sint32
{
	Advisor_Anim_PointToMap = 0,
	Advisor_Anim_Point_N    = 1,
	Advisor_Anim_Point_NE   = 2,
	Advisor_Anim_Point_E    = 3,
	Advisor_Anim_Point_SE   = 4,
	Advisor_Anim_Point_S    = 5,
	Advisor_Anim_Point_SW   = 6,
	Advisor_Anim_Point_W    = 7,
	Advisor_Anim_Point_NW   = 8,
	Advisor_Anim_Point_Up   = 9,
	Advisor_Anim_Talk_Top   = 10,

	Advisor_Anim_Count,
};
assert_sizeof(Advisor_Anim, 0x4);


enum Bubble_Type : sint32
{
	Bubble_CantDo            = 0,
	Bubble_Idle              = 1,
	Bubble_CollectCrystal    = 2,
	Bubble_CollectOre        = 3,
	Bubble_CollectStud       = 4,
	Bubble_CollectDynamite   = 5,
	Bubble_CollectBarrier    = 6,
	Bubble_CollectElecFence  = 7,
	Bubble_CollectDrill      = 8,
	Bubble_CollectSpade      = 9,
	Bubble_CollectHammer     = 10,
	Bubble_CollectSpanner    = 11,
	Bubble_CollectLaser      = 12,
	Bubble_CollectPusher     = 13,
	Bubble_CollectFreezer    = 14,
	Bubble_CollectBirdScarer = 15,
	Bubble_CarryCrystal      = 16,
	Bubble_CarryOre          = 17,
	Bubble_CarryStud         = 18,
	Bubble_CarryDynamite     = 19,
	Bubble_CarryBarrier      = 20,
	Bubble_CarryElecFence    = 21,
	Bubble_Goto              = 22,
	Bubble_Dynamite          = 23,
	Bubble_Reinforce         = 24,
	Bubble_Drill             = 25, // Not to be confused with Dig (clearing with shovel).
	Bubble_Repair            = 26,
	Bubble_Dig               = 27, // Clearing with shovel, not to be confused with Drill. Naming convention varies for this.
	Bubble_Flee              = 28,
	Bubble_PowerOff          = 29,
	Bubble_CallToArms        = 30,
	Bubble_ElecFence         = 31,
	Bubble_Eat               = 32,
	Bubble_Drive             = 33,
	Bubble_Upgrade           = 34,
	Bubble_BuildPath         = 35,
	Bubble_Train             = 36,
	Bubble_Recharge          = 37,
	Bubble_Request           = 38,

	Bubble_Type_Count,
};
assert_sizeof(Bubble_Type, 0x4);


enum Message_Type : sint32
{
	Message_Null                          = 0,  // (void)
	Message_Select                        = 1,  // (LegoObject* selectObj, bool32 noDoubleSelect)
	Message_Selected                      = 2,
	Message_ClearSelection                = 3,  // (void)
	Message_Deselect                      = 4,  // (LegoObject* deselectObj)
	Message_Goto                          = 5,  // (LegoObject* moveObj, UNUSED AITask* referrerTask, const Point2I* blockPos)
	Message_RockMonsterGoto               = 6,  // (LegoObject* moveObj, UNUSED AITask* referrerTask, const Point2I* blockPos)
	Message_RockMonsterGotoComplete       = 7,  // (LegoObject* moveObj)
	Message_UserGoto                      = 8,  // (_, bool32 reevalTasks, const Point2I* blockPos) Uses all selected units.
	Message_FirstPerson                   = 9,  // (_, uint32 cameraFrame)
	Message_TrackObject                   = 10, // (void) Uses primary selected unit.
	Message_TopView                       = 11, // (void)
	Message_PlaySample                    = 12, // (_, SFX_ID sfxID)
	Message_Dig                           = 13, // (LegoObject* diggerObj, UNUSED AITask* referrerTask, const Point2I* blockPos)
	Message_DigComplete                   = 14, // (_, LOWORD(Direction blockDirection)|HIWORD(bool32 isWallConnection), const Point2I* blockPos)
	Message_Repair                        = 15, // (LegoObject* obj, UNUSED AITask* referrerTask, const Point2I* blockPos)
	Message_RepairComplete                = 16,
	Message_Reinforce                     = 17, // (LegoObject* obj, UNUSED AITask* referrerTask, const Point2I* blockPos)
	Message_ReinforceComplete             = 18,
	Message_RockFall                      = 19, // (_, LOWORD(Direction blockDirection)|HIWORD(bool32 isWallConnection), const Point2I* blockPos)
	Message_RockFallComplete              = 20, // (_, _, const Point2I* blockPos)
	Message_GenerateCrystal               = 21, // (_, _, const Point2I* blockPos)
	Message_GenerateCrystalComplete       = 22, // (LegoObject* generatedObj)
	Message_CollectCrystal                = 23, // (LegoObject* collectObj, UNUSED AITask* referrerTask, const Point2I* blockPos)
	Message_CollectCrystalComplete        = 24, // (LegoObject* collectObj, LegoObject* refineryObj)
	Message_CrystalToRefinery             = 25, // (LegoObject* carrierObj, LegoObject* refineryObj)
	Message_CrystalToRefineryComplete     = 26, // (void)
	Message_GenerateOre                   = 27, // (_, _, const Point2I* blockPos)
	Message_GenerateOreComplete           = 28, // (LegoObject* generatedObj)
	Message_CollectOre                    = 29, // (LegoObject* collectObj)
	Message_CollectOreComplete            = 30,
	Message_GenerateRockMonster           = 31, // (void)
	Message_GenerateRockMonsterComplete   = 32, // (LegoObject* generatedObj)
	Message_GatherRock                    = 33, // (LegoObject* gathererObj)
	Message_GatherRockComplete            = 34, // (LegoObject* gathererObj, LegoObject* attackTargetObj(?))
	Message_PickRandomFloor               = 35, // (LegoObject* pickerObj) liveObj is passed to Message_PickRandomFloorComplete.
	Message_PickRandomFloorComplete       = 36, // (LegoObject* pickerObj, _, const Point2I* blockPos)
	Message_AttackBuilding                = 37, // (LegoObject* attackerObj, LegoObject* attackTargetObj)
	Message_AttackBuildingComplete        = 38,
	Message_Clear                         = 39, // (LegoObject* obj, UNUSED AITask* referrerTask, const Point2I* blockPos)
	Message_ClearComplete                 = 40, // (_, _, const Point2I* blockPos)
	Message_GetIn                         = 41, // <unused>
	Message_ManSelectedAndMonsterClicked  = 42, // <unused>
	Message_FollowAttack                  = 43, // (LegoObject* followerObj, LegoObject* attackTargetObj)
	Message_CollectTool                   = 44, // (LegoObject* collectorObj, UNUSED AITask* referrerTask, const Point2I* blockPos)
	Message_ReduceSelection               = 45, // (void)
	Message_ClearFallIn                   = 46,
	Message_ClearFallInComplete           = 47, // (_, _, const Point2I* blockPos)
	Message_BuildPath                     = 48, // (LegoObject* builderObj(?), UNUSED AITask* referrerTask, const Point2I* blockPos)
	Message_BuildPathComplete             = 49,
	Message_Train                         = 50, // (LegoObject* trainingObj, UNUSED AITask* referrerTask, const Point2I* blockPos)
	Message_TrainComplete                 = 51,
	Message_GenerateCrystalAndOre         = 52, // (_, _, const Point2I* blockPos)
	Message_GenerateCrystalAndOreComplete = 53,
	Message_GenerateFromCryOre            = 54, // (_, _, const Point2I* blockPos)
	Message_GenerateFromCryOreComplete    = 55,
	Message_Upgrade                       = 56, // (LegoObject* upgradingObj, UNUSED AITask* referrerTask, const Point2I* blockPos)
	Message_UpgradeComplete               = 57,
	Message_ClearBuilding                 = 58,
	Message_ClearBuildingComplete         = 59, // (_, _, const Point2I* blockPos)
	Message_ClearInitial                  = 60,
	Message_ClearInitialComplete          = 61, // (_, _, const Point2I* blockPos)
	Message_ClearRemovePath               = 62,
	Message_ClearRemovePathComplete       = 63, // (_, _, const Point2I* blockPos)
	Message_Debug_DestroyAll              = 64, // (void)

	Message_Type_Count,
};
assert_sizeof(Message_Type, 0x4);


enum Reward_Type : sint32
{
	Reward_Crystals      = 0,
	Reward_Ore           = 1,
	Reward_Diggable      = 2,
	Reward_Constructions = 3,
	Reward_Caverns       = 4,
	Reward_Figures       = 5,
	Reward_RockMonsters  = 6,
	Reward_Oxygen        = 7,
	Reward_Timer         = 8,
	Reward_Score         = 9,

	Reward_Type_Count,
};
assert_sizeof(Reward_Type, 0x4);


enum Info_Type : sint32
{
	Info_CrystalFound         = 0,
	Info_OreSeamFound         = 1,
	Info_RockMonster          = 2,
	Info_LavaRockMonster      = 3,
	Info_IceRockMonster       = 4,
	Info_UnderAttack          = 5,
	Info_Landslide            = 6,
	Info_CaveIn               = 7,
	Info_Constructed          = 8,
	Info_CavernLocated        = 9,
	Info_LegoManDeath         = 10,
	Info_VehicleDeath         = 11,
	Info_BuildingDeath        = 12,
	Info_DynamitePlaced       = 13,
	Info_NoPower              = 14,
	Info_PowerDrain           = 15,
	Info_AirDepleting         = 16,
	Info_AirLow               = 17,
	Info_AirOut               = 18,
	Info_AirRestored          = 19,
	Info_TrainDriver          = 20,
	Info_TrainDynamite        = 21,
	Info_TrainRepair          = 22,
	Info_TrainPilot           = 23,
	Info_TrainSailor          = 24,
	Info_TrainScanner         = 25,
	Info_OreCollected         = 26,
	Info_WallDug              = 27,
	Info_WallReinforced       = 28,
	Info_CrystalPower         = 29,
	Info_LavaErode            = 30,
	Info_SlugEmerge           = 31,
	Info_PathCompleted        = 32,
	Info_FoundMinifigure      = 33,
	Info_CanUpgradeMinifigure = 34,
	Info_CanTrainMinifigure   = 35,
	Info_CrystalSeamFound     = 36,
	Info_GenericSeamFound     = 37,
	Info_GenericDeath         = 38,
	Info_GenericMonster       = 39,

	Info_Type_Count,
};
assert_sizeof(Info_Type, 0x4);


enum ToolTip_Type : sint32
{
	ToolTip_Null                       = 0,

	ToolTip_RadarBlock                 = 1,
	ToolTip_UnitSelect                 = 2,
	ToolTip_Construction               = 3,
	ToolTip_InterfaceMenu              = 4,
	ToolTip_InterfaceMenuBackButton    = 5,
	ToolTip_MapBlock                   = 6,
	ToolTip_Priority                   = 7,
	ToolTip_InfoMenuContinue           = 8,
	ToolTip_InfoMenuDisableFuture      = 9,
	ToolTip_RadarToggle                = 10,
	ToolTip_RadarObjectView            = 11,
	ToolTip_RadarZoomIn                = 12,
	ToolTip_RadarZoomOut               = 13,
	ToolTip_RadarMapView               = 14,
	ToolTip_InfoDockGoto               = 15,
	ToolTip_InfoDockClose              = 16,
	ToolTip_TopPanelCallToArms         = 17,
	ToolTip_TopPanelInfo               = 18,
	ToolTip_TopPanelOptions            = 19,
	ToolTip_TopPanelPriorities         = 20,
	ToolTip_PriorityDisable            = 21,
	ToolTip_PriorityUpOne              = 22,
	ToolTip_PriorityReset              = 23,
	ToolTip_CamControlZoomIn           = 24,
	ToolTip_CamControlZoomOut          = 25,
	ToolTip_CamControlCycle            = 26,
	ToolTip_CamControlRotate           = 27,
	ToolTip_SideBar_Ore                = 28,
	ToolTip_SideBar_Crystals           = 29,
	ToolTip_Close                      = 30,
	ToolTip_PreviousMessage            = 31,
	ToolTip_NextMessage                = 32,
	ToolTip_More                       = 33,
	ToolTip_Back                       = 34,
	ToolTip_CamControlCycleMinifigures = 35,
	ToolTip_Reward_Save                = 36,
	ToolTip_Reward_Advance             = 37,
	ToolTip_FrontEnd_Back              = 38,

	ToolTip_Type_Count,
};
assert_sizeof(ToolTip_Type, 0x4);


enum Text_Type : sint32
{
	Text_Walk                = 0,
	Text_Drill               = 1,
	Text_NoDrill             = 2,
	Text_CantDrill           = 3,
	Text_Reinforce           = 4,
	Text_NoReinforce         = 5,
	Text_CantReinforce       = 6,
	Text_PickupCrystal       = 7,
	Text_Encyclopedia        = 8,
	Text_RadarHelp           = 9,
	Text_BuildHelp           = 10,
	Text_SelectionHelp       = 11,
	Text_Drive               = 12,
	Text_CantDrive           = 13,
	Text_MakeTeleporter      = 14,
	Text_CannotPlaceBuilding = 15,
	Text_CrystalOreDisabled  = 16,
	Text_CrystalFound        = 17,
	Text_CavernDiscovered    = 18,
	Text_OreFound            = 19,
	Text_AirSupplyLow        = 20,
	Text_AirSupplyRunningOut = 21,
	Text_SpaceToContinue     = 22,
	Text_GameCompleted       = 23,
	Text_ManTrained          = 24,
	Text_UnitUpgraded        = 25,

	Text_Type_Count,
};
assert_sizeof(Text_Type, 0x4);


enum Interface_MenuType : sint32
{
	Interface_Menu_Main              = 0,
	Interface_Menu_Ground            = 1,
	Interface_Menu_Erode             = 2,
	Interface_Menu_PlaceFence        = 3,
	Interface_Menu_Construction      = 4,
	Interface_Menu_Rubble            = 5,
	Interface_Menu_UNK_6             = 6,
	Interface_Menu_Wall              = 7,
	Interface_Menu_Tracker           = 8,
	Interface_Menu_LegoMan           = 9,
	Interface_Menu_LandVehicle       = 10,
	Interface_Menu_WaterVehicle      = 11,
	Interface_Menu_UnmannedVehicle   = 12,
	Interface_Menu_Building          = 13,
	Interface_Menu_ElectricFence     = 14,
	Interface_Menu_FP                = 15,
	Interface_Menu_GetTool           = 16,
	Interface_Menu_TrainSkill        = 17,
	Interface_Menu_UpgradeVehicle    = 18,
	Interface_Menu_BuildBuilding     = 19,
	Interface_Menu_BuildSmallVehicle = 20,
	Interface_Menu_BuildLargeVehicle = 21,

	Interface_Menu_Type_Count, // May not be correct after learning about Encyclopedia

	Interface_Menu_Encyclopedia      = 23,
};
assert_sizeof(Interface_MenuType, 0x4);


enum Interface_MenuItemType : sint32
{
	Interface_MenuItem_Back                  = 0,

	Interface_MenuItem_TeleportMan           = 1,
	Interface_MenuItem_BuildBuilding         = 2,
	Interface_MenuItem_BuildSmallVehicle     = 3,
	Interface_MenuItem_BuildLargeVehicle     = 4,
	Interface_MenuItem_LayPath               = 5,
	Interface_MenuItem_RemovePath            = 6,
	Interface_MenuItem_RepairLava            = 7,
	Interface_MenuItem_GeologistTest         = 8,
	Interface_MenuItem_ClearRubble           = 9,
	Interface_MenuItem_Dam                   = 10,
	Interface_MenuItem_Dig                   = 11,
	Interface_MenuItem_Reinforce             = 12,
	Interface_MenuItem_Dynamite              = 13,
	Interface_MenuItem_PlaceFence            = 14,
	Interface_MenuItem_DeselectDig           = 15,
	Interface_MenuItem_CancelConstruction    = 16,
	Interface_MenuItem_SelectMan             = 17,
	Interface_MenuItem_SelectVehicle         = 18,
	Interface_MenuItem_SelectBuilding        = 19,
	Interface_MenuItem_VehiclePickUp         = 20,
	Interface_MenuItem_MinifigurePickUp      = 21,
	Interface_MenuItem_UnLoadVehicle         = 22,
	Interface_MenuItem_UnLoadMinifigure      = 23,
	Interface_MenuItem_GetIn                 = 24,
	Interface_MenuItem_GetOut                = 25,
	Interface_MenuItem_GotoDock              = 26,
	Interface_MenuItem_LegoManGoto           = 27,
	Interface_MenuItem_VehicleGoto           = 28,
	Interface_MenuItem_VehicleDig            = 29,
	Interface_MenuItem_LegoManDig            = 30,
	Interface_MenuItem_GoFeed                = 31,
	Interface_MenuItem_DeleteMan             = 32,
	Interface_MenuItem_DropBirdScarer        = 33,
	Interface_MenuItem_PowerOn               = 34,
	Interface_MenuItem_PowerOff              = 35,
	Interface_MenuItem_Repair                = 36,
	Interface_MenuItem_MakeTeleporterPrimary = 37,
	Interface_MenuItem_EjectCrystal          = 38,
	Interface_MenuItem_EjectOre              = 39,
	Interface_MenuItem_DeleteBuilding        = 40,
	Interface_MenuItem_DeleteVehicle         = 41,
	Interface_MenuItem_DeleteElectricFence   = 42,
	Interface_MenuItem_Attack                = 43,
	Interface_MenuItem_GotoTopView           = 44,
	Interface_MenuItem_GotoFirstPerson       = 45,
	Interface_MenuItem_GotoSecondPerson      = 46,
	Interface_MenuItem_TrackObject           = 47,
	Interface_MenuItem_GetTool               = 48,
	Interface_MenuItem_GetDrill              = 49,
	Interface_MenuItem_GetSpade              = 50,
	Interface_MenuItem_GetHammer             = 51,
	Interface_MenuItem_GetSpanner            = 52,
	Interface_MenuItem_GetLaser              = 53,
	Interface_MenuItem_GetPusherGun          = 54,
	Interface_MenuItem_GetFreezerGun         = 55,
	Interface_MenuItem_GetBirdScarer         = 56,
	Interface_MenuItem_TrainSkill            = 57,
	Interface_MenuItem_TrainDriver           = 58,
	Interface_MenuItem_TrainEngineer         = 59,
	Interface_MenuItem_TrainGeologist        = 60,
	Interface_MenuItem_TrainPilot            = 61,
	Interface_MenuItem_TrainSailor           = 62,
	Interface_MenuItem_TrainDynamite         = 63,
	Interface_MenuItem_UpgradeMan            = 64,
	Interface_MenuItem_UpgradeBuilding       = 65,
	Interface_MenuItem_UpgradeVehicle        = 66,
	Interface_MenuItem_UpgradeEngine         = 67,
	Interface_MenuItem_UpgardeDrill          = 68,
	Interface_MenuItem_UpgardeScan           = 69,
	Interface_MenuItem_UpgardeCarry          = 70,
	Interface_MenuItem_Encyclopedia          = 71,
	Interface_MenuItem_ClearSelection        = 72,
	Interface_MenuItem_Build                 = 73,

	Interface_MenuItem_Type_Count,
};
assert_sizeof(Interface_MenuItemType, 0x4);


enum Panel_Type : sint32
{
	Panel_Radar          = 0,
	Panel_RadarFill      = 1,
	Panel_RadarOverlay   = 2,
	Panel_Messages       = 3,
	Panel_MessagesSide   = 4,
	Panel_CrystalSideBar = 5,
	Panel_TopPanel       = 6,
	Panel_Information    = 7,
	Panel_PriorityList   = 8,
	Panel_CameraControl  = 9,
	Panel_InfoDock       = 10,
	Panel_Encyclopedia   = 11,

	Panel_Type_Count,
};
assert_sizeof(Panel_Type, 0x4);


enum PanelButton_Type : sint32
{
	PanelButton_Radar_Toggle           = 0,
	PanelButton_Radar_TaggedObjectView = 1,
	PanelButton_Radar_ZoomIn           = 2,
	PanelButton_Radar_ZoomOut          = 3,
	PanelButton_Radar_MapView          = 4,
	PanelButton_Radar_Count,

	PanelButton_CrystalSideBar_Ore      = 0,
	PanelButton_CrystalSideBar_Crystals = 1,
	PanelButton_CrystalSideBar_Count,

	PanelButton_TopPanel_Options    = 0,
	PanelButton_TopPanel_Priorities = 1,
	PanelButton_TopPanel_CallToArms = 2,
	PanelButton_TopPanel_Count,

	PanelButton_Information_Toggle   = 0,
	PanelButton_Information_Function = 1,
	PanelButton_Information_Count,

	PanelButton_PriorityList_Disable1 = 0,
	PanelButton_PriorityList_Disable2 = 1,
	PanelButton_PriorityList_Disable3 = 2,
	PanelButton_PriorityList_Disable4 = 3,
	PanelButton_PriorityList_Disable5 = 4,
	PanelButton_PriorityList_Disable6 = 5,
	PanelButton_PriorityList_Disable7 = 6,
	PanelButton_PriorityList_Disable8 = 7,
	PanelButton_PriorityList_Disable9 = 8,
	PanelButton_PriorityList_UpOne1   = 9,
	PanelButton_PriorityList_UpOne2   = 10,
	PanelButton_PriorityList_UpOne3   = 11,
	PanelButton_PriorityList_UpOne4   = 12,
	PanelButton_PriorityList_UpOne5   = 13,
	PanelButton_PriorityList_UpOne6   = 14,
	PanelButton_PriorityList_UpOne7   = 15,
	PanelButton_PriorityList_UpOne8   = 16,
	PanelButton_PriorityList_Close    = 17,
	PanelButton_PriorityList_Reset    = 18,
	PanelButton_PriorityList_Count,

	PanelButton_CameraControl_ZoomIn         = 0,
	PanelButton_CameraControl_ZoomOut        = 1,
	PanelButton_CameraControl_CycleBuildings = 2,
	PanelButton_CameraControl_Rotate         = 3,
	PanelButton_CameraControl_Count,

	PanelButton_InfoDock_Goto  = 0,
	PanelButton_InfoDock_Close = 1,
	PanelButton_InfoDock_Count,

	PanelButton_Encyclopedia_Close = 0,
	PanelButton_Encyclopedia_Count,


	PanelButton_Type_Count = 24,
};
assert_sizeof(PanelButton_Type, 0x4);


enum Pointer_Type : sint32
{
	Pointer_Standard                 = 0,
	Pointer_Blank                    = 1,
	Pointer_Selected                 = 2,
	Pointer_Drill                    = 3,
	Pointer_CantDrill                = 4,
	Pointer_Clear                    = 5,
	Pointer_Go                       = 6,
	Pointer_CantGo                   = 7,
	Pointer_Teleport                 = 8,
	Pointer_CantTeleport             = 9,
	Pointer_Reinforce                = 10,
	Pointer_CantReinforce            = 11,
	Pointer_RadarPan                 = 12,
	Pointer_TrackObject              = 13,
	Pointer_Help                     = 14,
	Pointer_CantHelp                 = 15,
	Pointer_PutDown                  = 16,
	Pointer_GetIn                    = 17,
	Pointer_GetOut                   = 18,
	Pointer_TutorialBlockInfo        = 19,
	Pointer_Okay                     = 20,
	Pointer_NotOkay                  = 21,
	Pointer_CanBuild                 = 22,
	Pointer_CannotBuild              = 23,
	Pointer_Dynamite                 = 24,
	Pointer_CantDynamite             = 25,
	Pointer_PickUp                   = 26,
	Pointer_CantPickUp               = 27,
	Pointer_PickUpOre                = 28,
	Pointer_LegoManCantDig           = 29,
	Pointer_VehicleCantDig           = 30,
	Pointer_LegoManDig               = 31,
	Pointer_VehicleDig               = 32,
	Pointer_LegoManCantPickUp        = 33,
	Pointer_VehicleCantPickUp        = 34,
	Pointer_LegoManPickUp            = 35,
	Pointer_VehiclePickUp            = 36,
	Pointer_LegoManCantGo            = 37,
	Pointer_VehicleCantGo            = 38,
	Pointer_LegoManGo                = 39,
	Pointer_VehicleGo                = 40,
	Pointer_LegoManClear             = 41,
	Pointer_VehicleClear             = 42,
	Pointer_SurfaceType_Immovable    = 43,
	Pointer_SurfaceType_Hard         = 44,
	Pointer_SurfaceType_Medium       = 45,
	Pointer_SurfaceType_Loose        = 46,
	Pointer_SurfaceType_Soil         = 47,
	Pointer_SurfaceType_Lava         = 48,
	Pointer_SurfaceType_Water        = 49,
	Pointer_SurfaceType_OreSeam      = 50,
	Pointer_SurfaceType_Lake         = 51,
	Pointer_SurfaceType_CrystalSeam  = 52,
	Pointer_SurfaceType_RechargeSeam = 53,
	Pointer_CantZoom                 = 54,
	Pointer_Zoom                     = 55,

	Pointer_Type_Count,
};
assert_sizeof(Pointer_Type, 0x4);


enum Activity_Type : sint32
{
	Activity_Stand               = 0,
	Activity_Route               = 1,
	Activity_RouteRubble         = 2,
	Activity_RunPanic            = 3,
	Activity_Drill               = 4,
	Activity_Teleport            = 5,
	Activity_Walk                = 6,
	Activity_Reinforce           = 7,
	Activity_Reverse             = 8,
	Activity_TurnLeft            = 9,
	Activity_TurnRight           = 10,
	Activity_CantDo              = 11,
	Activity_Emerge              = 12,
	Activity_Enter               = 13,
	Activity_EnterRein           = 14,
	Activity_Collect             = 15,
	Activity_Gather              = 16,
	Activity_Carry               = 17,
	Activity_CarryRubble         = 18,
	Activity_Throw               = 19,
	Activity_CarryTurnLeft       = 20,
	Activity_CarryTurnRight      = 21,
	Activity_CarryStand          = 22,
	Activity_HitLeft             = 23,
	Activity_HitRight            = 24,
	Activity_HitFront            = 25,
	Activity_HitBack             = 26,
	Activity_HitHard             = 27,
	Activity_Dynamite            = 28,
	Activity_Deposit             = 29,
	Activity_Clear               = 30,
	Activity_Place               = 31,
	Activity_Repair              = 32,
	Activity_Slip                = 33,
	Activity_Rest                = 34,
	Activity_Eat                 = 35,
	Activity_Stamp               = 36,
	Activity_ThrowMan            = 37,
	Activity_ThrownByRockMonster = 38,
	Activity_GetUp               = 39,
	Activity_BuildPath           = 40,
	Activity_Upgrade             = 41,
	Activity_Explode             = 42,
	Activity_Unpowered           = 43,
	Activity_FireLaser           = 44,
	Activity_Freezed             = 45,
	Activity_FreezeStart         = 46,
	Activity_FreezeMelt          = 47,
	Activity_Recharge            = 48,
	Activity_WakeUp              = 49,
	Activity_Train               = 50,
	Activity_FloatOn             = 51,
	Activity_FloatOff            = 52,
	Activity_Opening             = 53,
	Activity_Closing             = 54,
	Activity_Open                = 55,
	Activity_Waiting1            = 56,
	Activity_Waiting2            = 57,
	Activity_Waiting3            = 58,
	Activity_Waiting4            = 59,
	Activity_Waiting5            = 60,
	Activity_Waiting6            = 61,
	Activity_Waiting7            = 62,
	Activity_Waiting8            = 63,
	Activity_Waiting9            = 64,
	Activity_Waiting10           = 65,
	Activity_Waiting11           = 66,
	Activity_Waiting12           = 67,
	Activity_Waiting13           = 68,
	Activity_Waiting14           = 69,
	Activity_Waiting15           = 70,
	Activity_Waiting16           = 71,
	Activity_Waiting17           = 72,
	Activity_Waiting18           = 73,
	Activity_Waiting19           = 74,
	Activity_Waiting20           = 75,
	Activity_Waiting21           = 76,
	Activity_Crumble             = 77,
	Activity_TeleportIn          = 78,

	Activity_Type_Count,
};
assert_sizeof(Activity_Type, 0x4);


enum Lego_SurfaceType : sint32
{
	Lego_SurfaceType_Tunnel         = 0,
	Lego_SurfaceType_Immovable      = 1,
	Lego_SurfaceType_Hard           = 2,
	Lego_SurfaceType_Medium         = 3,
	Lego_SurfaceType_Loose          = 4,
	Lego_SurfaceType_Soil           = 5,
	Lego_SurfaceType_Lava           = 6,
	Lego_SurfaceType_Water          = 7,
	Lego_SurfaceType_OreSeam        = 8,
	Lego_SurfaceType_Lake           = 9,
	Lego_SurfaceType_CrystalSeam    = 10,
	Lego_SurfaceType_RechargeSeam   = 11,
	Lego_SurfaceType_Rubble         = 12,
	Lego_SurfaceType_Reinforcement  = 13,
	Lego_SurfaceType_Path           = 14,
	Lego_SurfaceType_SlugHole       = 15,
	Lego_SurfaceType_Undiscovered   = 16,
	Lego_SurfaceType_Cavern         = 17,

	Lego_SurfaceType_Count,
};
assert_sizeof(Lego_SurfaceType, 0x4);


enum LegoObject_ToolType : sint32
{
	LegoObject_ToolType_Drill       = 0,
	LegoObject_ToolType_Spade       = 1,
	LegoObject_ToolType_Hammer      = 2,
	LegoObject_ToolType_Spanner     = 3,
	LegoObject_ToolType_Laser       = 4,
	LegoObject_ToolType_PusherGun   = 5,
	LegoObject_ToolType_BirdScarer  = 6,
	LegoObject_ToolType_FreezerGun  = 7,
	
	// Special tool types that are carried but not equipped
	LegoObject_ToolType_Barrier     = 8,
	LegoObject_ToolType_Dynamite    = 9,
	LegoObject_ToolType_CryOre      = 10, // PowerCrystals and Ore

	LegoObject_ToolType_Count, // = 11,
};
assert_sizeof(LegoObject_ToolType, 0x4);


enum Weapon_KnownType : uint32
{
	Weapon_KnownType_None    = 0,
	Weapon_KnownType_Laser   = 1,
	Weapon_KnownType_Pusher  = 2,
	Weapon_KnownType_Freezer = 3,
	Weapon_KnownType_Boulder = 4, // (unused)
};
assert_sizeof(Weapon_KnownType, 0x4);


enum LegoObject_AbilityType : sint32
{
	LegoObject_AbilityType_Pilot    = 0,
	LegoObject_AbilityType_Sailor   = 1,
	LegoObject_AbilityType_Driver   = 2,
	LegoObject_AbilityType_Dynamite = 3,
	LegoObject_AbilityType_Repair   = 4,
	LegoObject_AbilityType_Scanner  = 5,

	LegoObject_AbilityType_Count,
};
assert_sizeof(LegoObject_AbilityType, 0x4);


enum LegoObject_AbilityFlags : uint32 // [LegoRR/LegoObject.c|flags:0x4|type:uint] These flags contain LiveObject abilities, and are the one of the fields stored/restored with ObjectRecall.
{
	// Alternate names, when not used for LiveObjects
	ABILITY_FLAG_NONE     = 0,
	ABILITY_FLAG_PILOT    = (1 << LegoObject_AbilityType_Pilot),    // 0x1,
	ABILITY_FLAG_SAILOR   = (1 << LegoObject_AbilityType_Sailor),   // 0x2,
	ABILITY_FLAG_DRIVER   = (1 << LegoObject_AbilityType_Driver),   // 0x4,
	ABILITY_FLAG_DYNAMITE = (1 << LegoObject_AbilityType_Dynamite), // 0x8,
	ABILITY_FLAG_REPAIR   = (1 << LegoObject_AbilityType_Repair),   // 0x10,
	ABILITY_FLAG_SCANNER  = (1 << LegoObject_AbilityType_Scanner),  // 0x20,

	ABILITY_FLAGS_ALL     = ((1 << LegoObject_AbilityType_Count) - 1), // 0x3f,
};
flags_end(LegoObject_AbilityFlags, 0x4);

#define LegoObject_AbilityToFlag(abilityType) ((LegoObject_AbilityFlags)(1 << (abilityType)))

#define LegoObject_AbilityFlagsHasType(abilityFlags, abilityType) ((abilityFlags) & LegoObject_AbilityToFlag((abilityType)))


enum LegoObject_Type : sint32 // [LegoRR/LegoObject.c|enum:0x4|type:int]
{
	LegoObject_TVCamera          = -1, // special case for object list

	LegoObject_None				 = 0,

	LegoObject_Vehicle           = 1,
	LegoObject_MiniFigure        = 2,
	LegoObject_RockMonster       = 3,
	LegoObject_Building          = 4,
	LegoObject_Boulder           = 5,
	LegoObject_PowerCrystal      = 6,
	LegoObject_Ore               = 7,
	LegoObject_Dynamite          = 8,
	LegoObject_Barrier           = 9,
	LegoObject_UpgradePart       = 10,
	LegoObject_ElectricFence     = 11,
	LegoObject_SpiderWeb         = 12,
	LegoObject_OohScary          = 13,
	LegoObject_ElectricFenceStud = 14,
	LegoObject_Path              = 15,
	LegoObject_Pusher            = 16,
	LegoObject_Freezer           = 17,
	LegoObject_IceCube           = 18,
	LegoObject_LaserShot         = 19,

	LegoObject_Type_Count,
};
assert_sizeof(LegoObject_Type, 0x4);


// Note -1 is applied to all type flags because enum value 0 is LegoObject_None, and we need to skip it.
enum LegoObject_TypeFlags : uint32 // [LegoRR/LegoObject.c|flags:0x4|type:uint] Flags for specifying all (non-TVCamera) object types as a bitfield.
{
	OBJECT_TYPE_FLAG_NONE              = 0,
	OBJECT_TYPE_FLAG_VEHICLE           = (1 << (LegoObject_Vehicle-1)),           // 0x1,
	OBJECT_TYPE_FLAG_MINIFIGURE        = (1 << (LegoObject_MiniFigure-1)),        // 0x2,
	OBJECT_TYPE_FLAG_ROCKMONSTER       = (1 << (LegoObject_RockMonster-1)),       // 0x4,
	OBJECT_TYPE_FLAG_BUILDING          = (1 << (LegoObject_Building-1)),          // 0x8,
	OBJECT_TYPE_FLAG_BOULDER           = (1 << (LegoObject_Boulder-1)),           // 0x10,
	OBJECT_TYPE_FLAG_POWERCRYSTAL      = (1 << (LegoObject_PowerCrystal-1)),      // 0x20,
	OBJECT_TYPE_FLAG_ORE               = (1 << (LegoObject_Ore-1)),               // 0x40,
	OBJECT_TYPE_FLAG_DYNAMITE          = (1 << (LegoObject_Dynamite-1)),          // 0x80,
	OBJECT_TYPE_FLAG_BARRIER           = (1 << (LegoObject_Barrier-1)),           // 0x100,
	OBJECT_TYPE_FLAG_UPGRADEPART       = (1 << (LegoObject_UpgradePart-1)),       // 0x200,
	OBJECT_TYPE_FLAG_ELECTRICFENCE     = (1 << (LegoObject_ElectricFence-1)),     // 0x400,
	OBJECT_TYPE_FLAG_SPIDERWEB         = (1 << (LegoObject_SpiderWeb-1)),         // 0x800,
	OBJECT_TYPE_FLAG_OOHSCARY          = (1 << (LegoObject_OohScary-1)),          // 0x1000,
	OBJECT_TYPE_FLAG_ELECTRICFENCESTUD = (1 << (LegoObject_ElectricFenceStud-1)), // 0x2000,
	OBJECT_TYPE_FLAG_PATH              = (1 << (LegoObject_Path-1)),              // 0x4000,
	OBJECT_TYPE_FLAG_PUSHER            = (1 << (LegoObject_Pusher-1)),            // 0x8000,
	OBJECT_TYPE_FLAG_FREEZER           = (1 << (LegoObject_Freezer-1)),           // 0x10000,
	OBJECT_TYPE_FLAG_ICECUBE           = (1 << (LegoObject_IceCube-1)),           // 0x20000,
	OBJECT_TYPE_FLAG_LASERSHOT         = (1 << (LegoObject_LaserShot-1)),         // 0x40000,

	// Flags for which object types are serviced by the teleporter.
	OBJECT_TYPE_FLAGS_TELEPORTED       = (OBJECT_TYPE_FLAG_VEHICLE|OBJECT_TYPE_FLAG_MINIFIGURE|OBJECT_TYPE_FLAG_BUILDING|OBJECT_TYPE_FLAG_ELECTRICFENCE),

	OBJECT_TYPE_FLAGS_ALL              = ((1 << (LegoObject_AbilityType_Count-1)) - 1), // 0x7ffff,
};
flags_end(LegoObject_TypeFlags, 0x4);

#define LegoObject_TypeToFlag(objType) ((LegoObject_TypeFlags)(1 << ((objType)-1)))

#define LegoObject_TypeFlagsHasType(objTypeFlags, objType) ((objTypeFlags) & LegoObject_TypeToFlag((objType)))


// Hardcoded so many places in the game
enum LegoObject_ID : sint32
{
	LegoObject_ID_Ore          = 0,
	LegoObject_ID_ProcessedOre = 1,

	LegoObject_ID_Ore_Count,

	LegoObject_ID_Pilot = 0,

	LegoObject_ID_Count = 15,

	// Used to signify that a level has no designated slug, may be used elsewhere.
	LegoObject_ID_Invalid = 20,
};
assert_sizeof(LegoObject_ID, 0x4);


enum SurfaceTexture : uint8 // [LegoRR/Lego.c|enum:0x1|type:byte|tags:MAPFILE,COORDNIBBLE]
{
	TEXTURE_GROUND              = 0,
	TEXTURE_WALL_F_SOIL         = 1,
	TEXTURE_WALL_F_LOOSE        = 2,
	TEXTURE_WALL_F_MEDIUM       = 3,
	TEXTURE_WALL_F_HARD         = 4,
	TEXTURE_WALL_F_IMMOVABLE    = 5,
	TEXTURE_ERODE_LOW           = 6,
	TEXTURE_07                  = 7,
	TEXTURE_RUBBLE_FULL         = 16,
	TEXTURE_RUBBLE_HIGH         = 17,
	TEXTURE_RUBBLE_MEDIUM       = 18,
	TEXTURE_RUBBLE_LOW          = 19,
	TEXTURE_ERODE_MEDIUM        = 22,
	TEXTURE_WALL_F_CRYSTALSEAM  = 32,
	TEXTURE_WALL_R_SOIL         = 33,
	TEXTURE_WALL_R_LOOSE        = 34,
	TEXTURE_WALL_R_MEDIUM       = 35,
	TEXTURE_WALL_R_HARD         = 36,
	TEXTURE_WALL_R_IMMOVABLE    = 37,
	TEXTURE_ERODE_HIGH          = 38,
	TEXTURE_SLUGHOLE            = 48,
	TEXTURE_WALL_C_SOIL         = 49,
	TEXTURE_WALL_C_LOOSE        = 50,
	TEXTURE_WALL_C_MEDIUM       = 51,
	TEXTURE_WALL_C_HARD         = 52,
	TEXTURE_WALL_C_IMMOVABLE    = 53,
	TEXTURE_ERODE_FULL          = 54,
	TEXTURE_WALL_F_ORESEAM      = 64,
	TEXTURE_WATER               = 69,
	TEXTURE_LAVA                = 70,
	TEXTURE_LAVA_NOTHOT         = 71,
	TEXTURE_WALL_O_SOIL         = 81,
	TEXTURE_WALL_O_LOOSE        = 82,
	TEXTURE_WALL_O_MEDIUM       = 83,
	TEXTURE_WALL_O_HARD         = 84,
	TEXTURE_WALL_O_IMMOVABLE    = 85,
	TEXTURE_PATH_4              = 96,
	TEXTURE_PATH_BUILD          = 97,
	TEXTURE_PATH_2              = 98,
	TEXTURE_PATH_C              = 99,
	TEXTURE_PATH_3              = 100,
	TEXTURE_PATH_1              = 101,
	TEXTURE_FOUNDATION_POWERED  = 102,
	TEXTURE_WALL_F_RECHARGESEAM = 103,
	TEXTURE_TUNNEL              = 112,
	TEXTURE_PATH_4_POWERED      = 113,
	TEXTURE_PATH_2_POWERED      = 114,
	TEXTURE_PATH_C_POWERED      = 115,
	TEXTURE_PATH_3_POWERED      = 116,
	TEXTURE_PATH_1_POWERED      = 117,
	TEXTURE_FOUNDATION          = 118,
	TEXTURE_WALL_GAP            = 119,

	TEXTURE__INVALID            = 255,
};
assert_sizeof(SurfaceTexture, 0x1);


enum Lego_PathType : uint8 // [LegoRR/Lego.c|enum:0x1|type:byte|tags:MAPFILE]
{
	Lego_PathType_None   = 0,

	Lego_PathType_Rubble = 1,
	Lego_PathType_Path   = 2,
};
assert_sizeof(Lego_PathType, 0x1);


enum Lego_PredugType : uint8 // [LegoRR/Lego.c|enum:0x1|type:byte|tags:MAPFILE]
{
	Lego_PredugType_Wall             = 0,

	Lego_PredugType_Cavern_Exposed   = 1,
	Lego_PredugType_Cavern_Hidden    = 2,
	Lego_PredugType_SlugHole_Exposed = 3,
	Lego_PredugType_SlugHole_Hidden  = 4,
};
assert_sizeof(Lego_PredugType, 0x1);


enum Lego_ErodeType : uint8 // [LegoRR/Lego.c|enum:0x1|type:byte|tags:MAPFILE]
{
	Lego_ErodeType_None              = 0,

	Lego_ErodeType_Adjacent_VerySlow = 1,
	Lego_ErodeType_Source_VerySlow   = 2,
	Lego_ErodeType_Adjacent_Slow     = 3,
	Lego_ErodeType_Source_Slow       = 4,
	Lego_ErodeType_Adjacent_Medium   = 5,
	Lego_ErodeType_Source_Medium     = 6,
	Lego_ErodeType_Adjacent_Fast     = 7,
	Lego_ErodeType_Source_Fast       = 8,
	Lego_ErodeType_Adjacent_VeryFast = 9,
	Lego_ErodeType_Source_VeryFast   = 10,
};
assert_sizeof(Lego_ErodeType, 0x1);


enum Lego_FallInType : uint8 // [LegoRR/Lego.c|enum:0x1|type:byte|tags:MAPFILE]
{
	Lego_FallInType_None            = 0,
	Lego_FallInType_Normal_Low      = 1,
	Lego_FallInType_Normal_Medium   = 2,
	Lego_FallInType_Normal_High     = 3,
	Lego_FallInType_Normal_VeryHigh = 4,
	Lego_FallInType_Danger_Low      = 5,
	Lego_FallInType_Danger_Medium   = 6,
	Lego_FallInType_Danger_High     = 7,
	Lego_FallInType_Danger_VeryHigh = 8,
};
assert_sizeof(Lego_FallInType, 0x1);


enum Lego_CryOreType : uint8 // [LegoRR/Lego.c|enum:0x1|type:byte|tags:MAPFILE]
{
	Lego_CryOreType_None           = 0,

	Lego_CryOreType_Crystal_Lv0_1  = 1,
	Lego_CryOreType_Ore_Lv0_1      = 2,
	Lego_CryOreType_Crystal_Lv1_1  = 3,
	Lego_CryOreType_Ore_Lv1_1      = 4,

	Lego_CryOreType_Crystal_Lv0_3  = 5,
	Lego_CryOreType_Ore_Lv0_3      = 6,
	Lego_CryOreType_Crystal_Lv1_3  = 7,
	Lego_CryOreType_Ore_Lv1_3      = 8,

	Lego_CryOreType_Crystal_Lv0_5  = 9,
	Lego_CryOreType_Ore_Lv0_5      = 10,
	Lego_CryOreType_Crystal_Lv1_5  = 11,
	Lego_CryOreType_Ore_Lv1_5      = 12,

	Lego_CryOreType_Crystal_Lv0_10 = 13,
	Lego_CryOreType_Ore_Lv0_10     = 14,
	Lego_CryOreType_Crystal_Lv1_10 = 15,
	Lego_CryOreType_Ore_Lv1_10     = 16,

	Lego_CryOreType_Crystal_Lv0_25 = 17,
	Lego_CryOreType_Ore_Lv0_25     = 18,
	Lego_CryOreType_Crystal_Lv1_25 = 19,
	Lego_CryOreType_Ore_Lv1_25     = 20,
};
assert_sizeof(Lego_CryOreType, 0x1);



enum ViewMode : sint32 // [LegoRR/Lego.c|enum:0x4|type:int]
{
	ViewMode_FP  = 0,
	ViewMode_Top = 1,

	ViewMode_Count,
};
assert_sizeof(ViewMode, 0x4);


enum LOD_PolyLevel : sint32
{
	LOD_LowPoly    = 0,
	LOD_MediumPoly = 1,
	LOD_HighPoly   = 2,
	LOD_FPPoly     = 3,

	LOD_PolyLevel_Count,
};
assert_sizeof(LOD_PolyLevel, 0x4);


// Upgrade types for vehicles. MSB [Carry][Scan][Speed][Drill] LSB.
enum LegoObject_UpgradeType : uint32 // [LegoRR/LegoObject.c|enum:0x4|type:uint]
{
	LegoObject_UpgradeType_Drill = 0,
	LegoObject_UpgradeType_Speed = 1,
	LegoObject_UpgradeType_Scan  = 2,
	LegoObject_UpgradeType_Carry = 3,

	LegoObject_UpgradeType_Count,
};
assert_sizeof(LegoObject_UpgradeType, 0x4);


// Upgrade flags for vehicles. MSB [Carry][Scan][Speed][Drill] LSB.
// Compared as a mask against the vehicle level.
enum LegoObject_UpgradeFlags : uint32 // [LegoRR/LegoObject.c|flags:0x4|type:uint]
{
	UPGRADE_FLAG_NONE  = 0,
	UPGRADE_FLAG_DRILL = (1 << LegoObject_UpgradeType_Drill), // 0x1,
	UPGRADE_FLAG_SPEED = (1 << LegoObject_UpgradeType_Speed), // 0x2,
	UPGRADE_FLAG_SCAN  = (1 << LegoObject_UpgradeType_Scan),  // 0x4,
	UPGRADE_FLAG_CARRY = (1 << LegoObject_UpgradeType_Carry), // 0x8,

	UPGRADE_FLAGS_ALL  = ((1 << LegoObject_UpgradeType_Count) - 1), // 0xf,
};
flags_end(LegoObject_UpgradeFlags, 0x4);

#define LegoObject_UpgradeToFlag(upgradeType) ((LegoObject_UpgradeFlags)(1 << (upgradeType)))

#define LegoObject_UpgradeFlagsHasType(upgradeFlags, upgradeType) ((upgradeFlags) & LegoObject_UpgradeToFlag((upgradeType)))


enum WallHighlightType : sint32 // [LegoRR/Lego.c|enum:0x4|type:int|tags:BIGENUMALIAS] (white, gray, red, green, blue, dark-yellow)
{
	WALLHIGHLIGHT_NONE      = 0,
	WALLHIGHLIGHT_DIG       = 1,
	WALLHIGHLIGHT_DYNAMITE  = 2,
	WALLHIGHLIGHT_REINFORCE = 3,
	WALLHIGHLIGHT_SELECTED  = 4,
	WALLHIGHLIGHT_TUTORIAL  = 5,
};
assert_sizeof(WallHighlightType, 0x4);


enum SFX_ID : sint32 // [LegoRR/SFX.c|enum:0x4|type:int] SFX_ID is different from actual Sample indexes, these are hardcoded values that can easily be looked up by ID
{
    SFX_ID_Invalid              = -1,

    SFX_NULL                    = 0,
    SFX_Stamp                   = 1,
    SFX_Drill                   = 2,
    SFX_DrillFade               = 3,
    SFX_RockBreak               = 4,
    SFX_Drip                    = 5,
    SFX_Ambient                 = 6,
    SFX_AmbientLoop             = 7,
    SFX_Step                    = 8,
    SFX_RockMonster             = 9,
    SFX_RockMonster2            = 10,
    SFX_RockMonsterStep         = 11,
    SFX_MFDeposit               = 12,
    SFX_ButtonPressed           = 13,
    SFX_MFLift                  = 14,
    SFX_MFThrow                 = 15,
    SFX_Walker                  = 16,
    SFX_YesSir                  = 17,
    SFX_Build                   = 18,
    SFX_Okay                    = 19,
    SFX_NotOkay                 = 20,
    SFX_InterfaceSlideOnScreen  = 21,
    SFX_InterfaceSlideOffScreen = 22,
    SFX_PanelSlideOnScreen      = 23,
    SFX_PanelSlideOffScreen     = 24,
    SFX_Siren                   = 25,
    SFX_CrystalRecharge         = 26,
    SFX_Laser                   = 27,
    SFX_LaserHit                = 28,
    SFX_LazerRecharge           = 29,
    SFX_TopPriority             = 30,
    SFX_ImmovableRock           = 31,
    SFX_Wall                    = 32,
    SFX_Floor                   = 33,
    SFX_BoulderHit              = 34,
    SFX_Place                   = 35,
    SFX_PlaceOre                = 36,
    SFX_PlaceCrystal            = 37,
    SFX_Lava                    = 38,
    SFX_RockWipe                = 39,
    SFX_FallIn                  = 40,
    SFX_MusicLoop               = 41,
    SFX_CaptainSlide            = 42,
    SFX_Dynamite                = 43,
    SFX_AmbientMusicLoop        = 44,

    // Number of sound effects preloaded by SFX_Initialise.
    // This number is must be added to sfxGlobs.hashNameCount to get the total number of sfx.
    SFX_Preload_Count,
};
assert_sizeof(SFX_ID, 0x4);


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
 ******** Globals
 **********************************************************************************/

#pragma region Globals

#pragma endregion

}
