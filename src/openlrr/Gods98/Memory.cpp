// Memory.cpp : 
//

#include "Errors.h"

#include "Memory.h"


/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// <LegoRR.exe @00545a20>
Gods98::Mem_Globs & Gods98::memGlobs = *(Gods98::Mem_Globs*)0x00545a20; // (no init)

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

/*__inline void* __cdecl Mem_Alloc(unsigned long size) { return std::malloc(size); }
__inline void* __cdecl Mem_ReAlloc(void* addr, unsigned long size) { return std::realloc(addr, size); }
__inline void __cdecl Mem_Free(void* addr) { std::free(addr); }*/


// <LegoRR.exe @004896b0>
void __cdecl Gods98::Mem_Initialise(void)
{
	log_firstcall();

	for (Mem_HandleValue loop = 0; loop < MEMORY_MAXHANDLES; loop++) {
		memGlobs.handleList[loop].addr = nullptr;
		memGlobs.handleList[loop].flags = Mem_HandleFlags::MEMORY_HANDLE_FLAG_NONE;
	}
}

// <missing>
void __cdecl Gods98::Mem_Shutdown(bool32 freeAll)
{
	// debug-only functions
}

// <LegoRR.exe @004896d0>
Gods98::Mem_HandleValue __cdecl Gods98::Mem_AllocHandle(uint32 size)
{
	log_firstcall();

	for (Mem_HandleValue loop = 0; loop < MEMORY_MAXHANDLES; loop++){
		if (!(memGlobs.handleList[loop].flags & Mem_HandleFlags::MEMORY_HANDLE_FLAG_USED)){

			memGlobs.handleList[loop].addr = Mem_Alloc(size);

			memGlobs.handleList[loop].flags |= Mem_HandleFlags::MEMORY_HANDLE_FLAG_USED;
			return (Mem_HandleValue)loop;
		}
	}

	Error_Fatal(true, "Run out of memory handles");
	return MEMORY_HANDLE_INVALID /*-1*/;
}

// <LegoRR.exe @00489720>
void __cdecl Gods98::Mem_FreeHandle(Mem_HandleValue handle)
{
	log_firstcall();

	if (handle >= 0 && handle < MEMORY_MAXHANDLES) {
		if (memGlobs.handleList[handle].flags & Mem_HandleFlags::MEMORY_HANDLE_FLAG_USED) {
			Mem_Free(memGlobs.handleList[handle].addr);
			memGlobs.handleList[handle].addr = nullptr;
			memGlobs.handleList[handle].flags = Mem_HandleFlags::MEMORY_HANDLE_FLAG_NONE;
		}
		else Error_Warn(true, "Handle has already been freed");
	}
	else Error_Warn(true, "Handle is out of range");
}

// <LegoRR.exe @00489760>
void * __cdecl Gods98::Mem_AddressHandle(Mem_HandleValue handle)
{
	log_firstcall();

	if (handle >= 0 && handle < MEMORY_MAXHANDLES) {
		if (memGlobs.handleList[handle].flags & Mem_HandleFlags::MEMORY_HANDLE_FLAG_USED) {

			return memGlobs.handleList[handle].addr;

		}
		else Error_Warn(true, "Handle has already been freed");
	}
	else Error_Warn(true, "Handle is out of range");

	return nullptr;
}

#pragma endregion

