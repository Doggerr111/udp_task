#include "servercontroller.h"
#include <QCoreApplication>
#include "packetparser.h"
#include "datatypes.h"
ServerController::ServerController()
{
    mServer = new UDPServer(this);
    connect(mServer, &UDPServer::dataReceived, this, &ServerController::onServerDataReceived);

    mClientTimer = new QTimer(this);
    connect(mClientTimer, &QTimer::timeout, this, &ServerController::onClientTimer);
}

void ServerController::startServer(quint16 port)
{
    if (!mServer->startServer(port)) return;

    QString configPath = QCoreApplication::applicationDirPath() + "/limits.json";
    auto limitsOpt = LimitsReader::read(configPath);
    if (limitsOpt.has_value())
    {
        mDataLimits = limitsOpt.value();
        mClientTimer->start(10000); //каждый 10с. проверяем клиентов
        emit serverStarted();
    }


}

void ServerController::stopServer()
{
    mServer->stopServer();
    mClients.clear();
    if (mClientTimer)
        mClientTimer->stop();
    emit serverStopped();
}

//при получении сообщения от клиента сервером
void ServerController::onServerDataReceived(const QByteArray &data, const QHostAddress &clientAddress, quint16 clientPort)
{

    QString key = clientAddress.toString() + ":" + QString::number(clientPort);

    bool isNew = !mClients.contains(key);
    ClientInfo& client = mClients[key];
    client.address = clientAddress;
    client.port = clientPort;
    client.lastSeen = QDateTime::currentDateTime();

    auto sensorDataOpt = PacketParser::parse(data);

    SensorData sensorData;
    if (sensorDataOpt.has_value())
    {
        sensorData = sensorDataOpt.value();
        bool valid = checkLimits(sensorData);

        QByteArray response;
        response.append(char(0));   // идентификатор сообщения
        response.append(char(valid ? 1 : 0));  // флаг корректности
        mServer->sendResponse(response, clientAddress, clientPort);
        emit clientDataReceived(clientAddress.toString(), clientPort, sensorData, valid);
        if (!valid) client.errorCount++;
    }

    if (isNew)
        emit clientAdded(clientAddress.toString(), clientPort, client.errorCount);
    else
        emit clientUpdated(clientAddress.toString(), clientPort, client.errorCount);


}

void ServerController::onClientTimer()
{
    QDateTime now = QDateTime::currentDateTime();
    for (auto it = mClients.begin(); it != mClients.end();)
    {
        if (it->lastSeen.secsTo(now) > 10)  // удаляем после 10 секунд бездействия
        {
            it = mClients.erase(it);
            emit clientRemoved(it->address.toString(), it->port);
        }
        else
            ++it;
    }
}

bool ServerController::checkLimits(const SensorData &data) const noexcept
{
    return data.x >= mDataLimits.xMin && data.x <= mDataLimits.xMax &&
           data.y >= mDataLimits.yMin && data.y <= mDataLimits.yMax &&
           data.v >= mDataLimits.vMin && data.v <= mDataLimits.vMax &&
           data.m >= mDataLimits.mMin && data.m <= mDataLimits.mMax &&
           data.s >= mDataLimits.sMin && data.s <= mDataLimits.sMax &&
           data.a >= mDataLimits.aMin && data.a <= mDataLimits.aMax &&
           data.p >= mDataLimits.pMin && data.p <= mDataLimits.pMax;
}
