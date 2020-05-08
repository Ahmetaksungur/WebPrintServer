#include "server.h"
#include "qhttpserver.h"
#include "qhttprequest.h"
#include "qhttpresponse.h"

Server::Server(QObject *parent) : QObject(parent)
{
    QCoreApplication::setOrganizationName(QLatin1String("WEB PRINT SERVER"));
    QCoreApplication::setApplicationName(QLatin1String("WEB PRINT SERVER"));
    QCoreApplication::setApplicationVersion(QLatin1String("0.1"));
    settings = new QSettings();
    if(settings->value("port").isNull())
        settings->setValue("port", 12365);
    if(settings->value("host").isNull())
        settings->setValue("host", "127.0.0.1");
    server = new QHttpServer(this);
    serverHandler = new ServerHandler();

    connect(server, SIGNAL(newRequest(QHttpRequest*, QHttpResponse*)),
            serverHandler, SLOT(newRequest(QHttpRequest*, QHttpResponse*)));
}

Server::~Server()
{
    delete tray;
    delete server;
    delete serverHandler;
}

void Server::showSystemTray()
{
    QAction *helpAction = new QAction("help",this);
    helpAction->setIcon(QIcon(":/new/icon/help.png"));
    connect(helpAction,SIGNAL(triggered(bool)),this,SLOT(open()));

    QAction *exitAction = new QAction("exit",this);
    exitAction->setIcon(QIcon(":/new/icon/exit.png"));
    connect(exitAction,SIGNAL(triggered(bool)),this,SLOT(close()));

    QMenu *trayMenu = new QMenu();
    trayMenu->addAction(helpAction);
    trayMenu->addAction(exitAction);

    tray = new QSystemTrayIcon();
    tray->setContextMenu(trayMenu);
    tray->setToolTip(tr("WEB PRINT SERVER"));
    tray->setIcon(QIcon(":/new/icon/icon.png"));
    tray->show();
    connect(tray,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(trayActivated(QSystemTrayIcon::ActivationReason)));
}

void Server::trayActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
        break;
    case QSystemTrayIcon::DoubleClick:
        open();
        break;
    }
}

void Server::run()
{
    QString host = settings->value("host").toString();
    QHostAddress address;
    if(host.isEmpty()){
        address.setAddress(QHostAddress::Any);
    }else{
        address.setAddress(host);
    }
    server->listen(address, settings->value("port").toInt());
    showSystemTray();
}

void Server::open()
{
    QDesktopServices::openUrl(QUrl("http://" + settings->value("host").toString() + ":" + settings->value("port").toString()));
}

void Server::close()
{
    server->close();
    qApp->quit();
}
