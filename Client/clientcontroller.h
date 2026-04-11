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
    bool setServerParams(const QString& ip, quint16 port);
    void disconnectFromServer();
    void sendData(const SensorData& data);
private:
    UDPClient* mUDPClient;
    QTimer*    mSendTimer; //для отправки с частотой 1 гц
public slots:
    void onDataReceived(const SensorData& data); //получение пользовательского ввода с UI
private slots:
    void onSendTimer();
    void onClientError(const QString& senderError);
    void onPacketSent(const QByteArray& data);
    void onServerResponseReceived(const QByteArray& data);
signals:
    void error(const QString& message);
    void serverConfigured(const QString& ip, quint16 port);
    void configurationCleared();
    void dataRequested(); //сигнал для UI, вызывается по таймеру
    void serverResponseReceived(bool isValid);

};

#endif // CLIENTCONTROLLER_H
