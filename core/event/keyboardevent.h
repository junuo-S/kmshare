#pragma once
#include "abstractevent.h"
#include <set>

class KeyboardEvent : public AbstractEvent
{
public:
    enum class KeyMsgType : short { KeyDown, KeyUp };

    KeyboardEvent(unsigned int keyCode, KeyMsgType type);
    void setKeyCode(unsigned int keyCode);
    unsigned int getKeyCode() const;
    void setMsgType(KeyMsgType type);
    KeyMsgType getMsgType() const;
    virtual void post() const override;
    virtual std::string toString() override;

private:
    unsigned int m_keyCode;
    KeyMsgType m_msgType;
    static std::set<unsigned int> systemKeyCodes;
};
