#include <windows.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "hook.h"
#include "dxscreen.h"
//#include "sharedmemory.h"


DWORD WINAPI MainLoop(LPVOID param)
{
  /*  wcap::SharedMemory offsets_map("offsets", 64);
    char* presentstr = offsets_map.begin();
    char* presentexstr = presentstr + 32;
*/
    wchar_t presentstr[32];
    wchar_t presentexstr[32];

    std::wifstream in("offsets.txt");
    in.getline(presentstr, 32);
    in.getline(presentexstr, 32);

    uint64_t present = 0;
    uint64_t presentex = 0;

    std::wstringstream ss1;
    ss1 << presentstr;
    ss1 >> present;

    std::wstringstream ss2;
    ss2 << presentexstr;
    ss2 >> presentex;

    std::wstringstream ss3;
    ss3 << L"Present offset:" << presentstr << std::endl;
    ss3 << L"PresentEx offset:" << presentexstr << std::endl;

    std::wofstream out("ioffsets.txt");
    out << present << std::endl << presentex << std::endl;
    out.flush();

    MessageBoxW(NULL, ss3.str().c_str(), L"Dll injected", MB_OK);

    return 0;
}

extern "C"
{

BOOL WINAPI DllMain(HMODULE, DWORD reason, LPVOID)
{
    MessageBoxW(NULL, L"DllMain injected", L"DllMain injected", MB_OK);

    char param[64];
    switch (reason)
    {
    case DLL_PROCESS_ATTACH:
    {
        DWORD thrID;
        //wcap::SharedMemory offsets_map("offsets", 64);
         strcpy(param, "Process");
        //CreateThread(0, 0, MainLoop, (LPVOID)&param[0], 0, &thrID);
        //MainLoop((LPVOID)&param[0]);
    } break;
    case DLL_THREAD_ATTACH:
    {
        DWORD thrID;
        strcpy(param, "Thread");
        //CreateThread(0, 0, MainLoop, (LPVOID)&param[0], 0, &thrID);
        //MainLoop((LPVOID)&param[0]);
    }
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

}
