#include "Packet.h"
#ifndef MINIGAMECHARACTERPACKET_H // include guard
#define MINIGAMECHARACTERPACKET_H

class MinigameCharacterPacket : public Packet {
public:
    float x;
    float y;
    bool facingRight;
    bool isFinished;

    MinigameCharacterPacket();

    unsigned int getSize() override;

    int serializePayload(char* data) override;

    int deserializePayload(char* data) override;
};

#endif