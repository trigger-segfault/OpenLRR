// Teleporter.cpp : 
//

#include "Teleporter.h"
#include "..\object\Object.h"
#include "..\Game.h"
#include "..\..\..\openlrr\engine\core\Maths.h"


/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @004ab450>
LegoRR::Teleporter_Globs & LegoRR::teleporterGlobs = *(LegoRR::Teleporter_Globs*)0x004ab450;

// <LegoRR.exe @00504188>
bool32 & LegoRR::g_Teleporter_BOOL_00504188 = *(bool32*)0x00504188;

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @0046a630>
void __cdecl LegoRR::Teleporter_RemoveAll(TeleporterService* teleporter)
{
	// -FC iterate through list and free memory
	if (teleporter != nullptr) {
		Teleporter_RemoveAll(teleporter->next);
		Gods98::Mem_Free(teleporter);
	}
}

// <LegoRR.exe @0046a650>
void __cdecl LegoRR::Teleporter_Restart(void)
{
	teleporterGlobs.count = 0;
	Teleporter_RemoveAll(teleporterGlobs.current);
	teleporterGlobs.current = nullptr;
}

// <LegoRR.exe @0046a680>
bool32 __cdecl LegoRR::Teleporter_LiveObjectCallback_Service(LegoObject* liveObj, void* search)
{
	// -FC, make local pointer here to make this function useable as a callback
	SearchTeleporter_10* locSearch = (SearchTeleporter_10*)search;

	if (liveObj->type == locSearch->objType) {
		liveObj->teleporter_modeFlags = locSearch->modeFlags;
		liveObj->teleporter_teleportFlags = locSearch->teleportFlags;
		if (locSearch->modeFlags & 2) {
			liveObj->teleporter = locSearch->teleporter;
			locSearch->teleporter->count += 1;
		}
		teleporterGlobs.count = teleporterGlobs.count + 1;
		liveObj->flags4 = liveObj->flags4 | LIVEOBJ4_UNK_8; // -FC, TODO: name this flag
	}
	return 0;
}

/* This function is weird, it returns EDX:EAX, but what for?? */

// <LegoRR.exe @0046a6e0>
Point2F __cdecl LegoRR::Teleporter_GetCameraPosition(void)
{
	Point2F PVar1;
	Map3D* surfMap;
	LegoCamera* cam;
	Vector3F* out_worldPos;
	Point2F local_14;
	Vector3F surfPosition;

	cam = legoGlobs.cameraMain;
	if (legoGlobs.viewMode != ViewMode_Top) {
		cam = legoGlobs.cameraFP;
	}
	out_worldPos = &surfPosition;
	surfMap = Lego_GetMap();
	Camera_GetTopdownWorldPos(cam, surfMap, out_worldPos);
	PVar1.y = surfPosition.y;
	PVar1.x = surfPosition.x;
	return PVar1;
}

// <LegoRR.exe @0046a730>
void __cdecl LegoRR::Teleporter_Add(TeleporterService* teleporter)
{
	teleporter->next =teleporterGlobs.current;
	teleporterGlobs.current = teleporter;
}

// <LegoRR.exe @0046a750>
bool32 __cdecl LegoRR::Teleporter_LiveObjectCallback_Unk(LegoObject* liveObj, void* teleportObjType)
{
	LegoObject_Type objType;
	// -FC, make local typeflags object for function passthrough
	LegoObject_TypeFlags *locTypeFlags = (LegoObject_TypeFlags*)teleportObjType;

	objType = Teleporter_GetServiceObjectType(*locTypeFlags);
	if (liveObj->type == objType && liveObj->teleporter_modeFlags) {
		g_Teleporter_BOOL_00504188 = FALSE;
	}
	return FALSE;
}

