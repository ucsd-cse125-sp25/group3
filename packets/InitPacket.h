#include "Packet.h"
#ifndef INITPACKET_H // include guard
#define INITPACKET_H

enum CharacterType {
    CHARACTER_1,
    CHARACTER_2,
    CHARACTER_3,
    CHARACTER_4,
    NONE,
};

class InitPacket : public Packet {
public:
    CharacterType character;

    // window
    int windowWidth;
    int windowHeight;

    InitPacket();

    unsigned int getSize() override;

    int serializePayload(char* data) override;

    int deserializePayload(char* data) override;
};

#endif