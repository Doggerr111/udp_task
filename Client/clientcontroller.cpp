#include "clientcontroller.h"
#include "packetbuilder.h"
ClientController::ClientController()
{
    mUDPClient = new UDPClient(this);
    connect(mUDPClient, &UDPClient::error, this, &ClientController::onClientError);
    connect(mUDPClient, &UDPClient::dataSent, this, &ClientController::onPacketSent);
    connect(mUDPClient, &UDPClient::responseReceived, this, &ClientController::onServerResponseReceived);
    mSendTimer = new QTimer(this);
    connect(mSendTimer, &QTimer::timeout, this, &ClientController::onSendTimer);
}

bool ClientController::setServerParams(const QString &ip, quint16 port)
{
    if (ip.isEmpty())
    {
        emit error("IP-адрес не может быть пустым");
        return false;
    }
    if (port == 0 || port > 65535)
    {
        emit error("Порт должен быть в диапазоне 1-65535");
        return false;
    }
    if (!mUDPClient->setServerParams(ip, port))
    {
        emit error("Неверный IP-адрес");
        return false;
    }
    mUDPClient->startListening();
    mSendTimer->start(1000);
    emit serverConfigured(ip, port);
    return true;
}

void ClientController::disconnectFromServer()
{
    mUDPClient->clearConnection();
    mSendTimer->stop();
    emit configurationCleared();
}

void ClientController::sendData(const SensorData &data)
{
    qDebug()<<"sendData() called";
    auto byteArray = PacketBuilder::pack(data);
    mUDPClient->sendData(byteArray);
}

void ClientController::onDataReceived(const SensorData &data)
{

    auto byteArray = PacketBuilder::pack(data);
    mUDPClient->sendData(byteArray);

}

void ClientController::onSendTimer()
{
    qDebug()<<"onSendTimer() called";
    emit dataRequested();
}

void ClientController::onClientError(const QString &senderError)
{
    emit error(senderError);
}

void ClientController::onPacketSent(const QByteArray &data)
{
    qDebug()<<"data sent:" + QString::number(data.size()) + " bytes";
}

void ClientController::onServerResponseReceived(const QByteArray &data)
{
    qDebug() << "response:" << data.toHex();

    if (data.size() < 2)
    {
        emit error("Неверный ответ сервера");
        return;
    }

    uint8_t msgId = static_cast<uint8_t>(data[0]);
    uint8_t flag = static_cast<uint8_t>(data[1]);

    if (msgId != 0)
    {
        emit error("Неизвестный тип сообщения");
        return;
    }

    emit serverResponseReceived(flag == 1);
}
