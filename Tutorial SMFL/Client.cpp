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

	sf::Socket::Status status = socket->receive(customPacket.packet);

	if (status == sf::Socket::Status::Done) {
		customPacket.packet >> customPacket.type;
		EVENT_MANAGER.Emit(customPacket.type, guid, customPacket);
	}
	else if (status == sf::Socket::Status::Disconnected)
	{
		EVENT_MANAGER.Emit(DISCONNECT, guid, customPacket);
	}
}