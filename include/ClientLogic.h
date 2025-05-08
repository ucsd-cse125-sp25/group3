#include <iostream>
#include <string>
#include "Window.h"
#include "Cube.h"
#include "core.h"
#include <vector>
#include "../packets/PacketFactory.h"

#ifndef OS_TYPES
#define OS_TYPES
enum class OSType: uint8_t {
    APPLE = 0,
    OTHER = 1,
};
#endif

#ifndef CLIENT_LOGIC
#define CLIENT_LOGIC
class client_logic {
    public:
        static std::vector<KeyType> pendingKeys;

        static void error_callback(int error, const char* description);

        static void setup_opengl_settings();

        static void setup_callbacks(GLFWwindow* window);

        static KeyType handleUserInput(GLFWwindow* window);

        static void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);
};
#endif
