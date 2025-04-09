#include "PacketManager.h"
#include <iostream>

void PacketManager::HandleHandshake(sf::Packet& packet)
{
	std::string message;
	packet >> message;
	
	std::cout << "Mensaje recibido del cliente: " << message << std::endl;
}

void PacketManager::HandleTest(sf::Packet& packet)
{
	std::string message;
	packet >> message;
	
	std::cout << "Mensaje recibido del cliente: " << message << std::endl;
}

void PacketManager::ProcessPacket(CustomPacket customPacket)
{
	customPacket.packet >> customPacket.type;

	switch (customPacket.type)
	{
	case HANDSHAKE:
		HandleHandshake(customPacket.packet);
		break;
	case LOGIN:
		break;
	case REGISTER:
		break;
	case MOVIMIENTO:
		break;
	case TEST:
		HandleTest(customPacket.packet);
		break;
	case WAIT:
		break;
	default:
		break;
	}

	customPacket.packet.clear();
}
