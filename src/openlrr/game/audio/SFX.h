// SFX.h : 
//

#pragma once

#include "../../common.h"
#include "../../engine/geometry.h"
#include "../../engine/audio/Sound.h"
#include "../../engine/gfx/Containers.h"

#include "../GameCommon.h"


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

// Parameter type wrapper around `SFX_GetType` for use with `LwsFindSFXIDFunc`.
__inline bool32 __cdecl SFX_Callback_FindSFXIDFunc(const char* name, OUT uint32* sfxID)
{
	return SFX_GetType(name, reinterpret_cast<SFX_ID*>(sfxID));
}


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
// wPos must be non-null if (!sound3D)
// <LegoRR.exe @00465310>
sint32 __cdecl SFX_Container_Random_Play_OrInitSoundUnk(Gods98::Container* cont, SFX_ID sfxID, bool32 loop, bool32 sound3D, OPTIONAL const Vector3F* wPos);


// if (sfxGlobs.flags & SFX_GlobFlags::SFX_GLOB_FLAG_UNK_8)
//   sound3D and loop MUST be a boolean with the LSB (0x1) set or unset
// wPos must be non-null if ((sfxGlobs.flags & SFX_GLOB_FLAG_UNK_8) && !sound3D)
// <LegoRR.exe @00465350>
sint32 __cdecl SFX_Random_Play_OrInitSoundUnk(IDirect3DRMFrame3* frame, SFX_ID sfxID, bool32 loop, bool32 sound3D, OPTIONAL const Vector3F* wPos);

// Parameter type wrapper around `SFX_Random_Play_OrInitSoundUnk` for use with `LwsPlaySample3DFunc`.
__inline sint32 __cdecl SFX_Callback_PlaySample3DFunc(void* frame, uint32 type, bool32 loop, bool32 onCont, const Vector3F* wPos)
{
	return SFX_Random_Play_OrInitSoundUnk(reinterpret_cast<IDirect3DRMFrame3*>(frame), static_cast<SFX_ID>(type), loop, onCont, wPos);
}


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

