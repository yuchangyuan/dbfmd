#include <QCoreApplication>
#include <QTextStream>
#include <QDebug>
#include <QString>
#include "login.h"
#include "client.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if (a.arguments().size() < 2) {
        qDebug() << "Not specify token file, do login.";

        // NOTE: should create on heap
        Login* login = new Login(&a);

        login->doInput();
        login->doLogin();
    }
    else {
        Client *client = Client::create(a.arguments()[1], &a);
        if (0 == client) {
            QCoreApplication::exit(EXIT_FAILURE);
        }
        client->refreshChannel();
        client->doUpdatePlaylist();
    }

    return a.exec();
}
