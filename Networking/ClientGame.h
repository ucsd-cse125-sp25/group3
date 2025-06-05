#ifdef _WIN32
    #include <winsock2.h>
    #include <Windows.h>
#endif
#include "ClientNetwork.h"
#include <string>
#include "Window.h"
#include "core.h"
#include "ClientLogic.h"
#include "../packets/PacketFactory.h"

class ClientGame
{

public:
    GLFWwindow* window;
    ClientNetwork* network; 
    MinigameCharacter minigameCharacter;

    ClientGame();
    void sendPendingPackets();
    void sendPacket(Packet& packet);
    void sendInitPacket();
    void sendKeyPacket(KeyType);
    char network_data[MAX_PACKET_SIZE];
    void update();
};