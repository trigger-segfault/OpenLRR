// Object.h : 
//

#pragma once

#include "../../common.h"
#include "../../engine/colour.h"
#include "../../engine/geometry.h"
#include "../../engine/undefined.h"

#include "../../engine/core/ListSet.hpp"
#include "../../engine/gfx/Containers.h"
#include "../../engine/drawing/Images.h"

#include "../GameCommon.h"

#include "../audio/SFX.h"

// Temporarily include until a better location is found for SaveStruct_18
#include "../front/FrontEnd.h"

#include "BezierCurve.h"


namespace LegoRR
{; // !<---

/**********************************************************************************
 ******** Forward Declarations
 **********************************************************************************/

#pragma region Forward Declarations

/*struct CreatureData;
struct BuildingData;
struct VehicleData;
struct UpgradeData;
struct Flocks;
struct HiddenObject;
struct ObjectStats;
struct RoutingBlock;
struct TeleporterService;
enum LOD_PolyLevel : sint32;*/

struct Flocks;
struct FlocksItem;

#pragma endregion

/**********************************************************************************
 ******** Constants
 **********************************************************************************/

#pragma region Constants

#define OBJECT_MAXLISTS				32			// 2^32 - 1 possible tasks...
#define OBJECT_MAXCYCLEUNITS		256
#define OBJECT_MAXHIDDEN			200
#define OBJECT_MAXRECHARGESEAMS		10
#define OBJECT_MAXSLUGHOLES			20

#define OBJECT_DRILL_COEF			0.05f
#define OBJECT_CARRY_COEF			0.5f
//#define OBJECT_LIFT_COEF			OBJECT_CARRY_COEF
#define OBJECT_WEB_COEF				0.05f
// For minifigures only
#define OBJECT_LAKE_COEF			0.8f

#pragma endregion

/**********************************************************************************
 ******** Function typedefs
 **********************************************************************************/

#pragma region Function typedefs

struct LegoObject; // Predeclaration

typedef bool32 (__cdecl* LegoObject_RunThroughListsCallback)(LegoObject* liveObj, void* data);

#pragma endregion

/**********************************************************************************
 ******** Enumerations
 **********************************************************************************/

#pragma region Enums

enum LiveFlags1 : uint32 // [LegoRR/LegoObject.c|flags:0x4|type:uint]
{
	LIVEOBJ1_NONE         = 0,
	LIVEOBJ1_MOVING       = 0x1,
	LIVEOBJ1_LIFTING      = 0x2,
	LIVEOBJ1_TURNING      = 0x4,
	LIVEOBJ1_DRILLING     = 0x8,
	LIVEOBJ1_DRILLINGSTART= 0x10, // Set initially with `LIVEOBJ1_DRILLING` when drilling starts. Then unset after sound is played?
	LIVEOBJ1_REINFORCING  = 0x40,
	LIVEOBJ1_TURNRIGHT    = 0x80,
	LIVEOBJ1_UNK_100      = 0x100,
	LIVEOBJ1_UNK_200      = 0x200,
	LIVEOBJ1_CARRYING     = 0x400,
	LIVEOBJ1_UNK_800      = 0x800,
	LIVEOBJ1_UNK_1000     = 0x1000,
	LIVEOBJ1_UNK_2000     = 0x2000,
	LIVEOBJ1_UNK_4000     = 0x4000,
	LIVEOBJ1_UNK_8000     = 0x8000,
	LIVEOBJ1_UNK_10000    = 0x10000,
	LIVEOBJ1_UNK_20000    = 0x20000,
	LIVEOBJ1_CLEARING     = 0x40000,
	LIVEOBJ1_PLACING      = 0x80000,
	LIVEOBJ1_UNK_100000   = 0x100000,
	LIVEOBJ1_UNK_200000   = 0x200000,
	LIVEOBJ1_UNK_400000   = 0x400000,
	LIVEOBJ1_UNK_800000   = 0x800000,
	LIVEOBJ1_UNK_1000000  = 0x1000000,
	LIVEOBJ1_CAUGHTINWEB  = 0x2000000,
	LIVEOBJ1_SLIPPING     = 0x4000000,
	LIVEOBJ1_UNK_10000000 = 0x10000000,
	LIVEOBJ1_REST         = 0x20000000,
	LIVEOBJ1_EATING       = 0x40000000,
	LIVEOBJ1_UNK_80000000 = 0x80000000,
};
flags_end(LiveFlags1, 0x4);


enum LiveFlags2 : uint32 // [LegoRR/LegoObject.c|flags:0x4|type:uint]
{
	LIVEOBJ2_NONE         = 0,
	LIVEOBJ2_THROWING     = 0x1,
	LIVEOBJ2_THROWN       = 0x2,
	LIVEOBJ2_UNK_4        = 0x4,
	LIVEOBJ2_DRIVING      = 0x8,
	LIVEOBJ2_UNK_10       = 0x10,
	LIVEOBJ2_UNK_20       = 0x20,
	LIVEOBJ2_UNK_40       = 0x40,
	LIVEOBJ2_UNK_100      = 0x100,
	LIVEOBJ2_UNK_200      = 0x200,
	LIVEOBJ2_TRAINING     = 0x400,
	LIVEOBJ2_UNK_800      = 0x800,
	LIVEOBJ2_UNK_1000     = 0x1000,
	LIVEOBJ2_UNK_2000     = 0x2000,
	LIVEOBJ2_UNK_4000     = 0x4000,
	LIVEOBJ2_UPGRADING    = 0x8000,
	LIVEOBJ2_TRIGGERFRAMECALLBACK = 0x10000,
	LIVEOBJ2_UNK_20000    = 0x20000,
	LIVEOBJ2_UNK_40000    = 0x40000,
	LIVEOBJ2_UNK_80000    = 0x80000,
	LIVEOBJ2_UNK_100000   = 0x100000,
	LIVEOBJ2_UNK_200000   = 0x200000,
	LIVEOBJ2_UNK_400000   = 0x400000,
	LIVEOBJ2_UNK_800000   = 0x800000,
	LIVEOBJ2_UNK_1000000  = 0x1000000,
	LIVEOBJ2_UNK_2000000  = 0x2000000,
	LIVEOBJ2_UNK_4000000  = 0x4000000,
	LIVEOBJ2_UNK_8000000  = 0x8000000,
	LIVEOBJ2_UNK_10000000 = 0x10000000,
	LIVEOBJ2_UNK_20000000 = 0x20000000,
	LIVEOBJ2_UNK_40000000 = 0x40000000,
	LIVEOBJ2_UNK_80000000 = 0x80000000,
};
flags_end(LiveFlags2, 0x4);


enum LiveFlags3 : uint32 // [LegoRR/LegoObject.c|flags:0x4|type:uint]
{
	LIVEOBJ3_NONE           = 0,
	LIVEOBJ3_UNK_1          = 0x1,
	LIVEOBJ3_UNK_2          = 0x2,
	LIVEOBJ3_UNK_4          = 0x4,
	LIVEOBJ3_UNK_8          = 0x8,
	LIVEOBJ3_UNK_10         = 0x10,
	LIVEOBJ3_UNK_20         = 0x20,
	LIVEOBJ3_UNK_40         = 0x40,
	LIVEOBJ3_UNK_80         = 0x80,
	LIVEOBJ3_CANSELECT      = 0x100,
	LIVEOBJ3_UNK_200        = 0x200,
	LIVEOBJ3_UNK_400        = 0x400,
	LIVEOBJ3_UNK_1000       = 0x1000,
	LIVEOBJ3_UNK_2000       = 0x2000,
	LIVEOBJ3_UNK_4000       = 0x4000,
	LIVEOBJ3_UNK_8000       = 0x8000,
	LIVEOBJ3_UNK_10000      = 0x10000,
	LIVEOBJ3_SIMPLEOBJECT   = 0x20000,
	LIVEOBJ3_UNK_40000      = 0x40000,
	LIVEOBJ3_IGNOREME_UNK   = 0x80000,
	LIVEOBJ3_UNK_100000     = 0x100000,
	LIVEOBJ3_UNK_200000     = 0x200000,
	LIVEOBJ3_UNK_400000     = 0x400000,
	LIVEOBJ3_UNK_800000     = 0x800000,
	LIVEOBJ3_UNK_1000000    = 0x1000000,
	LIVEOBJ3_UNK_2000000    = 0x2000000,
	LIVEOBJ3_CANGATHER      = 0x4000000,
	LIVEOBJ3_UNK_8000000    = 0x8000000,
	LIVEOBJ3_CANROUTERUBBLE = 0x10000000,
	LIVEOBJ3_HASPOWER       = 0x20000000,
	LIVEOBJ3_UNK_40000000   = 0x40000000,
	LIVEOBJ3_POWEROFF       = 0x80000000,
};
flags_end(LiveFlags3, 0x4);


enum LiveFlags4 : uint32 // [LegoRR/LegoObject.c|flags:0x4|type:uint]
{
	LIVEOBJ4_NONE          = 0,
	LIVEOBJ4_UNK_1         = 0x1,
	LIVEOBJ4_UNK_2         = 0x2,
	LIVEOBJ4_UNK_8         = 0x8,
	LIVEOBJ4_UNK_10        = 0x10,
	LIVEOBJ4_CALLTOARMS_20 = 0x20,
	LIVEOBJ4_UNK_40        = 0x40,
	LIVEOBJ4_UNK_80        = 0x80,
	LIVEOBJ4_UNK_200       = 0x200,
	LIVEOBJ4_UNK_800       = 0x800,
	LIVEOBJ4_UNK_1000      = 0x1000,
	LIVEOBJ4_UNK_2000      = 0x2000,
	LIVEOBJ4_UNK_4000      = 0x4000,
	LIVEOBJ4_UNK_8000      = 0x8000,
	LIVEOBJ4_UNK_10000     = 0x10000,
	LIVEOBJ4_UNK_20000     = 0x20000,
	LIVEOBJ4_UNK_40000     = 0x40000,
	LIVEOBJ4_UNK_80000     = 0x80000,
	LIVEOBJ4_UNK_100000    = 0x100000,
	LIVEOBJ4_UNK_200000    = 0x200000,
};
flags_end(LiveFlags4, 0x4);


enum LiveFlags5 : uint32 // [LegoRR/LegoObject.c|flags:0x4|type:uint] These flags contain LegoObject abilities, and are the one of the fields stored/restored with ObjectRecall.
{
	/*LIVEOBJ5_NONE             = 0,
	LIVEOBJ5_ABILITY_PILOT    = 0x1,
	LIVEOBJ5_ABILITY_SAILOR   = 0x2,
	LIVEOBJ5_ABILITY_DRIVER   = 0x4,
	LIVEOBJ5_ABILITY_DYNAMITE = 0x8,
	LIVEOBJ5_ABILITY_REPAIR   = 0x10,
	LIVEOBJ5_ABILITY_SCANNER  = 0x20,*/

