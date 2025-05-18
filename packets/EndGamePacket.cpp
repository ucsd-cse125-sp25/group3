#include "EndGamePacket.h"

EndGamePacket::EndGamePacket(){
    length = 0;
}

unsigned int EndGamePacket::getSize() {
    return getHeaderSize() + length;
}

int EndGamePacket::serializePayload(char* data) {
    unsigned int offset = getHeaderSize();

    return offset;
}

int EndGamePacket::deserializePayload(char* data) {
    unsigned int offset = getHeaderSize();

    return offset;
}