#pragma once
#include <SFML/Network.hpp>
#include <iostream>
#include <memory>
#include "ClientHandler.h"

#define LISTENER_PORT 55001

class Server {
private:
    sf::TcpListener listener;
    sf::SocketSelector socketSelector;
    std::vector<std::unique_ptr<ClientHandler>> clients;

    bool isRunning;

public:

    Server();

    ~Server();

    void Start();

    void Update();
};

