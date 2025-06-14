#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <memory>

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

    NPC_STATE = 8,

    GUI_UPDATE = 9,

    WIN_STATE = 11,

    TIME_UPDATE = 12, 

    INIT_MINIGAME = 13,

    MINIGAME_FINISHED = 14

}; 

// Packet structure will be type (4 bytes), length (4 bytes), data (N bytes) 
class Packet {
    public:
        unsigned int packet_type; // type that maps to PacketType
        unsigned int length; // size of the payload in bytes
        std::vector<char> payload;
        static unsigned int getHeaderSize();
        virtual unsigned int getSize();
        virtual int serializeHeader(char * data);
        virtual int serializePayload(char * data);
        int serialize(char * data);
        virtual int deserializeHeader(char * data);
        virtual int deserializePayload(char * data);
        int deserialize(char * data);
};

#endif
