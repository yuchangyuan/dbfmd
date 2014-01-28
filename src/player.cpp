#include "player.h"

Player::Player(QObject *parent) :
    QObject(parent), playing_(false), qmp_(this)
{
    connect(&qmp_, SIGNAL(positionChanged(qint64)),
            SIGNAL(positionChanged(qint64)));
    connect(&qmp_, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),
            SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)));
    connect(&qmp_, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),
            SLOT(mediaStatusChanged_(QMediaPlayer::MediaStatus)));
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

void Player::updateTrack(const QJsonObject trackInfo, QUrl url)
{
    // 1. stop current track
    qmp_.stop();

    // 2. update info
    trackInfo_ = trackInfo;

    // 3. setup use new url
    qDebug() << "player, play url:" << url;
    qmp_.setMedia(url);

    // 4. playr or pause
    if (playing_) {
        qmp_.play();
    }
}

QJsonObject Player::trackInfo()
{
    return trackInfo_;
}

void Player::mediaStatusChanged_(QMediaPlayer::MediaStatus status)
{
}

void Player::rate(QString sid)
{
    if (sid != trackInfo_["sid"].toString()) return;

    if (0 != trackInfo_["like"].toInt()) {
        trackInfo_["like"] = 0;
        emit(rateChanged(false));
    }
    else {
        trackInfo_["like"] = 1;
        emit(rateChanged(true));
    }
}
