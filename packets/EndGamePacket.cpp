#include "EndGamePacket.h"

EndGamePacket::EndGamePacket(){
    length = sizeof(closedClient);
}

unsigned int EndGamePacket::getSize() {
    return getHeaderSize() + length;
}

int EndGamePacket::serializePayload(char* data) {
    unsigned int offset = getHeaderSize();

    std::memcpy(data + offset, &closedClient, sizeof(closedClient));
    offset += sizeof(closedClient);

    return offset;
}

int EndGamePacket::deserializePayload(char* data) {
    unsigned int offset = getHeaderSize();

    std::memcpy(&closedClient, data + offset, sizeof(closedClient));
    offset += sizeof(closedClient);

    return offset;
}