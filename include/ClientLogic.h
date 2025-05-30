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
#include "../minigame/minigame.h"
#include "../minigame/include/platform.h"
// #include <GL/glew.h>
#include "../minigame/include/stb_image.h"

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
        static ImGuiIO* io;
        static ImVec2 displaySize;
        static ImFont* handwritingFont;
        static bool availableChars[4];
        static MiniGame miniGame;
        static bool miniGameInitialized;  
        static std::vector<Platform> miniGamePlatforms;

        static std::vector<std::unique_ptr<Packet>> pendingPackets;

        static void updateAvailableChars(GuiUpdatePacket& packet);

        static void setMinigamePlatforms(const InitMinigamePacket& initMinigamePacket);

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
};
#endif
