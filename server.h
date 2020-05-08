#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QIcon>
#include <QMenu>
#include <QAction>
#include <QProcess>
#include <QSettings>
#include <QDesktopServices>
#include <QSystemTrayIcon>
#include <QApplication>
#include <QCoreApplication>

#include "serverhandler.h"
#include "qhttpserverfwd.h"

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);
    ~Server();
    void run();
    void showSystemTray();

public slots:
    void trayActivated(QSystemTrayIcon::ActivationReason reason);
    void open();
    void close();

private:
    QSettings *settings;
    QHttpServer *server;
    ServerHandler *serverHandler;
    QSystemTrayIcon *tray;
};

#endif // SERVER_H
