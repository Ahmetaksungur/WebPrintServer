#include "printer.h"

printer::printer(QObject *parent) : QObject(parent)
{
    connect(&_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(remoteDate(QNetworkReply*)));
}

QString printer::getAllPrinter()
{
    QStringList printer;
    QList<QPrinterInfo> list = QPrinterInfo::availablePrinters();
    while (!list.isEmpty()) {
        QPrinterInfo tmp = list.takeFirst();
        printer.append(tmp.printerName());
    }
    return "[\""+printer.join("\",\"")+"\"]";
}

QString printer::getDefaultPrinter()
{
    return QPrinterInfo::defaultPrinter().printerName();
}

void printer::exec(QByteArray content, QString contentType, QString printer, QString size, QString margin, bool ispreview, bool isurl)
{
    _content = content;
    _contentType = contentType;
    _ispreview = ispreview;
    if(!printer.isEmpty()) {
        _printer.setPrinterName(printer);
    }
    if(!size.isEmpty()) {
        QStringList s = size.split("*");
        if(s.length()>1){
            qreal w = QString(s.at(0)).toDouble();
            qreal h = QString(s.at(1)).toDouble();
            _printer.setPageSize(QPrinter::Custom);
            _printer.setPaperSize(QSizeF(w,h),QPrinter::Millimeter);
        }
    }
    if(!margin.isEmpty()) {
        QStringList s = margin.split("*");
        if(s.length()>3){
            qreal left = QString(s.at(0)).toDouble();
            qreal top = QString(s.at(1)).toDouble();
            qreal right = QString(s.at(2)).toDouble();
            qreal bottom = QString(s.at(3)).toDouble();
            _printer.setPageMargins(left,top,right,bottom,QPrinter::Millimeter);
        }
    }
    if(isurl){
        QNetworkRequest request;
        QSslConfiguration conf;
        conf.setProtocol(QSsl::AnyProtocol);
        conf.setPeerVerifyMode(QSslSocket::VerifyNone);
        request.setSslConfiguration(conf);
        request.setUrl(QUrl(content));
        _manager.get(request);
    }else if (ispreview) {
        preview();
    }else{
        print();
    }
}

void printer::remoteDate(QNetworkReply *reply)
{
    _content = reply->readAll();
    reply->close();
    if (_ispreview) {
        preview();
    }else{
        print();
    }
}

void printer::print()
{
    if(_contentType == "img") {
        QPixmap img = QPixmap();
        img.loadFromData(_content);
        QPainter painter(&_printer);
        QRect rect = painter.viewport();
        QSize size = img.size();
        size.scale(rect.size(), Qt::KeepAspectRatio);
        painter.setViewport((rect.width()-size.width())/2, (rect.height()-size.height())/2, size.width(), size.height());
        painter.setWindow(img.rect());
        painter.drawPixmap(0, 0, img);
    } else if(_contentType == "txt") {
        QPainter painter(&_printer);
        painter.setViewport(0, 0, _printer.width(), _printer.height());
        painter.drawText(0, 0, _content);
    } else {
        QTextDocument doc;
        doc.setPageSize(QSizeF(_printer.pageRect().size()));
        doc.setHtml(_content);
        doc.print(&_printer);
    }
}

void printer::preview()
{
    QPrintPreviewDialog *dialog = new QPrintPreviewDialog(&_printer);
    connect(dialog, SIGNAL(paintRequested(QPrinter*)),
            this, SLOT(setPreviewContent(QPrinter*)));
    dialog->setWindowModality(Qt::WindowModal);
    dialog->setWindowFlags(dialog->windowFlags() | Qt::WindowStaysOnTopHint);
    dialog->exec();
}

void printer::setPreviewContent(QPrinter *p)
{
    if(_contentType == "img") {
        QPixmap img = QPixmap();
        img.loadFromData(_content);
        QPainter painter(p);
        QRect rect = painter.viewport();
        QSize size = img.size();
        size.scale(rect.size(), Qt::KeepAspectRatio);
        painter.setViewport((rect.width()-size.width())/2, (rect.height()-size.height())/2, size.width(), size.height());
        painter.setWindow(img.rect());
        painter.drawPixmap(0, 0, img);
    } else if(_contentType == "txt") {
        QPainter painter(p);
        painter.setViewport(0, 0, p->width(), p->height());
        painter.drawText(0, 0, _content);
    } else {
        QTextDocument doc;
        doc.setPageSize(QSizeF(p->pageRect().size()));
        doc.setHtml(_content);
        doc.print(p);
    }
}
