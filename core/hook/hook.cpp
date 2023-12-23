#include "hook.h"

#include <iostream>

HHOOK Hook::s_mouseHook = NULL;
HHOOK Hook::s_keybroadHook = NULL;

Hook::Hook()
{

}

LRESULT Hook::mouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode >= 0)
    {
        MSLLHOOKSTRUCT* mouseStruct = (MSLLHOOKSTRUCT*)lParam;
        switch (wParam) {
            case WM_LBUTTONDOWN:
                std::cout << "左键按下" << std::endl;
                break;
            case WM_LBUTTONUP:
                std::cout << "左键抬起" << std::endl;
                break;
            case WM_RBUTTONDOWN:
                std::cout << "右键按下" << std::endl;
                break;
            case WM_RBUTTONUP:
                std::cout << "右键抬起" << std::endl;
                break;
            case WM_MOUSEWHEEL:
                std::cout << "滚轮滚动: " << GET_WHEEL_DELTA_WPARAM(mouseStruct->mouseData) << std::endl;
                break;
            case WM_MOUSEMOVE:
                std::cout << "鼠标移动：x=" << mouseStruct->pt.x << ", y=" << mouseStruct->pt.y << std::endl;
                break;
            default:
                break;
        }
    }

    // 传递事件给下一个钩子或默认处理
    return CallNextHookEx(s_mouseHook, nCode, wParam, lParam);
}

LRESULT Hook::keybroadProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    return LRESULT();
}

bool Hook::installGlobalKeybroadHook()
{
    s_keybroadHook = SetWindowsHookEx(WH_KEYBOARD_LL, Hook::keybroadProc, NULL, NULL);
    if (s_keybroadHook)
        return true;
    return false;
}

bool Hook::installGlobalMouseHook()
{
    s_mouseHook = SetWindowsHookEx(WH_MOUSE_LL, Hook::mouseProc, NULL, NULL);
    if (s_mouseHook)
        return true;
    return false;
}

void Hook::uninstallGlobalKeybroadHook()
{
    UnhookWindowsHookEx(s_keybroadHook);
}

void Hook::uninstallGlobalMouseHook()
{
    UnhookWindowsHookEx(s_mouseHook);
}
