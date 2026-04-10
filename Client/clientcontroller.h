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
    void sendData(const SensorData& data);
private:
    UDPClient* mUDPClient;
    QTimer*    mSendTimer; //для отправки с частотой 1 гц
public slots:
    void onDataReceived(const SensorData& data); //получение пользовательского ввода с UI
private slots:
    void onSendTimer();
    void onSenderError(const QString& senderError);
    void onSenderDataSent(const QByteArray& data);
    void onSenderResponseReceived(const QByteArray& data);
signals:
    void error(const QString& message);
    void dataRequested(); //сигнал для UI, вызывается по таймеру
    void serverResponseReceived(bool isValid);

};

#endif // CLIENTCONTROLLER_H
