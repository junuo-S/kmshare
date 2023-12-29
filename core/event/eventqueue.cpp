#include "eventqueue.h"

#include "abstractevent.h"
#include "mouseevent.h"
#include "keyboardevent.h"

EventQueue* EventQueue::s_instance = new EventQueue;

EventQueue *EventQueue::instance()
{
    return s_instance;
}

EventQueue::~EventQueue()
{
    std::lock_guard<std::mutex> guard(m_mutex);
    while (!m_eventQueue.empty())
    {
        delete m_eventQueue.front();
        m_eventQueue.pop();
    }
}

void EventQueue::push(AbstractEvent *event)
{
    std::lock_guard<std::mutex> guard(m_mutex);
    m_eventQueue.push(event);
    notifyAll();
}

void EventQueue::pop()
{
    std::lock_guard<std::mutex> guard(m_mutex);
    return m_eventQueue.pop();
}

AbstractEvent *EventQueue::front()
{
    std::lock_guard<std::mutex> guard(m_mutex);
    return m_eventQueue.front();
}

AbstractEvent *EventQueue::back()
{
    std::lock_guard<std::mutex> guard(m_mutex);
    return m_eventQueue.back();
}

bool EventQueue::empty()
{
    std::lock_guard<std::mutex> guard(m_mutex);
    return m_eventQueue.empty();
}

void EventQueue::waitForEvent()
{
    std::unique_lock<std::mutex> uniqueLock(m_mutex);
    m_condition.wait(uniqueLock);
}

void EventQueue::notifyAll()
{
    m_condition.notify_all();
}
