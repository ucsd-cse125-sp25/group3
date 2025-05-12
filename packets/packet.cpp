#include "packet.h"
#include <vector>

enum class PacketType : uint8_t {
    ACTION = 1,
    WINDOW = 2,
};

static int overhead = 3;

Packet::Packet(const char* message) {
    uint16_t length = static_cast<uint16_t>(message[0]);
    uint8_t type = static_cast<uint8_t>(message[1]);

    this->type = type;
    this->length = length;
    this->data.resize(length / 2);
    std::memcpy(&this->data[0], &message[overhead], data.size() * sizeof(uint16_t));
}

char* Packet::serialize() {
    char* serializedData = new char[length + overhead];
    memcpy(&serializedData[0], &this->length, sizeof length);
    memcpy(&serializedData[2], &this->type, sizeof type);
    memcpy(&serializedData[3], &this->data[0], data.size() * sizeof(uint16_t));
    return serializedData;
}