#include <vector>
#include "CustomPacket.h"
#include "PacketHandler.h"

class ClientHandler {
private:
    std::unique_ptr<sf::TcpSocket> socket;

    PacketHandler packetHandler;

public:
    explicit ClientHandler(std::unique_ptr<sf::TcpSocket> clientSocket);

    void HandleIncomingPackets();

    inline sf::TcpSocket* getSocket() { return socket.get(); }
};