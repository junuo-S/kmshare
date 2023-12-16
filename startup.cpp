#include <iostream>

#include "core/event/keyboardevent.h"

int main(int argc, char* argv[])
{
    KeyboardEvent event(91, KeyboardEvent::KeyMsgType::KeyDown);
    event.post();
    return 0;
}
