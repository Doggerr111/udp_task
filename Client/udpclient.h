#ifndef UDPClient_H
#define UDPClient_H
#include <QUdpSocket>

class UDPClient: public QObject
{
    Q_OBJECT
public:
    UDPClient(QObject* parent = nullptr);
    bool setServerParams(const QString& ip, quint16 port);
    void sendData(const QByteArray& bytes);
    void startListening();

private slots:
    void onReadyRead();


signals:
    void error(const QString& message);
    void responseReceived(const QByteArray& data);
    void dataSent(const QByteArray& data);

private:
    quint16 mServerPort;
    QHostAddress mServerAddress;
    QUdpSocket* mSocket;
};

#endif // UDPClient_H
