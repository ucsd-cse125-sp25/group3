
#include "ClientGame.h" 

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
}

void ClientGame::sendActionPackets()
{
    // send action packet
    const unsigned int packet_size = sizeof(Packet);
    char packet_data[packet_size];

    Packet packet;
    packet.packet_type = ACTION_EVENT;

    packet.serialize(packet_data);

    NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
}

void ClientGame::sendEchoPackets(std::string input) {
    const unsigned int packet_size = sizeof(Packet);
    char packet_data[packet_size];

    Packet packet;
    packet.packet_type = ECHO_EVENT;
    // memcpy(packet.payload, input.data(), sizeof input);
    // packet.message = input.data();
    packet.serialize(packet_data);
    // printf(packet_data);
    NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
}

void ClientGame::sendKeyPackets(KeyType key) {
    Packet packet;
    packet.packet_type = KEY_EVENT;
    packet.payload.resize(1);
    memcpy(packet.payload.data(), &key, sizeof(key));
    packet.length = packet.payload.size();

    const unsigned int packet_size = packet.getSize();

    std::vector<char> packet_data(packet_size);  
    packet.serialize(packet_data.data());

    NetworkServices::sendMessage(network->ConnectSocket, packet_data.data(), packet_size);
}

void ClientGame::update()
{
    glfwPollEvents();

    for (int i=0; i<client_logic::pendingKeys.size(); i++) {
        
        if (client_logic::pendingKeys[i] == KeyType::KEY_ESC) {
            close(network->ConnectSocket);
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
        sendKeyPackets(client_logic::pendingKeys[i]);
    }
    client_logic::pendingKeys.clear();

    KeyType input = client_logic::handleUserInput(window);

    if (input != KeyType::NONE) {
        printf("sending key event packet\n");
        sendKeyPackets(input);
    } 

    while (true) {
        char header[Packet::getHeaderSize()];
        int data_length = network->receivePackets(header, Packet::getHeaderSize());

        if (data_length == -1) {
            break;
        }
        Packet packet;
        packet.deserializeHeader(header);
        char data[packet.length];
        data_length = network->receivePackets(data, packet.length);
        packet.deserializePayload(data);
        
        switch (packet.packet_type) {

            case ACTION_EVENT:
                printf("client received action event packet from server\n");
                break;
            case ECHO_EVENT:
                printf("client recieved echo event packet from server\n");
                break;
            case STATE_UPDATE:
                printf("client recieved state update from server\n");
                // printf("payload size: %d\n", packet.length);
                Window::update(packet.payload.data(), packet.length);
                // Window::render(window);
                // Window::cube->update();
                break;
            default:

                printf("error in packet types\n");

                break;
        }
    }
    Window::render(window);
    // Window::updateRest(packet.payload.data());
    // Window::applyRest(packet.payload.data(), , packet.length);
    // Window::cube->update();
    // Window::idleCallback();
}

