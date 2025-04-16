
#include "ServerGame.h"
#include <iostream>
#include <cstdio>
#include <chrono>
#include <thread>
#include <cassert>
#define TICK 30 //in ms

unsigned int ServerGame::client_id; 

ServerGame::ServerGame(void)
{
    // id's to assign clients for our table
    client_id = 0;

    // set up the server network to listen 
    network = new ServerNetwork(); 
}

void ServerGame::update()
{
    // get new clients
   if(network->acceptNewClient(client_id))
   {
        printf("client %d has been connected to the server\n",client_id);

        client_id++;
   }

   receiveFromClients();
}

void ServerGame::receiveFromClients()
{
    auto start = std::chrono::steady_clock::now();
    Packet packet;

    // go through all clients
    // std::map<unsigned int, SOCKET>::iterator iter;
    std::map<unsigned int, int>::iterator iter;

    for(iter = network->sessions.begin(); iter != network->sessions.end(); iter++)
    {
        int data_length = network->receiveData(iter->first, network_data);

        if (data_length <= 0) 
        {
            //no data recieved
            continue;
        }

        int i = 0;
        while (i < (unsigned int)data_length) 
        {
            
            packet.deserialize(&(network_data[i]));
            i += sizeof(Packet);

            switch (packet.packet_type) {

                case INIT_CONNECTION:

                    printf("server received init packet from client\n");

                    // sendActionPackets();

                    break;

                case ACTION_EVENT:

                    printf("server received action event packet from client\n");

                    sendActionPackets();

                    break;

                case ECHO_EVENT:
                    printf("server recieved echo event packet from client\n");
                    printf("Server recieved: %s\n", packet.message);
                    sendEchoPackets(std::string(packet.message));
                    break;
                default:

                    printf("error in packet types\n");

                    break;
            }
        }
    }
    auto orig_diff = std::chrono::steady_clock::now() - start;
    auto milli_diff = std::chrono::duration_cast<std::chrono::milliseconds>(orig_diff);
    auto wait = std::chrono::milliseconds(TICK) - milli_diff;
    assert(wait.count() >= 0);
    std::this_thread::sleep_for(wait);
    
    // std::cout << "diff: " << milli_diff.count() << "\n";
    
    // printf("wait: %d\n", wait.count());
    
    // std::cout << "diff: " << d.count() << "\n";
    //assert(std::chrono::system_clock::now() - std::chrono::milliseconds(TICK) == start && "Exceeded server tick");
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
    memcpy(packet.message, response.data(), sizeof response);
    packet.serialize(packet_data);
    network->sendToAll(packet_data,packet_size);
}