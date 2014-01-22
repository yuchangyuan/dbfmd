#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include "client.h"
#include <QJsonObject>
#include <QUrl>
#include <QMediaPlayer>

class Player : public QObject
{
    Q_OBJECT

    bool playing_;
    QJsonObject trackInfo_;
    QMediaPlayer qmp_;

public:
    explicit Player(QObject *parent = 0);

    // should only be called after first updateTrack();
    void pause();
    void play();

    // cacehd version of current song info
    QJsonObject trackInfo();

signals:
    void positionChanged(qint64 pos);
    void mediaStatusChanged(QMediaPlayer::MediaStatus status);

public slots:
    // should be called when current song changed in client
    // then player will stop current song, choose new song
    // the begin to play or just pause at beginning
    void updateTrack(QJsonObject trackInfo, QUrl url);
private slots:
    void bufferStatusChanged(int status);

};

#endif // PLAYER_H
