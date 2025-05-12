// may need #include "stdafx.h" in visual studio
#include "ServerGame.h"
// used for multi-threading
// #include <process.h>
#include <iostream>
#include <string>

void serverLoop(void *);

ServerGame * server;

int main()
{
   // initialize the server
    server = new ServerGame();
    serverLoop(NULL);
}

void serverLoop(void * arg) 
{ 
    while(true) 
    {
        server->update();
    }
}