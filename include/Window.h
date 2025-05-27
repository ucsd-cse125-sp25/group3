#pragma once

#include "Camera.h"
#include "Cube.h"
#include "Scene.h"
#include "Model.h"
#include "Shader.h"
#include "core.h"
#include "Character.h"

#include "Skeleton.h"
#include "Animation.h"
#include "AnimationPlayer.h"

#include "ShaderManager.h"
#include "TextureManager.h"
#include "ModelManager.h"

class Window {
public:
    // Window Properties
    static int width;
    static int height;
    static const char* windowTitle;

    // Objects to render
    static Cube* cube;
    static Cube* floor;
    static Character* character;

    static Scene* scene;
    // static Model* model;

    static AnimationPlayer* animationPlayer;

    // Shader Program
    // static GLuint shaderProgram;
    // static GLuint shaderProgram_uv;
    // static GLuint shaderProgram_anim;
    static ShaderManager* shaderManager;
    static ModelManager* modelManager;
    static TextureManager* textureManager;


    //track the mouse
    static bool altDown;
    static bool firstMouse;

    // Act as Constructors and desctructors
    static bool initializeProgram();
    static bool initializeObjects();
    static void cleanUp();

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