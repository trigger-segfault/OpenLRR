// Object.h : 
//

#pragma once

#include "../../common.h"
#include "../../engine/colour.h"
#include "../../engine/geometry.h"
#include "../../engine/undefined.h"

#include "../../engine/gfx/Containers.h"
#include "../../engine/drawing/Images.h"

#include "../audio/SFX.h"

#include "../GameCommon.h"

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

namespace Dummy {
	struct SaveStruct_18
	{
		/*00,18*/ undefined field_0x0_0x17[24];
		/*18*/
	};
	assert_sizeof(SaveStruct_18, 0x18);
};

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

enum LiveFlags1 : uint32 // [LegoRR/LegoObject.c|flags:0x4|type:uint]
{
	LIVEOBJ1_NONE         = 0,
	LIVEOBJ1_MOVING       = 0x1,
	LIVEOBJ1_LIFTING      = 0x2,
	LIVEOBJ1_TURNING      = 0x4,
	LIVEOBJ1_UNK_8        = 0x8,
	LIVEOBJ1_UNK_10       = 0x10,
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
	LIVEOBJ1_UNK_4000000  = 0x4000000,
	LIVEOBJ1_UNK_10000000 = 0x10000000,
	LIVEOBJ1_REST         = 0x20000000,
	LIVEOBJ1_EATING       = 0x40000000,
	LIVEOBJ1_UNK_80000000 = 0x80000000,
};
flags_end(LiveFlags1, 0x4);


