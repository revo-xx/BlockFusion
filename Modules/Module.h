#pragma once
#include "../JNI/jni.h"
#include "../SDK/Minecraft.h"
#include <windows.h>

class Module {
public:
    const char* name;
    int key;
    bool enabled;

    Module(const char* name, int key) : name(name), key(key), enabled(false) {}
    virtual ~Module() {}

    virtual void onUpdate(Minecraft* mc) = 0;
    
    void toggle() {
        enabled = !enabled;
    }
};
