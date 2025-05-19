
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
    packet.windowWidth = Window::width;
    packet.windowHeight = Window::height;
    sendPacket(packet);
}

void ClientGame::sendKeyPacket(KeyType key) {
    KeyPacket packet;
    packet.packet_type = KEY_INPUT;
    packet.key_type = key;
    sendPacket(packet);
}

void ClientGame::sendPendingPackets() {

    for (auto& packet : client_logic::pendingPackets) {
        const unsigned int packet_size = packet->getSize();
        std::vector<char> packet_data(packet_size);  
        packet->serialize(packet_data.data());
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
        int headerSize = Packet::getHeaderSize();
        std::vector<char> header(headerSize);
        int data_length = network->receivePackets(header.data(), headerSize);

        if (data_length == -1) {
            break;
        }
        Packet temp;
        temp.deserializeHeader(header.data());
        //char data[packet.length];
        std::vector<char> full_packet(headerSize + temp.length);
        std::copy(header.begin(), header.end(), full_packet.begin());
        data_length = network->receivePackets(full_packet.data() + headerSize, temp.length);
        std::unique_ptr<Packet> packet = PacketFactory::createFromBuffer(full_packet.data());

        switch (temp.packet_type) {
            case INIT_PLAYER:
                // TODO: Make this work with packet class
                printf("recieved init player packet from server\n");
                /*Window::setClientID(packet.payload.data());*/
                Window::setInitState(packet.payload.data());
                break;
            case ACTION_EVENT:
                printf("client received action event packet from server\n");
                break;
            case ECHO_EVENT:
                printf("client recieved echo event packet from server\n");
                break;
            case STATE_UPDATE:
            {
                printf("client recieved state update from server\n");
                StateUpdatePacket* statePacket = dynamic_cast<StateUpdatePacket*>(packet.get());
                // printf("payload size: %d\n", packet.length);
                if (Window::initialized) Window::applyServerState(*statePacket);
                // Window::render(window);
                // Window::cube->update();
                break;
            }
            case END_GAME:
                printf("client end game packet from server\n");
                // TODO: Make this work with END_GAME packet
                unsigned int closedClient;
                memcpy(&closedClient, packet.payload.data(), sizeof(closedClient));

                if (closedClient == Window::client_id) {
                    #ifdef _WIN32
                    closesocket(network->ConnectSocket);
                    #else
                    close(network->ConnectSocket);
                    #endif
                    glfwSetWindowShouldClose(window, GL_TRUE);
                } else {
                    Window::removeClient(closedClient);
                }
                break;
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

