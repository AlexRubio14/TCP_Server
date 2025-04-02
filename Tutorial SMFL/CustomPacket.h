#pragma once
#include <SFML/Network.hpp>
#include "PacketType.h"

class CustomPacket
{
public:

	sf::Packet packet;
	PacketType type;
};

sf::Packet& operator >>(sf::Packet& packet, PacketType& type);
