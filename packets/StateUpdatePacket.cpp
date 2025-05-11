#include "StateUpdatePacket.h"

StateUpdatePacket::StateUpdatePacket(){
    length = sizeof(base_model) + sizeof(next_model) + sizeof(uint8_t);
}

unsigned int StateUpdatePacket::getSize() {
    return getHeaderSize() + length;
}

int StateUpdatePacket::serializePayload(char* data) {
    unsigned int offset = getHeaderSize();

    std::memcpy(data + offset, &base_model, sizeof(base_model));
    offset += sizeof(base_model);

    std::memcpy(data + offset, &next_model, sizeof(next_model));
    offset += sizeof(next_model);

    uint8_t invisibleByte = isInvisible ? 1 : 0;
    std::memcpy(data + offset, &invisibleByte, sizeof(invisibleByte));
    offset += sizeof(invisibleByte);

    return offset;
}

int StateUpdatePacket::deserializePayload(char* data) {
    unsigned int offset = getHeaderSize();

    std::memcpy(&base_model, data + offset, sizeof(base_model));
    offset += sizeof(base_model);

    std::memcpy(&next_model, data + offset, sizeof(next_model));
    offset += sizeof(next_model);

    uint8_t invisibleByte;
    std::memcpy(&invisibleByte, data + offset, sizeof(invisibleByte));
    isInvisible = (invisibleByte == 1);
    offset += sizeof(invisibleByte);

    return offset;
}