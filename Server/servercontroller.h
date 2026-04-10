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
private slots:
    void onServerDataReceived(const QByteArray& data, const QHostAddress& clientAddress,
                              quint16 clientPort);
    void onClientTimer();
private:
    bool checkLimits(const SensorData& data) const noexcept;

signals:
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
        uint errorCount;
    };

private:
    UDPServer* mServer;
    QTimer* mClientTimer; //для отключения клиентов
    LimitsReader::Limits mDataLimits;
    //строка IP:port как ключ
    QHash<QString, ClientInfo> mClients;





};

#endif // SERVERCONTROLLER_H
