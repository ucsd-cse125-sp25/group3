#ifdef _WIN32
    #include <winsock2.h>
    #include <Windows.h>
#endif
#include "ClientNetwork.h"
#include <string>

class ClientGame
{

public:

    ClientGame();
    ~ClientGame(void);

    ClientNetwork* network; 
    void sendActionPackets();
    void sendEchoPackets(std::string);
    char network_data[MAX_PACKET_SIZE];
    void update(std::string);
};