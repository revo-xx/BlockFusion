#include "JNI/jni.h"
#include <iostream>
#include <windows.h>

FILE *pFile = nullptr;

void init(HMODULE hModule)
{
    AllocConsole();
    freopen_s(&pFile, "CONOUT$", "w", stdout);
    printf("DLL Injected Successfully!\n");
}

BOOL APIENTRY DllMain(HMODULE hModule,
                      DWORD ul_reason_for_call,
                      LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)init, hModule, 0, 0);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}