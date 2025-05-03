#include "RoomManager.h"

RoomManager& RoomManager::Instance()
{
	static RoomManager instance;
	return instance;
}

bool RoomManager::CreateRoom(const std::string& roomId, std::shared_ptr<Client> client)
{
	auto roomIt = FindRoomById(roomId);

	if (roomIt == rooms.end()) // There isn't a room with the id, then we create it
	{
		std::unique_ptr<Room> room = std::make_unique<Room>(roomId);
		room->AddClient(client);
		rooms.push_back(std::move(room));

		return true;
	}

	return false; // There exists a room with this id
}

bool RoomManager::JoinRoom(const std::string& roomId, std::shared_ptr<Client> client)
{
	auto roomIt = FindRoomById(roomId);

	if (roomIt != rooms.end())
	{
		roomIt->get()->AddClient(client);
		std::cout << "Client " << client->GetUsername() << " joined room with id: " << roomId << std::endl;
		return true;
	}
	else
	{
		std::cerr << "Room with ID " << roomId << " not found." << std::endl;
		return false;
	}
}

void RoomManager::LeaveRoom(const std::string& roomId, std::shared_ptr<Client> client)
{
	auto roomIt = FindRoomById(roomId);

	if (roomIt != rooms.end())
	{
		roomIt->get()->RemoveClient(client);
	}
	else
	{
		std::cerr << "Room with ID " << roomId << " not found." << std::endl;
	}
}

void RoomManager::DeleteRoom(const std::string& roomId)
{
	auto roomIt = FindRoomById(roomId);

	if (roomIt != rooms.end())
	{
		rooms.erase(roomIt);
		std::cout << "Room with ID " << roomId << " deleted." << std::endl;
	}
	else
	{
		std::cerr << "Room with ID " << roomId << " not found." << std::endl;
	}
}

std::vector<std::unique_ptr<Room>>::iterator RoomManager::FindRoomById(const std::string& roomId)
{
	auto roomIt = std::find_if(rooms.begin(), rooms.end(),
		[roomId](const std::unique_ptr<Room>& room) {
			return room->GetId() == roomId;
		});

	if (roomIt != rooms.end())
		std::cout << "Room with ID " << roomId << " found." << std::endl;
	else
		std::cerr << "Room with ID " << roomId << " not found." << std::endl;

	return roomIt;
}
