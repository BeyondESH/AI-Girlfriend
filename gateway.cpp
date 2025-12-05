#include "gateway.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkReply>
#include <thread>
#include <QHttpMultiPart>
#include <QHttpPart>
#include <QFile>

GateWay::GateWay(QObject *parent)
    : QObject{parent}
    ,_websocket(new QWebSocket)
    ,_networkAccessMgr(new QNetworkAccessManager(this))
    ,_wsConnected(false)
{

    wsConnect(QUrl("ws://localhost:10096"));
    QObject::connect(this,&GateWay::signal_connectAsrWS,this,[this](){
        this->wsConnect(QUrl("ws://localhost:10096"));
    },Qt::QueuedConnection);

    std::thread([this](){
        while(this->_wsConnected==false){
            emit signal_connectAsrWS();
            std::this_thread::sleep_for(std::chrono::seconds(10));
        }
    }).detach();
    QObject::connect(this->_websocket,&QWebSocket::connected,[this](){
        qDebug()<<"websocket连接成功";
        _wsConnected=true;
    });
    QObject::connect(_websocket,&QWebSocket::errorOccurred,[this](){
        qDebug()<<"websocket出现错误:"<<_websocket->errorString();
        _wsConnected=false;
        std::thread([this](){
            while(this->_wsConnected==false){
                emit signal_connectAsrWS();
                std::this_thread::sleep_for(std::chrono::seconds(10));
            }
        }).detach();
    });
    QObject::connect(_websocket,&QWebSocket::disconnected,[this](){
        qDebug()<<"websocket断开连接";
        _wsConnected=false;
        std::thread([this](){
            while(this->_wsConnected==false){
                emit signal_connectAsrWS();
                std::this_thread::sleep_for(std::chrono::seconds(10));
            }
        }).detach();
    });

    QObject::connect(_websocket,&QWebSocket::textMessageReceived,[this](const QString &message){
        // 处理asr接收的文字
        QJsonDocument jsonDoc=QJsonDocument::fromJson(message.toUtf8());
        QJsonObject jsonObj=jsonDoc.object();
        QString mode=jsonObj["mode"].toString();
        if(mode!="2pass-offline"){
            return;
        }
        QString content=jsonObj["text"].toString();
        qDebug().noquote()<<"我说:"<<content;
        sendllmMessage(content,ReqId::VOICE_LLM);
    });
}

GateWay::~GateWay()
{
    if(_websocket!=nullptr){
        _websocket->deleteLater();
        _websocket=nullptr;
    }
}

void GateWay::wsConnect(const QUrl &url)
{
    _websocket->open(url);
    qDebug()<<"websocket连接中:"<<url.toDisplayString();
}

void GateWay::wsConnectAsrServer(const QUrl &url)
{
    _websocket->open(url);
    qDebug()<<"websocket连接ASR服务器中:"<<url.toDisplayString();
}

void GateWay::wsSendPcmData(const QByteArray &pcmData)
{
    int count=_websocket->sendBinaryMessage(pcmData);
    // qDebug()<<"发送字节数:"<<count;
}

void GateWay::wsSend(const QString &msg)
{
    int count=_websocket->sendTextMessage(msg);
    qDebug()<<"发送字节数:"<<count;
}

void GateWay::wsSendAsrConfig()
{
    QJsonObject jsonObj;
    jsonObj["mode"]="2pass";
    jsonObj["wav_name"] = "realtime_recording";
    jsonObj["is_speaking"] = true;
    jsonObj["wav_format"] = "pcm";
    QJsonArray chunkSizeArray;
    chunkSizeArray << 5 << 10 << 5;
    jsonObj["chunk_size"] = chunkSizeArray;
    jsonObj["audio_fs"] = 16000; // 采样率
    jsonObj["itn"] = true;
    QJsonDocument doc(jsonObj);
    QString jsonString=doc.toJson(QJsonDocument::Compact);
    wsSend(jsonString);
}

void GateWay::get(const QUrl &url,ReqId id)
{
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QNetworkReply* reply= _networkAccessMgr->get(request);
    connect(reply,&QNetworkReply::finished,[reply,id,this](){
        if(reply->error()!=QNetworkReply::NoError){
            QString errorString=reply->errorString();
            handle_http_finished(errorString.toUtf8(),id,ErrorCode::ERROR_NETWORK);
            return;
        }
        QByteArray data=reply->readAll();
        handle_http_finished(data,id,ErrorCode::SUCCESS);
        reply->deleteLater();
    });
}

