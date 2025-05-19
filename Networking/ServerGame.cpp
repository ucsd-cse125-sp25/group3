
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

void ServerGame::receiveFromClients() {
    auto start = std::chrono::steady_clock::now();
    // Packet packet;

    // go through all clients
    #ifdef _WIN32
        std::map<unsigned int, SOCKET>::iterator iter;
    #else
        std::map<unsigned int, int>::iterator iter;
    #endif
    
    for(iter = network->sessions.begin(); iter != network->sessions.end();) {
        ClientStatus packetsDone = ONGOING;

        if (playersData.find(iter->first) == playersData.end()) {
            printf("Client %d does not have associated player data\n", iter->first);
            continue;
        } 
        PlayerData player = playersData[iter->first];

        while (packetsDone == ONGOING) {
            int headerSize = Packet::getHeaderSize();
            //char header[headerSize];
            std::vector<char> header(Packet::getHeaderSize());
            int data_length = network->receiveData(iter->first, header.data(), headerSize);
           
            if (data_length == 0) {
                printf("Client %d has disconnected\n", iter->first);
                playersData.erase(iter->first);
                iter = network->sessions.erase(iter);
                packetsDone = DISCONNECT;
                break;
            } else if (data_length == -1) { //no packets left
                packetsDone = SUCCESS;
                // iter++;
                break;
            } 
            Packet temp;
            temp.deserializeHeader(header.data());
            //char data[packet.length];
            std::vector<char> full_packet(headerSize + temp.length);
            std::copy(header.begin(), header.end(), full_packet.begin());
            data_length = network->receiveData(iter->first, full_packet.data() + headerSize, temp.length);
            // printf("data length read: %d\n", data_length);
            // if (data_length <= 0) 
            // {
            //     player.cube.update();
            //     playersData[iter->first] = player;
            //     sendPlayerState(iter->first);
            //     continue;
            // }

            std::unique_ptr<Packet> packet = PacketFactory::createFromBuffer(full_packet.data());
            if (!packet) {
                printf("Received invalid packet from client %d\n", iter->first);
                break;
            }

            switch (packet->packet_type) {
                case INIT_CONNECTION: {
                    InitPacket* initPacket = dynamic_cast<InitPacket*>(packet.get());
                    printf("server received init packet from client\n");
                    player.init(initPacket);
                    printf("player is character %d\n", initPacket->character);
                    playersData[iter->first] = player;
                    sendInitPlayerState(iter->first);
                    break;
                }
                case ACTION_EVENT: {
                    printf("server received action event packet from client\n");
                    sendActionPackets();
                    break;
                }
                case ECHO_EVENT: {
                    /* std::string message(packet.payload.begin(), packet.payload.end());
                    printf("server recieved echo event packet from client\n");
                    printf("Server recieved: %s\n", packet.payload.data());
                    sendEchoPackets(message); */
                    printf("client recieved echo event packet from server\n");
                    break;
                }
                case KEY_INPUT: {
                    KeyPacket* keyPacket = dynamic_cast<KeyPacket*>(packet.get());
                    printf("server recieved key input packet from client\n");
                    if (keyPacket) {
                        player.calculateNewPos(keyPacket->key_type);
                        //playersData[iter->first] = player;
                        //sendPlayerState(iter->first);
                    } else {
                        printf("Error: Failed to cast to KeyPacket\n");
                    }
                    break;
                }
                case END_GAME: {
                    printf("server recieved end game packet from client\n");
                    // disconnectClient();
                    // printf("Client %d has disconnected\n", iter->first);
                    // playersData.erase(iter->first);
                    // iter = network->sessions.erase(iter);
                    packetsDone = DISCONNECT;
                    break;
                }
                case CURSOR_EVENT: {
                    CursorPacket* cursorPacket = dynamic_cast<CursorPacket*>(packet.get());
                    printf("server recieved cursor event packet from client\n");
                    player.handleCursor(cursorPacket->currX, cursorPacket->currY);
                    break;
                }
                default: {
                    printf("error in packet types\n");
                    break;
                }
            }
        }
        if (packetsDone == DISCONNECT)  {
            disconnectClient(iter->first);
            printf("Client %d has disconnected\n", iter->first);
            iter = network->sessions.erase(iter);
        } else {
            player.camera.Update(player.cube.getPosition()); 
            player.cube.update();
            playersData[iter->first] = player;
            iter++;
        }
    }
    sendStateUpdate();
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

//should specify id of who's disconnecting so other clients know to remove that client's data
void ServerGame::disconnectClient(int client_id) {
    Packet packet;
    packet.packet_type = END_GAME;
    char buf[4];
    memcpy(buf, &client_id, sizeof(client_id));
    packet.payload.insert(packet.payload.end(), &buf[0], &buf[4]);
    packet.length = sizeof(client_id);
    const unsigned int packet_size = packet.getSize();
    //char packet_data[packet_size];
    std::vector<char> packet_data(packet_size);
    packet.serialize(packet_data.data());
    network->sendToAll(packet_data.data(), packet_size);
    playersData.erase(client_id);
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

//cube baseModel, model
//then camera floats
// TODO: Make this work with packet classes
void ServerGame::sendInitPlayerState(unsigned int client_id) {
    // printf("sending state\n");
    PlayerData player = playersData[client_id];
    Packet packet;
    packet.packet_type = INIT_PLAYER;
    player.toVector(client_id, &packet.payload);
    // player.cube.toVector(&packet.payload);
    packet.length = packet.payload.size();
    const unsigned int packet_size = packet.getSize();
    //char packet_data[packet_size];
    std::vector<char> packet_data(packet_size);
    packet.serialize(packet_data.data());
    network->sendToOne(client_id, packet_data.data(), packet_size);
    // printf("done sending\n");
    // for (int i=0; i<64; i++) {
    //     printf("elem %d: %hhx\n", i, (unsigned char) packet.payload[i]);
    // }
}

/*
void ServerGame::sendStateUpdate() {
    std::map<unsigned int, PlayerData>::iterator iter;
    Packet packet;
    packet.packet_type = STATE_UPDATE;
    unsigned int numClients = 0;
    
    for (iter = playersData.begin(); iter != playersData.end(); iter++) {
        PlayerData player = iter->second;

        if (player.initialized) {
            player.toVector(iter->first, &packet.payload);
            numClients++;
        }
    }
    char buf[4];
    memcpy(buf, &numClients, sizeof(numClients));
    packet.payload.insert(packet.payload.begin(), &buf[0], &buf[4]);
    packet.length = packet.payload.size();
    const unsigned int packet_size = packet.getSize();
    std::vector<char> packet_data(packet_size);
    packet.serialize(packet_data.data());
    network->sendToAll(packet_data.data(), packet_size);
*/
// TODO: Figure out numClients, include all players in the state update packet
void ServerGame::sendPlayerState(unsigned int client_id) {
    PlayerData& player = playersData[client_id];

    StateUpdatePacket packet;
    packet.packet_type = STATE_UPDATE;

    packet.altDown = player.altDown;

    // cube
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            packet.model[i][j] = player.cube.model[i][j];
        }
    }
    packet.isInvisible = player.cube.isInvisible;

    // camera 
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            packet.viewProjectMtx[i][j] = player.camera.ViewProjectMtx[i][j];
        }
    }

    for (int i = 0; i < 3; ++i) {
        packet.eye[i] = player.camera.Eye[i];
    }

    for (int i = 0; i < 3; ++i) {
        packet.center[i] = player.camera.Center[i];
    }

    packet.azimuth = player.camera.Azimuth;
    packet.incline = player.camera.Incline;
    packet.aspect = player.camera.Aspect;

    // minimap camera
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            packet.miniViewProjectMtx[i][j] = player.miniMapCam.ViewProjectMtx[i][j];
        }
    }

    for (int i = 0; i < 3; ++i) {
        packet.miniEye[i] = player.miniMapCam.Eye[i];
    }

    for (int i = 0; i < 3; ++i) {
        packet.miniCenter[i] = player.miniMapCam.Center[i];
    }

    packet.miniAzimuth = player.miniMapCam.Azimuth;
    packet.miniIncline = player.miniMapCam.Incline;
    packet.miniAspect = player.miniMapCam.Aspect;

    const unsigned int packet_size = packet.getSize();
    char packet_data[packet_size];
    packet.serialize(packet_data);

    network->sendToAll(packet_data, packet_size);
}