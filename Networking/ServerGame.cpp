
#include "ServerGame.h"
#include <iostream>
#include <cstdio>
#include <chrono>
#include <thread>
#include <cassert>
#include <set>
#include <vector>
#include <ctime>
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

    ServerLogic::loadAABBs();

    std::string movingArtifacts[3] = {"horse", "skeleton", "lion"};
    srand(time(NULL));
    unsigned int choice = (unsigned int) (rand() % 3);
    std::cout << "check" << std::endl;
    std::cout << choice << std::endl;
    AABB artifact_bb = ServerLogic::museumAABBs[movingArtifacts[choice]];
    artifact.init(artifact_bb.min, artifact_bb.max, artifact_bb.getCenter(), choice);
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

    // if (ServerLogic::gameStarted && gameStartTime.time_since_epoch().count() == 0) {
    //     std::cout<< "yes in update set initial game start time " << std::endl;
    //     gameStartTime = std::chrono::steady_clock::now();
    // }

    std::string timeStr = getCurrentTimeString();

    TimeUpdate timePacket;
    strncpy(timePacket.timeString, timeStr.c_str(), sizeof(timePacket.timeString));

    char timeData[timePacket.getSize()];
    timePacket.serialize(timeData);

    network->sendToAll(timeData, timePacket.getSize());

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
        player->detectStop();
        // if (player->cube.type == CharacterType::CHARACTER_4) {
        //     player->cube.animState = AnimState::SG_Shooting_Gun;
        // } else if (player->cube.type != CharacterType::NONE) {
        //     player->cube.animState = AnimState::FT_Idle;
        // }

        while (packetsDone == ONGOING) {
            int headerSize = Packet::getHeaderSize();
            //char header[headerSize];
            std::vector<char> header(Packet::getHeaderSize());
            int data_length = network->receiveData(iter->first, header.data(), headerSize);
        //    printf("client %u read %d bytes\n", iter->first, data_length);
            if (data_length == 0) {
                printf("trying to disconnect client %u\n", iter->first);
                // printf("Client %d has disconnected\n", iter->first);
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
                    player->init(initPacket, iter->first);

                    if (!player->initialized) {
                        sendInitPlayerState(iter->first);
                    } else {
                        sendGuiUpdate(iter->first, true);
                    }
                    ServerGame::gameStartTime = std::chrono::steady_clock::now();
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
                    //printf("server recieved key input packet from client\n");
                    if (keyPacket) {
                        
                        if (player->currentState != PLAYING) {
                            printf("recieved gui input\n");
                            player->handleGuiInput(keyPacket->key_type);
                            sendGuiUpdate(iter->first, false);
                        } else if (ServerLogic::processMovement(recievedMovementKeys, keyPacket->key_type)) {
                            player->calculateNewPos(keyPacket->key_type, &artifact, ServerLogic::museumAABBs,playersData);
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
                case WIN_STATE: {
                    printf("Someone win the game! ");
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

    if(artifact.holder != nullptr)
    {
        //printf("Enter check win conditon!");
        if(ServerLogic::winCondition(artifact.holder))
        {
            if (!winAlreadySent) {
                winAlreadySent = true;
                //currentState = WIN_CONDITION;
                printf("Thieves Win\n");

                WinPacket packet;
                packet.packet_type = WIN_STATE;
                packet.winningClientID = artifact.holder->client_id;
                const unsigned int packet_size = packet.getSize();
                std::vector<char> packet_data(packet_size);
                packet.serialize(packet_data.data());
                network->sendToAll(packet_data.data(), packet_size);
            }

            
        }

    }

    // // Security Guard win: time reaches 00:00
    // unsigned int guardClientID = -1;

    // for (const auto& pair : playersData) {
    //     if (pair.second->cube.type == CharacterType::CHARACTER_4) {
    //         guardClientID = pair.first;
    //         break;
    //     }
    // }

    // if (ServerLogic::gameStarted && !winAlreadySent) {
    //     using namespace std::chrono;
    //     auto now = steady_clock::now();
    //     int elapsed = duration_cast<seconds>(now - gameStartTime).count();
    //     if (elapsed >= GAME_DURATION_SECONDS) {
    //         winAlreadySent = true;
    //         std::cout << "Security Guard Win (timeout)" << std::endl;

    //         WinPacket packet;
    //         packet.packet_type = WIN_STATE;
    //         packet.winningClientID = -1;  // no one specific wins
    //         packet.winnerType = WinnerType::GUARD;

    //         const unsigned int packet_size = packet.getSize();
    //         std::vector<char> packet_data(packet_size);
    //         packet.serialize(packet_data.data());

    //         network->sendToAll(packet_data.data(), packet_size);
    //     }
    // }


    std::map<unsigned int, NPCState>::iterator npcIter;

    for (npcIter=npcData.begin(); npcIter!=npcData.end(); npcIter++) {
        NPCState npc = npcData[npcIter->first];
        npc.update(ServerLogic::museumAABBs);
        npcData[npcIter->first] = npc;
    }
    artifact.update(false);
    
    // sendGuiUpdate();

    // if (!ServerLogic::gameStarted) {
    // } 
    //     sendStateUpdate();
    // } else {
    //     sendGuiUpdate();
    // }
    if (ServerLogic::gameStarted) {
        sendStateUpdate();

        static auto lastTimeBroadcast = std::chrono::steady_clock::now();
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - lastTimeBroadcast);

        if (elapsed.count() >= 1) {

            lastTimeBroadcast = now;

            TimeUpdate timePacket;
            // std::string current = getCurrentTimeString();  
            // strncpy(timePacket.timeString, current.c_str(), sizeof(timePacket.timeString));
            std::string current = getCurrentTimeString();  
            // std::cout << "current time  " << getCurrentTimeString() << std::endl;

            // whether time end 
            if (current == "00:00" && !winAlreadySent) {
                winAlreadySent = true;

                unsigned int guardClientID = -1;
                for (const auto& pair : playersData) {
                    if (pair.second->cube.type == CharacterType::CHARACTER_4) {
                        guardClientID = pair.first;
                        break;
                    }
                }

                if (guardClientID != -1) {
                    WinPacket packet;
                    packet.packet_type = WIN_STATE;
                    packet.winningClientID = guardClientID;
                    packet.winnerType = WinnerType::GUARD;

                    const unsigned int packet_size = packet.getSize();
                    std::vector<char> packet_data(packet_size);
                    packet.serialize(packet_data.data());
                    network->sendToAll(packet_data.data(), packet_size);

                    printf("Security Guard Wins\n");
                }
            }


            strncpy(timePacket.timeString, current.c_str(), sizeof(timePacket.timeString) - 1); 
            timePacket.timeString[sizeof(timePacket.timeString) - 1] = '\0'; 
            std::cout << "[Server] Sending time: " << timePacket.timeString << std::endl;

            const unsigned int packet_size = timePacket.getSize();
            std::vector<char> packet_data(packet_size);
            timePacket.serialize(packet_data.data());

            network->sendToAll(packet_data.data(), packet_size);
        }

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
    PlayerData* player = playersData[client_id];

    InitPlayerPacket packet;
    packet.packet_type = INIT_PLAYER;

    packet.clientID = client_id;
    packet.currentState = player->currentState;
    packet.altDown = player->altDown;
    packet.radarActive = player->radarActive;

    packet.artifact_id = artifact.id;

    player->cube.saveToPacket(packet);
    player->camera.saveToPacket(packet, false);
    player->camera.saveToPacket(packet, true);

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

std::string ServerGame::getCurrentTimeString() {
    using namespace std::chrono;
    auto now = steady_clock::now();
    int elapsed = duration_cast<seconds>(now - gameStartTime).count();
    int remaining = std::max(GAME_DURATION_SECONDS - elapsed, 0);

    int minutes = remaining / 60;
    int seconds = remaining % 60;

    char buffer[8];
    snprintf(buffer, sizeof(buffer), "%02d:%02d", minutes, seconds);
    return std::string(buffer);
}
