#ifndef SERVERHANDLER_H
#define SERVERHANDLER_H

#include <QObject>
#include <QMap>
#include <QUrl>
#include <QList>
#include <QPair>
#include <QTextDocument>
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
#include <QUrlQuery>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/QPrintPreviewDialog>
#include <QtPrintSupport/QPrinterInfo>
#else
#include <QPrinter>
#include <QPrintDialog>
#include <QPrintPreviewDialog>
#include <QPrinterInfo>
#endif

#include "qhttpserver.h"
#include "qhttprequest.h"
#include "qhttpresponse.h"
#include "printer.h"

class ServerHandler : public QObject
{
    Q_OBJECT
public:
    explicit ServerHandler(QObject *parent = 0);

private:
    void parseGet();
    void parsePost();
    QString get(QString key);
    QString post(QString key);

signals:
    void execPrinter(QByteArray content,QString contentType,QString printer,QString size,QString margin,bool ispreview,bool isurl);

public slots:
    void newRequest(QHttpRequest *req, QHttpResponse *res);
    void accumulate(const QByteArray &data);
    void handlerReply();
    void echo(QString body);

private:
    QHttpRequest* req;
    QHttpResponse* res;
    printer* p;
    QByteArray _postData;
    QMap<QString, QString> postData;
    QMap<QString, QString> getData;
};

#endif // SERVERHANDLER_H
