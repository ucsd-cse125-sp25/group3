#include <string>
#include <vector>
#include <algorithm>
#define MAX_PACKET_SIZE 1000000
#define MSSG_SIZE 100
#ifndef NETWORKDATA_H
#define NETWORKDATA_H

enum PacketTypes {

    INIT_CONNECTION = 0,

    ACTION_EVENT = 1,

    ECHO_EVENT = 2,

    KEY_EVENT = 3,

}; 

struct Packet {

    unsigned int packet_type;
    unsigned int length;
    // char message[MSSG_SIZE];
    std::vector<char> payload;

    unsigned int getSize() {
        return sizeof(packet_type) + sizeof(length) + payload.size();
    }

    void serialize(char * data) {
        memcpy(data, &packet_type, sizeof(unsigned int));
        memcpy(&data[4], &length, sizeof(unsigned int));
        // memcpy(&data[8], message, payload.size()); 
        if (length > 0)
        std::copy(payload.begin(), payload.end(), &data[8]);
    }

    void deserialize(char * data) {
        memcpy(&packet_type, data, sizeof(unsigned int));
        memcpy(&length, &data[4], sizeof(unsigned int));
        if (length > 0)
        payload.insert(payload.begin(), &data[8], &data[8 + length]);
        // memcpy(message, &data[8], sizeof message);
        
    }
};

#endif