#include "NPCPacket.h"

NPCPacket::NPCPacket(){
    length = sizeof(npcID) + sizeof(animState) + sizeof(model);
}

unsigned int NPCPacket::getSize() {
    return getHeaderSize() + length;
}

int NPCPacket::serializePayload(char* data) {
    unsigned int offset = getHeaderSize();
    std::memcpy(data + offset, &npcID, sizeof(npcID));
    offset += sizeof(npcID);
    std::memcpy(data + offset, &animState, sizeof(animState));
    offset += sizeof(animState);
    std::memcpy(data + offset, &model, sizeof(model));
    offset += sizeof(model);
    return offset;
}

int NPCPacket::deserializePayload(char* data) {
    unsigned int offset = getHeaderSize();
    std::memcpy(&npcID, data + offset, sizeof(npcID));
    offset += sizeof(npcID);
    std::memcpy(&animState, data + offset, sizeof(animState));
    offset += sizeof(animState);
    std::memcpy(&model, data + offset, sizeof(model));
    offset += sizeof(model);
    return offset;
}