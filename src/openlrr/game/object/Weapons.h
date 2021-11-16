// Weapons.h : 
//

#pragma once

#include "../../common.h"
#include "../../engine/colour.h"
#include "../../engine/geometry.h"
#include "../../engine/undefined.h"

#include "../../engine/gfx/Containers.h"
#include "../../engine/drawing/Images.h"

#include "../GameCommon.h"
#include "BezierCurve.h"


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

#define WEAPON_MAXWEAPONS			3
#define WEAPON_MAXFIRES				2

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

struct WeaponsModel // [LegoRR/Weapons.c|struct:0xa8]
{
	/*00,18*/	Gods98::Container* fireNullPairs[WEAPON_MAXWEAPONS][WEAPON_MAXFIRES];
	/*18,c*/	Gods98::Container* xPivotNulls[WEAPON_MAXWEAPONS];
	/*24,c*/	Gods98::Container* yPivotNulls[WEAPON_MAXWEAPONS];
	/*30,4*/	real32 pivotMaxZ;			// (ae: PivotMaxZ)
	/*34,4*/	real32 pivotMinZ;			// (ae: PivotMinZ)
	/*38,c*/	bool32 fireNullPairFrames[WEAPON_MAXWEAPONS];	// (valid: [0,1])
	/*44,48*/	undefined reserved3[0x48];
	/*8c,c*/	Upgrade_PartInfo* weaponParts[WEAPON_MAXWEAPONS];
	/*98,c*/	undefined reserved4[0xc];
	/*a4,4*/	uint32 weaponCount;			// (valid: [0,3])
	/*a8*/
};
assert_sizeof(WeaponsModel, 0xa8);


struct WeaponStats // [LegoRR/Weapons.c|struct:0x4b68]
{
	/*0000,4b00*/	real32 ObjectRatios[20][15][16]; // (inits: -1.0f)
	/*4b00,4*/	bool32 isSlowDeath; // (init: false)
	/*4b04,4*/	real32 SlowDeath_initial; // Initial damage percent
	/*4b08,4*/	real32 SlowDeath_duration;
	/*4b0c,4*/	real32 RechargeTime; // (init: 0.0f)
	/*4b10,4*/	real32 DefaultDamage; // (init: 0.0f)
	/*4b14,4*/	real32 DischargeRate;
	/*4b18,4*/	sint32 Ammo;
	/*4b1c,4*/	real32 WeaponRange; // (init: 150.0f)
	/*4b20,48*/	real32 WallDestroyTimes[18]; // (defaults: 5.0f)
	/*4b68*/
};
assert_sizeof(WeaponStats, 0x4b68);


struct ItemStruct_34 // [LegoRR/???|struct:0x10]
{
	/*00,4*/	Gods98::Mesh* mesh1;
	/*04,4*/	Gods98::Mesh* mesh2;
	/*08,4*/	Gods98::Container* cont;
	/*0c,4*/	real32 time;
	/*10*/
};
assert_sizeof(ItemStruct_34, 0x10);


struct Struct_2b0 // [LegoRR/Weapons.c|struct:0x2b0] Seen in a table of [10]. This is likely an extension of the Weapons module.
{
	/*000,4*/	undefined4 unseen_0;
	/*004,25c*/	BezierCurve routing;
	/*260,4*/	undefined4 unseen_260;
	/*264,4*/	real32 float_264;
	/*268,c*/	Vector3F vector_268;
	/*274,c*/	Vector3F vector_274;
	/*280,c*/	Vector3F position_280;
	/*28c,4*/	real32 float_28c;
	/*290,c*/	Vector3F vector_290;
	/*29c,4*/	undefined4 field_29c;
	/*2a0,4*/	undefined4 field_2a0;
	/*2a4,4*/	LegoObject* object_2a4;
	/*2a8,4*/	sint32 weaponType;
	/*2ac,4*/	undefined4 field_2ac;
	/*2b0*/
};
assert_sizeof(Struct_2b0, 0x2b0);


struct Weapon_Globs // [LegoRR/Weapons.c|struct:0x1b90|tags:GLOBS]
{
	/*0000,4*/	uint32 weaponCount;
	/*0004,4*/	char** weaponNameList;
	/*0008,4*/	WeaponStats* weaponStatsList;
	/*000c,a0*/	ItemStruct_34 ItemStruct34Unk_TABLE[10];
	/*00ac,1ae0*/	Struct_2b0 Struct2B0Unk_TABLE[10];
	/*1b8c,4*/	Gods98::Config* config;
	/*1b90*/
};
assert_sizeof(Weapon_Globs, 0x1b90);

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals


#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @0046ee40>
#define Weapon_Initialise ((bool32 (__cdecl* )(const Gods98::Config* config, const char* gameName))0x0046ee40)

// <LegoRR.exe @0046f390>
#define Weapon_GetWeaponTypeByName ((uint32 (__cdecl* )(const char* weaponName))0x0046f390)

// <LegoRR.exe @0046f3d0>
#define Weapon_GetRechargeTime ((real32 (__cdecl* )(sint32 weaponType))0x0046f3d0)

