#ifndef CONTROL_H
#define CONTROL_H

#include <QObject>
#include "client.h"
#include "player.h"
#include <QMediaPlayer>
#include <QJsonObject>

class Control : public QObject
{
    Q_OBJECT

    Client *client_;
    Player *player_;

    void updateTrack();

public:
    explicit Control(Client *client, Player *player, QObject *parent = 0);

    void start();
    bool doOperation(Client::OpType type, QString& message, int ch_id = -1);
    QJsonObject trackInfo();

signals:

public slots:

private slots:
    void playerPositionChanged(qint64 pos);
    void playerMediaStatusChanged(QMediaPlayer::MediaStatus status);
    void clientOperationFinish(Client::OpType type, bool success, QString message);
};

#endif // CONTROL_H
