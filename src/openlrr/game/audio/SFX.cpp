// SFX.cpp : 
//

#include "../../engine/audio/3DSound.h"
#include "../../engine/core/Maths.h"
#include "../../engine/core/Memory.h"
#include "../../engine/core/Utils.h"
#include "../../engine/gfx/Containers.h"
#include "../../legacy.h"

#include "SFX.h"


/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @00502468>
LegoRR::SFX_Globs & LegoRR::sfxGlobs = *(LegoRR::SFX_Globs*)0x00502468;

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @00464a00>
void __cdecl LegoRR::SFX_Initialise(void)
{
    /// FIX TEST: Ensure "global" sound handle data is set to zero
    std::memset(&sfxGlobs, 0, sizeof(sfxGlobs));
    sfxGlobs.globalSampleSoundHandle = -1;
    sfxGlobs.globalSampleSFXType = SFX_NULL;

    uint32 arraySize = SFX_MAXSAMPLES * sizeof(uint32);
    sfxGlobs.hashNameList = (uint32*)Gods98::Mem_Alloc(arraySize); //0x7bc
    
    /// FIX TEST: test ensuring zeroed-out array
    std::memset(sfxGlobs.hashNameList, 0, arraySize); // (SFX_MAXSAMPLES * sizeof(uint32)));

    SFX_RegisterName(SFX_NULL);
    SFX_RegisterName(SFX_Stamp);
    SFX_RegisterName(SFX_Drill);
    SFX_RegisterName(SFX_DrillFade);
    SFX_RegisterName(SFX_RockBreak);
    SFX_RegisterName(SFX_Drip);
    SFX_RegisterName(SFX_Ambient);
    SFX_RegisterName(SFX_AmbientLoop);
    SFX_RegisterName(SFX_Step);
    SFX_RegisterName(SFX_RockMonster);
    SFX_RegisterName(SFX_RockMonster2);
    SFX_RegisterName(SFX_RockMonsterStep);
    SFX_RegisterName(SFX_MFDeposit);
    SFX_RegisterName(SFX_ButtonPressed);
    SFX_RegisterName(SFX_MFLift);
    SFX_RegisterName(SFX_MFThrow);
    SFX_RegisterName(SFX_Walker);
    SFX_RegisterName(SFX_YesSir);
    SFX_RegisterName(SFX_Build);
    SFX_RegisterName(SFX_Okay);
    SFX_RegisterName(SFX_NotOkay);
    SFX_RegisterName(SFX_InterfaceSlideOnScreen);
    SFX_RegisterName(SFX_InterfaceSlideOffScreen);
    SFX_RegisterName(SFX_PanelSlideOnScreen);
    SFX_RegisterName(SFX_PanelSlideOffScreen);
    SFX_RegisterName(SFX_Siren);
    SFX_RegisterName(SFX_CrystalRecharge);
    SFX_RegisterName(SFX_Laser);
    SFX_RegisterName(SFX_LaserHit);
    SFX_RegisterName(SFX_LazerRecharge);
    SFX_RegisterName(SFX_TopPriority);
    SFX_RegisterName(SFX_ImmovableRock);
    SFX_RegisterName(SFX_Wall);
    SFX_RegisterName(SFX_Floor);
    SFX_RegisterName(SFX_BoulderHit);
    SFX_RegisterName(SFX_Place);
    SFX_RegisterName(SFX_PlaceOre);
    SFX_RegisterName(SFX_PlaceCrystal);
    SFX_RegisterName(SFX_Lava);
    SFX_RegisterName(SFX_RockWipe);
    SFX_RegisterName(SFX_FallIn);
    SFX_RegisterName(SFX_MusicLoop);
    SFX_RegisterName(SFX_CaptainSlide);
    SFX_RegisterName(SFX_Dynamite);
    SFX_RegisterName(SFX_AmbientMusicLoop);

    Gods98::Container_SetSoundTriggerCallback(SFX_Container_SoundTriggerCallback, nullptr);
}

