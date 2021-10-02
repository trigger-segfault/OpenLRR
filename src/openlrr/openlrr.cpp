// openlrr.cpp : 
//

#include "platform/windows.h"
#include "platform/resource.h"

#include "openlrr.h"
#include "hook.h"
#include "interop.h"
#include "dllmain.h"

#include "Gods98/Main.h"


/**********************************************************************************
 ******** Globals
 **********************************************************************************/

#pragma region Globals

/// TODO: Test if assigning this beforehand causes any slowdown and risks a race
///        condition when hooking WinMain.
OpenLRR_Globs openlrrGlobs = { nullptr };

OpenLRR_Config openlrrConfig = { 0 };

#pragma endregion

/**********************************************************************************
 ******** Functions
 **********************************************************************************/

#pragma region Functions

FILE* MakeConsole(void)
{
    FILE* pConsoleFile = nullptr;
    ::AllocConsole();
    ::freopen_s(&pConsoleFile, "CONOUT$", "w", stdout);
    ::SetConsoleTitleA("");
    CONSOLE_CURSOR_INFO conCursorInfo;
    ::GetConsoleCursorInfo(::GetStdHandle(STD_OUTPUT_HANDLE), &conCursorInfo);
    conCursorInfo.bVisible = false;
    ::SetConsoleCursorInfo(::GetStdHandle(STD_OUTPUT_HANDLE), &conCursorInfo);
    return pConsoleFile;
}

bool InjectOpenLRR(HINSTANCE hInstanceDll)
{
    // Store our dll instance so we can access it for loading custom OpenLRR resources later on.
    openlrrGlobs.hInstDll = hInstanceDll;
    openlrrGlobs.method = InjectMethod::DllImport;


    // Determine if openlrr-injector was used to load this dll.
    static const constexpr uint8 eipPatch[2]        = { 0xEB, 0xFE }; // infinite jmp to itself
    static const constexpr uint32 PROCESS_EIP       = 0x0048F2C0; // LegoRR.exe!entry
    static const constexpr uint32 PROCESS_WINMAIN   = 0x00477a60; // LegoRR.exe!WinMain

    if (hook_cmp(PROCESS_EIP, eipPatch, sizeof(eipPatch)) == 0)
        openlrrGlobs.method = InjectMethod::Injector;


    // Redirect `LegoRR.exe!WinMain` to immediately jump to `openlrr.dll!StartOpenLRR`.
    bool result = hook_write_jmpret(PROCESS_WINMAIN, StartOpenLRR);

    // Alternate hook method. Less stable, and not supported by openlrr-injector.
    //// Replaces `LegoRR.exe!entry`'s call to `LegoRR.exe!WinMain` with `openlrr.dll!StartOpenLRR`.
    //bool result = hook_write_call(0x0048f3fa, StartOpenLRR, nullptr);

    return result;
}

extern "C" __declspec(dllexport) sint32 __stdcall StartOpenLRR(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, sint32 nCmdShow)
{
    openlrrGlobs.conout = MakeConsole();

    const char* methodName;
    switch (openlrrGlobs.method) {
    case InjectMethod::Unknown:     methodName = "Unknown";     break;
    case InjectMethod::NotAttached: methodName = "NotAttached"; break;
    case InjectMethod::Injector:    methodName = "Injector";    break;
    case InjectMethod::DllImport:   methodName = "DllImport";   break;
    default:                        methodName = "<???>";       break;
    }
    std::printf("InjectMethod::%s\n", methodName);
    //::system("pause");

    // Hook all functions that aren't being called through OpenLRR code.
    interop_hook_all();

    return Gods98::Main_WinMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}

extern "C" __declspec(dllexport) void __cdecl Dummy(void)
{
}

#pragma endregion
