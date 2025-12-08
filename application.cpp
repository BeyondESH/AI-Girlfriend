#include "application.h"
#include "configmgr.h"
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QDateTime>
#include <QUuid>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QHttpMultiPart>

Application::Application(QObject *parent)
    : QObject{parent}
    ,_audioMgr(new AudioMgr(this))
    ,_gateWay(new GateWay(this))
{
    connect(_audioMgr,&AudioMgr::signal_handlePcmData,_gateWay,&GateWay::slot_handlePcmData);
    connect(_audioMgr,&AudioMgr::signal_endAsrRecord,_gateWay,&GateWay::slot_endAsrRecord);
    connect(_gateWay,&GateWay::signal_tts_finished,_audioMgr,&AudioMgr::slot_tts_finished);
    connect(_gateWay,&GateWay::signal_asr_text,this,&Application::signal_asr_text);
    
    // 连接TTS播放完成信号
    connect(_audioMgr,&AudioMgr::signal_tts_playback_finished,this,&Application::signal_tts_playback_finished);
    
    // 连接LLM回复信号，添加到历史并转发
    connect(_gateWay,&GateWay::signal_receive_llm,this,[this](const QString &content){
        addMessageToHistory("Assistant", content);
        emit signal_receive_llm(content);
    });
    
    // 初始化历史对话目录
    _historyDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/conversations";
    QDir dir;
    if (!dir.exists(_historyDir)) {
        dir.mkpath(_historyDir);
    }
    
    loadConversationList();
    // _audioMgr->test();
}

Application::~Application()
{
    saveCurrentConversation();
}

void Application::sendAsrConfig()
{
    _gateWay->wsSendAsrConfig();
}

void Application::sendChatMessage(QString text)
{
    addMessageToHistory("User", text);
    // 传递聊天历史以实现上下文记忆（不包含刚添加的用户消息，因为sendllmMessage会自己添加）
    QJsonArray historyWithoutLast = _chatHistory;
    if (!historyWithoutLast.isEmpty()) {
        historyWithoutLast.removeLast(); // 移除刚添加的用户消息
    }
    _gateWay->sendllmMessage(text, ReqId::CHAT_LLM, historyWithoutLast);
}

void Application::startVoiceRecord()
{
    auto state = _audioMgr->state();
    if (state == QtAudio::StoppedState) {
        sendAsrConfig();
        _audioMgr->recordAsr();
    }
}

void Application::stopVoiceRecord()
{
    auto state = _audioMgr->state();
    if (state == QtAudio::ActiveState || state == QtAudio::SuspendedState) {
        _audioMgr->asrStop();
    }
}

void Application::toggleVoiceRecord()
{
    on_pushButtonRecord_clicked();
}

// 历史对话管理
void Application::createNewConversation()
{
    saveCurrentConversation();
    
    _currentConversationId = generateConversationId();
    _chatHistory = QJsonArray();
    
    QJsonObject newConv;
    newConv["id"] = _currentConversationId;
    newConv["title"] = "新对话";
    newConv["createTime"] = QDateTime::currentDateTime().toString(Qt::ISODate);
    newConv["updateTime"] = QDateTime::currentDateTime().toString(Qt::ISODate);
    
    _conversationList.prepend(newConv);
    saveConversationList();
    
    emit currentConversationIdChanged();
    emit chatHistoryChanged();
    emit conversationListChanged();
}

void Application::loadConversation(const QString &conversationId)
{
    saveCurrentConversation();
    
    QString filePath = _historyDir + "/" + conversationId + ".json";
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        _chatHistory = doc.array();
        file.close();
    } else {
        _chatHistory = QJsonArray();
    }
    
    _currentConversationId = conversationId;
    emit currentConversationIdChanged();
    emit chatHistoryChanged();
}

void Application::deleteConversation(const QString &conversationId)
{
    // 删除文件
    QString filePath = _historyDir + "/" + conversationId + ".json";
    QFile::remove(filePath);
    
    // 从列表中移除
    for (int i = 0; i < _conversationList.size(); ++i) {
        if (_conversationList[i].toObject()["id"].toString() == conversationId) {
            _conversationList.removeAt(i);
            break;
        }
    }
    saveConversationList();
    
    // 如果删除的是当前对话，创建新对话
    if (_currentConversationId == conversationId) {
        if (_conversationList.isEmpty()) {
            createNewConversation();
        } else {
            loadConversation(_conversationList[0].toObject()["id"].toString());
        }
    }
    
    emit conversationListChanged();
}

void Application::addMessageToHistory(const QString &role, const QString &content)
{
    if (_currentConversationId.isEmpty()) {
        createNewConversation();
    }
    
    QJsonObject msg;
    msg["role"] = role;
    msg["content"] = content;
    msg["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);
    
    _chatHistory.append(msg);
    
    // 更新对话标题（使用第一条消息）
    if (_chatHistory.size() == 1 && role == "User") {
        for (int i = 0; i < _conversationList.size(); ++i) {
            QJsonObject conv = _conversationList[i].toObject();
            if (conv["id"].toString() == _currentConversationId) {
                QString title = content.left(20);
                if (content.length() > 20) title += "...";
                conv["title"] = title;
                conv["updateTime"] = QDateTime::currentDateTime().toString(Qt::ISODate);
                _conversationList[i] = conv;
                break;
            }
        }
        saveConversationList();
        emit conversationListChanged();
    }
    
    emit chatHistoryChanged();
    saveCurrentConversation();
}

void Application::clearCurrentHistory()
{
    _chatHistory = QJsonArray();
    saveCurrentConversation();
    emit chatHistoryChanged();
}

QJsonArray Application::getConversationList()
{
    return _conversationList;
}

void Application::checkOllamaStatus()
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest request(QUrl("http://localhost:11434/api/tags"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    
    QNetworkReply *reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply, manager]() {
        bool online = (reply->error() == QNetworkReply::NoError);
        _ollamaOnline = online;
        QString info = online ? "Ollama 服务正常运行" : reply->errorString();
        
        if (online) {
            QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
            QJsonArray models = doc.object()["models"].toArray();
            info = QString("已加载 %1 个模型").arg(models.size());
        }
        
        emit ollamaStatusChanged(online, info);
        reply->deleteLater();
        manager->deleteLater();
    });
}

