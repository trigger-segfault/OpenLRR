#pragma once

#include "common.h"

//#define DEFINE_AUTOINJECT(rvAddress, function) InjectFunction func_ ##function = InjectFunction((void*)( rvAddress ), (function), #function , true)


uint32 hook_to_dist(void* src, void* dst, sint32 srcOffset, sint32 dstOffset);
void* hook_from_dist(void* src, uint32 dist, sint32 srcOffset, sint32 distOffset);


inline uint32 hook_to_dist(uint32 src, void* dst, sint32 srcOffset, sint32 dstOffset) {
    return hook_to_dist((void*)src, dst, srcOffset, dstOffset); }

inline uint32 hook_to_dist(uint32 src, uint32 dst, sint32 srcOffset, sint32 dstOffset) {
    return hook_to_dist((void*)src, (void*)dst, srcOffset, dstOffset); }

inline uint32 hook_from_dist(uint32 src, uint32 dist, sint32 srcOffset, sint32 distOffset) {
    return (uint32)hook_from_dist((void*)src, dist, srcOffset, distOffset); }

bool hook_write_jmp(void* address, void* jmpAddress, OPTIONAL uint8* backup = nullptr);
inline bool hook_write_jmp(uint32 address, void* jmpAddress, OPTIONAL uint8* backup = nullptr) {
    return hook_write_jmp((void*)address, jmpAddress, backup); }

bool hook_write_jmpret(void* address, void* jmpAddress, OPTIONAL uint8* backup = nullptr);
inline bool hook_write_jmpret(uint32 address, void* jmpAddress, OPTIONAL uint8* backup = nullptr) {
    return hook_write_jmpret((void*)address, jmpAddress, backup); }

bool hook_write_call(void* address, void* callAddress, OPTIONAL uint8* backup = nullptr);
inline bool hook_write_call(uint32 address, void* callAddress, OPTIONAL uint8* backup = nullptr) {
    return hook_write_call((void*)address, callAddress, backup); }

bool hook_write(void* address, const uint8* newData, size_t size, OPTIONAL uint8* backup = nullptr);
inline bool hook_write(uint32 address, const uint8* newData, size_t size, OPTIONAL uint8* backup = nullptr) {
    return hook_write((void*)address, newData, size, backup); }

//uint32 hook_to_dist(void* src, void* dst, sint32 srcOffset, sint32 dstOffset);
//void* hook_from_dist(void* src, uint32 dist, sint32 srcOffset, sint32 distOffset);

/*static uint32 hook_get_dist(uint32 src, void* dst, sint32 srcOffset, sint32 dstOffset)
{
    return hook_get_dist((void*)src, dst, srcOffset, dstOffset);
}
static uint32 hook_get_dist(void* src, uint32 dst, sint32 srcOffset, sint32 dstOffset)
{
    return hook_get_dist(src, (void*)dst, srcOffset, dstOffset);
}
static uint32 hook_get_dist(uint32 src, uint32 dst, sint32 srcOffset, sint32 dstOffset)
{
    return hook_get_dist((void*)src, (void*)dst, srcOffset, dstOffset);
}*/

/*bool __cdecl hook_call(void* address, void* callAddress, OPTIONAL void** backup);
bool __cdecl hook_jmp(void* address, void* callAddress, OPTIONAL void** backup);
bool __cdecl hook_jmpret(void* address, void* callAddress, OPTIONAL void** backup);

bool __cdecl hook_write(void* address, const uint8* newData, size_t size, OPTIONAL uint8* backup);*/

//void __cdecl hook_func()
