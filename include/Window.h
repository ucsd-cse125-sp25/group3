#pragma once

#include "Camera.h"
// #include "Cube.h"
#include "Skeleton.h"
#include "Skin.h"
#include "Animation.h"
#include "Player.h"
#include "Shader.h"
#include "core.h"

class Window {
public:
    // Window Properties
    static int width;
    static int height;
    static const char* windowTitle;

    // Objects to render
    static Skeleton* skel;
    static Skin* skin;
    static Animation* anim;
    static Player* player;
    //static Cube* cube;

    static unsigned int fileRender;


    // Shader Program
    static GLuint shaderProgram;

    // Act as Constructors and desctructors
    static bool initializeProgram();
    static bool initializeObjects(const char *skelfile, const char *skinfile, const char *animfile);
    static bool initializeObjectsSkelSkin(const char *skelfile, const char *skinfile);
    static bool initializeObjectsSkelOnly(const char *file);
    static bool initializeObjectsSkinOnly(const char *file);
    static void cleanUp();

    static void Window::setRenderFlag(unsigned int flag);

    // for the Window
    static GLFWwindow* createWindow(int width, int height);
    static void resizeCallback(GLFWwindow* window, int width, int height);

    // update and draw functions
    static void idleCallback();
    static void displayCallback(GLFWwindow*);

    // helper to reset the camera
    static void resetCamera();

    // callbacks - for interaction
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouse_callback(GLFWwindow* window, int button, int action, int mods);
    static void cursor_callback(GLFWwindow* window, double currX, double currY);
};