#include <SFML/Network.hpp>
#include <iostream>
#include <string>

#define LISTENER_PORT 55000


void Test(sf::Packet& data)
{
	std::string message;
	data >> message;

	std::cout << "Mensaje recibido del cliente: " << message << std::endl;
}



void HandShake(sf::Packet& data)
{
	std::string message;
	data >> message;

	std::cout << "Mensaje recibido del cliente: " << message << std::endl;
}

// ----------------------------------- Server -----------------------------------
void main()
{

	sf::TcpListener listener;
	sf::SocketSelector socketSelector;

	sf::TcpSocket client;

	std::vector<sf::TcpSocket*> clients;
	sf::TcpSocket* newClient;

	bool closeServer = false;

	if (listener.listen(LISTENER_PORT) != sf::Socket::Status::Done) // TODO: Manejar escuchar nuevos puertos
	{
		std::cerr << "No puedo escuchar el puerto!" << std::endl;
		closeServer = true;
	}

	socketSelector.add(listener);

	while (!closeServer)
	{
		if (socketSelector.wait())
		{
			if (socketSelector.isReady(listener)) // Bloquea el hilo hasta que haya algo nuevo
			{
				newClient = new sf::TcpSocket();
				if (listener.accept(*newClient) == sf::Socket::Status::Done)
				{
					newClient->setBlocking(false);
					socketSelector.add(*newClient);
					clients.push_back(newClient);
					std::cout << clients.size() << std::endl;
					std::cout << "Nueva Conexion Establecida" << std::endl;
				}
			}
			else
			{
				for (int i = 0; i < clients.size(); i++)
				{
					if (socketSelector.isReady(*clients[i]))
					{
						sf::Packet packet;
						if (clients[i]->receive(packet) == sf::Socket::Status::Done)
						{
							//std::cout << "Input recibido de la direccion: " << client.getRemoteAddress().value() << std::endl;

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


							packet.clear();
						}
						if (clients[i]->receive(packet) == sf::Socket::Status::Disconnected)
						{
							std::cout << "Cliente desconectado en la direccion: " << clients[i]->getRemoteAddress().value() << std::endl;
							socketSelector.remove(*clients[i]);
							delete clients[i];
							clients.erase(clients.begin() + i);
							i--;
						}
					}
				}
			}
		}


		/*if (listener.accept(client) == sf::Socket::Status::Done)
		{
			

			sf::Packet packet;
			std::string message = "Hola cliente";

			packet << TipoPaquete::HANDSHAKE <<  message;

			if (client.send(packet) == sf::Socket::Status::Done)
			{
				std::cout << "Mensaje enviado: " << message << std::endl;
				packet.clear();
			}
			else
			{
				std::cerr << "Error al enviar el mensaje al cliente" << std::endl;
			}
		}
		else
		{
			std::cout << "Se jodio el intento de conexion" << std::endl;
		}*/
	}
}

