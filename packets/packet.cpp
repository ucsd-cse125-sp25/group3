#include "Packet.h"

unsigned int Packet::getSize() {
    return sizeof(packet_type) + sizeof(length) + payload.size();
}

void Packet::serialize(char * data) {
    memcpy(data, &packet_type, sizeof(unsigned int));
    memcpy(&data[4], &length, sizeof(unsigned int));
    // memcpy(&data[8], message, payload.size()); 
    if (length > 0)
    std::copy(payload.begin(), payload.end(), &data[8]);
}

void Packet::deserialize(char * data) {
    memcpy(&packet_type, data, sizeof(unsigned int));
    memcpy(&length, &data[4], sizeof(unsigned int));
    if (length > 0)
    payload.insert(payload.begin(), &data[8], &data[8 + length]);
    // memcpy(message, &data[8], sizeof message);  
}