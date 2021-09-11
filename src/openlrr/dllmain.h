#pragma once

#include "common.h"

extern HINSTANCE g_hDllInstance;

/// LEGACY:
// Community Edition/LRRMonitor requires that this function be exported by LRRCE.DLL/LRRMO.DLL
// This requires `extern "C"` and `__cdecl` to avoid function name mangling.
extern "C" __declspec(dllexport) void __cdecl Dummy(void);

// Entry point where we inject any desired functions.
BOOL APIENTRY DllMain(HMODULE hModule, DWORD fdwReason, LPVOID lpReserved);



