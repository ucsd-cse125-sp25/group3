#include "InitPacket.h"

InitPacket::InitPacket(){
    length = sizeof(character);
}

unsigned int InitPacket::getSize() {
    return sizeof(packet_type) + sizeof(length) + sizeof(character);
}

int InitPacket::serialize(char* data) {
    unsigned int offset = 0;

    std::memcpy(data + offset, &packet_type, sizeof(packet_type));
    offset += sizeof(packet_type);

    std::memcpy(data + offset, &length, sizeof(length));
    offset += sizeof(length);

    std::memcpy(data + offset, &character, sizeof(character));
    offset += sizeof(character);

    return offset;
}

int InitPacket::deserialize(char* data) {
    unsigned int offset = 0;

    std::memcpy(&packet_type, data + offset, sizeof(packet_type));
    offset += sizeof(packet_type);

    std::memcpy(&length, data + offset, sizeof(length));
    offset += sizeof(length);

    std::memcpy(&character, data + offset, sizeof(character));
    offset += sizeof(character);

    return offset;
}