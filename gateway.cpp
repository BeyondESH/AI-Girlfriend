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
#include <QTimer>
#include "configmgr.h"

GateWay::GateWay(QObject *parent)
    : QObject{parent}
    ,_websocket(new QWebSocket(QString(), QWebSocketProtocol::VersionLatest, this))
    ,_networkAccessMgr(new QNetworkAccessManager(this))
    ,_wsConnected(false)
    ,_reconnectTimer(new QTimer(this))
    ,_offlineProcessed(false)
    ,_isFileRecognizing(false)
{
    // 初始化重连定时器
    _reconnectTimer->setInterval(10000); // 10秒重连一次
    _reconnectTimer->setSingleShot(true); // 单次触发，避免重复
    connect(_reconnectTimer, &QTimer::timeout, this, [this](){
        if(!_wsConnected && _websocket->state() == QAbstractSocket::UnconnectedState){
            qDebug() << "尝试重连ASR服务器...";
            wsConnect(QUrl(ConfigMgr::instance().asrServerUrl()));
        }
    });

    // 连接信号槽（先设置好再连接）
    QObject::connect(this->_websocket,&QWebSocket::connected,[this](){
        qDebug()<<"websocket连接成功";
        _wsConnected=true;
        emit wsConnectedChanged();
        _reconnectTimer->stop();
    });
    
    QObject::connect(_websocket,&QWebSocket::errorOccurred,[this](QAbstractSocket::SocketError error){
        qDebug()<<"websocket出现错误:"<<error<<_websocket->errorString();
        _wsConnected=false;
        emit wsConnectedChanged();
        // 延迟重连
        if(!_reconnectTimer->isActive()){
            _reconnectTimer->start();
        }
    });
    
    QObject::connect(_websocket,&QWebSocket::disconnected,[this](){
        qDebug()<<"websocket断开连接";
        _wsConnected=false;
        emit wsConnectedChanged();
        // 延迟重连
        if(!_reconnectTimer->isActive()){
            _reconnectTimer->start();
        }
    });

    QObject::connect(_websocket,&QWebSocket::textMessageReceived,[this](const QString &message){
        // 处理asr接收的文字
        QJsonDocument jsonDoc=QJsonDocument::fromJson(message.toUtf8());
        QJsonObject jsonObj=jsonDoc.object();
        QString mode=jsonObj["mode"].toString();
        QString content=jsonObj["text"].toString();
        
        qDebug() << "ASR收到消息, mode:" << mode << "text:" << content;
        
        // 发送实时转写信号（非最终结果）
        if(mode=="2pass-online" && !content.isEmpty()){
            emit signal_asr_text(content, false);
        }
        
        // 处理离线识别结果（文件识别模式）
        if(mode=="offline"){
            emit signal_file_recognize_result(content);
            qDebug().noquote()<<"文件识别结果:"<<content;
            return;
        }
        
        // 处理 2pass 离线结果（实时录音模式）
        if(mode!="2pass-offline"){
            return;
        }
        
        // 文件识别模式下，每次 2pass-offline 结果都要处理（累积）
        if(_isFileRecognizing){
            // 文件识别模式：发送信号让 Application 累积结果
            emit signal_asr_text(content, true);
            qDebug() << "文件识别累积结果:" << content;
            return;
        }
        
        // 实时录音模式：检查是否已经处理过 offline 结果
        if(_offlineProcessed){
            qDebug()<<"已处理过 offline 结果，跳过本次";
            return;
        }
        _offlineProcessed = true;  // 标记为已处理
        
        // 发送停止录音信号（通知 UI 停止录音状态）
        emit signal_stop_recording();
        
        // 发送最终转写结果
        emit signal_asr_text(content, true);
        qDebug().noquote()<<"我说:"<<content;
        
        // 发送信号让 Application 处理语音消息（这样可以访问完整的对话历史）
        if(!content.trimmed().isEmpty()){
            emit signal_voice_message_ready(content);
        } else {
            qDebug()<<"用户未说话，跳过LLM请求";
        }
    });
    
    // 延迟首次连接，确保事件循环已启动
    QTimer::singleShot(100, this, [this](){
        wsConnect(QUrl(ConfigMgr::instance().asrServerUrl()));
    });
}

GateWay::~GateWay()
{
    _reconnectTimer->stop();
    if(_websocket != nullptr){
        _websocket->close();
    }
    // websocket 已设置父对象，会自动删除
}

void GateWay::wsConnect(const QUrl &url)
{
    qDebug()<<"websocket准备连接:"<<url.toString()<<"当前状态:"<<_websocket->state();
    
    // 如果正在连接或已连接，不要重复操作
    if(_websocket->state() == QAbstractSocket::ConnectingState) {
        qDebug()<<"websocket正在连接中，跳过";
        return;
    }
    
    if(_websocket->state() == QAbstractSocket::ConnectedState) {
        qDebug()<<"websocket已连接，跳过";
        return;
    }
    
    // 如果有未完成的连接，先关闭
    if(_websocket->state() != QAbstractSocket::UnconnectedState) {
        qDebug()<<"关闭现有连接...";
        _websocket->abort();
        // 等待状态变为未连接后再尝试
        QTimer::singleShot(500, this, [this, url](){
            if(_websocket->state() == QAbstractSocket::UnconnectedState) {
                _websocket->open(url);
            }
        });
        return;
    }
    
    qDebug()<<"websocket开始连接:"<<url.toString();
    _websocket->open(url);
}

