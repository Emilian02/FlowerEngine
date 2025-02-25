#include "Precompiled.h"
#include "EventManager.h"

using namespace FlowerEngine;

namespace
{
    std::unique_ptr<EventManager> sEventManager;
}

void EventManager::StaticInitialize()
{
    ASSERT(sEventManager == nullptr, "EventManager:: is already initialize");
    sEventManager = std::make_unique<EventManager>();
    sEventManager->Initialize();
}

void EventManager::StaticTerminate()
{
    if (sEventManager != nullptr)
    {
        sEventManager->Terminate();
        sEventManager.reset();
    }
}

EventManager* EventManager::Get()
{
    ASSERT(sEventManager != nullptr, "EventManager: is not initialized");
    return sEventManager.get();
}

void EventManager::Boradcast(const Event& e)
{
    sEventManager->BroadcastPrivate(e);
}

EventManager::~EventManager()
{
    ASSERT(mEventListeners.empty(), "EventManager: terminate must be called");
}

void EventManager::Initialize()
{
    mEventListeners.clear();
}

void EventManager::Terminate()
{
    mEventListeners.clear();
}

ListenerId EventManager::AddListener(EventType eventType, const EventCallback& cb)
{
    mEventListeners[eventType][++mNextLitener] = cb;
    return mNextLitener;
}

void EventManager::RemoveListener(EventType eventType, ListenerId listenerId)
{
    auto listenerIter = mEventListeners.find(eventType);
    if (listenerIter != mEventListeners.end())
    {
        auto iter = listenerIter->second.find(listenerId);
        if (iter != listenerIter->second.end())
        {
            listenerIter->second.erase(iter);
        }
    }
}

void EventManager::BroadcastPrivate(const Event& e)
{
    auto listenerIter = mEventListeners.find(e.GetType());
    if (listenerIter != mEventListeners.end())
    {
        for (auto& cb : listenerIter->second)
        {
            cb.second(e);
        }
    }
}
