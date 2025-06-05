#include "Packet.h"
#ifndef WINPACKET_H 
#define WINPACKET_H

enum class WinnerType : unsigned int {
    THIEF = 0,
    GUARD = 1
};

class WinPacket : public Packet {
public:
    unsigned int winningClientID;
    
    WinPacket();

    unsigned int getSize() override;

    int serializePayload(char* data) override;

    int deserializePayload(char* data) override;

    WinnerType winnerType;
};

#endif