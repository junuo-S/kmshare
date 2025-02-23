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
    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
}

void MouseEvent::rightButtonDown() const
{
    mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
}

void MouseEvent::leftButtonUp() const
{
    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}

void MouseEvent::rightButtonUp() const
{
    mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
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
    mouse_event(MOUSEEVENTF_WHEEL, 0, 0, m_rate, 0);
}

void MouseEvent::mouseMove() const
{
    static int screenWidth = qApp->primaryScreen()->geometry().width();
    static int screenHeight = qApp->primaryScreen()->geometry().height();
    QPoint currentPos = QCursor::pos();
#ifdef _WIN32
    mouse_event(MOUSEEVENTF_MOVE, m_dxPercent * screenWidth, m_dyPercent * screenHeight, 0, 0);
#else
    QCursor::setPos(currentPos + QPoint(m_dxPercent * screenWidth, m_dyPercent * screenHeight));
#endif // _WIN32
}
