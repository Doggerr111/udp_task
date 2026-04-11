#ifndef UDPClient_H
#define UDPClient_H
#include <QUdpSocket>

class UDPClient: public QObject
{
    Q_OBJECT
public:
    UDPClient(QObject* parent = nullptr);
    bool setServerParams(const QString& ip, quint16 port);
    void clearConnection();
    void startListening();
    void sendData(const QByteArray& bytes);

private slots:
    void onReadyRead();
    void onSocketError(QAbstractSocket::SocketError socketError);

signals:
    void error(const QString& message);
    void responseReceived(const QByteArray& data);
    void dataSent(const QByteArray& data);

private:
    quint16 mServerPort;
    QHostAddress mServerAddress;
    std::unique_ptr<QUdpSocket> mSocket;
};

#endif // UDPClient_H
