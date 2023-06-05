// Weapons.cpp : 
//

#include "../../engine/core/Maths.h"
#include "../../engine/core/Utils.h"
#include "../../engine/gfx/Mesh.h"
#include "../../engine/input/Input.h"

#include "../Game.h"
#include "../audio/SFX.h"
#include "../effects/Effects.h"
#include "../front/Reward.h"
#include "BezierCurve.h"
#include "Collision.h"
#include "Object.h"
#include "Stats.h"
#include "Weapons.h"


/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @00504870>
LegoRR::Weapon_Globs & LegoRR::weaponGlobs = *(LegoRR::Weapon_Globs*)0x00504870;

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @0046ee40>
bool32 __cdecl LegoRR::Weapon_Initialise(const Gods98::Config* config, const char* gameName)
{
	/// SANITY: Memzero all weapons data before using.
	std::memset(&weaponGlobs, 0, sizeof(weaponGlobs));

	weaponGlobs.config = config;
	weaponGlobs.weaponCount = 0;

	const Gods98::Config* arrayFirst = Gods98::Config_FindArray(config, Lego_ID("WeaponTypes"));
	if (arrayFirst == nullptr) {
		return false;
	}

	for (const Gods98::Config* item = arrayFirst; item != nullptr; item = Gods98::Config_GetNextItem(item)) {
		weaponGlobs.weaponCount++;
	}

	const uint32 allocSize1 = weaponGlobs.weaponCount * sizeof(WeaponStats);
	weaponGlobs.weaponStatsList = (WeaponStats*)Gods98::Mem_Alloc(allocSize1);
	if (weaponGlobs.weaponStatsList == nullptr) {
		return false;
	}

	/// FIX APPLY: ZERO OUT MEMORY SO THAT WE DON'T GET THE DISCHARGE GLITCH WITH POWER BEAM.
	std::memset(weaponGlobs.weaponStatsList, 0, allocSize1);

	for (uint32 i = 0; i< weaponGlobs.weaponCount; i++) {
		WeaponStats* weaponStats = &weaponGlobs.weaponStatsList[i];

		for (uint32 objType = 0; objType < LegoObject_Type_Count; objType++) {
			for (uint32 objID = 0; objID < LegoObject_ID_Count; objID++) {
				for (uint32 objLevel = 0; objLevel < OBJECT_MAXLEVELS; objLevel++) {
					weaponStats->objectCoefs[objType][objID][objLevel] = -1.0f;
				}
			}
		}
		/// FIX APPLY: ZERO OUT MEMORY SO THAT WE DON'T GET THE DISCHARGE GLITCH WITH POWER BEAM.
		weaponStats->isSlowDeath = false;
		weaponStats->slowDeathInitialCoef = 0.0f;
		weaponStats->slowDeathDuration = 0.0f;
		weaponStats->rechargeTime = 0.0f;
		weaponStats->damage = 0.0f;
		weaponStats->dischargeRate = 0.0f;
		weaponStats->ammo = 0;
		weaponStats->weaponRange = 150.0f;
		for (uint32 surfaceType = 0; surfaceType < Lego_SurfaceType_Count; surfaceType++) {
			weaponStats->wallDestroyTimes[surfaceType] = 5.0f;
		}
	}

	const uint32 allocSize2 = weaponGlobs.weaponCount * sizeof(char*);
	weaponGlobs.weaponNameList = (char**)Gods98::Mem_Alloc(allocSize2);

	/// SANITY: Memzero
	std::memset(weaponGlobs.weaponNameList, 0, allocSize2);

	if (weaponGlobs.weaponNameList != nullptr) {

		const Gods98::Config* item = arrayFirst;
		for (uint32 i = 0; item != nullptr; i++, item = Gods98::Config_GetNextItem(item)) {
			WeaponStats* weaponStats = &weaponGlobs.weaponStatsList[i];

			weaponGlobs.weaponNameList[i] = Gods98::Util_StrCpy(Gods98::Config_GetItemName(item));

			char* stringParts[std::max(2, OBJECT_MAXLEVELS)];
			char surfaceNameBuff[128];

			const Gods98::Config* statFirst = Gods98::Config_FindArray(config, Lego_ID("WeaponTypes", Gods98::Config_GetItemName(item)));
			for (const Gods98::Config* stat = statFirst; stat != nullptr; stat = Gods98::Config_GetNextItem(stat)) {

				if (::_stricmp(Gods98::Config_GetItemName(stat), "SlowDeath") == 0) {
					/// FIX APPLY: Don't modify the config strings. BAD! NO TOUCH!
					char* str = Gods98::Util_StrCpy(Gods98::Config_GetDataString(stat));
					const uint32 argc = Gods98::Util_TokeniseSafe(str, stringParts, ":", 2);
					Config_FatalItem(argc < 2, stat, "Not enough parts in WeaponTypes SlowDeath");

					weaponStats->isSlowDeath = true;
					weaponStats->slowDeathInitialCoef = (real32)std::atof(stringParts[0]);
					weaponStats->slowDeathDuration = (real32)std::atof(stringParts[1]);

					Gods98::Mem_Free(str);
				}
				else if (::_stricmp(Gods98::Config_GetItemName(stat), "RechargeTime") == 0) {
					weaponStats->rechargeTime = (real32)std::atof(Gods98::Config_GetDataString(stat));
				}
				else if (::_stricmp(Gods98::Config_GetItemName(stat), "DefaultDamage") == 0) {
					weaponStats->damage = (real32)std::atof(Gods98::Config_GetDataString(stat));
				}
				else if (::_stricmp(Gods98::Config_GetItemName(stat), "DischargeRate") == 0) {
					weaponStats->dischargeRate = (real32)std::atof(Gods98::Config_GetDataString(stat));
				}
				else if (::_stricmp(Gods98::Config_GetItemName(stat), "Ammo") == 0) {
					/// FIX APPLY: Ammo was assigned to zero whenever a check for its name failed (after surfaceTypes, before objectCoefs)
					weaponStats->ammo = std::atoi(Gods98::Config_GetDataString(stat));
				}
				else if (::_stricmp(Gods98::Config_GetItemName(stat), "WeaponRange") == 0) {
					weaponStats->weaponRange = (real32)std::atof(Gods98::Config_GetDataString(stat));
				}
				else {
					uint32 surfaceType;
					for (surfaceType = 0; surfaceType < Lego_SurfaceType_Count; surfaceType++) {
						std::sprintf(surfaceNameBuff, "WallDestroyTime_%s", legoGlobs.surfaceName[surfaceType] + std::strlen("Lego_SurfaceType_"));
						if (::_stricmp(Gods98::Config_GetItemName(stat), surfaceNameBuff) == 0) {
							weaponStats->wallDestroyTimes[surfaceType] = (real32)std::atof(Gods98::Config_GetDataString(stat));
							break;
						}
					}

					// Key wasn't a surface type, the only other thing it can be is object coefs.
					if (surfaceType == Lego_SurfaceType_Count) {
						if (::_strnicmp(Gods98::Config_GetItemName(stat), "WallDestroyTime_", std::strlen("WallDestroyTime_")) == 0) {
							Config_WarnItemF(true, stat, "Unknown WeaponTypes surface %s", Gods98::Config_GetItemName(stat));
						}

						LegoObject_Type objType = LegoObject_None;
						LegoObject_ID objID = (LegoObject_ID)0;
						/// FIX APPLY: Don't infinite loop when failing to parse an object name...
						if (Lego_GetObjectByName(Gods98::Config_GetItemName(stat), &objType, &objID, nullptr)) {
							const uint32 objLevelCount = Stats_GetLevels(objType, objID);

							/// FIX APPLY: Don't modify the config strings. BAD! NO TOUCH!
							char* str = Gods98::Util_StrCpy(Gods98::Config_GetDataString(stat));
							const uint32 argc = Gods98::Util_TokeniseSafe(str, stringParts, ":", OBJECT_MAXLEVELS);
							Config_WarnItemF(argc < objLevelCount, stat, "Not enough levels in WeaponTypes object coef %s", Gods98::Config_GetItemName(stat));

							for (uint32 objLevel = 0; objLevel < objLevelCount; objLevel++) {
								/// FIX BAZ MOD: Boulder::TeleportBig missing one level, which should be the same as previous values.
								weaponStats->objectCoefs[objType][objID][objLevel] = (real32)std::atof(stringParts[std::min(argc-1, objLevel)]);
							}

							Gods98::Mem_Free(str);
						}
					}
				}
			}
		}
		return true;


		//Gods98::Mem_Free(weaponGlobs.weaponNameList);
	}
	Gods98::Mem_Free(weaponGlobs.weaponStatsList);

	return false;
}

