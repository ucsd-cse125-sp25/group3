#include <iostream>
#include <string>
#include "Window.h"
#include "Cube.h"
#include "core.h"
#include <map>
#include <cmath>
#include <string>
#include <vector>
#include "../packets/PacketFactory.h"
#include "../minigame/minigame.h"
#include "../minigame/include/platform.h"
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
#include "AudioManager.h"
// #include "../minigame/minigame.h"
// #include <GL/glew.h>
// #include "../minigame/include/stb_image.h"

#include "AudioManager.h"


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
    NONE = -1,
    THIEF_WIN = 0,
    GUARD_WIN = 1
};

class client_logic {
    public:
        static ImGuiIO* io;
        static ImVec2 displaySize;
        static ImFont* handwritingFont;
        static bool availableChars[4];
        static MiniGame miniGame;
        static bool miniGameInitialized;  
        static std::vector<Platform> miniGamePlatformsServer;

        static ImFont* s_font_italic ;
        static ImFont* s_font_bold ;
        static GLuint background_texture;
        static int background_width;
        static int background_height;
        static GLuint title_texture;
        static int title_width;
        static int title_height;
        static bool audio_enabled;


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

        static void RenderFancyTextButton(const char* label, bool& clicked, ImFont* fontNormal, ImFont* fontHover, bool& selected, AudioManager* audio);

        static void RenderFancyTextButton(const char* label, bool& clicked, ImFont* fontNormal, ImFont* fontHover);

        static void setStartPage(GameState currState);

        static void setCharacterSelectPage(GameState currState, GLFWwindow* window, AudioManager* audio);

        static void setMainGameWindow(GLFWwindow* window);

        static void receiveTimeFromServer(int serverSocket);

        static void setMinigamePlatformsServer(const InitMinigamePacket& initMinigamePacket);

        // timer
        static std::string currentTimeString;

        // win 
        static WinState gameResult;
        static CharacterType playerRole;

        // static GLuint img_thief_win, img_thief_lose, img_guard_win, img_guard_lose;
        // static int img_width, img_height;
};
#endif
