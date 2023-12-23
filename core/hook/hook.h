#pragma once

#ifdef _WIN32
#include <Windows.h>
#endif

class Hook
{
public:
    static bool installGlobalMouseHook();
    static bool installGlobalKeybroadHook();
    static void uninstallGlobalMouseHook();
    static void uninstallGlobalKeybroadHook();

protected:
    Hook();

private:
    static LRESULT CALLBACK mouseProc(int nCode, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK keybroadProc(int nCode, WPARAM wParam, LPARAM lParam);

    static HHOOK s_mouseHook;
    static HHOOK s_keybroadHook;
};
