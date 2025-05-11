#include "Packet.h"
#ifndef INITPACKET_H // include guard
#define INITPACKET_H

enum CharacterType {
    CHARACTER_1,
    CHARACTER_2,
    CHARACTER_3,
    CHARACTER_4
};

class InitPacket : public Packet {
public:
    CharacterType character;

    InitPacket();

    unsigned int getSize() override;

    int serializePayload(char* data) override;

    int deserializePayload(char* data) override;
};

#endif