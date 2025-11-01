#include "websocketmgr.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

WebSocketMgr::WebSocketMgr(QObject *parent)
    : QObject{parent},_websocket(nullptr),_isConnected(false)
{
    _websocket=new QWebSocket;
    QObject::connect(this->_websocket,&QWebSocket::connected,[this](){
        qDebug()<<"websocket连接成功";
        _isConnected=true;
    });
    QObject::connect(_websocket,&QWebSocket::errorOccurred,[this](){
        qDebug()<<"websocket出现错误:"<<_websocket->errorString();
    });
    QObject::connect(_websocket,&QWebSocket::disconnected,[this](){
        qDebug()<<"websocket断开连接";
        _isConnected=false;
    });
}

WebSocketMgr::~WebSocketMgr()
{
    stop();
    if(_websocket!=nullptr){
        _websocket->deleteLater();
        _websocket=nullptr;
    }
}

void WebSocketMgr::connect(const QUrl &url)
{
    _websocket->open(url);
    qDebug()<<"websocket连接中:"<<url.toDisplayString();
}

void WebSocketMgr::connectAsrServer(const QUrl &url)
{
    _websocket->open(url);
    qDebug()<<"websocket连接ASR服务器中:"<<url.toDisplayString();
}

void WebSocketMgr::sendPcmData(const QByteArray &pcmData)
{

}

void WebSocketMgr::send(const QString &msg)
{
    int cout=_websocket->sendTextMessage(msg);
    qDebug()<<"发送字节数:"<<cout;
}

void WebSocketMgr::sendConfig()
{
    QJsonObject jsonObj;
    jsonObj["mode"]="2pass";
    jsonObj["wav_name"] = "realtime_recording";
    jsonObj["is_speaking"] = true;
    jsonObj["wav_format"] = "pcm";
    QJsonArray chunkSizeArray;
    chunkSizeArray << 5 << 10 << 5;
    jsonObj["chunk_size"] = chunkSizeArray;
    jsonObj["audio_fs"] = 16000; // 默认采样率
    jsonObj["itn"] = true;
    QJsonDocument doc(jsonObj);
    QString jsonString=doc.toJson(QJsonDocument::Compact);
    send(jsonString);
}

void WebSocketMgr::sendConfig(const int &sampleRate)
{
    QJsonObject jsonObj;
    jsonObj["mode"]="2pass";
    jsonObj["wav_name"] = "realtime_recording";
    jsonObj["is_speaking"] = true;
    jsonObj["wav_format"] = "pcm";
    QJsonArray chunkSizeArray;
    chunkSizeArray << 5 << 10 << 5;
    jsonObj["chunk_size"] = chunkSizeArray;
    jsonObj["audio_fs"] = sampleRate; // 采样率
    jsonObj["itn"] = true;
    QJsonDocument doc(jsonObj);
    QString jsonString=doc.toJson(QJsonDocument::Compact);
    send(jsonString);
}

void WebSocketMgr::stop()
{
    _isConnected=false;
}

bool WebSocketMgr::isConnected() const
{
    return _isConnected.load();
}

void WebSocketMgr::slot_handlePcmData(const QByteArray &pcmData)
{

}