// On failure, returns g_WeaponTypes_COUNT
// Weapon IDs are 1-indexed it seems...
// <LegoRR.exe @0046f390>
uint32 __cdecl LegoRR::Weapon_GetWeaponIDByName(const char* weaponName)
{
	for (uint32 weaponID = 0; weaponID < weaponGlobs.weaponCount; weaponID++) {
		if (::_stricmp(weaponGlobs.weaponNameList[weaponID], weaponName) == 0) {
			return weaponID;
		}
	}
	return weaponGlobs.weaponCount + 1; // Invalid weaponID??
}

// <LegoRR.exe @0046f3d0>
real32 __cdecl LegoRR::Weapon_GetRechargeTime(uint32 weaponID)
{
	return weaponGlobs.weaponStatsList[weaponID].rechargeTime;
}

// <LegoRR.exe @0046f400>
real32 __cdecl LegoRR::Weapon_GetDischargeRate(uint32 weaponID)
{
	return weaponGlobs.weaponStatsList[weaponID].dischargeRate;
}

// <LegoRR.exe @0046f430>
real32 __cdecl LegoRR::Weapon_GetWeaponRange(uint32 weaponID)
{
	return weaponGlobs.weaponStatsList[weaponID].weaponRange;
}

// <LegoRR.exe @0046f460>
real32 __cdecl LegoRR::Weapon_GetWallDestroyTime(uint32 weaponID, Lego_SurfaceType surfaceType)
{
	return weaponGlobs.weaponStatsList[weaponID].wallDestroyTimes[surfaceType];
}

// <LegoRR.exe @0046f490>
real32 __cdecl LegoRR::Weapon_GetDamageForObject(uint32 weaponID, LegoObject* liveObj)
{
	const real32 coef = weaponGlobs.weaponStatsList[weaponID].objectCoefs[liveObj->type][liveObj->id][liveObj->objLevel];
	if (coef == -1.0f) {
		// Damage for this specific object type not defined, return default damage.
		return weaponGlobs.weaponStatsList[weaponID].damage;
	}
	return coef;
}

// elapsed is only passed a non-zero value when unkSlowDeathBool is FALSE. But this may be coincidence.
// <LegoRR.exe @0046f530>
void __cdecl LegoRR::Weapon_GenericDamageObject(LegoObject* liveObj, uint32 weaponID, bool32 hit, real32 elapsed, const Point2F* dir)
{
	if ((liveObj->flags3 & LIVEOBJ3_CANDAMAGE) && LegoObject_IsActive(liveObj, true)) {

		/// CHANGE: Getter moved from top of function.
		const real32 damage = Weapon_GetDamageForObject(weaponID, liveObj);

		const WeaponStats* weaponStats = &weaponGlobs.weaponStatsList[weaponID];
		if (weaponStats->isSlowDeath) {
			if (hit) {
				// SlowDeath == MORE DOTS! (damage over time)
				LegoObject_AddDamage2(liveObj, weaponStats->slowDeathInitialCoef * damage, true, elapsed);
				liveObj->weaponID = weaponID;
				liveObj->weaponSlowDeath = weaponStats->slowDeathDuration;
				LegoObject_Hit(liveObj, dir, true);
			}
			else {
				LegoObject_AddDamage2(liveObj, damage * elapsed, true, elapsed);
			}
		}
		else {
			/// FIXME: Are slow death weapons supposed to not count towards attack quotas?
			if (liveObj->type == LegoObject_RockMonster) {
				RewardQuota_RockMonsterAttacked();
			}

			LegoObject_AddDamage2(liveObj, damage, true, elapsed);
			if (hit) {
				LegoObject_Hit(liveObj, dir, true);
			}
		}
	}
}

// <LegoRR.exe @0046f640>
void __cdecl LegoRR::Weapon_GunDamageObject(LegoObject* liveObj, real32 damage, bool32 reactToHit)
{
	LegoObject_AddDamage2(liveObj, damage, true, 1.0f);
	LegoObject_Hit(liveObj, nullptr, reactToHit);
}

// Probably projectile hitting an object and the projectile object being removed.
// Along with effects of weapon hitting.
// <LegoRR.exe @0046f670>
void __cdecl LegoRR::Weapon_Projectile_FUN_0046f670(Weapon_Projectile* projectile)
{
	if (projectile->projectileObject != nullptr) {

		const real32 weaponRange = Weapon_GetWeaponRange(projectile->weaponID);
		if (weaponRange != 0.0f && weaponRange <= Gods98::Maths_Vector3DDistance(&projectile->initialPos, &projectile->currPos)) {

			LegoObject_Remove(projectile->projectileObject);
			projectile->projectileObject = nullptr;
		}
		else {
			Vector3F travelDistance;
			Gods98::Maths_Vector3DSubtract(&travelDistance, &projectile->currPos, &projectile->lastPos);

			SearchWeapons18_2 search = { 0 }; // dummy init

			search.success = false;
			search.ref_float_4 = 1.0f;
			search.foundObject = nullptr;
			search.ignoreObject = projectile->shooterObject;
			search.fromPos = &projectile->lastPos;
			search.fromToDistance = &travelDistance;

			for (auto obj : objectListSet.EnumerateSkipUpgradeParts()) {
				Weapon_LegoObject_Callback_FUN_00471630(obj, &search);
			}
			//LegoObject_RunThroughListsSkipUpgradeParts(Weapon_LegoObject_Callback_FUN_00471630, &search);

			Weapon_LegoObject_FUN_004718f0(&search);

			if (search.success) {
				LegoObject_Remove(projectile->projectileObject);
				projectile->projectileObject = nullptr;

				// success does not mean foundObject is non-null
				if (search.foundObject != nullptr) {
					Vector3F worldPos;
					Gods98::Maths_RayEndPoint(&worldPos, search.fromPos, search.fromToDistance, search.ref_float_4);
					Weapon_GunHitObject(search.foundObject, &projectile->dir, &worldPos, projectile->weaponID, projectile->knownWeapon);
				}
			}
		}
	}
}

// Update Projectile and Lazer shots.
// <LegoRR.exe @0046f810>
void __cdecl LegoRR::Weapon_Update(real32 elapsedGame)
{
	for (uint32 i = 0; i < WEAPON_MAXPROJECTILES; i++) {
		Weapon_Projectile* projectile = &weaponGlobs.projectileList[i];
		if (projectile->projectileObject != nullptr) {
			Weapon_Projectile_UpdatePath(projectile, elapsedGame);
		}
	}

	for (auto obj : objectListSet.EnumerateSkipUpgradeParts()) {
		Weapon_LegoObject_Callback_UpdateObject(obj, &elapsedGame);
	}
	//LegoObject_RunThroughListsSkipUpgradeParts(Weapon_LegoObject_Callback_UpdateObject, &elapsedGame);

	for (uint32 i = 0; i < WEAPON_MAXPROJECTILES; i++) {
		Weapon_Projectile* projectile = &weaponGlobs.projectileList[i];
		Weapon_Projectile_FUN_0046f670(projectile);
	}

	for (uint32 i = 0; i < WEAPON_MAXLAZERS; i++) {
		Weapon_Lazer* lazer = &weaponGlobs.lazerList[i];
		if (lazer->innerMesh != nullptr) {
			if (lazer->timer < 0.0f) {
				Gods98::Mesh_Remove(lazer->innerMesh, lazer->cont->activityFrame);
				Gods98::Mesh_Remove(lazer->outerMesh, lazer->cont->activityFrame);
				lazer->innerMesh = nullptr;
				/// TODO: Should we also nullify lazer->outerMesh?
			}
			lazer->timer -= elapsedGame;
		}
	}
}

