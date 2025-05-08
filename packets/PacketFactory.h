#include "Packet.h"
#include "KeyPacket.h"

#ifndef PACKETFACTORY_H // include guard
#define PACKETFACTORY_H

class PacketFactory {
    public:
        static std::unique_ptr<Packet> createFromBuffer(char* data);
    };
    
#endif