#ifndef PRINTER_H
#define PRINTER_H
#include <QObject>
#include <QtCore>
#include <QIcon>
#include <QPainter>
#include <QCloseEvent>
#include <QTextDocument>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QSslSocket>
#include <QSslConfiguration>
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

class printer : public QObject
{
    Q_OBJECT
public:
    explicit printer(QObject *parent = 0);
    QString getAllPrinter();
    QString getDefaultPrinter();
    void print();
    void preview();

public slots:
    void exec(QByteArray content,QString contentType="html",QString printer="",QString size="",QString margin="",bool ispreview=true,bool isurl=false);
    void remoteDate(QNetworkReply *reply);
    void setPreviewContent(QPrinter *p);

private:
    QNetworkAccessManager _manager;
    QPrinter _printer;
    QString _contentType;
    QByteArray _content;
    bool _ispreview;
};

#endif // PRINTER_H
