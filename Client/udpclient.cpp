#include "udpclient.h"

UDPClient::UDPClient(QObject *parent)
    : QObject(parent)
{
    mSocket = new QUdpSocket(parent);
}

bool UDPClient::setServerParams(const QString &ip, quint16 port)
{
    QHostAddress address(ip);
    if (address.isNull())
        return false;
    mServerAddress = address;
    mServerPort = port;
    return true;
}

void UDPClient::sendData(const QByteArray &bytes)
{
    qint64 sent = mSocket->writeDatagram(bytes, mServerAddress, mServerPort);
    if (sent == -1)
        emit error("Ошибка отправки данных");
    else if (sent != bytes.size())
        emit error("Отправлено не все байты");
}


void UDPClient::startListening()
{
    if (!mSocket->bind())
    {
        emit error("Не удалось привязать сокет для получения ответов");
        return;
    }
    connect(mSocket, &QUdpSocket::readyRead, this, &UDPClient::onReadyRead);
}

void UDPClient::onReadyRead()
{
    while (mSocket->hasPendingDatagrams())
    {
        QByteArray buffer;
        buffer.resize(mSocket->pendingDatagramSize());
        mSocket->readDatagram(buffer.data(), buffer.size());
        qDebug()<<buffer;
        emit responseReceived(buffer);
    }
}

