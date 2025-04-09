#include <vector>
#include "PacketManager.h"

class ClientManager {
private:
    std::unique_ptr<sf::TcpSocket> socket;

    PacketManager packetHandler;

public:
    explicit ClientManager(std::unique_ptr<sf::TcpSocket> clientSocket);

    void HandleIncomingPackets();

    inline sf::TcpSocket* GetSocketAdress() { return socket.get(); }
};