#include <iostream>
#include <string>
#include "Window.h"
#include "Cube.h"
#include "core.h"
#include <vector>
#include "../packets/Packet.h"

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
    KEY_E = 9,
    KEY_SPACE = 10,
    KEY_ESC = 11,
    KEY_R = 12,
    KEY_ALT_PRESS = 13,
    KEY_ALT_RELEASE = 14,
};
#endif

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
        static std::vector<Packet> pendingPackets;

        static void error_callback(int error, const char* description);

        static void setup_opengl_settings();

        static void setup_callbacks(GLFWwindow* window);

        static KeyType handleUserInput(GLFWwindow* window);

        static void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);

        static void cursor_callback(GLFWwindow* window, double currX, double currY);
};
#endif
