#include <iostream>

#include "core/event/keyboardevent.h"
#include "core/event/mouseevent.h"

int main(int argc, char* argv[])
{
    MouseEvent event(MouseEvent::MouseMsgType::MouseMove, 320, 50, 50);
    event.post();
    return 0;
}
