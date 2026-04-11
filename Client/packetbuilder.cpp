#include "packetbuilder.h"
#include "datatypes.h"

QByteArray PacketBuilder::pack(const SensorData &data)
{
    QByteArray packet(8, 0);

    uint16_t word1 = 0;
    word1 |= ((data.x & 0x3F));
    uint8_t y = static_cast<uint8_t>(data.y) & 0x3F;
    word1 |= (y << 8);

    uint16_t word2 = 0;
    word2 |= ((data.v & 0xFF));
    word2 |= ((data.m & 0x03) << 8);
    word2 |= ((data.s & 0x03) << 12);

    uint16_t word3 = 0;
    uint8_t a = static_cast<uint8_t>((data.a + 12.7) * 10 + 0.5);
    word3 |= a;
    word3 |= ((data.p & 0xFF) << 8);

    //uint16_t word4 = 0;
    packet[0] = static_cast<char>(word1);
    packet[1] = static_cast<char>(word1 >> 8);
    packet[2] = static_cast<char>(word2);
    packet[3] = static_cast<char>(word2 >> 8);
    packet[4] = static_cast<char>(word3);
    packet[5] = static_cast<char>(word3 >> 8);
    //packet[6], packet[7] остаются 0 (резерв R)
    return packet;
}
