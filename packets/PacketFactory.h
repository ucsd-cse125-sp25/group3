#include "Packet.h"
#include "KeyPacket.h"
#include "InitPacket.h"
#include "StateUpdatePacket.h"
#include "CursorPacket.h"
#include "EndGamePacket.h"
#include "InitPlayerPacket.h"
#include "NPCPacket.h"
#include "GuiUpdatePacket.h"
#include "WinPacket.h"
#include "TimeUpdate.h"
#include "InitMinigamePacket.h"
#include "MinigameFinishedPacket.h"

#ifndef PACKETFACTORY_H // include guard
#define PACKETFACTORY_H

class PacketFactory {
    public:
        static std::unique_ptr<Packet> createFromBuffer(char* data);
    };
    
#endif