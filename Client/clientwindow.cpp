#include "clientwindow.h"
#include "./ui_clientwindow.h"
#include <QDateTime>
ClientWindow::ClientWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::ClientWindow),
      mController{std::make_unique<ClientController>()}
{
    ui->setupUi(this);
    connect(mController.get(), &ClientController::error, this, &ClientWindow::onControllerError);
    connect(mController.get(), &ClientController::dataRequested, this, &ClientWindow::onDataRequested);
    connect(mController.get(), &ClientController::serverResponseReceived, this, &ClientWindow::onServerResponseReceived);
}

ClientWindow::~ClientWindow()
{
    delete ui;
}

void ClientWindow::onControllerError(const QString &message)
{
    QMessageBox::critical(this, "Ошибка", message);
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


void ClientWindow::on_pushButtonConnect_clicked()
{
    if (mController->setServerParams(ui->lineEditIP->text(), ui->lineEditPort->text().toInt()))
    {
        ui->stackedWidget->setCurrentIndex(1);
    }
}

