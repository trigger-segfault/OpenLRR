// Memory.h : 
//
/// APIS: -
/// DEPENDENCIES: -, (Errors)
/// DEPENDENTS: Files

#pragma once

#include "../../common.h"
#include "../../legacy.h"


namespace Gods98
{; // !<---

/**********************************************************************************
 ******** Constants
 **********************************************************************************/

#pragma region Constants

#define MEMORY_MAXHANDLES					2000

#define MEMORY_HANDLE_INVALID				(static_cast<Mem_HandleValue>(-1))

#pragma endregion

/**********************************************************************************
 ******** Enumerations
 **********************************************************************************/

#pragma region Enums

flags_scoped(Mem_GlobFlags) : uint32
{
	MEMORY_GLOB_FLAG_NONE = 0,
};
flags_scoped_end(Mem_GlobFlags, 0x4);


flags_scoped(Mem_HandleFlags) : uint32
{
	MEMORY_HANDLE_FLAG_NONE = 0,

	MEMORY_HANDLE_FLAG_USED = 0x1,
};
flags_scoped_end(Mem_HandleFlags, 0x4);

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
assert_sizeof(Mem_Handle, 0x8);


struct Mem_Globs
{
	/*0000,3e80*/ Mem_Handle handleList[MEMORY_MAXHANDLES];
	/*3e80,4*/ Mem_GlobFlags flags;  // (unused)
	/*3e84*/
};
assert_sizeof(Mem_Globs, 0x3e84);

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

#define Mem_StopAtID(i)			(i)
#define Mem_StopAtAddress(i)	(i)
#define Mem_DebugTrash(a,v,s)


// void* std::malloc(size_t);
// <LegoRR.exe @0048de90>
__inline void * Mem_Alloc(uint32 size)
{
	return legacy::malloc(size);
	//return ((void* (__cdecl*)(uint32))0x0048de90)(size);
}

// void std::free(void*);
// <LegoRR.exe @0048de40>
__inline void Mem_Free(void * addr)
{
	return legacy::free(addr);
	//((void(__cdecl*)(void*))0x0048de40)(addr);
}

// void* std::realloc(void*, size_t);
// <LegoRR.exe @0048df40>
__inline void * Mem_ReAlloc(void * addr, uint32 size)
{
	return legacy::realloc(addr, size);
	//return ((void*(__cdecl*)(void*, uint32))0x0048df40)(addr, size);
}


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
