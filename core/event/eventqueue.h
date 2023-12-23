#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

class AbstractEvent;

class EventQueue
{
public:
    static EventQueue* instance();
    ~EventQueue();
    void push(AbstractEvent* event);
    void pop();
    AbstractEvent* front();
    AbstractEvent* back();
    bool empty();

protected:
    EventQueue() = default;

private:
    static EventQueue* s_instance;
    std::queue<AbstractEvent*> m_eventQueue;
    std::mutex m_mutex;
    std::condition_variable m_condition;
};
