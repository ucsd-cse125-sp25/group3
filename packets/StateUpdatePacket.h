#include <vector>
#include "Packet.h"
#ifndef STATEUPDATEPACKET_H // include guard
#define STATEUPDATEPACKET_H

class StateUpdatePacket : public Packet {
public:
    float base_model[4][4];
    float next_model[4][4];
    bool isInvisible;

    StateUpdatePacket();

    unsigned int getSize() override;

    int serialize(char* data) override;

    int deserialize(char* data) override;
};

#endif