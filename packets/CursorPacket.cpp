#include "CursorPacket.h"

CursorPacket::CursorPacket(){
    length = sizeof(currX) + sizeof(currY);
}

unsigned int CursorPacket::getSize() {
    return getHeaderSize() + length;
}

int CursorPacket::serializePayload(char* data) {
    unsigned int offset = getHeaderSize();

    std::memcpy(data + offset, &currX, sizeof(currX));
    offset += sizeof(currX);

    std::memcpy(data + offset, &currY, sizeof(currY));
    offset += sizeof(currY);

    return offset;
}

int CursorPacket::deserializePayload(char* data) {
    unsigned int offset = getHeaderSize();

    std::memcpy(&currX, data + offset, sizeof(currX));
    offset += sizeof(currX);

    std::memcpy(&currY, data + offset, sizeof(currY));
    offset += sizeof(currY);

    return offset;
}