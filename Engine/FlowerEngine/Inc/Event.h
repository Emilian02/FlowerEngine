#pragma once

namespace FlowerEngine
{
    enum class EventType
    {
        None,
        SpacePressed,
        PKey,
    };
    class Event
    {
    public:
        Event() = default;
        Event(EventType et) : mEventType(et) {}
        virtual ~Event() = default;

        EventType GetType() const { return mEventType; }

    private:
        EventType mEventType = EventType::None;
    };

    class SpacePressedEvent : public Event
    {
    public:
        SpacePressedEvent() : Event(EventType::SpacePressed) {}
    };

    class PKeyPresssedEvent : public Event
    {
    public:
        PKeyPresssedEvent() : Event(EventType::PKey) {};
    };
}