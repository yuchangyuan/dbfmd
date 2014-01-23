#include <QTextStream>
#include <QDebug>
#include <QString>
#include "login.h"
#include "client.h"
#include "control.h"

#ifdef Q_OS_OSX
#include <QGuiApplication>
#else
#include <QCoreApplication>
#endif

int main(int argc, char *argv[])
{
#ifdef Q_OS_OSX
    QGuiApplication a(argc, argv);
#else
    QCoreApplication a(argc, argv);
#endif

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
        Player *player = new Player(&a);
        Control *control = new Control(client, player, &a);
        control->start();
    }

    return a.exec();
}
