#pragma once
#include <vector>
#include "Client.h"
#include <memory>
#include <iostream>

class Room
{
private:
	std::string id;
	std::vector<std::shared_ptr<Client>> clients;	
public:

	Room() = default;
	Room(const std::string id);

	void AddClient(std::shared_ptr<Client> client);
	void RemoveClient(std::shared_ptr<Client> client);

	void CheckIfRoomFull(std::shared_ptr<Client> client);
	void CheckIfRoomEmpty();

	inline std::string GetId() const { return id; }
	inline void SetId(const std::string id) { this->id = id; }
	inline std::vector<std::shared_ptr<Client>> GetClients() const { return clients; }

};