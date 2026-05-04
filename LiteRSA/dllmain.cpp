// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "inner_use.h"
#include <stdio.h>

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    //Static linking: lpReserved = 000000E43E1EF310 (when loading) and 0x0000000000000001 (when unload)
    //Dynamic linking : lpReserved = 0x0000000000000000 (when loading and when unloading)
    //printf("%p\n", lpReserved);

    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        TCHAR dllName[MAX_PATH];
        GetModuleFileName(hModule, dllName, MAX_PATH);
        return CheckCB(dllName);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}