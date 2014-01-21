#ifndef LOGIN_H
#define LOGIN_H

#include <QObject>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTimerEvent>

class Login : public QObject
{
    Q_OBJECT

private:
    QString email_;
    QString password_;

    QNetworkAccessManager manager_;

public:
    explicit Login(QObject *parent = 0);

    void doInput(QString email = 0, QString password = 0);
    void doLogin();

signals:

public slots:
    void loginReply(QNetworkReply *reply);
};

#endif // LOGIN_H
