#include "MinigameCharacterPacket.h"

MinigameCharacterPacket::MinigameCharacterPacket(){
    length = sizeof(x) + sizeof(y) + sizeof(uint8_t) + sizeof(uint8_t);
}

unsigned int MinigameCharacterPacket::getSize() {
    return getHeaderSize() + length;
}

int MinigameCharacterPacket::serializePayload(char* data) {
    unsigned int offset = getHeaderSize();

    std::memcpy(data + offset, &x, sizeof(x));
    offset += sizeof(x);

    std::memcpy(data + offset, &y, sizeof(y));
    offset += sizeof(y);

    uint8_t facingRightByte = facingRight ? 1 : 0;
    std::memcpy(data + offset, &facingRightByte, sizeof(facingRightByte));
    offset += sizeof(facingRightByte);

    uint8_t isFinishedByte = isFinished ? 1 : 0;
    std::memcpy(data + offset, &isFinishedByte, sizeof(isFinishedByte));
    offset += sizeof(isFinishedByte);

    return offset;
}

int MinigameCharacterPacket::deserializePayload(char* data) {
    unsigned int offset = getHeaderSize();

    std::memcpy(&x, data + offset, sizeof(x));
    offset += sizeof(x);

    std::memcpy(&y, data + offset, sizeof(y));
    offset += sizeof(y);

    uint8_t facingRightByte;
    std::memcpy(&facingRightByte, data + offset, sizeof(facingRightByte));
    facingRight = (facingRightByte == 1);
    offset += sizeof(facingRightByte);

    uint8_t isFinishedByte;
    std::memcpy(&isFinishedByte, data + offset, sizeof(isFinishedByte));
    isFinished = (isFinishedByte == 1);
    offset += sizeof(isFinishedByte);

    return offset;
}