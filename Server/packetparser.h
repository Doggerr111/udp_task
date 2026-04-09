#ifndef PACKETPARSER_H
#define PACKETPARSER_H

#include <QByteArray>
#include <optional>
class SensorData;
class PacketParser
{
public:
    PacketParser() = delete;
    static std::optional<SensorData> parse(const QByteArray& packet);
};

#endif // PACKETPARSER_H