// <LegoRR.exe @00464ee0>
void __cdecl LegoRR::SFX_Container_SoundTriggerCallback(const char* sfxName, Gods98::Container* cont, void* data)
{
    SFX_ID sfxID = SFX_ID::SFX_NULL; // dummy init
    if (SFX_GetType(sfxName, &sfxID)) {

        SFX_Container_Random_Play_OrInitSoundUnk(cont, sfxID, false, true, nullptr);
    }
}

// <LegoRR.exe @00464f10>
void __cdecl LegoRR::SFX_SetSamplePopulateMode(bool32 on)
{
    if (on) sfxGlobs.flags |= SFX_GlobFlags::SFX_GLOB_FLAG_POPULATEMODE;
    else    sfxGlobs.flags &= ~SFX_GlobFlags::SFX_GLOB_FLAG_POPULATEMODE;
}

// <LegoRR.exe @00464f30>
bool32 __cdecl LegoRR::SFX_GetType(OPTIONAL const char* sfxName, OUT SFX_ID* sfxID)
{
    if (sfxName != nullptr) {
        uint32 hashValue = Gods98::Util_HashString(sfxName, false, true);

        uint32 totalCount = sfxGlobs.hashNameCount + (uint32)SFX_ID::SFX_Preload_Count;
        for (uint32 i = 0; i < totalCount; i++) {
            if (hashValue == sfxGlobs.hashNameList[i]) {
                *sfxID = (SFX_ID)i;
                return true;
            }
        }

        // This flag presumably states the SFX table is still being built
        if (sfxGlobs.flags & SFX_GlobFlags::SFX_GLOB_FLAG_POPULATEMODE) {
            *sfxID = (SFX_ID)totalCount;
            // Removed duplicate call to Util_HashString, present in LegoRR
            sfxGlobs.hashNameList[totalCount] = hashValue;

            // We can't use this inside the array because of the SFX_Preload_Count difference.
            sfxGlobs.hashNameCount++;
            return true;
        }
    }
    return false;
}

// <LegoRR.exe @00464fc0>
bool32 __cdecl LegoRR::SFX_LoadSampleProperty(char* value, SFX_ID sfxID)
{
    char volBuff[64] = { 0 }; // dummy init
    char* sampleNames[100];

    SFX_Property* lastItem = nullptr;
    SFX_Property* curItem = &sfxGlobs.samplePropTable[sfxID];
    sfxGlobs.samplePropTable[sfxID].next = nullptr;

    bool32 stream = false;
    bool32 success = true;

    uint32 count = Gods98::Util_Tokenise(value, sampleNames, ",");

    for (uint32 i = 0; i < count; i++) {
        const char* s = sampleNames[i];
        bool32 simultaneous = false;
        sint32 volume = 0;

        if (*s == '*') { // simultaneous modifier ("multi")
            s++;
            simultaneous = true;
        }

        if (*s == '#') { // volume modifier: #-vol#
            s++; // skip opening '#'
            char* v = volBuff;
            while (*s != '#') {
                *v++ = *s++; // Copy number between #...#
            }
            s++; // skip closing '#'
            /// FIX APPLY: null-terminate volume buffer
            *v = '\0';

            volume = std::atoi(volBuff);
        }

        if (*s == '@') { // stream modifier
            s++;
            stream = true;
        }

        SFX_Property* nextItem = nullptr; // dummy init

        const char* fname = s;
        sint32 sound3DHandle = Gods98::Sound3D_Load(s, stream, simultaneous, volume);

        /// FIX APPLY: (comment this out) Restore sound group functionality
        ///  (normally first-grouped sound would be overwritten by second)
        //curItem->sound3DHandle = sound3DHandle;

        uint32 groupIndex = sfxGlobs.sampleGroupCount;
        if (sound3DHandle == -1) {
            success = false;
            //break; // surprisingly there is no break on failure
        }
        else {
            nextItem = curItem;
            if (lastItem != nullptr) { // add to grouped sounds
                curItem = &sfxGlobs.sampleGroupTable[sfxGlobs.sampleGroupCount++];
                lastItem->next = curItem;

                sfxGlobs.sampleGroupTable[groupIndex].next = nullptr;
                nextItem = curItem;
            }
            /// FIX APPLY: Restore sound group functionality
            ///  (normally first-grouped sound would be overwritten by second)
            curItem->sound3DHandle = sound3DHandle;

            lastItem = nextItem;
        }
    }
    return success;
}

