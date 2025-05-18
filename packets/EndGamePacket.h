#include "Packet.h"
#ifndef ENDGAMEPACKET_H // include guard
#define ENDGAMEPACKET_H

class EndGamePacket : public Packet {
public:
    EndGamePacket();

    unsigned int getSize() override;

    int serializePayload(char* data) override;

    int deserializePayload(char* data) override;
};

#endif