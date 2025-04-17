
#include "ClientNetwork.h"

ClientNetwork::ClientNetwork(void) {
    #ifdef _WIN32
        // create WSADATA object
        WSADATA wsaData;

        // socket
        ConnectSocket = INVALID_SOCKET;
    #else
        // socket
        ConnectSocket = -1;
    #endif

    // holds address info for socket to connect to
    struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;

    #ifdef _WIN32
        // Initialize Winsock
        iResult = WSAStartup(MAKEWORD(2,2), &wsaData);

        if (iResult != 0) {
            printf("WSAStartup failed with error: %d\n", iResult);
            exit(1);
        }
        // set address info
        ZeroMemory( &hints, sizeof(hints) );
    #else
        memset(&hints, 0, sizeof(hints));
    #endif

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;  //TCP connection!!!

    iResult = getaddrinfo("127.0.0.1", DEFAULT_PORT, &hints, &result);

    if( iResult != 0 )
    {
        printf("getaddrinfo failed with error: %d\n", iResult);
        #ifdef _WIN32
            WSACleanup();
        #endif
        exit(1);
    }

    // Attempt to connect to an address until one succeeds
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
            ptr->ai_protocol);

        #ifdef _WIN32
            if (ConnectSocket == INVALID_SOCKET) {
                printf("socket failed with error: %ld\n", WSAGetLastError());
                WSACleanup();
                exit(1);
            }
        #else
            if (ConnectSocket < 0) {
                printf("socket failed with error\n");
                freeaddrinfo(ptr);
                freeaddrinfo(result);
                exit(1);
            }
        #endif

        // Connect to server.
        iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);

        #ifdef _WIN32
            if (iResult == SOCKET_ERROR)
            {
                closesocket(ConnectSocket);
                ConnectSocket = INVALID_SOCKET;
                printf ("The server is down... did not connect");
            }
        #else
            if (iResult < 0)
            {
                printf ("The server is down... did not connect");
                freeaddrinfo(result);
                close(ConnectSocket);
                exit(1);
            }
        #endif
    }

    // no longer need address info for server
    freeaddrinfo(result);

    // if connection failed
    #ifdef _WIN32
        if (ConnectSocket == INVALID_SOCKET)
        {
            printf("Unable to connect to server!\n");
            WSACleanup();
            exit(1);
        }
    #else
        if (ConnectSocket < 0)
        {
            printf("Unable to connect to server!\n");
            close(ConnectSocket);
            #ifdef _WIN32
                WSACleanup();
            #endif
            exit(1);
        }
    #endif

    // Set the mode of the socket to be nonblocking
    u_long iMode = 1;

    #ifdef _WIN32
        iResult = ioctlsocket(ConnectSocket, FIONBIO, &iMode);
        if (iResult == SOCKET_ERROR)
        {
            printf("ioctlsocket failed with error: %d\n", WSAGetLastError());
            closesocket(ConnectSocket);
            WSACleanup();
            exit(1);
        }
    #else
        iResult = fcntl(ConnectSocket, F_SETFL, fcntl(ConnectSocket, F_GETFL, 0) | O_NONBLOCK);
        if (iResult < 0)
        {
            printf("socket failed\n");
            close(ConnectSocket);
            #ifdef _WIN32
                WSACleanup();
            #endif
            exit(1);
        }
    #endif

    //disable nagle
    char value = 1;
    setsockopt( ConnectSocket, IPPROTO_TCP, TCP_NODELAY, &value, sizeof( value ) );
}

int ClientNetwork::receivePackets(char * recvbuf) 
{
    iResult = NetworkServices::receiveMessage(ConnectSocket, recvbuf, MAX_PACKET_SIZE);

    if ( iResult == 0 )
    {
        printf("Connection closed\n");
        close(ConnectSocket);
        #ifdef _WIN32
            WSACleanup();
        #endif
        exit(1);
    }

    return iResult;
}