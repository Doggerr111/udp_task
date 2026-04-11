#include "serverwindow.h"
#include "./ui_serverwindow.h"
#include <QSplitter>
#include <QMessageBox>
#include "datatypes.h"
ServerWindow::ServerWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::ServerWindow),
      mController(std::make_unique<ServerController>()),
      mIsServerRunning{false}
{
    ui->setupUi(this);
    setupController();
    setupWidgets();
}

ServerWindow::~ServerWindow()
{
    delete ui;
}

void ServerWindow::setupController()
{
    connect(mController.get(), &ServerController::serverStarted, this, &ServerWindow::onServerStarted);
    connect(mController.get(), &ServerController::serverStopped, this, &ServerWindow::onServerStoped);
    connect(mController.get(), &ServerController::clientAdded,   this, &ServerWindow::onClientAdded);
    connect(mController.get(), &ServerController::clientRemoved, this, &ServerWindow::onClientRemoved);
    connect(mController.get(), &ServerController::clientUpdated, this, &ServerWindow::onClientUpdated);
    connect(mController.get(), &ServerController::error,         this, &ServerWindow::onControllerError);
    connect(mController.get(), &ServerController::clientDataReceived, this, &ServerWindow::onClientDataReceived);
}

void ServerWindow::setupWidgets()
{
    QLayout* oldLayout = ui->frameData->layout();
    delete oldLayout;

    QSplitter* splitter = new QSplitter(Qt::Horizontal, ui->frameData);
    ui->tableWidgetClients->setParent(splitter);
    ui->textEditLog->setParent(splitter);
    splitter->addWidget(ui->tableWidgetClients);
    splitter->addWidget(ui->textEditLog);
    QVBoxLayout* mainLayout = new QVBoxLayout(ui->frameData);
    mainLayout->addWidget(splitter);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    ui->tableWidgetClients->setColumnCount(3);
    ui->tableWidgetClients->setHorizontalHeaderLabels({"IP","Порт","Количество ошибок"});
    ui->tableWidgetClients->resizeColumnsToContents();
    ui->tableWidgetClients->setSortingEnabled(true);
    ui->tableWidgetClients->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->textEditLog->setReadOnly(true);
    QIntValidator *portValidator = new QIntValidator(1, 65535, this);
    ui->lineEditPort->setValidator(portValidator);
}

void ServerWindow::sortClientsTable()
{
    ui->tableWidgetClients->sortItems(2);
    ui->tableWidgetClients->resizeColumnsToContents();
}

void ServerWindow::updateStatus()
{
    QString statusText;
    if (mIsServerRunning)
    {
        int port = ui->lineEditPort->text().toInt();
        int clients = ui->tableWidgetClients->rowCount();
        statusText = QString("Статус сервера: запущен на порту %1 | Клиентов: %2")
                .arg(port).arg(clients);
    }
    else
        statusText = "Статус сервера: неактивен";
    ui->statusbar->showMessage(statusText);
}

void ServerWindow::appendSystemMessage(const QString &msg)
{
    QString html = QString("<b>[СИСТЕМА] %1</b>").arg(msg);
    ui->textEditLog->append(html);
}


void ServerWindow::on_pushButtonStartStop_clicked()
{
    mIsServerRunning ? mController->stopServer()
                     : mController->startServer(static_cast<quint16>(ui->lineEditPort->text().toInt()));
}

void ServerWindow::onControllerError(const QString &message)
{
    QMessageBox::critical(this, "Ошибка", message);
}

void ServerWindow::onServerStarted()
{
    mIsServerRunning = true;
    ui->lineEditPort->setReadOnly(true);
    ui->pushButtonStartStop->setText("Остановить сервер");
    appendSystemMessage("Сервер запущен на порту " + ui->lineEditPort->text());
    updateStatus();
}

void ServerWindow::onServerStoped()
{
    mIsServerRunning = false;
    ui->lineEditPort->setReadOnly(false);
    ui->tableWidgetClients->setRowCount(0);
    ui->pushButtonStartStop->setText("Запустить сервер");
    appendSystemMessage("Сервер остановлен");
    updateStatus();
}

void ServerWindow::onClientRemoved(const QString &ip, quint16 port)
{
    for (int row = 0; row < ui->tableWidgetClients->rowCount(); ++row)
    {
        if (ui->tableWidgetClients->item(row, 0)->text() == ip &&
            ui->tableWidgetClients->item(row, 1)->text() == QString::number(port))
        {
            ui->tableWidgetClients->removeRow(row);
            sortClientsTable();
            updateStatus();
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

    QTableWidgetItem* errorItem = new QTableWidgetItem();
    errorItem->setData(Qt::DisplayRole, errorCount);
    ui->tableWidgetClients->setItem(row, 2, errorItem);

    sortClientsTable();
    updateStatus();
}

void ServerWindow::onClientUpdated(const QString &ip, quint16 port, uint errorCount)
{
    for (int row = 0; row < ui->tableWidgetClients->rowCount(); ++row)
    {
        if (ui->tableWidgetClients->item(row, 0)->text() == ip &&
            ui->tableWidgetClients->item(row, 1)->text() == QString::number(port))
        {
            ui->tableWidgetClients->item(row, 2)->setData(Qt::DisplayRole, errorCount);
            sortClientsTable();
            updateStatus();
            break;
        }
    }

}

void ServerWindow::onClientDataReceived(const QString &ip, quint16 port, const SensorData &data, bool isValid)
{
    QString status = isValid ? "КОРРЕКТНО" : "НЕКОРРЕКТНО";
    QString color = isValid ? "green" : "red";
    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");

    QString html = QString("<b>[%1] %2:%3:</b> "
                           "X=%4 Y=%5 V=%6 "
                           "M=%7 S=%8 "
                           "A=%9 P=%10 | "
                           "<span style='color:%11;'>%12</span>")
            .arg(timestamp)
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

