#include "udpserver.h"
UDPServer::UDPServer(QObject *parent)
    : QObject(parent)
{
    mUdpSocket = new QUdpSocket(this);
}

bool UDPServer::startServer(quint16 port)
{
    if (!mUdpSocket->bind(QHostAddress::Any, port))
    {
        emit error(mUdpSocket->errorString());
        return false;
    }

    connect(mUdpSocket, &QUdpSocket::readyRead, this, &UDPServer::onReadyRead);
    return true;
}

void UDPServer::stopServer()
{
    if (mUdpSocket)
    {
        mUdpSocket->close();
        disconnect(mUdpSocket, &QUdpSocket::readyRead, this, &UDPServer::onReadyRead);
    }
}

bool UDPServer::sendResponse(const QByteArray &response, const QHostAddress &clientAddress, quint16 clientPort)
{
     auto size = mUdpSocket->writeDatagram(response, clientAddress, clientPort);
     return size == response.size() ? true : false;
}

void UDPServer::onReadyRead()
{
    qDebug()<<"readyRead()";
    while (mUdpSocket->hasPendingDatagrams())
    {
        QByteArray buffer;
        buffer.resize(mUdpSocket->pendingDatagramSize());

        QHostAddress clientAddress;
        quint16 clientPort;

        mUdpSocket->readDatagram(buffer.data(), buffer.size(), &clientAddress, &clientPort);

        qDebug() << "Получено от" << clientAddress.toString() << clientPort;
        qDebug() << "Данные:" << buffer.toHex();

        //отправляем в контроллер
        emit dataReceived(buffer, clientAddress, clientPort);
    }
}
