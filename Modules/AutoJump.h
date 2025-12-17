#pragma once
#include "Module.h"
#include <iostream>

class AutoJump : public Module {
public:
    AutoJump() : Module("AutoJump", 0) {} 

    void onUpdate(Minecraft* mc) override {
        if (!enabled || !(GetAsyncKeyState(VK_SPACE) & 0x8000)) return;

        EntityPlayer* player = mc->getThePlayer();
        if (player) {
            player->jump();
            delete player;
        }
    }
};