// <LegoRR.exe @0046f8d0>
bool32 __cdecl LegoRR::Weapon_LegoObject_Callback_UpdateObject(LegoObject* liveObj, void* pElapsed)
{
	const real32 elapsed = *(real32*)pElapsed;
	// Update recharge timer.
	if (liveObj->weaponRechargeTimer > 0.0f) {
		liveObj->weaponRechargeTimer -= elapsed;
	}

	// Check if we should play Recharge SFX based on the weapon timer.
	const WeaponsModel* weapons = Weapon_LegoObject_GetWeaponsModel(liveObj);
	if (weapons != nullptr) {

		for (uint32 frameNo = 0; frameNo < weapons->weaponCount; frameNo++) {

			const real32 oldWeaponTimer = Weapon_LegoObject_GetWeaponTimer(liveObj, frameNo);
			if (oldWeaponTimer > 0.0f) {

				real32 newWeaponTimer = (oldWeaponTimer - elapsed);
				Weapon_LegoObject_SetWeaponTimer(liveObj, newWeaponTimer, frameNo);
				// In the scenario that GetWeaponTimer doesn't guarantee the same value fed to the setter, uncomment this.
				//newWeaponTimer = Weapon_LegoObject_GetWeaponTimer(liveObj, frameNo);

				/// TODO: Choose how to handle random sound choice here.
				///       (Not that there are multiple sounds defined for this SFX, but still...)
				const real32 sampleTime = SFX_Random_GetSamplePlayTime(SFX_LazerRecharge);

				const real32 standardSampleTime = (sampleTime * STANDARD_FRAMERATE * Lego_GetGameSpeed());

				// Play sound if we've passed the point where the Recharge SFX won't end before the recharge timer.

				if (standardSampleTime > newWeaponTimer && standardSampleTime < oldWeaponTimer) {
					Vector3F wPos;
					Gods98::Container* cont = LegoObject_GetActivityContainer(liveObj);
					Gods98::Container_GetPosition(cont, nullptr, &wPos);
					SFX_Random_PlaySound3DOnContainer(nullptr, SFX_LazerRecharge, false, false, &wPos);
				}
			}
		}
	}

	// Update slow death timer.
	if (liveObj->weaponSlowDeath > 0.0f) {
		Weapon_GenericDamageObject(liveObj, liveObj->weaponID, false, elapsed, nullptr);
		liveObj->weaponSlowDeath -= elapsed;
	}

	return false;
}

// effectSpawnPos is required if LegoObject_IsRockMonsterCanGather(liveObj).
// <LegoRR.exe @0046fa30>
void __cdecl LegoRR::Weapon_GunHitObject(LegoObject* liveObj, const Vector3F* dir, OPTIONAL const Vector3F* effectSpawnPos, uint32 weaponID, Weapon_KnownType knownWeapon)
{
	Point2F dirVec;
	Gods98::Maths_Vector3DMake2D(&dirVec, dir);

	if (LegoObject_IsRockMonsterCanGather(liveObj)) {
		Effect_Spawn_BoulderExplode(effectSpawnPos);
	}

	switch (knownWeapon) {
	case Weapon_KnownType_Laser:
		if (StatsObject_GetStatsFlags2(liveObj) & STATS2_CANLASER) {
			const real32 damage = StatsObject_GetLaserDamage(liveObj);
			Weapon_GunDamageObject(liveObj, damage, true);
		}
		if (effectSpawnPos != nullptr) {
			Effect_Spawn_Particle(MISCOBJECT_LAZERHIT, effectSpawnPos, nullptr);
		}
		break;

	case Weapon_KnownType_Pusher:
		if (StatsObject_GetStatsFlags2(liveObj) & STATS2_CANPUSH) {
			const real32 pusherDist = StatsObject_GetPusherDist(liveObj);
			const real32 damage = StatsObject_GetPusherDamage(liveObj);
			Gods98::Maths_Vector2DScale(&dirVec, &dirVec, 1.5f);
			Weapon_GunDamageObject(liveObj, damage, true);
			LegoObject_Push(liveObj, &dirVec, pusherDist);
		}
		if (effectSpawnPos != nullptr) {
			Effect_Spawn_Particle(MISCOBJECT_PUSHERHIT, effectSpawnPos, nullptr);
		}
		break;

	case Weapon_KnownType_Freezer:

		if (StatsObject_GetStatsFlags2(liveObj) & STATS2_CANFREEZE) {
			real32 freezerTime = StatsObject_GetFreezerTime(liveObj);
			if (freezerTime <= 0.0f) freezerTime = 10.0f; // Default freezer time

			const real32 damage = StatsObject_GetFreezerDamage(liveObj);
			Weapon_GunDamageObject(liveObj, damage, false);
			LegoObject_Freeze(liveObj, freezerTime);
		}
		if (effectSpawnPos != nullptr) {
			Effect_Spawn_Particle(MISCOBJECT_FREEZERHIT, effectSpawnPos, nullptr);
		}
		break;

	default:
		Weapon_GenericDamageObject(liveObj, weaponID, true, 0.0f, &dirVec);
		break;
	}
}


// <LegoRR.exe @0046fbe0>
void __cdecl LegoRR::Weapon_Projectile_UpdatePath(Weapon_Projectile* projectile, real32 elapsed)
{
	Vector3F newDir = { 0.0f }; // dumy init

	Gods98::Container* cont = LegoObject_GetActivityContainer(projectile->projectileObject);
	Gods98::Container_GetPosition(cont, nullptr, &projectile->lastPos);

	const real32 velocity = projectile->speed * elapsed;
	if (!projectile->isCurvedPath) {
		Gods98::Maths_RayEndPoint(&projectile->currPos, &projectile->currPos, &projectile->dir, velocity);
		Gods98::Container_SetPosition(cont, nullptr, projectile->currPos.x, projectile->currPos.y, projectile->currPos.z);

		newDir = projectile->dir;
	}
	else {
		projectile->currDistance += velocity;

		Point2F curvePoint;
		LegoRR::BezierCurve_Interpolate(&projectile->curve, projectile->currDistance, &curvePoint);
		// Yeah... I don't know what's going on here. Curve x is used for 3D x,y. And curve y is used for 3D z.
		// Maybe this is for curved upward arcs, like throwing a boulder(?)
		projectile->currPos = projectile->initialPos;
		projectile->currPos.x += curvePoint.x * projectile->dir.x;
		projectile->currPos.y += curvePoint.x * projectile->dir.y;
		projectile->currPos.z += curvePoint.y;

		Gods98::Container_SetPosition(cont, nullptr, projectile->currPos.x, projectile->currPos.y, projectile->currPos.z);

		Gods98::Maths_Vector3DSubtract(&newDir, &projectile->currPos, &projectile->lastPos);
		newDir.z *= 0.25f;
	}

	Gods98::Container_SetOrientation(cont, nullptr, newDir.x, newDir.y, newDir.z, 0.0f, 0.0f, -1.0f);
}

// <LegoRR.exe @0046fdb0>
bool32 __cdecl LegoRR::Weapon_Projectile_AddStraightPath(LegoObject* liveObj, const Vector3F* fromPos, const Vector3F* fromToDistance, sint32 weaponID, Weapon_KnownType knownWeapon)
{
	if (knownWeapon == Weapon_KnownType_Laser) {
		// Drain crystals for pusher beam
		LegoObject_Weapon_FUN_004375c0(liveObj, weaponID, 1.0f);
	}

	LegoObject_Type objType = LegoObject_None; // dummy init
	LegoObject_ID objID = (LegoObject_ID)0; // dummy init
	Weapon_GetObjectTypeAndID_ByKnownWeaponType(knownWeapon, (LegoObject_Type*)&objType, &objID);

	ObjectModel* objModel = nullptr; // dummy init
	if (Lego_GetObjectTypeModel(objType, objID, &objModel)) {

		const sint32 handle = Weapon_Projectile_GetNextAvailable();
		if (handle != -1) {
			Weapon_Projectile* projectile = &weaponGlobs.projectileList[handle];

			projectile->isCurvedPath = false;
			projectile->weaponID = weaponID;
			projectile->knownWeapon = knownWeapon;

			projectile->speed = 5.0f;
			projectile->unused_29c = 0;

			projectile->initialPos = *fromPos;
			projectile->currPos = *fromPos;
			projectile->dir = *fromToDistance;
			Gods98::Maths_Vector3DNormalize(&projectile->dir);

			projectile->shooterObject = liveObj;
			projectile->projectileObject = LegoObject_CreateInWorld(objModel, objType, objID, 0, fromPos->x, fromPos->y, 0.0f);

			// UpdatePath will assign projectile->lastPos
			Weapon_Projectile_UpdatePath(projectile, 0.0f);
			return true;
		}
	}
	return false;
}

