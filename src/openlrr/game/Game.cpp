// Game.cpp : 
//

#include "../engine/Main.h"

#include "audio/SFX.h"
#include "Game.h"


/**********************************************************************************
 ******** Game Entry point
 **********************************************************************************/

#pragma region Entry point

void __cdecl LegoRR::Lego_Gods_Go_Setup(const char* programName, OUT Gods98::Main_State* mainState)
{
	std::memset(&LegoRR::legoGlobs, 0, 0xef8); //sizeof(legoGlobs);
	LegoRR::legoGlobs.gameName = programName;

	
	/*Gods98::Main_State mainState = {
		Lego_Initialise,
		Lego_MainLoop,
		Lego_Shutdown_Debug, // proper shutdown, with cleanup
	};*/

	mainState->Initialise = Lego_Initialise;
	mainState->MainLoop = Lego_MainLoop;
	mainState->Shutdown = Lego_Shutdown_Debug; // shutdown with cleanup

	if (Gods98::Main_ProgrammerMode() != 10) { // PROGRAMMER_MODE_10
		mainState->Shutdown = Lego_Shutdown; // immediate shutdown, no cleanup
	}
}

// This is the GAME entry point as called by WinMain,
//  this should hook the Main_State loop functions and only perform basic initial setup.
// (this can return bool32, but does not)
// <LegoRR.exe @0041f950>
void __cdecl LegoRR::Lego_Gods_Go(const char* programName)
{
	log_firstcall();

	Gods98::Main_State mainState = { nullptr }; // dummy init
	Lego_Gods_Go_Setup(programName, &mainState);

	Gods98::Main_SetTitle(programName);
	Gods98::Main_SetState(&mainState);

	/*std::memset(&LegoRR::legoGlobs, 0, 0xef8); //sizeof(legoGlobs);
	LegoRR::legoGlobs.gameName = programName;

	/// FLUFF OPENLRR: Wrap the program name in parenthesis and start with "OpenLRR"
	char buff[1024];
	if (std::strcmp(programName, "OpenLRR") != 0) {
		std::sprintf(buff, "%s (%s)", "OpenLRR", programName);
		programName = buff;
	}
	Gods98::Main_SetTitle(programName);
	
	Gods98::Main_State mainState = {
		Lego_Initialise,
		Lego_MainLoop,
		Lego_Shutdown_Debug, // proper shutdown, with cleanup
	};
	if (Gods98::Main_ProgrammerMode() != 10) { // PROGRAMMER_MODE_10
		mainState.Shutdown = Lego_Shutdown; // immediate shutdown, no cleanup
	}

	Gods98::Main_SetState(&mainState);*/
}

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @005570c0>
LegoRR::Lego_Globs & LegoRR::legoGlobs = *(LegoRR::Lego_Globs*)0x005570c0;

#pragma endregion



// <LegoRR.exe @0042c260>
bool32 __cdecl LegoRR::Level_HandleEmergeTriggers(Lego_Level* level, const Point2I* blockPos, OUT Point2I* emergeBlockPos)
{
    /// DEBUG: (testing) Enable multiple emerges at once :)
//#define DEBUG_MULTIPLE_EMERGE


#ifdef DEBUG_MULTIPLE_EMERGE
    uint32 emergedCount = 0;
#endif

    for (uint32 i = 0; i < level->emergeCount; i++) {
        EmergeTrigger* trigger = &level->emergeTriggers[i];
        const Point2I* triggerPos = &trigger->blockPos;
        if ((trigger->blockPos.x == blockPos->x && trigger->blockPos.y == blockPos->y) &&
            trigger->timeout == 0.0f)
        {
            for (uint32 j = 0; j < EMERGE_MAXPOINTS; j++) {
                const EmergeBlock* emergePt = &trigger->emergePoints[j];
                if (emergePt->used) {

                    LegoObject* legoObj = LegoObject_TryGenerateRMonster(
                        &legoGlobs.rockMonsterData[level->EmergeCreature],
                        LegoObject_RockMonster, level->EmergeCreature,
                        emergePt->blockPos.x, emergePt->blockPos.y);
                    
                    if (legoObj != nullptr) {

                        #ifdef DEBUG_MULTIPLE_EMERGE
                        // Everything after is just setting the output parameter and returning,
                        //  which we don't need with this preprocessor after the first emerge.
                        if (emergedCount++ > 0)
                            continue;  
                        #endif

                        trigger->timeout = level->EmergeTimeOut;
                        if (emergeBlockPos != nullptr) {
                            *emergeBlockPos = emergePt->blockPos;
                        }

                        #ifndef DEBUG_MULTIPLE_EMERGE
                        return true;
                        #endif
                    }
                }
            }
        }
    }

    #ifdef DEBUG_MULTIPLE_EMERGE
    /// CUSTOM: Spawn all the things!
    return (emergedCount != 0);

    #else
    return false;

    #endif
}

