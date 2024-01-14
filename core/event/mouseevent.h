#include "abstractevent.h"

#include <QJsonObject>

class MouseEvent : public AbstractEvent
{
public:
    enum class MouseMsgType : short
    {
        UnKnown,
        LeftButtonClicked,
        LeftButtonDown,
        LeftButtonUp,
        RightButtonClicked,
        RightButtonDown,
        RightButtonUp,
        MiddleButtonClicked,
        LeftButtonDoubleClicked,
        // RightButtonDoubleClicked, // 暂时不支持右键双击
        MouseMove,
        MouseWheel,
    };
    explicit MouseEvent(MouseMsgType msgType, int rate = 0, double dxPercent = 0, double dyPercent = 0);
    void setMsgType(MouseMsgType msgType);
    MouseMsgType getMsgType() const;
    void setRate(int rate);
    int getRate() const;
    void setDxPercent(double dxPercent);
    double getDxPercent() const;
    void setDyPercent(double dyPercent);
    double getDyPercent() const;
    virtual void post() const override;
    virtual std::string toString() override;
    virtual std::string getClassName() const override;
    static MouseEvent* fromJsonString(const std::string& json);
    static MouseEvent* fromJsonObject(const QJsonObject& jsonObject);

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
    double m_dxPercent = 0;
    double m_dyPercent = 0;
};
