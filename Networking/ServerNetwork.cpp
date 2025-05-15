
#include "ServerNetwork.h"
#include <cstdio>

const char *ip = nullptr;

ServerNetwork::ServerNetwork(void)
{

    // our sockets for the server
    #ifdef _WIN32
        // create WSADATA object
        WSADATA wsaData;
        ListenSocket = INVALID_SOCKET;
        ClientSocket = INVALID_SOCKET;
    #else
        ListenSocket = -1;
        ClientSocket = -1;
    #endif


    // address info for the server to listen to
    struct addrinfo *result = NULL;
    struct addrinfo hints;

    #ifdef _WIN32
        // Initialize Winsock
        iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
        if (iResult != 0) {
            printf("WSAStartup failed with error: %d\n", iResult);
            exit(1);
        }
    #endif

    // set address information
    #ifdef _WIN32
        ZeroMemory(&hints, sizeof(hints));
    #else
        memset(&hints, 0, sizeof(hints));
    #endif
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;    // TCP connection!!!
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    // get ip address of server
    // sockaddr_in* resultIn = (struct sockaddr_in *) &(result->ai_addr);
    // ip = inet_ntoa(resultIn->sin_addr);

    // std::cout << ip << std::endl;

    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        #ifdef _WIN32
            WSACleanup();
        #endif
        exit(1);
    }

    // Create a SOCKET for connecting to server
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

    #ifdef _WIN32
        if (ListenSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            freeaddrinfo(result);
            WSACleanup();
            exit(1);
        }
    #else
        if (ListenSocket < 0) {
            printf("socket failed with error\n");
            freeaddrinfo(result);
            exit(1);
        }
    #endif

    // Set the mode of the socket to be nonblocking
    u_long iMode = 1;

    #ifdef _WIN32
        iResult = ioctlsocket(ListenSocket, FIONBIO, &iMode);
        if (iResult == SOCKET_ERROR) {
            printf("ioctlsocket failed with error: %d\n", WSAGetLastError());
            closesocket(ListenSocket);
            WSACleanup();
            exit(1);
        }
    #else
        iResult = fcntl(ListenSocket, F_SETFL, fcntl(ListenSocket, F_GETFL, 0) | O_NONBLOCK);
        if (iResult < 0) {    
            printf("fcntl failed");
            close(ListenSocket);
            exit(1);
        }
    #endif
        
    // Setup the TCP listening socket
    iResult = ::bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);

    #ifdef _WIN32
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        exit(1);
    }
    #else
    if (iResult < 0) {
        printf("bind failed");
        close(ListenSocket);
        exit(1);
    }
    #endif

    // no longer need address information
    freeaddrinfo(result);

    // start listening for new clients attempting to connect
    iResult = listen(ListenSocket, SOMAXCONN);

    #ifdef _WIN32
    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        exit(1);
    }
    #else
    if (iResult < 0) {
        printf("Listen failed");
        close(ListenSocket);
        exit(1);
    }
    #endif
        
}

// accept new connections
bool ServerNetwork::acceptNewClient(unsigned int & id)
{
    // if client waiting, accept the connection and save the socket
    ClientSocket = accept(ListenSocket,NULL,NULL);
    #ifdef _WIN32
        if (ClientSocket != INVALID_SOCKET) {
            //disable nagle on the client's socket
            char value = 1;
            setsockopt( ClientSocket, IPPROTO_TCP, TCP_NODELAY, &value, sizeof( value ) );
            // insert new client into session id table
            sessions.insert( pair<unsigned int, SOCKET>(id, ClientSocket) );
            return true;
        }
    #else
    if (ClientSocket > 0) {
    
        //disable nagle on the client's socket
        char value = 1;
        setsockopt( ClientSocket, IPPROTO_TCP, TCP_NODELAY, &value, sizeof( value ) );
        // insert new client into session id table
        sessions.insert( pair<unsigned int, int>(id, ClientSocket) );
        return true;
    }
    #endif
    return false;
}

// receive incoming data
int ServerNetwork::receiveData(unsigned int client_id, char * recvbuf, int bufLength)
{
    int totalRead = 0;

    if( sessions.find(client_id) != sessions.end()) {
        #ifdef _WIN32
        SOCKET currentSocket = sessions[client_id];
        #else
        int currentSocket = sessions[client_id];
        #endif

        while (totalRead != bufLength) {
            iResult = NetworkServices::receiveMessage(currentSocket, recvbuf + totalRead, bufLength - totalRead);
            
            if (iResult == 0) {
                printf("Connection closed\n");
                #ifdef _WIN32
                closesocket(currentSocket);
                #else
                close(currentSocket);
                #endif
                return 0;
            } else if (iResult < 0 && totalRead == 0) { //nothing to read
                return -1;
            } else if (iResult > 0) {
                totalRead += iResult;
            }
            
        }
    }

    return totalRead;
} 

// send data to all clients
void ServerNetwork::sendToAll(char * packets, int totalSize)
{
    #ifdef _WIN32
    SOCKET currentSocket;
    std::map<unsigned int, SOCKET>::iterator iter;
    #else
    int currentSocket;
    std::map<unsigned int, int>::iterator iter;
    #endif
    int iSendResult;

    for (iter = sessions.begin(); iter != sessions.end(); iter++)
    {
        currentSocket = iter->second;
        iSendResult = NetworkServices::sendMessage(currentSocket, packets, totalSize);
        
        #ifdef _WIN32
        if (iSendResult == SOCKET_ERROR) {
            printf("send failed with error: %d\n", WSAGetLastError());
            closesocket(currentSocket);
        }
            #else
        if (iSendResult < 0) {
            printf("sendToAll failed");
            close(currentSocket);
        }
        #endif
        
    }
}