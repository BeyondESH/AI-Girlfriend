#include "httpmgr.h"
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

HttpMgr::HttpMgr(QObject *parent)
    : QObject{parent},_networkAccessMgr(new QNetworkAccessManager(this))
{
    connect(this,&HttpMgr::signal_http_finished,this,&HttpMgr::slot_http_finished);
}

void HttpMgr::get(const QUrl &url,ReqId id)
{
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QNetworkReply* reply= _networkAccessMgr->get(request);
    connect(reply,&QNetworkReply::finished,[reply,id,this](){
        if(reply->error()!=QNetworkReply::NoError){
            QString errorString=reply->errorString();
            emit signal_http_finished(errorString,id,ErrorCode::ERROR_NETWORK);
            return;
        }
        QString data=reply->readAll();
        emit signal_http_finished(data,id,ErrorCode::SUCCESS);
        reply->deleteLater();
    });
}

void HttpMgr::post(const QUrl &url, ReqId id,QByteArray &data)
{
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QNetworkReply* reply=_networkAccessMgr->post(request,data);
    connect(reply,&QNetworkReply::finished,[reply,id,this](){
        if(reply->error()!=QNetworkReply::NoError){
            QString errorString=reply->errorString();
            emit signal_http_finished(errorString,id,ErrorCode::ERROR_NETWORK);
            return;
        }
        QString data=reply->readAll();
        emit signal_http_finished(data,id,ErrorCode::SUCCESS);
        reply->deleteLater();
    });
}

void HttpMgr::slot_http_finished(QString data,ReqId id,ErrorCode ec)
{
    if(ec!=ErrorCode::SUCCESS){
        qWarning()<<"Network Error:"<<data;
        return;
    }
    switch(id){
        case ReqId::ID_SEND_CHAT:{
            QJsonDocument jsonDoc=QJsonDocument::fromJson(data.toUtf8());
            QJsonObject jsonObj=jsonDoc.object();
            QString content=jsonObj["message"].toObject()["content"].toString();
            qDebug().noquote()<<"她说:"<<content;
            emit signal_llmResponse_finished();
            return;
        }
        case ReqId::ID_TEST:{
            qDebug()<<data;
            return;
        }
    }
}

void HttpMgr::slot_signal_sendllmMessage(const QString &text)
{
    QJsonObject rootObj,messageObj;
    rootObj["model"]="qwen3:8b";
    rootObj["stream"]=false;
    rootObj["think"]=false;
    messageObj["role"]="user";
    QString content="请以用户女友的身份回答:"+text;
    messageObj["content"]=content;
    QJsonArray messageArray;
    messageArray.append(messageObj);
    rootObj["messages"]=messageArray;
    QJsonDocument jsonDoc(rootObj);
    QByteArray data=jsonDoc.toJson(QJsonDocument::Indented);
    HttpMgr *httpMgr=new HttpMgr();
    httpMgr->post(QUrl("http://localhost:11434/api/chat"),ReqId::ID_SEND_CHAT,data);
}
