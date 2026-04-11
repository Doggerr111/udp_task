#include "servercontroller.h"
#include <QCoreApplication>
#include "packetparser.h"
#include "datatypes.h"
ServerController::ServerController()
    : mServer{std::make_unique<UDPServer>()},
      mClientTimer{std::make_unique<QTimer>()}
{
    connect(mServer.get(), &UDPServer::dataReceived, this, &ServerController::onServerDataReceived);
    connect(mServer.get(), &UDPServer::error, this, &ServerController::onServerError);
    connect(mClientTimer.get(), &QTimer::timeout, this, &ServerController::onClientTimer);
}

void ServerController::startServer(quint16 port)
{
    if (port == 0 || port > 65535)
    {
        emit error("Порт должен быть в диапазоне 1-65535");
        return;
    }
    if (!mServer->startServer(port)) return;

    QString configPath = QCoreApplication::applicationDirPath() + "/limits.json";
    auto limitsOpt = LimitsReader::read(configPath);
    if (limitsOpt.has_value())
    {
        mDataLimits = limitsOpt.value();
        mClientTimer->start(CLEANUP_INTERVAL_MS); //проверка клиентов
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
        emit clientDataReceived(client.getFormatedAddress(), clientPort, sensorData, valid);
        if (!valid) client.errorCount++;
    }
    if (isNew)
        emit clientAdded(client.getFormatedAddress(), clientPort, client.errorCount);
    else
        emit clientUpdated(client.getFormatedAddress(), clientPort, client.errorCount);
}

void ServerController::onServerError(const QString &message)
{
    emit error(message);
}

void ServerController::onClientTimer()
{
    QDateTime now = QDateTime::currentDateTime();
    for (auto it = mClients.begin(); it != mClients.end();)
    {
        if (it->lastSeen.secsTo(now) > INACTIVE_TIMEOUT_SEC)  // удаляем после бездействия
        {
            emit clientRemoved(it->getFormatedAddress(), it->port);
            it = mClients.erase(it);

        }
        else
            ++it;
    }
}

bool ServerController::checkLimits(const SensorData &data) const noexcept
{
    static constexpr double LIMITS_EPS = 1e-5;
    return data.x >= mDataLimits.xMin && data.x <= mDataLimits.xMax &&
           data.y >= mDataLimits.yMin && data.y <= mDataLimits.yMax &&
           data.v >= mDataLimits.vMin && data.v <= mDataLimits.vMax &&
           data.m >= mDataLimits.mMin && data.m <= mDataLimits.mMax &&
           data.s >= mDataLimits.sMin && data.s <= mDataLimits.sMax &&
           data.a + LIMITS_EPS >= mDataLimits.aMin && data.a - LIMITS_EPS <= mDataLimits.aMax &&
           data.p >= mDataLimits.pMin && data.p <= mDataLimits.pMax;
}