void GateWay::post(const QUrl &url,QByteArray &data,ReqId id)
{
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QNetworkReply* reply=_networkAccessMgr->post(request,data);
    connect(reply,&QNetworkReply::finished,[reply,id,this](){
        if(reply->error()!=QNetworkReply::NoError){
            QString errorString=reply->errorString();
            handle_http_finished(errorString.toUtf8(),id,ErrorCode::ERROR_NETWORK);
            return;
        }
        QByteArray data=reply->readAll();
        handle_http_finished(data,id,ErrorCode::SUCCESS);
        reply->deleteLater();
    });
}

void GateWay::post(const QUrl &url, QHttpMultiPart *multiPart, ReqId id)
{
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,
                      "multipart/form-data; boundary=" + multiPart->boundary());
    QNetworkReply* reply=_networkAccessMgr->post(request,multiPart);
    connect(reply,&QNetworkReply::finished,[reply,id,this](){
        if(reply->error()!=QNetworkReply::NoError){
            QString errorString=reply->errorString();
            handle_http_finished(errorString.toUtf8(),id,ErrorCode::ERROR_NETWORK);
            return;
        }
        QByteArray data=reply->readAll();
        handle_http_finished(data,id,ErrorCode::SUCCESS);
        reply->deleteLater();
    });
}


void GateWay::slot_handlePcmData(const QByteArray &pcmData)
{
    wsSendPcmData(pcmData);
}

void GateWay::handle_http_finished(QByteArray data, ReqId id, ErrorCode ec)
{
    if(ec!=ErrorCode::SUCCESS){
        qWarning()<<"Network Error:"<<QString(data);
        return;
    }
    switch(id){
        case ReqId::CHAT_LLM:{
            QJsonDocument jsonDoc=QJsonDocument::fromJson(data);
            QJsonObject jsonObj=jsonDoc.object();
            QString content=jsonObj["message"].toObject()["content"].toString();
            qDebug().noquote()<<"她说:"<<content;
            emit signal_receive_llm(content);
            return;
        }
        case ReqId::VOICE_LLM:{
            QJsonDocument jsonDoc=QJsonDocument::fromJson(data);
            QJsonObject jsonObj=jsonDoc.object();
            QString content=jsonObj["message"].toObject()["content"].toString();
            qDebug().noquote()<<"她说:"<<content;
            sendttsMessage(content);
            return;
        }
        case ReqId::SEDN_TTS:{
            emit signal_tts_finished(data);
            return;
        }
    }
}

void GateWay::sendllmMessage(const QString &text,ReqId id)
{
    QJsonObject rootObj,messageObj;
    rootObj["model"]="qwen3:8b";
    rootObj["stream"]=false;
    rootObj["think"]=false;
    messageObj["role"]="user";
    QString content="你是动漫《名侦探柯南》里的女角色灰原哀，以第一人称视角回答:"+text;
    messageObj["content"]=content;
    QJsonArray messageArray;
    messageArray.append(messageObj);
    rootObj["messages"]=messageArray;
    QJsonDocument jsonDoc(rootObj);
    QByteArray data=jsonDoc.toJson(QJsonDocument::Indented);
    post(QUrl("http://localhost:11434/api/chat"),data,ReqId::CHAT_LLM);
}

void GateWay::sendttsMessage(const QString &text)
{
    // 创建multipart/form-data
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    // 添加tts_text字段
    QHttpPart ttsTextPart;
    ttsTextPart.setHeader(QNetworkRequest::ContentDispositionHeader,
                          QVariant("form-data; name=\"tts_text\""));
    ttsTextPart.setBody(text.toUtf8());
    multiPart->append(ttsTextPart);
    // 添加prompt_text字段
    QHttpPart promptTextPart;
    promptTextPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"prompt_text\""));
    promptTextPart.setBody(QString("不过，应该没事吧，如果那个姓黑田的人，真的就是你所怀疑的朗姆，又在那么近的距离看到我这张脸，照理说应该会察觉我就是背叛组织的雪莉，这个时候，应该早就闯进了博士家才对，但是刚才博士传来的简讯，都是在说今天晚餐的事").toUtf8());
    multiPart->append(promptTextPart);
    // prompt_wav 文件字段
    QHttpPart wavPart;
    wavPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"prompt_wav\"; filename=\"huiyuanai2.wav\""));
    wavPart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("audio/wav"));
    QFile file(":/audioSample/sample/huiyuanai2.WAV");
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "文件打开失败:" << file.errorString();
        return;
    }
    wavPart.setBody(file.readAll());
    file.close();
    multiPart->append(wavPart);

    post(QUrl("http://localhost:50000/inference_zero_shot"),multiPart,ReqId::SEDN_TTS);
}

void GateWay::slot_endAsrRecord()
{
    qDebug() << "slot_endAsrRecord called";
    QJsonObject jsonObj;
    jsonObj["is_speaking"]=false;
    QJsonDocument doc(jsonObj);
    QString jsonString=doc.toJson(QJsonDocument::Compact);
    wsSend(jsonString);
}


