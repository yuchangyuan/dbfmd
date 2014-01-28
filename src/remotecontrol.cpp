#include "remotecontrol.h"
#include <nzmqt/nzmqt.hpp>

#include <QJsonDocument>

using namespace nzmqt;

RemoteControl::RemoteControl(Control *control, Player *player,
                             QObject *parent) :
    QObject(parent), control_(control), player_(player),
    context_(createDefaultContext(parent))
{
    sock_pub_ = context_->createSocket(ZMQSocket::TYP_PUB, this);
    sock_pub_->bindTo("tcp://*:12677");

    sock_rep_ = context_->createSocket(ZMQSocket::TYP_REP, this);
    sock_rep_->bindTo("tcp://*:12678");

    connect(sock_rep_, SIGNAL(messageReceived(QList<QByteArray>)),
            SLOT(request(QList<QByteArray>)));

    context_->start();
}


void RemoteControl::request(QList<QByteArray> req)
{
    bool res;
    QString err_msg;

    if (req.size() < 1) return;

    qDebug() << "get remote request" << req;

    if ("info" == req[0]) {
        QByteArray ok_msg(QJsonDocument(control_->trackInfo()).toJson());
        reply(true, "", &ok_msg);
        return;
    }

    if ("skip" == req[0]) {
        res = control_->doOperation(Client::OP_SKIP, err_msg);
        reply(res, err_msg);
        return;
    }

    if ("rate" == req[0]) {
        res = control_->doOperation(Client::OP_RATE, err_msg);
        reply(res, err_msg);
        return;
    }

    if ("trash" == req[0]) {
        res = control_->doOperation(Client::OP_TRASH, err_msg);
        reply(res, err_msg);

        return;
    }

    if ("switch" == req[0]) {
        if (req.size() < 2) {
            reply(false, "missing channel id");
            return;
        }
        bool ok;
        int ch_id = req[1].toInt(&ok);
        if (!ok) {
            reply(false, "channel id is not integer");
            return;
        }

        res = control_->doOperation(Client::OP_UPDTE_PLAYLIST, err_msg, ch_id);
        reply(res, err_msg);
        return;
    }


    // play/pause player_ operation
    if ("play" == req[0]) {
    player_->play();
        reply(true, "");
        return;
    }

    if ("pause" == req[0]) {
    player_->pause();
        reply(true, "");
        return;
    }
}

void RemoteControl::reply(bool res, const QString err_msg, QByteArray *ok_msg)
{
    QList<QByteArray> rep;
    if (res) {
    rep << "ok";
        if (0 != ok_msg) rep << *ok_msg;
    }
    else {
        rep << "error";
        rep << err_msg.toUtf8();
    }

    sock_rep_->sendMessage(rep);
}
