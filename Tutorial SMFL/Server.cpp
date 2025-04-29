#include "Server.h"
#include "Client.h"
#include "EventManager.h"
#include "DatabaseManager.h"

Server::Server()
{
    isRunning = true;
}

void Server::Start()
{
    if (listener.listen(LISTENER_PORT) == sf::Socket::Status::Done)
    {
        std::cout << "Server launched at port: " << LISTENER_PORT << std::endl;

        socketSelector.add(listener);

        DB_MANAGER.ConnectDb();

        EVENT_MANAGER.Subscribe(DISCONNECT, [this](int guid, CustomPacket& customPacket) {
            Client* client = CLIENT_MANAGER.GetClientById(guid);
            if (client != nullptr)
            {
                socketSelector.remove(client->GetSocket());
                std::cout << "socket erased from socketSelector" << std::endl;
            }
            else
            {
                std::cerr << "Trying to disconnect non-existing client ( guid = " << guid << ")" << std::endl;
            }
        });

        CLIENT_MANAGER.Init();
		PACKET_MANAGER.Init();
    }
    else
    {
        isRunning = false;
        std::cerr << "I can't listen to the port: " << LISTENER_PORT << std::endl;
        return;
    }
}

void Server::Update()
{
    while (isRunning) 
    {
        if (socketSelector.wait()) 
        {
            if (socketSelector.isReady(listener)) 
            {
                HandleNewConnection();
            }
            else 
            {
                CLIENT_MANAGER.UpdateClients(socketSelector);
            }
        }
    }

    DB_MANAGER.DisconnectDb();
}

void Server::HandleNewConnection()
{
    Client& newClient = CLIENT_MANAGER.CreateClient();

    if (listener.accept(newClient.GetSocket()) == sf::Socket::Status::Done)
    {
        newClient.GetSocket().setBlocking(false);
        socketSelector.add(newClient.GetSocket());
        std::cout << "New client connected" << std::endl;
    }
}

Server::~Server()
{
}
