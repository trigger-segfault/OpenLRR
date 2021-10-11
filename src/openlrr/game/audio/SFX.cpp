// SFX.cpp : 
//

#include "../../engine/audio/3DSound.h"
#include "../../engine/core/Utils.h"
#include "../../engine/core/Memory.h"
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
void __cdecl LegoRR::SFX_InitHashNames(void)
{
    /// FIX APPLY: Ensure "global" sound handle data is set to zero
    std::memset(&sfxGlobs, 0, sizeof(sfxGlobs));
    sfxGlobs.globalSampleSoundHandle = -1;
    sfxGlobs.globalSampleSFXType = SFX_NULL; // SFX__INVALID;

    sfxGlobs.hashNameList = (uint32*)Gods98::Mem_Alloc(495 * sizeof(uint32)); //0x7bc
    
    /// FIX APPLY: test ensuring zeroed-out array
    std::memset(sfxGlobs.hashNameList, 0, (495 * sizeof(uint32)));

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

    Gods98::Container_SetSoundTriggerCallback((Gods98::ContainerSoundTriggerCallback)0x00464ee0, nullptr);
}

// <LegoRR.exe @00464ee0>
//void __cdecl LegoRR::SFX_Container_SoundTriggerCallback(const char* sampleName, Gods98::Container* cont, void* data);

// <LegoRR.exe @00464fc0>
bool32 __cdecl LegoRR::SFX_Sample_LoadProperty(char* value, sint32 index)
{
    char volBuff[64] = { 0 }; // dummy init
    char* sampleNames[100];

    SFX_Property* lastItem = nullptr;
    SFX_Property* curItem = &sfxGlobs.samplePropTable[index];
    sfxGlobs.samplePropTable[index].next = nullptr;

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
            s++;
            char* v = volBuff;
            while (*s != '#') {
                *v++ = *s++;
            }
            s++;
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

#pragma endregion
