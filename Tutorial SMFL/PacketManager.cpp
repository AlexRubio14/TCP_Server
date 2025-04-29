#include "PacketManager.h"
#include <iostream>
#include "EventManager.h"
#include "DatabaseManager.h"
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

	EVENT_MANAGER.Subscribe(REGISTER, [](int guid, CustomPacket& customPacket) {
		std::string username;
		std::string password;
		customPacket.packet >> username >> password;
		DB_MANAGER.CreateUser(username, password);
		});
}

void PacketManager::ProcessPacket(int guid, CustomPacket customPacket)
{
	customPacket.packet >> customPacket.type;

	EVENT_MANAGER.Emit(customPacket.type, guid, customPacket);
}
