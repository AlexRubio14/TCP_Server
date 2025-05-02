#include "Client.h"
#include "PacketManager.h"
#include "EventManager.h"
#include "PacketType.h"
#include <iostream>

Client::Client()
{
    socket = std::make_unique<sf::TcpSocket>();
    clientState = NONE;
    guid = "0";
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
    }
}