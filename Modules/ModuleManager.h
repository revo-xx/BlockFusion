#pragma once
#include <vector>
#include "Module.h"
#include "AutoJump.h"
#include "AutoClicker.h"
#include "KillAura.h"
#include "Velocity.h"
#include "NoFall.h"
#include "Fly.h"
#include "Sprint.h"
#include "../Hooks.h"

class ModuleManager {
public:
    std::vector<Module*> modules;

    ModuleManager() {
        modules.push_back(new AutoJump());
        modules.push_back(new AutoClicker());
        modules.push_back(new KillAura());
        modules.push_back(new Velocity());
        modules.push_back(new NoFall());
        modules.push_back(new Fly());
        modules.push_back(new Sprint());
    }

    ~ModuleManager() {
        for (auto m : modules) {
            delete m;
        }
    }

    void updateAll(Minecraft* mc) {
        static bool pressed[256] = { false };
        for (auto m : modules) {
            if (m->key != 0 && !Hooks::g_ShowMenu) {
                if (GetAsyncKeyState(m->key) & 0x8000) {
                    if (!pressed[m->key]) {
                        m->toggle();
                        printf("%s %s\n", m->name, m->enabled ? "Enabled" : "Disabled");
                        pressed[m->key] = true;
                    }
                } else {
                    pressed[m->key] = false;
                }
            }

            m->onUpdate(mc);
        }
    }
};
