#include <iostream>
#include <QApplication>

#include "core/event/keyboardevent.h"
#include "core/event/mouseevent.h"
#include "gui/kmsharewindow.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    KMShareWindow kmShareWindow;
    kmShareWindow.show();
    AbstractEvent* event = new MouseEvent(MouseEvent::MouseMsgType::LeftButtonClicked);
    std::cout << event->toString() << std::endl;
    return app.exec();
}
