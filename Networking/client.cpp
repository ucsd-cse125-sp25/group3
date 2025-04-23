// may need #include "stdafx.h" in visual studio
#include "ClientGame.h"
// used for multi-threading
//#include <process.h>
#include <iostream>
#include <string>
#include "Window.h"
#include "core.h"

void clientLoop(void);

ClientGame * client;

GLFWwindow* window;


void error_callback(int error, const char* description) {
    // Print error.
    std::cerr << description << std::endl;
}

void setup_opengl_settings() {
    // Enable depth buffering.
    glEnable(GL_DEPTH_TEST);
    // Related to shaders and z value comparisons for the depth buffer.
    glDepthFunc(GL_LEQUAL);
    // Set polygon drawing mode to fill front and back of each polygon.
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // Set clear color to black.
    glClearColor(0.0, 0.0, 0.0, 0.0);
}

void setup_callbacks(GLFWwindow* window) {
    // Set the error callback.
    glfwSetErrorCallback(error_callback);
    // Set the window resize callback.
    glfwSetWindowSizeCallback(window, Window::resizeCallback);

    // Set the key callback.
    glfwSetKeyCallback(window, Window::keyCallback);

    // Set the mouse and cursor callbacks
    glfwSetMouseButtonCallback(window, Window::mouse_callback);
    glfwSetCursorPosCallback(window, Window::cursor_callback);
}

int main()
{   
    // window = Window::createWindow(800, 600);
    // if (!window) exit(EXIT_FAILURE);

    // setup_callbacks(window);
    // // Setup OpenGL settings.
    // setup_opengl_settings();

    // // Initialize the shader program; exit if initialization fails.
    // if (!Window::initializeProgram()) exit(EXIT_FAILURE);
    // // Initialize objects/pointers for rendering; exit if initialization fails.
    // if (!Window::initializeObjects()) exit(EXIT_FAILURE);

    // initialize the client
    client = new ClientGame();
    clientLoop();


}

void clientLoop()
{
    while(true) 
    // while(!glfwWindowShouldClose(window))
    {
        //  // Main render display callback. Rendering of objects is done here.
        // Window::displayCallback(window);

        // // Idle callback. Updating objects, etc. can be done here.
        // Window::idleCallback();

        do game stuff
        std::string input;
        std::getline(std::cin, input);
        client->update(input);
    }
    // Window::cleanUp();
    // // Destroy the window.
    // glfwDestroyWindow(window);
    // // Terminate GLFW.
    // glfwTerminate();

    // exit(EXIT_SUCCESS);
}