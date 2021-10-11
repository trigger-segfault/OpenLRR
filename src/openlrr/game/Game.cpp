// Game.cpp : 
//

#include "../engine/Main.h"

#include "Game.h"


/**********************************************************************************
 ******** Forward Global Namespace Declarations
 **********************************************************************************/

#pragma region Forward Declarations

/// TODO: Remove me once Lego module is finished
#define Lego_Initialise ((bool32(__cdecl*)(void))0x0041fa80)
#define Lego_MainLoop ((bool32(__cdecl*)(real32))0x00423210)
#define Lego_Shutdown ((void(__cdecl*)(void))0x00424c20)
#define Lego_Shutdown_Debug ((void(__cdecl*)(void))0x00424c30)

#define Level_TryGeneratedRockMonster ((LegoRR::LegoObject*(__cdecl*)(int**, LegoRR::ObjectType, sint32, uint32, uint32))0x0043b1f0)

#pragma endregion


/**********************************************************************************
 ******** Game Entry point
 **********************************************************************************/

#pragma region Entry point

// This is the GAME entry point as called by WinMain,
//  this should hook the Main_State loop functions and only perform basic initial setup.
// (this can return bool32, but does not)
// <LegoRR.exe @0041f950>
void __cdecl Gods98::Gods_Go(const char* programName)
{
	log_firstcall();

	std::memset(&LegoRR::legoGlobs, 0, 0xef8 /*sizeof(legoGlobs)*/);
	LegoRR::legoGlobs.gameName = programName;

	/// FLUFF OPENLRR: Wrap the program name in parenthesis and start with "OpenLRR"
	char buff[1024];
	if (std::strcmp(programName, "OpenLRR") != 0) {
		std::sprintf(buff, "%s (%s)", "OpenLRR", programName);
		programName = buff;
	}
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

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @005570c0>
LegoRR::Lego_Globs & LegoRR::legoGlobs = *(LegoRR::Lego_Globs*)0x005570c0;

#pragma endregion



// <LegoRR.exe @0042c260>
bool32 __cdecl LegoRR::Level_HandleEmergeTriggers(LevelData* level, const Point2I* blockPos, OUT Point2I* emergeBlockPos)
{
    /// DEBUG: (testing) Enable multiple emerges at once :)
#define DEBUG_MULTIPLE_EMERGE


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

                    LegoObject* legoObj = Level_TryGeneratedRockMonster(
                        (int**)((uint8*)legoGlobs.RockMonsterData_TABLE + (level->EmergeCreature*0x74)),
                        ObjectType::OBJECT_ROCKMONSTER, level->EmergeCreature,
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

