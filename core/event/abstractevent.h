#pragma once

#include <string>

class AbstractEvent
{
public:
    virtual void post() const = 0;
    virtual ~AbstractEvent() {};
    virtual std::string toString() = 0;
};
