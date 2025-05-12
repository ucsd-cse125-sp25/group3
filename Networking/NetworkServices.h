#pragma once
#ifdef _WIN32
    #include <winsock2.h>
    #include <Windows.h>
#else
    #include <iostream>
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netdb.h>
    #include <arpa/inet.h>
    #include <unistd.h>
#endif

class NetworkServices {
    public:
    #ifdef _WIN32
        static int sendMessage(SOCKET curSocket, char * message, int messageSize);
        static int receiveMessage(SOCKET curSocket, char * buffer, int bufSize);
    #else
        static int sendMessage(int curSocket, char * message, int messageSize);
        static int receiveMessage(int curSocket, char * buffer, int bufSize);
    #endif
};
