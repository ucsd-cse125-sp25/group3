// may need #include "stdafx.h" in visual studio
#include "ClientGame.h"
// used for multi-threading
//#include <process.h>
#include <iostream>
#include <string>
// #include "Window.h"
// #include "core.h"
#include "ClientLogic.h"

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
    // while(true) 
    while(!glfwWindowShouldClose(client->window))
    {
        //  // Main render display callback. Rendering of objects is done here.
        // Window::displayCallback(client->window);

        // // Idle callback. Updating objects, etc. can be done here.
        // Window::idleCallback();

        // do game stuff
        // std::string input;
        // std::getline(std::cin, input);
        client->update();
    }
    Window::cleanUp();
    // Destroy the window.
    glfwDestroyWindow(client->window);
    // Terminate GLFW.
    glfwTerminate();

    exit(EXIT_SUCCESS);
}