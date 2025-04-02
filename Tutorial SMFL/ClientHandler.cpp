#include "ClientHandler.h"
#include <iostream>

ClientHandler::ClientHandler(std::unique_ptr<sf::TcpSocket> clientSocket)
    : socket(std::move(clientSocket)) 
{ 
    socket->setBlocking(false);
}

void ClientHandler::HandleIncomingPackets()
{
    std::unique_ptr<CustomPacket> customPacket = std::make_unique<CustomPacket>();

    if (socket->receive(customPacket->packet) == sf::Socket::Status::Done) {
        packetHandler.ProcessPacket(customPacket.get);
    }
    if (socket->receive(customPacket->packet) == sf::Socket::Status::Disconnected)
    {
        std::cout << "Cliente desconectado en la direccion: " << socket->getRemoteAddress().value() << std::endl;
        /*socketSelector.remove(*clients[i]);
        delete clients[i];
        clients.erase(clients.begin() + i);
        i--;*/
    }
}
