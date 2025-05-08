#include "KeyPacket.h"

KeyPacket::KeyPacket(){
    length = sizeof(key_type) + sizeof(tick) + sizeof(is_down);
}

unsigned int KeyPacket::getSize() {
    return sizeof(packet_type) + sizeof(length) + sizeof(tick) + sizeof(key_type) + sizeof(is_down);
}

int KeyPacket::serialize(char* data) {
    unsigned int offset = 0;

    std::memcpy(data + offset, &length, sizeof(length));
    offset += sizeof(length);

    std::memcpy(data + offset, &packet_type, sizeof(packet_type));
    offset += sizeof(packet_type);

    std::memcpy(data + offset, &tick, sizeof(tick));
    offset += sizeof(tick);

    std::memcpy(data + offset, &key_type, sizeof(key_type));
    offset += sizeof(key_type);

    std::memcpy(data + offset, &is_down, sizeof(is_down));
    offset += sizeof(is_down);
    return offset;
}

int KeyPacket::deserialize(char* data) {
    unsigned int offset = 0;

    std::memcpy(&length, data + offset, sizeof(length));
    offset += sizeof(length);

    std::memcpy(&packet_type, data + offset, sizeof(packet_type));
    offset += sizeof(packet_type);

    std::memcpy(&tick, data + offset, sizeof(tick));
    offset += sizeof(tick);

    std::memcpy(&key_type, data + offset, sizeof(key_type));
    offset += sizeof(key_type);

    std::memcpy(&is_down, data + offset, sizeof(is_down));
    offset += sizeof(is_down);
    return offset;
}