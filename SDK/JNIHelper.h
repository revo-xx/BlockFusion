#pragma once
#include "../JNI/jni.h"
#include <string>

class JNIHelper {
public:
    static JNIEnv* env;

    static jclass findClass(const char* name) {
        return env->FindClass(name);
    }

    static jmethodID getMethod(jclass clazz, const char* name, const char* sig) {
        return env->GetMethodID(clazz, name, sig);
    }

    static jmethodID getStaticMethod(jclass clazz, const char* name, const char* sig) {
        return env->GetStaticMethodID(clazz, name, sig);
    }

    static jfieldID getField(jclass clazz, const char* name, const char* sig) {
        return env->GetFieldID(clazz, name, sig);
    }

    static jobject getStaticObjectField(jclass clazz, jfieldID fieldID) {
        return env->GetStaticObjectField(clazz, fieldID);
    }
};
