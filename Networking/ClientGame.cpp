#include "ClientGame.h" 
#include <GLFW/glfw3.h>

/* void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if(action != GLFW_PRESS && action != GLFW_RELEASE) return; // Only concerned about these events; skip if we receive something else
    printf("Action: %d Key: %d\n", action, key);
    ClientGame* game = static_cast<ClientGame*>(glfwGetWindowUserPointer(window));
    auto it = keys.find(key);
    if (it == keys.end()) return; // not tracking this key, don't do anything
    KeyType mappedKey = it->second;

    //TODO: Potentially deal with case where client attempts to send more events than allowed
    if(game->input_buffer.size() < game->MAX_KEY_EVENTS_PER_TICK) {
        game->input_buffer.push_back({mappedKey, action, game->current_tick});
    }
}
 */

 void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    // Get reference to the game context
    ClientGame* game = static_cast<ClientGame*>(glfwGetWindowUserPointer(window));
    // Multiplayer input handling
    if (action == GLFW_PRESS || action == GLFW_RELEASE) {
        printf("Action: %d Key: %d\n", action, key);
    
        if (game) {
            auto it = keys.find(key);
            if (it != keys.end()) {
                KeyType mappedKey = it->second;

                // Limit key events per tick
                if (game->input_buffer.size() < game->MAX_KEY_EVENTS_PER_TICK) {
                    game->input_buffer.push_back({mappedKey, action, game->current_tick});
                }
            }
        }
    }

    // Local key handling (camera, UI, etc.)
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_LEFT_ALT || key == GLFW_KEY_RIGHT_ALT) {
            Window::altDown = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }

        switch (key) {
            case GLFW_KEY_ESCAPE:
                // Close the window. This causes the program to also terminate.
                glfwSetWindowShouldClose(game->window, GL_TRUE);
                break;

            case GLFW_KEY_R:
                Window::resetCamera();
                break;

            default:
                if (Window::cube != nullptr) {
                    Window::cube->userInput(key);
                    // cube->handleContinuousInput(window);
                }
                break;
        }
    } else if (action == GLFW_RELEASE) {
        if (key == GLFW_KEY_LEFT_ALT || key == GLFW_KEY_RIGHT_ALT) {
            Window::altDown = false;
            glfwSetInputMode(game->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            Window::firstMouse = true;
        }
    }
}

ClientGame::ClientGame(int character)
{
    network = new ClientNetwork();
    
    window = Window::createWindow(800, 600);
    if (!window) exit(EXIT_FAILURE);

    client_logic::setup_callbacks(window);
    // Setup OpenGL settings.
    client_logic::setup_opengl_settings();

    // Initialize the shader program; exit if initialization fails.
    if (!Window::initializeProgram()) exit(EXIT_FAILURE);
    // Initialize objects/pointers for rendering; exit if initialization fails.
    if (!Window::initializeObjects()) exit(EXIT_FAILURE);

    // send init packet
    Packet packet;
    packet.packet_type = INIT_CONNECTION;
    packet.payload.resize(sizeof(int));
    memcpy(packet.payload.data(), &character, sizeof(character));
    // packet.length = 0;
    packet.length = packet.payload.size();
    const unsigned int packet_size = packet.getSize();
    std::vector<char> packet_data(packet_size);
    packet.serialize(packet_data.data());
    NetworkServices::sendMessage(network->ConnectSocket, packet_data.data(), packet_size);

    // Allows us to access ClientGame* through the window
    glfwSetWindowUserPointer(window, this);

    // Creates interrupts fwhen a key is pressed
    glfwSetKeyCallback(window, key_callback);

    // TODO: Initialize based off server reply
    current_tick = 0; 
    tick_rate = 30; // in ms 
}

void ClientGame::sendPacket(Packet& packet) {
    const unsigned int packet_size = packet.getSize();
    std::vector<char> packet_data(packet_size);  
    packet.serialize(packet_data.data());
    printf("Packet of size %d\n", packet_size);

    NetworkServices::sendMessage(network->ConnectSocket, packet_data.data(), packet_size);
}

// NOTE: Currently sending and receiving one packet per tick
void ClientGame::update()
{
    auto start = std::chrono::steady_clock::now();
    // Send packets to the server

    // a. Send key packets 
    for (const KeyEvent& input : input_buffer) {
        KeyPacket packet;
        packet.packet_type = KEY_EVENT;
        packet.key_type = input.key_type;
        packet.tick = input.tick;
        packet.is_down = input.action == GLFW_PRESS;
        sendPacket(packet);
        printf("Client Sending key event for key %d at tick %u\n", input.key_type, input.tick);
    }
    input_buffer.clear();

    int data_length = network->receivePackets(network_data);

    // Receive packets from the server
    if (data_length > 0) {
        int i = 0;

        while (i < (unsigned int)data_length) 
        {
            Packet packet;
            int bytes_read = packet.deserialize(&(network_data[i]));
            i += bytes_read;
        
            switch (packet.packet_type) {
                case ECHO_EVENT:
                    printf("client recieved echo event packet from server\n");
                    /* printf("Client recieved: %s\n", packet.payload.data()); */
                    break;
                case STATE_UPDATE:
                    printf("Client received STATE_UPDATE at tick %u\n", current_tick);
                    Window::update(packet.payload.data(), packet.length);
                    break;
                default:
                    printf("client received unknown packet type from server\n");
                    break;
            }
        }
    }
    Window::render(window);
    Window::idleCallback();

    auto orig_diff = std::chrono::steady_clock::now() - start;
    auto milli_diff = std::chrono::duration_cast<std::chrono::milliseconds>(orig_diff);
    auto wait = std::chrono::milliseconds(tick_rate) - milli_diff;
    //assert(wait.count() >= 0);
    //std::this_thread::sleep_for(wait);
    std::this_thread::sleep_for(std::max(wait, std::chrono::milliseconds(0)));
    if(wait.count() < 0) {
        printf("WARNING: Tick took longer than allocated by %ld ms\n", -wait.count());
    }
    current_tick += 1;
}

