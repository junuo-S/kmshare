#include "hook.h"

#include <iostream>
#include <QCursor>

#include "core/event/eventqueue.h"

HHOOK Hook::s_mouseHook = NULL;
HHOOK Hook::s_keybroadHook = NULL;
int Hook::s_globalX = 0;
int Hook::s_globalY = 0;

Hook::Hook()
{

}

LRESULT Hook::mouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    bool bCallNext = false;
    if (nCode >= 0)
    {
        MSLLHOOKSTRUCT* mouseStruct = (MSLLHOOKSTRUCT*)lParam;
        switch (wParam)
        {
            case WM_LBUTTONDOWN:
                enqueueMouseEvent(MouseEvent::MouseMsgType::LeftButtonDown);
                break;
            case WM_LBUTTONUP:
                enqueueMouseEvent(MouseEvent::MouseMsgType::LeftButtonUp);
                break;
            case WM_RBUTTONDOWN:
                enqueueMouseEvent(MouseEvent::MouseMsgType::RightButtonDown);
                break;
            case WM_RBUTTONUP:
                enqueueMouseEvent(MouseEvent::MouseMsgType::RightButtonUp);
                break;
            case WM_MOUSEWHEEL:
                enqueueMouseEvent(MouseEvent::MouseMsgType::MouseWheel, GET_WHEEL_DELTA_WPARAM(mouseStruct->mouseData));
                break;
            case WM_MOUSEMOVE:
				enqueueMouseEvent(MouseEvent::MouseMsgType::MouseMove, 0, mouseStruct->pt.x - s_globalX, mouseStruct->pt.y - s_globalY);
				updateCurrentPos();
                bCallNext = true;
                break;
            default:
                break;
        }
    }

    if (bCallNext)
        return CallNextHookEx(s_mouseHook, nCode, wParam, lParam);
    return 1;
}

LRESULT Hook::keybroadProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    return CallNextHookEx(s_keybroadHook, nCode, wParam, lParam);
}

MouseEvent* Hook::enqueueMouseEvent(MouseEvent::MouseMsgType msgType, int rate /*= 0*/, int dx /*= 0*/, int dy /*= 0*/)
{
    auto event = new MouseEvent(msgType, rate, dx, dy);
    EventQueue::instance()->push(event);
    std::cout << "enqueue: " << event->toString() << std::endl;
    return event;
}

KeyboardEvent* Hook::enqueueKeybroadEvent(unsigned int keyCode, KeyboardEvent::KeyMsgType msgType)
{
    auto event = new KeyboardEvent(keyCode, msgType);
	EventQueue::instance()->push(event);
    std::cout << "enqueue: " << event->toString() << std::endl;
	return event;
}

void Hook::updateCurrentPos()
{
	QPoint globalPos = QCursor::pos();
	s_globalX = globalPos.x();
	s_globalY = globalPos.y();
}

bool Hook::installGlobalKeybroadHook()
{
    s_keybroadHook = SetWindowsHookEx(WH_KEYBOARD_LL, Hook::keybroadProc, NULL, NULL);
    if (s_keybroadHook)
    {
        updateCurrentPos();
        return true;
    }
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
