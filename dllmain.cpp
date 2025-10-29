#include "JNI/jni.h"
#include <iostream>
#include <windows.h>

FILE *pFile = nullptr;

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

        jclass minecraftClass = env->FindClass("ave");
        jmethodID getMinecraft = env->GetStaticMethodID(minecraftClass, "A", "()Lave;");
        jobject minecraftInstance = env->CallStaticObjectMethod(minecraftClass, getMinecraft);

        jfieldID playerField = env->GetFieldID(minecraftClass, "h", "Lbew;");
        jobject player = env->GetObjectField(minecraftInstance, playerField);

        if (player == NULL)
        {
            printf("Player object is NULL\n");
            return;
        }

        jclass playerClass = env->GetObjectClass(player);

        jmethodID jumpMethod = env->GetMethodID(playerClass, "bF", "()V");
        env->CallVoidMethod(player, jumpMethod);
    }
    else
    {
        printf("Failed to obtain JNIEnv pointer\n");
    }

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
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

şuan çalışırmı