// <LegoRR.exe @0046ff30>
bool32 __cdecl LegoRR::Weapon_Projectile_AddCurvedPath(LegoObject* liveObj, const Vector3F* fromPos, const Vector3F* toPos, sint32 weaponID, Weapon_KnownType knownWeapon)
{
	LegoObject_Type objType = LegoObject_None; // dummy init
	LegoObject_ID objID = (LegoObject_ID)0; // dummy init
	Weapon_GetObjectTypeAndID_ByKnownWeaponType(knownWeapon, &objType, &objID);

	ObjectModel* objModel = nullptr; // dummy init
	if (Lego_GetObjectTypeModel(objType, objID, &objModel)) {

		const sint32 handle = Weapon_Projectile_GetNextAvailable();
		if (handle != -1) {
			Weapon_Projectile* projectile = &weaponGlobs.projectileList[handle];

			projectile->isCurvedPath = true;
			projectile->weaponID = weaponID;
			projectile->knownWeapon = knownWeapon;

			projectile->speed = 4.0f;
			projectile->unused_29c = 0;

			projectile->initialPos = *fromPos;
			projectile->currPos = *fromPos;
			Gods98::Maths_Vector3DSubtract(&projectile->dir, toPos, fromPos);
			Gods98::Maths_Vector3DNormalize(&projectile->dir);

			const real32 dist = Gods98::Maths_Vector3DDistance(fromPos, toPos);
			const real32 zDist = toPos->z - fromPos->z;
			const Point2F p2 = { dist, zDist };
			const Point2F p0 = { 0.0f, 0.0f };

			Point2F p1 = { 1.0f, -1.3f };
			Point2F p3 = { 0.3f,  1.0f };
			BezierCurve_Vector2DChangeLength(&p1, dist / 5.0f);
			BezierCurve_Vector2DChangeLength(&p3, dist / 3.0f);

			BezierCurve* curve = &projectile->curve;
			BezierCurve_BuildPoints(curve, &p0, &p1, &p2, &p3, BEZIERCURVE_MAXPOINTS);

			projectile->totalDistance = BezierCurve_UpdateDistances(curve);
			projectile->currDistance = 0.0f;

			projectile->shooterObject = liveObj;
			projectile->projectileObject = LegoObject_CreateInWorld(objModel, objType, objID, 0, fromPos->x, fromPos->y, 0.0f);

			// UpdatePath will assign projectile->lastPos
			Weapon_Projectile_UpdatePath(projectile, 0.0f);
			return true;
		}
	}
	return false;
}

// <LegoRR.exe @004701b0>
void __cdecl LegoRR::Weapon_GetObjectTypeAndID_ByKnownWeaponType(Weapon_KnownType knownWeapon, OUT LegoObject_Type* objType, OUT LegoObject_ID* objID)
{
	switch (knownWeapon) {
	case Weapon_KnownType_Laser:
		*objType = LegoObject_LaserShot;
		*objID = (LegoObject_ID)0;
		break;
	case Weapon_KnownType_Pusher:
		*objType = LegoObject_Pusher;
		*objID = (LegoObject_ID)0;
		break;
	case Weapon_KnownType_Freezer:
		*objType = LegoObject_Freezer;
		*objID = (LegoObject_ID)0;
		break;
	case Weapon_KnownType_Boulder:
		*objType = LegoObject_Boulder;
		*objID = (LegoObject_ID)0;
		break;
		/// SANITY: Default output values
	default:
		*objType = LegoObject_None;
		*objID = (LegoObject_ID)0;
		break;
	}

}

// <LegoRR.exe @00470230>
sint32 __cdecl LegoRR::Weapon_Projectile_GetNextAvailable(void)
{
	for (sint32 i = 0; i < WEAPON_MAXPROJECTILES; i++) {
		if (weaponGlobs.projectileList[i].projectileObject == nullptr)
			return i;
	}
	return -1;
}

// <LegoRR.exe @00470250>
sint32 __cdecl LegoRR::Weapon_Lazer_GetNextAvailable(void)
{
	for (sint32 i = 0; i < WEAPON_MAXLAZERS; i++) {
		if (weaponGlobs.lazerList[i].innerMesh == nullptr)
			return i;
	}
	return -1;
}


// <LegoRR.exe @00470270>
LegoRR::LegoObject* __cdecl LegoRR::Weapon_FireLazer(const Vector3F* fromPos, const Vector3F* dir, LegoObject* liveObj, real32 elapsed, real32 weaponRange, sint32 weaponID, real32 coef)
{
	SFX_Random_PlaySound3DOnContainer(nullptr, SFX_Laser, false, false, fromPos);

	LegoObject* foundObj = nullptr;
	real32 weaponDist = 1.0f;
	Vector3F weaponDir;
	Gods98::Maths_Vector3DScale(&weaponDir, dir, weaponRange);
	Weapon_DoCallbacksSearch_FUN_00471b20(fromPos, &weaponDir, &foundObj, &weaponDist, liveObj);

	Vector3F toPos;
	Gods98::Maths_RayEndPoint(&toPos, fromPos, &weaponDir, weaponDist);

	Gods98::Container* cont = LegoObject_GetActivityContainer(liveObj);
	Weapon_Lazer_Add(cont, fromPos, &toPos);

	// There must have been more to this function that was stripped by the compiler.
	// This function is filled with so much unexpected redundancy and variable jank:
	// * var_00's with var never being used.
	// * 2(3) null checks for foundObj, each inside the previous check's block.

	if (foundObj != nullptr) {
		// Lazer hitting an object.

		Effect_Spawn_Particle(MISCOBJECT_LAZERHIT, &toPos, nullptr);
		SFX_Random_PlaySound3DOnContainer(nullptr, SFX_LaserHit, false, false, &toPos);

		if (liveObj != nullptr && liveObj->type == LegoObject_MiniFigure) {
			// Hit with direction.
			Point2F hitDir;
			Gods98::Maths_Vector3DMake2D(&hitDir, dir);
			LegoObject_Hit(foundObj, &hitDir, true);
		}
		else {
			// Hit without direction.
			LegoObject_Hit(foundObj, nullptr, true);
		}
	}
	else if (weaponDist < 1.0f) { // Can't hit blocks that are too close(?)
		// Lazer hitting a block.

		Effect_Spawn_BoulderExplode(&toPos);
		SFX_Random_PlaySound3DOnContainer(nullptr, SFX_LaserHit, false, false, &toPos);

		Point2I blockPos = { 0 }; // dummy init
		Map3D_WorldToBlockPos_NoZ(Lego_GetMap(), toPos.x, toPos.y, &blockPos.x, &blockPos.y);

		const Lego_SurfaceType terrain = (Lego_SurfaceType)blockValue(Lego_GetLevel(), blockPos.x, blockPos.y).terrain;

		if (Level_Block_IsWall(blockPos.x, blockPos.y) && !Level_Block_IsImmovable(&blockPos) &&
			(terrain != Lego_SurfaceType_RechargeSeam))
		{
			const real32 destroyTime = Weapon_GetWallDestroyTime(weaponID, terrain);
			if (Level_Block_Damage(blockPos.x, blockPos.y, destroyTime, elapsed)) {

				Level_DestroyWall(Lego_GetLevel(), blockPos.x, blockPos.y, false);
			}
		}
		else if (Level_Block_IsPath(&blockPos)) {
			AITask_DoClearTypeAction(&blockPos, Message_ClearRemovePathComplete);
			Level_BlockUpdateSurface(Lego_GetLevel(), blockPos.x, blockPos.y, 0); // 0 = reserved field
		}
	}

	LegoObject_Weapon_FUN_004375c0(liveObj, weaponID, coef);
	return foundObj;
}

