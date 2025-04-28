#include "Server.h"
#include "Client.h"
#include "EventManager.h"

Server::Server()
{
    isRunning = true;
}

void Server::Start()
{
    if (listener.listen(LISTENER_PORT) == sf::Socket::Status::Done)
    {
        socketSelector.add(listener);

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

        std::cout << "Servidor iniciado en el puerto " << LISTENER_PORT << std::endl;
    }
    else
    {
        isRunning = false;
        std::cerr << "No puedo escuchar el puerto!" << std::endl;
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
}

void Server::HandleNewConnection()
{
    Client& newClient = CLIENT_MANAGER.CreateClient();

    if (listener.accept(newClient.GetSocket()) == sf::Socket::Status::Done)
    {
        newClient.GetSocket().setBlocking(false);
        socketSelector.add(newClient.GetSocket());
        std::cout << "Nuevo cliente conectado." << std::endl;
    }
}

Server::~Server()
{
}
