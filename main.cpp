#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
 
#include <stdlib.h>
#include <stdio.h>
#include "Window.h"
#include "Skeleton.h"
#include "core.h"

void error_callback(int error, const char* description) {
    // Print error.
    std::cerr << description << std::endl;
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

void print_versions() {
    // Get info of GPU and supported OpenGL version.
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL version supported: " << glGetString(GL_VERSION)
              << std::endl;

    // If the shading language symbol is defined.
#ifdef GL_SHADING_LANGUAGE_VERSION
    std::cout << "Supported GLSL version is: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
#endif
}

int main(int argc, char *argv[]) {

    // Create the GLFW window.
    GLFWwindow* window = Window::createWindow(800, 600);
    if (!window) exit(EXIT_FAILURE);

    // Parse command line
    if (argc <= 1) {
        std::cout << "Usage: ./menv <.skel file> <.skin file> <.anim file>";
        exit(EXIT_FAILURE);
    }

    // Print OpenGL and GLSL versions.
    print_versions();
    // Setup callbacks.
    setup_callbacks(window);
    // Setup OpenGL settings.
    setup_opengl_settings();

    //skel, skin and anim
    if (argc == 4){
        Window::setRenderFlag(0b111);
        // Initialize the shader program; exit if initialization fails.
        if (!Window::initializeProgram()) exit(EXIT_FAILURE);
        if (!Window::initializeObjects(argv[1], argv[2], argv[3])) exit(EXIT_FAILURE);
    } else if (argc == 3){
        //TODO: skel, anim
        //skel, skin
        Window::setRenderFlag(0b011);
        // Initialize the shader program; exit if initialization fails.
        if (!Window::initializeProgram()) exit(EXIT_FAILURE);
        // Initialize objects/pointers for rendering; exit if initialization fails.
        if (!Window::initializeObjectsSkelSkin(argv[1], argv[2])) exit(EXIT_FAILURE);
    } else { //skel only, skin only
        int j = 0;
        while (argv[1][j] != '\0') {
            j++;
        }
        
        char skelFEx[6] = ".skel";
        char skinFEx[6] = ".skin";
        bool skelTrue = true;
        bool skinTrue = true;

        for(int i = 0; i < 6; i++){
            if (skelFEx[5-i] != argv[1][j-i]){
                skelTrue = false;
            }
            if (skinFEx[5-i] != argv[1][j-i]){
                skinTrue = false;
            }
        }

        if (skelTrue){
            Window::setRenderFlag(0b001);
            // Initialize the shader program; exit if initialization fails.
            if (!Window::initializeProgram()) exit(EXIT_FAILURE);
            // Initialize objects/pointers for rendering; exit if initialization fails.
            if (!Window::initializeObjectsSkelOnly(argv[1])) exit(EXIT_FAILURE);
        } else if (skinTrue) {
            Window::setRenderFlag(0b010);
            // Initialize the shader program; exit if initialization fails.
            if (!Window::initializeProgram()) exit(EXIT_FAILURE);
            // Initialize objects/pointers for rendering; exit if initialization fails.
            if (!Window::initializeObjectsSkinOnly(argv[1])) exit(EXIT_FAILURE);
        } else {
            std::cout << "Usage: ./menv <.skel file> or <.skin file>";
            exit(EXIT_FAILURE);
        }
    }
    // Loop while GLFW window should stay open.
    while (!glfwWindowShouldClose(window)) {
        // Main render display callback. Rendering of objects is done here.
        Window::displayCallback(window);

        // Idle callback. Updating objects, etc. can be done here.
        Window::idleCallback();
    }

    Window::cleanUp();
    // Destroy the window.
    glfwDestroyWindow(window);
    // Terminate GLFW.
    glfwTerminate();

    exit(EXIT_SUCCESS);
}