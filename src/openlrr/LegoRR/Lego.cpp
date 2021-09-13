
#include "Lego.h"
#include "../Gods98/Main.h"


/**********************************************************************************
 ******** Forward Global Namespace Declarations
 **********************************************************************************/

#pragma region Forward Declarations

/// TODO: Remove me once Lego module is finished
#define Lego_Initialise ((bool32(__cdecl*)(void))0x0041fa80)
#define Lego_MainLoop ((bool32(__cdecl*)(real32))0x00423210)
#define Lego_Shutdown ((void(__cdecl*)(void))0x00424c20)
#define Lego_Shutdown_Debug ((void(__cdecl*)(void))0x00424c30)
#define legoGlobs ((void*)0x005570c0)
//legoGlobs+4
#define legoGlobs_gameName ((const char**)0x005570c4)

#pragma endregion


/**********************************************************************************
 ******** Game Entry point
 **********************************************************************************/

#pragma region Entry point

// This is the GAME entry point as called by WinMain,
//  this should hook the Main_State loop functions and only perform basic initial setup.
// (this can return bool32, but does not)
// (LRR_Go)
// <LegoRR.exe @0041f950>
void __cdecl Gods98::Gods_Go(const char* programName)
{
	log_firstcall();

	std::memset(/*&*/legoGlobs, 0, 0xef8 /*sizeof(legoGlobs)*/);
	*legoGlobs_gameName = programName;

	Main_SetTitle(programName);

	Main_State mainState;
	mainState.Initialise = Lego_Initialise;
	mainState.MainLoop = Lego_MainLoop;
	mainState.Shutdown = Lego_Shutdown_Debug; // shutdown with cleanup
	if (Main_ProgrammerMode() != 10 /*PROGRAMMER_MODE_10*/) {
		mainState.Shutdown = Lego_Shutdown; // immediate shutdown, no cleanup
	}

	Main_SetState(&mainState);
}

#pragma endregion
