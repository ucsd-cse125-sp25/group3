#include <vector>
#include "Packet.h"
#ifndef STATEUPDATEPACKET_H // include guard
#define STATEUPDATEPACKET_H

class StateUpdatePacket : public Packet {
public:
    float model[4][4];
    bool isInvisible;

    StateUpdatePacket();

    unsigned int getSize() override;

    int serializePayload(char* data) override;

    int deserializePayload(char* data) override;
};

#endif