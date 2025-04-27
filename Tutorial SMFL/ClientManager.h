#include <vector>
#include "PacketManager.h"
#include <unordered_map>
#include "Client.h"

#define CLIENT_MANAGER ClientManager::Instance()


class ClientManager {
private:
    std::unique_ptr<sf::TcpSocket> socket;

    std::unordered_map<int,std::unique_ptr<Client>> clients;

    ClientManager() = default;
    ClientManager(const ClientManager&) = delete;
    ClientManager& operator=(const ClientManager&) = delete;

    int guidCount = 1;

public:

    static ClientManager& Instance();

    void Init();

    explicit ClientManager(std::unique_ptr<sf::TcpSocket> clientSocket);

    Client& CreateClient();

    void DisconnectClient(int guid);

    int CreateGuid();

    void UpdateClients(sf::SocketSelector& _socketSelector);

    inline Client& GetClientById(int guid) { return *clients[guid]; }
};