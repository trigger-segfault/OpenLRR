// Materials.h : 
//
/// APIS: IDirect3DRM3, IDirect3DRMMaterial[12]
/// DEPENDENCIES: Main, (Errors)
/// DEPENDENTS: Map3D

#pragma once

#include "../../common.h"


namespace Gods98
{; // !<---

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

// Essentially a typedef pointer to IDirect3DRMMaterial2 (when treated as Material*)
struct Material { uint32 dummy; };
static_assert(sizeof(Material) == 0x4, "");

/// ALT: Similar to the Movie/Animation C wrapper typedefs
//typedef void Material;

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @00489780>
Material* __cdecl Material_Create(real32 emissiveRed, real32 emissiveGreen, real32 emissiveBlue,
								real32 specularRed, real32 specularGreen, real32 specularBlue, real32 power);


// <unused?>
void __cdecl Material_Remove(Material* material);

// <unused?>
void __cdecl Material_SetEmissive(Material* material, real32 emissiveRed, real32 emissiveGreen, real32 emissiveBlue);

// <unused?>
void __cdecl Material_SetSpecular(Material* material, real32 specularRed, real32 specularGreen, real32 specularBlue, real32 power);

#pragma endregion

}
