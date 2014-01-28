#ifndef OPERATIONPROCESSOR_H
#define OPERATIONPROCESSOR_H

#include <QObject>
#include <QNetworkReply>
#include <QJsonObject>

#include "client.h"

class OperationProcessor : public QObject
{
    Q_OBJECT

    QNetworkReply* reply_;
    Client::OpType type_;
    QString sid_;

public:
    explicit OperationProcessor(QObject *parent = 0);

    // set reply, connect signal to ready, sid is used when doEnd/doSkip/doRate
    void setReply(QNetworkReply *reply, Client::OpType type, QString sid);

signals:
    // when success, message is sid
    void finish(Client::OpType type, bool success, QString message, QJsonObject obj);

public slots:
    // emmit signal finish
    void ready();
};

#endif // OPERATIONPROCESSOR_H
