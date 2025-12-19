#pragma once
#include "JNIHelper.h"
#include "EntityPlayer.h"
#include "World.h"

class Minecraft {
public:
    jobject instance;

    Minecraft(jobject obj) : instance(obj) {}

    static Minecraft getMinecraft() {
        jclass clazz = JNIHelper::findClass("ave");
        jmethodID method = JNIHelper::getStaticMethod(clazz, "A", "()Lave;");
        jobject obj = JNIHelper::env->CallStaticObjectMethod(clazz, method);
        JNIHelper::env->DeleteLocalRef(clazz);
        return Minecraft(obj);
    }

    EntityPlayer* getThePlayer() {
        if (!instance) return nullptr;
        jclass clazz = JNIHelper::env->GetObjectClass(instance);
        jfieldID field = JNIHelper::getField(clazz, "h", "Lbew;");
        jobject playerObj = JNIHelper::env->GetObjectField(instance, field);
        JNIHelper::env->DeleteLocalRef(clazz);
        if (!playerObj) return nullptr;
        return new EntityPlayer(playerObj);
    }

    World* getTheWorld() {
        if (!instance) return nullptr;
        jclass clazz = JNIHelper::env->GetObjectClass(instance);
        jfieldID field = JNIHelper::getField(clazz, "f", "Lbdb;");
        jobject worldObj = JNIHelper::env->GetObjectField(instance, field);
        JNIHelper::env->DeleteLocalRef(clazz);
        if (!worldObj) return nullptr;
        return new World(worldObj);
    }

    void clickMouse() {
        if (!instance) return;
        jclass clazz = JNIHelper::env->GetObjectClass(instance);
        jmethodID method = JNIHelper::getMethod(clazz, "aw", "()V");
        if (method) {
            JNIHelper::env->CallVoidMethod(instance, method);
        }
        JNIHelper::env->DeleteLocalRef(clazz);
    }

    void attackEntity(EntityPlayer* player, Entity* target) {
        if (!instance || !player || !target) return;
        jclass clazz = JNIHelper::env->GetObjectClass(instance);
        jfieldID controllerField = JNIHelper::getField(clazz, "c", "Lbda;");
        jobject controller = JNIHelper::env->GetObjectField(instance, controllerField);

        if (controller) {
            jclass controllerClass = JNIHelper::env->GetObjectClass(controller);
            jmethodID attackMethod = JNIHelper::getMethod(controllerClass, "a", "(Lwn;Lpk;)V");
            if (attackMethod) {
                JNIHelper::env->CallVoidMethod(controller, attackMethod, player->instance, target->instance);
            }
            JNIHelper::env->DeleteLocalRef(controllerClass);
            JNIHelper::env->DeleteLocalRef(controller);
        }
        JNIHelper::env->DeleteLocalRef(clazz);
    }

    ~Minecraft() {
        if (instance) {
            JNIHelper::env->DeleteLocalRef(instance);
        }
    }
};
