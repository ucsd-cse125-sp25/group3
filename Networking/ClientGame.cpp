
#include "ClientGame.h" 

ClientGame::ClientGame(void)
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
    packet.length = 0;
    const unsigned int packet_size = packet.getSize();
    char packet_data[packet_size];
    packet.serialize(packet_data);
    NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
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
    char packet_data[packet_size];
    packet.serialize(packet_data);
    NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
}

void ClientGame::update()
{
    KeyType input = client_logic::handleUserInput(window);

    if (input != KeyType::NONE) {
        printf("sending key event packet\n");
        sendKeyPackets(input);
    } 

    int data_length = network->receivePackets(network_data);

    if (data_length > 0) {
        int i = 0;

        while (i < (unsigned int)data_length) 
        {
            Packet packet;
            packet.deserialize(&(network_data[i]));
            i += packet.getSize();
        
            switch (packet.packet_type) {

                case ACTION_EVENT:

                    printf("client received action event packet from server\n");

                    // sendActionPackets();

                    break;
                case ECHO_EVENT:
                    printf("client recieved echo event packet from server\n");
                    /* printf("Client recieved: %s\n", packet.payload.data()); */
                    break;
                case STATE_UPDATE:
                    printf("client recieved state update from server\n");
                    Window::update(packet.payload.data());
                    break;
                default:

                    printf("error in packet types\n");

                    break;
            }
        }
    }
    Window::render(window);
    Window::idleCallback();
}

