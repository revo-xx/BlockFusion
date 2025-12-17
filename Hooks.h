#pragma once
#include <windows.h>
#include <gl/GL.h>

typedef BOOL(WINAPI* twglSwapBuffers)(HDC hdc);

namespace Hooks {
    extern twglSwapBuffers oSwapBuffers;
    extern HWND window;
    extern WNDPROC oWndProc;
    extern bool g_ShowMenu;

    bool Init();
    void Unhook();
    
    LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    BOOL WINAPI hSwapBuffers(HDC hdc);
}