// <LegoRR.exe @004650e0>
sint32 __cdecl LegoRR::SFX_Random_GetSound3DHandle(SFX_ID sfxID)
{
	if (sfxID != SFX_NULL) {
		if (sfxGlobs.samplePropTable[sfxID].next != nullptr) {
			// This SFX is a group of possible sounds, randomly select one... just why??
			SFX_Property* sfxProp = &sfxGlobs.samplePropTable[sfxID];

			uint32 groupCount = 0;
			for (SFX_Property* g = sfxProp; g != nullptr; g = g->next) {
				groupCount++;
			}

			uint32 rngIndex = (uint32)((sint32)Gods98::Maths_Rand() % groupCount);

			SFX_Property* sfxGroup = sfxProp;
			for (uint32 i = 0; i < rngIndex; i++) {
				sfxGroup = sfxGroup->next;
			}

			// Swap the randomly selected 3DSound handle with our root property.
			/// TODO: This has be dangerous... With all the random sound selecting...
			sint32 group3DHandle = sfxGroup->sound3DHandle;
			group3DHandle = sfxProp->sound3DHandle;
			sfxProp->sound3DHandle = group3DHandle;
		}
		return sfxGlobs.samplePropTable[sfxID].sound3DHandle;
	}

	return -1; // Invalid handle
}

// <LegoRR.exe @00465140>
void __cdecl LegoRR::SFX_StopGlobalSample(void)
{
	if (sfxGlobs.globalSampleSoundHandle != -1 && sfxGlobs.globalSampleDuration > 0.0f) {
		SFX_Sound3D_StopSound(sfxGlobs.globalSampleSoundHandle);
		sfxGlobs.globalSampleSoundHandle = -1;
		sfxGlobs.globalSampleDuration = 0.0f;
	}
}

// <LegoRR.exe @00465180>
bool32 __cdecl LegoRR::SFX_SetGlobalSampleDurationIfLE0_AndNullifyHandle(real32 duration)
{
	if (sfxGlobs.globalSampleDuration <= 0.0f) {
		sfxGlobs.globalSampleDuration = duration;
		sfxGlobs.globalSampleSoundHandle = -1;
		return true;
	}
	return false;
}

// <LegoRR.exe @004651b0>
bool32 __cdecl LegoRR::SFX_Random_SetAndPlayGlobalSample(SFX_ID sfxID, OPTIONAL OUT sint32* handle)
{
	if (sfxGlobs.globalSampleDuration <= 0.0f) {
		sfxGlobs.globalSampleSoundHandle = SFX_Random_Play_OrAddToQueue(sfxID, false);

		if (sfxGlobs.globalSampleSoundHandle != -1) {
			real32 playTime = SFX_Random_GetSamplePlayTime(sfxID);
			sfxGlobs.globalSampleSFXType = sfxID;
			sfxGlobs.globalSampleDuration = (playTime * STANDARD_FRAMERATE);

			if (handle) *handle = sfxGlobs.globalSampleSoundHandle;

			return true;
		}
	}
	return false;
}

// <LegoRR.exe @00465220>
void __cdecl LegoRR::SFX_AddToQueue(SFX_ID sfxID, Gods98::SoundMode mode)
{
	if (sfxGlobs.soundQueueCount_1 < 10) {
		sfxGlobs.soundQueueSFXTable_1[sfxGlobs.soundQueueCount_1] = sfxID;
		sfxGlobs.soundQueueModesTable_1[sfxGlobs.soundQueueCount_1] = mode;
		sfxGlobs.soundQueueCount_1++;
	}
}

