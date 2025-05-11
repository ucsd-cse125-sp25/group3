#include "Packet.h"

unsigned int Packet::getHeaderSize() {
    return sizeof(packet_type) + sizeof(length);
}

unsigned int Packet::getSize() {
    return getHeaderSize() + payload.size();
}

int Packet::serializeHeader(char * data) {
    unsigned int offset = 0;
    memcpy(data + offset, &packet_type, sizeof(packet_type));
    offset += sizeof(packet_type);

    memcpy(data + offset, &length, sizeof(length));
    offset += sizeof(length);

    if (getSize() > MAX_PACKET_SIZE) throw std::runtime_error("Packet size is greater than maximum size permitted");

    return offset;
}

int Packet::serializePayload(char * data) {
    unsigned int offset = getHeaderSize();
    if (length > 0) {
        std::copy(payload.begin(), payload.end(), data + offset);
        offset += length;
    }

    return offset;
}

int Packet::serialize(char * data) {
    serializeHeader(data);
    return serializePayload(data);
}

int Packet::deserializeHeader(char * data) {
    unsigned int offset = 0;
    memcpy(&packet_type, data, sizeof(packet_type));
    offset += sizeof(packet_type);
    memcpy(&length, data + offset, sizeof(length));
    if (length > MAX_PACKET_SIZE - offset) throw std::runtime_error("Packet size is greater than maximum size permitted");
    return offset;
}

int Packet::deserializePayload(char * data) {
    unsigned int offset = getHeaderSize();
    payload.clear();

    if (length > 0) {
        payload.insert(payload.begin(), data, data + length);
        offset += length;
    }
    return offset;
}

int Packet::deserialize(char * data) {
    deserializeHeader(data);
    return deserializePayload(data);
}