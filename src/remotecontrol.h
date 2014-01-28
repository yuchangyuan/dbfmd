#ifndef REMOTECONTROL_H
#define REMOTECONTROL_H

#include <QObject>
#include <nzmqt/nzmqt.hpp>
#include "control.h"
#include <QMediaPlayer>

class RemoteControl : public QObject
{
    Q_OBJECT

    Control *control_;
    Player *player_;

    nzmqt::ZMQContext *context_;
    nzmqt::ZMQSocket *sock_pub_;
    nzmqt::ZMQSocket *sock_rep_;

    // for sock_rep_
    void reply(bool res, const QString err_msg, QByteArray *ok_msg = 0);
    // for sock_pub_
    void publish(QString type, QByteArray data);

public:
    explicit RemoteControl(Control *control, Player *player, QObject *parent = 0);

signals:

public slots:
    void request(QList<QByteArray> req);

private slots:
    // from player_
    void positionChanged(qint64 pos);
    void mediaStatusChanged(QMediaPlayer::MediaStatus status);
    void rateChanged(bool like);
    void paused(bool status);

};

#endif // REMOTECONTROL_H
