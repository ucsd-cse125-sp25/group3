
#include "ServerGame.h"
#include <iostream>
#include <cstdio>
#include <chrono>
#include <thread>
#include <cassert>
#include <set>
#include <vector>
#define TICK 30 //in ms

unsigned int ServerGame::client_id; 

ServerGame::ServerGame(void)
{
    // id's to assign clients for our table
    client_id = 0;
    
    // set up the server network to listen 
    network = new ServerNetwork(); 

    for (unsigned int i=0; i<NUM_NPC; i++) {
        NPCState npc = NPCState();
        npcData.insert(pair<unsigned int, NPCState>(i, npc));
    }

    // texture is not sent by server; client determines this.

    float refW = 2880.0f;
    float refH = 1800.0f;
    std::vector<Platform> minigamePlatforms;
    minigamePlatforms.emplace_back((330.0f / refW), (1700.0f / refH), (200.0f / refW), (27.0f / refH), 0);//1
    minigamePlatforms.emplace_back((0.0f   / refW), (1475.0f / refH), (270.0f / refW), (27.0f / refH), 0); // 2
    minigamePlatforms.emplace_back((0.0f   / refW), (1595.0f / refH), (360.0f / refW), (32.0f / refH), 0); // 3
    minigamePlatforms.emplace_back((468.0f / refW), (1212.0f / refH), (360.0f / refW), (43.0f / refH), 0); // 4
    minigamePlatforms.emplace_back((1205.0f / refW), (1060.0f / refH), (520.0f / refW), (62.0f / refH), 0); // 5
    minigamePlatforms.emplace_back((1890.0f / refW), (1268.0f / refH), (408.0f / refW), (58.0f / refH), 0); // 6
    minigamePlatforms.emplace_back((2570.0f / refW), (930.0f / refH), (320.0f / refW), (55.0f / refH), 0); // 7
    minigamePlatforms.emplace_back((520.0f / refW), (750.0f / refH), (410.0f / refW), (54.0f / refH), 0); // 8
    minigamePlatforms.emplace_back((1400.0f / refW), (555.0f / refH), (480.0f / refW), (55.0f / refH), 0); // 9
    minigamePlatforms.emplace_back((2270.0f / refW), (510.0f / refH), (480.0f / refW), (37.0f / refH), 0); // 10

    ServerGame::minigame = MiniGameState(refW, refH, minigamePlatforms);
}