// <LegoRR.exe @00465260>
sint32 __cdecl LegoRR::SFX_Random_Play_OrAddToQueue(SFX_ID sfxID, bool32 loop)
{
	if (!SFX_IsQueueMode()) {

		if (sfxID != SFX_NULL && SFX_IsSoundOn()) {

			sint32 rngSound3DHandle = SFX_Random_GetSound3DHandle(sfxID);
			if (rngSound3DHandle != 0) {
				return Gods98::Sound3D_Play2(Gods98::Sound3DPlay::Normal, nullptr, rngSound3DHandle, loop, nullptr);
			}
			//return 0; // rngSound3DHandle; // return 0; // (EAX)
		}
		//return (sint32)sfxID; // (EAX) // return 0; or return sfxID; if (!SFX_IsSoundOn())
	}
	else {
		if (sfxGlobs.sfxInstanceCount < 10) {
			SFX_Instance* sfxInst = &sfxGlobs.sfxInstanceTable[sfxGlobs.sfxInstanceCount];
			sfxInst->sfxID = sfxID;
			sfxGlobs.sfxInstanceTable[sfxGlobs.sfxInstanceCount].flags &= ~SFX_InstanceFlags::SFX_INSTANCE_FLAG_UNK_1; // !playing? queued?
			sfxGlobs.sfxInstanceCount++;

			//return (sint32)sfxInst; // (EAX)
		}
		// Whatever value was passed into EAX here would be returned... this is why we can't have nice things.
		//return 0; // (in_EAX)
	}

	/// FIX APPLY: Sanity with return values. This also fixes Issue #3 where a guaranteed random landslide
	///            on the first tick of "Rubble Trouble" returns junk data into globalSampleSoundHandle.
	return -1;
}

// <LegoRR.exe @004652d0>
void __cdecl LegoRR::SFX_Random_SetBufferVolume(SFX_ID sfxID, sint32 volume)
{
	/// FIXME: The randomness here smells like a disaster waiting to happen.
	sint32 rngSound3DHandle = SFX_Random_GetSound3DHandle(sfxID);
	Gods98::Sound3D_SetBufferVolume(rngSound3DHandle, volume);
}

// <LegoRR.exe @004652f0>
sint32 __cdecl LegoRR::SFX_Random_GetBufferVolume(SFX_ID sfxID)
{
	/// FIXME: The randomness here smells like a disaster waiting to happen.
	sint32 rngSound3DHandle = SFX_Random_GetSound3DHandle(sfxID);
	return Gods98::Sound3D_GetBufferVolume(rngSound3DHandle);
}

// if (SFX_IsQueueMode())
//   sound3D and loop MUST be a boolean with the LSB (0x1) set or unset
// position must be non-null if (!sound3D)
// <LegoRR.exe @00465310>
sint32 __cdecl LegoRR::SFX_Container_Random_Play_OrInitSoundUnk(Gods98::Container* cont, SFX_ID sfxID, bool32 loop, bool32 sound3D, OPTIONAL const Vector3F* position)
{
	IDirect3DRMFrame3* frame = (cont ? Gods98::Container_GetMasterFrame(cont) : nullptr);

	return SFX_Random_Play_OrInitSoundUnk(frame, sfxID, loop, sound3D, position);
}

