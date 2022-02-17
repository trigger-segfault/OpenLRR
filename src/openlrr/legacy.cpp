// legacy.h : 
//

#include "legacy.h"


/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// Global variable for random number generation.

// moved to: `engine/core/Maths.cpp`
// <LegoRR.exe @004b0cc8>
//uint32 & legacy::g_rand = *(uint32*)0x004b0cc8; // = 1U;

// These globals are used with inlined isspace, isdigit, is*, _isctype functions

// (unused)
// <LegoRR.exe @004b0d68>
uint16* (& legacy::g_pctype) = *(uint16**)0x004b0d68; // = legacy::g_wctype;

// (unused)
// <LegoRR.exe @004b0d6c>
uint16* (& legacy::g_pwctype) = *(uint16**)0x004b0d6c; // = legacy::g_wctype;

// <LegoRR.exe @004b0d72>
uint16 (& legacy::g_wctype)[256] = *(uint16 (*)[256])0x004b0f74;

// <LegoRR.exe @004b0f74>
sint32 & legacy::g_pcharwidth = *(sint32*)0x004b0f74; // = 1;

#pragma endregion
