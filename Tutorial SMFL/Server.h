#pragma once
#include <SFML/Network.hpp>
#include <iostream>
#include <memory>
#include "ClientManager.h"

#define LISTENER_PORT 55000

class Server {
private:
    sf::TcpListener listener;
    sf::SocketSelector socketSelector;
    std::vector<std::unique_ptr<ClientManager>> clients;

    bool isRunning;

public:

    Server();

    ~Server();

    void Start();

    void Update();
};

