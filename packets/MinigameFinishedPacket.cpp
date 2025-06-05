#include "MinigameFinishedPacket.h"

MinigameFinishedPacket::MinigameFinishedPacket(){
    length = sizeof(clientID);
}

unsigned int MinigameFinishedPacket::getSize() {
    return getHeaderSize() + length;
}

int MinigameFinishedPacket::serializePayload(char* data) {
    unsigned int offset = getHeaderSize();

    std::memcpy(data + offset, &clientID, sizeof(clientID));
    offset += sizeof(clientID);

    return offset;
}

int MinigameFinishedPacket::deserializePayload(char* data) {
    unsigned int offset = getHeaderSize();

    std::memcpy(&clientID, data + offset, sizeof(clientID));
    offset += sizeof(clientID);

    return offset;
}