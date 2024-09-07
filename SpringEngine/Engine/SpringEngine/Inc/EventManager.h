#pragma once

#include "Event.h"

namespace SpringEngine
{
    using EventCallback = std::function<void(const Event*)>;
    using EventListeners = std::map<EventType, std::vector<EventCallback>>;

    class EventManager final
    {
    public:
        static void StaticInitialize();
        static void StaticTerminate();
        static EventManager* Get();
        static void Broadcast(const Event* event);

        EventManager() = default;
        ~EventManager();

        void Initialize();
        void Terminate();

        void AddListener(EventType eventType, EventCallback cb);
        void RemoveListener(EventType eventType, EventCallback cb);

    private:
        void BroadcastPrivate(const Event* event);

        EventListeners mListeners;

    };
}
