#include "ServerNetwork.h"
#include "ServerLogic.h"

class ServerGame
{

public:

    ServerGame(void);
    ~ServerGame(void);

    void update();
    void receiveFromClients();
    void sendActionPackets();
    void sendEchoPackets(std::string);

private:

   // IDs for the clients connecting for table in ServerNetwork 
    static unsigned int client_id;

   // The ServerNetwork object 
    ServerNetwork* network;

    // data buffer
    char network_data[MAX_PACKET_SIZE];

    std::map<unsigned int, PlayerData> playersData; 
};
