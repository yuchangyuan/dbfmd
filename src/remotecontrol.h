#ifndef REMOTECONTROL_H
#define REMOTECONTROL_H

#include <QObject>
#include <nzmqt/nzmqt.hpp>
#include "control.h"

class RemoteControl : public QObject
{
    Q_OBJECT

    Control *control_;
    Player *player_;

    nzmqt::ZMQContext *context_;
    nzmqt::ZMQSocket *sock_pub_;
    nzmqt::ZMQSocket *sock_rep_;

    void reply(bool res, const QString err_msg, QByteArray *ok_msg = 0);

public:
    explicit RemoteControl(Control *control, Player *player, QObject *parent = 0);

signals:

public slots:
    void request(QList<QByteArray> req);
};

#endif // REMOTECONTROL_H
