#include "login.h"
#include <QDebug>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QCoreApplication>

Login::Login(QObject *parent) :
    QObject(parent), manager_(this)
{
    connect(&manager_, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(loginReply(QNetworkReply*)));
}

void Login::doInput(QString email, QString password)
{
    QTextStream cerr(stderr);
    QTextStream cin(stdin);
    if (email == 0) {
        cerr << "email: " << flush;
        email_ = cin.readLine();
    }
    else {
        email_ = email;
    }

    if (password == 0) {
        cerr << "password: " << flush;
        password_ = cin.readLine();
    }
    else {
        password_ = password;
    }

    //qDebug() << "email = " << email_ << ", password = " << password_;
}

void Login::doLogin()
{
    qDebug() << "do login";

    QNetworkRequest req(QUrl("http://www.douban.com/j/app/login"));

    QUrlQuery q;
    q.addQueryItem("app_name", "radio_desktop_win");
    q.addQueryItem("version", "100");
    q.addQueryItem("email", email_);
    q.addQueryItem("password", password_);


    req.setHeader(QNetworkRequest::ContentTypeHeader,
                  "application/x-www-form-urlencoded");

    manager_.post(req, q.toString().toLocal8Bit());
}

void Login::loginReply(QNetworkReply* reply)
{
    QByteArray ret = reply->readAll();

    qDebug() << QJsonDocument::fromJson(ret);

    QTextStream cout(stdout);
    cout << ret << endl;

    // exit system
    QCoreApplication::exit(EXIT_SUCCESS);
}
