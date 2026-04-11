#ifndef CLIENTCONTROLLER_H
#define CLIENTCONTROLLER_H

#include <QUdpSocket>
#include <QTimer>
#include "udpclient.h"
#include "datatypes.h"
class ClientController: public QObject
{
    Q_OBJECT
public:
    ClientController();
    ~ClientController() = default;
    void setServerParams(const QString& ip, quint16 port);
    void disconnectFromServer();
    void sendData(const SensorData& data);
private slots:
    void onSendTimer();
    void onNetworkError(const QString& senderError);
    void onPacketSent(const QByteArray& data);
    void onServerResponseReceived(const QByteArray& data);
signals:
    void error(const QString& message);
    void serverConfigured(const QString& ip, quint16 port);
    void configurationCleared();
    void dataRequested(); //сигнал для UI, вызывается по таймеру
    void serverResponseReceived(bool isValid);

private:
    std::unique_ptr<UDPClient> mUDPClient;
    std::unique_ptr<QTimer> mSendTimer; //для отправки с частотой 1 гц

};

#endif // CLIENTCONTROLLER_H
