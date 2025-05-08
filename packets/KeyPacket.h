#include "Packet.h"
#ifndef KEYPACKET_H // include guard
#define KEYPACKET_H

enum KeyType {
    NONE = 0,
    KEY_W = 1,
    KEY_S = 2,
    KEY_D = 3,
    KEY_A = 4,
    KEY_T = 5,
    KEY_G = 6,
    KEY_K = 7,
    KEY_L = 8,
    KEY_E = 9,
    KEY_SPACE = 10,
};

class KeyPacket : public Packet {
public:
    KeyType key_type; 

    KeyPacket();

    unsigned int getSize() override;

    int serialize(char* data) override;

    int deserialize(char* data) override;
};

#endif