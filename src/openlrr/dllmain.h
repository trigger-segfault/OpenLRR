// dllmain.h : 
//

#pragma once

#include "platform/windows.h"

#include "common.h"

// Entry point where we inject our OpenLRR hook.
BOOL APIENTRY DllMain(HMODULE hModule, DWORD fdwReason, LPVOID lpReserved);

