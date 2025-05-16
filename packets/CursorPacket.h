#include "Packet.h"
#ifndef CURSORPACKET_H // include guard
#define CURSORPACKET_H 

class CursorPacket : public Packet {
public:
    double currX;
    double currY;

    CursorPacket();

    unsigned int getSize() override;

    int serializePayload(char* data) override;

    int deserializePayload(char* data) override;
};

#endif