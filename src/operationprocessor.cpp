#include "operationprocessor.h"
#include <QJsonDocument>
#include <QJsonObject>

OperationProcessor::OperationProcessor(QObject *parent) :
    QObject(parent)
{

}

void OperationProcessor::setReply(QNetworkReply *reply, Client::OpType type)
{
    this->type_ = type;
    this->reply_ = reply;

    connect(reply, SIGNAL(finished()), SLOT(ready()));
}

void OperationProcessor::ready()
{
    QByteArray data = reply_->readAll();
    // qDebug() << "type" << type_ << "data" << data;
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(data, &err);
    if (QJsonParseError::NoError == err.error) {
        emit(finish(type_, true, "", doc.object()));
    }
    else {
        emit(finish(type_, false, err.errorString(), QJsonObject()));
    }

    // delete later
    this->deleteLater();
}
