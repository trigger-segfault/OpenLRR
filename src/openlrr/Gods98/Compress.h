#pragma once

#include "../common.h"


namespace Gods98
{; // !<---

/**********************************************************************************
 ******** Constants
 **********************************************************************************/

#pragma region Constants

#define COMPRESSION_UNPACK_BUFFER_EXTEND 20000

#pragma endregion

/**********************************************************************************
 ******** Enumerations
 **********************************************************************************/

#pragma region Enums

#define RNC_COMPRESS_BEST 1
#define RNC_COMPRESS_FAST 2

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @0049ca80>
uint32 __cdecl RNC_Uncompress(IN void* bufferIn, OUT void** bufferOut);

#pragma endregion

}
