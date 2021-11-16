// Effects.h : 
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

#pragma endregion

/**********************************************************************************
 ******** Constants
 **********************************************************************************/

#pragma region Constants

#define EFFECT_ELECTRICFENCE_MAXBEAMINSTANCES	30
#define EFFECT_MISC_MAXINSTANCES				10

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

struct EffectElectricFenceBeam // [LegoRR/Effects.c|struct:0xf4]
{
	/*00,4*/	Gods98::Container* cont; // LWS:true
	/*04,78*/	Gods98::Container* contTable[EFFECT_ELECTRICFENCE_MAXBEAMINSTANCES];
	/*7c,78*/	bool32 finishedTable[EFFECT_ELECTRICFENCE_MAXBEAMINSTANCES];
	/*f4*/
};
assert_sizeof(EffectElectricFenceBeam, 0xf4);


struct EffectMisc // [LegoRR/Effects.c|struct:0x38]
{
	/*00,28*/	Gods98::Container* contTable[EFFECT_MISC_MAXINSTANCES];
	/*28,4*/	uint32 count; // (max of 10)
	/*2c,4*/	undefined4 field_2c;
	/*30,4*/	Gods98::Container* cont; // LWS,true | LWO,true
	/*34,4*/	undefined4 field_34;
	/*38*/
};
assert_sizeof(EffectMisc, 0x38);


struct EffectRockFall // [LegoRR/Effects.c|struct:0xa4]
{
	/*00,4*/	Gods98::Container* cont; // LWS:true, or ANIM:true
	/*04,10*/	Gods98::Container* contTable[4];
	/*14,10*/	uint32 xBlockPosTable[4];
	/*24,10*/	uint32 yBlockPosTable[4];
	/*34,10*/	bool32 finishedTable[4];
	/*44,4*/	char* itemName; // (temporary stack buffer, BROKEN AF)
	/*48,4*/	sint32 itemFrameCount;
	/*4c,48*/	undefined field_0x4c_0x93[72];
	/*94,10*/	undefined4 bitfieldTable_94[4];
	/*a4*/
};
assert_sizeof(EffectRockFall, 0xa4);


struct Effect_Globs // [LegoRR/Effects.c|struct:0xc78|tags:GLOBS]
{
	/*000,30*/	uint32 rockFallCompletedTypes[12]; // (size not related to [style:3])
	/*030,30*/	uint32 rockFallCompletedIndexes[12]; // (size not related to [style:3])
	/*060,7b0*/	EffectRockFall rockFallEffects[12]; // [style:3][type:4] LWS:true, or ANIM:true
	/*810,10*/	char* rockFallStyleName[4];
	/*820,4*/	uint32 rockFallStyleCount;
	/*824,4*/	sint32 rockFallStyleIndex;
	/*828,38*/	EffectMisc lazerHitEffect; // LWS:true, or LWO:true
	/*860,38*/	EffectMisc pusherHitEffect; // LWS:true, or LWO:true
	/*898,38*/	EffectMisc freezerHitEffect; // LWS:true, or LWO:true
	/*8d0,38*/	EffectMisc pathDustEffect; // LWS:true, or LWO:true
	/*908,e0*/	EffectMisc lavaErosionSmokeEffects[4]; // [1-4] LWS:true, or LWO:true
	/*9e8,38*/	EffectMisc birdScarerEffect; // LWS:true, or LWO:true
	/*a20,38*/	EffectMisc upgradeEffect; // LWS:true, or LWO:true
	/*a58,10*/	Gods98::Container* boulderExplodeContTable[4];
	/*a68,10*/	Gods98::Container* smashPathContTable[4];
	/*a78,1e8*/	EffectElectricFenceBeam efenceEffects[2]; // [shortBeam,longBeam]
	/*c60,4*/	Gods98::Container* explosionCont; // LWS:true
	/*c64,10*/	Gods98::Container* explosionContTable[4];
	/*c74,4*/	uint32 explosionCount;
	/*c78*/
};
assert_sizeof(Effect_Globs, 0xc78);

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @004c8180>
extern Effect_Globs & effectGlobs;

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @0040bcf0>
#define Effect_StopAll ((void (__cdecl* )(void))0x0040bcf0)

// <LegoRR.exe @0040bd10>
#define Effect_Spawn_BoulderExplode_AtObjectOther ((void (__cdecl* )(LegoObject* liveObj))0x0040bd10)

// <LegoRR.exe @0040bd40>
#define Effect_Spawn_BoulderExplode ((void (__cdecl* )(const Vector3F* position))0x0040bd40)

// Must pass a non-null LegoObject* -OR- Vector3F* in order to get a position. (LegoObject* has precedence)
// <LegoRR.exe @0040bde0>
#define Effect_Spawn_SmashPath ((void (__cdecl* )(OPTIONAL LegoObject* liveObj, OPTIONAL const Vector3F* position))0x0040bde0)

