#pragma once
#include <functional>
#include <string>
#include "PacketType.h"
#include "PacketManager.h"

#define EVENT_MANAGER EventManager::Instance()

class EventManager {
public:
    using Callback = std::function<void(int, CustomPacket&)>;

    void Subscribe(const PacketType type, Callback callback);

    void Emit(const PacketType type, int data, CustomPacket customPacket);

    static EventManager& Instance();

private:

    std::unordered_map<PacketType, std::vector<Callback>> subscribers;

    EventManager() = default;
    EventManager(const EventManager&) = delete;
    EventManager& operator=(const EventManager&) = delete;
};