// <LegoRR.exe @0046f400>
#define Weapon_GetDischargeRate ((real32 (__cdecl* )(sint32 weaponType))0x0046f400)

// <LegoRR.exe @0046f430>
#define Weapon_GetWeaponRange ((real32 (__cdecl* )(sint32 weaponType))0x0046f430)

// <LegoRR.exe @0046f460>
#define Weapon_GetWallDestroyTime ((real32 (__cdecl* )(sint32 weaponType, Lego_SurfaceType surfaceType))0x0046f460)

// <LegoRR.exe @0046f490>
#define Weapon_GetDamageForLegoObject ((real32 (__cdecl* )(sint32 weaponType, LegoObject* liveObj))0x0046f490)

// <LegoRR.exe @0046f530>
#define Weapon_LegoObject_FUN_0046f530 ((void (__cdecl* )(LegoObject* liveObj, sint32 weaponType, bool32 param_3, real32 param_4, Point2F* param_5))0x0046f530)

// <LegoRR.exe @0046f640>
#define Weapon_LegoObject_FUN_0046f640 ((void (__cdecl* )(LegoObject* liveObj, real32 param_2, sint32 param_3))0x0046f640)

// <LegoRR.exe @0046f670>
#define Weapon_Struct2B0_FUN_0046f670 ((void (__cdecl* )(Struct_2b0* param_1))0x0046f670)

// <LegoRR.exe @0046f810>
#define Weapon_Struct2B0_Update_Table_FUN_0046f810 ((void (__cdecl* )(real32 elapsedGame))0x0046f810)

// <LegoRR.exe @0046f8d0>
#define Weapon_LegoObject_Callback_FUN_0046f8d0 ((bool32 (__cdecl* )(LegoObject* liveObj, real32* lpElapsed))0x0046f8d0)

// <LegoRR.exe @0046fa30>
#define Weapon_LegoObject_FUN_0046fa30 ((void (__cdecl* )(LegoObject* liveObj, sint32** param_2, Vector3F* param_3, sint32* param_4, sint32 param_5))0x0046fa30)

// <LegoRR.exe @0046fbe0>
#define Weapon_Struct2B0_FUN_0046fbe0 ((void (__cdecl* )(Struct_2b0* param_1, real32 elapsed))0x0046fbe0)

// <LegoRR.exe @0046fdb0>
#define Weapon_Struct2B0_FUN_0046fdb0 ((bool32 (__cdecl* )(LegoObject* liveObj, Vector3F* param_2, Vector3F* param_3, uint32 weaponType, WeaponKnownType param_5))0x0046fdb0)

// <LegoRR.exe @0046ff30>
#define Weapon_UpdateRoutingUnk_FUN_0046ff30 ((bool32 (__cdecl* )(undefined4 param_1, Vector3F* param_2, Vector3F* param_3, sint32 param_4, WeaponKnownType knownWeapon))0x0046ff30)

// <LegoRR.exe @004701b0>
#define Weapon_GetObjectTypeAndID_ByKnownWeaponType ((void (__cdecl* )(WeaponKnownType knownWeapon, OUT LegoObject_Type* objType, OUT LegoObject_ID* objID))0x004701b0)

// <LegoRR.exe @00470230>
#define Weapon_Struct2B0_GetNextAvailable ((sint32 (__cdecl* )(void))0x00470230)

// <LegoRR.exe @00470250>
#define Weapon_ItemStruct34_GetNextAvailable ((sint32 (__cdecl* )(void))0x00470250)

// <LegoRR.exe @00470270>
#define Weapon_DoLaserUnk_FUN_00470270 ((LegoObject* (__cdecl* )(Vector3F* sndPosition, Vector3F* param_2, LegoObject* in_liveObj, real32 param_4, real32 param_5, sint32 weaponType, real32 param_7))0x00470270)

// <LegoRR.exe @00470520>
#define Weapon_LegoObject_Collision_FUN_00470520 ((bool32 (__cdecl* )(LegoObject* liveObj, Point2F* param_2, Point2F* param_3, bool32 param_4))0x00470520)

// <LegoRR.exe @00470570>
#define Weapon_LegoObject_CollisionBox_FUN_00470570 ((bool32 (__cdecl* )(LegoObject* liveObj, Point2F* param_2, Point2F* param_3, Point2F* optout_point, bool32 param_5))0x00470570)

// <LegoRR.exe @00470800>
#define Weapon_LegoObject_CollisionRadius_FUN_00470800 ((bool32 (__cdecl* )(LegoObject* liveObj, Point2F* param_2, Point2F* param_3, bool32 param_4))0x00470800)

// <LegoRR.exe @004708f0>
#define Weapon_LegoObject_TestCollision_FUN_004708f0 ((bool32 (__cdecl* )(LegoObject* liveObj, const Point2F* fromPos, const Point2F* scalar))0x004708f0)

// <LegoRR.exe @00470950>
#define Weapon_ItemStruct34_FUN_00470950 ((void (__cdecl* )(Container* resData, Vector3F* vector1, Vector3F* vector2))0x00470950)

