#include "CustomPacket.h"

sf::Packet& operator>>(sf::Packet& packet, PacketType& type)
{
	int temp;
	packet >> temp;
	type = static_cast<PacketType>(temp);

	return packet;
}
