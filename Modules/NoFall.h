#pragma once
#include "Module.h"

class NoFall : public Module {
public:
    NoFall() : Module("NoFall", 0) {}

    void onUpdate(Minecraft* mc) override {
        if (!enabled) return;

        EntityPlayer* player = mc->getThePlayer();
        if (player) {
            player->setOnGround(true);
            delete player;
        }
    }
};
