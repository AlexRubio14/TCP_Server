#include "PacketManager.h"
#include <iostream>
#include "EventManager.h"
#include "DatabaseManager.h"
#include "ClientManager.h"
#include "RoomManager.h"
void PacketManager::HandleHandshake(sf::Packet& packet)
{
	std::string messageFromClient;
	packet >> messageFromClient;
	
	std::cout << "Messages received from client: " << messageFromClient << std::endl;
}

void PacketManager::SendHandshake(const std::string guid)
{
	std::string responseMessage = "Hello client, i'm the server";
	CustomPacket responsePacket(HANDSHAKE);

	responsePacket.packet << responseMessage;

	std::shared_ptr<Client> client = CLIENT_MANAGER.GetPendingClientById(guid);
	if (client != nullptr)
	{
		SendPacketToClient(client, responsePacket);
		std::cout << "Message send to client: " << responseMessage << std::endl;
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

	EVENT_MANAGER.Subscribe(REGISTER, [](std::string guid, CustomPacket& customPacket) {
		std::string username;
		std::string password;
		customPacket.packet >> username >> password;

		std::cout << std::endl << "Username is: " << username << " and the password is: " << password;

		std::string message;
		CustomPacket responsePacket;

		switch (DB_MANAGER.CreateUser(username, password))
		{
		case RegisterResult::SUCCESS: {
			message = "User registered successfully";
			responsePacket.packet << REGISTER_SUCCES << message;


			std::string authGuid = CLIENT_MANAGER.PromoteClientToAuthenticated(guid, username);
			EVENT_MANAGER.Emit(REGISTER_SUCCES, authGuid, responsePacket);
			std::cout << "User " << username << " registered successfully nad promoted to authenticated" << std::endl;
			break;
		}
		case RegisterResult::USERNAME_TAKEN:

			message = "Username already taken";
			responsePacket.packet << REGISTER_ERROR << message;

			EVENT_MANAGER.Emit(REGISTER_ERROR, guid, responsePacket); 
			std::cout << "User " << username << " failed to register because: " << message << std::endl;
			break;
		case RegisterResult::QUERY_ERROR:

			message = "Error querying the database";
			responsePacket.packet << REGISTER_ERROR << message;

			EVENT_MANAGER.Emit(REGISTER_ERROR, guid, responsePacket);
			std::cout << "User " << username << " failed to register because: " << message << std::endl;
			break;
		case RegisterResult::INSERT_FAILED:

			message = "Error inserting the user into the database";
			responsePacket.packet << REGISTER_ERROR << message;

			EVENT_MANAGER.Emit(REGISTER_ERROR, guid, responsePacket);
			std::cout << "User " << username << " failed to register because: " << message << std::endl;
			break;
		default:
			break;
		}
	});

	EVENT_MANAGER.Subscribe(REGISTER_ERROR, [this](std::string guid, CustomPacket& customPacket) {

		std::shared_ptr<Client> client = CLIENT_MANAGER.GetPendingClientById(guid);
		
		if(client != nullptr)
			SendPacketToClient(client, customPacket);
	});

	EVENT_MANAGER.Subscribe(REGISTER_SUCCES, [this](std::string guid, CustomPacket& customPacket) {
		
		std::shared_ptr<Client> client = CLIENT_MANAGER.GetAuthoritedClientById(guid);
		
		if (client != nullptr)
			SendPacketToClient(client, customPacket);
	});

	EVENT_MANAGER.Subscribe(LOGIN, [this](std::string guid, CustomPacket& customPacket) {

		std::string username;
		std::string password;
		customPacket.packet >> username >> password;

		std::string message;
		CustomPacket responsePacket;

		switch (DB_MANAGER.ValidateUser(username, password))
		{
		case LoginResult::SUCCESS: {

			message = "User logged in successfully";
			responsePacket.packet << LOGIN_SUCCESS << message;

			std::string authGuid = CLIENT_MANAGER.PromoteClientToAuthenticated(guid, username);
			EVENT_MANAGER.Emit(LOGIN_SUCCESS, authGuid, responsePacket);
			std::cout << "User " << username << " logged in successfully and promoted to Authenticated" << std::endl;
			break;
		}
		case LoginResult::INVALID_CREDENTIALS:

			message = "Credentials used for login are invalid";
			responsePacket.packet << LOGIN_ERROR << message;

			EVENT_MANAGER.Emit(LOGIN_ERROR, guid, responsePacket);
			std::cout << "User " << username << " failed to log in because: " << message << std::endl;

			break;
		case LoginResult::QUERY_ERROR:

			message = "Error querying the database";
			responsePacket.packet << LOGIN_ERROR << message;

			EVENT_MANAGER.Emit(LOGIN_ERROR, guid, responsePacket);
			std::cout << message << std::endl;
			break;
		default:
			break;
		}
	});

	EVENT_MANAGER.Subscribe(LOGIN_ERROR, [this](std::string guid, CustomPacket& customPacket) {
		
		std::shared_ptr<Client> client = CLIENT_MANAGER.GetPendingClientById(guid);
		
		if (client != nullptr)
			SendPacketToClient(client, customPacket);
	});

	EVENT_MANAGER.Subscribe(LOGIN_SUCCESS, [this](std::string guid, CustomPacket& customPacket) {
		
		std::shared_ptr<Client> client = CLIENT_MANAGER.GetAuthoritedClientById(guid);
		
		if (client != nullptr)
			SendPacketToClient(client, customPacket);
	});

	EVENT_MANAGER.Subscribe(CREATE_ROOM, [](std::string guid, CustomPacket& customPacket) {
		std::string roomId;
		customPacket.packet >> roomId;
		std::cout << "Creating room with id: " << roomId << std::endl;
		std::shared_ptr<Client> client = CLIENT_MANAGER.GetAuthoritedClientById(guid);
		if (client && ROOM_MANAGER.CreateRoom(roomId, client))
		{
			std::string responseMessage = "Room created successfully";
			CustomPacket responsePacket(CREATE_ROOM_SUCCES);
			responsePacket.packet << responseMessage;
			EVENT_MANAGER.Emit(CREATE_ROOM_SUCCES, guid, responsePacket);
		}
		else
		{
			std::string responseMessage = "A room with the id you entered already exists";
			CustomPacket responsePacket(CREATE_ROOM_ERROR);
			responsePacket.packet << responseMessage;
			EVENT_MANAGER.Emit(CREATE_ROOM_ERROR, guid, responsePacket);
		}
		});

	EVENT_MANAGER.Subscribe(CREATE_ROOM_ERROR, [this](std::string guid, CustomPacket& customPacket) {

		std::shared_ptr<Client> client = CLIENT_MANAGER.GetAuthoritedClientById(guid);

		if (client != nullptr)
			SendPacketToClient(client, customPacket);
		std::cout << "Error creating room: " << std::endl;
		});

	EVENT_MANAGER.Subscribe(CREATE_ROOM_SUCCES, [this](std::string guid, CustomPacket& customPacket) {
		std::shared_ptr<Client> client = CLIENT_MANAGER.GetAuthoritedClientById(guid);

		if (client != nullptr)
			SendPacketToClient(client, customPacket);
		std::cout << "Room created succesfully" << std::endl;
		});

	EVENT_MANAGER.Subscribe(JOIN_ROOM, [](std::string guid, CustomPacket& customPacket) {
		std::string roomId;
		customPacket.packet >> roomId;
		std::shared_ptr<Client> client = CLIENT_MANAGER.GetAuthoritedClientById(guid);
		std::cout << client.get()->GetGuid() << " is trying to join room with id: " << roomId << std::endl;
		if (client != nullptr && ROOM_MANAGER.JoinRoom(roomId, client))
		{
			CustomPacket responsePacket(JOIN_ROOM_SUCCES);
			std::string responseMessage = "The user joined the room successfully";
			responsePacket.packet << responseMessage;
			EVENT_MANAGER.Emit(JOIN_ROOM_SUCCES, guid, responsePacket);
			client->SetCurrentRoomId(roomId);
		}
		else
		{
			CustomPacket responsePacket(JOIN_ROOM_ERROR);
			std::string responseMessage = "The user can't join to the room, try with another id";
			responsePacket.packet << responseMessage;
			EVENT_MANAGER.Emit(JOIN_ROOM_SUCCES, guid, responsePacket);
		}
		});

	EVENT_MANAGER.Subscribe(JOIN_ROOM_ERROR, [this](std::string guid, CustomPacket& customPacket) {
		std::shared_ptr<Client> client = CLIENT_MANAGER.GetAuthoritedClientById(guid);

		if (client != nullptr)
			SendPacketToClient(client, customPacket);
		});

	EVENT_MANAGER.Subscribe(JOIN_ROOM_SUCCES, [this](std::string guid, CustomPacket& customPacket) {
		std::shared_ptr<Client> client = CLIENT_MANAGER.GetAuthoritedClientById(guid);

		if (client != nullptr)
			SendPacketToClient(client, customPacket);
		});


	EVENT_MANAGER.Subscribe(START_GAME, [this](std::string guid, CustomPacket& customPacket) {
		std::cout << "Starting game" << std::endl;
		//TODO: start game logic
		std::shared_ptr<Client> client = CLIENT_MANAGER.GetAuthoritedClientById(guid);

		Room* room = ROOM_MANAGER.GetFullRoom();

		int clientCount = room->GetClients().size();

		std::string ip, username;
		int index;

		CustomPacket responsePacket(START_GAME);

		int clientIndex;

		for (int i = 0; i < clientCount; i++) // Write every client data in the room
		{
			ip = room->GetClients()[i]->GetSocket().getRemoteAddress().value().toString();
			username = room->GetClients()[i]->GetUsername();
			index = i;
			responsePacket.packet << ip << username << index;

			if (client->GetGuid() == room->GetClients()[i]->GetGuid())
				clientIndex = i;
		}

		// Write the client data to know who you are

		ip = client->GetSocket().getRemoteAddress().value().toString();
		username = client->GetUsername();
		responsePacket.packet << ip << username << clientIndex;

		SendPacketToClient(client, responsePacket);

		});
}

void PacketManager::ProcessPacket(std::string guid, CustomPacket customPacket)   
{
	customPacket.packet >> customPacket.type;

	std::cout << "Processing packet of type: " << customPacket.type << std::endl;

	EVENT_MANAGER.Emit(customPacket.type, guid, customPacket);
}

void PacketManager::SendPacketToClient(const std::shared_ptr<Client> client, CustomPacket& responsePacket)
{
	if (client->GetSocket().send(responsePacket.packet) == sf::Socket::Status::Done)
		std::cout << "Message sent to client " << std::endl;
	else
		std::cerr << "Error sending the message to client" << std::endl;
}
