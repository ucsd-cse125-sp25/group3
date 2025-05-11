#include "KeyPacket.h"

KeyPacket::KeyPacket(){
    length = sizeof(key_type);
}

unsigned int KeyPacket::getSize() {
    return getHeaderSize() + length;
}

int KeyPacket::serializePayload(char* data) {
    unsigned int offset = getHeaderSize();

    std::memcpy(data + offset, &key_type, sizeof(key_type));
    offset += sizeof(key_type);

    return offset;
}

int KeyPacket::deserializePayload(char* data) {
    unsigned int offset = getHeaderSize();

    std::memcpy(&key_type, data + offset, sizeof(key_type));
    offset += sizeof(key_type);

    return offset;
}