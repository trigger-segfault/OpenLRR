// hook.cpp : 
//

#include "platform/windows.h"

#include "hook.h"


uint32 hook_to_dist(void* src, void* dst, sint32 srcOffset, sint32 dstOffset)
{
    return (((uint32)dst + dstOffset) - ((uint32)src + srcOffset));
}

void* hook_from_dist(void* src, uint32 dist, sint32 srcOffset, sint32 distOffset)
{
    return (void*)(((uint32)dist + ((uint32)src + srcOffset)) - distOffset);
}

bool hook_write_jmp(void* address, void* jmpAddress, OPTIONAL uint8* backup)
{
    const uint32 instrSize = 5;

    uint8 newData[instrSize] = { 0xE9, 0x90, 0x90, 0x90, 0x90 }; // 0xE9 = JMP 0x90 = NOP (PLACEHOLDER) 0xC3 = RET
    //uint8 origData[sizeof(newData)] = { 0 };

    uint32 addrDist = hook_to_dist(address, jmpAddress, instrSize, 0); // 5 == end of CALL instruction

    std::memcpy(&newData[1], &addrDist, 4);
    bool result = hook_write(address, newData, sizeof(newData), backup); // origData);
    //std::memcpy(&addrDist, &origData[1], 4);

    //if (backup) *backup = hook_from_dist(address, addrDist, instrSize, 0);
    return result;
}

bool hook_write_jmpret(void* address, void* jmpAddress, OPTIONAL uint8* backup)
{
    const uint32 instrSize = 5;

    uint8 newData[instrSize+1] = { 0xE9, 0x90, 0x90, 0x90, 0x90, 0xC3 }; // 0xE9 = JMP 0x90 = NOP (PLACEHOLDER) 0xC3 = RET
    //uint8 origData[sizeof(newData)] = { 0 };

    uint32 addrDist = hook_to_dist(address, jmpAddress, instrSize, 0); // 5 == end of CALL instruction

    std::memcpy(&newData[1], &addrDist, 4);
    bool result = hook_write(address, newData, sizeof(newData), backup); // origData);
    //std::memcpy(&addrDist, &origData[1], 4);

    //if (backup) *backup = hook_from_dist(address, addrDist, instrSize, 0);
    return result;
}

bool hook_write_call(void* address, void* callAddress, OPTIONAL uint8* backup)
{
    const uint32 instrSize = 5;

    uint8 newData[instrSize] = { 0xE8, 0x90, 0x90, 0x90, 0x90 }; // 0xE8 = CALL 0x00 = NOP (PLACEHOLDER)
    //uint8 origData[sizeof(newData)] = { 0 };

    //DWORD addrDist = ((DWORD)callAddress - ((DWORD)address + instrSize));  // calculate jump distance
    uint32 addrDist = hook_to_dist(address, callAddress, instrSize, 0); // 5 == end of CALL instruction

    std::memcpy(&newData[1], &addrDist, 4);
    bool result = hook_write(address, newData, sizeof(newData), backup); // origData);
    //std::memcpy(&addrDist, &origData[1], 4);

    //if (backup) *backup = hook_from_dist(address, addrDist, instrSize, 0);
    return result;
}

bool hook_write_addr(void* address, const void* newAddress, OPTIONAL void** backup)
{
	return hook_write(address, (const uint8*)newAddress, sizeof(newAddress), (uint8*)backup); // origData);
}

bool hook_write(void* address, const uint8* newData, size_t size, OPTIONAL uint8* backup)
{
    DWORD newProtect = PAGE_EXECUTE_READWRITE;
    DWORD backupProtect, dummy;
    if (!::VirtualProtect(address, size, newProtect, &backupProtect)) {
        std::printf("VirtualProtect PREPARE failed %i\n", (sint32)::GetLastError());
        return false;
    }

    //if (backupProtect & PAGE_GUARD)
    //    std::printf("VirtualProtect PAGE_GUARD @ 0x%p (flags: 0x%08x)\n", address, backupProtect);
    //else if (backupProtect == PAGE_EXECUTE)
    //    std::printf("");
    //else if (backupProtect == PAGE_EXECUTE_READ)
    //    std::printf("");
    //else
    //    std::printf("VirtualProtect @ 0x%p (flags: 0x%08x)\n", address, backupProtect);


    if (backup) std::memcpy(backup, address, size);  // make backup
    std::memcpy(address, newData, size); // write new data

    if (!::VirtualProtect(address, size, backupProtect, &dummy)) {
        std::printf("VirtualProtect RESTORE failed %i\n", (sint32)::GetLastError());
        return false;
    }
    return true;
}

bool hook_read(void* address, uint8* data, size_t size)
{
    DWORD newProtect = PAGE_EXECUTE_READWRITE;
    DWORD backupProtect, dummy;
    if (!::VirtualProtect(address, size, newProtect, &backupProtect)) {
        std::printf("VirtualProtect PREPARE failed %i\n", (sint32)::GetLastError());
        return false;
    }

    std::memcpy(data, address, size); // read into data

    if (!::VirtualProtect(address, size, backupProtect, &dummy)) {
        std::printf("VirtualProtect RESTORE failed %i\n", (sint32)::GetLastError());
        return false;
    }
    return true;
}

sint32 hook_cmp(void* address, const uint8* data, size_t size)
{
    DWORD newProtect = PAGE_EXECUTE_READWRITE;
    DWORD backupProtect, dummy;
    if (!::VirtualProtect(address, size, newProtect, &backupProtect)) {
        std::printf("VirtualProtect PREPARE failed %i\n", (sint32)::GetLastError());
        return INT_MIN;// -2;
    }

    sint32 cmp = std::memcmp(address, data, size); // compare with data

    if (!::VirtualProtect(address, size, backupProtect, &dummy)) {
        std::printf("VirtualProtect RESTORE failed %i\n", (sint32)::GetLastError());
        return INT_MIN;// -2;
    }
    return cmp;
}


