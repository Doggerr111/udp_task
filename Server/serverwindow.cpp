#include "serverwindow.h"
#include "./ui_serverwindow.h"
#include <QUdpSocket>
#include "datatypes.h"
ServerWindow::ServerWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::ServerWindow),
      mController(std::make_unique<ServerController>()),
      mIsServerRunning{false}
{
    ui->setupUi(this);
    connect(mController.get(), &ServerController::serverStarted, this, &ServerWindow::onServerStarted);
    connect(mController.get(), &ServerController::serverStopped, this, &ServerWindow::onServerStoped);
    connect(mController.get(), &ServerController::clientAdded,   this, &ServerWindow::onClientAdded);
    connect(mController.get(), &ServerController::clientRemoved, this, &ServerWindow::onClientRemoved);
    connect(mController.get(), &ServerController::clientUpdated, this, &ServerWindow::onClientUpdated);
    connect(mController.get(), &ServerController::clientDataReceived, this, &ServerWindow::onClientDataReceived);
    ui->tableWidgetClients->setColumnCount(3);
}

ServerWindow::~ServerWindow()
{
    delete ui;
}


void ServerWindow::on_pushButtonStartStop_clicked()
{
    mIsServerRunning ? mController->stopServer()
                     : mController->startServer(static_cast<quint16>(ui->lineEditPort->text().toInt()));
}

void ServerWindow::onServerStarted()
{
    mIsServerRunning = true;
    ui->pushButtonStartStop->setText("Остановить сервер");
}

void ServerWindow::onServerStoped()
{
    mIsServerRunning = false;
    ui->tableWidgetClients->setRowCount(0);
    ui->pushButtonStartStop->setText("Запустить сервер");
}

void ServerWindow::onClientRemoved(const QString &ip, quint16 port)
{
    for (int row = 0; row < ui->tableWidgetClients->rowCount(); ++row)
    {
        if (ui->tableWidgetClients->item(row, 0)->text() == ip &&
            ui->tableWidgetClients->item(row, 1)->text() == QString::number(port))
        {
            ui->tableWidgetClients->removeRow(row);
            break;
        }
    }
}

void ServerWindow::onClientAdded(const QString &ip, quint16 port, uint errorCount)
{
    int row = ui->tableWidgetClients->rowCount();
    ui->tableWidgetClients->insertRow(row);
    ui->tableWidgetClients->setItem(row, 0, new QTableWidgetItem(ip));
    ui->tableWidgetClients->setItem(row, 1, new QTableWidgetItem(QString::number(port)));
    ui->tableWidgetClients->setItem(row, 2, new QTableWidgetItem(QString::number(errorCount)));
}

void ServerWindow::onClientUpdated(const QString &ip, quint16 port, uint errorCount)
{
    for (int row = 0; row < ui->tableWidgetClients->rowCount(); ++row)
    {
        if (ui->tableWidgetClients->item(row, 0)->text() == ip &&
            ui->tableWidgetClients->item(row, 1)->text() == QString::number(port))
        {
            ui->tableWidgetClients->item(row, 2)->setText(QString::number(errorCount));
            break;
        }
    }
}

void ServerWindow::onClientDataReceived(const QString &ip, quint16 port, const SensorData &data, bool isValid)
{
    QString status = isValid ? "КОРРЕКТНО" : "НЕКОРРЕКТНО";
    QString color = isValid ? "green" : "red";

    QString html = QString("<b>От %1:%2:</b> "
                           "X=%3 Y=%4 V=%5 "
                           "M=%6 S=%7 "
                           "A=%8 P=%9 | "
                           "<span style='color:%10;'>%11</span>")
            .arg(ip)
            .arg(port)
            .arg(data.x)
            .arg(data.y)
            .arg(data.v)
            .arg(data.m)
            .arg(data.s)
            .arg(data.a)
            .arg(data.p)
            .arg(color)
            .arg(status);

    ui->textEditLog->append(html);
}

