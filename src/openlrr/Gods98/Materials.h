#pragma once

#include "../common.h"


namespace Gods98
{; // !<---

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

struct Material_Dummy { uint32 dummy; };
static_assert(sizeof(Material_Dummy) == 0x4, "");

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @00489780>
Material_Dummy* __cdecl Material_Create(real32 emissiveRed, real32 emissiveGreen, real32 emissiveBlue,
								real32 specularRed, real32 specularGreen, real32 specularBlue, real32 power);


// <unused?>
void __cdecl Material_Remove(Material_Dummy* material);

// <unused?>
void __cdecl Material_SetEmissive(Material_Dummy* material, real32 emissiveRed, real32 emissiveGreen, real32 emissiveBlue);

// <unused?>
void __cdecl Material_SetSpecular(Material_Dummy* material, real32 specularRed, real32 specularGreen, real32 specularBlue, real32 power);

#pragma endregion

}