// if (SFX_IsQueueMode())
//   sound3D and loop MUST be a boolean with the LSB (0x1) set or unset
// position must be non-null if (SFX_IsQueueMode() && !sound3D)
// <LegoRR.exe @00465350>
sint32 __cdecl LegoRR::SFX_Random_Play_OrInitSoundUnk(IDirect3DRMFrame3* frame, SFX_ID sfxID, bool32 loop, bool32 sound3D, OPTIONAL const Vector3F* position)
{
	if (!SFX_IsQueueMode()) {

		if (sfxID != SFX_NULL && SFX_IsSoundOn()) {

			sint32 rngSound3DHandle = SFX_Random_GetSound3DHandle(sfxID);
			if (rngSound3DHandle != 0) {

				if (sound3D) {
					return Sound3D_Play2(Gods98::Sound3DPlay::OnFrame, frame, rngSound3DHandle, loop, nullptr);
				}
				else {
					return Sound3D_Play2(Gods98::Sound3DPlay::OnPos, nullptr, rngSound3DHandle, loop, position);
				}
			}
		}
	}
	else {
		if (sfxGlobs.sfxInstanceCount < 10) {
			SFX_Instance* sfxInst = &sfxGlobs.sfxInstanceTable[sfxGlobs.sfxInstanceCount];
			sfxInst->sfxID = sfxID;
			sfxInst->frame = frame;

			
			sfxInst->flags &= ~(SFX_InstanceFlags::SFX_INSTANCE_FLAG_SOUND3D|SFX_InstanceFlags::SFX_INSTANCE_FLAG_LOOPING);
			sfxInst->flags |= SFX_InstanceFlags::SFX_INSTANCE_FLAG_UNK_1; // playing?
			if (loop)    sfxInst->flags |= SFX_InstanceFlags::SFX_INSTANCE_FLAG_LOOPING;
			if (sound3D) sfxInst->flags |= SFX_InstanceFlags::SFX_INSTANCE_FLAG_SOUND3D;

			// Original flag manipulation had some pretty strict requirements for the booleans:
			//sfxInst->flags = ((sound3D & 1U) << 1 | loop & 1U) << 1 |
			//	(sfxInst->flags & 0xfffffff9) | 0x1;
			
			if (position) sfxInst->position = *position;

			sfxGlobs.sfxInstanceCount++;
		}
	}

	return 0;
}

// <LegoRR.exe @00465420>
real32 __cdecl LegoRR::SFX_Random_GetSamplePlayTime(SFX_ID sfxID)
{
	sint32 rngSound3DHandle = SFX_Random_GetSound3DHandle(sfxID);
	if (rngSound3DHandle != 0) {
		return Gods98::Sound3D_GetSamplePlayTime(rngSound3DHandle);
	}
	return 0.0f;
}

// <LegoRR.exe @00465450>
void __cdecl LegoRR::SFX_Sound3D_StopSound(sint32 handle)
{
	Gods98::Sound3D_StopSound(handle);
}

// <LegoRR.exe @00465460>
void __cdecl LegoRR::SFX_Update(real32 elapsed)
{
	bool32 origQueueMode = SFX_IsQueueMode(); // Backup flag8

	sfxGlobs.globalSampleDuration -= elapsed;

	SFX_SetQueueMode(false, false); // Turn off flag8

	/// FIXME: Figure out what field type soundQueueModesTable_1/2 really is, bool32 for looping??
	for (uint32 i = 0; i < sfxGlobs.soundQueueCount_2; i++) {
		SFX_Random_Play_OrAddToQueue(sfxGlobs.soundQueueSFXTable_2[i], (bool32)sfxGlobs.soundQueueModesTable_2[i]);
	}

	if (origQueueMode) {
		SFX_SetQueueMode_AndFlush(true); // Restore flag8 (and perform other functionality)
	}

	// Alternate for loop copy:
	//std::memcpy(sfxGlobs.soundQueueSFXTable_2, sfxGlobs.soundQueueSFXTable_1,
	//            (sfxGlobs.soundQueueCount_1 * sizeof(SFX_ID)));

	//std::memcpy(sfxGlobs.soundQueueModesTable_2, sfxGlobs.soundQueueModesTable_1,
	//            (sfxGlobs.soundQueueCount_1 * sizeof(Gods98::SoundMode)));

	for (uint32 i = 0; i < sfxGlobs.soundQueueCount_1; i++) {
		sfxGlobs.soundQueueSFXTable_2[i] = sfxGlobs.soundQueueSFXTable_1[i];
	}
	for (uint32 i = 0; i < sfxGlobs.soundQueueCount_1; i++) {
		sfxGlobs.soundQueueModesTable_2[i] = sfxGlobs.soundQueueModesTable_1[i];
	}

	sfxGlobs.soundQueueCount_2 = sfxGlobs.soundQueueCount_1;
	sfxGlobs.soundQueueCount_1 = 0;

	SFX_Sound3D_Update();
}

