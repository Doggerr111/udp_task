#include <iostream>
#include "Client/packetbuilder.h"
#include "Server/packetparser.h"
#include "common/datatypes.h"
using namespace std;

void testPackUnpackDebug(const SensorData& original) {
    QByteArray packet = PacketBuilder::pack(original);
    SensorData decoded = PacketParser::parse(packet).value();

    std::cout << "Original: x=" << (int)original.x
              << " y=" << (int)original.y
              << " v=" << (int)original.v
              << " m=" << (int)original.m
              << " s=" << (int)original.s
              << " a=" << original.a
              << " p=" << (int)original.p << std::endl;

    std::cout << "Decoded:  x=" << (int)decoded.x
              << " y=" << (int)decoded.y
              << " v=" << (int)decoded.v
              << " m=" << (int)decoded.m
              << " s=" << (int)decoded.s
              << " a=" << decoded.a
              << " p=" << (int)decoded.p << std::endl;
}

void test(const SensorData& original)
{
    QByteArray packet = PacketBuilder::pack(original);
    SensorData decoded = PacketParser::parse(packet).value();

    if (original == decoded)
        std::cout << "Test passed!" << std::endl;
    else
        std::cout << "Test failed!" << std::endl;

}

int main() {
    //testPackUnpackDebug({42, -20, 128, 2, 1, 5.0f, 100});
    test({42, -20, 128, 2, 1, 5.0f, 100});
    test({0, -32, 0, 0, 0, -12.7f, 0});
    test({63, 31, 255, 3, 3, 12.8f, 130});
    return 0;
}
