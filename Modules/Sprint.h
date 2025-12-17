#pragma once
#include "Module.h"

class Sprint : public Module {
public:
    Sprint() : Module("Sprint", 0) {}

    void onUpdate(Minecraft* mc) override {
        if (!enabled) return;

        EntityPlayer* player = mc->getThePlayer();
        if (player) {
            player->setSprinting(true);
            delete player;
        }
    }
};
