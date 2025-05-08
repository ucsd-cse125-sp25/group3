#include <iostream>
#include <string>
#include "Window.h"
#include "Cube.h"
#include "core.h"

namespace client_logic {
    void error_callback(int error, const char* description);

    void setup_opengl_settings();

    void setup_callbacks(GLFWwindow* window);
}