#pragma once

#include "common.h"

/// OLD:
// Community Edition requires that this function be exported by LRRCE.DLL
// This requires `extern "C"` and `__cdecl` to avoid function name mangling.
extern "C" __declspec(dllexport) void __cdecl Dummy(void);

int __stdcall OpenLRR_WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int cmdShow);
