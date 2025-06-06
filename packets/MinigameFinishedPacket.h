#include "Packet.h"
#ifndef MINIGAMEFINISHEDPACKET_H // include guard
#define MINIGAMEFINISHEDPACKET_H

class MinigameFinishedPacket : public Packet {
public:
    unsigned int clientID;

    MinigameFinishedPacket();

    unsigned int getSize() override;

    int serializePayload(char* data) override;

    int deserializePayload(char* data) override;
};

#endif