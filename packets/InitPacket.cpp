#include "InitPacket.h"

InitPacket::InitPacket(){
    length = sizeof(character);
}

unsigned int InitPacket::getSize() {
    return getHeaderSize() + length;
}

int InitPacket::serializePayload(char* data) {
    unsigned int offset = getHeaderSize();

    std::memcpy(data + offset, &character, sizeof(character));
    offset += sizeof(character);

    return offset;
}

int InitPacket::deserializePayload(char* data) {
    unsigned int offset = getHeaderSize();

    std::memcpy(&character, data + offset, sizeof(character));
    offset += sizeof(character);

    return offset;
}