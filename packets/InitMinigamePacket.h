#include "Packet.h"
#ifndef INITMINIGAMEPACKET_H // include guard
#define INITMINIGAMEPACKET_H 

class InitMinigamePacket : public Packet {
public:
    // minigame platforms - the index of each corresponds to the platform #
    int numPlatforms;
    std::vector<float> platformX;
    std::vector<float> platformY;
    std::vector<float> platformWidth;
    std::vector<float> platformHeight;

    InitMinigamePacket();

    unsigned int getSize() override;

    int serializePayload(char* data) override;

    int deserializePayload(char* data) override;

    void updateLength();

    int serializeHeader(char* data) override;
};

#endif