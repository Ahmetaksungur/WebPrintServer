#include "serverhandler.h"

ServerHandler::ServerHandler(QObject *parent) : QObject(parent)
{
    p = new printer();
    connect(this,SIGNAL(execPrinter(QByteArray,QString,QString,QString,QString,bool,bool)),p,SLOT(exec(QByteArray,QString,QString,QString,QString,bool,bool)));
}

void ServerHandler::newRequest(QHttpRequest *req, QHttpResponse *res)
{
    this->req = req;
    this->res = res;
    _postData.clear();
    connect(req, SIGNAL(data(const QByteArray&)), this, SLOT(accumulate(const QByteArray&)));
    connect(req, SIGNAL(end()), this, SLOT(handlerReply()));
}

void ServerHandler::accumulate(const QByteArray &data)
{
    _postData.append(data);
}

void ServerHandler::parseGet()
{
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    QUrlQuery url = QUrlQuery(req->url());
#else
    QUrl url = req->url();
#endif
    getData.clear();
    QList<QPair<QString, QString>> items = url.queryItems();
    for(int i=0; i<items.size();i++) {
        getData.insert(items.at(i).first, items.at(i).second);
    }
}

QString ServerHandler::get(QString key)
{
    if(getData.contains(key)) {
        return getData.value(key);
    }
    return "";
}

void ServerHandler::parsePost()
{
    postData.clear();
    QList<QByteArray> items = _postData.split('&');
    for(int i=0; i<items.length();i++){
        QList<QByteArray> item = items.at(i).split('=');
        postData.insert(QString(item.at(0)),QString(QByteArray::fromPercentEncoding(item.at(1))));
    }
}

QString ServerHandler::post(QString key)
{
    if(postData.contains(key)) {
        return postData.value(key);
    }
    return "";
}

void ServerHandler::handlerReply()
{
    if(req->method() == QHttpRequest::HTTP_OPTIONS){
        res->setHeader("Access-Control-Allow-Headers", "Origin, X-Requested-With, content-type");
        res->setHeader("Access-Control-Allow-Origin", "*");
        res->setHeader("Access-Control-Allow-Methods", "POST, GET");
        res->writeHead(200);
        res->end();
        return;
    }
    QString path = req->path();
    parseGet();
    bool isurl = false;
    QString code;
    QByteArray content;
    if(_postData.length()>0){
        code = get("code");
        if(code == "raw"){
            content = _postData;
        }else if (code == "base64") {
            content = QByteArray::fromBase64(QByteArray::fromPercentEncoding(_postData));
        }else if (code == "url") {
            content = QByteArray::fromPercentEncoding(_postData);
            isurl = true;
        }else{
            content = QByteArray::fromPercentEncoding(_postData);
            if(content.left(5) == "data="){
                content = content.right(content.length()-5);
            }
        }
    }
    path = path.right(path.length()-1);
    QString body = "ok";
    if(path == "getAllPrinter") {
        body = p->getAllPrinter();
    }else if(path == "getDefaultPrinter") {
        body = p->getDefaultPrinter();
    }else if(path == "printPreview") {
        emit execPrinter(content, get("type"), get("printer"), get("page"), get("margin"), true, isurl);
    }else if(path == "printNow") {
        emit execPrinter(content, get("type"), get("printer"), get("page"), get("margin"), true, isurl);
    }else if(path == "version") {
        body = QCoreApplication::applicationVersion();
    }else{
        body += "GET /version<br>";
        body += "GET /getAllPrinter<br>";
        body += "GET /getDefaultPrinter<br>";
        body += "POST /printNow?printer=cannon&page=width*height&margin=left*top*right*bottom&type=img/html&code=raw/base64/url<br>";
        body += "POST /printPreview?printer=cannon&page=width*height&margin=left*top*right*bottom&type=img/html&code=raw/base64/url<br>";
    }
    echo(body);
}

void ServerHandler::echo(QString body)
{
    QString format = get("format");
    if(!format.isEmpty()){
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
        body = QString(QByteArray::fromPercentEncoding(format.toUtf8())).arg(body);
#else
        body = format.arg(body);
#endif
    }
    QByteArray data = body.toUtf8();
    res->setHeader("Content-Length", QString::number(data.size()));
    res->setHeader("Content-type", "text/html; charset=utf-8");
    res->setHeader("Access-Control-Allow-Headers", "Origin, X-Requested-With, content-type");
    res->setHeader("Access-Control-Allow-Origin", "*");
    res->setHeader("Access-Control-Allow-Methods", "POST, GET");
    res->setHeader("Cache-Control", "no-cache");
    res->setHeader("Pragma", "no-cache");
    res->writeHead(200);
    res->end(data);
}
