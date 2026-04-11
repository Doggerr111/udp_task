#ifndef UDPSERVER_H
#define UDPSERVER_H
#include <QUdpSocket>

class UDPServer: public QObject
{
    Q_OBJECT
public:
    UDPServer(QObject* parent = nullptr);
    UDPServer(const UDPServer&) = delete;
    UDPServer& operator=(const UDPServer&) = delete;
    bool startServer(quint16 port);
    void stopServer();
    bool sendResponse(const QByteArray& response, const QHostAddress& clientAddress, quint16 clientPort);
private slots:
    void onReadyRead();
    void onSocketError(QAbstractSocket::SocketError socketError);
signals:
    void dataReceived(const QByteArray& data, const QHostAddress& clientAddr,
                      quint16 clientPort);
    void error(const QString& message);
private:
    std::unique_ptr<QUdpSocket> mUdpSocket;
};

#endif // UDPSERVER_H
