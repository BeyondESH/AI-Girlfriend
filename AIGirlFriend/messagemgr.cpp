#include "messagemgr.h"
#include <QDebug>

MessageMgr::MessageMgr(QObject *parent)
    : QObject{parent}
{}

MessageMgr::~MessageMgr()
{

}

void MessageMgr::slots_handleAsrMessage(const QString &message)
{
    qDebug().noquote()<<message;
}