// <LegoRR.exe @00465510>
void __cdecl LegoRR::SFX_Sound3D_Update(void)
{
	Gods98::Sound3D_Update(); // (inlined)
}

// <LegoRR.exe @00465520>
void __cdecl LegoRR::SFX_SetSoundOn(bool32 soundOn, bool32 stopAll)
{
	if (soundOn) {
		sfxGlobs.flags |= SFX_GlobFlags::SFX_GLOB_FLAG_SOUNDON;
	}
	else {
		sfxGlobs.flags &= ~SFX_GlobFlags::SFX_GLOB_FLAG_SOUNDON;

		if (stopAll) {
			Gods98::Sound3D_StopAllSounds();
		}

		sfxGlobs.globalSampleSFXType = SFX_NULL;
	}
}

// <LegoRR.exe @00465560>
void __cdecl LegoRR::SFX_SetSoundOn_AndStopAll(bool32 soundOn)
{
	SFX_SetSoundOn(soundOn, true);
}

// <LegoRR.exe @00465570>
bool32 __cdecl LegoRR::SFX_IsQueueMode(void)
{
	return (sfxGlobs.flags & SFX_GlobFlags::SFX_GLOB_FLAG_QUEUEMODE);
}

// <LegoRR.exe @00465580>
void __cdecl LegoRR::SFX_SetQueueMode_AndFlush(bool32 on)
{
	SFX_SetQueueMode(on, true);
}

// <LegoRR.exe @00465590>
void __cdecl LegoRR::SFX_SetQueueMode(bool32 on, bool32 flushQueued)
{
	if (on) {
		sfxGlobs.flags |= SFX_GlobFlags::SFX_GLOB_FLAG_QUEUEMODE;
	}
	else {
		sfxGlobs.flags &= ~SFX_GlobFlags::SFX_GLOB_FLAG_QUEUEMODE;
		if (flushQueued) {
			for (uint32 i = 0; i < sfxGlobs.sfxInstanceCount; i++) {
				SFX_Instance* sfxInst = &sfxGlobs.sfxInstanceTable[i];

				if (!(sfxInst->flags & SFX_InstanceFlags::SFX_INSTANCE_FLAG_UNK_1)) {
					SFX_Random_Play_OrAddToQueue(sfxInst->sfxID,
														((sfxInst->flags & SFX_InstanceFlags::SFX_INSTANCE_FLAG_LOOPING) != 0)); // != 0 for awkward precise flag boolean parameters
				}
				else {
					SFX_Random_Play_OrInitSoundUnk(sfxInst->frame, sfxInst->sfxID,
														  ((sfxInst->flags & SFX_InstanceFlags::SFX_INSTANCE_FLAG_LOOPING) != 0), // != 0 for awkward precise flag boolean parameters
														  ((sfxInst->flags & SFX_InstanceFlags::SFX_INSTANCE_FLAG_SOUND3D) != 0), // != 0 for awkward precise flag boolean parameters
														  &sfxInst->position);
				}
			}
		}
	}
}

// <LegoRR.exe @00465630>
bool32 __cdecl LegoRR::SFX_IsSoundOn(void)
{
	return (sfxGlobs.flags & SFX_GlobFlags::SFX_GLOB_FLAG_SOUNDON);
}

#pragma endregion
