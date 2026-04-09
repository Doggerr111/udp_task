#ifndef PACKETBUILDER_H
#define PACKETBUILDER_H
#include <QByteArray>

class SensorData;
class PacketBuilder
{
public:
    static QByteArray pack(const SensorData& data);
private:
    PacketBuilder() = delete;
};

#endif // PACKETBUILDER_H
