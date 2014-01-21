#include <QCoreApplication>
#include <QTextStream>
#include <QDebug>
#include <QString>
#include "login.h"
#include <QNetworkAccessManager>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if (a.arguments().size() < 2) {
        qDebug() << "Not specify token file, do login.";

        // NOTE: should use create on heap
        Login* login = new Login(&a);

        login->doInput();
        login->doLogin();
    }

    return a.exec();
}
