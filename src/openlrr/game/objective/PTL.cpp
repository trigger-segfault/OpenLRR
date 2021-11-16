// PTL.cpp : 
//

#include "../../engine/core/Config.h"
#include "../../engine/core/Errors.h"
#include "../../engine/core/Memory.h"
#include "../../engine/core/Utils.h"

#include "../object/Object.h"
#include "../Game.h"

#include "../interface/Messages.h"
#include "PTL.h"

#if 0
// <LegoRR.exe @00408bb0>
//uint32 __cdecl Construction_GetBuildingBase(const char* name);
#define Construction_GetBuildingBase ((uint32(__cdecl*)(const char*))0x00408bb0)

#define LegoObject_UpdatePowerConsumption ((void(__cdecl*)(LegoObject*))0x0043c830)
#endif


/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @00556be0>
LegoRR::PTL_Globs& LegoRR::ptlGlobs = *(LegoRR::PTL_Globs*)0x00556be0;

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

using namespace Gods98;

// <LegoRR.exe @0045daa0>
bool32 __cdecl LegoRR::PTL_Initialise(const char* fname, const char* gameName)
{
    ptlGlobs.count = 0;

    Gods98::Config* ptl;
    if ((ptl = Gods98::Config_Load(fname))) {

        const Gods98::Config* prop;
        for (prop = Config_FindArray(ptl, gameName); prop != nullptr; prop = Config_GetNextItem(prop)) {
            Error_Fatal(ptlGlobs.count >= PTL_MAXPROPERTIES, "Too many PTL properties");

            ptlGlobs.table[ptlGlobs.count].event  = Message_LookupPTLEventIndex(Config_GetItemName(prop));
            ptlGlobs.table[ptlGlobs.count].action = Message_LookupPTLEventIndex(Config_GetDataString(prop));
            ptlGlobs.count++;
        }

        Gods98::Config_Free(ptl);
        return true;
    }
    return false;
}

// <LegoRR.exe @0045db30>
void __cdecl LegoRR::PTL_EventToAction(IN OUT MessageAction* message)
{
    for (uint32 i = 0; i < ptlGlobs.count; i++) {
        if (message->event == ptlGlobs.table[i].event) {
            message->event = ptlGlobs.table[i].action;
            return;
        }
    }
    /// TODO: No error handled done by LegoRR
}


#pragma endregion
