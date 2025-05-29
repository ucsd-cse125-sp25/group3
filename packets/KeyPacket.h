#include "Packet.h"
#ifndef KEYPACKET_H // include guard
#define KEYPACKET_H

enum class KeyType {
    NONE = 0,
    KEY_W = 1,
    KEY_S = 2,
    KEY_D = 3,
    KEY_A = 4,
    KEY_T = 5,
    KEY_G = 6,
    KEY_K = 7,
    KEY_L = 8,
    KEY_E = 9,
    KEY_SPACE = 10,
    KEY_ESC = 11,
    KEY_R = 12,
    KEY_ALT_PRESS = 13,
    KEY_ALT_RELEASE = 14,
    KEY_F = 15,
    MENU_START = 16,
    CHAR_SEL_BACK = 17,
};

class KeyPacket : public Packet {
public:
    KeyType key_type; 

    KeyPacket();

    unsigned int getSize() override;

    int serializePayload(char* data) override;

    int deserializePayload(char* data) override;
};

#endif