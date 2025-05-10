#include "StateUpdatePacket.h"

StateUpdatePacket::StateUpdatePacket(){
    length = sizeof(base_model) + sizeof(next_model) + sizeof(uint8_t);
}

unsigned int StateUpdatePacket::getSize() {
    return sizeof(packet_type) + sizeof(length) + length;
}

int StateUpdatePacket::serialize(char* data) {
    unsigned int offset = 0;

    std::memcpy(data + offset, &packet_type, sizeof(packet_type));
    offset += sizeof(packet_type);

    std::memcpy(data + offset, &length, sizeof(length));
    offset += sizeof(length);

    std::memcpy(data + offset, &base_model, sizeof(base_model));
    offset += sizeof(base_model);

    std::memcpy(data + offset, &next_model, sizeof(next_model));
    offset += sizeof(next_model);

    uint8_t invisibleByte = isInvisible ? 1 : 0;
    std::memcpy(data + offset, &invisibleByte, sizeof(invisibleByte));
    offset += sizeof(invisibleByte);

    return offset;
}

int StateUpdatePacket::deserialize(char* data) {
    unsigned int offset = 0;

    std::memcpy(&packet_type, data + offset, sizeof(packet_type));
    offset += sizeof(packet_type);

    std::memcpy(&length, data + offset, sizeof(length));
    offset += sizeof(length);

    std::memcpy(&base_model, data + offset, sizeof(base_model));
    offset += sizeof(base_model);

    std::memcpy(&next_model, data + offset, sizeof(next_model));
    offset += sizeof(next_model);

    uint8_t invisibleByte;
    std::memcpy(&invisibleByte, data + offset, sizeof(invisibleByte));
    offset += sizeof(invisibleByte);
    isInvisible = (invisibleByte == 1);

    return offset;
}