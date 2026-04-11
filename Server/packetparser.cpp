#include "packetparser.h"
#include "datatypes.h"
#include <array>
std::optional<SensorData> PacketParser::parse(const QByteArray &packet)
{
    uint16_t word1 = (static_cast<uint8_t>(packet[1]) << 8) | static_cast<uint8_t>(packet[0]);
    uint16_t word2 = (static_cast<uint8_t>(packet[3]) << 8) | static_cast<uint8_t>(packet[2]);
    uint16_t word3 = (static_cast<uint8_t>(packet[5]) << 8) | static_cast<uint8_t>(packet[4]);
    //uint16_t word4 = (static_cast<uint8_t>(packet[7]) << 8) | static_cast<uint8_t>(packet[6]); //резерв

    uint8_t x = static_cast<uint8_t>(word1 & 0x3F);
    int8_t y_raw = static_cast<int8_t>((word1 >> 8) & 0x3F);
    //0x20 - 6 бит, указывает на знак
    int8_t y = (y_raw & 0x20) ? (y_raw - 64) : y_raw;

    uint8_t v = static_cast<uint8_t>(word2 & 0xFF);
    uint8_t m = static_cast<uint8_t>((word2 >> 8) & 0x03);
    uint8_t s = static_cast<uint8_t>((word2 >> 12) & 0x03);

    uint8_t a_int = static_cast<uint8_t>(word3 & 0xFF);
    float a = (a_int / 10.0) - 12.7; //обратное преобразование
    uint8_t p = static_cast<uint8_t>((word3 >> 8) & 0xFF);
    return SensorData{x,y,v,m,s,a,p};
}
