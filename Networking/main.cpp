// may need #include "stdafx.h" in visual studio
#include "ServerGame.h"
#include "ClientGame.h"
// used for multi-threading
#include <process.h>
#include <iostream>
#include <string>

void serverLoop(void *);
void clientLoop(void);

ServerGame * server;
ClientGame * client;

int main()
{

   // initialize the server
    server = new ServerGame();
   
    // create thread with arbitrary argument for the run function
    _beginthread( serverLoop, 0, (void*)12);

    // initialize the client
    client = new ClientGame();
    clientLoop();
}

void serverLoop(void * arg) 
{ 
    while(true) 
    {
        server->update();
    }
}

void clientLoop()
{
    while(true)
    {
        //do game stuff
        std::string input;
        std::getline(std::cin, input);
        client->update(input);
    }
}