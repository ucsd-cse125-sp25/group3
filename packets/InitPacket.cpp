#include "InitPacket.h"

InitPacket::InitPacket(){
    length = sizeof(character) + sizeof(windowWidth) + sizeof(windowHeight);
}

unsigned int InitPacket::getSize() {
    return getHeaderSize() + length;
}

int InitPacket::serializePayload(char* data) {
    unsigned int offset = getHeaderSize();

    std::memcpy(data + offset, &character, sizeof(character));
    offset += sizeof(character);

    std::memcpy(data + offset, &windowWidth, sizeof(windowWidth));
    offset += sizeof(windowWidth);

    std::memcpy(data + offset, &windowHeight, sizeof(windowHeight));
    offset += sizeof(windowHeight);

    return offset;
}

int InitPacket::deserializePayload(char* data) {
    unsigned int offset = getHeaderSize();

    std::memcpy(&character, data + offset, sizeof(character));
    offset += sizeof(character);

    std::memcpy(&windowWidth, data + offset, sizeof(windowWidth));
    offset += sizeof(windowWidth);

    std::memcpy(&windowHeight, data + offset, sizeof(windowHeight));
    offset += sizeof(windowHeight);

    return offset;
}