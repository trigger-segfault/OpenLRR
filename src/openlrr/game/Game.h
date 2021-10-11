#pragma once

#include "../common.h"

#include "../engine/core/Config.h"


/**********************************************************************************
 ******** Game Entry point
 **********************************************************************************/

namespace Gods98
{; // !<---
#pragma region Entry point

// This is the GAME entry point as called by WinMain,
//  this should hook the Main_State loop functions and only perform basic initial setup.
// (this can return bool32, but does not)
// (LRR_Go)
// <LegoRR.exe @0041f950>
void __cdecl Gods_Go(const char* programName);

#pragma endregion
}



namespace LegoRR
{; // !<---

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#if 0
#pragma region Functions

// (first function in module)
// <LegoRR.exe @0041f7a0>
void __cdecl Level_IncCrystals(void);


// [...]


// (LRR_Go)
// <LegoRR.exe @0041f950>
//void __cdecl Gods_Go(const char* programName);

// (LRR_SetLoadFlag_StartTeleporter)
// <LegoRR.exe @0041f9b0>
void __cdecl Lego_SetLoadFlag_StartTeleporter(void);

// <LegoRR.exe @0041fa70>
real32 __cdecl Game_GetGameSpeed(void);

// Main_State.Initialise function
// (LRR_Initialise)
// <LegoRR.exe @0041fa80>
bool32 __cdecl Lego_Initialise(void);

// (LRR_LoadMiscObjectsnitialise)
// <LegoRR.exe @00422780>
void __cdecl Lego_LoadMiscObjects(const Gods98::Config* root);

// (LRR_GetCurrentCamera_Container)
// <LegoRR.exe @00422fb0>
Gods98::Container* __cdecl Lego_GetCurrentCamera_Container(void);

// (LRR_GetElapsedAbs)
// <LegoRR.exe @00422fe0>
real32 __cdecl Lego_GetElapsedAbs(void);

// (LRR_RenameInputUnk)
// <LegoRR.exe @00422ff0>
void __cdecl Lego_RenameInputUnk(void);

// (LRR_Input_UpdateTyping)
// <LegoRR.exe @00423120>
void __cdecl Lego_Input_UpdateTyping(void);

// Main_State.MainLoop function
// (LRR_MainLoop)
// <LegoRR.exe @00423210>
bool32 __cdecl Lego_MainLoop(real32 elapsed);


// [...]


// Main_State.Shutdown function
// Simply halts music playback, then calls std::exit(0); (by calling Lego_Exit();)
// (LRR_Shutdown)
// <LegoRR.exe @00424c20>
void __cdecl Lego_Shutdown(void);

// Main_State.Shutdown function  (ProgrammerMode_10)
// This is only called for ProgrammerMode_10. Proper game cleanup is performed here.
// (LRR_Shutdown_Debug)
// <LegoRR.exe @00424c30>
void __cdecl Lego_Shutdown_Debug(void);

// Simply halts music playback, then calls std::exit(0);
// (LRR_Exit)
// <LegoRR.exe @00424fd0>
void __cdecl Lego_Exit(void);


// [... ... ...]


// (last function of Lego module)
// <LegoRR.exe @004359d0>
void __cdecl LRRGame_SetCallToArmsOn(bool32 callToArms);


#pragma endregion
#endif

}
