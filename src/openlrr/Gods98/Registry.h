// Registry.h : 
//
/// APIS: win32
/// DEPENDENCIES: -
/// DEPENDENTS: Files, Main

#pragma once

#include "../common.h"


namespace Gods98
{; // !<---

/**********************************************************************************
 ******** Enumerations
 **********************************************************************************/

#pragma region Enums

// Supported registry information types
enum RegistryType
{
	REGISTRY_STRING_VALUE = 0,
	REGISTRY_DWORD_VALUE  = 1,
};
static_assert(sizeof(RegistryType) == 0x4, "");

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <CLGen.exe @00401660>
// <LegoRR.exe @0048b5f0>
const char* __cdecl Registry_GetKeyFromPath(const char* path, OUT char* str);


// <CLGen.exe @00401690>
bool32 __cdecl Registry_SetValue(const char* path,
					   const char* key, 
					   RegistryType dataType,
					   const void* data, 
					   uint32 dataSize);


// <LegoRR.exe @0048b620>
bool32 __cdecl Registry_GetValue(const char* path,
					   const char* key, 
					   RegistryType dataType, 
					   OUT void* data, 
					   uint32 dataSize);

// <CLGen.exe @004016c0>
bool32 __cdecl Registry_SetValue_Recursive(HKEY parent,
										const char* path, 
										const char* key, 
										RegistryType dataType,
										const void* data, 
										uint32 dataSize);

// <LegoRR.exe @0048b650>
bool32 __cdecl Registry_GetValue_Recursive(HKEY parent,
										const char* path,
										const char* key,
										RegistryType dataType,
										OUT void* data,
										uint32 dataSize);

#pragma endregion

}
