#include "Room.h"
#include "EventManager.h"
#include "RoomManager.h"


Room::Room(const std::string id)
{
	this->id = id;
}

void Room::AddClient(std::shared_ptr<Client> client)
{
	clients.push_back(client);
	std::cout << "Client added to room" << std::endl;
	CheckIfRoomFull();
}

void Room::RemoveClient(std::shared_ptr<Client> client)
{
	auto clientsIt = std::remove(clients.begin(), clients.end(), client);
	if (clientsIt != clients.end())
	{
		clients.erase(clientsIt, clients.end());
		std::cout << "Client: " << client->GetGuid() << " removed from room" << std::endl;
		CheckIfRoomEmpty();
	}
	else
	{
		std::cerr << "Client not found in room" << std::endl;
	}
}

void Room::CheckIfRoomFull()
{
	if (clients.size() >= 4)
	{
		std::cout << "Room is full" << std::endl;
		CustomPacket customPacket;
		EVENT_MANAGER.Emit(START_GAME, "", customPacket);
	}

	std::cout << "The size of the room is: " << clients.size() << std::endl;
}

void Room::CheckIfRoomEmpty()
{
	if (clients.size() == 0)
	{
		std::cout << "Room is empty" << std::endl;
		ROOM_MANAGER.DeleteRoom(id);
	}
}
