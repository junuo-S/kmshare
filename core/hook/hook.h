#pragma once

#ifdef _WIN32
#include <Windows.h>
#endif
#include <QTimer>

#include "core/event/mouseevent.h"
#include "core/event/keyboardevent.h"

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
    static MouseEvent* enqueueMouseEvent(MouseEvent::MouseMsgType msgType, int rate = 0, double dxPercent = 0, double dyPercent = 0);
    static KeyboardEvent* enqueueKeybroadEvent(unsigned int keyCode, KeyboardEvent::KeyMsgType msgType);
	static MouseEvent* shareMouseEvent(MouseEvent::MouseMsgType msgType, int rate = 0, double dxPercent = 0, double dyPercent = 0);
    static MouseEvent* shareMouseMoveEvent();
	static KeyboardEvent* shareKeybroadEvent(unsigned int keyCode, KeyboardEvent::KeyMsgType msgType);
    static void updateCurrentPos();
    static void shareEvent(AbstractEvent* event);

    static HHOOK s_mouseHook;
    static HHOOK s_keybroadHook;
    static int s_globalX;
    static int s_globalY;
    static QTimer* s_timer;
};
