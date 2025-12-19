#pragma once
#include "Module.h"

class Velocity : public Module {
public:
    Velocity() : Module("Velocity", 0) {}

    void onUpdate(Minecraft* mc) override {
        if (!enabled) return;

        EntityPlayer* player = mc->getThePlayer();
        if (player) {
            if (player->getHurtTime() > 0) {
                player->setMotion(0, 0, 0);
            }
            delete player;
        }
    }
};
