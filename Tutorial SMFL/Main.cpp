#include <SFML/Network.hpp>
#include "Server.h"

// ----------------------------------- Server -----------------------------------

void main()
{
	Server server;
	server.start();
	server.Update();
}

