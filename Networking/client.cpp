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
    // int choice;
    // std::cout << "Choose your character:\n";
    // std::cout << "0 - thief one\n";
    // std::cout << "1 - thief two\n";
    // std::cout << "2 - thief three\n";
    // std::cout << "3 - security guard\n";
    // std::cin >> choice;

    // // initialize the client
    // std::cout << "Choice passed in\n";
    client = new ClientGame();
    //ClientGame tempClient((CharacterType) choice);
    std::cout << "Client game created\n";
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
    std::cout << "Client loop starting\n";
    while(!glfwWindowShouldClose(client->window))
    {
        //  // Main render display callback. Rendering of objects is done here.
        // Window::displayCallback(client->window);

        // // Idle callback. Updating objects, etc. can be done here.
        // Window::idleCallback();
        client->update();
    }
    std::cout << "Window closed\n";
    Window::cleanUp();
    // Destroy the window.
    glfwDestroyWindow(client->window);
    // Terminate GLFW.
    glfwTerminate();

    exit(EXIT_SUCCESS);
}