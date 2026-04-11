#include "udpserver.h"
UDPServer::UDPServer(QObject *parent)
    : QObject(parent),
      mUdpSocket{std::make_unique<QUdpSocket>()}
{
    connect(mUdpSocket.get(), &QUdpSocket::errorOccurred, this, &UDPServer::onSocketError);
}

bool UDPServer::startServer(quint16 port)
{
    if (mUdpSocket->state() == QAbstractSocket::BoundState) //уже запущен
        return true;

    if (!mUdpSocket->bind(QHostAddress::Any, port))
        return false;

    connect(mUdpSocket.get(), &QUdpSocket::readyRead, this, &UDPServer::onReadyRead);
    return true;
}

void UDPServer::stopServer()
{
    if (mUdpSocket)
    {
        mUdpSocket->close();
        disconnect(mUdpSocket.get(), &QUdpSocket::readyRead, this, &UDPServer::onReadyRead);
    }
}

bool UDPServer::sendResponse(const QByteArray &response, const QHostAddress &clientAddress, quint16 clientPort)
{
     quint64 size = mUdpSocket->writeDatagram(response, clientAddress, clientPort);
     return size != -1;
}

void UDPServer::onReadyRead()
{
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

void UDPServer::onSocketError(QAbstractSocket::SocketError socketError)
{
    QString errorMsg = QString("Ошибка сокета %1: %2")
                           .arg(socketError)
                           .arg(mUdpSocket->errorString());
    emit error(errorMsg);
}
