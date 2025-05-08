
#include "ClientGame.h" 

ClientGame::ClientGame(CharacterType character)
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
    sendInitPacket(character);
}

void ClientGame::sendPacket(Packet& packet) {
    const unsigned int packet_size = packet.getSize();

    std::vector<char> packet_data(packet_size);  
    packet.serialize(packet_data.data());
    printf("Packet of size %d\n", packet_size);

    NetworkServices::sendMessage(network->ConnectSocket, packet_data.data(), packet_size);
}

void ClientGame::sendInitPacket(CharacterType character) {
    InitPacket packet;
    packet.packet_type = INIT_CONNECTION;
    packet.character = character;
    packet.length = packet.payload.size();
    sendPacket(packet);
}

void ClientGame::sendKeyPacket(KeyType key) {
    KeyPacket packet;
    packet.packet_type = KEY_EVENT;
    packet.key_type = key;
    sendPacket(packet);
}

void ClientGame::update()
{
    glfwPollEvents();

    for (int i=0; i<client_logic::pendingKeys.size(); i++) {
        sendKeyPacket(client_logic::pendingKeys[i]);
    }
    client_logic::pendingKeys.clear();

    KeyType input = client_logic::handleUserInput(window);

    if (input != KeyType::NONE) {
        printf("sending key event packet\n");
        sendKeyPacket(input);
    } 

    int data_length = network->receivePackets(network_data);

    if (data_length > 0) {
        int i = 0;

        while (i < (unsigned int)data_length) 
        {
            Packet packet;
            int bytes_read = packet.deserialize(&(network_data[i]));
            i += bytes_read;
        
            switch (packet.packet_type) {
                case STATE_UPDATE:
                    printf("client recieved state update from server\n");
                    Window::update(packet.payload.data(), packet.length);
                    // Window::render(window);
                    // Window::cube->update();
                    break;
                default:
                    printf("client received unknown packet type from server\n");
                    break;
            }
        }
    }
    
    Window::render(window);
    // Window::cube->update();
    // Window::idleCallback();
}

