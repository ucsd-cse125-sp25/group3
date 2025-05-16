#include "PacketFactory.h"

std::unique_ptr<Packet> PacketFactory::createFromBuffer(char* data) {
    Packet temp;
    temp.deserializeHeader(data);

    std::unique_ptr<Packet> packet = nullptr;

    switch (temp.packet_type) {
        case INIT_CONNECTION:
            packet = std::make_unique<InitPacket>();
            break;
        case KEY_INPUT:
            packet = std::make_unique<KeyPacket>();
            break;
        case STATE_UPDATE:
            packet = std::make_unique<StateUpdatePacket>();
            break;
        case CURSOR_EVENT:
            packet = std::make_unique<CursorPacket>();
            break;
        default:
            printf("Unknown packet type passed to PacketFactory: %d \n", packet->packet_type);
            return nullptr;
    }

    packet->deserialize(data);

    return packet;
}