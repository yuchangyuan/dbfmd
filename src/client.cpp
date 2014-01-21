#include "client.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QTextStream>
#include <QDebug>
#include <QNetworkReply>
#include "operationprocessor.h"

Client::Client(QObject *parent) :
    QObject(parent), manager_(this)
{
    login_ = false;
    ch_id_ = 0; // my channel
}

Client::~Client()
{
}

void Client::login(QString user_id, qint64 expire, QString token)
{
    user_id_ = user_id;
    expire_ = expire;
    token_ = token;
    login_ = true;

    qDebug() << "user_id:" << user_id <<
         "expire:" << expire <<
         "token:" << token;
}

void Client::logout()
{
    login_ = false;
}

Client* Client::create(QString login_json, QObject *parent)
{
    QTextStream cerr(stderr);
    Client *c = new Client(parent);

    // ONLY return not logined Client when file open failed.
    QFile file(login_json);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return c;
    QByteArray json = file.readAll();
    file.close();
    QJsonParseError err;

    QJsonDocument doc = QJsonDocument::fromJson(json, &err);
    if (QJsonParseError::NoError != err.error) {
        cerr << err.errorString() << endl;
        return 0;
    }

    QJsonObject obj = doc.object();
    QJsonValue token = obj["token"];
    QJsonValue user_id = obj["user_id"];
    QJsonValue expire = obj["expire"];

    if (!token.isString() || !user_id.isString() || !expire.isString()) {
        cerr << "json file miss key or type error" << endl;
        return 0;
    }

    bool ok;
    qint64 expire1 = expire.toString().toLongLong(&ok);
    if (!ok) {
        cerr << "expire type not int" << endl;
        return 0;
    }

    QString token1 = token.toString();
    QString user_id1 = user_id.toString();

    c->login(user_id1, expire1, token1);
    return c;
}

void Client::doOperation(OpType type, int ch_id)
{
    switch (type) {
    case OP_REFRESH_CHANNEL:
        refreshChannel();
        break;
    case OP_END:
        doEnd();
        break;
    default:
        break;
    }
}

void Client::refreshChannel()
{
}

void Client::doEnd()
{
    // do something, then call doOperation_
    QUrl url("http://");
    doOperation_(OP_END, url);
}

void Client::doOperation_(OpType type, QUrl url)
{
    OperationProcessor *cp = new OperationProcessor(this);

    QNetworkReply *reply = manager_.get(QNetworkRequest(url));
    cp->setReply(reply, type);
}

void Client::operationFinish_(OpType type, bool success, QString message, QJsonObject *obj)
{
    if (success) {
        switch (type) {
        case OP_REFRESH_CHANNEL:
            break;
        case OP_END:
            break;
        case OP_RATE:
            break;
        case OP_SKIP:
            break;
        case OP_TRASH:
            break;
        case OP_UPDTE_PLAYLIST:
            break;
        }
    }

    emit(operationFinish(type, success, message));
}
