#include <iostream>
#include <string>
#include "Window.h"
#include "Cube.h"
#include "core.h"

#ifndef KEY_TYPES
#define KEY_TYPES
enum class KeyType: uint8_t {
    NONE = 0,
    KEY_W = 1,
    KEY_S = 2,
    KEY_D = 3,
    KEY_A = 4,
    KEY_T = 5,
    KEY_G = 6,
    KEY_K = 7,
    KEY_L = 8,
    KEY_SPACE = 9,
};
#endif

void error_callback(int error, const char* description);

void setup_opengl_settings();

void setup_callbacks(GLFWwindow* window);

KeyType handleUserInput(GLFWwindow* window);