#include "Packet.h"
#ifndef NPCPACKET_H // include guard
#define NPCPACKET_H

class NPCPacket : public Packet {
public:
    unsigned int npcID;
    float model[4][4];
    NPCPacket();
    unsigned int getSize() override;
    int serializePayload(char* data) override;
    int deserializePayload(char* data) override;
};

#endif