#include "messagemgr.h"
#include <QDebug>
#include <QDateTime>
#include <QJsonObject>
#include <QJsonDocument>


MessageMgr::MessageMgr(QObject *parent)
    : QObject{parent},_isInCommunication(false)
{}

MessageMgr::~MessageMgr()
{

}

void MessageMgr::slots_handleAsrMessage(const QString &message)
{
    qDebug()<<message;

    QJsonDocument jsonDoc=QJsonDocument::fromJson(message.toUtf8());
    QJsonObject jsonObj=jsonDoc.object();
    QString mode=jsonObj["mode"].toString();
    if(mode!="2pass-offline"){
        return;
    }
    QString text=jsonObj["text"].toString();
    qDebug()<<"我说:"<<text;
    /////
    /// 仅测试
    emit signal_sendllmMessage(text);  // 队列交给多线程，这里仅作开发测试

    qint64 timestamp=QDateTime::currentMSecsSinceEpoch();
    Message msg(1,text,true,timestamp);
    _mutex.lock();
    _msgQueue.enqueue(msg);
    _mutex.unlock();
}

void MessageMgr::slot_llmResponse_finished()
{
    _isInCommunication=false;
}
