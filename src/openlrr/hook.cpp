
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

bool hook_write(void* address, const uint8* newData, size_t size, OPTIONAL uint8* backup)
{
    DWORD newProtect = PAGE_EXECUTE_READWRITE;
    DWORD backupProtect, dummy;
    if (!::VirtualProtect(address, size, newProtect, &backupProtect)) {
        std::printf("VirtualProtect PREPARE failed %i\n", (int)::GetLastError());
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
        std::printf("VirtualProtect RESTORE failed %i\n", (int)::GetLastError());
        return false;
    }
    return true;
}


/*

uint32 hook_to_dist(void* src, void* dst, sint32 srcOffset, sint32 dstOffset)
{
    return (((uint32)dst + dstOffset) - ((uint32)src + srcOffset));
}
void* hook_from_dist(void* src, uint32 dist, sint32 srcOffset, sint32 distOffset)
{
    return (void*)(((uint32)dist + ((uint32)src + srcOffset)) - distOffset);
}

bool __cdecl hook_jmp(void* address, void* callAddress, OPTIONAL void** backup)
{

}

bool __cdecl hook_jmpret(void* address, void* callAddress, OPTIONAL void** backup)
{
    const uint32 instrSize = 5;

    uint8 JMP[instrSize + 1] = { 0xE9, 0x90, 0x90, 0x90, 0x90, 0xC3 }; // 0xE9 = JMP 0x90 = NOP (PLACEHOLDER) 0xC3 = RET
    uint8 JMPBackup[sizeof(JMP)] = { 0 };

    //DWORD CALLDist = ((DWORD)callAddress - ((DWORD)address + instrSize));  // calculate jump distance
    uint32 CALLDist = hook_to_dist(address, callAddress, instrSize, 0); // 5 == end of CALL instruction

    std::memcpy(&CALL[1], &CALLDist, 4);
    bool result = hook_write(address, CALL, sizeof(CALL), CALLBackup);
    std::memcpy(&CALLDist, &CALLBackup[1], 4);

    if (backup) *backup = (void*)hook_from_dist(address, CALLDist, instrSize, 0);
    return result;
}

bool __cdecl hook_call(void* address, void* callAddress, OPTIONAL void** backup)
{
    const uint32 instrSize = 5;

    uint8 CALL[instrSize] = { 0xE8, 0x90, 0x90, 0x90, 0x90 }; // 0xE8 = CALL 0x00 = NOP (PLACEHOLDER)
    uint8 CALLBackup[sizeof(CALL)] = { 0 };

    //DWORD CALLDist = ((DWORD)callAddress - ((DWORD)address + instrSize));  // calculate jump distance
    uint32 CALLDist = hook_to_dist(address, callAddress, instrSize, 0); // 5 == end of CALL instruction

    std::memcpy(&CALL[1], &CALLDist, 4);
    bool result = hook_write(address, CALL, sizeof(CALL), CALLBackup);
    std::memcpy(&CALLDist, &CALLBackup[1], 4);

    if (backup) *backup = (void*)hook_from_dist(address, CALLDist, instrSize, 0);
    return result;
}

bool __cdecl hook_write(void* address, const uint8* newData, size_t size, OPTIONAL uint8* backup)
{
    DWORD newProtect = PAGE_EXECUTE_READWRITE;
    DWORD backupProtect, dummy;
    if (!::VirtualProtect(address, size, newProtect, &backupProtect)) {
        std::printf("VirtualProtect PREPARE failed %i\n", (int)::GetLastError());
        return false;
    }

    if (backup) std::memcpy(backup, address, size);  // make backup
    std::memcpy(address, newData, size); // write new data

    if (!::VirtualProtect(address, size, backupProtect, &dummy)) {
        std::printf("VirtualProtect RESTORE failed %i\n", (int)::GetLastError());
        return false;
    }
    return true;
}

*/
