#pragma once
#include "Entity.h"

class EntityPlayer : public Entity {
public:
    EntityPlayer(jobject obj) : Entity(obj) {}

    void jump() {
        if (!instance) return;
        jclass clazz = JNIHelper::env->GetObjectClass(instance);
        jmethodID method = JNIHelper::getMethod(clazz, "bF", "()V");
        if (method) {
            JNIHelper::env->CallVoidMethod(instance, method);
        }
        JNIHelper::env->DeleteLocalRef(clazz);
    }

    void setOnGround(bool state) {
        if (!instance) return;
        jclass clazz = JNIHelper::env->GetObjectClass(instance);
        jfieldID field = JNIHelper::getField(clazz, "C", "Z");
        JNIHelper::env->SetBooleanField(instance, field, state);
        JNIHelper::env->DeleteLocalRef(clazz);
    }

    void setMotion(double x, double y, double z) {
        if (!instance) return;
        jclass clazz = JNIHelper::env->GetObjectClass(instance);
        JNIHelper::env->SetDoubleField(instance, JNIHelper::getField(clazz, "v", "D"), x);
        JNIHelper::env->SetDoubleField(instance, JNIHelper::getField(clazz, "w", "D"), y);
        JNIHelper::env->SetDoubleField(instance, JNIHelper::getField(clazz, "x", "D"), z);
        JNIHelper::env->DeleteLocalRef(clazz);
    }

    int getHurtTime() {
        if (!instance) return 0;
        jclass clazz = JNIHelper::env->GetObjectClass(instance);
        int ht = JNIHelper::env->GetIntField(instance, JNIHelper::getField(clazz, "au", "I"));
        JNIHelper::env->DeleteLocalRef(clazz);
        return ht;
    }

    void setFlying(bool state) {
        if (!instance) return;
        jclass clazz = JNIHelper::env->GetObjectClass(instance);
        jfieldID capField = JNIHelper::getField(clazz, "bA", "Lwl;");
        jobject capObj = JNIHelper::env->GetObjectField(instance, capField);
        
        jclass capClass = JNIHelper::env->GetObjectClass(capObj);
        jfieldID flyField = JNIHelper::getField(capClass, "b", "Z");
        JNIHelper::env->SetBooleanField(capObj, flyField, state);

        JNIHelper::env->DeleteLocalRef(capClass);
        JNIHelper::env->DeleteLocalRef(capObj);
        JNIHelper::env->DeleteLocalRef(clazz);
    }

    void attackEntity(Entity* target) {
        if (!instance || !target->instance) return;
        // This usually requires PlayerControllerMP, but we can use a simpler way or call it via Minecraft
    }
};
