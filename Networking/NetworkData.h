#include <string>

#define MAX_PACKET_SIZE 1000000
#define MSSG_SIZE 100
#ifndef NETWORKDATA_H
#define NETWORKDATA_H

enum PacketTypes {

    INIT_CONNECTION = 0,

    ACTION_EVENT = 1,

    ECHO_EVENT = 2,

}; 

struct Packet {

    unsigned int packet_type;
    char message[MSSG_SIZE];

    void serialize(char * data) {
        memcpy(data, &packet_type, sizeof(unsigned int));
        memcpy(&data[4], message, sizeof message);
        
    }

    void deserialize(char * data) {
        memcpy(&packet_type, data, sizeof(unsigned int));
        
       
        memcpy(message, &data[4], sizeof message);
        
    }
};

#endif