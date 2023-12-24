#include "keyboardevent.h"

#ifdef _WIN32
#include "Windows.h"
#endif
#include <QJsonObject>
#include <QJsonDocument>

KeyboardEvent::KeyboardEvent(unsigned int keyCode, KeyboardEvent::KeyMsgType type)
    : m_keyCode(keyCode)
    , m_msgType(type)
{

}

unsigned int KeyboardEvent::getKeyCode() const
{
    return m_keyCode;
}

void KeyboardEvent::setKeyCode(unsigned int keyCode)
{
    m_keyCode = keyCode;
}

KeyboardEvent::KeyMsgType KeyboardEvent::getMsgType() const
{
    return m_msgType;
}

void KeyboardEvent::setMsgType(KeyboardEvent::KeyMsgType type)
{
    m_msgType = type;
}

#ifdef _WIN32
void KeyboardEvent::post() const
{
    HWND hWnd = GetForegroundWindow();
    UINT msgId = -1;
    switch (m_msgType)
    {
    case KeyMsgType::KeyDown:
        msgId = WM_KEYDOWN;
        break;
    case KeyMsgType::KeyUp:
        msgId = WM_KEYUP;
        break;
    default:
        break;
    }
    if (msgId != -1)
        PostMessageA(hWnd, msgId, m_keyCode, 0);
}

std::string KeyboardEvent::toString()
{
    QJsonObject json;
    json.insert("className", "KeyboardEvent");
    json.insert("m_keyCode", (int)m_keyCode);
    json.insert("m_msgType", (unsigned short)m_msgType);
    QJsonDocument jsonDoc(json);
    return jsonDoc.toJson().toStdString();
}

#endif
