#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>
#ifndef PACKET_H // include guard
#define PACKET_H

#define MAX_PACKET_SIZE 1000000
#define MSSG_SIZE 100

enum PacketTypes {

    INIT_CONNECTION = 0,

    ACTION_EVENT = 1,

    ECHO_EVENT = 2,

    KEY_INPUT = 3,

    STATE_UPDATE = 4,

    END_GAME = 5,

    CURSOR_EVENT = 6,

    INIT_PLAYER = 7,
}; 

// Packet structure will be length (4 bytes), type (4 bytes), data (N bytes) 
class Packet {
    public:
        unsigned int packet_type; // type that maps to PacketType
        unsigned int length; // size of the payload in bytes
        std::vector<char> payload;
        virtual unsigned int getSize();
        static unsigned int getHeaderSize();
        virtual int serialize(char * data);
        virtual int deserialize(char * data);
        virtual int deserializeHeader(char * header);
        virtual int deserializePayload(char * data);
};

#endif