// <LegoRR.exe @00470a20>
#define Weapon_Struct34_FUN_00470a20 ((void (__cdecl* )(Mesh* param_1, real32 scalar, Vector3F* vector1, Vector3F* vector2, real32 param_5, real32 param_6, real32 param_7, real32 param_8))0x00470a20)

// <LegoRR.exe @00471580>
#define Weapon_LegoObject_GetWeaponModel ((void* (__cdecl* )(LegoObject* liveObj))0x00471580)

// <LegoRR.exe @004715b0>
#define Weapon_LegoObject_GetWeaponTimer ((real32 (__cdecl* )(LegoObject* liveObj, uint32 frameNo))0x004715b0)

// <LegoRR.exe @004715d0>
#define Weapon_LegoObject_SetWeaponTimer ((void (__cdecl* )(LegoObject* liveObj, real32 timer, uint32 frameNo))0x004715d0)

// <LegoRR.exe @004715f0>
#define Weapon_MathUnk_CheckVectorsZScalar_InRange ((bool32 (__cdecl* )(const Vector3F* vectorPos, const Vector3F* vectorScaled, real32 minZ, real32 maxZ, real32 scalar))0x004715f0)

// <LegoRR.exe @00471630>
#define Weapon_LegoObject_Callback_FUN_00471630 ((bool32 (__cdecl* )(LegoObject* liveObj, SearchData18_2* search))0x00471630)

// <LegoRR.exe @004718f0>
#define Weapon_LegoObject_FUN_004718f0 ((void (__cdecl* )(SearchData18_2* search))0x004718f0)

// <LegoRR.exe @00471b20>
#define Weapon_DoCallbacksSearch_FUN_00471b20 ((bool32 (__cdecl* )(Vector3F* vecCector, Vector3F* vecDistance, LegoObject** out_liveObj, real32* ref_float_4, undefined4 param_5))0x00471b20)

// <LegoRR.exe @00471b90>
#define Weapon_LegoObject_DoCallbacksSearch_FUN_00471b90 ((bool32 (__cdecl* )(LegoObject* liveObj1, LegoObject* liveObj2))0x00471b90)

// <LegoRR.exe @00471c20>
#define Weapon_LegoObject_SeeThroughWalls_FUN_00471c20 ((bool32 (__cdecl* )(LegoObject* liveObj, LegoObject* liveObj2))0x00471c20)

// <LegoRR.exe @00471ce0>
#define Weapon_GetFireNull ((Gods98::Container* (__cdecl* )(WeaponsModel* weapon, uint32 frameNo, uint32 pairNo))0x00471ce0)

// <LegoRR.exe @00471d00>
#define Weapon_GetXPivotNull ((Gods98::Container* (__cdecl* )(WeaponsModel* weapon, uint32 frameNo2))0x00471d00)

// <LegoRR.exe @00471d10>
#define Weapon_GetYPivotNull ((Gods98::Container* (__cdecl* )(WeaponsModel* weapon, uint32 frameNo))0x00471d10)

// <LegoRR.exe @00471d20>
#define Weapon_UnkStruct_FUN_00471d20 ((void (__cdecl* )(WeaponsModel* weapon, const Vector3F* vector68, real32 param_3, bool32 assignVectors68, uint32 frameNo))0x00471d20)

// <LegoRR.exe @00471f30>
#define Weapon_GetVectors44 ((void (__cdecl* )(WeaponsModel* weapon, OUT Vector3F* vector44, uint32 frameNo))0x00471f30)

// <LegoRR.exe @00471f60>
#define Weapon_LegoObject_GetCollCenterPosition ((bool32 (__cdecl* )(LegoObject* liveObj, OUT Vector3F* vector))0x00471f60)

// <LegoRR.exe @00471fa0>
#define Weapon_LegoObject_IsPoweredTracker ((bool32 (__cdecl* )(LegoObject* liveObj))0x00471fa0)

// <LegoRR.exe @00471fe0>
#define Weapon_LegoObject_FUN_00471fe0 ((bool32 (__cdecl* )(LegoObject* in_liveObj, real32 param_2, real32 param_3))0x00471fe0)

// <LegoRR.exe @004721c0>
#define Weapon_LegoObject_FUN_004721c0 ((bool32 (__cdecl* )(LegoObject* liveObj, real32 param_2, sint32 param_3))0x004721c0)

// <LegoRR.exe @00472280>
#define Weapon_LegoObject_FUN_00472280 ((bool32 (__cdecl* )(LegoObject* liveObj, real32 elapsed))0x00472280)

// <LegoRR.exe @00472320>
#define Weapon_LegoObject_SetObject_3c4 ((bool32 (__cdecl* )(LegoObject* liveObj, LegoObject* object3c4))0x00472320)

// <LegoRR.exe @00472340>
#define Weapon_LegoObject_WithinWeaponRange ((bool32 (__cdecl* )(LegoObject* liveObj, LegoObject* otherObj))0x00472340)

// <LegoRR.exe @004723f0>
#define Weapon_LegoObject_WithinAwarenessRange ((bool32 (__cdecl* )(LegoObject* liveObj, LegoObject* otherObj))0x004723f0)

#pragma endregion

}
