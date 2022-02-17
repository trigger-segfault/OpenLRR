// dllmain.cpp : 
//

#include "platform/windows.h"

#include "dllmain.h"
#include "OpenLRR.h"


// At the moment this is disabled because it worsens the race condition
//  when launching OpenLRR though the Community Edition method.
//#define USE_CREATETHREAD


static bool isAttached = false;


// Used with `USE_CREATETHREAD` define.
DWORD WINAPI DllMainThread(LPVOID hModule)
{
    InjectOpenLRR((HMODULE)hModule);

    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,
                      DWORD   fdwReason,
                      LPVOID  lpReserved)
{
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        if (!isAttached) {
            isAttached = true;

            #ifdef USE_CREATETHREAD

            ::DisableThreadLibraryCalls(hModule);
            ::CreateThread(0, 0, DllMainThread, hModule, 0, 0);

            #else

            InjectOpenLRR(hModule);

            #endif
        }
        break;
    //case DLL_THREAD_ATTACH:
        //std::printf("[0x%08x] DLL_THREAD_ATTACH\n", ::timeGetTime());
        //break;
    //case DLL_THREAD_DETACH:
        //std::printf("[0x%08x] DLL_THREAD_DETACH\n", ::timeGetTime());
        //break;
    case DLL_PROCESS_DETACH:
        //std::printf("[0x%08x] DLL_PROCESS_DETACH\n", ::timeGetTime());
        break;
    }
    return TRUE;
}
