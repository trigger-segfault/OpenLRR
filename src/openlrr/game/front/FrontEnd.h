// FrontEnd.h : 
//

#pragma once

#include "../../common.h"

#include "../../engine/video/Movie.h"


namespace LegoRR
{; // !<---

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

//struct Front_Globs
//{
//	/*???*/
//};
//assert_sizeof(Front_Globs, 0x0);

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @???>
//extern Front_Globs & frontGlobs;

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

/*// <LegoRR.exe @00415080>
void __cdecl Front_UpdateOptionsSliders(void);*/

// Plays an alread-loaded Movie_t from the G98CMovie C wrapper API.
// Allows terminating the movie playback during runtime with isSkippable.
// <LegoRR.exe @00415630>
void __cdecl Front_PlayMovie(Gods98::Movie_t* mov, bool32 skippable);

// <LegoRR.exe @004156f0>
void __cdecl Front_ShowIntroSplash(const char* imageKey, bool32 skippable, const char* timeKey);

// <LegoRR.exe @00415840>
void __cdecl Front_ShowIntroMovie(const char* movieKey, bool32 skippable);

#pragma endregion

}
