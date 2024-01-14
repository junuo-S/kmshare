#include "mouseevent.h"

#ifdef _WIN32
#include <Windows.h>
#endif
#include <QJsonDocument>
#include <QCursor>
#include <QApplication>
#include <QScreen>

MouseEvent::MouseEvent(MouseMsgType msgType, int rate, double dxPercent, double dyPercent)
    : m_msgType(msgType)
    , m_rate(rate)
    , m_dxPercent(dxPercent)
    , m_dyPercent(dyPercent)
{
}

void MouseEvent::setMsgType(MouseMsgType msgType)
{
    this->m_msgType = msgType;
}

MouseEvent::MouseMsgType MouseEvent::getMsgType() const
{
    return m_msgType;
}

void MouseEvent::setRate(int rate)
{
    this->m_rate = rate;
}

int MouseEvent::getRate() const
{
    return m_rate;
}

void MouseEvent::setDxPercent(double dxPercent)
{
    this->m_dxPercent = dxPercent;
}

double MouseEvent::getDxPercent() const
{
    return m_dxPercent;
}

void MouseEvent::setDyPercent(double dyPercent)
{
    this->m_dyPercent = dyPercent;
}

double MouseEvent::getDyPercent() const
{
    return m_dyPercent;
}

#ifdef _WIN32
void MouseEvent::post() const
{
    if (m_msgType == MouseMsgType::UnKnown)
        return;
    switch (m_msgType)
    {
    case MouseMsgType::LeftButtonClicked:
        leftButtonClicked();
        break;
    case MouseMsgType::LeftButtonDown:
        leftButtonDown();
        break;
    case MouseMsgType::LeftButtonUp:
        leftButtonUp();
        break;
    case MouseMsgType::LeftButtonDoubleClicked:
        leftButtonDoubleClicked();
        break;
    case MouseMsgType::MiddleButtonClicked:
        break;
    case MouseMsgType::MouseMove:
        mouseMove();
        break;
    case MouseMsgType::MouseWheel:
        scrollWheel();
        break;
    case MouseMsgType::RightButtonClicked:
        rightButtonClicked();
        break;
    case MouseMsgType::RightButtonDown:
        rightButtonDown();
        break;
    case MouseMsgType::RightButtonUp:
        rightButtonUp();
        break;
    default:
        break;
    }
}

std::string MouseEvent::toString()
{
    QJsonObject json;
    json.insert("className", "MouseEvent");
    json.insert("m_msgType", (unsigned short)m_msgType);
    json.insert("m_rate", m_rate);
    json.insert("m_dxPercent", m_dxPercent);
    json.insert("m_dyPercent", m_dyPercent);
    QJsonDocument jsonDoc(json);
    return jsonDoc.toJson().toStdString() + "split";
}

std::string MouseEvent::getClassName() const
{
    return "MouseEvent";
}

MouseEvent* MouseEvent::fromJsonString(const std::string& json)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(json.c_str());
    if (jsonDoc.isNull() || !jsonDoc.isObject())
        return nullptr;
    return MouseEvent::fromJsonObject(jsonDoc.object());
}

MouseEvent* MouseEvent::fromJsonObject(const QJsonObject& jsonObject)
{
    if (jsonObject.isEmpty() || "MouseEvent" != jsonObject.value("className").toString())
        return nullptr;

    MouseEvent::MouseMsgType msgType = static_cast<MouseEvent::MouseMsgType>(jsonObject.value("m_msgType").toInt());
    int rate = jsonObject.value("m_rate").toInt();
    double dxPercent = jsonObject.value("m_dxPercent").toDouble();
    double dyPercent = jsonObject.value("m_dyPercent").toDouble();
    return new MouseEvent(msgType, rate, dxPercent, dyPercent);
}

void MouseEvent::leftButtonDown() const
{
    POINT cursorPos;
    GetCursorPos(&cursorPos); // 获取当前鼠标位置

    INPUT input = { 0 };

    // 鼠标按下
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTDOWN;
    input.mi.dx = cursorPos.x * 65536 / GetSystemMetrics(SM_CXSCREEN);
    input.mi.dy = cursorPos.y * 65536 / GetSystemMetrics(SM_CYSCREEN);
    SendInput(1, &input, sizeof(INPUT));
}

void MouseEvent::rightButtonDown() const
{
    POINT cursorPos;
    GetCursorPos(&cursorPos); // 获取当前鼠标位置

    INPUT input = { 0 };

    // 鼠标按下
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_RIGHTDOWN;
    input.mi.dx = cursorPos.x * 65536 / GetSystemMetrics(SM_CXSCREEN);
    input.mi.dy = cursorPos.y * 65536 / GetSystemMetrics(SM_CYSCREEN);
    SendInput(1, &input, sizeof(INPUT));
}

void MouseEvent::leftButtonUp() const
{
    POINT cursorPos;
    GetCursorPos(&cursorPos); // 获取当前鼠标位置

    INPUT input = { 0 };

    // 鼠标抬起
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTUP;
    input.mi.dx = cursorPos.x * 65536 / GetSystemMetrics(SM_CXSCREEN);
    input.mi.dy = cursorPos.y * 65536 / GetSystemMetrics(SM_CYSCREEN);
    SendInput(1, &input, sizeof(INPUT));
}

void MouseEvent::rightButtonUp() const
{
    POINT cursorPos;
    GetCursorPos(&cursorPos); // 获取当前鼠标位置

    INPUT input = { 0 };

    // 鼠标抬起
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_RIGHTUP;
    input.mi.dx = cursorPos.x * 65536 / GetSystemMetrics(SM_CXSCREEN);
    input.mi.dy = cursorPos.y * 65536 / GetSystemMetrics(SM_CYSCREEN);
    SendInput(1, &input, sizeof(INPUT));
}

void MouseEvent::leftButtonClicked() const
{
    leftButtonDown();
    leftButtonUp();
}

void MouseEvent::rightButtonClicked() const
{
    rightButtonDown();
    rightButtonUp();
}

void MouseEvent::leftButtonDoubleClicked() const
{
    leftButtonClicked();
    leftButtonClicked();
}

void MouseEvent::scrollWheel() const
{
    INPUT input = {};
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_WHEEL;
    input.mi.mouseData = m_rate;
    SendInput(1, &input, sizeof(INPUT));
}

void MouseEvent::mouseMove() const
{
    static int screenWidth = qApp->primaryScreen()->geometry().width();
    static int screenHeight = qApp->primaryScreen()->geometry().height();
    QCursor::setPos(QCursor::pos() + QPoint(m_dxPercent * screenWidth, m_dyPercent * screenHeight));
}
#endif
