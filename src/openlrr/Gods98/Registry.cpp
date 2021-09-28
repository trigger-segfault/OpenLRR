// Registry.cpp : 
//

#include "../platform/windows.h"

#include "Registry.h"


/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// <LegoRR.exe @0048b5f0>
// <CLGen.exe @00401660>
const char* __cdecl Gods98::Registry_GetKeyFromPath(const char* path, OUT char* str)
{
	log_firstcall();

	*str = '\0';
	while (*path && *path != '\\')
	{
		*str = *path;
		path++;
		str++;
	}

	if (*path == '\\') path++;
	*str = '\0';

	return path;
}


// <CLGen.exe @00401690>
bool32 __cdecl Gods98::Registry_SetValue(const char* path,
					   const char* key, 
					   RegistryType dataType,
					   const void* data, 
					   uint32 dataSize)
{
	return Registry_SetValue_Recursive(HKEY_LOCAL_MACHINE, path, key, dataType, data, dataSize);
}


// <LegoRR.exe @0048b620>
bool32 __cdecl Gods98::Registry_GetValue(const char* path,
					   const char* key, 
					   RegistryType dataType, 
					   OUT void* data, 
					   uint32 dataSize)
{
	log_firstcall();

	return Registry_GetValue_Recursive(HKEY_LOCAL_MACHINE, path, key, dataType, data, dataSize);
}

// <CLGen.exe @004016c0>
bool32 __cdecl Gods98::Registry_SetValue_Recursive(HKEY parent,
										const char* path, 
										const char* key, 
										RegistryType dataType,
										const void* data, 
										uint32 dataSize)
{
	HKEY localKey;
	char str[100];

	const char* newPath = Registry_GetKeyFromPath(path, str);

	if (!str[0])
	{
		// Set the value
		switch (dataType)
		{
		case RegistryType::REGISTRY_STRING_VALUE:
			::RegSetValueExA(parent, key, 0, REG_SZ, (const BYTE*)data, dataSize);
			return true;
		case RegistryType::REGISTRY_DWORD_VALUE:
			::RegSetValueExA(parent, key, 0, REG_DWORD, (const BYTE*)data, dataSize);
			return true;
		default:
			return FALSE;
		}
	}
	else
	{
		// Open the key and recurse
		::RegCreateKeyA(parent, str, &localKey);

		if (!localKey) return FALSE;

		bool32 retVal = (bool32)Registry_SetValue_Recursive(localKey, newPath, key, dataType, data, dataSize);
		
		::RegCloseKey(localKey);

		return retVal;
	}
}

// <LegoRR.exe @0048b650>
bool32 __cdecl Gods98::Registry_GetValue_Recursive(HKEY parent,
										const char* path,
										const char* key,
										RegistryType dataType,
										OUT void* data,
										uint32 dataSize)
{
	log_firstcall();

	HKEY localKey;
	char str[100];

	const char* newPath = Registry_GetKeyFromPath(path, str);

	if (!str[0])
	{
		DWORD _dataSize = dataSize;
		DWORD _dataType = 0;

		// Set the value
		switch (dataType)
		{
		case REGISTRY_STRING_VALUE:
			return ::RegQueryValueExA(parent, key, 0, &_dataType, (BYTE*)data, &_dataSize) == ERROR_SUCCESS;
		case REGISTRY_DWORD_VALUE:
			return ::RegQueryValueExA(parent, key, 0, &_dataType, (BYTE*)data, &_dataSize) == ERROR_SUCCESS;
		default:
			return FALSE;
		}
	}
	else
	{
		// Open the key and recurse
		::RegCreateKeyA(parent, str, &localKey);

		if (!localKey) return false;

		bool32 retVal = (bool32)Registry_GetValue_Recursive(localKey, newPath, key, dataType, data, dataSize);

		::RegCloseKey(localKey);

		return retVal;
	}
}

#pragma endregion