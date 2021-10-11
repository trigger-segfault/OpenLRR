// FrontEnd.cpp : 
//

#pragma once

#include "../../engine/core/Config.h"
#include "../../engine/core/Files.h"
#include "../../engine/core/Memory.h"
#include "../../engine/drawing/Images.h"
#include "../../engine/input/Input.h"
#include "../../engine/video/Movie.h"
#include "../../engine/Main.h"

#include "../Game.h"

#include "FrontEnd.h"


/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

/*// <LegoRR.exe @00415080>
void __cdecl LegoRR::Front_UpdateOptionsSliders(void)
{
    int saveIndex;
    SaveData* save;

    saveIndex = lego::save::Save_GetSaveNumber();
    save = lego::save::Save_GetSaveData_OfNumber(saveIndex);
    if (save != NULL) {
        PausedMenu_SliderSoundVolume(save->SliderSoundVolume);
        PausedMenu_SliderBrightness(save->SliderBrightness);
        PausedMenu_SliderMusicVolume(save->SliderMusicVolume);
    }
    return;
}*/

// Plays an alread-loaded Movie_t from the G98CMovie C wrapper API.
// Allows terminating the movie playback during runtime with isSkippable.
// <LegoRR.exe @00415630>
void __cdecl LegoRR::Front_PlayMovie(Gods98::Movie_t* mov, bool32 skippable)
{
    /// QOL APPLY: Always skippable movies/splashes (TODO: move this to optional?)
    skippable = true;

    // NOTE: Speed is not supported by Movie_Update
    const real32 MOVIE_SPEED = 1.0f;

    // HARDCODED SCREEN RESOLUTION!!
    Rect2I destRect = { 0, 0, 640, 480 };
    real32 timer = 0.0;

    while (Gods98::Movie_Update(mov, MOVIE_SPEED, &destRect)) {
        Gods98::Main_LoopUpdate(false);

        timer += MOVIE_SPEED;
        if (timer > (uint32)Gods98::Movie_GetDuration(mov))
            break;

        if (skippable) {
            if (Gods98::Input_AnyKeyPressed() || Gods98::INPUT.msrb || Gods98::INPUT.mslb)
                break;
        }
    }
}

// <LegoRR.exe @004156f0>
void __cdecl LegoRR::Front_ShowIntroSplash(const char* imageKey, bool32 skippable, const char* timeKey)
{
    /// QOL APPLY: Always skippable movies/splashes (TODO: move this to optional?)
    skippable = true;

    real32 seconds = Config_GetRealValue(Lego_Config(), Config_ID(Lego_GameName(), "Main", timeKey));
    if (seconds == 0.0f)
        seconds = 3.0; // default time, in seconds

    uint32 startTime = Gods98::Main_GetTime();
    uint32 duration = (uint32)(seconds * 1000.0f);

    if (imageKey != nullptr) {
        Gods98::Image* image;
        const char* fName = Gods98::Config_GetTempStringValue(Lego_Config(), Config_ID(Lego_GameName(), "Main", imageKey));

        if (fName && (image = Gods98::Image_LoadBMP(fName))) {
            while (true) {
                Gods98::Image_Display(image, nullptr);
                Gods98::Main_LoopUpdate(false);
                
                Gods98::Main_Sleep(100); // replaces `::Sleep(ms)`

                if (Gods98::Main_GetTime() >= startTime + duration)
                    break;

                if (skippable) {
                    if (Gods98::Input_AnyKeyPressed() || Gods98::INPUT.msrb || Gods98::INPUT.mslb)
                        break;
                }
            }
            Gods98::Image_Remove(image);
        }
    }
}

// <LegoRR.exe @00415840>
void __cdecl LegoRR::Front_ShowIntroMovie(const char* movieKey, bool32 skippable)
{
    /// QOL APPLY: Always skippable movies/splashes (TODO: move this to optional?)
    skippable = true;

    if (movieKey != nullptr) {
        /// CHANGE: No need to allocate memory for the string
        const char* fName = Gods98::Config_GetTempStringValue(Lego_Config(), Config_ID(Lego_GameName(), "Main", movieKey));
        //char* fName = Gods98::Config_GetStringValue(Lego_Config(), Config_ID(Lego_GameName(), "Main", movieKey));

        if (fName && Gods98::File_Exists(fName)) {
            Gods98::Movie_t* mov = Gods98::Movie_Load(fName);
            Front_PlayMovie(mov, skippable);
            Gods98::Movie_Free(mov);

            /// CHANGE: No need to allocate memory for the string
            //Gods98::Mem_Free(fName);
        }
    }
}

#pragma endregion
