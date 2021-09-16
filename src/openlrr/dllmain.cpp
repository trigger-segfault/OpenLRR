
#include "hook.h"
#include "interop.h"
#include "dllmain.h"
#include "openlrr.h"


HINSTANCE g_hDllInstance = nullptr;
static BOOL isAttached = false;


/// LEGACY:
extern "C" __declspec(dllexport) void __cdecl Dummy(void) { }


FILE* MakeConsole()
{
    FILE* pConsoleFile = NULL;
    AllocConsole();
    freopen_s(&pConsoleFile, "CONOUT$", "w", stdout);
    SetConsoleTitleA("");
    CONSOLE_CURSOR_INFO conCursorInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &conCursorInfo);
    conCursorInfo.bVisible = false;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &conCursorInfo);
    return pConsoleFile;
}

DWORD WINAPI MainThread(LPVOID param) 
{
	g_hDllInstance = (HMODULE)param;
	if (!isAttached) {
		isAttached = true;
	}
    interop_hook_Gods98_WinMain();

    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,
                      DWORD   fdwReason,
                      LPVOID  lpReserved)
{
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        CreateThread(0, 0, MainThread, hModule, 0, 0);
        //::CreateThread(NULL, )
        /*if (::ResumeThread(ProcessInfo.hThread) == (DWORD)-1) {
            _tprintf(_T("ResumeThread failed\n"));
            return_error(-1);
        }*/
        //interop_hook_Gods98_WinMain();
        //MakeConsole();
        //std::printf("[0x%08x] DLL_PROCESS_ATTACH\n", ::timeGetTime());
        //g_hDllInstance = hModule;
        /*if (!isAttached) {
            //std::printf("[0x%08x] DLL_PROCESS_ATTACH starting...\n", ::timeGetTime());

            //interop_hook_WinMain_call();
            //interop_hook_all();

            isAttached = true;
        }*/
        break;
    //case DLL_THREAD_ATTACH:
        //std::printf("[0x%08x] DLL_THREAD_ATTACH\n", ::timeGetTime());
        //InjectFunction::UnloadAll();
        //break;
    //case DLL_THREAD_DETACH:
        //std::printf("[0x%08x] DLL_THREAD_DETACH\n", ::timeGetTime());
        //InjectFunction::UnloadAll();
        //break;
    case DLL_PROCESS_DETACH:
        //std::printf("[0x%08x] DLL_PROCESS_DETACH\n", ::timeGetTime());
        //InjectFunction::UnloadAll();
        break;
    }
    return TRUE;
}
