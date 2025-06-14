#include "ServerNetwork.h"
#include "../include/ServerLogic.h"
#define NUM_NPC 3

class ServerGame
{

    friend class Packet;
    friend class KeyPacket;

public:

    ServerGame(void);

    void update();
    void receiveFromClients();
    void sendActionPackets();
    void sendEchoPackets(std::string);
    void sendInitPlayerState(unsigned int client_id);
    void sendInitNPCState(unsigned int client_id);
    void sendStateUpdate();
    void disconnectClient(unsigned int client_id);
    void sendGuiUpdate(unsigned int client_id, bool sendAll);
    void sendInitMinigamePacket(unsigned int client_id);

    std::chrono::time_point<std::chrono::steady_clock> gameStartTime;
    const int GAME_DURATION_SECONDS = 300; // 5 minutes
    // const int GAME_DURATION_SECONDS = 10;// 5 minutes
    std::string getCurrentTimeString() ;


private:

   // IDs for the clients connecting for table in ServerNetwork 
    static unsigned int client_id;

   // The ServerNetwork object 
    ServerNetwork* network;

    // data buffer
    char network_data[MAX_PACKET_SIZE];

    ArtifactState artifact;
    std::map<unsigned int, PlayerData*> playersData;
    std::map<unsigned int, NPCState> npcData;

    bool winAlreadySent = false;

     std::vector<Platform> minigamePlatforms;
};
