#include <vector>
#ifndef PACKET_H // include guard
#define PACKET_H

// Packet structure will be length (2 bytes), type (1 byte), data (N bytes) 
class Packet {
    uint8_t type;
    public:
        uint16_t length;     // Length of payload
        std::vector<uint16_t> data; // Payload
        Packet(const char* message);
        char* serialize();
};

#endif