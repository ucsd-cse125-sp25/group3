#pragma once
// #include <winsock2.h>
// #include <Windows.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

class NetworkServices {
    public:
        // static int sendMessage(SOCKET curSocket, char * message, int messageSize);
        // static int receiveMessage(SOCKET curSocket, char * buffer, int bufSize);
        static int sendMessage(int curSocket, char * message, int messageSize);
        static int receiveMessage(int curSocket, char * buffer, int bufSize);
};
