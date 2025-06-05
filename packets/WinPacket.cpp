#include "WinPacket.h"
#include <cstring>  // for memcpy

WinPacket::WinPacket() {
    packet_type = WIN_STATE; 
    length = sizeof(winningClientID);
}

unsigned int WinPacket::getSize() {
    return getHeaderSize() + length;
}

int WinPacket::serializePayload(char* data) {
    unsigned int offset = getHeaderSize();

    std::memcpy(data + offset, &winningClientID, sizeof(winningClientID));
    offset += sizeof(winningClientID);

    return offset;
}

int WinPacket::deserializePayload(char* data) {
    unsigned int offset = getHeaderSize();

    std::memcpy(&winningClientID, data + offset, sizeof(winningClientID));
    offset += sizeof(winningClientID);

    return offset;
}
