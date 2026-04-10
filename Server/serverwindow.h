#ifndef SERVERWINDOW_H
#define SERVERWINDOW_H

#include <QMainWindow>
#include "servercontroller.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ServerWindow; }
QT_END_NAMESPACE

class ServerWindow : public QMainWindow
{
    Q_OBJECT

public:
    ServerWindow(QWidget *parent = nullptr);
    ~ServerWindow();

private slots:
    void on_pushButtonStartStop_clicked();

    void onServerStarted();
    void onServerStoped();

    void onClientRemoved(const QString& ip, quint16 port);
    void onClientAdded  (const QString& ip, quint16 port, uint errorCount);
    void onClientUpdated(const QString& ip, quint16 port, uint errorCount);
    void onClientDataReceived(const QString& ip, quint16 port, const SensorData& data, bool isValid);

private:
    Ui::ServerWindow *ui;
    std::unique_ptr<ServerController> mController;
    bool mIsServerRunning;
};
#endif // SERVERWINDOW_H
