#ifdef _WIN32
    #include <winsock2.h>
    #include <Windows.h>
#endif
#include "ClientNetwork.h"
#include <string>
#include "../include/Window.h"
#include "../include/core.h"
#include "../include/ClientLogic.h"
#include "../packets/PacketFactory.h"
#include <chrono>
#include <thread>

// Maps GLFW key to a key type
const std::unordered_map<int, KeyType> keys = {
    {GLFW_KEY_W, KeyType::KEY_W},
    {GLFW_KEY_S, KeyType::KEY_S},
    {GLFW_KEY_D, KeyType::KEY_D},
    {GLFW_KEY_A, KeyType::KEY_A},
    {GLFW_KEY_T, KeyType::KEY_T},
    {GLFW_KEY_G, KeyType::KEY_G},
    {GLFW_KEY_K, KeyType::KEY_K},
    {GLFW_KEY_L, KeyType::KEY_L},
    {GLFW_KEY_E, KeyType::KEY_E}
};

struct KeyEvent {
    KeyType key_type; // NOT the GLFW key but its mapping to the key type
    int action; // GLFW_PRESS, GLFW_RELEASE
    unsigned int tick; // client tick it was triggered on
};

class ClientGame
{

public:

    ClientGame(int character);
    ~ClientGame(void);
    GLFWwindow* window;
    ClientNetwork* network; 
    unsigned int current_tick;
    unsigned int tick_rate;
    std::vector<KeyEvent> input_buffer; // Consists of key events, gets cleared every tick
    const size_t MAX_KEY_EVENTS_PER_TICK = 4; // Max size of input_buffer
    void sendPacket(Packet& packet);
    char network_data[MAX_PACKET_SIZE];
    void update();
};