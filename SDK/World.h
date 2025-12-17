#pragma once
#include "JNIHelper.h"
#include "Entity.h"
#include <vector>

class World {
public:
    jobject instance;

    World(jobject obj) : instance(obj) {}

    std::vector<jobject> getLoadedEntities() {
        std::vector<jobject> entities;
        if (!instance) return entities;

        jclass clazz = JNIHelper::env->GetObjectClass(instance);
        jfieldID field = JNIHelper::getField(clazz, "f", "Ljava/util/List;"); // loadedEntityList
        jobject listObj = JNIHelper::env->GetObjectField(instance, field);

        jclass listClass = JNIHelper::env->FindClass("java/util/List");
        jmethodID sizeMethod = JNIHelper::getMethod(listClass, "size", "()I");
        jmethodID getMethod = JNIHelper::getMethod(listClass, "get", "(I)Ljava/lang/Object;");

        int size = JNIHelper::env->CallIntMethod(listObj, sizeMethod);
        for (int i = 0; i < size; i++) {
            jobject entity = JNIHelper::env->CallObjectMethod(listObj, getMethod, i);
            entities.push_back(entity);
        }

        JNIHelper::env->DeleteLocalRef(listClass);
        JNIHelper::env->DeleteLocalRef(listObj);
        JNIHelper::env->DeleteLocalRef(clazz);
        return entities;
    }

    ~World() {
        if (instance) JNIHelper::env->DeleteLocalRef(instance);
    }
};
