// Weapons.h : 
//

#pragma once

#include "../GameCommon.h"
#include "BezierCurve.h"
#include "Upgrade.h"


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

#define WEAPON_MAXLAZERS			10
#define WEAPON_MAXPROJECTILES		10

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

struct SearchData18_2 // [LegoRR/search.c|struct:0x18]
{
	/*00,4*/	undefined4 field_0;
	/*04,4*/	real32 ref_float_4;
	/*08,4*/	Vector3F* vectorp_8;
	/*0c,4*/	Vector3F* vectorp_c;
	/*10,4*/	bool32 field_10;
	/*14,4*/	undefined4 field_14;
	/*18*/
};
assert_sizeof(SearchData18_2, 0x18);


struct WeaponsModel // [LegoRR/Weapons.c|struct:0xa8]
{
	/*00,18*/	Gods98::Container* fireNullPairs[WEAPON_MAXWEAPONS][WEAPON_MAXFIRES];
	/*18,c*/	Gods98::Container* xPivotNulls[WEAPON_MAXWEAPONS];
	/*24,c*/	Gods98::Container* yPivotNulls[WEAPON_MAXWEAPONS];
	/*30,4*/	real32 pivotMaxZ;			// (ae: PivotMaxZ)
	/*34,4*/	real32 pivotMinZ;			// (ae: PivotMinZ)
	/*38,c*/	bool32 fireNullPairFrames[WEAPON_MAXWEAPONS];	// (valid: [0,1])
	/*44,48*/	Vector3F vectors3_44[WEAPON_MAXWEAPONS];
	/*68,48*/	Vector3F vectors3_68[WEAPON_MAXWEAPONS];
	/*8c,c*/	Upgrade_PartInfo* weaponParts[WEAPON_MAXWEAPONS];
	/*98,c*/	real32 timers[WEAPON_MAXWEAPONS];
	/*a4,4*/	uint32 weaponCount;			// (valid: [0,3])
	/*a8*/
};
assert_sizeof(WeaponsModel, 0xa8);


struct WeaponStats // [LegoRR/Weapons.c|struct:0x4b68]
{
	/*0000,4b00*/	real32 objectCoefs[LegoObject_Type_Count][LegoObject_ID_Count][OBJECT_MAXLEVELS]; // (cfg: [object] l:e:v:e:l:s, inits: -1.0f)
	/*4b00,4*/	bool32 isSlowDeath;          // (cfg: SlowDeath exists)
	/*4b04,4*/	real32 slowDeathInitialCoef; // (cfg: SlowDeath[0]) Initial damage multiplier.
	/*4b08,4*/	real32 slowDeathDuration;    // (cfg: SlowDeath[1])
	/*4b0c,4*/	real32 rechargeTime;         // (cfg: RechargeTime, default: 0.0f)
	/*4b10,4*/	real32 damage;               // (cfg: DefaultDamage, default: 0.0f)
	/*4b14,4*/	real32 dischargeRate;        // (cfg: DischargeRate)
	/*4b18,4*/	sint32 ammo;                 // (cfg: Ammo, unused?)
	/*4b1c,4*/	real32 weaponRange;          // (cfg: WeaponRange, default: 150.0f)
	/*4b20,48*/	real32 wallDestroyTimes[Lego_SurfaceType_Count]; // (cfg: WallDestroyTime_[surface], defaults: 5.0f)
	/*4b68*/
};
assert_sizeof(WeaponStats, 0x4b68);


struct Weapon_Lazer // [LegoRR/???|struct:0x10]
{
	/*00,4*/	Gods98::Mesh* innerMesh; // Inner bright beam.
	/*04,4*/	Gods98::Mesh* outerMesh; // Outer blue-ish beam glow.
	/*08,4*/	Gods98::Container* cont;
	/*0c,4*/	real32 timer; // Countdown duration in frames.
	/*10*/
};
assert_sizeof(Weapon_Lazer, 0x10);


