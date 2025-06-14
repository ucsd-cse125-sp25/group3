#include "InitPlayerPacket.h"

InitPlayerPacket::InitPlayerPacket(){
    length = sizeof(artifact_id) + sizeof(clientID) + sizeof(currentState) + sizeof(type) + sizeof(animState) + sizeof(uint8_t) + sizeof(uint8_t) + sizeof(model) + sizeof(viewProjectMtx) + sizeof(eye) + sizeof(center) + sizeof(azimuth) + sizeof(incline) + sizeof(aspect) + sizeof(miniViewProjectMtx) + sizeof(miniEye) + sizeof(miniCenter) + sizeof(miniAzimuth) + sizeof(miniIncline) + sizeof(miniAspect) + sizeof(uint8_t);
}

unsigned int InitPlayerPacket::getSize() {
    return getHeaderSize() + length;
}

int InitPlayerPacket::serializePayload(char* data) {
    unsigned int offset = getHeaderSize();

    std::memcpy(data + offset, &artifact_id, sizeof(artifact_id));
    offset += sizeof(artifact_id);

    std::memcpy(data + offset, &clientID, sizeof(clientID));
    offset += sizeof(clientID);

    std::memcpy(data + offset, &currentState, sizeof(currentState));
    offset += sizeof(currentState);

    std::memcpy(data + offset, &type, sizeof(type));
    offset += sizeof(type);

    std::memcpy(data + offset, &animState, sizeof(animState));
    offset += sizeof(animState);

    uint8_t altDownByte = altDown ? 1 : 0;
    std::memcpy(data + offset, &altDownByte, sizeof(altDownByte));
    offset += sizeof(altDownByte);

    uint8_t radarActiveByte = radarActive ? 1 : 0;
    std::memcpy(data + offset, &radarActiveByte, sizeof(radarActiveByte));
    offset += sizeof(radarActiveByte);

    std::memcpy(data + offset, &model, sizeof(model));
    offset += sizeof(model);

    uint8_t invisibleByte = isInvisible ? 1 : 0;
    std::memcpy(data + offset, &invisibleByte, sizeof(invisibleByte));
    offset += sizeof(invisibleByte);

    std::memcpy(data + offset, &viewProjectMtx, sizeof(viewProjectMtx));
    offset += sizeof(viewProjectMtx);

    std::memcpy(data + offset, &eye, sizeof(eye));
    offset += sizeof(eye);

    std::memcpy(data + offset, &center, sizeof(center));
    offset += sizeof(center);

    std::memcpy(data + offset, &azimuth, sizeof(azimuth));
    offset += sizeof(azimuth);

    std::memcpy(data + offset, &incline, sizeof(incline));
    offset += sizeof(incline);

    std::memcpy(data + offset, &aspect, sizeof(aspect));
    offset += sizeof(aspect);

    std::memcpy(data + offset, &miniViewProjectMtx, sizeof(miniViewProjectMtx));
    offset += sizeof(miniViewProjectMtx);

    std::memcpy(data + offset, &miniEye, sizeof(miniEye));
    offset += sizeof(miniEye);

    std::memcpy(data + offset, &miniCenter, sizeof(miniCenter));
    offset += sizeof(miniCenter);

    std::memcpy(data + offset, &miniAzimuth, sizeof(miniAzimuth));
    offset += sizeof(miniAzimuth);

    std::memcpy(data + offset, &miniIncline, sizeof(miniIncline));
    offset += sizeof(miniIncline);

    std::memcpy(data + offset, &miniAspect, sizeof(miniAspect));
    offset += sizeof(miniAspect);

    return offset;
}

int InitPlayerPacket::deserializePayload(char* data) {
    unsigned int offset = getHeaderSize();

    std::memcpy(&artifact_id, data + offset, sizeof(artifact_id));
    offset += sizeof(artifact_id);

    std::memcpy(&clientID, data + offset, sizeof(clientID));
    offset += sizeof(clientID);

    std::memcpy(&currentState, data + offset, sizeof(currentState));
    offset += sizeof(currentState);

    std::memcpy(&type, data + offset, sizeof(type));
    offset += sizeof(type);

    std::memcpy(&animState, data + offset, sizeof(animState));
    offset += sizeof(animState);

    uint8_t altDownByte;
    std::memcpy(&altDownByte, data + offset, sizeof(altDownByte));
    altDown = (altDownByte == 1);
    offset += sizeof(altDownByte);

    uint8_t radarActiveByte;
    std::memcpy(&radarActiveByte, data + offset, sizeof(radarActiveByte));
    radarActive = (radarActiveByte == 1);
    offset += sizeof(radarActiveByte);

    std::memcpy(&model, data + offset, sizeof(model));
    offset += sizeof(model);

    uint8_t invisibleByte;
    std::memcpy(&invisibleByte, data + offset, sizeof(invisibleByte));
    isInvisible = (invisibleByte == 1);
    offset += sizeof(invisibleByte);

    std::memcpy(&viewProjectMtx, data + offset, sizeof(viewProjectMtx));
    offset += sizeof(viewProjectMtx);

    std::memcpy(&eye, data + offset, sizeof(eye));
    offset += sizeof(eye);

    std::memcpy(&center, data + offset, sizeof(center));
    offset += sizeof(center);

    std::memcpy(&azimuth, data + offset, sizeof(azimuth));
    offset += sizeof(azimuth);

    std::memcpy(&incline, data + offset, sizeof(incline));
    offset += sizeof(incline);

    std::memcpy(&aspect, data + offset, sizeof(aspect));
    offset += sizeof(aspect);

    std::memcpy(&miniViewProjectMtx, data + offset, sizeof(miniViewProjectMtx));
    offset += sizeof(miniViewProjectMtx);

    std::memcpy(&miniEye, data + offset, sizeof(miniEye));
    offset += sizeof(miniEye);

    std::memcpy(&miniCenter, data + offset, sizeof(miniCenter));
    offset += sizeof(miniCenter);

    std::memcpy(&miniAzimuth, data + offset, sizeof(miniAzimuth));
    offset += sizeof(miniAzimuth);

    std::memcpy(&miniIncline, data + offset, sizeof(miniIncline));
    offset += sizeof(miniIncline);

    std::memcpy(&miniAspect, data + offset, sizeof(miniAspect));
    offset += sizeof(miniAspect);

    return offset;
}