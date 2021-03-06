// Registry.h : 
//
/// APIS: win32
/// DEPENDENCIES: -
/// DEPENDENTS: Files, Main

#pragma once

#include "../../common.h"


namespace Gods98
{; // !<---

/**********************************************************************************
 ******** Enumerations
 **********************************************************************************/

#pragma region Enums

// Supported registry value kinds
enum class RegistryValue : uint32
{
	String = 0,
	Dword  = 1,
};
assert_sizeof(RegistryValue, 0x4);

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
					   RegistryValue dataType,
					   const void* data, 
					   uint32 dataSize);


// <LegoRR.exe @0048b620>
bool32 __cdecl Registry_GetValue(const char* path,
					   const char* key, 
					   RegistryValue dataType, 
					   OUT void* data, 
					   uint32 dataSize);

// <CLGen.exe @004016c0>
bool32 __cdecl Registry_SetValue_Recursive(HKEY parent,
										const char* path, 
										const char* key, 
										RegistryValue dataType,
										const void* data, 
										uint32 dataSize);

// <LegoRR.exe @0048b650>
bool32 __cdecl Registry_GetValue_Recursive(HKEY parent,
										const char* path,
										const char* key,
										RegistryValue dataType,
										OUT void* data,
										uint32 dataSize);

#pragma endregion

}
