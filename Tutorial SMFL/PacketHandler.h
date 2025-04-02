#pragma once
#include "CustomPacket.h"

class PacketHandler
{
private:

public:
	void HandleIncomingPackets();

	void ProcessPacket(std::unique_ptr<CustomPacket> customPacket);
};

