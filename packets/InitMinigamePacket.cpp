#include "InitMinigamePacket.h"
void InitMinigamePacket::updateLength() {
    length = sizeof(numPlatforms) + (numPlatforms * sizeof(float) * 4);
}

InitMinigamePacket::InitMinigamePacket(){
    length = sizeof(numPlatforms);
    numPlatforms = 0;
}

unsigned int InitMinigamePacket::getSize() {
    updateLength();
    return getHeaderSize() + length;
}

int InitMinigamePacket::serializeHeader(char* data) {
    updateLength();
    return Packet::serializeHeader(data);
}

int InitMinigamePacket::serializePayload(char* data) {
    unsigned int offset = getHeaderSize();

    std::memcpy(data + offset, &numPlatforms, sizeof(numPlatforms));
    offset += sizeof(numPlatforms);

    if(platformX.size() != platformY.size() || platformX.size() != platformWidth.size() || platformX.size() != platformHeight.size()) throw std::runtime_error("platforms malfromed; must have x,y, width, and height for each platform");

    for (unsigned int i = 0; i < numPlatforms; ++i) {
        std::memcpy(data + offset, &platformX[i], sizeof(platformX[i]));
        offset += sizeof(platformX[i]);

        std::memcpy(data + offset, &platformY[i], sizeof(platformY[i]));
        offset += sizeof(platformY[i]);

        std::memcpy(data + offset, &platformWidth[i], sizeof(platformWidth[i]));
        offset += sizeof(platformWidth[i]);

        std::memcpy(data + offset, &platformHeight[i], sizeof(platformHeight[i]));
        offset += sizeof(platformHeight[i]);
    }

    return offset;
}

int InitMinigamePacket::deserializePayload(char* data) {
    unsigned int offset = getHeaderSize();

    std::memcpy(&numPlatforms, data + offset, sizeof(numPlatforms));
    offset += sizeof(numPlatforms);

    for (unsigned int i = 0; i < numPlatforms; ++i) {
        float nextX = 0;
        std::memcpy(&nextX, data + offset, sizeof(nextX));
        platformX.push_back(std::move(nextX));
        offset += sizeof(nextX);

        float nextY = 0;
        std::memcpy(&nextY, data + offset, sizeof(nextY));
        platformY.push_back(std::move(nextY));
        offset += sizeof(nextY);

        float nextWidth = 0;
        std::memcpy(&nextWidth, data + offset, sizeof(nextWidth));
        platformWidth.push_back(std::move(nextWidth));
        offset += sizeof(nextWidth);

        float nextHeight = 0;
        std::memcpy(&nextHeight, data + offset, sizeof(nextHeight));
        platformHeight.push_back(std::move(nextHeight));
        offset += sizeof(nextHeight);
    }

    updateLength();
    return offset;
}
