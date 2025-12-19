#pragma once
#include "JNIHelper.h"

class Entity {
public:
    jobject instance;

    Entity(jobject obj) : instance(obj) {}

    bool isSameObject(Entity* other) {
        if (!instance || !other || !other->instance) return false;
        return JNIHelper::env->IsSameObject(instance, other->instance);
    }

    bool isLiving() {
        if (!instance) return false;
        jclass livingClass = JNIHelper::env->FindClass("pr");
        bool result = JNIHelper::env->IsInstanceOf(instance, livingClass);
        JNIHelper::env->DeleteLocalRef(livingClass);
        return result;
    }

    float getDistanceTo(Entity* other) {
        if (!instance || !other || !other->instance) return 999.0f;
        jclass clazz = JNIHelper::env->GetObjectClass(instance);
        jmethodID method = JNIHelper::getMethod(clazz, "g", "(Lpk;)F");
        float dist = JNIHelper::env->CallFloatMethod(instance, method, other->instance);
        JNIHelper::env->DeleteLocalRef(clazz);
        return dist;
    }

    bool isDead() {
        if (!instance) return true;
        jclass clazz = JNIHelper::env->GetObjectClass(instance);
        jfieldID field = JNIHelper::getField(clazz, "I", "Z");
        bool dead = JNIHelper::env->GetBooleanField(instance, field);
        JNIHelper::env->DeleteLocalRef(clazz);
        return dead;
    }

    void setSprinting(bool state) {
        if (!instance) return;
        jclass clazz = JNIHelper::env->GetObjectClass(instance);
        jmethodID method = JNIHelper::getMethod(clazz, "d", "(Z)V");
        if (method) {
            JNIHelper::env->CallVoidMethod(instance, method, state);
        }
        JNIHelper::env->DeleteLocalRef(clazz);
    }

    ~Entity() {
        if (instance) JNIHelper::env->DeleteLocalRef(instance);
    }
};