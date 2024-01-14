#include "hook.h"

#include <QCursor>
#include <QApplication>
#include <QScreen>

#include "core/event/eventqueue.h"
#include "core/network/kmshareserver.h"

HHOOK Hook::s_mouseHook = NULL;
HHOOK Hook::s_keybroadHook = NULL;
int Hook::s_globalX = 0;
int Hook::s_globalY = 0;
QTimer* Hook::s_timer = nullptr;

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
                shareMouseEvent(MouseEvent::MouseMsgType::LeftButtonDown);
                break;
            case WM_LBUTTONUP:
                shareMouseEvent(MouseEvent::MouseMsgType::LeftButtonUp);
                break;
            case WM_RBUTTONDOWN:
                shareMouseEvent(MouseEvent::MouseMsgType::RightButtonDown);
                break;
            case WM_RBUTTONUP:
                shareMouseEvent(MouseEvent::MouseMsgType::RightButtonUp);
                break;
            case WM_MOUSEWHEEL:
                shareMouseEvent(MouseEvent::MouseMsgType::MouseWheel, GET_WHEEL_DELTA_WPARAM(mouseStruct->mouseData));
                break;
            case WM_MOUSEMOVE:
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

MouseEvent* Hook::enqueueMouseEvent(MouseEvent::MouseMsgType msgType, int rate /*= 0*/, double dxPercent /*= 0*/, double dyPercent /*= 0*/)
{
    auto event = new MouseEvent(msgType, rate, dxPercent, dyPercent);
    EventQueue::instance()->push(event);
    return event;
}

KeyboardEvent* Hook::enqueueKeybroadEvent(unsigned int keyCode, KeyboardEvent::KeyMsgType msgType)
{
    auto event = new KeyboardEvent(keyCode, msgType);
	EventQueue::instance()->push(event);
	return event;
}

MouseEvent* Hook::shareMouseEvent(MouseEvent::MouseMsgType msgType, int rate /*= 0*/, double dxPercent /*= 0*/, double dyPercent /*= 0*/)
{
	auto event = new MouseEvent(msgType, rate, dxPercent, dyPercent);
    shareEvent(event);
	return event;
}

MouseEvent* Hook::shareMouseMoveEvent()
{
	static int screenWidth = qApp->primaryScreen()->size().width();
	static int screenHeight = qApp->primaryScreen()->size().height();
	static double threshold = 0.95;

    int currentX = QCursor::pos().x();
    int currentY = QCursor::pos().y();
    updateCurrentPos();
    if (currentX == s_globalX && currentY == s_globalY)
        return nullptr;
    double deltaXPercent = (QCursor::pos().x() - s_globalX) * 1.0 / screenWidth;
    double deltaYPercent = (QCursor::pos().y() - s_globalY) * 1.0 / screenHeight;
	s_globalX = QCursor::pos().x();
	s_globalY = QCursor::pos().y();
    if (std::abs(deltaXPercent) > threshold || std::abs(deltaYPercent) > threshold)
        return nullptr;
    auto event = shareMouseEvent(MouseEvent::MouseMsgType::MouseMove, 0, deltaXPercent, deltaYPercent);
    return event;
}

KeyboardEvent* Hook::shareKeybroadEvent(unsigned int keyCode, KeyboardEvent::KeyMsgType msgType)
{
	auto event = new KeyboardEvent(keyCode, msgType);
    shareEvent(event);
	return event;
}

void Hook::updateCurrentPos()
{
	QPoint globalPos = QCursor::pos();
    static QRect screenRect = qApp->primaryScreen()->geometry();
    static int threshold = 10;
    if (globalPos.y() < screenRect.top() + threshold)
        QCursor::setPos(globalPos.x(), screenRect.bottom() - threshold);
    else if (globalPos.y() > screenRect.bottom() - threshold)
        QCursor::setPos(globalPos.x(), screenRect.top() + threshold);
    if (globalPos.x() < screenRect.left() + threshold)
        QCursor::setPos(screenRect.right() - threshold, globalPos.y());
    else if (globalPos.x() > screenRect.right() - threshold)
        QCursor::setPos(screenRect.left() + threshold, globalPos.y());
}

void Hook::shareEvent(AbstractEvent* event)
{
    if (!event)
        return;
    auto deviceList = KMShareServer::deviceList();
	for (Device* device : deviceList)
	{
		if (device->m_needShare && device->isConnected())
		{
			device->m_tcpSocket->write(event->toString().c_str());
		}
	}
    delete event;
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
    s_timer = new QTimer;
    s_timer->setInterval(10);
    QObject::connect(s_timer, &QTimer::timeout, Hook::shareMouseMoveEvent);
    s_timer->start();
    s_mouseHook = SetWindowsHookEx(WH_MOUSE_LL, Hook::mouseProc, NULL, NULL);
    s_globalX = QCursor::pos().x();
    s_globalY = QCursor::pos().y();
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
    s_timer->stop();
    delete s_timer;
    s_timer = nullptr;
}
