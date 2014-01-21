#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QNetworkAccessManager>

class Client : public QObject
{
    Q_OBJECT

public:
    // type
    typedef enum OpType_ {
        OP_END, OP_SKIP, OP_RATE, OP_TRASH,
        OP_UPDTE_PLAYLIST, OP_REFRESH_CHANNEL
    } OpType;

private:

    QString user_id_;
    qint64 expire_;
    QString token_;

    // has login info?
    bool login_;

    // network access manager
    QNetworkAccessManager manager_;

    // playlist, and current track, index in playlist
    QJsonArray *playlist_;
    int track_;

    // channel list and current channel id
    QJsonArray *channel_;
    int ch_id_;


    // function
    void doOperation_(OpType type, QUrl url);

public:
    explicit Client(QObject *parent = 0);
    ~Client();

    void login(QString user_id, qint64 expire, QString token);
    void logout();
    static Client* create(QString login_json, QObject *parent = 0);

    void refreshChannel();
    QJsonArray* channelList();

    // song operation, should only operate in single thread
    void doOperation(OpType type, int ch_id = -1);

    void doEnd(); // song finish normally
    void doSkip(); // current song skipped
    void doRate(); // like/unlike current song
    void doTrash(); // trash current song
    void doUpdatePlaylist(int ch_id = -1);  // get playlist

    // current status
    QJsonArray* currentPlaylist();
    int currentTrack();
    int currentChannel();

signals:
    void operationFinish(OpType type, bool success, QString message);

public slots:

private slots:
    void operationFinish_(OpType type, bool success, QString message, QJsonObject *obj);
};

#endif // CLIENT_H
