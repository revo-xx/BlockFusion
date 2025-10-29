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
        jfieldID playerX = env->GetFieldID(playerClass, "field_147554_b", "D");
        jfieldID playerY = env->GetFieldID(playerClass, "field_147555_c", "D");
        jfieldID playerZ = env->GetFieldID(playerClass, "field_147552_d", "D");

        if (player != NULL)
        {
            while (true)
            {
                double x = env->GetDoubleField(player, playerX);
                double y = env->GetDoubleField(player, playerY);
                double z = env->GetDoubleField(player, playerZ);
                printf("Player Position - X: %f, Y: %f, Z: %f\n", x, y, z);
                Sleep(1000); // Sleep for 1 second
            }
        }
    }
    else
    {
        printf("Failed to obtain JNIEnv pointer\n");
    }
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