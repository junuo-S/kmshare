#include "abstractevent.h"

class MouseEvent : public AbstractEvent
{
public:
    enum class MouseMsgType : short
    {
        UnKnown,
        LeftButtonClicked,
        RightButtonClicked,
        MiddleButtonClicked,
        LeftButtonDoubleClicked,
        // RightButtonDoubleClicked, // 暂时不支持右键双击
        MouseMove,
        MouseWheel,
    };
    explicit MouseEvent(MouseMsgType msgType, int rate = 0, int dx = 0, int dy = 0);
    void setMsgType(MouseMsgType msgType);
    MouseMsgType getMsgType() const;
    void setRate(int rate);
    int getRate() const;
    void setDx(int dx);
    int getDx() const;
    void setDy(int dy);
    int getDy() const;
    virtual void post() const override;
    virtual std::string toString() override;

private:
    void leftButtonDown() const;
    void leftButtonUp() const;
    void rightButtonDown() const;
    void rightButtonUp() const;
    void leftButtonClicked() const;
    void rightButtonClicked() const;
    void leftButtonDoubleClicked() const;
    void scrollWheel() const;
    void mouseMove() const;

    MouseMsgType m_msgType = MouseMsgType::UnKnown;
    // 记录滚轮转动的速率
    int m_rate = 0;
    // 记录鼠标移动的情况
    int m_dx = 0;
    int m_dy = 0;
};
