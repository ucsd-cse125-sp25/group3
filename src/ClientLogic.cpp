#include "ClientLogic.h" 

std::vector<std::unique_ptr<Packet>> client_logic::pendingPackets;

void client_logic::error_callback(int error, const char* description) {
    // Print error.
    std::cerr << description << std::endl;
}

void client_logic::setup_opengl_settings() {
    // Enable depth buffering.
    glEnable(GL_DEPTH_TEST);
    // Related to shaders and z value comparisons for the depth buffer.
    glDepthFunc(GL_LEQUAL);
    // Set polygon drawing mode to fill front and back of each polygon.
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // Set clear color to black.
    glClearColor(0.0, 0.0, 0.0, 0.0);
}

void client_logic::setup_callbacks(GLFWwindow* window) {
    // Set the error callback.
    glfwSetErrorCallback(error_callback);
    // Set the window resize callback.
    glfwSetWindowSizeCallback(window, Window::resizeCallback);

    // Set the key callback.
    // glfwSetKeyCallback(window, Window::keyCallback);
    glfwSetKeyCallback(window, client_logic::keyCallBack);
    // Set the mouse and cursor callbacks
    glfwSetMouseButtonCallback(window, Window::mouse_callback);
    glfwSetCursorPosCallback(window, client_logic::cursor_callback);
}

KeyType client_logic::handleUserInput(GLFWwindow* window) {
    
    KeyType key = KeyType::NONE;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        key = KeyType::KEY_W;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        key = KeyType::KEY_S;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        key = KeyType::KEY_D;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        key = KeyType::KEY_A;

    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
        key = KeyType::KEY_T;

    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
        key = KeyType::KEY_G;

    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        key = KeyType::KEY_K;

    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        key = KeyType::KEY_L;
    
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        key = KeyType::KEY_E;

    if (key != KeyType::NONE) {
        auto packet = std::make_unique<KeyPacket>();
        packet->packet_type = KEY_INPUT;
        packet->key_type = key;
        pendingPackets.push_back(std::move(packet));
    }
        return key;
}

void client_logic::keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods) {
    // printf("running callback\n");
    // Packet packet;
    // KeyType keyType;

    if (action == GLFW_PRESS) {

        if (key == GLFW_KEY_LEFT_ALT || key == GLFW_KEY_RIGHT_ALT) 
        {
            // altDown = true;
            // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // 显示鼠标
            auto packet = std::make_unique<KeyPacket>();
            packet->packet_type = KEY_INPUT;
            packet->key_type = KEY_ALT_PRESS;
            pendingPackets.push_back(std::move(packet));
        }

        switch (key) 
        {
            case GLFW_KEY_ESCAPE: {
                // Close the window. This causes the program to also terminate.
                // pendingKeys.push_back(KeyType::KEY_ESC);
                // packet.payload.resize(1);
                // memcpy(packet.payload.data(), &key, sizeof(key));

                auto packet = std::make_unique<EndGamePacket>();
                packet->packet_type = END_GAME;
                pendingPackets.push_back(std::move(packet));
                break;
            }
            case GLFW_KEY_R: {
                // resetCamera();
                auto packet = std::make_unique<KeyPacket>();
                packet->packet_type = KEY_INPUT;
                packet->key_type = KEY_R;
                pendingPackets.push_back(std::move(packet));
                break;
            }
            case GLFW_KEY_SPACE: {
                auto packet = std::make_unique<KeyPacket>();
                packet->packet_type = KEY_INPUT;
                packet->key_type = KEY_SPACE;
                pendingPackets.push_back(std::move(packet));
                
                // pendingKeys.push_back(KeyType::KEY_SPACE);
                // printf("size: %zu\n", pendingKeys.size());
                    // cube->userInput(key);
                    //cube->handleContinuousInput(window);
                
                break;
            }
            case GLFW_KEY_F: {
                auto packet = std::make_unique<KeyPacket>();
                packet->packet_type = KEY_INPUT;
                packet->key_type = KEY_F;
                pendingPackets.push_back(std::move(packet));
                break;
            }
            default:
                break;
        }
    }
    else if (action == GLFW_RELEASE)
    {
        if (key == GLFW_KEY_LEFT_ALT || key == GLFW_KEY_RIGHT_ALT) 
        {
            // altDown = false;
            // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            // firstMouse = true;
            auto packet = std::make_unique<KeyPacket>();
            packet->packet_type = KEY_INPUT;
            packet->key_type = KEY_ALT_RELEASE;
            pendingPackets.push_back(std::move(packet));
        }
    }
}

void client_logic::cursor_callback(GLFWwindow* window, double currX, double currY) {
    auto packet = std::make_unique<CursorPacket>();
    packet->packet_type = CURSOR_EVENT;
    packet->currX = currX;
    packet->currY = currY;
    pendingPackets.push_back(std::move(packet));
}
