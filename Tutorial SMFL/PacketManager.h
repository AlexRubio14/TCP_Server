#pragma once
#include "CustomPacket.h"

class PacketManager
{
private:

	void HandleHandshake(sf::Packet& packet);

	void HandleTest(sf::Packet& packet);

public:

	void ProcessPacket(CustomPacket customPacket);
};

