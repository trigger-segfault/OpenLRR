// legacy.h : 
//

#include "legacy.h"


/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// Global variable for random number generation.

// <LegoRR.exe @004b0cc8>
uint32 & legacy::g_rand = *(uint32*)0x004b0cc8; // = 1U;

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

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// Random functions must retain their original PRNG implementation.

/// PRNG: MS Quick C - Linear Congruential Generator (LCG)
// Sets the current seed without modification.
// <LegoRR.exe @0048e420>
void __cdecl legacy::srand(uint32 seed)
{
	log_firstcall();

	legacy::g_rand = seed;
}

/// PRNG: MS Quick C - Linear Congruential Generator (LCG)
// Although this returns a value between [0,0x7fff], an sint32 type is still returned.
// <LegoRR.exe @0048e430>
sint32 __cdecl legacy::rand(void)
{
	log_firstcall();

	// <https://www.csee.umbc.edu/courses/undergraduate/426/fall14/lectures/l19/l19slides.pdf>
	// <https://web.archive.org/web/20210913181809/https://www.csee.umbc.edu/courses/undergraduate/426/fall14/lectures/l19/l19slides.pdf>

	// Linear Congruential Generator (LCG) Parameters
	// 
	// |    impl    |  n   |   a    |    c    |
	// |:----------:|:----:|:------:|:-------:|
	// | MS Quick C | 2^32 | 214013 | 2531011 |

	legacy::g_rand = (legacy::g_rand * 214013) + 2531011; // * 0x343fd) + 0x269ec3;
	return ((sint32)legacy::g_rand >> 16) & LEGACY_RAND_MAX; // 0x7fff;
}

#pragma endregion
