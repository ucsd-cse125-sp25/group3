// may need #include "stdafx.h" in visual studio
#include "ServerGame.h"
#include "ClientGame.h"
// used for multi-threading
//#include <process.h>
#include <iostream>
#include <string>

void clientLoop(void);

ClientGame * client;

int main()
{   
    // initialize the client
    client = new ClientGame();
    clientLoop();
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