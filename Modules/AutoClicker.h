#pragma once
#include "Module.h"

class AutoClicker : public Module {
public:
    AutoClicker() : Module("AutoClicker", 0) {}

    void onUpdate(Minecraft* mc) override {
        if (!enabled || !(GetAsyncKeyState(VK_LBUTTON) & 0x8000)) return;

        mc->clickMouse();
    }
};
