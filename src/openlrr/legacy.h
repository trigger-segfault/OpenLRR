// legacy.h : Header file for access to legacy standard library functions in
//            LegoRR native code.
//

#pragma once

#include "common.h"


namespace legacy
{; // !<---

/**********************************************************************************
 ******** Constants
 **********************************************************************************/

#pragma region Constants

#define LEGACY_RAND_MAX		0x7fff

#pragma endregion

/**********************************************************************************
 ******** Structures
 **********************************************************************************/

#pragma region Structs

struct FILE;


#pragma endregion

/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

// Global variable for random number generation.

// <LegoRR.exe @004b0cc8>
extern uint32 & g_rand;

// These globals are used with inlined isspace, isdigit, is*, _isctype functions

// (unused)
// <LegoRR.exe @004b0d68>
extern uint16* (&g_pctype);

// (unused)
// <LegoRR.exe @004b0d6c>
extern uint16* (&g_pwctype);

// <LegoRR.exe @004b0d72>
extern uint16 (&g_wctype)[256];

// <LegoRR.exe @004b0f74>
extern sint32 & g_pcharwidth;

#pragma endregion

/**********************************************************************************
 ******** PRNG Functions
 **********************************************************************************/

#pragma region PRNG Functions

// Random functions must retain their original PRNG implementation.

/// PRNG: MS Quick C - Linear Congruential Generator (LCG)
// Sets the current seed without modification.
// <LegoRR.exe @0048e420>
void __cdecl srand(uint32 seed);

/// PRNG: MS Quick C - Linear Congruential Generator (LCG)
// Although this returns a value between [0,0x7fff], an sint32 type is still returned.
// <LegoRR.exe @0048e430>
sint32 __cdecl rand(void);

#pragma endregion

/**********************************************************************************
 ******** I/O Functions
 **********************************************************************************/

#pragma region I/O Functions

// void std::free(void*);
// <LegoRR.exe @0048de40>
__inline void free(void* ptr)
{
	((void(__cdecl*)(void*))0x0048de40)(ptr);
}

// void* std::malloc(size_t);
// <LegoRR.exe @0048de90>
__inline void* malloc(uint32 size)
{
	return ((void*(__cdecl*)(uint32))0x0048de90)(size);
}

// void* std::realloc(void*, size_t);
// <LegoRR.exe @0048df40>
__inline void* realloc(void* ptr, uint32 new_size)
{
	return ((void*(__cdecl*)(void*, uint32))0x0048df40)(ptr, new_size);
}

// <LegoRR.exe @0048e770>
[[noreturn]] __inline void exit(sint32 exit_code)
{
	((void(__cdecl*)(sint32))0x0048e770)(exit_code);
}

// This isn't actually going it be used, but is listed anyway.
// void* operator_new(size_t);
// <LegoRR.exe @0048ed90>
__inline void* operator_new(uint32 size)
{
	return ((void* (__cdecl*)(uint32))0x0048ed90)(size);
}

// <LegoRR.exe @0048eda0>
__inline sint32 fclose(legacy::FILE* stream)
{
	return ((sint32(__cdecl*)(legacy::FILE*))0x0048eda0)(stream);
}

// <LegoRR.exe @0048ee50>
__inline legacy::FILE* fopen(const char* filename, const char* mode)
{
	return ((legacy::FILE* (__cdecl*)(const char*, const char*))0x0048ee50)(filename, mode);
}

// <LegoRR.exe @0048f570>
__inline sint32 ftell(legacy::FILE* stream)
{
	return ((sint32(__cdecl*)(legacy::FILE*))0x0048f570)(stream);
}

// <LegoRR.exe @0048f720>
__inline sint32 fseek(legacy::FILE* stream, sint32 offset, sint32 origin)
{
	return ((sint32(__cdecl*)(legacy::FILE*, sint32, sint32))0x0048f720)(stream, offset, origin);
}

// <LegoRR.exe @0048f7c0>
__inline uint32 fwrite(IN const void* buffer, uint32 size, uint32 count, legacy::FILE* stream)
{
	return ((uint32(__cdecl*)(const void*, uint32, uint32, legacy::FILE*))0x0048f7c0)(buffer, size, count, stream);
}

// uses `void* std::malloc(size_t);` when buffer is NULL.
// <LegoRR.exe @0048fcc0>
__inline char* _getcwd(char* buffer, sint32 maxlen)
{
	return ((char* (__cdecl*)(char*, sint32))0x0048fcc0)(buffer, maxlen);
}

// <LegoRR.exe @0048fe50>
__inline uint32 fread(OUT void* buffer, uint32 size, uint32 count, legacy::FILE* stream)
{
	return ((uint32(__cdecl*)(void*, uint32, uint32, legacy::FILE*))0x0048fe50)(buffer, size, count, stream);
}


/// WARNING: SUPPOSEDLY std::fflush, but not 100% sure.
// <LegoRR.exe @0048ffe0>
__inline sint32 fflush(legacy::FILE* stream)
{
	return ((sint32(__cdecl*)(legacy::FILE*))0x0048ffe0)(stream);
}


// <LegoRR.exe @004900e0>
__inline sint32 fgetc(legacy::FILE* stream)
{
	return ((sint32(__cdecl*)(legacy::FILE*))0x004900e0)(stream);
}

// <LegoRR.exe @00490110>
__inline char* fgets(OUT char* str, sint32 count, legacy::FILE* stream)
{
	return ((char* (__cdecl*)(char*, sint32, legacy::FILE*))0x00490110)(str, count, stream);
}

// <LegoRR.exe @00490190>
__inline sint32 vfprintf(legacy::FILE* stream, const char* format, std::va_list vlist)
{
	return ((sint32(__cdecl*)(legacy::FILE*, const char*, std::va_list))0x00490190)(stream, format, vlist);
}


// Is it legal to inline this?
// <unused>
__inline sint32 fprintf(legacy::FILE* stream, const char* format, ...)
{
	std::va_list vlist;
	va_start(vlist, format);
	sint32 r = legacy::vfprintf(stream, format, vlist);
	va_end(vlist);
	return r;
}


// uses `void* std::malloc(size_t);` when absPath is NULL.
// <LegoRR.exe @00490260>
__inline char* _fullpath(char* absPath, const char* relPath, uint32 maxLength)
{
	return ((char* (__cdecl*)(char*, const char*, uint32))0x00490260)(absPath, relPath, maxLength);
}

// <LegoRR.exe @004941b0>
__inline sint32 vfscanf(legacy::FILE* stream, const char* format, std::va_list vlist)
{
	return ((sint32(__cdecl*)(legacy::FILE*, const char*, std::va_list))0x004941b0)(stream, format, vlist);
}

// (address comes before, but implemented after vfscanf for clean reference)
// Is it legal to inline this?
// <LegoRR.exe @00490330>
__inline sint32 fscanf(legacy::FILE* stream, const char* format, ...)
{
	std::va_list vlist;
	va_start(vlist, format);
	sint32 r = legacy::vfscanf(stream, format, vlist);
	va_end(vlist);
	return r;
}

#pragma endregion

}
