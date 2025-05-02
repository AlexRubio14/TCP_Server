#include "PacketManager.h"
#include <iostream>
#include "EventManager.h"
#include "DatabaseManager.h"
#include "ClientManager.h"
void PacketManager::HandleHandshake(sf::Packet& packet)
{
	std::string messageFromClient;
	packet >> messageFromClient;
	
	std::cout << "Mensaje recibido del cliente: " << messageFromClient << std::endl;
}

void PacketManager::HandleTest(sf::Packet& packet)
{
	std::string message;
	packet >> message;
	
	std::cout << "Mensaje recibido del cliente: " << message << std::endl;
}

void PacketManager::SendHandshake(const std::string guid)
{
	std::string responseMessage = "Hola cliente, soy el servidor";
	CustomPacket responsePacket(HANDSHAKE);

	responsePacket.packet << responsePacket.type << responseMessage;

	std::shared_ptr<Client> client = CLIENT_MANAGER.GetPendingClientById(guid);
	if (client != nullptr)
	{
		if (client->GetSocket().send(responsePacket.packet) == sf::Socket::Status::Done)
			std::cout << "Mensaje enviado al cliente: " << responseMessage << std::endl;
		else
			std::cerr << "Error al enviar el mensaje al cliente." << std::endl;
	}
}

PacketManager& PacketManager::Instance()
{
	static PacketManager instance;
	return instance;
}

void PacketManager::Init()
{
	EVENT_MANAGER.Subscribe(HANDSHAKE, [this](std::string guid, CustomPacket& customPacket) {
		HandleHandshake(customPacket.packet);
		SendHandshake(guid);
		});

	EVENT_MANAGER.Subscribe(TEST, [this](std::string guid, CustomPacket& customPacket) {
		HandleTest(customPacket.packet);
		});

	EVENT_MANAGER.Subscribe(REGISTER, [](std::string guid, CustomPacket& customPacket) {
		std::string username;
		std::string password;
		customPacket.packet >> username >> password;

		if (DB_MANAGER.CreateUser(username, password))
		{
			CLIENT_MANAGER.PromoteClientToAuthenticated(guid, username);
			
			//TODO: send a meesage to client register_Succesful
		}
		});

	//TODO: send a meesage to client register_error

}

void PacketManager::ProcessPacket(std::string guid, CustomPacket customPacket)
{
	customPacket.packet >> customPacket.type;

	EVENT_MANAGER.Emit(customPacket.type, guid, customPacket);
}
