#include "Hooks.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_opengl2.h"
#include "Modules/ModuleManager.h"
#include <iostream>
#include <cstdint>

extern ModuleManager* g_moduleManager;

twglSwapBuffers Hooks::oSwapBuffers = nullptr;
HWND Hooks::window = nullptr;
WNDPROC Hooks::oWndProc = nullptr;
bool Hooks::g_ShowMenu = false;

static uint8_t originalBytes[14];

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK Hooks::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (uMsg == WM_KEYDOWN && wParam == VK_INSERT) {
        g_ShowMenu = !g_ShowMenu;
        return 0;
    }

    if (g_ShowMenu && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
        return true;

    return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

bool Hooks::Init() {
    window = FindWindowA("LWJGL", nullptr);
    if (!window) window = GetForegroundWindow(); 
    if (!window) return false;
    oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
    void* target = (void*)GetProcAddress(GetModuleHandleA("opengl32.dll"), "wglSwapBuffers");
    if (!target) return false;
    oSwapBuffers = (twglSwapBuffers)VirtualAlloc(nullptr, 20, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    memcpy(originalBytes, target, 5);
    memcpy((void*)oSwapBuffers, originalBytes, 5);
    uint8_t jmpBack[] = { 0xFF, 0x25, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    uintptr_t backAddr = (uintptr_t)target + 5;
    memcpy(jmpBack + 6, &backAddr, 8);
    memcpy((uint8_t*)oSwapBuffers + 5, jmpBack, 14);

    
    DWORD oldProtect;
    VirtualProtect(target, 5, PAGE_EXECUTE_READWRITE, &oldProtect);
    uintptr_t relAddr = (uintptr_t)hSwapBuffers - ((uintptr_t)target + 5);
    *(uint8_t*)target = 0xE9;
    *(uint32_t*)((uint8_t*)target + 1) = (uint32_t)relAddr;
    VirtualProtect(target, 5, oldProtect, &oldProtect);

    return true;
}

BOOL WINAPI Hooks::hSwapBuffers(HDC hdc) {
    static bool init = false;
    if (!init) {
        if (ImGui::CreateContext()) {
            ImGui_ImplWin32_Init(window);
            ImGui_ImplOpenGL2_Init();
            
            
            ImGuiStyle& style = ImGui::GetStyle();
            style.WindowRounding = 5.0f;
            style.FrameRounding = 3.0f;
            style.ScrollbarRounding = 0.0f;
            style.GrabRounding = 3.0f;
            style.WindowBorderSize = 0.0f;
            style.FrameBorderSize = 1.0f;

            ImVec4* colors = style.Colors;
            colors[ImGuiCol_Text]                   = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
            colors[ImGuiCol_WindowBg]               = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
            colors[ImGuiCol_ChildBg]                = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
            colors[ImGuiCol_PopupBg]                = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
            colors[ImGuiCol_Border]                 = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
            colors[ImGuiCol_FrameBg]                = ImVec4(0.16f, 0.29f, 0.48f, 0.54f);
            colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
            colors[ImGuiCol_FrameBgActive]          = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
            colors[ImGuiCol_TitleBg]                = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
            colors[ImGuiCol_TitleBgActive]          = ImVec4(0.16f, 0.29f, 0.48f, 1.00f);
            colors[ImGuiCol_CheckMark]              = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
            colors[ImGuiCol_SliderGrab]             = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
            colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
            colors[ImGuiCol_Button]                 = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
            colors[ImGuiCol_ButtonHovered]          = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
            colors[ImGuiCol_ButtonActive]           = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
            colors[ImGuiCol_Header]                 = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
            colors[ImGuiCol_HeaderHovered]          = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
            colors[ImGuiCol_HeaderActive]           = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);

            init = true;
        }
    }

    if (init && g_ShowMenu) {
        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowSize(ImVec2(300, 400), ImGuiCond_FirstUseEver);
        if (ImGui::Begin("BlockFusion Menu", &g_ShowMenu)) {
            if (g_moduleManager) {
                for (auto m : g_moduleManager->modules) {
                    ImGui::Checkbox(m->name, &m->enabled);
                }
            }
        }
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
    }

    return oSwapBuffers(hdc);
}

void Hooks::Unhook() {
    if (oWndProc) SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)oWndProc);
    
    void* target = (void*)GetProcAddress(GetModuleHandleA("opengl32.dll"), "wglSwapBuffers");
    if (target) {
        DWORD oldProtect;
        VirtualProtect(target, 5, PAGE_EXECUTE_READWRITE, &oldProtect);
        memcpy(target, originalBytes, 5);
        VirtualProtect(target, 5, oldProtect, &oldProtect);
    }
    
    if (oSwapBuffers) VirtualFree((void*)oSwapBuffers, 0, MEM_RELEASE);
}