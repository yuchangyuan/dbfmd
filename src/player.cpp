#include "player.h"

Player::Player(QObject *parent) :
    QObject(parent), playing_(false), qmp_(this)
{
    connect(&qmp_, SIGNAL(positionChanged(qint64)),
            SIGNAL(positionChanged(qint64)));
    connect(&qmp_, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),
            SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)));
    connect(&qmp_, SIGNAL(bufferStatusChanged(int)),
            SLOT(bufferStatusChanged(int)));
}

void Player::play()
{
    if (playing_) return;
    playing_ = true;
    qmp_.play();
}

void Player::pause()
{
    if (!playing_) return;
    playing_ = false;
    qmp_.pause();
}

void Player::updateTrack(QJsonObject trackInfo, QUrl url)
{

    // 1. stop current track
    qmp_.stop();

    // 2. update info
    trackInfo_ = trackInfo;

    // 3. setup use new url
    qDebug() << "player, play url:" << url;
    qmp_.setMedia(QMediaContent(url));

    // 4. playr or pause
    if (playing_) {
        qmp_.play();
    }
}

void Player::bufferStatusChanged(int status)
{
    qDebug() << "player, buffer:" << status;
}

