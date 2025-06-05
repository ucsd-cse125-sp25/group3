#pragma once

#include "Packet.h"
#include <cstring> // for memcpy, memset
#include <iostream>

class TimeUpdate : public Packet {
public:
    char timeString[8]; // formatted like "04:32"

    TimeUpdate();

    // int serialize(char* data) ;
    // int deserialize(char* data) ;
    int serializePayload(char* data) override;
    int deserializePayload(char* data) override;
    unsigned int getSize() override;
};
