#include "ClientManager.h"
#include <iostream>
#include "EventManager.h"
#include "PacketType.h"

ClientManager& ClientManager::Instance()
{
    static ClientManager instance;
    return instance;
}

void ClientManager::Init()
{
    EVENT_MANAGER.Subscribe(REGISTER, [](int guid, CustomPacket& packet) {
    });

    EVENT_MANAGER.Subscribe(DISCONNECT, [this](int guid, CustomPacket& customPacket) {
        DisconnectClient(guid);
    });
}

ClientManager::ClientManager(std::unique_ptr<sf::TcpSocket> clientSocket)
    : socket(std::move(clientSocket)) 
{ 
    socket->setBlocking(false);
}

Client& ClientManager::CreateClient()
{
    //TODO: create GUID
    int guid = guidCount;
    guidCount++;

    std::unique_ptr<Client> client = std::make_unique<Client>(guid);
    clients[guid] = std::move(client);

    return *clients[guid];
}

void ClientManager::DisconnectClient(int guid)
{
    clients.erase(guid);
    std::cout << "Client deleted from unorderedMap" << std::endl;
}

int ClientManager::CreateGuid()
{
    return 0;
}

void ClientManager::UpdateClients(sf::SocketSelector& _socketSelector)
{//Structure Binding improved in c++ v17 and considered best practice in modern c++, auto is compulsory using this feature because the linker needs to deduct the type of the variable
	for (auto& [id, client] : clients) 
	{
        if (client && _socketSelector.isReady(client->GetSocket())) 
        {
            client->HandleIncomingPackets();
        }
        else if (!client)
        {
            std::cerr << "Invalid client pointer detected for client ID: " << id << std::endl;
        }
    }

    std::cout << clients.size() << std::endl;
}

Client* ClientManager::GetClientById(int guid)
{
	std::unordered_map<int, std::unique_ptr<Client>>::iterator it = clients.find(guid);

    if (it != clients.end())
    {
        return it->second.get();
    }
    else
    {
        std::cerr << "Client with GUID " << guid << " not found." << std::endl;
        return nullptr;
    }
}


