#ifndef SERVERCONTROLLER_H
#define SERVERCONTROLLER_H
#include "udpserver.h"
#include "limitsreader.h"
#include <QDateTime>
#include <QTimer>
class SensorData;
class ServerController: public QObject
{
    Q_OBJECT
public:
    ServerController();
    void startServer(quint16 port);
    void stopServer();

private:
    bool checkLimits(const SensorData& data) const noexcept;

private slots:
    void onServerDataReceived(const QByteArray& data, const QHostAddress& clientAddress, quint16 clientPort);
    void onServerError(const QString& message);
    void onClientTimer();

signals:
    void error(const QString& message);
    void serverStarted();
    void serverStopped();
    void clientRemoved(const QString& ip, quint16 port);
    void clientAdded  (const QString& ip, quint16 port, uint errorCount);
    void clientUpdated(const QString& ip, quint16 port, uint errorCount);
    void clientDataReceived(const QString& ip, quint16 port, const SensorData& data, bool isValid);

private:
    struct ClientInfo
    {
        QHostAddress address;
        quint16 port;
        QDateTime lastSeen;
        uint errorCount = 0;

        QString getFormatedAddress() const
        {
            if (address.protocol() == QAbstractSocket::IPv4Protocol)
                return address.toString();
            //пробуем извлечь IPv4 из IPv4-mapped (чтобы не было ::ffff: в начале строки)
            quint32 ipv4 = address.toIPv4Address();
            if (ipv4 != 0)
                return QHostAddress(ipv4).toString();
            //IPv6
            return address.toString();
        }
    };

private:
    std::unique_ptr<UDPServer> mServer;
    std::unique_ptr<QTimer> mClientTimer; //для отключения клиентов
    LimitsReader::Limits mDataLimits;
    //строка IP:port как ключ
    QHash<QString, ClientInfo> mClients;

    static constexpr int INACTIVE_TIMEOUT_SEC = 5;   // клиент считается неактивным после 5 секунд бездействия
    static constexpr int CLEANUP_INTERVAL_MS = 3000; // проверка клиентов каждые 3 секунды
};

#endif // SERVERCONTROLLER_H