// <LegoRR.exe @00470520>
bool32 __cdecl LegoRR::Weapon_LegoObject_Collision_FUN_00470520(LegoObject* liveObj, const Point2F* fromPos, const Point2F* faceDir, bool32 doTestCollision)
{
	if (StatsObject_GetStatsFlags1(liveObj) & STATS1_COLLBOX) {
		return Weapon_LegoObject_CollisionBox_FUN_00470570(liveObj, fromPos, faceDir, nullptr, doTestCollision);
	}
	else {
		return Weapon_LegoObject_CollisionRadius_FUN_00470800(liveObj, fromPos, faceDir, doTestCollision);
	}
}

// <LegoRR.exe @00470570>
bool32 __cdecl LegoRR::Weapon_LegoObject_CollisionBox_FUN_00470570(LegoObject* liveObj, const Point2F* fromPos, const Point2F* faceDir, OPTIONAL OUT Point2F* intersection, bool32 doTestCollision)
{
	if (doTestCollision && Weapon_LegoObject_TestCollision_FUN_004708f0(liveObj, fromPos, faceDir)) {
		return false;
	}

	Gods98::Container* cont = LegoObject_GetActivityContainer(liveObj);
	const Size2F* box = StatsObject_GetCollBox(liveObj);

	const Vector3F boxSrcTransforms[4] = {
		{ (box->width * -0.5f), 0.0f, (box->height *  0.5f) },
		{ (box->width *  0.5f), 0.0f, (box->height *  0.5f) },
		{ (box->width *  0.5f), 0.0f, (box->height * -0.5f) },
		{ (box->width * -0.5f), 0.0f, (box->height * -0.5f) },
	};

	// 5th element for wrap-around. ALT: Just use [(i + _) % 4] in lower loop.
	Point2F boxPoints[5] = { { 0.0f } }; // dummy init

	for (uint32 i = 0; i < 4; i++) {
		Vector3F boxDstTransform;
		Gods98::Container_Transform(cont, &boxDstTransform, &boxSrcTransforms[i]);
		Gods98::Maths_Vector3DMake2D(&boxPoints[i], &boxDstTransform);
	}
	boxPoints[4] = boxPoints[0]; // Wrap around first and last point.


	for (uint32 i = 0; i < 4; i++) {
		const Point2F* pointA = &boxPoints[i];
		const Point2F* pointB = &boxPoints[i + 1];

		Point2F pointDiff;
		Gods98::Maths_Vector2DSubtract(&pointDiff, pointA, pointB);

		Point2F intersect;
		if (Gods98::Maths_Vector2DIntersection(&intersect, fromPos, faceDir, pointA, &pointDiff) != nullptr) {

			if (std::abs(pointDiff.x) <= std::abs(pointDiff.y)) {

				if ((intersect.y > pointA->y && intersect.y < pointB->y) ||
					(intersect.y < pointA->y && intersect.y > pointB->y))
				{
					if (intersection) *intersection = intersect;
					return true;
				}
			}
			else {
				if ((intersect.x > pointA->x && intersect.x < pointB->x) ||
					(intersect.x < pointA->x && intersect.x > pointB->x))
				{
					if (intersection) *intersection = intersect;
					return true;
				}
			}
		}
	}
	return false;
}

// <LegoRR.exe @00470800>
bool32 __cdecl LegoRR::Weapon_LegoObject_CollisionRadius_FUN_00470800(LegoObject* liveObj, const Point2F* fromPos, const Point2F* faceDir, bool32 doTestCollision)
{
	if (doTestCollision && Weapon_LegoObject_TestCollision_FUN_004708f0(liveObj, fromPos, faceDir)) {
		return false;
	}

	Point2F fromFacePos;
	Gods98::Maths_Vector2DAdd(&fromFacePos, fromPos, faceDir);

	Point2F toPos = { 0.0f }; // dummy init
	LegoObject_GetPosition(liveObj, &toPos.x, &toPos.y);

	Point2F collPos;
	Collision_PointOnLine(fromPos, &fromFacePos, &toPos, &collPos);

	const real32 collRadius = StatsObject_GetCollRadius(liveObj);
	return (Gods98::Maths_Vector2DDistance(&collPos, &toPos) < collRadius);
}

// <LegoRR.exe @004708f0>
bool32 __cdecl LegoRR::Weapon_LegoObject_TestCollision_FUN_004708f0(LegoObject* liveObj, const Point2F* fromPos, const Point2F* faceDir)
{
	Point2F toPos = { 0.0f }; // dummy init
	LegoObject_GetPosition(liveObj, &toPos.x, &toPos.y);

	Gods98::Maths_Vector2DSubtract(&toPos, &toPos, fromPos);
	return (Gods98::Maths_Vector2DDotProduct(&toPos, faceDir) < 0.0f);
}

// <LegoRR.exe @00470950>
void __cdecl LegoRR::Weapon_Lazer_Add(Gods98::Container* cont, const Vector3F* fromPos, const Vector3F* toPos)
{
	const sint32 handle = Weapon_Lazer_GetNextAvailable();
	if (handle != -1) {
		Weapon_Lazer* lazer = &weaponGlobs.lazerList[handle];

		// (MESH_TRANSFORM_FLAG_IDENTITY|MESH_RENDER_FLAG_ALPHA11)  (0x4400)
		const Gods98::Mesh_RenderFlags flags = (Gods98::Mesh_RenderFlags::MESH_TRANSFORM_FLAG_IDENTITY|Gods98::Mesh_RenderFlags::MESH_RENDER_FLAG_ALPHA11);
		lazer->innerMesh = Gods98::Mesh_CreateOnFrame(cont->activityFrame, nullptr, flags, nullptr, Gods98::Mesh_Type::Norm);
		lazer->outerMesh = Gods98::Mesh_CreateOnFrame(cont->activityFrame, nullptr, flags, nullptr, Gods98::Mesh_Type::Norm);

		Weapon_Lazer_InitMesh(lazer->innerMesh, 0.3f, fromPos, toPos, 0.6f, 0.6f, 0.6f, 1.0f);
		Weapon_Lazer_InitMesh(lazer->outerMesh, 1.0f, fromPos, toPos, 0.1f, 0.2f, 0.5f, 1.0f);

		lazer->cont = cont;
		lazer->timer = 5.0f; // Lasts for 5 frames.
	}
}

