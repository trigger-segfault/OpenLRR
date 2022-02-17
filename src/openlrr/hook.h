// hook.h : 
//

#pragma once

#include "common.h"


uint32 hook_to_dist(void* src, void* dst, sint32 srcOffset, sint32 dstOffset);
inline uint32 hook_to_dist(uint32 src, void* dst, sint32 srcOffset, sint32 dstOffset)
{
	return hook_to_dist((void*)src, dst, srcOffset, dstOffset);
}
inline uint32 hook_to_dist(uint32 src, uint32 dst, sint32 srcOffset, sint32 dstOffset)
{
	return hook_to_dist((void*)src, (void*)dst, srcOffset, dstOffset);
}


void* hook_from_dist(void* src, uint32 dist, sint32 srcOffset, sint32 distOffset);
inline uint32 hook_from_dist(uint32 src, uint32 dist, sint32 srcOffset, sint32 distOffset)
{
	return (uint32)hook_from_dist((void*)src, dist, srcOffset, distOffset);
}


bool hook_write_jmp(void* address, void* jmpAddress, OPTIONAL uint8* backup = nullptr);
inline bool hook_write_jmp(uint32 address, void* jmpAddress, OPTIONAL uint8* backup = nullptr)
{
	return hook_write_jmp((void*)address, jmpAddress, backup);
}


bool hook_write_jmpret(void* address, void* jmpAddress, OPTIONAL uint8* backup = nullptr);
inline bool hook_write_jmpret(uint32 address, void* jmpAddress, OPTIONAL uint8* backup = nullptr)
{
	return hook_write_jmpret((void*)address, jmpAddress, backup);
}


bool hook_write_call(void* address, void* callAddress, OPTIONAL uint8* backup = nullptr);
inline bool hook_write_call(uint32 address, void* callAddress, OPTIONAL uint8* backup = nullptr)
{
	return hook_write_call((void*)address, callAddress, backup);
}


bool hook_write_addr(void* address, const void* newAddress, OPTIONAL void** backup = nullptr);
inline bool hook_write_addr(uint32 address, const void* newAddress, OPTIONAL void** backup = nullptr)
{
	return hook_write_addr((void*)address, newAddress, backup);
}


bool hook_write(void* address, const uint8* newData, size_t size, OPTIONAL uint8* backup = nullptr);
inline bool hook_write(uint32 address, const uint8* newData, size_t size, OPTIONAL uint8* backup = nullptr)
{
	return hook_write((void*)address, newData, size, backup);
}


bool hook_read(void* address, uint8* data, size_t size);
inline bool hook_read(uint32 address, uint8* data, size_t size)
{
	return hook_read((void*)address, data, size);
}


sint32 hook_cmp(void* address, const uint8* data, size_t size);
inline sint32 hook_cmp(uint32 address, const uint8* data, size_t size)
{
	return hook_cmp((void*)address, data, size);
}
