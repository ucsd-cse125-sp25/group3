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
    int choice;
    std::cout << "Choose your character:\n";
    std::cout << "0 - thief one\n";
    std::cout << "1 - thief two\n";
    std::cout << "2 - thief three\n";
    std::cout << "3 - security guard\n";
    std::cin >> choice;

    // initialize the client
    client = new ClientGame((CharacterType) choice);
    clientLoop();
}

void clientLoop()
{

    //for echo packet testing
/*     while(true) {
        std::string input;
        std::getline(std::cin, input);
        client->update(input);
    } */
    while(!glfwWindowShouldClose(client->window))
    {
        //  // Main render display callback. Rendering of objects is done here.
        // Window::displayCallback(client->window);

        // // Idle callback. Updating objects, etc. can be done here.
        // Window::idleCallback();
        client->update();
    }
    Window::cleanUp();
    // Destroy the window.
    glfwDestroyWindow(client->window);
    // Terminate GLFW.
    glfwTerminate();

    exit(EXIT_SUCCESS);
}