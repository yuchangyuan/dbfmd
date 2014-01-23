#include "client.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QTextStream>
#include <QDebug>
#include <QNetworkReply>
#include <QUrlQuery>
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

const QString Client::CHANNEL_URL = "http://douban.fm/j/app/radio/channels";
const QString Client::SONG_URL = "http://douban.fm/j/app/radio/people";

void Client::login(QString user_id, QString expire, QString token)
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

    QString expire1 = expire.toString();
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
    case OP_RATE:
        doRate();
        break;
    case OP_SKIP:
        doSkip();
        break;
    case OP_TRASH:
        doTrash();
        break;
    case OP_UPDTE_PLAYLIST:
        doUpdatePlaylist(ch_id);
        break;
    default:
        break;
    }
}

void Client::refreshChannel()
{
    doOperation_(OP_REFRESH_CHANNEL, QUrl(CHANNEL_URL));
}

void Client::doEnd()
{
    if (playlistEmpty()) {
        ++track_;
        emit(operationFinish(OP_END, false, "playlist empty"));
        return;
    }

    if (!login_) {
        emit(operationFinish(OP_END, true, ""));
        return;
    }

    doOperation_(OP_END, genUrl(OP_END));
}

void Client::doSkip()
{
    if (playlistEmpty()) {
        ++track_;
        emit(operationFinish(OP_SKIP, true, "playlist empty"));
        return;
    }

    if (!login_) {
        emit(operationFinish(OP_SKIP, true, ""));
        return;
    }

    doOperation_(OP_SKIP, genUrl(OP_SKIP));
}

void Client::doRate()
{
    if (!login_) {
        emit(operationFinish(OP_TRASH, false, "not login"));
        return;
    }

    if (playlistEmpty()) {
        emit(operationFinish(OP_TRASH, false, "playlist empty"));
        return;
    }

    doOperation_(OP_RATE, genUrl(OP_RATE));
}

void Client::doTrash()
{
    if (!login_) {
        emit(operationFinish(OP_TRASH, false, "not login"));
        return;
    }

    if (playlistEmpty()) {
        emit(operationFinish(OP_TRASH, false, "playlist empty"));
        return;
    }

    doOperation_(OP_TRASH, genUrl(OP_TRASH));
}

void Client::doUpdatePlaylist(int ch_id)
{
    doOperation_(OP_UPDTE_PLAYLIST, genUrl(OP_UPDTE_PLAYLIST, ch_id));
}

bool Client::playlistEmpty()
{
    return track_ >= playlist_.size();
}

QUrl Client::genUrl(OpType type, int ch_id)
{
    QUrl url(SONG_URL);
    QUrlQuery q;

    q.addQueryItem("app_name", "radio_desktop_win");
    q.addQueryItem("version", "100");
    q.addQueryItem("kbps", "192");

    if (login_) {
        q.addQueryItem("user_id", user_id_);
        q.addQueryItem("expire", expire_);
        q.addQueryItem("token", token_);
    }

    if (type != OP_UPDTE_PLAYLIST) {
        q.addQueryItem("sid", getSid());
    }

    QString chstr;
    // NOTE: ch_id_ update here, not in callback,
    // there's no way to pass it to callback currently
    if ((type == OP_UPDTE_PLAYLIST) && (ch_id != -1)) {
        ch_id_ = ch_id;
    }
    chstr.sprintf("%d", ch_id_);
    q.addQueryItem("channel", chstr);

    switch (type) {
    case OP_END:
        q.addQueryItem("type", "e");
        break;
    case OP_RATE:
        if (getLike()) {
            q.addQueryItem("type", "u");
        }
        else {
            q.addQueryItem("type", "r");
        }
        break;
    case OP_SKIP:
        q.addQueryItem("type", "s");
        break;
    case OP_TRASH:
        q.addQueryItem("type", "b");
        break;
    case OP_UPDTE_PLAYLIST:
        q.addQueryItem("type", "n");
        break;
    default:
        break;
    }

    url.setQuery(q);

    return url;
}

void Client::doOperation_(OpType type, QUrl url)
{
    OperationProcessor *cp = new OperationProcessor(this);
    connect(cp, SIGNAL(finish(Client::OpType,bool,QString,QJsonObject)),
            SLOT(operationFinish_(Client::OpType,bool,QString,QJsonObject)));

    qDebug() << "request url:" << url;
    QNetworkReply *reply = manager_.get(QNetworkRequest(url));
    cp->setReply(reply, type);
}

void Client::operationFinish_(Client::OpType type, bool success,
                  QString message, QJsonObject obj)
{
    if (success) {
        switch (type) {
        case OP_REFRESH_CHANNEL:
            channel_ = obj["channels"].toArray();
            // qDebug() << "refresh channel" << channel_;
            qDebug() << "channel refreshed, size:" << channel_.size();
            break;
        case OP_END:
        case OP_SKIP:
        case OP_TRASH:
            ++track_;
            break;
        case OP_RATE:
            break;
        case OP_UPDTE_PLAYLIST:
            playlist_ = obj["song"].toArray();
            track_ = 0;

            qDebug() << "update playlist, channel:" << ch_id_;
            // qDebug() << "playlist object:" << obj;
            break;
        }
    }
    else {
        qDebug() << "operation fail, type:" << type << "message:" << message;
    }

    emit(operationFinish(type, success, message));
}


bool Client::getLike()
{
    return (playlist_[track_].toObject()["like"].toInt() != 0);
}

QString Client::getSid()
{
    return (playlist_[track_].toObject()["sid"].toString());
}

int Client::currentTrack()
{
    return track_;
}

QJsonArray Client::currentPlaylist()
{
    return playlist_;
}
