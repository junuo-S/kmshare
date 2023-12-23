#pragma once

class AbstractEvent
{
public:
    virtual void post() const = 0;
    virtual ~AbstractEvent() {};
};
