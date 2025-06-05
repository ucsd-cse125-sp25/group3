#include <iostream>
#include <string>
#include "Window.h"
#include "Cube.h"
#include "core.h"
#include <vector>
#include "../packets/PacketFactory.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
// #include <unistd.h>
#include <cstring>

#ifdef _WIN32 
    // #include <winsock2.h>
    #include <Windows.h>
#endif
    // #else 
//     #include <sys/socket.h>
// #endif
// #include "../minigame/minigame.h"
// #include <GL/glew.h>
// #include "../minigame/include/stb_image.h"

#ifndef OS_TYPES
#define OS_TYPES
enum class OSType: uint8_t {
    APPLE = 0,
    OTHER = 1,
};
#endif

#ifndef CLIENT_LOGIC
#define CLIENT_LOGIC

enum class WinState {
    NONE,
    THIEF_WIN,
    GUARD_WIN
};

class client_logic {
    public:
        static ImGuiIO* io;
        static ImVec2 displaySize;
        static ImFont* handwritingFont;
        static bool availableChars[4];

        static std::vector<std::unique_ptr<Packet>> pendingPackets;

        static void updateAvailableChars(GuiUpdatePacket& packet);

        static void error_callback(int error, const char* description);

        static void setup_opengl_settings();

        static void setup_callbacks(GLFWwindow* window);

        static void handleUserInput(GLFWwindow* window);

        static void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);

        static void cursor_callback(GLFWwindow* window, double currX, double currY);
        
        static bool LoadTextureFromMemory(const void* data, size_t data_size, GLuint* out_texture, int* out_width, int* out_height);

        static bool LoadTextureFromFile(const char* file_name, GLuint* out_texture, int* out_width, int* out_height);

        static void setStartPage(GameState currState);

        static void setCharacterSelectPage(GameState currState);

        static void setMainGameWindow(GLFWwindow* window);

        static void receiveTimeFromServer(int serverSocket);

        // timer
        static std::string currentTimeString;

        // win 
        static WinState gameResult;
        static CharacterType playerRole;

        // static GLuint img_thief_win, img_thief_lose, img_guard_win, img_guard_lose;
        // static int img_width, img_height;
};
#endif
