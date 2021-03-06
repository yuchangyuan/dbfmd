#include "control.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QUrl>

Control::Control(Client *client, Player *player, QObject *parent) :
    QObject(parent), client_(client), player_(player)
{
    connect(player_, SIGNAL(positionChanged(qint64)),
        SLOT(playerPositionChanged(qint64)));
    connect(player_, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),
            SLOT(playerMediaStatusChanged(QMediaPlayer::MediaStatus)));
    connect(client_, SIGNAL(operationFinish(Client::OpType,bool,QString)),
            SLOT(clientOperationFinish(Client::OpType,bool,QString)));
}


void Control::start()
{
    client_->refreshChannel();
    client_->doUpdatePlaylist(0);
    player_->play();
}


void Control::updateTrack()
{
    QJsonArray playlist = client_->currentPlaylist();
    int track = client_->currentTrack();
    QJsonObject trackInfo = playlist[track].toObject();
    QUrl url(trackInfo["url"].toString());

    player_->updateTrack(trackInfo, url);
}

void Control::playerPositionChanged(qint64 pos)
{
    // qDebug() << "player pos: " << pos;
}

void Control::playerMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    qDebug() << "player status:" << status;

    if (QMediaPlayer::LoadingMedia == status) {
        qDebug() << "info" << player_->trackInfo();
    }

    if (QMediaPlayer::BufferedMedia == status) {
    }

    if (QMediaPlayer::EndOfMedia == status) {
        QString msg;
        client_->doEnd(msg);
    }
}

void Control::clientOperationFinish(Client::OpType type, bool success,
                     QString message)
{
    if (!success) {
    // TODO
        qDebug() << "client op fail, " << "type:" << type << "message:"
                 << message;
        return;
    }

    switch (type) {
    case Client::OP_UPDTE_PLAYLIST:
        updateTrack();
        break;
    case Client::OP_END:
    case Client::OP_SKIP:
    case Client::OP_TRASH:
        if (client_->playlistEmpty()) {
            client_->doUpdatePlaylist();
        }
        else {
            updateTrack();
        }
        break;
    case Client::OP_LIKE:
    case Client::OP_UNLIKE:
        // TODO: update player_
        // NOTE: should handle the condition when rate called before End/Skip,
        // but return after End/Skip,
        player_->rate(message, (type == Client::OP_LIKE)); // message is sid
        break;
    case Client::OP_REFRESH_CHANNEL:
        break;
    }
}

bool Control::doOperation(Client::OpType type, QString &message, int ch_id)
{
    return client_->doOperation(type, message, ch_id);
}

