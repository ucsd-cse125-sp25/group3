#include "GuiUpdatePacket.h"

GuiUpdatePacket::GuiUpdatePacket(){
    length = sizeof(currentState) + sizeof(availableChars);
}

unsigned int GuiUpdatePacket::getSize() {
    return getHeaderSize() + length;
}

int GuiUpdatePacket::serializePayload(char* data) {
    unsigned int offset = getHeaderSize();

    std::memcpy(data + offset, &currentState, sizeof(currentState));
    offset += sizeof(currentState);

    std::memcpy(data + offset, &availableChars, sizeof(availableChars));
    offset += sizeof(availableChars);

    return offset;
}

int GuiUpdatePacket::deserializePayload(char* data) {
    unsigned int offset = getHeaderSize();

    std::memcpy(&currentState, data + offset, sizeof(currentState));
    offset += sizeof(currentState);

    std::memcpy(&availableChars, data + offset, sizeof(availableChars));
    offset += sizeof(availableChars);

    return offset;
}