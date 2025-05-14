#include "StateUpdatePacket.h"

StateUpdatePacket::StateUpdatePacket(){
    length = sizeof(model) + sizeof(uint8_t);
}

unsigned int StateUpdatePacket::getSize() {
    return getHeaderSize() + length;
}

int StateUpdatePacket::serializePayload(char* data) {
    unsigned int offset = getHeaderSize();

    std::memcpy(data + offset, &model, sizeof(model));
    offset += sizeof(model);

    uint8_t invisibleByte = isInvisible ? 1 : 0;
    std::memcpy(data + offset, &invisibleByte, sizeof(invisibleByte));
    offset += sizeof(invisibleByte);

    return offset;
}

int StateUpdatePacket::deserializePayload(char* data) {
    unsigned int offset = getHeaderSize();

    std::memcpy(&model, data + offset, sizeof(model));
    offset += sizeof(model);

    uint8_t invisibleByte;
    std::memcpy(&invisibleByte, data + offset, sizeof(invisibleByte));
    isInvisible = (invisibleByte == 1);
    offset += sizeof(invisibleByte);

    return offset;
}