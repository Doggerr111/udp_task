#include "udpclient.h"

UDPClient::UDPClient(QObject *parent)
    : QObject(parent),
      mServerPort{0},
      mSocket{std::make_unique<QUdpSocket>()}
{
    connect(mSocket.get(), &QUdpSocket::errorOccurred, this, &UDPClient::onSocketError);
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

void UDPClient::clearConnection()
{
    if (mSocket)
    {
        //отключаем прием данных
        disconnect(mSocket.get(), &QUdpSocket::readyRead, this, &UDPClient::onReadyRead);
        mSocket->close();
    }
    mServerAddress = QHostAddress();
    mServerPort = 0;
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
    if (mSocket->state() == QAbstractSocket::BoundState) return; //если уже слушаем

    if (!mSocket->bind())
    {
        emit error("Не удалось привязать сокет для получения ответов");
        return;
    }
    disconnect(mSocket.get(), &QUdpSocket::readyRead, this, &UDPClient::onReadyRead);
    connect(mSocket.get(), &QUdpSocket::readyRead, this, &UDPClient::onReadyRead);
}

void UDPClient::onReadyRead()
{
    while (mSocket->hasPendingDatagrams())
    {
        QByteArray buffer;
        buffer.resize(mSocket->pendingDatagramSize());
        mSocket->readDatagram(buffer.data(), buffer.size());
        qDebug() << "Response received:" << buffer.toHex();
        emit responseReceived(buffer);
    }
}

void UDPClient::onSocketError(QAbstractSocket::SocketError socketError)
{
    QString errorMsg = QString("Сокет ошибка %1: %2")
                           .arg(socketError)
                           .arg(mSocket->errorString());
    emit error(errorMsg);
}

