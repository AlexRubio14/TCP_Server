#include "Client.h"
#include "PacketManager.h"
#include "EventManager.h"
#include "PacketType.h"
#include <iostream>

Client::Client(const int _guid)
{
    socket = std::make_unique<sf::TcpSocket>();

    //TODO: Crear el GUID y darle el nickname con una query a la database
    guid = _guid;
}

Client::~Client()
{
    std::cout << "Client Destructor was called" << std::endl;
}


void Client::HandleIncomingPackets()
{
    CustomPacket customPacket;

    if (socket->receive(customPacket.packet) == sf::Socket::Status::Done) {
        PACKET_MANAGER.ProcessPacket(guid, customPacket);
    }
    else if (socket->receive(customPacket.packet) == sf::Socket::Status::Disconnected)
    {
        EVENT_MANAGER.Emit(DISCONNECT, guid, customPacket);
        //std::cout << "Cliente desconectado en la direccion: " << socket->getRemoteAddress().value() << std::endl;
    }
}