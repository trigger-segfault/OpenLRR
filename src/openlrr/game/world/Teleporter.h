// Teleporter.h : 
//

#pragma once

#include "../GameCommon.h"


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

#pragma endregion

/**********************************************************************************
 ******** Enumerations
 **********************************************************************************/

#pragma region Enums

enum TeleportObjectType : uint32 // [LegoRR/Teleporter.c|flags:0x4|type:uint|tags:FLAGASENUM]
{
	TELEPORT_SERVIVE_NONE          = 0,
	TELEPORT_SERVIVE_VEHICLE       = 0x1,
	TELEPORT_SERVIVE_MINIFIGURE    = 0x2,
	TELEPORT_SERVIVE_ROCKMONSTER   = 0x4,
	TELEPORT_SERVIVE_BUILDING      = 0x8,
	TELEPORT_SERVIVE_BOULDER       = 0x10,
	TELEPORT_SERVIVE_POWERCRYSTAL  = 0x20,
	TELEPORT_SERVIVE_ORE           = 0x40,
	TELEPORT_SERVIVE_DYNAMITE      = 0x80,
	TELEPORT_SERVIVE_BARRIER       = 0x100,
	TELEPORT_SERVIVE_UPGRADEPART   = 0x200,
	TELEPORT_SERVIVE_ELECTRICFENCE = 0x400,
	TELEPORT_SERVIVE_SPIDERWEB     = 0x800,
	TELEPORT_SERVIVE_OOHSCARY      = 0x1000,
};
flags_end(TeleportObjectType, 0x4);

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

struct SearchTeleporter_10 // [LegoRR/search.c|struct:0x10] Teleporter_Creat_FUN_0046a7d0
{
	/*00,4*/	LegoObject_Type objType;
	/*04,4*/	uint32 modeFlags;
	/*08,4*/	uint32 teleportFlags;
	/*0c,4*/	TeleporterService* teleporter;
	/*10*/
};
assert_sizeof(SearchTeleporter_10, 0x10);


struct TeleporterService // [LegoRR/Teleporter.c|struct:0x20]
{
	/*00,8*/	Point2F cameraPos; // (init: if flags 0x2)
	/*08,4*/	real32 float_8; // (init: 0 if flags 0x2)
	/*0c,4*/	real32 float_c; // (init: 3.0 if flags 0x2)
	/*10,4*/	uint32 count; // (init: 0)
	/*14,4*/	sint32 int_14; // (init: 40 if flags 0x1)
	/*18,4*/	uint32 flags; // (init: flags)
	/*1c,4*/	TeleporterService* next;
	/*20*/
};
assert_sizeof(TeleporterService, 0x20);


struct Teleporter_Globs // [LegoRR/Teleporter.c|struct:0x10|tags:GLOBS]
{
	/*00,4*/	uint32 count;
	/*04,4*/	sint32 intValue_40; // (const: 40)
	/*08,4*/	TeleporterService* current;
	/*0c,4*/	real32 floatValue_3_0; // (const: 3.0)
	/*10*/
};
assert_sizeof(Teleporter_Globs, 0x10);

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @004ab450>
extern Teleporter_Globs & teleporterGlobs;

// <LegoRR.exe @00504188>
extern bool32 & g_Teleporter_BOOL_00504188;

#pragma endregion

/**********************************************************************************
 ******** Macros
 **********************************************************************************/

#pragma region Macros

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// Previously named Teleporter_Free (old name conflict with "Teleporter_RemoveAll", now ResetAll).
// <LegoRR.exe @0046a630>
#define Teleporter_RemoveAll ((void (__cdecl* )(TeleporterService* teleporter))0x0046a630)

// Previously named Teleporter_RemoveAll (old name conflict with "Teleporter_Free", now RemoveAll).
// <LegoRR.exe @0046a650>
#define Teleporter_ResetAll ((void (__cdecl* )(void))0x0046a650)

// DATA: SearchTeleporter_10*
// <LegoRR.exe @0046a680>
#define Teleporter_LiveObjectCallback_Service ((bool32 (__cdecl* )(LegoObject* liveObj, void* search))0x0046a680)

// Point2F is returned BY VALUE here.
// <LegoRR.exe @0046a6e0>
#define Teleporter_GetCameraPosition ((Point2F (__cdecl* )(void))0x0046a6e0)

// <LegoRR.exe @0046a730>
#define Teleporter_Add ((void (__cdecl* )(TeleporterService* teleporter))0x0046a730)

// DATA: TeleportObjectType (pointer as value)
// <LegoRR.exe @0046a750>
#define Teleporter_LiveObjectCallback_Unk ((bool32 (__cdecl* )(LegoObject* liveObj, void* teleportObjType))0x0046a750)

// <LegoRR.exe @0046a780>
#define Teleporter_ServiceAll ((bool32 (__cdecl* )(TeleportObjectType teleportObjTypes))0x0046a780)

// <LegoRR.exe @0046a7d0>
#define Teleporter_Start ((void (__cdecl* )(TeleportObjectType teleportObjType, uint32 modeFlags, uint32 teleportFlags))0x0046a7d0)

// <LegoRR.exe @0046a880>
#define Teleporter_LiveObjectCallback_Update ((bool32 (__cdecl* )(LegoObject* liveObj, void* data_unused))0x0046a880)

// <LegoRR.exe @0046a9c0>
#define Teleporter_UpdateService ((bool32 (__cdecl* )(TeleporterService* teleporter, real32 elapsedGame))0x0046a9c0)

// <LegoRR.exe @0046a9f0>
#define Teleporter_Update ((void (__cdecl* )(real32 elapsedGame))0x0046a9f0)

// <LegoRR.exe @0046aa20>
#define Teleporter_GetServiceObjectType ((LegoObject_Type (__cdecl* )(TeleportObjectType teleportObjType))0x0046aa20)

#pragma endregion

}
