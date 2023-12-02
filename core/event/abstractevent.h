#pragma once

class AbstractEvent
{
public:
    virtual void post() = 0;
};
