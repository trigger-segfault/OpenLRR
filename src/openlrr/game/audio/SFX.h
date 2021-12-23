// SFX.h : 
//

#pragma once

#include "../../common.h"
#include "../../engine/gfx/Containers.h"
#include "../../engine/geometry.h"
#include "../../engine/undefined.h"

#include "../../engine/audio/Sound.h"


/**********************************************************************************
 ******** Forward Global Namespace Declarations
 **********************************************************************************/

#pragma region Forward Declarations

struct IDirect3DRMFrame3;

#pragma endregion


namespace LegoRR
{; // !<---

/**********************************************************************************
 ******** Constants
 **********************************************************************************/

#pragma region Constants

#define SFX_MAXSAMPLES          495
#define SFX_MAXSAMPLEGROUPS     200

#pragma endregion

/**********************************************************************************
 ******** Enumerations
 **********************************************************************************/

#pragma region Enums

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


enum SFX_InstanceFlags : uint32 // [LegoRR/SFX.c|flags:0x4|type:uint]
{
	SFX_INSTANCE_FLAG_NONE    = 0,

	SFX_INSTANCE_FLAG_UNK_1   = 0x1,
	SFX_INSTANCE_FLAG_LOOPING = 0x2,
	SFX_INSTANCE_FLAG_SOUND3D = 0x4,
};
flags_end(SFX_InstanceFlags, 0x4);


flags_scoped(SFX_GlobFlags) : uint32 // [LegoRR/SFX.c|flags:0x4|type:uint] SFX_GlobFlags
{
    SFX_GLOB_FLAG_NONE         = 0,

    SFX_GLOB_FLAG_SOUNDON      = 0x1,
    SFX_GLOB_FLAG_POPULATEMODE = 0x2,
    SFX_GLOB_FLAG_QUEUEMODE    = 0x8,
};
flags_scoped_end(SFX_GlobFlags, 0x4);

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

struct SFX_Property
{
	/*0,4*/         sint32 sound3DHandle;
	/*4,4*/         SFX_Property* next; // next group property
	/*8*/
};
assert_sizeof(SFX_Property, 0x8);


struct SFX_Instance // [LegoRR/SFX.c|struct:0x18]
{
	/*00,4*/        SFX_ID sfxID;
	/*04,4*/        IDirect3DRMFrame3* frame; // what happened to hiding D3DRM behind Containers and 3DSound?
	/*08,c*/        Vector3F position;
	/*14,4*/        SFX_InstanceFlags flags;
	/*18*/
};
assert_sizeof(SFX_Instance, 0x18);


struct SFX_Globs // [LegoRR/SFX.c|struct:0x1770|tags:GLOBS]
{
	/*0000,f78*/    SFX_Property samplePropTable[SFX_MAXSAMPLES];
	/*0f78,640*/    SFX_Property sampleGroupTable[SFX_MAXSAMPLEGROUPS];
	/*15b8,4*/      uint32* hashNameList;
	/*15bc,4*/      uint32 hashNameCount; // excludes SFX_Preload_Count (45) pre-loaded names
	/*15c0,4*/      uint32 sampleGroupCount;
	/*15c4,4*/      SFX_GlobFlags flags;
	/*15c8,f0*/     SFX_Instance sfxInstanceTable[10];
	/*16b8,4*/      uint32 sfxInstanceCount;
	/*16bc,4*/      real32 globalSampleDuration; // Duration is multiplied by 25.0
	/*16c0,4*/      sint32 globalSampleSoundHandle;
	/*16c4,4*/      SFX_ID globalSampleSFXType;
	/*16c8,28*/     SFX_ID soundQueueSFXTable_1[10];
	/*16f0,28*/     Gods98::SoundMode soundQueueModesTable_1[10];
	/*1718,28*/     SFX_ID soundQueueSFXTable_2[10];
	/*1740,28*/     Gods98::SoundMode soundQueueModesTable_2[10];
	/*1768,4*/      uint32 soundQueueCount_1;
	/*176c,4*/      uint32 soundQueueCount_2;
	/*1770*/
};
assert_sizeof(SFX_Globs, 0x1770);

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @00502468>
extern SFX_Globs & sfxGlobs;

#pragma endregion

/**********************************************************************************
 ******** Macros
 **********************************************************************************/

#pragma region Macros

#define SFX_RegisterName(n)		(sfxGlobs.hashNameList[n]=Gods98::Util_HashString(#n,false,true))

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @00464a00>
void __cdecl SFX_Initialise(void);

// <LegoRR.exe @00464ee0>
void __cdecl SFX_Container_SoundTriggerCallback(const char* sfxName, Gods98::Container* cont, void* data);

// <LegoRR.exe @00464f10>
void __cdecl SFX_SetSamplePopulateMode(bool32 on);

// <LegoRR.exe @00464f30>
bool32 __cdecl SFX_GetType(OPTIONAL const char* sfxName, OUT SFX_ID* sfxID);

// value is modified via Util_Tokenise
// <LegoRR.exe @00464fc0>
bool32 __cdecl SFX_LoadSampleProperty(char* value, SFX_ID sfxID);

// <LegoRR.exe @004650e0>
sint32 __cdecl SFX_Random_GetSound3DHandle(SFX_ID sfxID);

// <LegoRR.exe @00465140>
void __cdecl SFX_StopGlobalSample(void);

// <LegoRR.exe @00465180>
bool32 __cdecl SFX_SetGlobalSampleDurationIfLE0_AndNullifyHandle(real32 duration);

// <LegoRR.exe @004651b0>
bool32 __cdecl SFX_Random_SetAndPlayGlobalSample(SFX_ID sfxID, OPTIONAL OUT sint32* handle);

// <LegoRR.exe @00465220>
void __cdecl SFX_AddToQueue(SFX_ID sfxID, Gods98::SoundMode mode);

// <LegoRR.exe @00465260>
sint32 __cdecl SFX_Random_Play_OrAddToQueue(SFX_ID sfxID, bool32 loop);

// <LegoRR.exe @004652d0>
void __cdecl SFX_Random_SetBufferVolume(SFX_ID sfxID, sint32 volume);

// <LegoRR.exe @004652f0>
sint32 __cdecl SFX_Random_GetBufferVolume(SFX_ID sfxID);

// if (sfxGlobs.flags & SFX_GlobFlags::SFX_GLOB_FLAG_UNK_8)
//   sound3D and loop MUST be a boolean with the LSB (0x1) set or unset
// position must be non-null if (!sound3D)
// <LegoRR.exe @00465310>
sint32 __cdecl SFX_Container_Random_Play_OrInitSoundUnk(Gods98::Container* cont, SFX_ID sfxID, bool32 loop, bool32 sound3D, OPTIONAL const Vector3F* position);

// if (sfxGlobs.flags & SFX_GlobFlags::SFX_GLOB_FLAG_UNK_8)
//   sound3D and loop MUST be a boolean with the LSB (0x1) set or unset
// position must be non-null if ((sfxGlobs.flags & SFX_GLOB_FLAG_UNK_8) && !sound3D)
// <LegoRR.exe @00465350>
sint32 __cdecl SFX_Random_Play_OrInitSoundUnk(IDirect3DRMFrame3* frame, SFX_ID sfxID, bool32 loop, bool32 sound3D, OPTIONAL const Vector3F* position);

// <LegoRR.exe @00465420>
real32 __cdecl SFX_Random_GetSamplePlayTime(SFX_ID sfxID);

// <LegoRR.exe @00465450>
void __cdecl SFX_Sound3D_StopSound(sint32 handle);

// <LegoRR.exe @00465460>
void __cdecl SFX_Update(real32 elapsed);

// <LegoRR.exe @00465510>
void __cdecl SFX_Sound3D_Update(void);

// <LegoRR.exe @00465520>
void __cdecl SFX_SetSoundOn(bool32 soundOn, bool32 stopAll);

// <LegoRR.exe @00465560>
void __cdecl SFX_SetSoundOn_AndStopAll(bool32 soundOn);

// <LegoRR.exe @00465570>
bool32 __cdecl SFX_IsQueueMode(void);

// <LegoRR.exe @00465580>
void __cdecl SFX_SetQueueMode_AndFlush(bool32 on);

// <LegoRR.exe @00465590>
void __cdecl SFX_SetQueueMode(bool32 on, bool32 flushQueued);

// <LegoRR.exe @00465630>
bool32 __cdecl SFX_IsSoundOn(void);

#pragma endregion

}

