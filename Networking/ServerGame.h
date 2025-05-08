#include "ServerNetwork.h"
#include "../include/ServerLogic.h"

class ServerGame {

    friend class Packet;
    friend class KeyPacket;

public:

    ServerGame(void);
    ~ServerGame(void);

    void update();
    void receiveFromClients();
    void sendActionPackets();
    void sendEchoPackets(std::string);
    void sendPlayerState(unsigned int client_id);

private:

    // IDs for the clients connecting for table in ServerNetwork 
    static unsigned int client_id;

    // The ServerNetwork object 
    ServerNetwork* network;

    // The current server tick
    unsigned int current_tick;

    // data buffer
    char network_data[MAX_PACKET_SIZE];

    std::map<unsigned int, PlayerData> playersData; 
};
