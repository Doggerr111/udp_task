#include "clientcontroller.h"
#include "packetbuilder.h"
ClientController::ClientController()
{
    mUDPClient = new UDPClient(this);
    connect(mUDPClient, &UDPClient::error, this, &ClientController::onSenderError);
    connect(mUDPClient, &UDPClient::dataSent, this, &ClientController::onSenderDataSent);
    connect(mUDPClient, &UDPClient::responseReceived, this, &ClientController::onSenderResponseReceived);
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
    return true;
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

void ClientController::onSenderError(const QString &senderError)
{
    emit error(senderError);
}

void ClientController::onSenderDataSent(const QByteArray &data)
{
    qDebug()<<"data sent:" + QString::number(data.size()) + " bytes";
}

void ClientController::onSenderResponseReceived(const QByteArray &data)
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

//void ClientController::onConnectionRequested(const QString &ip, quint16 port)
//{
//    auto addres = QHostAddress(ip);
//    auto m_udpSocket = new QUdpSocket();
//    connect(m_udpSocket, &QUdpSocket::readyRead, this, [m_udpSocket](){
//        while (m_udpSocket->hasPendingDatagrams()) {
//            QByteArray buffer;
//            buffer.resize(m_udpSocket->pendingDatagramSize());
//            m_udpSocket->readDatagram(buffer.data(), buffer.size());
//            qDebug()<<buffer;
//        }
//    });


//    // Отправить тестовое сообщение
//    QByteArray testData = "ping";
//    m_udpSocket->writeDatagram(testData, addres, port);
//}