// <LegoRR.exe @0040bea0>
#define Effect_GetRockFallStyle ((bool32 (__cdecl* )(const char* name, OUT uint32* index))0x0040bea0)

// <LegoRR.exe @0040bef0>
#define Effect_SetRockFallStyle ((void (__cdecl* )(sint32 rockFallStyleIndex))0x0040bef0)

// <LegoRR.exe @0040bf00>
#define Effect_Load_RockFallStylesAll ((void (__cdecl* )(const Gods98::Config* root, const char* rootPath, Gods98::Container* contRoot))0x0040bf00)

// <LegoRR.exe @0040c000>
#define Effect_Load_RockFallStyle ((bool32 (__cdecl* )(Gods98::Container* contRoot, const char* filename, sint32 rockFallStyle, sint32 rockFallType, const char* itemName))0x0040c000)

// <LegoRR.exe @0040c0e0>
#define Effect_Load_ElectricFenceBeam ((bool32 (__cdecl* )(Gods98::Container* resRoot, const char* filename, bool32 isLongBeam))0x0040c0e0)

// <LegoRR.exe @0040c160>
#define Effect_Spawn_RockFall ((bool32 (__cdecl* )(RockFallType rockFallType, sint32 bx, sint32 by, real32 x, real32 y, real32 z, real32 dirX, real32 dirY))0x0040c160)

// <LegoRR.exe @0040c220>
#define Effect_ElectricFenceBeam_Spawn ((bool32 (__cdecl* )(bool32 longBeam, real32 xPos, real32 yPos, real32 zPos, real32 xDir, real32 yDir, real32 zDir))0x0040c220)

// <LegoRR.exe @0040c2d0>
#define Effect_UpdateAll ((uint32 (__cdecl* )(real32 elapsedGame, OUT uint32** rockFallTypes, OLUT uint32** rockFallSubtypes))0x0040c2d0)

// <LegoRR.exe @0040c400>
#define Effect_Update_BoulderExplode ((void (__cdecl* )(real32 elapsedGame))0x0040c400)

// <LegoRR.exe @0040c450>
#define Effect_Update_SmashPath ((void (__cdecl* )(real32 elapsedGame))0x0040c450)

// <LegoRR.exe @0040c4a0>
#define Effect_GetBlockPos_RockFall ((void (__cdecl* )(RockFallType rockFallType, sint32 index, OUT uint32* bx, OUT uint32* by))0x0040c4a0)

// <LegoRR.exe @0040c4e0>
#define Effect_Unk_RockFall_FUN_0040c4e0 ((Gods98::Container* (__cdecl* )(uint32 bx, uint32 by))0x0040c4e0)

// <LegoRR.exe @0040c5c0>
#define Effect_RemoveAll_BoulderExplode ((void (__cdecl* )(void))0x0040c5c0)

// <LegoRR.exe @0040c5f0>
#define Effect_RemoveAll_RockFall ((void (__cdecl* )(void))0x0040c5f0)

// <LegoRR.exe @0040c650>
#define Effect_Load_Explosion ((void (__cdecl* )(Gods98::Container* contRoot, const char* filename))0x0040c650)

// Must pass a non-null LegoObject* -OR- Vector3F* in order to get a position. (LegoObject* has precedence)
// <LegoRR.exe @0040c680>
#define Effect_Spawn_Explosion ((void (__cdecl* )(OPTIONAL LegoObject* liveObj, OPTIONAL const Point2F* position))0x0040c680)

// <LegoRR.exe @0040c760>
#define Effect_Update_Explosion ((void (__cdecl* )(real32 elapsedGame))0x0040c760)

// <LegoRR.exe @0040c7d0>
#define Effect_GetMiscEffectData ((EffectMisc* (__cdecl* )(MiscEffectType miscEffectType))0x0040c7d0)

// <LegoRR.exe @0040c850>
#define Effect_Load_Misc ((bool32 (__cdecl* )(EffectMisc* effectMisc, Gods98::Container* contRoot, const char* filename))0x0040c850)

// <LegoRR.exe @0040c8c0>
#define Effect_Initialise ((void (__cdecl* )(const Gods98::Config* root, const char* rootPath, Gods98::Container* cont))0x0040c8c0)

// <LegoRR.exe @0040caa0>
#define Effect_Update_MiscEffect ((void (__cdecl* )(EffectMisc* miscEffect, real32 elapsedGame))0x0040caa0)

// <LegoRR.exe @0040cb70>
#define Effect_Update_MiscEffectsAll ((void (__cdecl* )(real32 elapsedGame))0x0040cb70)

// <LegoRR.exe @0040cc10>
#define Effect_Spawn_Particle ((bool32 (__cdecl* )(MiscEffectType miscEffectType, const Vector3F* position, OPTIONAL const Vector3F* dir))0x0040cc10)

#pragma endregion

}
