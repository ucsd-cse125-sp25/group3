#include "WinPacket.h"
#include <cstring>  // for memcpy

WinPacket::WinPacket() {
    packet_type = WIN_STATE; 
    // length = sizeof(winningClientID);
    length = sizeof(winningClientID) + sizeof(winnerType);
}

unsigned int WinPacket::getSize() {
    return getHeaderSize() + length;
}

int WinPacket::serializePayload(char* data) {
    unsigned int offset = getHeaderSize();

    std::memcpy(data + offset, &winningClientID, sizeof(winningClientID));
    offset += sizeof(winningClientID);

    unsigned int winnerValue = static_cast<unsigned int>(winnerType);
    std::memcpy(data + offset, &winnerValue, sizeof(winnerValue));
    offset += sizeof(winnerValue);

    return offset;
}

int WinPacket::deserializePayload(char* data) {
    unsigned int offset = getHeaderSize();

    std::memcpy(&winningClientID, data + offset, sizeof(winningClientID));
    offset += sizeof(winningClientID);

    unsigned int winnerValue;
    std::memcpy(&winnerValue, data + offset, sizeof(winnerValue));
    winnerType = static_cast<WinnerType>(winnerValue);
    offset += sizeof(winnerValue);

    return offset;
}
