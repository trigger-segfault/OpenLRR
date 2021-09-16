
#include "openlrr.h"
#include "hook.h"
#include "interop.h"
#include "dllmain.h"

#include "Gods98/Main.h"
#include "Legacy/legacy_timeapi.h"


//#define make_func(ADDR, RETURN, CONV, ...) ((RETURN (CONV*)(__VA_ARGS__)) (ADDR))
//#define make_func2(ADDR, RETURN, CONV, ...) ((RETURN (CONV*) __VA_ARGS__) (ADDR))

/*int (APIENTRY* LegoRR_WinMain)(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int cmdShow)
{
    std::printf("[0x%08x] OpenLRRWinMain\n", ::timeGetTime());
}*/

int __stdcall OpenLRR_WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int cmdShow)
{
    MakeConsole();
    // currently this just wraps LegoRR.exe!WinMain, to let us know the hook worked
    std::printf("[0x%08x] OpenLRRWinMain\n", legacy::timeGetTime());

    interop_hook_all();

    // call LegoRR.exe!WinMain @ 0x00477a60
    //make_func2(0x00477a60, int,APIENTRY,(HINSTANCE,HINSTANCE,LPSTR,int))(hInstance, hPrevInstance, lpszCmdLine, cmdShow);
    //return ((int (APIENTRY*)(HINSTANCE, HINSTANCE, LPSTR, int))0x00477a60)(hInstance, hPrevInstance, lpszCmdLine, cmdShow);

    //#define Main_WinMain ((int (APIENTRY*)(HINSTANCE, HINSTANCE, LPSTR, int))0x00477a60)
    //return Main_WinMain(hInstance, hPrevInstance, lpszCmdLine, cmdShow);
    return Gods98::Main_WinMain(hInstance, hPrevInstance, lpszCmdLine, cmdShow);
}
