#include "Packet.h"
#ifndef WINPACKET_H 
#define WINPACKET_H

class WinPacket : public Packet {
public:
    unsigned int winningClientID;
    
    WinPacket();

    unsigned int getSize() override;

    int serializePayload(char* data) override;

    int deserializePayload(char* data) override;
};

#endif