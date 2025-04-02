#include "PacketHandler.h"

void PacketHandler::ProcessPacket(std::unique_ptr<CustomPacket> customPacket)
{
	std::string receivedMessage;

	TipoPaquete type;
	packet >> type;

	switch (type)
	{
	case HANDSHAKE:
		HandShake(packet);
		break;
	case LOGIN:
		break;
	case REGISTER:
		break;
	case MOVIMIENTO:
		break;
	case TEST:
		Test(packet);
		break;
	case WAIT:
		break;
	default:
		break;
	}

	customPacket.packet.clear();
}
