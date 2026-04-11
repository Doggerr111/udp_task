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
    void onServerConfigured(const QString& ip, quint16 port);
    void onServerConfigurationCleared();
    void onDataRequested();
    void onServerResponseReceived(bool isValid);
    void on_pushButtonConnect_clicked();
    void on_pushButtonDisconnect_clicked();

private:
    Ui::ClientWindow *ui;
    std::unique_ptr<ClientController> mController;
    static constexpr int CONNECTION_PAGE_INDX = 0;
    static constexpr int DATA_PAGE_INDX = 1;

};
#endif // CLIENTWINDOW_H
