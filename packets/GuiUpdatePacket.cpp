#include "GuiUpdatePacket.h"

GuiUpdatePacket::GuiUpdatePacket(){
    length = sizeof(currentState);
}

unsigned int GuiUpdatePacket::getSize() {
    return getHeaderSize() + length;
}

int GuiUpdatePacket::serializePayload(char* data) {
    unsigned int offset = getHeaderSize();

    std::memcpy(data + offset, &currentState, sizeof(currentState));
    offset += sizeof(currentState);

    

    return offset;
}

int GuiUpdatePacket::deserializePayload(char* data) {
    unsigned int offset = getHeaderSize();

    std::memcpy(&currentState, data + offset, sizeof(currentState));
    offset += sizeof(currentState);

    return offset;
}