	LIVEOBJ5_NONE             = 0,
	LIVEOBJ5_ABILITY_PILOT    = LegoObject_AbilityFlags::ABILITY_FLAG_PILOT,    // 0x1,
	LIVEOBJ5_ABILITY_SAILOR   = LegoObject_AbilityFlags::ABILITY_FLAG_SAILOR,   // 0x2,
	LIVEOBJ5_ABILITY_DRIVER   = LegoObject_AbilityFlags::ABILITY_FLAG_DRIVER,   // 0x4,
	LIVEOBJ5_ABILITY_DYNAMITE = LegoObject_AbilityFlags::ABILITY_FLAG_DYNAMITE, // 0x8,
	LIVEOBJ5_ABILITY_REPAIR   = LegoObject_AbilityFlags::ABILITY_FLAG_REPAIR,   // 0x10,
	LIVEOBJ5_ABILITY_SCANNER  = LegoObject_AbilityFlags::ABILITY_FLAG_SCANNER,  // 0x20,

	LIVEOBJ5_ABILITIES_ALL    = LegoObject_AbilityFlags::ABILITY_FLAGS_ALL,     // 0x20,
};
flags_end(LiveFlags5, 0x4);


flags_scoped(LegoObject_GlobFlags) : uint32 // [LegoRR/LegoObject.c|flags:0x4|type:uint] LegoObject_GlobFlags, ReservedPool LegoObject INITFLAGS
{
	OBJECT_GLOB_FLAG_NONE        = 0,
	OBJECT_GLOB_FLAG_INITIALISED = 0x1,
	OBJECT_GLOB_FLAG_REMOVING    = 0x2,  // set/unset by LegoObject_RemoveAll during LegoObject_Callback_Remove
	OBJECT_GLOB_FLAG_UNK_4       = 0x4,  // set by LiveManager_InitFlagsToggle_AndClearNumDrained
	                                     // unset by LegoObject_UpdateAll
	OBJECT_GLOB_FLAG_UNK_8       = 0x8,  // set by LiveManager_InitFlagsToggle_AndClearNumDrained
	                                     // unset by LegoObject_UpdateAll
	OBJECT_GLOB_FLAG_UPDATING    = 0x10, // set/unset by LegoObject_UpdateAll during LegoObject_Callback_Update
	OBJECT_GLOB_FLAG_LEVELENDING = 0x20, // set/unset by LegoObject_SetGlobFlag20, which is called by:
	                                     //   set(false) by Lego_LoadLevel
										 //   set(true)  by Lego_SetLoadFlag_StartTeleporter (for objective crystals failed)
										 //   set(true)  by Objective_SetCompleteStatus
	OBJECT_GLOB_FLAG_CYCLEUNITS = 0x40,  // set by LegoObject_CameraCycleUnits
	                                     // unset by Level_Free
};
flags_scoped_end(LegoObject_GlobFlags, 0x4);

#pragma endregion

/**********************************************************************************
 ******** Typedefs
 **********************************************************************************/

#pragma region Typedefs

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

struct RoutingBlock // [LegoRR/Routing.c|struct:0x14]
{
	/*00,8*/	Point2I	blockPos;
	/*08,8*/	Point2F	worldPos;
	/*10,1*/	uint8	flagsByte_10;
	/*11,1*/	uint8	byte_11;
	/*12,2*/	undefined field_0x12_0x13[2];
	/*14*/
};
assert_sizeof(RoutingBlock, 0x14);


struct HiddenObject // [LegoRR/LegoObject.c|struct:0x2c] Name is only guessed
{
	/*00,8*/	Point2I blockPos;		// (ol: xPos, yPos -> blockPos)
	/*08,8*/	Point2F worldPos;		// (ol: xPos, yPos)
	/*10,4*/	real32 heading;			// (ol: heading -> radians)
	/*14,4*/	ObjectModel* model;		// (ol: type [partial])
	/*18,4*/	LegoObject_Type type;	// (ol: type [partial])
	/*1c,4*/	LegoObject_ID id;		// (ol: type [partial])
	/*20,4*/	real32 health;			// (ol: health)
	/*24,4*/	char* olistID;			// (ol: Object%i)
	/*28,4*/	char* olistDrivingID;	// (ol: driving)
	/*2c*/
};
assert_sizeof(HiddenObject, 0x2c);


struct LegoObject // [LegoRR/LegoObject.c|struct:0x40c|tags:LISTSET]
{
	/*000,4*/       LegoObject_Type type;
	/*004,4*/       LegoObject_ID id;
	/*008,4*/       char* customName; // max size is 11 (NOT null-terminated)
	/*00c,4*/       VehicleModel* vehicle;
	/*010,4*/       CreatureModel* miniFigure;
	/*014,4*/       CreatureModel* rockMonster;
	/*018,4*/       BuildingModel* building;
	/*01c,4*/       Gods98::Container* other;
	/*020,4*/       Upgrade_PartModel* upgradePart; // First upgrade part model in linked list of parts.
	/*024,4*/       RoutingBlock* routeBlocks;	// Unknown pointer, likely in large allocated table
	/*028,4*/       uint32 routeBlockTotal;		// Total blocks to travel for current route
	/*02c,4*/       uint32 routeBlockCurrent;	// Number of blocks traveled (up to routeBlockTotal)
	/*030,25c*/		BezierCurve routeCurve;		// BezierCurve/Catmull-rom spline data
	/*28c,c*/       Vector3F vector_28c;
	/*298,8*/       Point2F point_298;
	/*2a0,c*/       Vector3F vector_2a0; // Used with faceDirection calculation.
	/*2ac,c*/       Vector3F faceDirection; // 1.0 to -1.0 directions that determine rotation with atan2
	/*2b8,4*/       real32 faceDirectionLength_2b8; // faceDirection length (faceDirection may be Vector4F...)
	/*2bc,4*/       sint32 strafeSignFP;  // (direction sign only, does higher numbers do not affect speed)
	/*2c0,4*/       sint32 forwardSignFP; // (direction sign only, does higher numbers do not affect speed)
	/*2c4,4*/       real32 rotateSpeedFP;
	/*2c8,4*/       undefined4 field_2c8;
	/*2cc,4*/       undefined4 field_2cc;
	/*2d0,4*/       undefined4 field_2d0;
	/*2d4,4*/       real32 animTime;
	/*2d8,4*/       uint32 animRepeat; // Number of times an activity animation is set to repeat (i.e. number of jumping jacks/reinforce hits). Zero is default.
	/*2dc,4*/       Gods98::Container* cont_2dc;
	/*2e0,4*/       sint32 index_2e0;
	/*2e4,4*/       Gods98::Container* cont_2e4;
	/*2e8,4*/       const char* activityName1;
	/*2ec,4*/       const char* activityName2; // Seems to be used with related objects like driven, swapped with activityName1.
	/*2f0,4*/       AITask* aiTask;
	/*2f4,8*/       Point2F point_2f4; // (init: -1.0f, -1.0f)
	/*2fc,4*/       LegoObject* boulderObject; // other half of boulderHolderObject
	/*300,4*/       LegoObject* boulderHolderObject; // other half of boulderObject (todo: better name)
	/*304,4*/       LegoObject* carryingThisObject;
	/*308,1c*/      LegoObject* carriedObjects[7]; // (includes carried vehicles)
	/*324,4*/       uint32 numCarriedObjects;
	/*328,4*/       uint32 carryNullFrames;
	/*32c,4*/       Flocks* flocks;
	/*330,4*/       uint32 objLevel;
	/*334,4*/       ObjectStats* stats;
	/*338,4*/       real32 float_338;
	/*33c,4*/       real32 float_33c;
	/*340,4*/       real32 health; // (assigned -1.0f)
	/*344,4*/       real32 energy; // (assigned -1.0f)
	/*348,4*/       sint32* stealTableptr_348; // element size is 0x4
	/*34c,4*/       LOD_PolyLevel polyLOD;
	/*350,4*/       sint32 soundHandle_350;
	/*354,4*/       SFX_Type soundHandle_354; // (engine sound only?)
	/*358,4*/       undefined4 field_358;
	/*35c,4*/       undefined4 field_35c;
	/*360,4*/       undefined4 field_360;
	/*364,4*/       LegoObject* object_364;
	/*368,4*/       real32 float_368;
	/*36c,4*/       LegoObject* driveObject; // (bi-directional link between driver and driven)
	/*370,14*/      LegoObject_ToolType carriedTools[5];
	/*384,4*/       uint32 numCarriedTools;
	/*388,4*/       real32 float_388;
	/*38c,4*/       Gods98::Image* bubbleImage;
	/*390,4*/       undefined4 teleporter_field_390;
	/*394,4*/       undefined4 teleporter_field_394;
	/*398,4*/       TeleporterService* teleporter;
	/*39c,4*/       undefined4 field_39c;
	/*3a0,4*/       undefined4 field_3a0;
	/*3a4,4*/       undefined4 field_3a4;
	/*3a8,4*/       undefined4 field_3a8;
	/*3ac,4*/       undefined4 field_3ac;
	/*3b0,4*/       undefined4 field_3b0;
	/*3b4,8*/       Point2F pushingVec2D;
	/*3bc,4*/       real32 pushingDist;
	/*3c0,4*/       LegoObject* throwObject; // (bi-directional link between thrower and thrown)
	/*3c4,4*/       LegoObject* object_3c4;
	/*3c8,4*/       undefined4 field_3c8;
	/*3cc,4*/       LegoObject* object_3cc;
	/*3d0,4*/       undefined4 field_3d0;
	/*3d4,4*/       real32 elapsedTime1; // elapsed time counter 1
	/*3d8,4*/       real32 elapsedTime2; // elapsed time counter 2
	/*3dc,4*/       real32 activityElapsedTime; // elapsed time since last order?
	/*3e0,4*/       LiveFlags1 flags1;
	/*3e4,4*/       LiveFlags2 flags2;
	/*3e8,4*/       LiveFlags3 flags3; // (assigned 0, flags?)
	/*3ec,4*/       LiveFlags4 flags4;
	/*3f0,4*/       LiveFlags5 flags5; // ability flags, and saved in ObjectRecall
	/*3f4,4*/       undefined4 field_3f4;
	/*3f8,4*/       undefined4 field_3f8;
	/*3fc,4*/       real32 floatSnd_3fc;
	/*400,4*/       real32 floatSnd_400;
	/*404,4*/       undefined4 field_404;
	/*408,4*/       LegoObject* nextFree; // (for listSet)
	/*40c*/
};
assert_sizeof(LegoObject, 0x40c);


struct LegoObject_Globs // [LegoRR/LegoObject.c|struct:0xc644|tags:GLOBS]
{
	/*0000,80*/     LegoObject* listSet[OBJECT_MAXLISTS];
	/*0080,4*/      LegoObject* freeList;
	/*0084,4b0*/    SFX_Type objectTtSFX[LegoObject_Type_Count][LegoObject_ID_Count]; // [objType:20][objID:15]
	/*0534,13c*/    const char* activityName[Activity_Type_Count]; // [activityType:79]
	/*0670,4*/      void* UnkSurfaceGrid_1_TABLE;
	/*0674,4*/      void* UnkSurfaceGrid_2_TABLE;
	/*0678,4*/      uint32 UnkSurfaceGrid_COUNT;
	/*067c,4*/      real32 radarElapsed_67c;
	/*0680,4*/      uint32 listCount;
	/*0684,4*/      LegoObject_GlobFlags flags;
	/*0688,2c*/     uint32 toolNullIndex[LegoObject_ToolType_Count]; // [toolType:11]
	/*06b4,4b00*/   uint32 objectTotalLevels[LegoObject_Type_Count][LegoObject_ID_Count][OBJECT_MAXLEVELS]; // [objType:20][objID:15][objLevel:16]
	/*51b4,4b00*/   uint32 objectPrevLevels[LegoObject_Type_Count][LegoObject_ID_Count][OBJECT_MAXLEVELS];  // [objType:20][objID:15][objLevel:16]
	/*9cb4,4*/      uint32 NERPs_TrainFlags;
	/*9cb8,4*/      LegoObject* minifigureObj_9cb8;
	/*9cbc,a0*/     Point2I slugHoleBlocks[OBJECT_MAXSLUGHOLES];
	/*9d5c,50*/     Point2I rechargeSeamBlocks[OBJECT_MAXRECHARGESEAMS];
	/*9dac,4*/      uint32 slugHoleCount;
	/*9db0,4*/      uint32 rechargeSeamCount;
	/*9db4,2260*/   HiddenObject hiddenObjects[OBJECT_MAXHIDDEN];
	/*c014,4*/      uint32 hiddenObjectCount;
	/*c018,4*/      real32 float_c018;
	/*c01c,18*/     /*Dummy::*/SaveStruct_18 savestruct18_c01c;
	/*c034,400*/    LegoObject* cycleUnits[OBJECT_MAXCYCLEUNITS];
	/*c434,4*/      uint32 cycleUnitCount;
	/*c438,4*/      uint32 cycleBuildingCount;
	/*c43c,190*/    LegoObject* liveObjArray100_c43c[100]; // Used for water docking vehicles?
	/*c5cc,4*/      uint32 uintCount_c5cc; // Count for liveObjArray100_c43c
	/*c5d0,18*/     const char* abilityName[LegoObject_AbilityType_Count]; // [abilityType:6]
	/*c5e8,18*/     Gods98::Image* ToolTipIcons_Abilities[LegoObject_AbilityType_Count]; // [abilityType:6]
	/*c600,2c*/     Gods98::Image* ToolTipIcons_Tools_TABLE[LegoObject_ToolType_Count]; // [toolType:11]
	/*c62c,4*/      Gods98::Image* ToolTipIcon_Blank;
	/*c630,4*/      Gods98::Image* ToolTipIcon_Ore;
	/*c634,4*/      uint32 BuildingsTeleported;
	/*c638,4*/      real32 LiveManager_TimerUnk;
	/*c63c,4*/      undefined4 s_stepCounter_c63c; // (static, counter %4 for step SFX)
	/*c640,4*/      void** s_FlocksDestroy_c640; // (static, Struct 0x10, used in Flocks activities (QUICK_DESTROY??))
	/*c644*/
};
assert_sizeof(LegoObject_Globs, 0xc644);

#pragma endregion

/**********************************************************************************
 ******** Classes
 **********************************************************************************/

#pragma region Classes

class LegoObject_ListSet : public ListSet::Collection<LegoObject_Globs>
{
public:
	LegoObject_ListSet(LegoObject_Globs& cont)
		: ListSet::Collection<LegoObject_Globs>(cont)
	{
	}

private:
	static bool FilterSkipIgnoreMes(const LegoObject* liveObj);
	/*{
		return ListSet::IsAlive(liveObj) && !(liveObj->flags3 & LIVEOBJ3_IGNOREME_UNK);
	}*/

public:
	LegoObject_ListSet::enumerable<FilterSkipIgnoreMes> EnumerateSkipIgnoreMes();
	/*{
		return this->EnumerateWhere<FilterSkipIgnoreMes>();
		//return LegoObject_ListSet::enumerable<FilterSkipIgnoreMes>(this->m_cont);
	}*/
};

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @004df790>
extern LegoObject_Globs & objectGlobs;

extern LegoObject_ListSet objectListSet;

#pragma endregion

/**********************************************************************************
 ******** Macros
 **********************************************************************************/

#pragma region Macros

#define Activity_RegisterName(n)		    (objectGlobs.activityName[n]=#n)
#define AbilityType_RegisterName(n)         (objectGlobs.abilityName[n]=#n)

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

/*#if 0
// <LegoRR.exe @00436ee0>
void __cdecl LegoObject_Initialise(void);

// <LegoRR.exe @00437310>
void __cdecl LegoObject_Shutdown(void);

// <LegoRR.exe @0044a330>
void __cdecl LegoObject_FP_GetPositionAndHeading(LegoObject* liveObj, int cameraFrame, OUT Vector3F* position, OPTIONAL OUT Vector3F* dir);
#define LegoObject_FP_GetPositionAndHeading ((decltype(LegoObject_FP_GetPositionAndHeading)*)0x0044a330)

// <LegoRR.exe @0044a4c0>
Gods98::Container* __cdecl LegoObject_GetActivityContainer(LegoObject* liveObj);
#define LegoObject_GetActivityContainer ((decltype(LegoObject_GetActivityContainer)*)0x0044a4c0)
#endif*/

// <LegoRR.exe @00436ee0>
#define LegoObject_Initialise ((void (__cdecl* )(void))0x00436ee0)

// <LegoRR.exe @00437310>
#define LegoObject_Shutdown ((void (__cdecl* )(void))0x00437310)

// <LegoRR.exe @00437370>
#define Object_Save_CopyStruct18 ((void (__cdecl* )(OUT SaveStruct_18* saveStruct18))0x00437370)

// <LegoRR.exe @00437390>
#define Object_Save_OverwriteStruct18 ((void (__cdecl* )(const SaveStruct_18* saveStruct18))0x00437390)

// <LegoRR.exe @004373c0>
#define LegoObject_GetObjectsBuilt ((sint32 (__cdecl* )(LegoObject_Type objType, bool32 excludeToolStore))0x004373c0)

// <LegoRR.exe @00437410>
#define Object_LoadToolTipIcons ((void (__cdecl* )(const Gods98::Config* config))0x00437410)

// <LegoRR.exe @00437560>
#define LegoObject_CleanupLevel ((void (__cdecl* )(void))0x00437560)

// <LegoRR.exe @004375c0>
#define LegoObject_Weapon_FUN_004375c0 ((void (__cdecl* )(LegoObject* liveObj, sint32 weaponType, real32 param_3))0x004375c0)

// <LegoRR.exe @00437690>
#define LegoObject_DoOpeningClosing ((bool32 (__cdecl* )(LegoObject* liveObj, bool32 open))0x00437690)

// <LegoRR.exe @00437700>
#define LegoObject_CleanupObjectLevels ((void (__cdecl* )(void))0x00437700)

// <LegoRR.exe @00437720>
#define LegoObject_GetLevelObjectsBuilt ((uint32 (__cdecl* )(LegoObject_Type objType, LegoObject_ID objID, sint32 objLevel, bool32 currentLevel))0x00437720)

// <LegoRR.exe @00437760>
#define LegoObject_GetPreviousLevelObjectsBuilt ((uint32 (__cdecl* )(LegoObject_Type objType, LegoObject_ID objID, sint32 objLevel))0x00437760)

// <LegoRR.exe @00437790>
#define LegoObject_IncLevelPathsBuilt ((void (__cdecl* )(bool32 incCurrent))0x00437790)

// <LegoRR.exe @004377b0>
#define LegoObject_FUN_004377b0 ((void (__cdecl* )(LegoObject* liveObj))0x004377b0)

// <LegoRR.exe @004377d0>
#define LegoObject_Callback_UnkDoRouting_IfObject2FC ((bool32 (__cdecl* )(LegoObject* liveObj1, LegoObject* liveObj2))0x004377d0)

// <LegoRR.exe @00437800>
#define LegoObject_Remove ((bool32 (__cdecl* )(LegoObject* liveObj))0x00437800)

// <LegoRR.exe @00437a70>
bool32 __cdecl LegoObject_RunThroughListsSkipIgnoreMes(LegoObject_RunThroughListsCallback callback, void* data);
//#define LegoObject_RunThroughListsSkipIgnoreMes ((bool32 (__cdecl* )(LegoObject_RunThroughListsCallback callback, void* data))0x00437a70)

// <LegoRR.exe @00437a90>
bool32 __cdecl LegoObject_RunThroughLists(LegoObject_RunThroughListsCallback callback, void* data, bool32 skipIgnoreMeObjs);
//#define LegoObject_RunThroughLists ((bool32 (__cdecl* )(LegoObject_RunThroughListsCallback callback, void* search, bool32 skipIgnoreMeObjs))0x00437a90)

// NOTE: When implementing this function, buffer overrun needs to be accounted for, an array of 11 chars is allocated
// And index 12 is null-terminated. This only works because of compiler-defined behavior of malloc uses units of 16 at smaller sizes.
// <LegoRR.exe @00437b40>
#define LegoObject_SetCustomName ((void (__cdecl* )(LegoObject* liveObj, OPTIONAL const char* customName))0x00437b40)

// <LegoRR.exe @00437ba0>
#define HiddenObject_RemoveAll ((void (__cdecl* )(void))0x00437ba0)

// <LegoRR.exe @00437c00>
#define HiddenObject_ExposeBlock ((void (__cdecl* )(const Point2I* blockPos))0x00437c00)

// <LegoRR.exe @00437ee0>
#define HiddenObject_Add ((void (__cdecl* )(ObjectModel* objModel, LegoObject_Type objType, LegoObject_ID objID, const Point2F* worldPos, real32 heading, real32 health, OPTIONAL const char* thisOLName, OPTIONAL const char* drivingOLName))0x00437ee0)

// <LegoRR.exe @00437f80>
#define LegoObject_CanShootObject ((bool32 (__cdecl* )(LegoObject* liveObj))0x00437f80)

// <LegoRR.exe @00437fc0>
#define LegoObject_Create ((LegoObject* (__cdecl* )(ObjectModel* objModel, LegoObject_Type objType, LegoObject_ID objID))0x00437fc0)

// <LegoRR.exe @00438580>
#define LegoObject_Create_internal ((LegoObject* (__cdecl* )(void))0x00438580)

// <LegoRR.exe @004385d0>
#define LiveManager_AddList ((void (__cdecl* )(void))0x004385d0)

// <LegoRR.exe @00438650>
#define LiveManager_GetNumBuildingsTeleported ((sint32 (__cdecl* )(sint32* stack))0x00438650)

// <LegoRR.exe @00438660>
#define LiveManager_SetNumBuildingsTeleported ((void (__cdecl* )(uint32 numTeleported))0x00438660)

// <LegoRR.exe @00438670>
#define LegoObject_SetCrystalPoweredColour ((void (__cdecl* )(LegoObject* liveObj, bool32 powered))0x00438670)

// <LegoRR.exe @00438720>
#define LegoObject_FUN_00438720 ((void (__cdecl* )(LegoObject* liveObj))0x00438720)

// <LegoRR.exe @00438840>
#define LegoObject_SetPowerOn ((void (__cdecl* )(LegoObject* liveObj, bool32 state))0x00438840)

// <LegoRR.exe @00438870>
#define LegoObject_CheckCondition_AndIsPowered ((bool32 (__cdecl* )(LegoObject* liveObj, bool32 state))0x00438870)

// <LegoRR.exe @004388d0>
#define LegoObject_CreateInWorld ((LegoObject* (__cdecl* )(void* objSrcData, LegoObject_Type objType, LegoObject_ID objID, sint32 objLevel, real32 xPos, real32 yPos, real32 heading))0x004388d0)

// <LegoRR.exe @00438930>
#define Game_Unk_DoSearchCallbacks_Unique ((LegoObject* (__cdecl* )(const Point2I* blockPos))0x00438930)

// <LegoRR.exe @00438970>
#define LiveManager_FUN_00438970 ((bool32 (__cdecl* )(LegoObject* liveObj, LegoObject** param_2))0x00438970)

// <LegoRR.exe @004389e0>
#define LegoObject_AddThisDrainedCrystals ((bool32 (__cdecl* )(LegoObject* liveObj, sint32 crystalCount))0x004389e0)

// <LegoRR.exe @00438a30>
#define LegoObject_GetBuildingUpgradeCost ((bool32 (__cdecl* )(LegoObject* liveObj, OPTIONAL OUT uint32* oreCost))0x00438a30)

// <LegoRR.exe @00438ab0>
#define LegoObject_FUN_00438ab0 ((void (__cdecl* )(LegoObject* liveObj))0x00438ab0)

// <LegoRR.exe @00438b70>
#define LegoObject_HasEnoughOreToUpgrade ((bool32 (__cdecl* )(LegoObject* liveObj, sint32 objLevel))0x00438b70)

// <LegoRR.exe @00438c20>
#define LegoObject_Search_FUN_00438c20 ((undefined4 (__cdecl* )(OPTIONAL LegoObject* liveObj, bool32 param_2))0x00438c20)

// <LegoRR.exe @00438ca0>
#define LegoObject_Search_FUN_00438ca0 ((undefined4 (__cdecl* )(LegoObject* liveObj, bool32 param_2))0x00438ca0)

// <LegoRR.exe @00438d20>
#define LegoObject_FUN_00438d20 ((LegoObject* (__cdecl* )(const Point2I* blockPos, LegoObject_Type objType, LegoObject_ID objID, sint32 objLevel))0x00438d20)

// <LegoRR.exe @00438da0>
#define LegoObject_FUN_00438da0 ((LegoObject* (__cdecl* )(LegoObject* liveObj, const Point2I* blockPos, LegoObject_Type objType, sint32 objLevel))0x00438da0)

// <LegoRR.exe @00438e40>
#define LegoObject_Search_FUN_00438e40 ((undefined4 (__cdecl* )(LegoObject* liveObj, undefined4 param_2))0x00438e40)

// <LegoRR.exe @00438eb0>
#define LegoObject_Search_FUN_00438eb0 ((undefined4 (__cdecl* )(LegoObject* liveObj))0x00438eb0)

// <LegoRR.exe @00438f20>
#define LegoObject_Search_FUN_00438f20 ((undefined4 (__cdecl* )(LegoObject* liveObj))0x00438f20)

// <LegoRR.exe @00438f90>
#define Game_FindBigTeleporter ((LegoObject* (__cdecl* )(const Point2F* worldPos))0x00438f90)

// <LegoRR.exe @00438ff0>
#define Game_FindSmallTeleporter ((LegoObject* (__cdecl* )(const Point2F* worldPos))0x00438ff0)

// <LegoRR.exe @00439050>
#define Game_FindWaterTeleporter ((LegoObject* (__cdecl* )(const Point2F* worldPos))0x00439050)

// <LegoRR.exe @004390b0>
#define Level_GetBuildingAtPosition ((LegoObject* (__cdecl* )(const Point2F* worldPos))0x004390b0)

// <LegoRR.exe @00439110>
#define LegoObject_Search_FUN_00439110 ((undefined4 (__cdecl* )(LegoObject* liveObj, OPTIONAL const Point2F* worldPos, LegoObject_AbilityFlags trainedType))0x00439110)

// <LegoRR.exe @00439190>
#define LegoObject_HasTraining ((bool32 (__cdecl* )(LegoObject* liveObj, LegoObject_AbilityFlags training))0x00439190)

// <LegoRR.exe @00439220>
#define LegoObject_IsDocksBuilding_Unk ((bool32 (__cdecl* )(LegoObject* liveObj))0x00439220)

// <LegoRR.exe @00439270>
#define LegoObject_CallbackSearch_FUN_00439270 ((bool32 (__cdecl* )(LegoObject* liveObj, sint32** search))0x00439270)

// <LegoRR.exe @004394c0>
#define Game_CanStoredObjectTypeBeSpawned ((bool32 (__cdecl* )(LegoObject_Type objType))0x004394c0)

// <LegoRR.exe @00439500>
#define LegoObject_Callback_CanSpawnStoredObjects ((bool32 (__cdecl* )(LegoObject* liveObj1, LegoObject* spawnObj))0x00439500)

// <LegoRR.exe @00439540>
#define Game_PTL_GenerateFromCryOre ((void (__cdecl* )(const Point2I* blockPos))0x00439540)

// <LegoRR.exe @00439600>
#define Game_PTL_GenerateCrystalsAndOre ((void (__cdecl* )(const Point2I* blockPos, uint32 objLevel))0x00439600)

// <LegoRR.exe @00439630>
#define Level_GenerateCrystal ((void (__cdecl* )(const Point2I* blockPos, uint32 objLevel, OPTIONAL const Point2F* worldPos, bool32 showInfoMessage))0x00439630)

// <LegoRR.exe @00439770>
#define Level_GenerateOre ((void (__cdecl* )(const Point2I* blockPos, uint32 objLevel, OPTIONAL const Point2F* worldPos, bool32 showInfoMessage))0x00439770)

// <LegoRR.exe @004398a0>
#define LegoObject_GetLangName ((char* (__cdecl* )(LegoObject* liveObj))0x004398a0)

// <LegoRR.exe @00439980>
#define Object_GetTypeName ((char* (__cdecl* )(LegoObject_Type objType, LegoObject_ID objID))0x00439980)

// <LegoRR.exe @00439a50>
#define Object_GetLangTheName ((char* (__cdecl* )(LegoObject_Type objType, LegoObject_ID objID))0x00439a50)

// <LegoRR.exe @00439b20>
#define Object_GetLangName ((char* (__cdecl* )(LegoObject_Type objType, LegoObject_ID objID))0x00439b20)

// <LegoRR.exe @00439bf0>
#define LegoObject_GetTypeAndID ((void (__cdecl* )(LegoObject* liveObj, OUT LegoObject_Type* objType, OUT LegoObject_ID* objID))0x00439bf0)

// <LegoRR.exe @00439c10>
#define Object_LoadToolNames ((void (__cdecl* )(const Gods98::Config* config, const char* gameName))0x00439c10)

// <LegoRR.exe @00439c50>
void __cdecl LiveManager_InitFlagsToggle_AndClearNumDrained(void);
//#define LiveManager_InitFlagsToggle_AndClearNumDrained ((void (__cdecl* )(void))0x00439c50)

// <LegoRR.exe @00439c80>
#define LegoObject_VehicleMaxCarryChecksTime_FUN_00439c80 ((bool32 (__cdecl* )(LegoObject* liveObj))0x00439c80)

// <LegoRR.exe @00439ce0>
#define LegoObject_TryCollectObject ((bool32 (__cdecl* )(LegoObject* liveObj, LegoObject* targetObj))0x00439ce0)

// <LegoRR.exe @00439e40>
#define LegoObject_FUN_00439e40 ((bool32 (__cdecl* )(LegoObject* liveObj, LegoObject* otherObj))0x00439e40)

// <LegoRR.exe @00439e90>
#define LegoObject_FUN_00439e90 ((bool32 (__cdecl* )(LegoObject* liveObj, LegoObject* targetObj, bool32 param_3))0x00439e90)

// <LegoRR.exe @00439f40>
#define LegoObject_CompleteVehicleUpgrade ((void (__cdecl* )(LegoObject* liveObj))0x00439f40)

// <LegoRR.exe @00439f90>
#define LegoObject_SetLevel ((bool32 (__cdecl* )(LegoObject* liveObj, sint32 objLevel))0x00439f90)

// <LegoRR.exe @00439fb0>
#define LegoObject_IsSmallTeleporter ((bool32 (__cdecl* )(LegoObject* liveObj))0x00439fb0)

// <LegoRR.exe @00439fd0>
#define LegoObject_IsBigTeleporter ((bool32 (__cdecl* )(LegoObject* liveObj))0x00439fd0)

// <LegoRR.exe @00439ff0>
#define LegoObject_IsWaterTeleporter ((bool32 (__cdecl* )(LegoObject* liveObj))0x00439ff0)

// <LegoRR.exe @0043a010>
#define LegoObject_UnkGetTerrainCrossBlock_FUN_0043a010 ((bool32 (__cdecl* )(LegoObject* liveObj, OPTIONAL OUT Point2I* blockPos))0x0043a010)

// <LegoRR.exe @0043a0d0>
#define LegoObject_UnkGetTerrainGetOutAtLandBlock_FUN_0043a0d0 ((bool32 (__cdecl* )(LegoObject* liveObj, OUT Point2I* blockPos))0x0043a0d0)

// <LegoRR.exe @0043a100>
#define LegoObject_CheckUnkGetInAtLand_FUN_0043a100 ((bool32 (__cdecl* )(LegoObject* liveObj, const Point2I* blockPos))0x0043a100)

// <LegoRR.exe @0043a130>
#define LegoObject_DropCarriedObject ((void (__cdecl* )(LegoObject* liveObj, bool32 param_2))0x0043a130)

// <LegoRR.exe @0043a3e0>
#define LegoObject_TryRequestOrDumpCarried ((bool32 (__cdecl* )(LegoObject* liveObj, const Point2I* blockPos, const Point2F* worldPos, bool32 place, bool32 setRouteFlag8))0x0043a3e0)

// <LegoRR.exe @0043a5c0>
#define LegoObject_TryDepositCarried ((bool32 (__cdecl* )(LegoObject* liveObj, LegoObject* destObj))0x0043a5c0)

// <LegoRR.exe @0043a8b0>
#define LegoObject_GetDepositNull ((Gods98::Container* (__cdecl* )(LegoObject* liveObj))0x0043a8b0)

// <LegoRR.exe @0043a910>
#define LegoObject_FUN_0043a910 ((LegoObject* (__cdecl* )(LegoObject* liveObj, LegoObject_Type objType, LegoObject_ID objID, sint32 objLevel))0x0043a910)

// <LegoRR.exe @0043aa80>
#define LegoObject_CanSpawnCarryableObject ((bool32 (__cdecl* )(LegoObject* liveObj, LegoObject_Type objType, LegoObject_ID objID))0x0043aa80)

// <LegoRR.exe @0043ab10>
#define LegoObject_FUN_0043ab10 ((void (__cdecl* )(LegoObject* liveObj, LegoObject* liveObj2))0x0043ab10)

// <LegoRR.exe @0043abb0>
#define Level_AddCrystals__unusedLegoObject ((void (__cdecl* )(LegoObject* liveObj_unused, uint32 crystalCount))0x0043abb0)

// <LegoRR.exe @0043abd0>
#define Level_AddOre__unusedLegoObject ((void (__cdecl* )(LegoObject* liveObj_unused, uint32 oreCount))0x0043abd0)

// <LegoRR.exe @0043abf0>
#define LegoObject_WaterVehicle_Unregister ((void (__cdecl* )(LegoObject* liveObj))0x0043abf0)

// <LegoRR.exe @0043ac20>
#define LegoObject_WaterVehicle_Register ((void (__cdecl* )(LegoObject* liveObj))0x0043ac20)

// <LegoRR.exe @0043aca0>
#define LegoObject_RegisterVehicle__callsForWater ((void (__cdecl* )(LegoObject* liveObj))0x0043aca0)

// <LegoRR.exe @0043acb0>
#define LegoObject_FUN_0043acb0 ((void (__cdecl* )(LegoObject* liveObj1, LegoObject* liveObj2))0x0043acb0)

// <LegoRR.exe @0043ad70>
#define LegoObject_RockMonster_FUN_0043ad70 ((void (__cdecl* )(LegoObject* liveObj))0x0043ad70)

// <LegoRR.exe @0043aeb0>
#define LegoObject_FUN_0043aeb0 ((void (__cdecl* )(LegoObject* liveObj))0x0043aeb0)

// <LegoRR.exe @0043af50>
#define LegoObject_Callback_TryStampMiniFigureWithCrystal ((bool32 (__cdecl* )(LegoObject* targetObj, LegoObject* stamperObj))0x0043af50)

// <LegoRR.exe @0043b010>
#define LegoObject_TryGenerateSlug ((LegoObject* (__cdecl* )(LegoObject* originObj, LegoObject_ID objID))0x0043b010)

// <LegoRR.exe @0043b160>
#define LegoObject_TryGenerateRMonsterAtRandomBlock ((LegoObject* (__cdecl* )(void))0x0043b160)

// <LegoRR.exe @0043b1f0>
#define LegoObject_TryGenerateRMonster ((LegoObject* (__cdecl* )(ObjectModel* objModel, LegoObject_Type objType, LegoObject_ID objID, uint32 bx, uint32 by))0x0043b1f0)

// <LegoRR.exe @0043b530>
void __cdecl LegoObject_UpdateAll(real32 elapsedGame);
//#define LegoObject_UpdateAll ((void (__cdecl* )(real32 elapsedGame))0x0043b530)

// <LegoRR.exe @0043b5e0>
void __cdecl LegoObject_RemoveAll(void);
//#define LegoObject_RemoveAll ((void (__cdecl* )(void))0x0043b5e0)

// RunThroughLists wrapper for LegoObject_Remove
// <LegoRR.exe @0043b610>
bool32 __cdecl LegoObject_Callback_Remove(LegoObject* liveObj, void* data_unused);
//#define LegoObject_Callback_Remove ((bool32 (__cdecl* )(LegoObject* liveObj, void* data_unused))0x0043b610)

// <LegoRR.exe @0043b620>
#define LiveManager_DoPickSphereCallbacks_MouseXY ((void (__cdecl* )(uint32 mouseX, uint32 mouseY, undefined4* ref_param_3))0x0043b620)

// <LegoRR.exe @0043b670>
#define LegoObject_Callback_FUN_0043b670 ((bool32 (__cdecl* )(LegoObject* liveObj, sint32** param_2))0x0043b670)

// <LegoRR.exe @0043b980>
#define LiveManager_DoSelection ((void (__cdecl* )(Gods98::Viewport* view, const Point2F* dragStart, Point2F* const dragEnd))0x0043b980)

// <LegoRR.exe @0043ba30>
#define LegoObject_CallbackDoSelection ((bool32 (__cdecl* )(LegoObject* liveObj, SearchViewportWindow_14* search))0x0043ba30)

// <LegoRR.exe @0043bae0>
#define LegoObject_SwapPolyFP ((void (__cdecl* )(LegoObject* liveObj, sint32 cameraFrameIndex, bool32 on))0x0043bae0)

// <LegoRR.exe @0043bb10>
#define LegoObject_FP_SetRanges ((void (__cdecl* )(LegoObject* liveObj, Gods98::Container* resData, real32 medPolyRange, real32 highPolyRange, bool32 param_5))0x0043bb10)

// <LegoRR.exe @0043bb90>
#define LegoObject_FP_Callback_SwapPolyMeshParts ((bool32 (__cdecl* )(LegoObject* liveObj, LiveObjectInfo* liveInfo))0x0043bb90)

// <LegoRR.exe @0043bdb0>
#define LegoObject_Check_LotsOfFlags1AndFlags2_FUN_0043bdb0 ((bool32 (__cdecl* )(LegoObject* liveObj))0x0043bdb0)

// <LegoRR.exe @0043bde0>
#define LegoObject_FUN_0043bde0 ((void (__cdecl* )(LegoObject* liveObj))0x0043bde0)

// <LegoRR.exe @0043be80>
#define LegoObject_FUN_0043be80 ((void (__cdecl* )(LegoObject* liveObj))0x0043be80)

// <LegoRR.exe @0043bf00>
#define LegoObject_TeleportUp ((void (__cdecl* )(LegoObject* liveObj))0x0043bf00)

// <LegoRR.exe @0043c4c0>
#define Object_DoOxygenCheck_FUN_0043c4c0 ((bool32 (__cdecl* )(LegoObject_Type objType1, LegoObject_ID objID1, LegoObject_Type objType2, sint32 LegoObject_ID objID2))0x0043c4c0)

// <LegoRR.exe @0043c540>
#define LegoObject_OxygenCallback_FUN_0043c540 ((bool32 (__cdecl* )(LegoObject* liveObj, real32* param_2))0x0043c540)

// <LegoRR.exe @0043c570>
#define LiveManager_UnkRadar_FUN_0043c570 ((void (__cdecl* )(real32 elapsedGame, bool32 isRadarMapView))0x0043c570)

// <LegoRR.exe @0043c5b0>
#define LegoObject_Callback_FUN_0043c5b0 ((bool32 (__cdecl* )(LegoObject* liveObj, bool32* pIsRadarMapView))0x0043c5b0)

// <LegoRR.exe @0043c6a0>
#define LegoObject_FUN_0043c6a0 ((bool32 (__cdecl* )(LegoObject* liveObj))0x0043c6a0)

// <LegoRR.exe @0043c700>
#define LegoObject_GetEquippedBeam ((uint32 (__cdecl* )(LegoObject* liveObj))0x0043c700)

// <LegoRR.exe @0043c750>
#define LegoObject_FUN_0043c750 ((bool32 (__cdecl* )(LegoObject* liveObj, LegoObject* object2FC, sint32 param_3))0x0043c750)

// <LegoRR.exe @0043c780>
#define LegoObject_Proc_FUN_0043c780 ((void (__cdecl* )(LegoObject* liveObj))0x0043c780)

// <LegoRR.exe @0043c7f0>
#define LegoObject_Proc_FUN_0043c7f0 ((void (__cdecl* )(LegoObject* liveObj))0x0043c7f0)

// <LegoRR.exe @0043c830>
#define LegoObject_UpdatePowerConsumption ((void (__cdecl* )(LegoObject* liveObj))0x0043c830)

// <LegoRR.exe @0043c910>
#define LegoObject_CheckCanSteal ((bool32 (__cdecl* )(LegoObject* liveObj))0x0043c910)

// <LegoRR.exe @0043c970>
#define LegoObject_FUN_0043c970 ((void (__cdecl* )(LegoObject* liveObj, real32 elapsed))0x0043c970)

// <LegoRR.exe @0043cad0>
#define LegoObject_Callback_Update ((bool32 (__cdecl* )(LegoObject* liveObj, void* pElapsed))0x0043cad0)

// <LegoRR.exe @0043f160>
#define LegoObject_ProcCarriedObjects_FUN_0043f160 ((void (__cdecl* )(LegoObject* liveObj))0x0043f160)

// <LegoRR.exe @0043f3c0>
#define LegoObject_ClearFlags4_40_AndSameForObject2FC ((void (__cdecl* )(LegoObject* liveObj_unused, LegoObject* liveObj))0x0043f3c0)

// <LegoRR.exe @0043f3f0>
#define LegoObject_TriggerFrameCallback ((void (__cdecl* )(Gods98::Container* cont, void* data))0x0043f3f0)

// <LegoRR.exe @0043f410>
#define LegoObject_QueueTeleport ((bool32 (__cdecl* )(LegoObject* liveObj, LegoObject_Type objType, LegoObject_ID objID))0x0043f410)

// <LegoRR.exe @0043f450>
#define LegoObject_FUN_0043f450 ((void (__cdecl* )(LegoObject* liveObj))0x0043f450)

// <LegoRR.exe @0043f820>
#define LegoObject_RemoveObject3CC ((bool32 (__cdecl* )(LegoObject* liveObj))0x0043f820)

// <LegoRR.exe @0043f840>
#define LegoObject_CallbackRemoveObject3CC ((bool32 (__cdecl* )(LegoObject* liveObj, LegoObject* otherObj))0x0043f840)

// <LegoRR.exe @0043f870>
#define LegoObject_TrainMiniFigure_instantunk ((void (__cdecl* )(LegoObject* liveObj, LegoObject_AbilityFlags trainFlags))0x0043f870)

// <LegoRR.exe @0043f960>
#define LegoObject_AddDamage2 ((void (__cdecl* )(LegoObject* liveObj, real32 damage, bool32 param_3, real32 param_4))0x0043f960)

// <LegoRR.exe @0043fa90>
#define LegoObject_UnkUpdateEnergyHealth ((void (__cdecl* )(LegoObject* liveObj, real32 elapsed))0x0043fa90)

// <LegoRR.exe @0043fe00>
#define LegoObject_MiniFigurePlayHurtSND ((bool32 (__cdecl* )(LegoObject* liveObj, real32 elapsed, real32 damage))0x0043fe00)

// <LegoRR.exe @0043fee0>
#define LegoObject_FUN_0043fee0 ((bool32 (__cdecl* )(LegoObject* carriedObj))0x0043fee0)

// <LegoRR.exe @00440080>
#define LegoObject_UnkCarryingVehicle_FUN_00440080 ((bool32 (__cdecl* )(LegoObject* liveObj))0x00440080)

// <LegoRR.exe @00440130>
#define LegoObject_TryFindLoad_FUN_00440130 ((bool32 (__cdecl* )(LegoObject* liveObj, LegoObject* targetObj))0x00440130)

// <LegoRR.exe @004402b0>
#define LegoObject_TryDock_FUN_004402b0 ((bool32 (__cdecl* )(LegoObject* liveObj))0x004402b0)

// <LegoRR.exe @004403f0>
#define LegoObject_TryDock_AtBlockPos_FUN_004403f0 ((void (__cdecl* )(LegoObject* liveObj, const Point2I* blockPos))0x004403f0)

// <LegoRR.exe @00440470>
#define LegoObject_FUN_00440470 ((bool32 (__cdecl* )(LegoObject* liveObj, bool32 param_2))0x00440470)

// <LegoRR.exe @00440690>
#define LegoObject_TryFindDriver_FUN_00440690 ((bool32 (__cdecl* )(LegoObject* liveObj, LegoObject* drivableObj))0x00440690)

// <LegoRR.exe @00440a70>
#define Game_DoDynamiteExplosionCallbacks ((bool32 (__cdecl* )(LegoObject* liveObj, real32 damageRadius, real32 maxDamage, real32 wakeRadius))0x00440a70)

// <LegoRR.exe @00440ac0>
#define LegoObject_Callback_DynamiteRadius ((bool32 (__cdecl* )(LegoObject* liveObj, SearchDynamiteRadius* search))0x00440ac0)

// Must pass a non-null LegoObject* -OR- Point2F* in order to get a position. (LegoObject* has precedence)
// <LegoRR.exe @00440b80>
#define Game_DoBirdScarerRadiusCallbacks_FUN_00440b80 ((void (__cdecl* )(OPTIONAL LegoObject* liveObj, OPTIONAL const Point2F* worldPos, real32 radius))0x00440b80)

// <LegoRR.exe @00440be0>
#define LegoObject_CallbackProc_FUN_00440be0 ((bool32 (__cdecl* )(LegoObject* liveObj, LegoObject** search))0x00440be0)

// <LegoRR.exe @00440ca0>
#define LegoObject_SetActivity ((void (__cdecl* )(LegoObject* liveObj, Activity_Type actType, bool32 unkbool2d8))0x00440ca0)

// <LegoRR.exe @00440cd0>
#define LegoObject_UpdateCarrying ((void (__cdecl* )(LegoObject* liveObj))0x00440cd0)

// <LegoRR.exe @00440eb0>
#define LegoObject_InitBoulderMesh_FUN_00440eb0 ((void (__cdecl* )(LegoObject* liveObj, Gods98::Container_Texture* contTexture))0x00440eb0)

// <LegoRR.exe @00440ef0>
#define LegoObject_FUN_00440ef0 ((bool32 (__cdecl* )(LegoObject* liveObj, undefined4 param_2, undefined4 param_3, undefined4 param_4, undefined4 param_5, undefined4 param_6, undefined4 param_7, undefined4 param_8))0x00440ef0)

// <LegoRR.exe @00440f30>
#define LegoObject_FUN_00440f30 ((bool32 (__cdecl* )(LegoObject* liveObj, uint32 param_2, uint32 param_3, uint32 param_4, uint32 param_5, uint32** param_6, uint32** param_7, uint32* param_8, undefined* param_9, undefined4 param_10))0x00440f30)

// <LegoRR.exe @004413b0>
#define LegoObject_FUN_004413b0 ((bool32 (__cdecl* )(LegoObject* liveObj, uint32 bx, uint32 by, uint32 param_4, uint32 param_5, uint32** param_6, uint32** param_7, real32* param_8, void* callback, undefined4 param_10))0x004413b0)

// <LegoRR.exe @004419c0>
#define LegoObject_RoutingPtr_Alloc_FUN_004419c0 ((bool32 (__cdecl* )(LegoObject* liveObj, uint32 count, real32* param_3, real32* param_4, real32* param_5))0x004419c0)

// <LegoRR.exe @00441c00>
#define LegoObject_UnkDoRouting_FUN_00441c00 ((void (__cdecl* )(LegoObject* liveObj, bool32 param_2))0x00441c00)

// <LegoRR.exe @00441df0>
#define LegoObject_Interrupt ((void (__cdecl* )(LegoObject* liveObj, bool32 actStand, bool32 dropCarried))0x00441df0)

// <LegoRR.exe @00442160>
#define LegoObject_DestroyBoulder_AndCreateExplode ((void (__cdecl* )(LegoObject* liveObj))0x00442160)

// <LegoRR.exe @00442190>
#define LegoObject_Proc_FUN_00442190 ((bool32 (__cdecl* )(LegoObject* liveObj, LegoObject* targetObj, sint32 param_3))0x00442190)

// <LegoRR.exe @00442390>
#define Game_GetWeaponUnk ((void (__cdecl* )(LegoObject* liveObj, sint32 weaponType))0x00442390)

// <LegoRR.exe @004424d0>
#define LegoObject_UnkActivityCrumble_FUN_004424d0 ((void (__cdecl* )(LegoObject* liveObj))0x004424d0)

// <LegoRR.exe @00442520>
#define LegoObject_GetPosition ((void (__cdecl* )(LegoObject* liveObj, OUT real32* xPos, OUT real32* yPos))0x00442520)

// <LegoRR.exe @00442560>
#define LegoObject_GetFaceDirection ((void (__cdecl* )(LegoObject* liveObj, OUT Point2F* direction))0x00442560)

// When vecDir is null, the input is set to a randomly generated vector.
// <LegoRR.exe @004425c0>
#define LegoObject_UnkUpdateOrientation ((void (__cdecl* )(LegoObject* liveObj, real32 theta, OPTIONAL const Vector3F* vecDir))0x004425c0)

// Returns faceDirection in radians (always positive value between 0 and M_PI*2).
// <LegoRR.exe @00442740>
#define LegoObject_GetHeading ((real32 (__cdecl* )(LegoObject* liveObj))0x00442740)

// <LegoRR.exe @004427b0>
#define LegoObject_GetBlockPos ((bool32 (__cdecl* )(LegoObject* liveObj, OUT sint32* bx, OUT sint32* by))0x004427b0)

// <LegoRR.exe @00442800>
#define LegoObject_GetWorldZCallback ((real32 (__cdecl* )(real32 xPos, real32 yPos, Map3D* map))0x00442800)

// The same as `LegoObject_GetWorldZCallback`, but returns a lower Z value with over Lake terrain.
// <LegoRR.exe @00442820>
#define LegoObject_GetWorldZCallback_Lake ((real32 (__cdecl* )(real32 xPos, real32 yPos, Map3D* map))0x00442820)

// <LegoRR.exe @004428b0>
#define LegoObject_FUN_004428b0 ((void (__cdecl* )(LegoObject* liveObj, real32 xDir, real32 yDir))0x004428b0)

// <LegoRR.exe @00442b60>
#define LegoObject_SetPositionAndHeading ((void (__cdecl* )(LegoObject* liveObj, real32 xPos, real32 yPos, real32 theta, bool32 assignHeading))0x00442b60)

// <LegoRR.exe @00442dd0>
sint32 __cdecl LegoObject_FP_UpdateMovement(LegoObject* liveObj, real32 elapsed, OUT real32* param_3);
//#define LegoObject_FP_UpdateMovement ((sint32 (__cdecl* )(LegoObject* liveObj, real32 elapsed, OUT real32* param_3))0x00442dd0)

// <LegoRR.exe @00443240>
#define LegoObject_RockFall_FUN_00443240 ((void (__cdecl* )(LegoObject* liveObj, real32 param_2))0x00443240)

// <LegoRR.exe @004437d0>
#define LegoObject_UpdateDriverObjectPositions_FUN_004437d0 ((void (__cdecl* )(LegoObject* liveObj))0x004437d0)

// <LegoRR.exe @00443930>
#define LegoObject_FUN_00443930 ((bool32 (__cdecl* )(LegoObject* liveObj))0x00443930)

// <LegoRR.exe @004439b0>
#define LegoObject_IsRockMonsterCanGather ((bool32 (__cdecl* )(LegoObject* liveObj))0x004439b0)

// <LegoRR.exe @004439d0>
#define LegoObject_FUN_004439d0 ((bool32 (__cdecl* )(LegoObject* liveObj, const Point2I* blockPos, OUT Point2I* pointUnkForI, undefined4 unused))0x004439d0)

// <LegoRR.exe @00443ab0>
#define LegoObject_RockMonster_DoWakeUp ((void (__cdecl* )(LegoObject* liveObj))0x00443ab0)

// <LegoRR.exe @00443b00>
#define LegoObject_FUN_00443b00 ((bool32 (__cdecl* )(LegoObject* liveObj, const Point2I* blockPos, bool32* pAlsoCheckBlockFlags1_10Not8))0x00443b00)

// <LegoRR.exe @00443b70>
#define LegoObject_LargeFlagsSwitch_FUN_00443b70 ((real32 (__cdecl* )(LegoObject* liveObj, real32 param_2))0x00443b70)

// <LegoRR.exe @00444360>
#define LegoObject_TryDock_AtObject2FC ((void (__cdecl* )(LegoObject* liveObj))0x00444360)

// <LegoRR.exe @004443b0>
#define LegoObject_FUN_004443b0 ((void (__cdecl* )(LegoObject* liveObj, real32 elapsed))0x004443b0)

// <LegoRR.exe @00444520>
#define LegoObject_FUN_00444520 ((bool32 (__cdecl* )(LegoObject* liveObj))0x00444520)

// <LegoRR.exe @00444720>
#define LegoObject_FUN_00444720 ((void (__cdecl* )(LegoObject* liveObj, const Point2F* worldPos))0x00444720)

// <LegoRR.exe @004448e0>
#define LegoObject_DoSlip ((void (__cdecl* )(LegoObject* liveObj))0x004448e0)

// <LegoRR.exe @00444940>
#define LegoObject_RoutingUnk_FUN_00444940 ((bool32 (__cdecl* )(LegoObject* liveObj, sint32 param_2, sint32 param_3, sint32 param_4))0x00444940)

// <LegoRR.exe @00445270>
#define LegoObject_FUN_00445270 ((void (__cdecl* )(LegoObject* liveObj, const Point2F* worldPos))0x00445270)

// <LegoRR.exe @004454a0>
#define LegoObject_FUN_004454a0 ((void (__cdecl* )(LegoObject* liveObj))0x004454a0)

// <LegoRR.exe @00445600>
#define LegoObject_Callback_FUN_00445600 ((bool32 (__cdecl* )(LegoObject* liveObj1, LegoObject** param_2))0x00445600)

// <LegoRR.exe @00445860>
#define LegoObject_FUN_00445860 ((bool32 (__cdecl* )(LegoObject* liveObj))0x00445860)

// <LegoRR.exe @004459a0>
#define LegoObject_FUN_004459a0 ((void (__cdecl* )(LegoObject* liveObj))0x004459a0)

// <LegoRR.exe @00445a30>
#define LegoObject_Callback_FUN_00445a30 ((bool32 (__cdecl* )(LegoObject* liveObj1, LegoObject* liveObj2))0x00445a30)

// <LegoRR.exe @00445af0>
#define LegoObject_Callback_FUN_00445af0 ((bool32 (__cdecl* )(LegoObject* liveObj1, LegoObject* liveObj2))0x00445af0)

// <LegoRR.exe @00446030>
#define LegoObject_DoCollisionCallbacks_FUN_00446030 ((LegoObject* (__cdecl* )(LegoObject* liveObj, const Point2F* param_2, real32 param_3, bool32 param_4))0x00446030)

// <LegoRR.exe @004460b0>
#define LegoObject_CallbackCollisionRadius_FUN_004460b0 ((bool32 (__cdecl* )(LegoObject* liveObj, sint32* search))0x004460b0)

// <LegoRR.exe @004463b0>
#define LegoObject_CallbackCollisionBox_FUN_004463b0 ((bool32 (__cdecl* )(LegoObject* liveObj, SearchCollision_14* search))0x004463b0)

// <LegoRR.exe @004468d0>
void __cdecl LegoObject_CalculateSpeeds(LegoObject* liveObj, real32 elapsed, OUT real32* routeSpeed, OUT real32* transSpeed);
//#define LegoObject_CalculateSpeeds ((void (__cdecl* )(LegoObject* liveObj, real32 elapsed, OUT real32* routeSpeed, OUT real32* transSpeed))0x004468d0)

// <LegoRR.exe @00446b80>
#define LegoObject_RoutingPtr_Realloc_FUN_00446b80 ((bool32 (__cdecl* )(LegoObject* liveObj, uint32 bx, uint32 by))0x00446b80)

// <LegoRR.exe @00446c80>
#define LegoObject_BlockRoute_FUN_00446c80 ((bool32 (__cdecl* )(LegoObject* liveObj, uint32 bx, uint32 by, bool32 useWideRange, OPTIONAL OUT Direction* direction, bool32 countIs8))0x00446c80)

// <LegoRR.exe @00447100>
#define LegoObject_RouteToDig_FUN_00447100 ((bool32 (__cdecl* )(LegoObject* liveObj, uint32 bx, uint32 by, bool32 tunnelDig))0x00447100)

// <LegoRR.exe @00447390>
#define Game_PTL_GatherRock ((bool32 (__cdecl* )(LegoObject* liveObj))0x00447390)

// <LegoRR.exe @00447470>
#define LegoObject_RoutingNoCarry_FUN_00447470 ((bool32 (__cdecl* )(LegoObject* liveObj, uint32 bx, uint32 by, LegoObject* liveObj2))0x00447470)

// <LegoRR.exe @004474d0>
#define Game_PTL_AttackBuilding ((bool32 (__cdecl* )(LegoObject* liveObj1, LegoObject* targetObj))0x004474d0)

// <LegoRR.exe @00447670>
#define LegoObject_FUN_00447670 ((sint32 (__cdecl* )(LegoObject* liveObj1, sint32 bx, sint32 by, LegoObject* liveObj2))0x00447670)

// <LegoRR.exe @004477b0>
#define LegoObject_FUN_004477b0 ((void (__cdecl* )(LegoObject* liveObj))0x004477b0)

// <LegoRR.exe @00447880>
#define LegoObject_FUN_00447880 ((sint32 (__cdecl* )(LegoObject* liveObj))0x00447880)

// <LegoRR.exe @004479f0>
#define LegoObject_Add25EnergyAndSetHealth ((bool32 (__cdecl* )(LegoObject* liveObj))0x004479f0)

// <LegoRR.exe @00447a40>
#define LegoObject_FUN_00447a40 ((void (__cdecl* )(LegoObject* liveObj))0x00447a40)

// <LegoRR.exe @00447a90>
#define LegoObject_FUN_00447a90 ((void (__cdecl* )(LegoObject* liveObj))0x00447a90)

// <LegoRR.exe @00447bc0>
#define LegoObject_DoBuildingsCallback_AttackByBoulder ((void (__cdecl* )(LegoObject* liveObj))0x00447bc0)

// <LegoRR.exe @00447be0>
#define LegoObject_CallbackBoulderAttackBuilding_FUN_00447be0 ((bool32 (__cdecl* )(LegoObject* liveObj, LegoObject* buildingLiveObj))0x00447be0)

// <LegoRR.exe @00447c10>
#define LegoObject_FUN_00447c10 ((void (__cdecl* )(LegoObject* liveObj, const Point2F* param_2, bool32 param_3))0x00447c10)

// <LegoRR.exe @00447dc0>
#define LegoObject_TeleportDownBuilding ((void (__cdecl* )(LegoObject* liveObj))0x00447dc0)

// <LegoRR.exe @00447df0>
#define LegoObject_FUN_00447df0 ((real32 (__cdecl* )(LegoObject* liveObj, real32 elapsed))0x00447df0)

// <LegoRR.exe @00447f00>
#define LegoObject_Container_ActivityUpdate_Check ((bool32 (__cdecl* )(LegoObject* liveObj))0x00447f00)

// <LegoRR.exe @00448160>
#define LegoObject_FUN_00448160 ((void (__cdecl* )(LegoObject* liveObj, real32 elapsed))0x00448160)

// <LegoRR.exe @00448a80>
#define LegoObject_Debug_DropActivateDynamite ((void (__cdecl* )(LegoObject* liveObj))0x00448a80)

// <LegoRR.exe @00448ac0>
#define LegoObject_TryDynamite_FUN_00448ac0 ((bool32 (__cdecl* )(LegoObject* liveObj, const Point2I* blockPos))0x00448ac0)

// <LegoRR.exe @00448b40>
#define LegoObject_PlaceCarriedBirdScarerAt ((bool32 (__cdecl* )(LegoObject* liveObj, const Point2I* blockPos))0x00448b40)

// <LegoRR.exe @00448c60>
#define LegoObject_PlaceBirdScarer_AndTickDown ((bool32 (__cdecl* )(LegoObject* liveObj))0x00448c60)

// <LegoRR.exe @00448d20>
#define LegoObject_TryElecFence_FUN_00448d20 ((bool32 (__cdecl* )(LegoObject* liveObj, const Point2I* blockPos))0x00448d20)

// <LegoRR.exe @00448f10>
#define LegoObject_TryBuildPath_FUN_00448f10 ((bool32 (__cdecl* )(LegoObject* liveObj))0x00448f10)

// <LegoRR.exe @00448f50>
#define LegoObject_TryUpgrade_FUN_00448f50 ((bool32 (__cdecl* )(LegoObject* liveObj, LegoObject* targetObj, sint32 targetObjLevel))0x00448f50)

// <LegoRR.exe @00449170>
#define LegoObject_TryTrain_FUN_00449170 ((bool32 (__cdecl* )(LegoObject* liveObj, LegoObject* targetObj, bool32 set_0xE_or0xF))0x00449170)

// <LegoRR.exe @004492d0>
#define LegoObject_TryRechargeCarried ((bool32 (__cdecl* )(LegoObject* liveObj))0x004492d0)

// For MiniFigures, the action repairs. For RockMonsters (enemies), the action drains.
// <LegoRR.exe @00449360>
#define LegoObject_TryRepairDrainObject ((bool32 (__cdecl* )(LegoObject* liveObj, LegoObject* targetObj, bool32 setRouteFlag20, bool32 setLive4Flag400000))0x00449360)

// <LegoRR.exe @00449500>
#define LegoObject_TryReinforceBlock ((bool32 (__cdecl* )(LegoObject* liveObj, const Point2I* blockPos))0x00449500)

// <LegoRR.exe @00449570>
#define LegoObject_TryClear_FUN_00449570 ((bool32 (__cdecl* )(LegoObject* liveObj, const Point2I* blockPos))0x00449570)

// <LegoRR.exe @004496a0>
#define LegoObject_MiniFigureHasBeamEquipped ((bool32 (__cdecl* )(LegoObject* liveObj))0x004496a0)

// <LegoRR.exe @004496f0>
#define LegoObject_TryAttackRockMonster_FUN_004496f0 ((bool32 (__cdecl* )(LegoObject* liveObj, LegoObject* targetObj))0x004496f0)

// <LegoRR.exe @004497e0>
#define LegoObject_TryAttackObject_FUN_004497e0 ((bool32 (__cdecl* )(LegoObject* liveObj, LegoObject* targetObj))0x004497e0)

// <LegoRR.exe @004498d0>
#define LegoObject_TryAttackPath_FUN_004498d0 ((bool32 (__cdecl* )(LegoObject* liveObj, const Point2I* blockPos))0x004498d0)

// <LegoRR.exe @004499c0>
#define LegoObject_TryDepart_FUN_004499c0 ((bool32 (__cdecl* )(LegoObject* liveObj))0x004499c0)

// <LegoRR.exe @00449b40>
#define LegoObject_RouteToFaceBlock ((bool32 (__cdecl* )(LegoObject* liveObj, uint32 bx, uint32 by, real32 distFrom))0x00449b40)

// <LegoRR.exe @00449c40>
#define LegoObject_Reinforcing_FUN_00449c40 ((bool32 (__cdecl* )(LegoObject* liveObj, undefined4 param_2, OUT bool32* param_bool))0x00449c40)

// <LegoRR.exe @00449d30>
#define LegoObject_GoEat_unk ((void (__cdecl* )(LegoObject* liveObj))0x00449d30)

// <LegoRR.exe @00449d80>
#define LegoObject_TryGoEat_FUN_00449d80 ((bool32 (__cdecl* )(LegoObject* liveObj1, LegoObject* liveObj2))0x00449d80)

// <LegoRR.exe @00449ec0>
void __cdecl LiveManager_AllCallback_FUN_00449ec0(void);
//#define LiveManager_AllCallback_FUN_00449ec0 ((void (__cdecl* )(void))0x00449ec0)

// <LegoRR.exe @00449ee0>
#define LegoObject_FlocksCallback_FUN_00449ee0 ((void (__cdecl* )(Flocks* flockData, FlocksItem* subdata, void* data))0x00449ee0)

// <LegoRR.exe @00449f90>
#define LegoObject_Hide2 ((void (__cdecl* )(LegoObject* liveObj, bool32 hide2))0x00449f90)

// <LegoRR.exe @00449fb0>
#define LegoObject_Callback_FUN_00449fb0 ((bool32 (__cdecl* )(LegoObject* liveObj, void* unused))0x00449fb0)

// <LegoRR.exe @0044a210>
#define LegoObject_Hide ((void (__cdecl* )(LegoObject* liveObj, bool32 hide))0x0044a210)

// <LegoRR.exe @0044a2b0>
#define LegoObject_IsHidden ((bool32 (__cdecl* )(LegoObject* liveObj))0x0044a2b0)

// <LegoRR.exe @0044a330>
#define LegoObject_FP_GetPositionAndHeading ((void (__cdecl* )(LegoObject* liveObj, sint32 cameraFrame, OUT Vector3F* worldPos, OUT Vector3F* dir))0x0044a330)

// <LegoRR.exe @0044a4c0>
#define LegoObject_GetActivityContainer ((Gods98::Container* (__cdecl* )(LegoObject* liveObj))0x0044a4c0)

// <LegoRR.exe @0044a560>
#define LegoObject_GetDrillNullPosition ((bool32 (__cdecl* )(LegoObject* liveObj, OUT real32* xPos, OUT real32* yPos))0x0044a560)

// <LegoRR.exe @0044a5d0>
#define LegoObject_FP_Move ((void (__cdecl* )(LegoObject* liveObj, sint32 forward, sint32 strafe, real32 rotate))0x0044a5d0)

// <LegoRR.exe @0044a650>
#define Level_RegisterRechargeSeam ((void (__cdecl* )(const Point2I* blockPos))0x0044a650)

// <LegoRR.exe @0044a690>
#define LegoObject_FindNearestRechargeSeam ((bool32 (__cdecl* )(LegoObject* liveObj, OPTIONAL OUT Point2I* blockPos))0x0044a690)

// <LegoRR.exe @0044a770>
#define LegoObject_RegisterSlimySlugHole ((void (__cdecl* )(const Point2I* blockPos))0x0044a770)

// <LegoRR.exe @0044a7b0>
#define LegoObject_FindNearestSlugHole ((bool32 (__cdecl* )(LegoObject* liveObj, OPTIONAL OUT Point2I* blockPos))0x0044a7b0)

// <LegoRR.exe @0044a890>
#define LegoObject_FUN_0044a890 ((bool32 (__cdecl* )(LegoObject* liveObj, sint32* param_2, sint32* param_3, bool32 param_4, bool32 param_5, bool32 param_6))0x0044a890)

// <LegoRR.exe @0044aa60>
#define Lego_QsortCompare_FUN_0044aa60 ((bool32 (__cdecl* )(sint32 param_1, sint32 param_2))0x0044aa60)

// <LegoRR.exe @0044aa90>
#define Res_LoadMeshLOD ((MeshLOD* (__cdecl* )(Gods98::Config* ae, const char* gameName, const char* dirname, LOD_PolyLevel polyMode, uint32 numCameraFrames))0x0044aa90)

// <LegoRR.exe @0044abd0>
#define Game_UnkBuildingPlaceDirection ((bool32 (__cdecl* )(LegoObject_Type objType, LegoObject_ID objID, bool32 param_3, bool32 param_4, const Point2F* mouseWorldPos, uint32 mouseBlockX, uint32 mouseBlockY, bool32 param_8, SelectPlace* selectPlace))0x0044abd0)

// <LegoRR.exe @0044af80>
#define ObjTts_Initialise ((void (__cdecl* )(const Gods98::Config* config, const char* gameName))0x0044af80)

// <LegoRR.exe @0044b040>
#define LegoObject_GetObjTtSFX ((sint32 (__cdecl* )(LegoObject* liveObj))0x0044b040)

// <LegoRR.exe @0044b080>
void __cdecl LegoObject_SetGlobFlag20(bool32 state);
//#define LegoObject_SetGlobFlag20 ((void (__cdecl* )(bool32 state))0x0044b080)

// <LegoRR.exe @0044b0a0>
#define LegoObject_FUN_0044b0a0 ((void (__cdecl* )(LegoObject* liveObj))0x0044b0a0)

// <LegoRR.exe @0044b110>
#define LegoObject_SpawnDropCrystals_FUN_0044b110 ((void (__cdecl* )(LegoObject* liveObj, sint32 crystalCount, bool32 param_3))0x0044b110)

// <LegoRR.exe @0044b250>
#define LegoObject_CallsSpawnDropCrystals_FUN_0044b250 ((void (__cdecl* )(LegoObject* liveObj))0x0044b250)

// <LegoRR.exe @0044b270>
#define LegoObject_GenerateTinyRMs_FUN_0044b270 ((void (__cdecl* )(LegoObject* liveObj))0x0044b270)

// <LegoRR.exe @0044b400>
#define Level_GenerateSmallSpiders ((void (__cdecl* )(uint32 x, uint32 y, uint32 randSeed))0x0044b400)

// <LegoRR.exe @0044b510>
#define LegoObject_DoThrowLegoman ((void (__cdecl* )(LegoObject* liveObj, LegoObject* thrownObj))0x0044b510)

// <LegoRR.exe @0044b610>
#define Tool_IsBeamWeapon ((bool32 (__cdecl* )(LegoObject_ToolType toolType))0x0044b610)

// <LegoRR.exe @0044b630>
#define MiniFigure_EquipTool ((void (__cdecl* )(LegoObject* liveObj, LegoObject_ToolType toolType))0x0044b630)

// <LegoRR.exe @0044b740>
#define LegoObject_HasToolEquipped ((bool32 (__cdecl* )(LegoObject* liveObj, LegoObject_ToolType toolType))0x0044b740)

// <LegoRR.exe @0044b780>
#define LegoObject_TaskHasTool_FUN_0044b780 ((bool32 (__cdecl* )(LegoObject* liveObj, AITask_Type taskType))0x0044b780)

// <LegoRR.exe @0044b7c0>
#define LegoObject_DoGetTool ((sint32 (__cdecl* )(LegoObject* liveObj, LegoObject_ToolType toolType))0x0044b7c0)

// <LegoRR.exe @0044b940>
#define LegoObject_Flocks_Initialise ((void (__cdecl* )(LegoObject* liveObj))0x0044b940)

// <LegoRR.exe @0044ba60>
#define FlocksMatrix_FUN_0044ba60 ((void (__cdecl* )(Gods98::Container* resData, IN OUT Matrix4F* matrix, real32 scalar))0x0044ba60)

// <LegoRR.exe @0044bb10>
#define Flocks_Callback_SubdataOrientationAnim ((void (__cdecl* )(Flocks* flocksData, FlocksItem* subdata, real32* pElapsed))0x0044bb10)

// <LegoRR.exe @0044bd70>
#define Flocks_Container_ReleaseCallback ((void (__cdecl* )(Flocks* flockData, FlocksItem* subdata, void* data))0x0044bd70)

// <LegoRR.exe @0044bda0>
#define Flocks_Free ((void (__cdecl* )(Flocks* flockData))0x0044bda0)

// <LegoRR.exe @0044bdf0>
#define LegoObject_Flocks_SetParameters ((void (__cdecl* )(LegoObject* liveObj, bool32 additive))0x0044bdf0)

// <LegoRR.exe @0044bef0>
#define LegoObject_Flocks_FUN_0044bef0 ((void (__cdecl* )(LegoObject* liveObj, real32 elapsed))0x0044bef0)

// <LegoRR.exe @0044c0f0>
#define LegoObject_FlocksDestroy ((void (__cdecl* )(LegoObject* liveObj))0x0044c0f0)

// <LegoRR.exe @0044c1c0>
#define Flocks_Update_FUN_0044c1c0 ((void (__cdecl* )(real32* pElapsed))0x0044c1c0)

// <LegoRR.exe @0044c290>
#define LegoObject_DestroyRockMonster_FUN_0044c290 ((bool32 (__cdecl* )(LegoObject* liveObj))0x0044c290)

// <LegoRR.exe @0044c2f0>
#define LegoObject_FUN_0044c2f0 ((bool32 (__cdecl* )(LegoObject* liveObj, real32 elapsed))0x0044c2f0)

// <LegoRR.exe @0044c3d0>
#define LegoObject_FUN_0044c3d0 ((void (__cdecl* )(LegoObject* liveObj))0x0044c3d0)

// <LegoRR.exe @0044c410>
#define LegoObject_Push ((bool32 (__cdecl* )(LegoObject* liveObj, const Point2F* pushVec2D, real32 pushDist))0x0044c410)

// <LegoRR.exe @0044c470>
#define LegoObject_FUN_0044c470 ((void (__cdecl* )(LegoObject* liveObj, real32 elapsed))0x0044c470)

// <LegoRR.exe @0044c760>
#define LegoObject_FUN_0044c760 ((void (__cdecl* )(LegoObject* liveObj))0x0044c760)

// <LegoRR.exe @0044c7c0>
#define LegoObject_Callback_UnkLaserTrackerToggleUnset_FUN_0044c7c0 ((bool32 (__cdecl* )(LegoObject* liveObj, void* unused))0x0044c7c0)

// <LegoRR.exe @0044c7f0>
#define LegoObject_MiniFigureHasBeamEquipped2 ((bool32 (__cdecl* )(LegoObject* liveObj))0x0044c7f0)

// Called by Panel_CheckCollision for type Panel_CameraControl
// <LegoRR.exe @0044c810>
void __cdecl LegoObject_CameraCycleUnits(void);
//#define LegoObject_CameraCycleUnits ((void (__cdecl* )(void))0x0044c810)

// pCheckMiniFigures : false to check Buildings, true to check MiniFigures
// <LegoRR.exe @0044c8b0>
bool32 __cdecl LegoObject_Callback_CameraCycleFindUnit(LegoObject* liveObj, OPTIONAL void* pNoBuildings);
//#define LegoObject_Callback_CameraCycleFindUnit ((bool32 (__cdecl* )(LegoObject* liveObj, OPTIONAL void* pNoBuildings))0x0044c8b0)

#pragma endregion

}