void Application::checkAsrStatus()
{
    // ASR 使用 WebSocket，检查连接状态
    bool online = _gateWay->wsConnected();
    _asrOnline = online;
    emit asrStatusChanged(online, online ? "FunASR 服务连接正常" : "FunASR 服务未连接");
}

void Application::checkTtsStatus()
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest request(QUrl("http://localhost:50000/"));
    
    QNetworkReply *reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply, manager]() {
        bool online = (reply->error() == QNetworkReply::NoError || 
                      reply->error() == QNetworkReply::ContentNotFoundError);
        _ttsOnline = online;
        QString info = online ? "CosyVoice 服务正常运行" : reply->errorString();
        emit ttsStatusChanged(online, info);
        reply->deleteLater();
        manager->deleteLater();
    });
}

QJsonArray Application::chatHistory() const
{
    return _chatHistory;
}

QJsonArray Application::conversationList() const
{
    return _conversationList;
}

QString Application::currentConversationId() const
{
    return _currentConversationId;
}

void Application::setCurrentConversationId(const QString &id)
{
    if (_currentConversationId != id) {
        loadConversation(id);
    }
}

void Application::saveCurrentConversation()
{
    if (_currentConversationId.isEmpty() || _chatHistory.isEmpty()) {
        return;
    }
    
    QString filePath = _historyDir + "/" + _currentConversationId + ".json";
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly)) {
        QJsonDocument doc(_chatHistory);
        file.write(doc.toJson());
        file.close();
    }
}

void Application::loadConversationList()
{
    QString listPath = _historyDir + "/conversations.json";
    QFile file(listPath);
    if (file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        _conversationList = doc.array();
        file.close();
    } else {
        _conversationList = QJsonArray();
    }
}

void Application::saveConversationList()
{
    QString listPath = _historyDir + "/conversations.json";
    QFile file(listPath);
    if (file.open(QIODevice::WriteOnly)) {
        QJsonDocument doc(_conversationList);
        file.write(doc.toJson());
        file.close();
    }
}

QString Application::generateConversationId()
{
    return QUuid::createUuid().toString(QUuid::WithoutBraces);
}

void Application::recognizeAudioFile(const QString &filePath)
{
    _audioRecognizeStatus = "正在识别...";
    emit audioRecognizeStatusChanged();
    
    // 使用FunASR离线识别音频文件
    // FunASR 2pass服务器的HTTP接口通常在同一端口
    ConfigMgr &config = ConfigMgr::instance();
    QString asrUrl = config.asrServerUrl();
    
    // 尝试使用HTTP接口（FunASR可能支持的端口）
    QString httpUrl = asrUrl;
    httpUrl.replace("ws://", "http://");
    httpUrl.replace("wss://", "https://");
    // 尝试不同的端口：10095(离线), 10096(2pass), 10097(备用)
    // 先尝试原端口
    
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        _audioRecognizeStatus = "文件打开失败";
        emit audioRecognizeStatusChanged();
        return;
    }
    
    QByteArray audioData = file.readAll();
    file.close();
    
    // 创建multipart请求
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    
    QHttpPart audioPart;
    audioPart.setHeader(QNetworkRequest::ContentDispositionHeader,
                        QVariant("form-data; name=\"file\"; filename=\"sample.wav\""));
    audioPart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("audio/wav"));
    audioPart.setBody(audioData);
    multiPart->append(audioPart);
    
    QUrl requestUrl(httpUrl);
    QNetworkRequest request(requestUrl);
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkReply *reply = manager->post(request, multiPart);
    multiPart->setParent(reply);
    
    connect(reply, &QNetworkReply::finished, this, [this, reply, manager]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray data = reply->readAll();
            qDebug() << "识别响应:" << data;
            QJsonDocument doc = QJsonDocument::fromJson(data);
            QString text = doc.object()["text"].toString();
            if (text.isEmpty()) {
                // 尝试其他可能的字段名
                text = doc.object()["result"].toString();
            }
            if (!text.isEmpty()) {
                ConfigMgr::instance().setTtsPromptText(text);
                _audioRecognizeStatus = "识别完成";
            } else {
                _audioRecognizeStatus = "识别结果为空";
            }
        } else {
            // HTTP识别失败，提示用户手动输入
            _audioRecognizeStatus = "识别服务不可用，请手动输入";
            qDebug() << "音频识别失败:" << reply->errorString();
        }
        emit audioRecognizeStatusChanged();
        reply->deleteLater();
        manager->deleteLater();
    });
}

void Application::on_pushButtonRecord_clicked()
{
    auto state = _audioMgr->state();
    qDebug() << "当前音频状态:" << state;
    switch (state) {
    case QtAudio::StoppedState:
        sendAsrConfig();
        _audioMgr->recordAsr();
        break;
    case QtAudio::ActiveState:
        _audioMgr->suspend();
        break;
    case QtAudio::SuspendedState:
        _audioMgr->resume();
        break;
    default:
        _audioMgr->asrStop();
        break;
    }
}
