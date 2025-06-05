#include "TimeUpdate.h"

TimeUpdate::TimeUpdate() {
    packet_type = TIME_UPDATE;
    memset(timeString, 0, sizeof(timeString));
    length = sizeof(timeString); 
}

int TimeUpdate::serializePayload(char* data) {
    memcpy(data + Packet::getHeaderSize(), timeString, sizeof(timeString));
    return getSize(); 
}

int TimeUpdate::deserializePayload(char* data){
    memcpy(timeString, data + Packet::getHeaderSize(), sizeof(timeString));

    // std::cout << "[Client deserialize] Raw string from packet: '" << timeString << "'" << std::endl;
    return getSize();
}

unsigned int TimeUpdate::getSize() {
    return Packet::getHeaderSize() + sizeof(timeString);
}
