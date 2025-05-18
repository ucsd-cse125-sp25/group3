#include <vector>
#include "Packet.h"
#ifndef STATEUPDATEPACKET_H // include guard
#define STATEUPDATEPACKET_H

class StateUpdatePacket : public Packet {
public:
    // actions
    bool altDown;

    // cube
    float model[4][4];
    bool isInvisible;

    // camera
    float viewProjectMtx[4][4];
    float eye[3];
    float center[3];
    float azimuth;
    float incline;
    float aspect;

    // minimap camera
    float miniViewProjectMtx[4][4];
    float miniEye[3];
    float miniCenter[3];
    float miniAzimuth;
    float miniIncline;
    float miniAspect;

    StateUpdatePacket();

    unsigned int getSize() override;

    int serializePayload(char* data) override;

    int deserializePayload(char* data) override;
};

#endif