struct Weapon_Projectile // [LegoRR/Weapons.c|struct:0x2b0] Seen in a table of [10]. This is likely an extension of the Weapons module.
{
	/*000,4*/	bool32 isCurvedPath;
	/*004,25c*/	BezierCurve curve;
	/*260,4*/	real32 totalDistance; // Curved paths only.
	/*264,4*/	real32 currDistance; // Curved paths only.
	/*268,c*/	Vector3F vector_268;
	/*274,c*/	Vector3F vector_274;
	/*280,c*/	Vector3F position_280;
	/*28c,4*/	real32 speed;
	/*290,c*/	Vector3F vector_290;
	/*29c,4*/	undefined4 field_29c;
	/*2a0,4*/	LegoObject* weaponObj; // field_2a0;
	/*2a4,4*/	LegoObject* object_2a4;
	/*2a8,4*/	sint32 weaponID;
	/*2ac,4*/	WeaponKnownType knownWeapon;
	/*2b0*/
};
assert_sizeof(Weapon_Projectile, 0x2b0);


struct Weapon_Globs // [LegoRR/Weapons.c|struct:0x1b90|tags:GLOBS]
{
	/*0000,4*/	uint32 weaponCount;
	/*0004,4*/	char** weaponNameList;
	/*0008,4*/	WeaponStats* weaponStatsList;
	/*000c,a0*/	Weapon_Lazer lazerList[WEAPON_MAXLAZERS];
	/*00ac,1ae0*/	Weapon_Projectile projectileList[WEAPON_MAXPROJECTILES];
	/*1b8c,4*/	Gods98::Config* config;
	/*1b90*/
};
assert_sizeof(Weapon_Globs, 0x1b90);

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @00504870>
extern Weapon_Globs & weaponGlobs;

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

// <LegoRR.exe @0046ee40>
#define Weapon_Initialise ((bool32 (__cdecl* )(const Gods98::Config* config, const char* gameName))0x0046ee40)

// On failure, returns weaponGlobs.weaponCount
// Weapon IDs are 1-indexed it seems...(?)
// <LegoRR.exe @0046f390>
#define Weapon_GetWeaponIDByName ((uint32 (__cdecl* )(const char* weaponName))0x0046f390)

// <LegoRR.exe @0046f3d0>
#define Weapon_GetRechargeTime ((real32 (__cdecl* )(sint32 weaponID))0x0046f3d0)

// <LegoRR.exe @0046f400>
#define Weapon_GetDischargeRate ((real32 (__cdecl* )(sint32 weaponID))0x0046f400)

// <LegoRR.exe @0046f430>
#define Weapon_GetWeaponRange ((real32 (__cdecl* )(sint32 weaponID))0x0046f430)

// <LegoRR.exe @0046f460>
#define Weapon_GetWallDestroyTime ((real32 (__cdecl* )(sint32 weaponID, Lego_SurfaceType surfaceType))0x0046f460)

// <LegoRR.exe @0046f490>
#define Weapon_GetDamageForObject ((real32 (__cdecl* )(sint32 weaponID, LegoObject* liveObj))0x0046f490)

// elapsed is only passed a non-zero value when unkSlowDeathBool is FALSE. But this may be coincidence.
// <LegoRR.exe @0046f530>
#define Weapon_GenericDamageObject ((void (__cdecl* )(LegoObject* liveObj, sint32 weaponID, bool32 unkSlowDeathBool, real32 elapsed, const Point2F* dir))0x0046f530)

// <LegoRR.exe @0046f640>
#define Weapon_GunDamageObject ((void (__cdecl* )(LegoObject* liveObj, real32 damage, bool32 param_3))0x0046f640)

// Probably projectile hitting an object and the projectile object being removed.
// Along with effects of weapon hitting.
// <LegoRR.exe @0046f670>
#define Weapon_Projectile_FUN_0046f670 ((void (__cdecl* )(Weapon_Projectile* projectile))0x0046f670)

// Update Projectile and Lazer shots.
// <LegoRR.exe @0046f810>
#define Weapon_Update ((void (__cdecl* )(real32 elapsedGame))0x0046f810)

// <LegoRR.exe @0046f8d0>
#define Weapon_LegoObject_Callback_FUN_0046f8d0 ((bool32 (__cdecl* )(LegoObject* liveObj, real32* pElapsed))0x0046f8d0)

// <LegoRR.exe @0046fa30>
#define Weapon_GunHitObject ((void (__cdecl* )(LegoObject* liveObj, const Vector3F* dirVec2D, OPTIONAL const Vector3F* worldPos, sint32 weaponID, WeaponKnownType knownWeapon))0x0046fa30)

