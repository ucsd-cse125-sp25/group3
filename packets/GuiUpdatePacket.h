#include "Packet.h"
#include "../include/core.h"
#ifndef GUIUPDATEPACKET_H // include guard
#define GUIUPDATEPACKET_H

class GuiUpdatePacket : public Packet {
    public:
        GameState currentState; 
        
        GuiUpdatePacket();
    
        unsigned int getSize() override;
    
        int serializePayload(char* data) override;
    
        int deserializePayload(char* data) override;
};
#endif