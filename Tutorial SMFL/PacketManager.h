#pragma once
#include "CustomPacket.h"

#define PACKET_MANAGER PacketManager::Instance()


class PacketManager
{
private:

	PacketManager() = default;
	PacketManager(const PacketManager&) = delete;
	PacketManager& operator=(const PacketManager&) = delete;

	void HandleHandshake(sf::Packet& packet);

	void HandleTest(sf::Packet& packet);

public:

	static PacketManager& Instance();

	void Init();

	void ProcessPacket(int guid, CustomPacket customPacket);
};

