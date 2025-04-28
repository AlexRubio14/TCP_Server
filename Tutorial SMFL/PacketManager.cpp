#include "PacketManager.h"
#include <iostream>
#include "EventManager.h"
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

PacketManager& PacketManager::Instance()
{
	static PacketManager instance;
	return instance;
}

void PacketManager::Init()
{
	EVENT_MANAGER.Subscribe(HANDSHAKE, [this](int guid, CustomPacket& customPacket) {
		HandleHandshake(customPacket.packet);
		});

	EVENT_MANAGER.Subscribe(TEST, [this](int guid, CustomPacket& customPacket) {
		HandleTest(customPacket.packet);
		});
}

void PacketManager::ProcessPacket(int guid, CustomPacket customPacket)
{
	customPacket.packet >> customPacket.type;

	std::cout << customPacket.type << std::endl;

	EVENT_MANAGER.Emit(customPacket.type, guid, customPacket);
}
