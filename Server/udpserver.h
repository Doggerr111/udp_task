#ifndef UDPSERVER_H
#define UDPSERVER_H
#include <QUdpSocket>

class UDPServer: public QObject
{
    Q_OBJECT
public:
    UDPServer(QObject* parent = nullptr);
    bool startServer(quint16 port);
    void stopServer();
    bool sendResponse(const QByteArray& response, const QHostAddress& clientAddress, quint16 clientPort);
private slots:
    void onReadyRead();
signals:
    void dataReceived(const QByteArray& data, const QHostAddress& clientAddr,
                      quint16 clientPort);
    void error(const QString& message);
private:
    QUdpSocket* mUdpSocket;
};

#endif // UDPSERVER_H
