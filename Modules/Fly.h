#pragma once
#include "Module.h"

class Fly : public Module {
public:
    Fly() : Module("Fly", 'F') {}

    void onUpdate(Minecraft* mc) override {
        EntityPlayer* player = mc->getThePlayer();
        if (player) {
            player->setFlying(enabled);
            delete player;
        }
    }
};