// <LegoRR.exe @00470a20>
void __cdecl LegoRR::Weapon_Lazer_InitMesh(Gods98::Mesh* mesh, real32 thickness, const Vector3F* fromPos, const Vector3F* toPos, real32 red, real32 green, real32 blue, real32 alpha)
{
	real32 TEXT_COORDS[16][2] = {
		{ 0.0f, 1.0f },
		{ 0.0f, 0.0f },
		{ 1.0f, 0.0f },
		{ 1.0f, 1.0f },
		{ 0.0f, 1.0f },
		{ 0.0f, 0.0f },
		{ 1.0f, 0.0f },
		{ 1.0f, 1.0f },
		{ 0.0f, 1.0f },
		{ 0.0f, 0.0f },
		{ 1.0f, 0.0f },
		{ 1.0f, 1.0f },
		{ 0.0f, 1.0f },
		{ 0.0f, 0.0f },
		{ 1.0f, 0.0f },
		{ 1.0f, 1.0f },
	};

	const uint32 FACE_DATA[48] = {
		 0,  1,  9,
		 9,  8,  0,
		 1,  2, 10,
		10,  9,  1,
		 2,  3, 11,
		11, 10,  2,
		 3,  4, 12,
		12, 11,  3,
		 4,  5, 13,
		13, 12,  4,
		 5,  6, 14,
		14, 13,  5,
		 6,  7, 15,
		15, 14,  6,
		 7,  0,  8,
		 8, 15,  7,
	};

	/// FIX APPLY: Yet another scenario where divide by zero can occur, originally these 0.0 checks were almost worthless...
	Vector3F distPos;
	Gods98::Maths_Vector3DSubtract(&distPos, toPos, fromPos);

	// I have absolutely no clue what this vertex position math is doing.
	// At least the variables are now labeled by axis...

	real32 local_400, local_404;
	if (distPos.y == 0.0f && distPos.z == 0.0f) {
		if (distPos.x != 0.0f) {
			Gods98::Maths_Vector3DNormalize(&distPos);
		}
		local_400 = 1.0f;
		local_404 = 0.0f;
	}
	else if (distPos.y == 0.0f) {
		Gods98::Maths_Vector3DNormalize(&distPos);
		local_400 = 0.0f;
		local_404 = 1.0f;
	}
	else {
		Gods98::Maths_Vector3DNormalize(&distPos);
		local_400 = std::sqrt(1.0f / ((distPos.z * distPos.z) / (distPos.y * distPos.y) + 1.0f));
		local_404 = -((distPos.z / distPos.y) * local_400);
	}


	const real32 x1_a = ((local_400 * distPos.y) - (local_404 * distPos.z)) * thickness;
	const real32 y1_a = -(local_400 * distPos.x) * thickness;
	const real32 z1_a = (local_404 * distPos.x) * thickness;
	const real32 y0_b = local_404 * thickness;
	const real32 z0_b = local_400 * thickness;
	const real32 x0_a = x1_a * 0.4142136f;
	const real32 y0_a = y1_a * 0.4142136f;
	const real32 z0_a = z1_a * 0.4142136f;
	const real32 y1_b = y0_b * 0.4142136f;
	const real32 z1_b = z0_b * 0.4142136f;

	const Vector3F BASE_VERTPOSES[8] = {
		{ (x0_a + 0.0f), (y0_a + y0_b), (z0_a + z0_b) },
		{ (x1_a + 0.0f), (y1_a + y1_b), (z1_a + z1_b) },
		{ (x1_a - 0.0f), (y1_a - y1_b), (z1_a - z1_b) },
		{ (x0_a - 0.0f), (y0_a - y0_b), (z0_a - z0_b) },
		{ (-x0_a - 0.0f), (-y0_a - y0_b), (-z0_a - z0_b) },
		{ (-x1_a - 0.0f), (-y1_a - y1_b), (-z1_a - z1_b) },
		{ (-x1_a + 0.0f), (-y1_a + y1_b), (-z1_a + z1_b) },
		{ (-x0_a + 0.0f), (-y0_a + y0_b), (-z0_a + z0_b) },
	};

	Vector3F vertPoses[16] = { { 0.0f } }; // dummy init
	Vector3F normals[8] = { { 0.0f } }; // dummy init
	for (uint32 i = 0; i < 8; i++) {
		// Pointlessly assigning normals > index 0, maybe in the past each vertex had a different normal.
		normals[i] = vertPoses[i];
		Gods98::Maths_Vector3DNormalize(&normals[i]);
		Gods98::Maths_Vector3DAdd(&vertPoses[i + 8], &BASE_VERTPOSES[i], toPos);
		Gods98::Maths_Vector3DAdd(&vertPoses[i + 0], &BASE_VERTPOSES[i], fromPos);
	}

	const sint32 groupID = Gods98::Mesh_AddGroup(mesh, 16, 16, 3, FACE_DATA);
	Gods98::Mesh_SetGroupColour(mesh, groupID, red, green, blue, Gods98::Mesh_Colour::Emissive);
	Gods98::Mesh_SetGroupMaterialValues(mesh, groupID, alpha, Gods98::Mesh_Colour::Alpha);
	Gods98::Mesh_SetVertices_SameNormal(mesh, groupID, 0, 16, vertPoses, &normals[0], TEXT_COORDS);
}

// <LegoRR.exe @00471580>
LegoRR::WeaponsModel* __cdecl LegoRR::Weapon_LegoObject_GetWeaponsModel(LegoObject* liveObj)
{
	switch (liveObj->type) {
	case LegoObject_Vehicle:
		return &liveObj->vehicle->weapons;

	case LegoObject_Building:
		return &liveObj->building->weapons;

	default:
		return nullptr;
	}
}

// <LegoRR.exe @004715b0>
real32 __cdecl LegoRR::Weapon_LegoObject_GetWeaponTimer(LegoObject* liveObj, uint32 frameNo)
{
	const WeaponsModel* weapons = Weapon_LegoObject_GetWeaponsModel(liveObj);
	return weapons->timers[frameNo];
}

// <LegoRR.exe @004715d0>
void __cdecl LegoRR::Weapon_LegoObject_SetWeaponTimer(LegoObject* liveObj, real32 timer, uint32 frameNo)
{
	WeaponsModel* weapons = Weapon_LegoObject_GetWeaponsModel(liveObj);
	weapons->timers[frameNo] = timer;
}

// Only Z value of the two vector parameters is used.
// <LegoRR.exe @004715f0>
bool32 __cdecl LegoRR::Weapon_MathUnk_CheckVectorsZScalar_InRange(const Vector3F* vectorPos, const Vector3F* vectorScaled, real32 minZ, real32 maxZ, real32 scalar)
{
	const real32 scaledZ = vectorPos->z + vectorScaled->z * scalar;
	return (scaledZ <= maxZ && scaledZ >= maxZ - minZ);
}

// DATA: SearchWeapons18_2*
// <LegoRR.exe @00471630>
bool32 __cdecl LegoRR::Weapon_LegoObject_Callback_FUN_00471630(LegoObject* liveObj, void* pSearch)
{
	SearchWeapons18_2* search = (SearchWeapons18_2*)pSearch;

	Point2F searchFromPos;
	Point2F searchDistance;
	Gods98::Maths_Vector3DMake2D(&searchFromPos, search->fromPos);
	Gods98::Maths_Vector3DMake2D(&searchDistance, search->fromToDistance);

	const real32 length1 = Gods98::Maths_Vector2DModulus(&searchDistance);

	const real32 EPSILON = 0.00001f; // 1e-05f

	if (liveObj != search->ignoreObject && (length1 <= -EPSILON || length1 >= EPSILON)) { // ~!= 0

		Point2F thisPos = { 0.0f }; // dummy init
		LegoObject_GetPosition(liveObj, &thisPos.x, &thisPos.y);

		const real32 worldZ = Map3D_GetWorldZ(Lego_GetMap(), thisPos.x, thisPos.y);

		Point2F collPos;
		Collision_PointOnLineRay(&searchFromPos, &searchDistance, &thisPos, &collPos);

		const real32 collRadius = StatsObject_GetCollRadius(liveObj);
		if (Gods98::Maths_Vector2DDistance(&thisPos, &collPos) <= collRadius) {

			Point2F local_20;
			Gods98::Maths_Vector2DSubtract(&local_20, &collPos, &searchFromPos);
			const real32 length2 = Gods98::Maths_Vector2DModulus(&local_20);
			if (length2 > -EPSILON && length2 < EPSILON) { // ~== 0
				
				const real32 collHeight = StatsObject_GetCollHeight(liveObj);
				if (Weapon_MathUnk_CheckVectorsZScalar_InRange(search->fromPos, search->fromToDistance, collHeight, worldZ, 0.0f)) {

					search->success = true;
					search->ref_float_4 = 0.0f;
					search->foundObject = liveObj;
					return false; // Return early (function always returns false)
				}
			}

			real32 fAxis;
			if ((searchDistance.x <= -EPSILON || searchDistance.x >= EPSILON) || (local_20.x <= -EPSILON || local_20.x >= EPSILON)) { // ~!= 0
				
				fAxis = searchDistance.x / local_20.x;
			}
			else if ((searchDistance.y <= -EPSILON || searchDistance.y >= EPSILON) || (local_20.y <= -EPSILON || local_20.y >= EPSILON)) { // ~!= 0

				fAxis = searchDistance.y / local_20.y;
			}
			else { //if ((searchDistance.y > -EPSILON && searchDistance.y < EPSILON) && (local_20.y > -EPSILON && local_20.y < EPSILON)) { // ~== 0

				return false; // Return early (function always returns false)
			}

			real32 fScalar = (length2 / length1);
			if (fAxis < 0.0f) {
				fScalar *= -1.0f;
			}
			if (fScalar > 0.0f && fScalar < search->ref_float_4) {

				const real32 collHeight = StatsObject_GetCollHeight(liveObj);
				if (Weapon_MathUnk_CheckVectorsZScalar_InRange(search->fromPos, search->fromToDistance, collHeight, worldZ, fScalar)) {

					search->success = true;
					search->ref_float_4 = fScalar;
					search->foundObject = liveObj;
				}
			}
		}
	}
	return false;
}

