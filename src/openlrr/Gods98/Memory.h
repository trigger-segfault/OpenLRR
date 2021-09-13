#pragma once

#include "../common.h"


namespace Gods98
{; // !<---

/**********************************************************************************
 ******** Constants
 **********************************************************************************/

#pragma region Constants

#define MEMORY_MAXHANDLES					2000

#define MEMORY_HANDLE_INVALID				((Mem_HandleValue) -1)

#pragma endregion

/**********************************************************************************
 ******** Enumerations
 **********************************************************************************/

#pragma region Enums

//#define MEMORY_HANDLE_FLAG_USED				0x00000001

enum Mem_GlobFlags : uint32
{
	MEMORY_FLAG_NONE = 0,
};
DEFINE_ENUM_FLAG_OPERATORS(Mem_GlobFlags);
static_assert(sizeof(Mem_GlobFlags) == 0x4, "");


enum Mem_HandleFlags : uint32
{
	MEMORY_HANDLE_FLAG_NONE = 0,

	MEMORY_HANDLE_FLAG_USED = 0x1,
};
DEFINE_ENUM_FLAG_OPERATORS(Mem_HandleFlags);
static_assert(sizeof(Mem_HandleFlags) == 0x4, "");

#pragma endregion

/**********************************************************************************
 ******** Typedefs
 **********************************************************************************/

#pragma region Typedefs

typedef uint32 Mem_HandleValue;

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

struct Mem_Handle
{
	/// FIX: stored as LPVOID*, rather than LPVOID
	/*0,4*/ void* addr;
	/*4,4*/ Mem_HandleFlags flags;
	/*8*/
};
static_assert(sizeof(Mem_Handle) == 0x8, "");


struct Mem_Globs
{
	/*0000,3e80*/ Mem_Handle handleList[MEMORY_MAXHANDLES];
	/*3e80,4*/ Mem_GlobFlags flags;  // (unused)
	/*3e84*/
};
static_assert(sizeof(Mem_Globs) == 0x3e84, "");

#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @00545a20>
extern Mem_Globs & memGlobs;

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

// std::malloc(void*)
// <LegoRR.exe @0048de90>
__inline void * Mem_Alloc(uint32 size)
{
	return ((void* (__cdecl*)(uint32))0x0048de90)(size);
}

// std::free(void*)
// <LegoRR.exe @0048de40>
__inline void Mem_Free(void * addr)
{
	((void(__cdecl*)(void*))0x0048de40)(addr);
}

// std::realloc(void*)
// <LegoRR.exe @0048df40>
__inline void * Mem_ReAlloc(void * addr, uint32 size)
{
	return ((void*(__cdecl*)(void*, uint32))0x0048df40)(addr, size);
}

//#define std_Mem_Alloc Mem_Alloc
//#define std_Mem_Free Mem_Free
//#define std_Mem_ReAlloc Mem_ReAlloc


// <LegoRR.exe @004896b0>
void __cdecl Mem_Initialise(void);

// <missing>
void __cdecl Mem_Shutdown(bool32 freeAll);

// <LegoRR.exe @004896d0>
Mem_HandleValue __cdecl Mem_AllocHandle(uint32 size);

// <LegoRR.exe @00489720>
void __cdecl Mem_FreeHandle(Mem_HandleValue handle);

// <LegoRR.exe @00489760>
void * __cdecl Mem_AddressHandle(Mem_HandleValue handle);

#pragma endregion

}
