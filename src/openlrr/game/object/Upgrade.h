// Upgrade.h : 
//

#pragma once

#include "../../common.h"
#include "../../engine/colour.h"
#include "../../engine/geometry.h"
#include "../../engine/undefined.h"

#include "../../engine/gfx/Containers.h"
#include "../../engine/drawing/Images.h"

#include "../GameCommon.h"


namespace LegoRR
{; // !<---

/**********************************************************************************
 ******** Forward Declarations
 **********************************************************************************/

#pragma region Forward Declarations

struct LegoObject;
struct Upgrade_PartInfo;
struct Upgrade_PartModel;
struct UpgradesModel;

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

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

struct Upgrade_PartInfo // [LegoRR/Upgrade.c|struct:0x14]
{
	/*00,4*/	uint32 level;			// (ae: key Level%i%i%i%i)
	/*04,4*/	uint32 nullFrameNo;		// (ae: value[1])
	/*08,4*/	char* nullObjectName;	// (ae: value[0])
	/*0c,4*/	char* weaponName;		// (ae: value[2], optional)
	/*10,4*/	Upgrade_PartModel* upgradeData;
	/*14*/
};
assert_sizeof(Upgrade_PartInfo, 0x14);


struct Upgrade_PartModel // [LegoRR/Upgrade.c|struct:0x10]
{
	/*00,4*/	LegoObject_ID objID;
	/*04,4*/	Gods98::Container* cont; // (LWO|ACT|MESH, true)
	/*08,4*/	LegoObject* nextObject;
	/*0c,4*/	Upgrade_PartInfo* partInfo; // (not too sure about this type match)
	/*10*/
};
assert_sizeof(Upgrade_PartModel, 0x10);


struct UpgradesModel // [LegoRR/Upgrade.c|struct:0x14]
{
	/*00,4*/	Upgrade_PartInfo* parts; // always ObjectUpgradePartData[200]
	/*04,4*/	uint32 partCount;
	/*08,4*/	uint32 currentLevel;
	/*0c,4*/	LegoObject_UpgradeFlags levelFlags; // [carry][scan][speed][drill]
	/*10,4*/	LegoObject* firstObject;
	/*14*/
};
assert_sizeof(UpgradesModel, 0x14);

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

 // Merged function: Object_Hide
 // <LegoRR.exe @00406bc0>
#define Upgrade_Part_Hide ((void (__cdecl* )(Upgrade_PartModel* upgradePart, bool32 hide))0x00406bc0)

// Merged function: Object_GetActivityContainer
// <LegoRR.exe @00406d60>
#define Upgrade_Part_GetActivityContainer ((Gods98::Container* (__cdecl* )(Upgrade_PartModel* upgradePart))0x00406d60)

// Not used by Upgrade_Part_*, but should be supported.
// Merged function: Object_SearchForPartName
// <LegoRR.exe @00406e80>
#define Upgrade_Part_SearchForPartName ((Gods98::Container* (__cdecl* )(Upgrade_PartModel* upgradePart, const char* name, uint32 frameNo))0x00406e80)

// Not used by Upgrade_Part_*, but should be supported.
// Merged function: Object_SetOwnerObject
// <LegoRR.exe @004082b0>
#define Upgrade_Part_SetOwnerObject ((void (__cdecl* )(Upgrade_PartModel* upgradePart, LegoObject* liveObj))0x004082b0)

// Merged function: Object_IsHidden
// <LegoRR.exe @004085d0>
#define Upgrade_Part_IsHidden ((bool32 (__cdecl* )(Upgrade_PartModel* upgradePart))0x004085d0)




// <LegoRR.exe @0046c2f0>
#define Upgrade_Part_Load ((void (__cdecl* )(OUT Upgrade_PartModel* upgradePart, LegoObject_ID objID, Gods98::Container* root, const char* filename))0x0046c2f0)

// <LegoRR.exe @0046c370>
#define Upgrade_Part_Clone ((void (__cdecl* )(IN Upgrade_PartModel* srcUpgradePart, OUT Upgrade_PartModel* destUpgradePart))0x0046c370)

// <LegoRR.exe @0046c3b0>
#define Upgrade_Part_Remove ((void (__cdecl* )(Upgrade_PartModel* upgradePart))0x0046c3b0)

// <LegoRR.exe @0046c3d0>
#define Upgrade_Part_MoveAnimation ((real32 (__cdecl* )(Upgrade_PartModel* upgradePart, real32 elapsed, uint32 unused_unkFrameNo))0x0046c3d0)

// <LegoRR.exe @0046c3f0>
#define Upgrade_Part_SetActivity ((bool32 (__cdecl* )(Upgrade_PartModel* upgradePart, const char* activityName))0x0046c3f0)

// <LegoRR.exe @0046c420>
#define Upgrade_Load ((void (__cdecl* )(UpgradesModel* upgrades, const Gods98::Config* act, const char* gameName))0x0046c420)

// <LegoRR.exe @0046c600>
#define Upgrade_SetUpgradeLevel ((void (__cdecl* )(UpgradesModel* upgrades, uint32 objLevel))0x0046c600)

#pragma endregion

}
