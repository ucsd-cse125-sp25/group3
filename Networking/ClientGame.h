#ifdef _WIN32
    #include <winsock2.h>
    #include <Windows.h>
#endif
#include "ClientNetwork.h"
#include <string>
#include "Window.h"
#include "core.h"
#include "ClientLogic.h"
// #include "NetworkData.h"
// #include "packet.h"

class ClientGame
{

public:

    ClientGame();
    ~ClientGame(void);
    GLFWwindow* window;
    ClientNetwork* network; 
    void sendActionPackets();
    void sendEchoPackets(std::string);
    void sendKeyPackets(KeyType);
    char network_data[MAX_PACKET_SIZE];
    void update();
};