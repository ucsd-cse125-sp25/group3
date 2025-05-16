
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
    packet.packet_type = KEY_INPUT;
    packet.key_type = key;
    sendPacket(packet);
}

// TODO: Dynamically determine type of packet to send
void ClientGame::sendPendingPackets() {

    for (auto& packet : client_logic::pendingPackets) {
        const unsigned int packet_size = packet.getSize();
        std::vector<char> packet_data(packet_size);  
        packet.serialize(packet_data.data());
        NetworkServices::sendMessage(network->ConnectSocket, packet_data.data(), packet_size);
    }
    client_logic::pendingPackets.clear();
}

void ClientGame::update()
{
    glfwPollEvents();

    KeyType input = client_logic::handleUserInput(window);
    sendPendingPackets();

    while (true) {
        std::vector<char> header(Packet::getHeaderSize());
        int data_length = network->receivePackets(header.data(), Packet::getHeaderSize());

        if (data_length == -1) {
            break;
        }
        Packet tempPacket;
        tempPacket.deserializeHeader(header.data());
        //char data[packet.length];
        std::vector<char> data(tempPacket.length);
        data_length = network->receivePackets(data.data(), tempPacket.length);
     
        switch (tempPacket.packet_type) {
            case ACTION_EVENT:
                printf("client received action event packet from server\n");
                break;
            case ECHO_EVENT:
                printf("client recieved echo event packet from server\n");
                break;
            case STATE_UPDATE:
            {
                printf("client recieved state update from server\n");
                StateUpdatePacket packet;
                packet.deserializeHeader(header.data());
                packet.deserializePayload(data.data());
                // printf("payload size: %d\n", packet.length);
                Window::applyServerState(packet);
                // Window::render(window);
                // Window::cube->update();
                break;
            }
            case END_GAME:
            {
                printf("client end game packet from server\n");
                #ifdef _WIN32
                closesocket(network->ConnectSocket);
                #else
                close(network->ConnectSocket);
                #endif
                glfwSetWindowShouldClose(window, GL_TRUE);
                break;
            }
            default:
                printf("client received unknown packet type from server\n");
                break;
        }
    }
    Window::render(window);
    // Window::updateRest(packet.payload.data());
    // Window::applyRest(packet.payload.data(), , packet.length);
    // Window::cube->update();
    // Window::idleCallback();
}

