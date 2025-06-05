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

    ClientGame();
    GLFWwindow* window;
    ClientNetwork* network; 
    void sendPendingPackets();
    void sendPacket(Packet& packet);
    void sendInitPacket();
    void sendKeyPacket(KeyType);
    void sendMinigameFinishedPacket();
    char network_data[MAX_PACKET_SIZE];
    void update();
};