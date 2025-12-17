// Modules/KillAura.h dosyasını güncelle
#pragma once
#include "Module.h"

class KillAura : public Module {
public:
    KillAura() : Module("KillAura", 'R') {}

    void onUpdate(Minecraft* mc) override {
        if (!enabled) return;

        EntityPlayer* localPlayer = mc->getThePlayer();
        World* world = mc->getTheWorld();

        if (localPlayer && world) {
            std::vector<jobject> entities = world->getLoadedEntities();
            Entity* closestTarget = nullptr;
            float closestDist = 4.0f; //distance

            for (jobject entityObj : entities) {
                Entity* target = new Entity(entityObj);
                
                
                if (!target->isSameObject(localPlayer) && target->isLiving() && !target->isDead()) {
                    float dist = localPlayer->getDistanceTo(target);
                    if (dist < closestDist) {
                        if (closestTarget) delete closestTarget;
                        closestTarget = target;
                        closestDist = dist;
                        continue; 
                    }
                }
                delete target;
            }

           
            if (closestTarget) {
                mc->attackEntity(localPlayer, closestTarget);
                mc->clickMouse();
                delete closestTarget;
            }
        }

        if (localPlayer) delete localPlayer;
        if (world) delete world;
    }
};