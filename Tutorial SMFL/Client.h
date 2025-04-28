#pragma once
#include <string>
#include <SFML/Network.hpp>

class Client
{
private:

	int guid;
	std::string nickname;
	std::unique_ptr<sf::TcpSocket> socket;

public:

	Client() = default;
	Client(const int guid);
	~Client();

	void HandleIncomingPackets();

	inline int GetGuid() const  { return guid; }
	inline std::string GetNickname() const { return nickname; } 
	inline sf::TcpSocket& GetSocket() { return *socket; }

};

