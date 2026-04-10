#ifndef CLIENTWINDOW_H
#define CLIENTWINDOW_H

#include <QMainWindow>
#include "clientcontroller.h"
#include <QMessageBox>
QT_BEGIN_NAMESPACE
namespace Ui { class ClientWindow; }
QT_END_NAMESPACE

class ClientWindow : public QMainWindow
{
    Q_OBJECT

public:
    ClientWindow(QWidget *parent = nullptr);
    ~ClientWindow();
public slots:
    void onControllerError(const QString& message);
private slots:
    void onDataRequested();
    void onServerResponseReceived(bool isValid);
    void on_pushButtonConnect_clicked();


private:
    Ui::ClientWindow *ui;
    std::unique_ptr<ClientController> mController;
};
#endif // CLIENTWINDOW_H