void GateWay::wsConnectAsrServer(const QUrl &url)
{
    _websocket->open(url);
    qDebug()<<"websocket连接ASR服务器中:"<<url.toDisplayString();
}

void GateWay::resetAsrSession()
{
    _offlineProcessed = false;
    _isFileRecognizing = false;
    qDebug()<<"ASR会话已重置";
}

void GateWay::setFileRecognizeMode(bool isFileRecognize)
{
    _isFileRecognizing = isFileRecognize;
    qDebug() << "文件识别模式:" << isFileRecognize;
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
    
    // TTS 请求和其他请求使用相同的处理方式
    connect(reply,&QNetworkReply::finished,[reply,id,this](){
        if(reply->error()!=QNetworkReply::NoError){
            QString errorString=reply->errorString();
            handle_http_finished(errorString.toUtf8(),id,ErrorCode::ERROR_NETWORK);
            reply->deleteLater();
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
            // 先发射LLM回复信号，让UI立即显示消息
            emit signal_receive_llm(content);
            // 然后发送TTS合成请求
            sendttsMessage(content);
            return;
        }
        case ReqId::SEDN_TTS:{
            emit signal_tts_finished(data);
            return;
        }
    }
}

void GateWay::sendllmMessage(const QString &text, ReqId id, const QJsonArray& chatHistory)
{
    ConfigMgr &config = ConfigMgr::instance();
    
    QJsonObject rootObj;
    rootObj["model"] = config.llmModelName();
    rootObj["stream"] = false;
    rootObj["think"] = false;
    
    QJsonArray messageArray;
    
    // 添加系统提示词
    QString systemPrompt = config.buildFullSystemPrompt();
    if (!systemPrompt.isEmpty()) {
        QJsonObject systemMsgObj;
        systemMsgObj["role"] = "system";
        systemMsgObj["content"] = systemPrompt;
        messageArray.append(systemMsgObj);
    }
    
    // 添加历史对话消息（实现上下文记忆）
    for (const QJsonValue &msg : chatHistory) {
        QJsonObject historyMsg = msg.toObject();
        QJsonObject msgObj;
        QString role = historyMsg["role"].toString();
        // 转换角色名称为Ollama格式
        if (role == "User") {
            msgObj["role"] = "user";
        } else if (role == "Assistant") {
            msgObj["role"] = "assistant";
        } else {
            continue; // 跳过未知角色
        }
        msgObj["content"] = historyMsg["content"].toString();
        messageArray.append(msgObj);
    }
    
    // 添加当前用户消息
    QJsonObject messageObj;
    messageObj["role"] = "user";
    messageObj["content"] = text;
    messageArray.append(messageObj);
    
    rootObj["messages"] = messageArray;
    QJsonDocument jsonDoc(rootObj);
    QByteArray data = jsonDoc.toJson(QJsonDocument::Indented);
    
    qDebug() << "发送LLM请求，消息数量:" << messageArray.size();
    post(QUrl(config.llmServerUrl()), data, id);
}

void GateWay::sendttsMessage(const QString &text)
{
    ConfigMgr &config = ConfigMgr::instance();
    
    // 创建multipart/form-data
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    
    // 添加tts_text字段
    QHttpPart ttsTextPart;
    ttsTextPart.setHeader(QNetworkRequest::ContentDispositionHeader,
                          QVariant("form-data; name=\"tts_text\""));
    ttsTextPart.setBody(text.toUtf8());
    multiPart->append(ttsTextPart);
    
    // 添加prompt_text字段（从配置获取）
    QHttpPart promptTextPart;
    promptTextPart.setHeader(QNetworkRequest::ContentDispositionHeader, 
                             QVariant("form-data; name=\"prompt_text\""));
    promptTextPart.setBody(config.ttsPromptText().toUtf8());
    multiPart->append(promptTextPart);
    
    // prompt_wav 文件字段（从配置获取）
    QHttpPart wavPart;
    wavPart.setHeader(QNetworkRequest::ContentDispositionHeader, 
                      QVariant("form-data; name=\"prompt_wav\"; filename=\"sample.wav\""));
    wavPart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("audio/wav"));
    
    QString wavPath = config.ttsPromptWav();
    // 处理Qt资源文件路径
    if (wavPath.startsWith(":/")) {
        wavPath = ":" + wavPath.mid(2); // 确保格式正确
    }
    
    QFile file(wavPath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "TTS音频文件打开失败:" << file.errorString() << "路径:" << wavPath;
        delete multiPart;
        return;
    }
    wavPart.setBody(file.readAll());
    file.close();
    multiPart->append(wavPart);
    
    qDebug() << "发送TTS请求，文本:" << text.left(50) << "... 样本路径:" << wavPath;

    post(QUrl(config.ttsServerUrl()), multiPart, ReqId::SEDN_TTS);
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
