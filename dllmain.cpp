#include "JNI/jni.h"
#include <iostream>
#include <windows.h>

FILE *pFile = nullptr;

JavaVM *vm = nullptr;
JNIEnv *env = nullptr;

void Init(HMODULE hModule)
{
    AllocConsole();
    freopen_s(&pFile, "CONOUT$", "w", stdout);
    printf("[+] DLL Injected Successfully!\n");

    jsize vmCount;
    if (JNI_GetCreatedJavaVMs(&vm, 1, &vmCount) != JNI_OK || vmCount == 0)
    {
        printf("[-] Failed to get Java VM\n");
        goto cleanup;
    }

    jint res = vm->GetEnv((void **)&env, JNI_VERSION_1_6);
    if (res == JNI_EDETACHED)
        res = vm->AttachCurrentThread((void **)&env, nullptr);

    if (res != JNI_OK || env == nullptr)
    {
        printf("[-] Failed to attach current thread to Java VM\n");
        goto cleanup;
    }

    printf("[+] Successfully obtained JNIEnv pointer\n");

    jclass minecraftClass = env->FindClass("ave");
    if (!minecraftClass)
    {
        printf("[-] Failed to find Minecraft class\n");
        goto cleanup;
    }

    jmethodID getMinecraft = env->GetStaticMethodID(minecraftClass, "A", "()Lave;");
    jobject minecraftInstance = env->CallStaticObjectMethod(minecraftClass, getMinecraft);

    jfieldID playerField = env->GetFieldID(minecraftClass, "h", "Lbew;");
    jobject player = env->GetObjectField(minecraftInstance, playerField);

    if (player == nullptr)
    {
        printf("[-] Player object is NULL\n");
        goto cleanup;
    }

    jclass playerClass = env->GetObjectClass(player);
    jmethodID jumpMethod = env->GetMethodID(playerClass, "bF", "()V");
    env->CallVoidMethod(player, jumpMethod);

    printf("[+] Jump method called successfully!\n");

cleanup:
    Sleep(1000);
    fclose(pFile);
    FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{
    switch (reason)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)Init, hModule, 0, nullptr);
        break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
