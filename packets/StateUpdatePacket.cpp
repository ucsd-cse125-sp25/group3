#include "StateUpdatePacket.h"
void StateUpdatePacket::updateLength() {
    length = sizeof(numClients);
    for (const auto& packet : clientPackets) {
        length += packet->getSize();
    }
}

StateUpdatePacket::StateUpdatePacket(){
    length = 0;
    numClients = 0;
}

unsigned int StateUpdatePacket::getSize() {
    updateLength();
    return getHeaderSize() + length;
}

int StateUpdatePacket::serializeHeader(char* data) {
    updateLength();
    return Packet::serializeHeader(data);
}

int StateUpdatePacket::serializePayload(char* data) {
    unsigned int offset = getHeaderSize();

    std::memcpy(data + offset, &numClients, sizeof(numClients));
    offset += sizeof(numClients);

    for (const auto& packet : clientPackets) {
        packet->serializePayload(data + offset);
        offset += packet->getSize();
    }

    return offset;
}

int StateUpdatePacket::deserializePayload(char* data) {
    unsigned int offset = getHeaderSize();

    std::memcpy(&numClients, data + offset, sizeof(numClients));
    offset += sizeof(numClients);

    clientPackets.clear();

    for (unsigned int i = 0; i < numClients; ++i) {
        auto packet = std::make_unique<InitPlayerPacket>();
        packet->deserializePayload(data + offset);
        offset += packet->getSize();

        clientPackets.push_back(std::move(packet));
    }

    updateLength();
    return offset;
}