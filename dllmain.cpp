#include "JNI/jni.h"
#include <iostream>
#include <windows.h>
#include "Modules/ModuleManager.h"
#include "SDK/JNIHelper.h"
#include "Hooks.h"

FILE *pFile = nullptr;
ModuleManager* g_moduleManager = nullptr;

JavaVM *vm;
JNIEnv *env = NULL;

void Init(HMODULE hModule)
{
    AllocConsole();
    freopen_s(&pFile, "CONOUT$", "w", stdout);
    printf("DLL Injected Successfully!\n");

    jsize vmCount;
    if (JNI_GetCreatedJavaVMs(&vm, 1, &vmCount) != JNI_OK || vmCount == 0)
    {
        printf("Failed to get Java VM\n");
        return;
    }

    jint res = vm->GetEnv((void **)&env, JNI_VERSION_1_6);
    if (res == JNI_EDETACHED)
    {
        res = vm->AttachCurrentThread((void **)&env, nullptr);
    }
    if (res != JNI_OK)
    {
        printf("Failed to attach current thread to Java VM\n");
        return;
    }
    if (env != nullptr)
    {
        printf("Successfully obtained JNIEnv pointer\n");
        JNIHelper::env = env;
        g_moduleManager = new ModuleManager();

        if (Hooks::Init()) {
            printf("Hooks initialized successfully!\n");
        } else {
            printf("Failed to initialize hooks!\n");
        }

        while (true)
        {
            if (GetAsyncKeyState(VK_END) & 0x8000)
                break;

            Minecraft mc = Minecraft::getMinecraft();
            if (mc.instance)
            {
                g_moduleManager->updateAll(&mc);
            }
            Sleep(10); 
        }
        Hooks::Unhook();
        delete g_moduleManager;
    }
    else
    {
        printf("Failed to obtain JNIEnv pointer\n");
    }

    printf("Unloading...\n");
    if (pFile) fclose(pFile);
    FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);
}

BOOL APIENTRY DllMain(HMODULE hModule,
                      DWORD ul_reason_for_call,
                      LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Init, hModule, 0, 0);
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}