// <LegoRR.exe @0046fbe0>
#define Weapon_Projectile_UpdatePath ((void (__cdecl* )(Weapon_Projectile* projectile, real32 elapsed))0x0046fbe0)

// <LegoRR.exe @0046fdb0>
#define Weapon_Projectile_AddStraightPath ((bool32 (__cdecl* )(LegoObject* liveObj, const Vector3F* fromPos, const Vector3F* toPos, uint32 weaponID, WeaponKnownType knownWeapon))0x0046fdb0)

// <LegoRR.exe @0046ff30>
#define Weapon_Projectile_AddCurvedPath ((bool32 (__cdecl* )(LegoObject* liveObj, const Vector3F* fromPos, const Vector3F* toPos, sint32 weaponID, WeaponKnownType knownWeapon))0x0046ff30)

// <LegoRR.exe @004701b0>
#define Weapon_GetObjectTypeAndID_ByKnownWeaponType ((void (__cdecl* )(WeaponKnownType knownWeapon, OUT LegoObject_Type* objType, OUT LegoObject_ID* objID))0x004701b0)

// <LegoRR.exe @00470230>
#define Weapon_Projectile_GetNextAvailable ((sint32 (__cdecl* )(void))0x00470230)

// <LegoRR.exe @00470250>
#define Weapon_Lazer_GetNextAvailable ((sint32 (__cdecl* )(void))0x00470250)

// <LegoRR.exe @00470270>
#define Weapon_FireLazer ((LegoObject* (__cdecl* )(const Vector3F* fromPos, const Vector3F* dir, LegoObject* liveObj, real32 elapsed, real32 weaponRange, sint32 weaponID, real32 coef))0x00470270)

// <LegoRR.exe @00470520>
#define Weapon_LegoObject_Collision_FUN_00470520 ((bool32 (__cdecl* )(LegoObject* liveObj, const Point2F* param_2, const Point2F* param_3, bool32 param_4))0x00470520)

// <LegoRR.exe @00470570>
#define Weapon_LegoObject_CollisionBox_FUN_00470570 ((bool32 (__cdecl* )(LegoObject* liveObj, const Point2F* param_2, const Point2F* param_3, OPTIONAL OUT Point2F* point, bool32 param_5))0x00470570)

// <LegoRR.exe @00470800>
#define Weapon_LegoObject_CollisionRadius_FUN_00470800 ((bool32 (__cdecl* )(LegoObject* liveObj, const Point2F* param_2, const Point2F* param_3, bool32 param_4))0x00470800)

// <LegoRR.exe @004708f0>
#define Weapon_LegoObject_TestCollision_FUN_004708f0 ((bool32 (__cdecl* )(LegoObject* liveObj, const Point2F* fromPos, const Point2F* scalar2D))0x004708f0)

// <LegoRR.exe @00470950>
#define Weapon_Lazer_Add ((void (__cdecl* )(Gods98::Container* cont, const Vector3F* fromPos, const Vector3F* toPos))0x00470950)

// <LegoRR.exe @00470a20>
#define Weapon_Lazer_InitMesh ((void (__cdecl* )(Gods98::Mesh* mesh, real32 thickness, const Vector3F* fromPos, const Vector3F* toPos, real32 red, real32 green, real32 blue, real32 alpha))0x00470a20)

// <LegoRR.exe @00471580>
#define Weapon_LegoObject_GetWeaponsModel ((WeaponsModel* (__cdecl* )(LegoObject* liveObj))0x00471580)

// <LegoRR.exe @004715b0>
#define Weapon_LegoObject_GetWeaponTimer ((real32 (__cdecl* )(LegoObject* liveObj, uint32 frameNo))0x004715b0)

// <LegoRR.exe @004715d0>
#define Weapon_LegoObject_SetWeaponTimer ((void (__cdecl* )(LegoObject* liveObj, real32 timer, uint32 frameNo))0x004715d0)

// Only Z value of the two vector parameters is used.
// <LegoRR.exe @004715f0>
#define Weapon_MathUnk_CheckVectorsZScalar_InRange ((bool32 (__cdecl* )(const Vector3F* vectorPos, const Vector3F* vectorScaled, real32 minZ, real32 maxZ, real32 scalar))0x004715f0)