// <LegoRR.exe @0046a780>
bool32 __cdecl LegoRR::Teleporter_ServiceAll(LegoObject_TypeFlags teleportObjTypes)
{
	TeleportObjectType teleObjFlag;
	uint32 i;

	i = 0;
	do {
		teleObjFlag = (TeleportObjectType)(1 << ((byte)i & 0x1f));
		g_Teleporter_BOOL_00504188 = TRUE;
		if ((teleportObjTypes & teleObjFlag) != TELEPORT_SERVIVE_NONE) {
			LegoObject_RunThroughListsSkipUpgradeParts(Teleporter_LiveObjectCallback_Unk, (void*)teleObjFlag);
			if (g_Teleporter_BOOL_00504188 == 0) {
				return FALSE;
			}
		}
		i = i + 1;
	} while (i < 31);
	return TRUE;
}


// <LegoRR.exe @0046a7d0>
void __cdecl LegoRR::Teleporter_Start(LegoObject_TypeFlags teleportObjType, uint32 modeFlags, uint32 teleportFlags)
{
	Point2F cameraPos;
	SearchTeleporter_10 search;

	search.objType = Teleporter_GetServiceObjectType(teleportObjType);
	search.modeFlags = modeFlags;
	search.teleportFlags = teleportFlags;
	if (modeFlags & 2) {
		search.teleporter = (TeleporterService*)Gods98::Mem_Alloc(0x20);
		Teleporter_Add(search.teleporter);
		search.teleporter->count = 0;
		if (teleportFlags & 2) {
			cameraPos = Teleporter_GetCameraPosition();
			search.teleporter->cameraPos = cameraPos;
			search.teleporter->float_8 = 0.0;
			search.teleporter->float_c = teleporterGlobs.floatValue_3_0;
		}
		if (teleportFlags & 1) {
			search.teleporter->int_14 = teleporterGlobs.intValue_40;
		}
		search.teleporter->flags = teleportFlags;
	}
	LegoObject_RunThroughListsSkipUpgradeParts(Teleporter_LiveObjectCallback_Service, &search);
	return;
}

// <LegoRR.exe @0046a880>
bool32 __cdecl LegoRR::Teleporter_LiveObjectCallback_Update(LegoObject* liveObj, void* data_unused)
{
	uint32 uVar1;
	float fVar2;
	float fVar3;
	short rng;
	Point2F position;
	TeleporterService* teleporter;

	if (liveObj->teleporter_modeFlags & 1) {
		liveObj->health = -1.0;
		liveObj->teleporter_teleportFlags = 0;
		liveObj->teleporter_modeFlags = 4;
		teleporterGlobs.count = teleporterGlobs.count - 1;
		return FALSE;
	}
	if (liveObj->teleporter_modeFlags & 2) {
		uVar1 = liveObj->teleporter_teleportFlags;
		if ((uVar1 & 2) == 0) {
			if ((uVar1 & 1) == 0) {
				if ((uVar1 & 4) != 0) {
					liveObj->health = -1.0;
					liveObj->flags3 = liveObj->flags3 | LIVEOBJ3_REMOVING;
					teleporterGlobs.count = teleporterGlobs.count - 1;
				}
			}
			else {
				teleporter = liveObj->teleporter;
				rng = Gods98::Maths_Rand();
				if ((int)rng % teleporter->int_14 == 0) {
					liveObj->health = -1.0;
					teleporter->count = teleporter->count - 1;
					goto LAB_0046a960;
				}
			}
		}
		else {
			teleporter = liveObj->teleporter;
			/* I think this teleports objects with priority to "near" the camera
				when the level completed, as time progresses, units farther away
				are teleported. */
			LegoObject_GetPosition(liveObj, &position.x, &position.y);
			fVar3 = position.x - (teleporter->cameraPos).x;
			fVar2 = position.y - (teleporter->cameraPos).y;
			teleporter = liveObj->teleporter;
			if (sqrt(fVar2 * fVar2 + fVar3 * fVar3) < teleporter->float_8) {
				liveObj->health = -1.0;
				teleporter->count = teleporter->count - 1;
			LAB_0046a960:
				liveObj->teleporter_modeFlags = 4;
				liveObj->teleporter = nullptr;
				liveObj->teleporter_teleportFlags = 0;
				teleporterGlobs.count = teleporterGlobs.count - 1;
				return FALSE;
			}
		}
	}
	return FALSE;
}

