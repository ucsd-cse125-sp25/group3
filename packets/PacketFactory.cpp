#include "PacketFactory.h"

std::unique_ptr<Packet> PacketFactory::createFromBuffer(char* data) {
    Packet temp;
    temp.deserializeHeader(data);

    std::unique_ptr<Packet> packet = nullptr;

    switch (temp.packet_type) {
        case INIT_CONNECTION:
            packet = std::make_unique<InitPacket>();
            break;
        case END_GAME:
            packet = std::make_unique<EndGamePacket>();
            break;
        case WIN_STATE:
            packet = std::make_unique<WinPacket>();
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
        case INIT_PLAYER:
            packet = std::make_unique<InitPlayerPacket>();
            break;
        case NPC_STATE:
            packet = std::make_unique<NPCPacket>();
            break;
        case GUI_UPDATE:
            packet = std::make_unique<GuiUpdatePacket>();
            break;
        case TIME_UPDATE:
            // std::cout << "in case time_update" << std::endl;
            packet = std::make_unique<TimeUpdate>();
            break;
        default:
            printf("Unknown packet type passed to PacketFactory: %d \n", packet->packet_type);
            return nullptr;
    }
    packet->deserialize(data);

    return packet;
}