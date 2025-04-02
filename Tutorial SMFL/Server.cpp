#include "Server.h"

Server::Server()
{
    isRunning = true;
}

void Server::Start()
{
    if (listener.listen(LISTENER_PORT) != sf::Socket::Status::Done) // TODO: Manejar escuchar nuevos puertos
    {
        std::cerr << "No puedo escuchar el puerto!" << std::endl;
        isRunning = false; 
        return;
    }

    socketSelector.add(listener);
    std::cout << "Servidor iniciado en el puerto " << LISTENER_PORT << std::endl;
}

void Server::Update()
{
    while (isRunning) 
    {
        if (socketSelector.wait()) 
        {
            if (socketSelector.isReady(listener)) 
            {
				std::unique_ptr<sf::TcpSocket> newClient = std::make_unique<sf::TcpSocket>();

                if (listener.accept(*newClient) == sf::Socket::Status::Done) 
                {
                    newClient->setBlocking(false);
                    socketSelector.add(*newClient);
                    clients.emplace_back(std::make_unique<ClientHandler>(std::move(newClient)));
                    std::cout << "Nuevo cliente conectado." << std::endl;
                }
            }
            else 
            {
                for (const std::unique_ptr<ClientHandler>& client : clients) 
                {
                    if (socketSelector.isReady(*client->GetSocket())) 
                        client->HandleIncomingPackets();
                }
            }
        }
    }
}

Server::~Server()
{
}
