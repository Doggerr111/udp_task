#ifndef PACKETPARSER_H
#define PACKETPARSER_H

#include <QByteArray>
#include <optional>
class SensorData;
class PacketParser
{
public:
    static std::optional<SensorData> parse(const QByteArray& packet);
private:
    PacketParser() = delete;
    PacketParser(const PacketParser&) = delete;
    PacketParser& operator=(const PacketParser&) = delete;
};

#endif // PACKETPARSER_H
