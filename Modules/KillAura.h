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
            jobject closestTargetObj = nullptr;
            float closestDist = 4.0f;

            for (jobject entityObj : entities) {
                Entity target(entityObj);
                
                if (!target.isSameObject(localPlayer) && target.isLiving() && !target.isDead()) {
                    float dist = localPlayer->getDistanceTo(&target);
                    if (dist < closestDist) {
                        if (closestTargetObj) JNIHelper::env->DeleteLocalRef(closestTargetObj);
                        closestTargetObj = JNIHelper::env->NewLocalRef(entityObj);
                        closestDist = dist;
                    }
                }
            }

            if (closestTargetObj) {
                Entity target(closestTargetObj);
                mc->attackEntity(localPlayer, &target);
                mc->clickMouse();
            }
        }

        if (localPlayer) delete localPlayer;
        if (world) delete world;
    }
};