// <LegoRR.exe @0046a9c0>
bool32 __cdecl LegoRR::Teleporter_UpdateService(TeleporterService* teleporter, real32 elapsedGame)
{
	if (teleporter != nullptr) {
		do {
			if ((teleporter->count) && (teleporter->flags == 2)) {
				teleporter->float_8 = teleporter->float_c * elapsedGame + teleporter->float_8;
			}
			teleporter = teleporter->next;
		} while (teleporter != nullptr);
	}
	return (bool32)teleporter;
}

// <LegoRR.exe @0046a9f0>
void __cdecl LegoRR::Teleporter_Update(real32 elapsedGame)
{
	Teleporter_UpdateService(teleporterGlobs.current, elapsedGame);
	if (teleporterGlobs.count) {
		LegoObject_RunThroughListsSkipUpgradeParts(Teleporter_LiveObjectCallback_Update, nullptr);
	}
}

// <LegoRR.exe @0046aa20>
LegoRR::LegoObject_Type __cdecl LegoRR::Teleporter_GetServiceObjectType(LegoObject_TypeFlags teleportObjType)
{
	if ((teleportObjType & TELEPORT_SERVIVE_VEHICLE) != TELEPORT_SERVIVE_NONE) {
		return LegoObject_Vehicle;
	}
	if ((teleportObjType & TELEPORT_SERVIVE_MINIFIGURE) != TELEPORT_SERVIVE_NONE) {
		return LegoObject_MiniFigure;
	}
	if ((teleportObjType & TELEPORT_SERVIVE_ROCKMONSTER) != TELEPORT_SERVIVE_NONE) {
		return LegoObject_RockMonster;
	}
	if ((teleportObjType & TELEPORT_SERVIVE_BUILDING) != TELEPORT_SERVIVE_NONE) {
		return LegoObject_Building;
	}
	if ((teleportObjType & TELEPORT_SERVIVE_BOULDER) != TELEPORT_SERVIVE_NONE) {
		return LegoObject_Boulder;
	}
	if ((teleportObjType & TELEPORT_SERVIVE_POWERCRYSTAL) != TELEPORT_SERVIVE_NONE) {
		return LegoObject_PowerCrystal;
	}
	if ((teleportObjType & TELEPORT_SERVIVE_ORE) != TELEPORT_SERVIVE_NONE) {
		return LegoObject_Ore;
	}
	if ((teleportObjType & TELEPORT_SERVIVE_DYNAMITE) != TELEPORT_SERVIVE_NONE) {
		return LegoObject_Dynamite;
	}
	if ((teleportObjType & TELEPORT_SERVIVE_BARRIER) != TELEPORT_SERVIVE_NONE) {
		return LegoObject_Barrier;
	}
	if ((teleportObjType & TELEPORT_SERVIVE_UPGRADEPART) != TELEPORT_SERVIVE_NONE) {
		return LegoObject_UpgradePart;
	}
	if ((teleportObjType & TELEPORT_SERVIVE_ELECTRICFENCE) != TELEPORT_SERVIVE_NONE) {
		return LegoObject_ElectricFence;
	}
	if ((teleportObjType & TELEPORT_SERVIVE_SPIDERWEB) != TELEPORT_SERVIVE_NONE) {
		return LegoObject_SpiderWeb;
	}
	/* OBJECT_OOHSCARY */
	if ((teleportObjType & TELEPORT_SERVIVE_OOHSCARY) != TELEPORT_SERVIVE_NONE) {
		return LegoObject_OohScary;
	}

	// -FC, if we didnt find an object return none found
	return LegoObject_None;
}


#pragma endregion
