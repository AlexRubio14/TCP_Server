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

    std::unique_ptr<Client> client = std::make_unique<Client>();
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
{
    for (auto& [id, client] : clients) //Structure Binding implemented in c++ v17
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
}