void ServerGame::update()
{
    // get new clients
   if(network->acceptNewClient(client_id))
   {
        PlayerData* player = new PlayerData();
        playersData.insert( pair<unsigned int, PlayerData*>(client_id, player) );
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
        std::set<KeyType> recievedMovementKeys;

        if (playersData.find(iter->first) == playersData.end()) {
            printf("Client %d does not have associated player data\n", iter->first);
            continue;
        } 
        PlayerData* player = playersData[iter->first];

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
                    player->init(initPacket);

                    if (!player->initialized) {
                        sendInitPlayerState(iter->first);
                    } else {
                        sendGuiUpdate(iter->first, true);
                    }
                    sendInitMinigamePacket(iter->first);
                    ServerLogic::attemptGameStart(playersData);
                    // if (player->initialized) {
                    //     ServerLogic::attemptGameStart(playersData);
                    // }
                    // printf("player is character %d\n", initPacket->character);
                    // playersData[iter->first] = player;
                    // sendInitPlayerState(iter->first);
                    // 
                    // ServerLogic::gameStarted = true;
                    //sendInitNPCState(iter->first);
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
                        if (player->currentState == IN_MINIGAME) {
                            if(ServerLogic::processMovement(recievedMovementKeys, keyPacket->key_type)) {
                                player->minigameCharacter.handleInput(keyPacket->key_type);
                            }
                            sendMinigameCharacterPacket(iter->first);
                        }
                        else if (player->currentState != PLAYING) {
                            printf("recieved gui input\n");
                            player->handleGuiInput(keyPacket->key_type);
                            sendGuiUpdate(iter->first, false);
                        } else if (ServerLogic::processMovement(recievedMovementKeys, keyPacket->key_type)) {
                            player->calculateNewPos(keyPacket->key_type, &artifact);
                        } 
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
                    // printf("server recieved cursor event packet from client\n");
                    if (player->currentState == PLAYING) player->handleCursor(cursorPacket->currX, cursorPacket->currY);
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
            // player->calculateNewPos(recievedKeys, &artifact);
            player->update();

            // player.camera.Update(player.cube.getPosition()); 
            // player.cube.update();
            // playersData[iter->first] = player;
            iter++;
        }
    }
    std::map<unsigned int, NPCState>::iterator npcIter;

    for (npcIter=npcData.begin(); npcIter!=npcData.end(); npcIter++) {
        NPCState npc = npcData[npcIter->first];
        npc.update();
        npcData[npcIter->first] = npc;
    }
    artifact.update();
    
    // sendGuiUpdate();

    // if (!ServerLogic::gameStarted) {
    // } 
    //     sendStateUpdate();
    // } else {
    //     sendGuiUpdate();
    // }
    if (ServerLogic::gameStarted) {
        sendStateUpdate();
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

//should specify id of who's disconnecting so other clients know to remove that client's data
void ServerGame::disconnectClient(unsigned int client_id) {
    EndGamePacket packet;
    packet.packet_type = END_GAME;
    packet.closedClient = client_id;
    const unsigned int packet_size = packet.getSize();
    std::vector<char> packet_data(packet_size);
    packet.serialize(packet_data.data());
    network->sendToAll(packet_data.data(), packet_size);
    ServerLogic::availableChars[playersData[client_id]->cube.type] = true;
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
void ServerGame::sendInitPlayerState(unsigned int client_id) {
    printf("Sending initial player state");
    PlayerData* player = playersData[client_id];

    InitPlayerPacket packet;
    packet.packet_type = INIT_PLAYER;

    packet.clientID = client_id;
    packet.currentState = player->currentState;
    packet.altDown = player->altDown;
    packet.radarActive = player->radarActive;

    player->cube.saveToPacket(packet);
    player->camera.saveToPacket(packet, false);
    player->camera.saveToPacket(packet, true);

    const unsigned int packet_size = packet.getSize();
    std::vector<char> packet_data(packet_size);
    packet.serialize(packet_data.data());

    network->sendToOne(client_id, packet_data.data(), packet_size);
}

void ServerGame::sendInitMinigamePacket(unsigned int client_id) {
    printf("Sending initial minigame state");

    InitMinigamePacket packet;
    packet.packet_type = INIT_MINIGAME;

    packet.numPlatforms = minigame.platforms.size();

    for(int i = 0; i < packet.numPlatforms; i++) {
        std::cout << "Sending platform: " << minigame.platforms[i].x << ", " << minigame.platforms[i].y << ", " << minigame.platforms[i].width << ", " << minigame.platforms[i].height << std::endl;
        packet.platformX.push_back(minigame.platforms[i].x);
        packet.platformY.push_back(minigame.platforms[i].y);
        packet.platformWidth.push_back(minigame.platforms[i].width);
        packet.platformHeight.push_back(minigame.platforms[i].height);
    }

    const unsigned int packet_size = packet.getSize();
    std::vector<char> packet_data(packet_size);
    packet.serialize(packet_data.data());

    network->sendToOne(client_id, packet_data.data(), packet_size);
}

//ideally would send game state to all clients at same time, probably at game start
void ServerGame::sendInitNPCState(unsigned int client_id) {
//     printf("Sending initial NPC state");
//     PlayerData& player = playersData[client_id];

//     InitPlayerPacket packet;
//     packet.packet_type = INIT_PLAYER;

//     packet.clientID = client_id;
//     packet.altDown = player.altDown;

//     player.cube.saveToPacket(packet);
//     player.camera.saveToPacket(packet, false);
//     player.camera.saveToPacket(packet, true);

//     const unsigned int packet_size = packet.getSize();
//     std::vector<char> packet_data(packet_size);
//     packet.serialize(packet_data.data());

//     network->sendToOne(client_id, packet_data.data(), packet_size);
}

void ServerGame::sendStateUpdate() {
    std::map<unsigned int, PlayerData*>::iterator playerIter;
    StateUpdatePacket packet;
    packet.packet_type = STATE_UPDATE;

    unsigned int numClients = 0;
    
    for (playerIter = playersData.begin(); playerIter != playersData.end(); playerIter++) {
        PlayerData* player = playerIter->second;

        if (player->initialized) {
            auto playerPacket = std::make_unique<InitPlayerPacket>();
            playerPacket->packet_type = INIT_PLAYER;
            playerPacket->clientID = playerIter->first;
            playerPacket->currentState = player->currentState;
            playerPacket->altDown = player->altDown;
            playerPacket->radarActive = player->radarActive;

            player->cube.saveToPacket(*playerPacket);
            player->camera.saveToPacket(*playerPacket, false);
            player->miniMapCam.saveToPacket(*playerPacket, true);

            packet.clientPackets.push_back(std::move(playerPacket));
            numClients++;
        }
    }
    packet.numClients = numClients;
    std::map<unsigned int, NPCState>::iterator npcIter;

    for (npcIter = npcData.begin(); npcIter != npcData.end(); npcIter++) {
        NPCState npc = npcIter->second;
        auto npcPacket = std::make_unique<NPCPacket>();
        npcPacket->packet_type = NPC_STATE;
        npcPacket->npcID = npcIter->first;
        npc.saveToPacket(*npcPacket);
        // npcPacket->npcID = npcIter->first;
        // npc.cube.saveToPacket(*npcPacket);
        packet.npcPackets.push_back(std::move(npcPacket));
    }
    packet.numNPCs = NUM_NPC;
    artifact.saveToPacket(packet);
    const unsigned int packet_size = packet.getSize();
    std::vector<char> packet_data(packet_size);
    packet.serialize(packet_data.data());
    network->sendToAll(packet_data.data(), packet_size);
}

void ServerGame::sendMinigameCharacterPacket(unsigned int client_id) {
    printf("Sending minigame character state to %d\n", client_id);
    PlayerData* player = playersData[client_id];

    MinigameCharacterPacket packet;
    packet.packet_type = MINIGAME_CHARACTER;

    packet.x = player->minigameCharacter.x;
    packet.y = player->minigameCharacter.y;
    packet.facingRight = player->minigameCharacter.facingRight;
    packet.isFinished = player->minigameCharacter.isFinished;

    const unsigned int packet_size = packet.getSize();
    std::vector<char> packet_data(packet_size);
    packet.serialize(packet_data.data());

    network->sendToOne(client_id, packet_data.data(), packet_size);
}

void ServerGame::sendGuiUpdate(unsigned int client_id, bool sendAll) {
    std::map<unsigned int, PlayerData*>::iterator playerIter;

    //for now just send current state every loop
    for (playerIter = playersData.begin(); playerIter != playersData.end(); playerIter++) {

        if (!sendAll && playerIter->first != client_id) continue;

        PlayerData* player = playersData[playerIter->first];

        // if (player->currentState == PLAYING || player->currentState == IN_MINIGAME) return;

        GuiUpdatePacket packet;
        packet.packet_type = GUI_UPDATE;
        packet.currentState = player->currentState;

        for (int i=0; i<4; i++) {
            packet.availableChars[i] = ServerLogic::availableChars[i];
        }
        const unsigned int packet_size = packet.getSize();
        std::vector<char> packet_data(packet_size);
        packet.serialize(packet_data.data());
        // printf("sending state: %d\n", player->currentState);
        network->sendToOne(playerIter->first, packet_data.data(), packet_size);
    }
}
