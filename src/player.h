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
    void rateChanged(bool like);
    void paused(bool status);

public slots:
    // should be called when current song changed in client
    // then player will stop current song, choose new song
    // the begin to play or just pause at beginning
    void updateTrack(const QJsonObject trackInfo, QUrl url);

    // update rate status after rate/unrate a track;
    void rate(QString sid);
private slots:
    void mediaStatusChanged_(QMediaPlayer::MediaStatus status);
};

#endif // PLAYER_H
