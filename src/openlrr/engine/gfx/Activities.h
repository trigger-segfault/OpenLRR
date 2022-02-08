// Activities.h : 
//
/// FILEIO: Config:Data[wad,std,cd]
/// APIS: -
/// DEPENDENCIES: Config, Utils, (Errors)
/// DEPENDENTS: Containers

#pragma once

#include "../../common.h"


namespace Gods98
{; // !<---

/**********************************************************************************
 ******** Function Typedefs
 **********************************************************************************/

#pragma region Function Typedefs

typedef void (__cdecl* ActivityReadCallback)(const char* filename, const char* gameID, uint32 intValue, real32 realValue, void* data);

#pragma endregion

/**********************************************************************************
 ******** Constants
 **********************************************************************************/

#pragma region Constants

#define ACTIVITY_MAXARGS		100
#define ACTIVITY_SEPARATOR		"/"
#define ACTIVITY_FILESTRING		"FILE"
#define ACTIVITY_TRANSCOSTRING	"TRANSCOEF"
#define ACTIVITY_SAMPLESTRING	"SAMPLE"
#define ACTIVITY_TRIGGERSTRING	"TRIGGER"
#define ACTIVITY_ACTIVITYSTRING	"Activities"
#define ACTIVITY_FILESUFFIX		"ae"

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// Possibly inlined inside Container_Load
// NOTE: arglist is modified and split by Util_Tokenise delimiter=ACTIVITY_SEPARATOR "/"
// <unknown>
uint32 __cdecl Activity_Load(const char* filename, const char* gameID, IN char* arglist, ActivityReadCallback ReadCallBack, void* data);

#pragma endregion

}
