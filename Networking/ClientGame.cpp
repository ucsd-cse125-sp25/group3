
#include "ClientGame.h" 


ClientGame::ClientGame(void)
{

    network = new ClientNetwork();

    // send init packet
    const unsigned int packet_size = sizeof(Packet);
    char packet_data[packet_size];

    Packet packet;
    packet.packet_type = INIT_CONNECTION;

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
    memcpy(packet.message, input.data(), sizeof input);
    // packet.message = input.data();
    packet.serialize(packet_data);
    // printf(packet_data);
    NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
}

void ClientGame::update(std::string input)
{
    sendEchoPackets(input);

    Packet packet;
    int data_length = network->receivePackets(network_data);

    while (data_length <= 0) {
        data_length = network->receivePackets(network_data);
    }

    // if (data_length <= 0) 
    // {
    //     //no data recieved
    //     return;
    // }

    int i = 0;
    while (i < (unsigned int)data_length) 
    {
        packet.deserialize(&(network_data[i]));
        i += sizeof(Packet);

        switch (packet.packet_type) {

            case ACTION_EVENT:

                printf("client received action event packet from server\n");

                sendActionPackets();

                break;
            case ECHO_EVENT:
                printf("client recieved echo event packet from server\n");
                printf("Client recieved: %s\n", packet.message);
                break;
            default:

                printf("error in packet types\n");

                break;
        }
    }
}