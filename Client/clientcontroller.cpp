#include "clientcontroller.h"
#include "packetbuilder.h"
#include "datatypes.h"
#include <QDateTime>
ClientController::ClientController()
    : mUDPClient(std::make_unique<UDPClient>()),  //удаляем сами, без parent
      mSendTimer(std::make_unique<QTimer>())
{
    connect(mUDPClient.get(), &UDPClient::error, this, &ClientController::onNetworkError);
    connect(mUDPClient.get(), &UDPClient::dataSent, this, &ClientController::onPacketSent);
    connect(mUDPClient.get(), &UDPClient::responseReceived, this, &ClientController::onServerResponseReceived);
    connect(mSendTimer.get(), &QTimer::timeout, this, &ClientController::onSendTimer);
}

void ClientController::setServerParams(const QString &ip, quint16 port)
{
    if (ip.isEmpty())
    {
        emit error("IP-адрес не может быть пустым");
        return;
    }
    if (port == 0 || port > 65535)
    {
        emit error("Порт должен быть в диапазоне 1-65535");
        return;
    }
    if (!mUDPClient->setServerParams(ip, port))
    {
        emit error("Неверный IP-адрес");
        return;
    }
    mUDPClient->startListening();
    mSendTimer->start(1000);
    emit serverConfigured(getFormattedServerIp(), port);
}

void ClientController::disconnectFromServer()
{
    mUDPClient->clearConnection();
    mSendTimer->stop();
    emit configurationCleared();
}

void ClientController::sendData(const SensorData &data)
{
    auto byteArray = PacketBuilder::pack(data);
    mUDPClient->sendData(byteArray);
}

QString ClientController::getFormattedServerIp() const
{
    auto address = mUDPClient->getServerAddress();
    if (address.protocol() == QAbstractSocket::IPv4Protocol)
        return address.toString();
    //пробуем извлечь IPv4 из IPv4-mapped (чтобы не было ::ffff: в начале строки)
    quint32 ipv4 = address.toIPv4Address();
    if (ipv4 != 0)
        return QHostAddress(ipv4).toString();
    //IPv6
    return address.toString();
}


void ClientController::onSendTimer()
{
    emit dataRequested();
}

void ClientController::onNetworkError(const QString &senderError)
{
    emit error(senderError);
}

void ClientController::onPacketSent(const QByteArray &data)
{
    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
    qDebug() << "[" << timestamp << "] Packet sent: " << data.size() << " bytes";
}

void ClientController::onServerResponseReceived(const QByteArray &data)
{
    constexpr int RESPONSE_MIN_SIZE = 2;
    constexpr uint8_t RESPONSE_MSG_ID = 0;
    qDebug() << "Response received:" << data.toHex();
    if (data.size() < RESPONSE_MIN_SIZE)
    {
        emit error("Неверный ответ сервера");
        return;
    }

    uint8_t msgId = static_cast<uint8_t>(data[0]);
    uint8_t flag = static_cast<uint8_t>(data[1]);
    if (msgId != RESPONSE_MSG_ID)
    {
        emit error("Неизвестный тип сообщения");
        return;
    }

    emit serverResponseReceived(flag == 1);
}
