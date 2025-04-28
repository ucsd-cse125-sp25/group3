#include "gtest/gtest.h"
#include "Packet.h"
#include <cstring>

// NOTE: These tests assumes packet structure is composed of length (4 bytes), type (4 bytes), data (N bytes) 
class PacketTest : public ::testing::Test {
    protected: 
        Packet packet;
        char buffer[MAX_PACKET_SIZE]; 

        void SetUp() {
            packet.packet_type = STATE_UPDATE;
            std::string test_msg = "Let's get testing! :D";
            packet.payload = std::vector<char>(test_msg.begin(), test_msg.end());
            packet.length = packet.payload.size();
        }
};

TEST_F(PacketTest, GetSize) {
    EXPECT_EQ(packet.getSize(), sizeof(unsigned int) * 2 + packet.payload.size());
}

TEST_F(PacketTest, Seralize) {
    packet.serialize(buffer);

    unsigned int type;
    unsigned int length;
    memcpy(&type, buffer, sizeof(unsigned int));
    memcpy(&length, buffer + 4, sizeof(unsigned int));

    EXPECT_EQ(type, packet.packet_type);
    EXPECT_EQ(length, packet.length);
    EXPECT_TRUE(std::equal(packet.payload.begin(), packet.payload.end(), buffer + 8));
}

TEST_F(PacketTest, Deserialize) {
    packet.serialize(buffer);

    Packet packet2;
    packet2.deserialize(buffer);

    EXPECT_EQ(packet2.packet_type, packet.packet_type);
    EXPECT_EQ(packet2.length, packet.length);
    EXPECT_EQ(packet2.payload, packet.payload);
}

TEST(PacketTestNoSetUp, EmptyPayload) {
    Packet packet;
    packet.packet_type = INIT_CONNECTION;
    packet.length = 0;

    char buffer[8]; 
    packet.serialize(buffer);

    Packet packet2;
    packet2.deserialize(buffer);

    EXPECT_EQ(packet2.packet_type, INIT_CONNECTION);
    EXPECT_EQ(packet2.length, 0);
    EXPECT_TRUE(packet2.payload.empty());
}

TEST(PacketTestNoSetUp, LargePayload) {
    Packet packet;
    packet.packet_type = STATE_UPDATE;
    packet.payload = std::vector<char>(MAX_PACKET_SIZE - (MAX_PACKET_SIZE / 10), 'E');
    packet.length = packet.payload.size();

    ASSERT_LE(packet.getSize(), MAX_PACKET_SIZE);

    char *buffer = new char[packet.getSize()];
    packet.serialize(buffer);

    Packet packet2;
    packet2.deserialize(buffer);

    EXPECT_EQ(packet2.packet_type, packet.packet_type);
    EXPECT_EQ(packet2.length, packet.length);
    EXPECT_EQ(packet2.payload, packet.payload);
}

TEST(PacketTestNoSetUp, MaxPayload) {
    Packet packet;
    packet.packet_type = STATE_UPDATE;
    packet.payload = std::vector<char>(MAX_PACKET_SIZE - 8, 'E');
    packet.length = packet.payload.size();

    ASSERT_EQ(packet.getSize(), MAX_PACKET_SIZE);

    char *buffer = new char[packet.getSize()];
    packet.serialize(buffer);

    Packet packet2;
    packet2.deserialize(buffer);

    EXPECT_EQ(packet2.packet_type, packet.packet_type);
    EXPECT_EQ(packet2.length, packet.length);
    EXPECT_EQ(packet2.payload, packet.payload);
}

TEST(PacketTestNoSetUp, TooLargePayloadSerialize) {
    Packet packet;
    packet.packet_type = STATE_UPDATE;
    packet.payload = std::vector<char>(MAX_PACKET_SIZE + 1, 'E');
    packet.length = packet.payload.size();

    char *buffer = new char[MAX_PACKET_SIZE + 1];

    EXPECT_THROW(packet.serialize(buffer), std::runtime_error);
}

TEST(PacketTestNoSetUp, TooLargePayloadDeserialize) {
    Packet packet;
    packet.packet_type = STATE_UPDATE;
    packet.length = MAX_PACKET_SIZE + 1;
    char *buffer = new char[8];
    packet.serialize(buffer);

    EXPECT_THROW(packet.deserialize(buffer), std::runtime_error);
}