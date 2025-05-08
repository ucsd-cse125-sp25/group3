#include "PacketFactory.h"

std::unique_ptr<Packet> PacketFactory::createFromBuffer(char* data) {
    unsigned int length;
    unsigned int type;

    unsigned int offset = 0;
    memcpy(&length, data + offset, sizeof(length));
    offset += sizeof(length);

    memcpy(&type, data + offset, sizeof(type));
    offset += sizeof(type);

    std::unique_ptr<Packet> packet = nullptr;

    switch (type) {
        /* case INIT_CONNECTION:
            packet = std::make_unique<InitPacket>();
            break; */
        case KEY_EVENT:
            packet = std::make_unique<KeyPacket>();
            break;
        default:
            printf("Unknown packet type passed to PacketFactory: %d \n", type);
            return nullptr;
    }

    packet->deserialize(data);

    return packet;
}