// DATA: SearchData18_2*
// <LegoRR.exe @00471630>
#define Weapon_LegoObject_Callback_FUN_00471630 ((bool32 (__cdecl* )(LegoObject* liveObj, void* search))0x00471630)

// <LegoRR.exe @004718f0>
#define Weapon_LegoObject_FUN_004718f0 ((void (__cdecl* )(SearchData18_2* search))0x004718f0)

// <LegoRR.exe @00471b20>
#define Weapon_DoCallbacksSearch_FUN_00471b20 ((bool32 (__cdecl* )(const Vector3F* vecCector, const Vector3F* vecDistance, OUT LegoObject** liveObj, IN OUT real32* float_4, undefined4 param_5))0x00471b20)

// <LegoRR.exe @00471b90>
#define Weapon_LegoObject_DoCallbacksSearch_FUN_00471b90 ((bool32 (__cdecl* )(LegoObject* liveObj, LegoObject* otherObj))0x00471b90)

// <LegoRR.exe @00471c20>
#define Weapon_LegoObject_SeeThroughWalls_FUN_00471c20 ((bool32 (__cdecl* )(LegoObject* liveObj, LegoObject* otherObj))0x00471c20)

// <LegoRR.exe @00471ce0>
#define Weapon_GetFireNull ((Gods98::Container* (__cdecl* )(WeaponsModel* weapons, uint32 frameNo, uint32 pairNo))0x00471ce0)

// <LegoRR.exe @00471d00>
#define Weapon_GetXPivotNull ((Gods98::Container* (__cdecl* )(WeaponsModel* weapons, uint32 frameNo))0x00471d00)

// <LegoRR.exe @00471d10>
#define Weapon_GetYPivotNull ((Gods98::Container* (__cdecl* )(WeaponsModel* weapons, uint32 frameNo))0x00471d10)

// <LegoRR.exe @00471d20>
#define Weapon_PivotTracker ((void (__cdecl* )(WeaponsModel* weapons, const Vector3F* targetWorldPos, real32 elapsed, bool32 assignTarget, uint32 frameNo))0x00471d20)

// May return out_vector44 (param_2)
// <LegoRR.exe @00471f30>
#define Weapon_GetVectors44 ((void (__cdecl* )(WeaponsModel* weapons, OUT Vector3F* vector44, uint32 frameNo))0x00471f30)

// <LegoRR.exe @00471f60>
#define Weapon_LegoObject_GetCollCenterPosition ((bool32 (__cdecl* )(LegoObject* liveObj, OUT Vector3F* vector))0x00471f60)

// <LegoRR.exe @00471fa0>
#define Weapon_LegoObject_IsActiveTracker ((bool32 (__cdecl* )(LegoObject* liveObj))0x00471fa0)

// <LegoRR.exe @00471fe0>
#define Weapon_LegoObject_UpdateLaserTracker ((bool32 (__cdecl* )(LegoObject* liveObj, real32 elapsed, uint32 frameNo))0x00471fe0)

// <LegoRR.exe @004721c0>
#define Weapon_LegoObject_UpdateNonLaserTracker ((bool32 (__cdecl* )(LegoObject* liveObj, real32 elapsed, uint32 frameNo))0x004721c0)

// <LegoRR.exe @00472280>
#define Weapon_LegoObject_UpdateTracker ((bool32 (__cdecl* )(LegoObject* liveObj, real32 elapsed))0x00472280)

// Removes the projectile reference if it matches the specified object.
// DATA: LegoObject* projectileObj
// <LegoRR.exe @00472320>
#define Weapon_Callback_RemoveProjectileReference ((bool32 (__cdecl* )(LegoObject* liveObj, void* projectileObj))0x00472320)

// <LegoRR.exe @00472340>
#define Weapon_LegoObject_WithinWeaponRange ((bool32 (__cdecl* )(LegoObject* liveObj, LegoObject* otherObj))0x00472340)

// <LegoRR.exe @004723f0>
#define Weapon_LegoObject_WithinAwarenessRange ((bool32 (__cdecl* )(LegoObject* liveObj, LegoObject* otherObj))0x004723f0)

#pragma endregion

}
