#include "operationprocessor.h"

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
    emit(finish(type_, true, "", 0));


    // delete later
    this->deleteLater();
}
