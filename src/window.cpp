#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "window.h"
#include <iostream>

Window::Window(int width, int height, const char* title)
    : width(width), height(height) {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(-1);
    }
    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        exit(-1);
    }

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, height, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

Window::~Window() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

GLFWwindow* Window::getGLFWwindow() const {
    return window;
}

void Window::run() {
    // Empty placeholder
}
