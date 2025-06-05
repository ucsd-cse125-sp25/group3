#include "StateUpdatePacket.h"
void StateUpdatePacket::updateLength() {
    length = sizeof(numClients);
    
    for (const auto& packet : clientPackets) {
        length += packet->getSize();
    }
    length += sizeof(numNPCs);

    for (const auto& packet : npcPackets) {
        length += packet->getSize();
    }
    length += sizeof(artifactModel);

    length += sizeof(artifact_state);
}

StateUpdatePacket::StateUpdatePacket(){
    length = 0;
    numClients = 0;
    numNPCs = 0;
    artifact_state = 0;
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
        packet->serialize(data + offset);
        offset += packet->getSize();
    }

    std::memcpy(data + offset, &numNPCs, sizeof(numNPCs));
    offset += sizeof(numNPCs);

    for (const auto& packet : npcPackets) {
        packet->serialize(data + offset);
        offset += packet->getSize();
    }

    std::memcpy(data + offset, &artifactModel, sizeof(artifactModel));
    offset += sizeof(artifactModel);

    std::memcpy(data + offset, &artifact_state, sizeof(artifact_state));
    offset += sizeof(artifact_state);

    return offset;
}

int StateUpdatePacket::deserializePayload(char* data) {
    unsigned int offset = getHeaderSize();

    std::memcpy(&numClients, data + offset, sizeof(numClients));
    offset += sizeof(numClients);

    clientPackets.clear();

    for (unsigned int i = 0; i < numClients; ++i) {
        auto packet = std::make_unique<InitPlayerPacket>();
        packet->deserialize(data + offset);
        offset += packet->getSize();

        clientPackets.push_back(std::move(packet));
    }
    std::memcpy(&numNPCs, data + offset, sizeof(numNPCs));
    offset += sizeof(numNPCs);
    npcPackets.clear();

    for (unsigned int i = 0; i < numNPCs; ++i) {
        auto packet = std::make_unique<NPCPacket>();
        packet->deserialize(data + offset);
        offset += packet->getSize();
        npcPackets.push_back(std::move(packet));
    }
    std::memcpy(&artifactModel, data + offset, sizeof(artifactModel));
    offset += sizeof(artifactModel);

    std::memcpy(&artifact_state, data + offset, sizeof(artifact_state));
    offset += sizeof(artifact_state);

    updateLength();
    return offset;
}