// <LegoRR.exe @004718f0>
void __cdecl LegoRR::Weapon_LegoObject_FUN_004718f0(SearchWeapons18_2* search)
{
	Vector3F rayEnd;
	Gods98::Maths_Vector3DAdd(&rayEnd, search->fromPos, search->fromToDistance);

	Map3D* map = Lego_GetMap();

	uint32 bx1 = 0; // dummy inits
	uint32 by1 = 0;
	if (!Map3D_WorldToBlockPos_NoZ(map, search->fromPos->x, search->fromPos->y, (sint32*)&bx1, (sint32*)&by1)) {
		if (bx1 >= map->blockWidth) {
			bx1 = map->blockWidth - 1;
		}
		if (by1 >= map->blockHeight) {
			by1 = map->blockHeight - 1;
		}
	}

	uint32 bx2 = 0; // dummy inits
	uint32 by2 = 0;
	if (!Map3D_WorldToBlockPos_NoZ(map, rayEnd.x, rayEnd.y, (sint32*)&bx2, (sint32*)&by2)) {
		if (bx2 >= map->blockWidth) {
			bx2 = map->blockWidth - 1;
		}
		if (by2 >= map->blockHeight) {
			by2 = map->blockHeight - 1;
		}
	}

	if (bx2 < bx1) {
		const uint32 swapx2 = bx2;
		bx2 = bx1;
		bx1 = swapx2;
	}
	if (by2 < by1) {
		const uint32 swapy2 = by2;
		by2 = by1;
		by1 = swapy2;
	}

	for (uint32 bx = bx1; bx <= bx2; bx++) {
		for (uint32 by = by1; by <= by2; by++) {

			Vector3F blockIntersection;
			if (Map3D_Intersections_Sub2_FUN_004518a0(Lego_GetMap(), bx, by, search->fromPos, search->fromToDistance, &blockIntersection)) {

				const real32 length1 = Gods98::Maths_Vector3DModulus(search->fromToDistance);
				const real32 length2 = Gods98::Maths_Vector3DDistance(&blockIntersection, search->fromPos);
				const real32 fScalar = length2 / length1;
				if (fScalar < search->ref_float_4) {
					search->success = true;
					search->ref_float_4 = fScalar;
					search->foundObject = nullptr;
				}
			}
		}
	}
}

// <LegoRR.exe @00471b20>
bool32 __cdecl LegoRR::Weapon_DoCallbacksSearch_FUN_00471b20(const Vector3F* vecCector, const Vector3F* vecDistance, OUT LegoObject** liveObj, IN OUT real32* float_4, OPTIONAL LegoObject* ignoreObj)
{
	SearchWeapons18_2 search = { 0 }; // dummy init

	search.success = false;
	search.ref_float_4 = *float_4;
	search.foundObject = nullptr;
	search.ignoreObject = ignoreObj;
	search.fromPos = vecCector;
	search.fromToDistance = vecDistance;

	for (auto obj : objectListSet.EnumerateSkipUpgradeParts()) {
		Weapon_LegoObject_Callback_FUN_00471630(obj, &search);
	}
	//LegoObject_RunThroughListsSkipUpgradeParts(Weapon_LegoObject_Callback_FUN_00471630, &search);

	Weapon_LegoObject_FUN_004718f0(&search);
	
	*float_4 = search.ref_float_4;
	*liveObj = search.foundObject;
	return search.success;
}

// <LegoRR.exe @00471b90>
bool32 __cdecl LegoRR::Weapon_LegoObject_DoCallbacksSearch_FUN_00471b90(LegoObject* liveObj, LegoObject* otherObj)
{
	Vector3F collCenter1;
	Vector3F collCenter2;
	Weapon_LegoObject_GetCollCenterPosition(liveObj, &collCenter1);
	Weapon_LegoObject_GetCollCenterPosition(otherObj, &collCenter2);

	Vector3F collDiff;
	Gods98::Maths_Vector3DSubtract(&collDiff, &collCenter2, &collCenter1);

	real32 scalar = 1.0f;
	LegoObject* foundObj;
	bool32 success = Weapon_DoCallbacksSearch_FUN_00471b20(&collCenter1, &collDiff, &foundObj, &scalar, liveObj);
	return (success && foundObj != otherObj);
}

// <LegoRR.exe @00471c20>
bool32 __cdecl LegoRR::Weapon_LegoObject_SeeThroughWalls_FUN_00471c20(LegoObject* liveObj, LegoObject* otherObj)
{
	/// FIXME: Store this CFG value for access, so that we can properly fiddle with config constants.
	// SeeThroughWalls property defaults to true.
	// I guess the code below this is to calculate sight when the object can't see through walls.
	if (Lego_IsSeeThroughWalls()) {
		return false;
	}

	Vector3F collCenter1;
	Vector3F collCenter2;
	Vector3F collDiff;

	Weapon_LegoObject_GetCollCenterPosition(liveObj, &collCenter1); // => search.fromPos
	Weapon_LegoObject_GetCollCenterPosition(otherObj, &collCenter2);

	Gods98::Maths_Vector3DSubtract(&collDiff, &collCenter2, &collCenter1); // => search.fromToDistance

	SearchWeapons18_2 search = { 0 }; // dummy init

	search.success = false;
	search.ref_float_4 = 1.0f;
	search.foundObject = nullptr;
	search.ignoreObject = nullptr;
	search.fromPos = &collCenter1;
	search.fromToDistance = &collDiff;

	Weapon_LegoObject_FUN_004718f0(&search);
	return search.success;
}

// <LegoRR.exe @00471ce0>
Gods98::Container* __cdecl LegoRR::Weapon_GetFireNull(const WeaponsModel* weapons, uint32 frameNo, uint32 pairNo)
{
	return weapons->fireNullPairs[frameNo][pairNo];
}

// <LegoRR.exe @00471d00>
Gods98::Container* __cdecl LegoRR::Weapon_GetXPivotNull(const WeaponsModel* weapons, uint32 frameNo)
{
	return weapons->xPivotNulls[frameNo];
}

// <LegoRR.exe @00471d10>
Gods98::Container* __cdecl LegoRR::Weapon_GetYPivotNull(const WeaponsModel* weapons, uint32 frameNo)
{
	return weapons->yPivotNulls[frameNo];
}

// targetWorldPos is required if assignTarget is true.
// <LegoRR.exe @00471d20>
void __cdecl LegoRR::Weapon_PivotTracker(WeaponsModel* weapons, OPTIONAL const Vector3F* targetWorldPos, real32 elapsed, bool32 assignTarget, uint32 frameNo)
{
	if (assignTarget) {
		weapons->targetWorldPos[frameNo] = *targetWorldPos;
	}

	Gods98::Container* xPivotNull = Weapon_GetXPivotNull(weapons, frameNo);

	const real32 EPSILON = 0.00001f; // 1e-05f

	if (xPivotNull != nullptr && elapsed > EPSILON) {

		Vector3F pivotPos;
		Gods98::Container_GetPosition(xPivotNull, nullptr, &pivotPos);

		Gods98::Maths_Vector3DSubtract(&pivotPos, &weapons->targetWorldPos[frameNo], &pivotPos);
		Gods98::Maths_Vector3DNormalize(&pivotPos);

		const real32 timeScalar = 5.0f / elapsed;
		Gods98::Maths_Vector3DScale(&weapons->fireDirections[frameNo], &weapons->fireDirections[frameNo], timeScalar);
		Gods98::Maths_Vector3DAdd(&weapons->fireDirections[frameNo], &weapons->fireDirections[frameNo], &pivotPos);
		Gods98::Maths_Vector3DNormalize(&weapons->fireDirections[frameNo]);


		Gods98::Container* yPivotNull = Weapon_GetYPivotNull(weapons, frameNo);
		if (yPivotNull != nullptr) {
			Gods98::Container_SetOrientation(yPivotNull, nullptr,
											 weapons->fireDirections[frameNo].x, weapons->fireDirections[frameNo].y, 0.0f,
											 0.0f, 0.0f, -1.0f);
		}
		Gods98::Container_SetOrientation(xPivotNull, nullptr,
										 weapons->fireDirections[frameNo].x, weapons->fireDirections[frameNo].y, weapons->fireDirections[frameNo].z,
										 0.0f, 0.0f, -1.0f);
	}
}

