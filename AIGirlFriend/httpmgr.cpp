#include "httpmgr.h"
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>

HttpMgr::HttpMgr(QObject *parent)
    : QObject{parent},_networkAccessMgr(new QNetworkAccessManager(this))
{
    connect(this,&HttpMgr::signals_http_finished,this,&HttpMgr::slots_http_finished);
}

void HttpMgr::get(const QUrl &url,ReqId id)
{
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QNetworkReply* reply= _networkAccessMgr->get(request);
    connect(reply,&QNetworkReply::finished,[reply,id,this](){
        if(reply->error()!=QNetworkReply::NoError){
            QString errorString=reply->errorString();
            emit signals_http_finished(errorString,id,ErrorCode::ERROR_NETWORK);
            return;
        }
        QString data=reply->readAll();
        emit signals_http_finished(data,id,ErrorCode::SUCCESS);
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
            emit signals_http_finished(errorString,id,ErrorCode::ERROR_NETWORK);
            return;
        }
        QString data=reply->readAll();
        emit signals_http_finished(data,id,ErrorCode::SUCCESS);
        reply->deleteLater();
    });
}

void HttpMgr::slots_http_finished(QString data,ReqId id,ErrorCode ec)
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
            return;
        }
        case ReqId::ID_TEST:{
            qDebug()<<data;
            return;
        }
    }
}