enum LiveFlags2 : uint32 // [LegoRR/LegoObject.c|flags:0x4|type:uint]
{
	LIVEOBJ2_NONE         = 0,
	LIVEOBJ2_UNK_1        = 0x1,
	LIVEOBJ2_UNK_2        = 0x2,
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
	LIVEOBJ2_UNK_10000    = 0x10000,
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


enum LiveFlags5 : uint32 // [LegoRR/LegoObject.c|flags:0x4|type:uint] These flags contain LiveObject abilities, and are the one of the fields stored/restored with ObjectRecall.
{
	LIVEOBJ5_NONE             = 0,
	LIVEOBJ5_ABILITY_PILOT    = 0x1,
	LIVEOBJ5_ABILITY_SAILOR   = 0x2,
	LIVEOBJ5_ABILITY_DRIVER   = 0x4,
	LIVEOBJ5_ABILITY_DYNAMITE = 0x8,
	LIVEOBJ5_ABILITY_REPAIR   = 0x10,
	LIVEOBJ5_ABILITY_SCANNER  = 0x20,
};
flags_end(LiveFlags5, 0x4);


flags_scoped(LegoObject_GlobFlags) : uint32 // [LegoRR/LegoObject.c|flags:0x4|type:uint] LegoObject_GlobFlags, ReservedPool LiveObject INITFLAGS
{
	OBJECT_GLOB_FLAG_NONE        = 0,
	OBJECT_GLOB_FLAG_INITIALISED = 0x1,
	OBJECT_GLOB_FLAG_UNK_20      = 0x20,
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
	/*00,8*/	Point2I blockPos;
	/*08,8*/	Point2F worldPos;
	/*10,4*/	real32 heading;
	/*14,4*/	void* objSrcData;
	/*18,4*/	LegoObject_Type objType;
	/*1c,4*/	sint32 objID;
	/*20,4*/	real32 health;
	/*24,4*/	char* thisOLName;
	/*28,4*/	char* drivingOLName;
	/*2c*/
};
assert_sizeof(HiddenObject, 0x2c);


struct LegoObject // [LegoRR/LegoObject.c|struct:0x40c|tags:LISTSET]
{
	/*000,4*/       LegoObject_Type type;
	/*004,4*/       sint32 id;
	/*008,4*/       char* customName; // max size is 11 (NOT null-terminated)
	/*00c,4*/       VehicleData* vehicle;
	/*010,4*/       CreatureData* miniFigure;
	/*014,4*/       CreatureData* rockMonster;
	/*018,4*/       BuildingData* building;
	/*01c,4*/       Gods98::Container* other;
	/*020,4*/       UpgradeData* upgrade;
	/*024,4*/       RoutingBlock* routeptr_24; // Unknown pointer, likely in large allocated table
	/*028,4*/       uint32 routingBlocksTotal; // total blocks to travel for current route
	/*02c,4*/       uint32 routingBlocksCurrent; // number of blocks traveled (up to routingBlocksTotal)
	/*030,25c*/		BezierCurve routingCurve; // BezierCurve/Catmull-rom spline data
	/*28c,c*/       Vector3F vector_28c;
	/*298,8*/       Point2F point_298;
	/*2a0,c*/       Vector3F vector_2a0;
	/*2ac,c*/       Vector3F faceDirection; // 1.0 to -1.0 directions that determine rotation with atan2
	/*2b8,4*/       real32 float_2b8;
	/*2bc,4*/       sint32 strafeSpeed_2bc;
	/*2c0,4*/       sint32 forwardSpeed_2c0;
	/*2c4,4*/       real32 rotateSpeed_2c4;
	/*2c8,4*/       undefined4 field_2c8;
	/*2cc,4*/       undefined4 field_2cc;
	/*2d0,4*/       undefined4 field_2d0;
	/*2d4,4*/       real32 float_2d4;
	/*2d8,4*/       bool32 unkbool_2d8;
	/*2dc,4*/       Gods98::Container* resData_2dc;
	/*2e0,4*/       sint32 index_2e0;
	/*2e4,4*/       Gods98::Container* resData_2e4;
	/*2e8,4*/       char* aitaskName1;
	/*2ec,4*/       char* aitaskName2;
	/*2f0,4*/       AITask* aitask_2f0;
	/*2f4,8*/       Point2F point_2f4; // (assigned -1.0f)
	/*2fc,4*/       LegoObject* object_2fc; // other half of object_300
	/*300,4*/       LegoObject* object_300; // other half of object_2fc
	/*304,4*/       LegoObject* carryingThisObject;
	/*308,1c*/      LegoObject* carriedObjects[7];
	/*324,4*/       uint32 numCarriedObjects;
	/*328,4*/       uint32 field_328;
	/*32c,4*/       Flocks* flocksData_32c;
	/*330,4*/       uint32 objLevel;
	/*334,4*/       ObjectStats* stats;
	/*338,4*/       real32 float_338;
	/*33c,4*/       real32 float_33c;
	/*340,4*/       real32 health; // (assigned -1.0f)
	/*344,4*/       real32 energy; // (assigned -1.0f)
	/*348,4*/       sint32* stealTableptr_348; // element size is 0x4
	/*34c,4*/       LOD_PolyLevel polyMode_34c;
	/*350,4*/       sint32 soundHandle_350;
	/*354,4*/       SFX_Type soundHandle_354; // (engine sound only?)
	/*358,4*/       undefined4 field_358;
	/*35c,4*/       undefined4 field_35c;
	/*360,4*/       undefined4 field_360;
	/*364,4*/       LegoObject* object_364;
	/*368,4*/       real32 float_368;
	/*36c,4*/       LegoObject* drivenObject; // same as drivingThisObject
	/*370,14*/      LegoObject_ToolType carriedTools[5];
	/*384,4*/       uint32 numCarriedTools;
	/*388,4*/       undefined4 field_388;
	/*38c,4*/       Gods98::Image* bubbleImage_38c;
	/*390,4*/       undefined4 teleporter_field_390;
	/*394,4*/       undefined4 teleporter_field_394;
	/*398,4*/       TeleporterService* teleporter_398;
	/*39c,4*/       undefined4 field_39c;
	/*3a0,4*/       undefined4 field_3a0;
	/*3a4,4*/       undefined4 field_3a4;
	/*3a8,4*/       undefined4 field_3a8;
	/*3ac,4*/       undefined4 field_3ac;
	/*3b0,4*/       undefined4 field_3b0;
	/*3b4,c*/       Vector3F vector_3b4;
	/*3c0,4*/       LegoObject* object_3c0;
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
	/*0000,80*/     LegoObject* listSet[32];
	/*0080,4*/      LegoObject* freeList;
	/*0084,4b0*/    SFX_Type objectTtSFX[LegoObject_Type_Count][LegoObject_ID_Count]; // [objType:20][objID:15]
	/*0534,13c*/    const char* activityName[Activity_Type_Count]; // [activityType:79]
	/*0670,4*/      void* UnkSurfaceGrid_1_TABLE;
	/*0674,4*/      void* UnkSurfaceGrid_2_TABLE;
	/*0678,4*/      uint32 UnkSurfaceGrid_COUNT;
	/*067c,4*/      real32 radarElapsed_67c;
	/*0680,4*/      uint32 listCount;
	/*0684,4*/      LegoObject_GlobFlags flags;
	/*0688,2c*/     sint32 toolNullIndex[LegoObject_ToolType_Count]; // [toolType:11] (tool priorities, behavior types?)
	/*06b4,4b00*/   uint32 objectTotalLevels[LegoObject_Type_Count][LegoObject_ID_Count][OBJECT_MAXLEVELS]; // [objType:20][objID:15][objLevel:16]
	/*51b4,4b00*/   uint32 objectPrevLevels[LegoObject_Type_Count][LegoObject_ID_Count][OBJECT_MAXLEVELS]; // [objType:20][objID:15][objLevel:16]
	/*9cb4,4*/      uint32 NERPs_TrainFlags;
	/*9cb8,4*/      LegoObject* minifigureObj_9cb8; // MINIFIGOBJ_004e9448
	/*9cbc,a0*/     Point2I slugHoleBlocks[20];
	/*9d5c,50*/     Point2I rechargeSeamBlocks[10];
	/*9dac,4*/      uint32 slugHoleCount;
	/*9db0,4*/      uint32 rechargeSeamCount;
	/*9db4,2260*/   HiddenObject hiddenObjects[200];
	/*c014,4*/      uint32 hiddenObjectCount;
	/*c018,4*/      real32 float_c018; // FLOAT_004eb7a8
	/*c01c,18*/     Dummy::SaveStruct_18 savestruct18_c01c;
	/*c034,400*/    LegoObject* liveObjArray256_c034[256]; // PTRLiveObject_ARRAY_004eb7c4
	/*c434,4*/      uint32 count_c434; // COUNT_004ebbc4
	/*c438,4*/      uint32 countBuildingsOnly_c438; // COUNTBuildingsOnly_004ebbc8
	/*c43c,190*/    LegoObject* liveObjArray100_c43c[100]; // Used for water docking vehicles?
	/*c5cc,4*/      uint32 uintCount_c5cc;
	/*c5d0,18*/     const char* abilityName[LegoObject_AbilityType_Count]; // [abilityType:6]
	/*c5e8,18*/     Gods98::Image* ToolTipIcons_Abilities[LegoObject_AbilityType_Count]; // [abilityType:6]
	/*c600,2c*/     Gods98::Image* ToolTipIcons_Tools_TABLE[LegoObject_ToolType_Count]; // [toolType:11]
	/*c62c,4*/      Gods98::Image* ToolTipIcon_Blank;
	/*c630,4*/      Gods98::Image* ToolTipIcon_Ore;
	/*c634,4*/      uint32 BuildingsTeleported;
	/*c638,4*/      real32 LiveManager_TimerUnk;
	/*c63c,4*/      undefined4 s_stepCounter_c63c; // (static, counter %4 for step SFX) DAT_004ebdcc
	/*c640,4*/      void** s_FlocksDestroy_c640; // (static, Struct 0x10, used in Flocks activities (QUICK_DESTROY??)) PTR_004ebdd0
	/*c644*/
};
assert_sizeof(LegoObject_Globs, 0xc644);

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @004df790>
extern LegoObject_Globs & objectGlobs;

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

// <LegoRR.exe @00436ee0>
void __cdecl LegoObject_Initialise(void);

// <LegoRR.exe @00437310>
void __cdecl LegoObject_Shutdown(void);

// <LegoRR.exe @0044a330>
void __cdecl LiveObject_FP_GetPositionAndHeading(LegoObject* liveObj, int cameraFrame, OUT Vector3F* position, OPTIONAL OUT Vector3F* dir);
#define LiveObject_FP_GetPositionAndHeading ((decltype(LiveObject_FP_GetPositionAndHeading)*)0x0044a330)

// <LegoRR.exe @0044a4c0>
Gods98::Container* __cdecl LiveObject_GetActivityContainer(LegoObject* liveObj);
#define LiveObject_GetActivityContainer ((decltype(LiveObject_GetActivityContainer)*)0x0044a4c0)

#pragma endregion

}
