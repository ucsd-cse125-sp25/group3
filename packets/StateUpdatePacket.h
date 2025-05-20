#include <vector>
#include "Packet.h"
#include "InitPlayerPacket.h"
#ifndef STATEUPDATEPACKET_H // include guard
#define STATEUPDATEPACKET_H

class StateUpdatePacket : public Packet {
public:
    unsigned int numClients;
    std::vector<std::unique_ptr<InitPlayerPacket>> clientPackets;

    StateUpdatePacket();

    unsigned int getSize() override;

    int serializePayload(char* data) override;

    int deserializePayload(char* data) override;

    void updateLength();

    int serializeHeader(char* data) override;
};

#endif