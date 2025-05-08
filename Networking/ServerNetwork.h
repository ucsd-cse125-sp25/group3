
#ifdef _WIN32
    #include <winsock2.h>
    #include <Windows.h>
    #include <ws2tcpip.h>
    #pragma comment (lib, "Ws2_32.lib")
#else
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netdb.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <netinet/tcp.h>
#endif
#include "NetworkServices.h"
#include "../packets/PacketFactory.h"
#include <map>
using namespace std; 
#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "2350" 
#include <iostream>

class ServerNetwork
{

public:

    ServerNetwork(void);
    ~ServerNetwork(void);

    // Socket to listen for new connections
    // Socket to give to the clients
    // table to keep track of each client's socket
    
    #ifdef _WIN32
        SOCKET ListenSocket;
        SOCKET ClientSocket;
        std::map<unsigned int, SOCKET> sessions; 
    #else
        int ListenSocket;
        int ClientSocket;
        std::map<unsigned int, int> sessions; 
    #endif

    // for error checking return values
    int iResult;

    // accept new connections
    bool acceptNewClient(unsigned int & id);

    // receive incoming data
    int receiveData(unsigned int client_id, char * recvbuf);

    // send data to all clients
    void sendToAll(char * packets, int totalSize);
};
