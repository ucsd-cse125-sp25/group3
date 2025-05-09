#include "Packet.h"

unsigned int Packet::getHeaderSize() {
    return sizeof(packet_type) + sizeof(length);
}

unsigned int Packet::getSize() {
    return sizeof(packet_type) + sizeof(length) + payload.size();
}

int Packet::serialize(char * data) {
    unsigned int offset = 0;
    memcpy(data + offset, &packet_type, sizeof(packet_type));
    offset += sizeof(packet_type);

    memcpy(data + offset, &length, sizeof(length));
    offset += sizeof(length);

    if (getSize() > MAX_PACKET_SIZE) {
        throw std::runtime_error("Packet size is greater than maximum size permitted");
    }
    else if (length > 0) {
        std::copy(payload.begin(), payload.end(), data + offset);
        offset += length;
    }
    return offset;
}

int Packet::deserialize(char * data) {
    unsigned int offset = 0;
    memcpy(&packet_type, data, sizeof(packet_type));
    offset += sizeof(packet_type);
    memcpy(&length, data + offset, sizeof(length));
    offset += sizeof(length);
    if (length > MAX_PACKET_SIZE - offset) {
        throw std::runtime_error("Packet size is greater than maximum size permitted");
    }
    payload.clear();
    if (length > 0) {
        payload.insert(payload.begin(), data + offset, data + offset + length);
        offset += length;
    }
    return offset;
}

int Packet::deserializeHeader(char * header) {
    unsigned int offset = 0;
    memcpy(&packet_type, header, sizeof(packet_type));
    offset += sizeof(packet_type);
    memcpy(&length, header + offset, sizeof(length));
    return offset;
}

int Packet::deserializePayload(char * data) {
    unsigned int offset = 0;
    payload.clear();

    if (length > 0) {
        payload.insert(payload.begin(), data, data + length);
        offset += length;
    }
    return offset;
}