#include "Packet.h"

unsigned int Packet::getSize() {
    return sizeof(packet_type) + sizeof(length) + payload.size();
}

void Packet::serialize(char * data) {
    memcpy(data, &packet_type, sizeof(unsigned int));
    memcpy(&data[4], &length, sizeof(unsigned int));
    if (getSize() > MAX_PACKET_SIZE) {
        throw std::runtime_error("Packet size is greater than maximum size permitted");
    }
    else if (length > 0) {
        std::copy(payload.begin(), payload.end(), &data[8]);
    }
}

void Packet::deserialize(char * data) {
    memcpy(&packet_type, data, sizeof(unsigned int));
    memcpy(&length, &data[4], sizeof(unsigned int));
    if (length > MAX_PACKET_SIZE - 8) {
        throw std::runtime_error("Packet size is greater than maximum size permitted");
    }
    else if (length > 0) {
        payload.insert(payload.begin(), &data[8], &data[8 + length]);
    }
}