// <LegoRR.exe @00471f30>
void __cdecl LegoRR::Weapon_GetFireDirection(const WeaponsModel* weapons, OUT Vector3F* dir, uint32 frameNo)
{
	*dir = weapons->fireDirections[frameNo];
}

// <LegoRR.exe @00471f60>
bool32 __cdecl LegoRR::Weapon_LegoObject_GetCollCenterPosition(LegoObject* liveObj, OUT Vector3F* vector)
{
	Gods98::Container* cont = LegoObject_GetActivityContainer(liveObj);
	Gods98::Container_GetPosition(cont, nullptr, vector);

	vector->z -= StatsObject_GetCollHeight(liveObj) / 2.0f; // Raise Z to center of collision box.
	return true;
}

// <LegoRR.exe @00471fa0>
bool32 __cdecl LegoRR::Weapon_LegoObject_IsActiveWithTracker(LegoObject* liveObj)
{
	if (StatsObject_GetStatsFlags2(liveObj) & STATS2_TRACKER) {
		return LegoObject_IsActive(liveObj, (liveObj->type != LegoObject_Building)); // Check powered state for buildings only.
	}
	return false;
}


// <LegoRR.exe @00471fe0>
bool32 __cdecl LegoRR::Weapon_LegoObject_UpdateSelectedTracker(LegoObject* liveObj, real32 elapsed, uint32 frameNo)
{
	WeaponsModel* weapons = Weapon_LegoObject_GetWeaponsModel(liveObj);

	Vector3F mouseWorldPos;
	Lego_GetMouseWorldPosition(&mouseWorldPos);

	Weapon_PivotTracker(weapons, &mouseWorldPos, elapsed, true, frameNo);

	// Is a position in the world clicked, and the weapon ready to use?
	if (Gods98::mslb() && (legoGlobs.flags2 & GAME2_MOUSE_INSIDEGAMEVIEW) &&
		Weapon_LegoObject_GetWeaponTimer(liveObj, frameNo) <= 0.0f)
	{
		// Don't shoot if the selection point is our current tracker object.
		LegoObject* selectedObj;
		bool selected = LegoObject_DoPickSphereSelection(Gods98::msx(), Gods98::msy(), &selectedObj);
		if (!selected || selectedObj != liveObj) {

			for (uint32 pairNo = 0; pairNo < WEAPON_MAXFIRES; pairNo++) {
				// It's unclear if any of the below functions will change this value, so make sure to check every loop.
				if (Level_GetCrystalCount(true) == 0) {
					break; // No crystals, can't fire.
				}

				Gods98::Container* fireNull = Weapon_GetFireNull(weapons, frameNo, pairNo);
				if (fireNull == nullptr) {
					// There's one or zero fire nulls, skip the rest of the loop.
					break; // End of function
				}

				const uint32 weaponID = Weapon_GetWeaponIDByName(weapons->weaponParts[frameNo]->weaponName);

				if (pairNo == 0) {
					// Logic that only needs to be handled once for both pairs.
					const real32 rechargeTime = Weapon_GetRechargeTime(weaponID);
					Weapon_LegoObject_SetWeaponTimer(liveObj, rechargeTime, frameNo);

					Lego_SetPointerSFX(PointerSFX_Okay);
				}

				const real32 weaponRange = Weapon_GetWeaponRange(weaponID);
				real32 coef = StatsObject_GetFunctionCoef(liveObj);
				if (coef == 0.0f) coef = 1.0f; // Default coef

				Vector3F firePos, fireDir;
				Weapon_GetFireDirection(weapons, &fireDir, frameNo);

				Gods98::Container_GetPosition(fireNull, nullptr, &firePos);

				LegoObject* projectileObj = Weapon_FireLazer(&firePos, &fireDir, liveObj, elapsed, weaponRange, weaponID, coef);
				if (projectileObj != nullptr) {
					Point2F fireDir2D;// = { fireDir.x, fireDir.y };
					Gods98::Maths_Vector3DMake2D(&fireDir2D, &fireDir);

					Weapon_GenericDamageObject(projectileObj, weaponID, true, 0.0f, &fireDir2D);
					liveObj->projectileObject = projectileObj;
				}
			}
		}
	}
	return true;
}

// <LegoRR.exe @004721c0>
bool32 __cdecl LegoRR::Weapon_LegoObject_UpdateUnselectedTracker(LegoObject* liveObj, real32 elapsed, uint32 frameNo)
{
	Gods98::Container* cont = LegoObject_GetActivityContainer(liveObj);
	if (cont != nullptr) {
		Vector3F wPos, dir, up;
		Gods98::Container_GetOrientation(cont, nullptr, &dir, &up);
		Gods98::Container_GetPosition(cont, nullptr, &wPos);

		Vector3F targetWorldPos;
		Gods98::Maths_RayEndPoint(&targetWorldPos, &wPos, &dir, 10000.0f);
		WeaponsModel* weapons = Weapon_LegoObject_GetWeaponsModel(liveObj);
		Weapon_PivotTracker(weapons, &targetWorldPos, elapsed, true, frameNo);
	}
	return true;
}

// <LegoRR.exe @00472280>
bool32 __cdecl LegoRR::Weapon_LegoObject_UpdateTracker(LegoObject* liveObj, real32 elapsed)
{
	const WeaponsModel* weapons = Weapon_LegoObject_GetWeaponsModel(liveObj);
	if (liveObj->flags4 & LIVEOBJ4_LASERTRACKERMODE) {
		for (uint32 frameNo = 0; frameNo < weapons->weaponCount; frameNo++) {
			Weapon_LegoObject_UpdateSelectedTracker(liveObj, elapsed, frameNo);
		}
	}
	else {
		for (uint32 frameNo = 0; frameNo < weapons->weaponCount; frameNo++) {
			Weapon_LegoObject_UpdateUnselectedTracker(liveObj, elapsed, frameNo);
		}
	}
	return true;
}

// <LegoRR.exe @00472320>
bool32 __cdecl LegoRR::Weapon_Callback_RemoveProjectileReference(LegoObject* liveObj, LegoObject* projectileObj)
{
	if (liveObj->projectileObject == projectileObj) {
		liveObj->projectileObject = nullptr;
	}
	return false;
}

// <LegoRR.exe @00472340>
bool32 __cdecl LegoRR::Weapon_LegoObject_WithinWeaponRange(LegoObject* liveObj, LegoObject* otherObj)
{
	const Weapon_KnownType knownWeaponType = LegoObject_GetEquippedBeam(liveObj);
	if (liveObj != nullptr && otherObj != nullptr && knownWeaponType != Weapon_KnownType_None) {
		const real32 weaponRange = Weapon_GetWeaponRange(knownWeaponType);

		// This might be some inlined collision distance-to function.
		Vector3F thisPos, otherPos;
		Weapon_LegoObject_GetCollCenterPosition(liveObj, &thisPos);
		Weapon_LegoObject_GetCollCenterPosition(otherObj, &otherPos);

		return (Gods98::Maths_Vector3DDistance(&otherPos, &thisPos) < weaponRange);
	}
	return false;
}

// <LegoRR.exe @004723f0>
bool32 __cdecl LegoRR::Weapon_LegoObject_WithinAwarenessRange(LegoObject* liveObj, LegoObject* otherObj)
{
	const Weapon_KnownType knownWeaponType = LegoObject_GetEquippedBeam(liveObj);
	if (liveObj != nullptr && otherObj != nullptr && knownWeaponType != Weapon_KnownType_None) {
		const real32 awarenessRange = StatsObject_GetAwarenessRange(liveObj);

		// This might be some inlined collision distance-to function.
		Vector3F thisPos, otherPos;
		Weapon_LegoObject_GetCollCenterPosition(liveObj, &thisPos);
		Weapon_LegoObject_GetCollCenterPosition(otherObj, &otherPos);

		return (Gods98::Maths_Vector3DDistance(&otherPos, &thisPos) < awarenessRange);
	}
	return false;
}

#pragma endregion
