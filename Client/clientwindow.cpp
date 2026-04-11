#include "clientwindow.h"
#include "./ui_clientwindow.h"
#include <QDateTime>
ClientWindow::ClientWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::ClientWindow),
      mController{std::make_unique<ClientController>()}
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(CONNECTION_PAGE_INDX);
    connect(mController.get(), &ClientController::error, this, &ClientWindow::onControllerError);
    connect(mController.get(), &ClientController::serverConfigured, this, &ClientWindow::onServerConfigured);
    connect(mController.get(), &ClientController::configurationCleared, this, &ClientWindow::onServerConfigurationCleared);
    connect(mController.get(), &ClientController::dataRequested, this, &ClientWindow::onDataRequested);
    connect(mController.get(), &ClientController::serverResponseReceived, this, &ClientWindow::onServerResponseReceived);
    QIntValidator *portValidator = new QIntValidator(1, 65535, this);
    ui->lineEditPort->setValidator(portValidator);
}

ClientWindow::~ClientWindow()
{
    delete ui;
}

void ClientWindow::onControllerError(const QString &message)
{
    QMessageBox::critical(this, "Ошибка", message);
}

void ClientWindow::onServerConfigured(const QString &ip, quint16 port)
{
    ui->stackedWidget->setCurrentIndex(DATA_PAGE_INDX);
    ui->labelConnectionInfo->setText(QString("Сервер: %1:%2 | Отправка данных...")
                                         .arg(ip).arg(port));
}

void ClientWindow::onServerConfigurationCleared()
{
    ui->stackedWidget->setCurrentIndex(CONNECTION_PAGE_INDX);
    ui->textEditResponse->clear();
}


void ClientWindow::on_pushButtonConnect_clicked()
{
//    bool ok;
//    int port = ui->lineEditPort->text().toInt(&ok);
//    if (!ok || port < 1 || port > 65535)
//    {
//        QMessageBox::warning(this, "Ошибка", "Порт должен быть числом от 1 до 65535");
//        return;
//    }
    mController->setServerParams(ui->lineEditIP->text(), ui->lineEditPort->text().toInt());
}


void ClientWindow::on_pushButtonDisconnect_clicked()
{
    mController->disconnectFromServer();
}


void ClientWindow::onDataRequested()
{
    SensorData data;
    data.x = static_cast<uint8_t>(ui->spinBoxX->value());
    data.y = static_cast<int8_t> (ui->spinBoxY->value());
    data.v = static_cast<uint8_t>(ui->spinBoxSpeed->value());
    data.m = static_cast<uint8_t>(ui->spinBoxMode->value());
    data.s = static_cast<uint8_t>(ui->spinBoxState->value());
    data.a = static_cast<float>  (ui->doubleSpinBoxAcceleration->value());
    data.p = static_cast<uint8_t>(ui->spinBoxPower->value());
    mController->sendData(data);
}

void ClientWindow::onServerResponseReceived(bool isValid)
{
    QString status = isValid ? "КОРРЕКТНО" : "НЕКОРРЕКТНО";
    QString color = isValid ? "green" : "red";

    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
    QString html = QString("<span style='color:%1;'>[%2] Ответ сервера: %3</span>")
            .arg(color).arg(timestamp).arg(status);
    ui->textEditResponse->append(html);
}



