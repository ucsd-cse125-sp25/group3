
#include "ServerGame.h"
#include <iostream>
#include <cstdio>
#include <chrono>
#include <thread>
#include <cassert>
#include <vector>
#define TICK 30 //in ms

unsigned int ServerGame::client_id; 

ServerGame::ServerGame(void)
{
    // id's to assign clients for our table
    client_id = 0;
    
    // set up the server network to listen 
    network = new ServerNetwork(); 

    // printf("%lu", sizeof(CubeState));
}

void ServerGame::update()
{
    // get new clients
   if(network->acceptNewClient(client_id))
   {
        PlayerData player;
        playersData.insert( pair<unsigned int, PlayerData>(client_id, player) );
        printf("client %d has been connected to the server\n",client_id);
        client_id++;
   }
   
   receiveFromClients();
}

void ServerGame::receiveFromClients()
{
    auto start = std::chrono::steady_clock::now();
    // Packet packet;

    // go through all clients
    #ifdef _WIN32
        std::map<unsigned int, SOCKET>::iterator iter;
    #else
        std::map<unsigned int, int>::iterator iter;
    #endif
    
    for(iter = network->sessions.begin(); iter != network->sessions.end(); iter++)
    {
        int data_length = network->receiveData(iter->first, network_data);
        
        if (playersData.find(iter->first) == playersData.end()) {
            printf("Client %d does not have associated player data\n", iter->first);
            continue;
        }

        PlayerData player = playersData[iter->first];

        // if (data_length <= 0) 
        // {
        //     player.cube.update();
        //     playersData[iter->first] = player;
        //     sendPlayerState(iter->first);
        //     continue;
        // }

        int i = 0;
        while (i < data_length) 
        {
            std::unique_ptr<Packet> packet = PacketFactory::createFromBuffer(&network_data[i]);
            // TODO: Handle case where an invalid packet is received
            if (packet == nullptr) {
                break; // couldn't create packet, skip
            }
            i += packet->getSize();

            switch (packet->packet_type) {
                case INIT_CONNECTION: {
                    InitPacket* initPacket = dynamic_cast<InitPacket*>(packet.get());
                    printf("server received init packet from client\n");
                    printf("player is character %d\n", initPacket->character);
                    player.setCharacter(initPacket->character);
                    playersData[iter->first] = player;
                    sendPlayerState(iter->first);
                    break;
                }
                case KEY_EVENT: {
                    KeyPacket* keyPacket = dynamic_cast<KeyPacket*>(packet.get());
                    if (keyPacket) {
                        player.calculateNewPos(keyPacket->key_type);
                        playersData[iter->first] = player;
                        sendPlayerState(iter->first);
                    } else {
                        printf("Error: Failed to cast to KeyPacket\n");
                    }
                    break;
                }
                default: {
                    printf("error in packet types\n");
                    break;
                }
            }
        }
        player.cube.update();
        playersData[iter->first] = player;
        sendPlayerState(iter->first);
        // playersData[iter->first] = player;
        // sendPlayerState(iter->first);
    }
    auto orig_diff = std::chrono::steady_clock::now() - start;
    auto milli_diff = std::chrono::duration_cast<std::chrono::milliseconds>(orig_diff);
    auto wait = std::chrono::milliseconds(TICK) - milli_diff;
    //assert(wait.count() >= 0);
    //std::this_thread::sleep_for(wait);
    std::this_thread::sleep_for(std::max(wait, std::chrono::milliseconds(0)));
    if(wait.count() < 0) {
        printf("WARNING: Tick took longer than allocated by %lld ms\n", -wait.count());
    }
}

void ServerGame::sendActionPackets()
{
    // send action packet
    const unsigned int packet_size = sizeof(Packet);
    char packet_data[packet_size];

    Packet packet;
    packet.packet_type = ACTION_EVENT;

    packet.serialize(packet_data);

    network->sendToAll(packet_data,packet_size);
}

void ServerGame::sendEchoPackets(std::string response) {
    const unsigned int packet_size = sizeof(Packet);
    char packet_data[packet_size];

    Packet packet;
    packet.packet_type = ECHO_EVENT;
    // memcpy(packet.payload, response.data(), sizeof response);
    packet.serialize(packet_data);
    // network->sendToAll(packet_data, packet_size);
}

void ServerGame::sendPlayerState(unsigned int client_id) {
    PlayerData& player = playersData[client_id];

    StateUpdatePacket packet;
    packet.packet_type = STATE_UPDATE;

    // Fill base_model_vector and state_model_vector from the cube
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            packet.base_model[i][j] = player.cube.baseModel[i][j];
            packet.next_model[i][j] = player.cube.model[i][j];
        }
    }
    packet.isInvisible = player.cube.isInvisible;

    const unsigned int packet_size = packet.getSize();
    char packet_data[packet_size];
    packet.serialize(packet_data);

    network->sendToAll(packet_data, packet_size);
    // for (int i=0; i<64; i++) {
    //     printf("elem %d: %hhx\n", i, (unsigned char) packet.payload[i]);
    // }
}