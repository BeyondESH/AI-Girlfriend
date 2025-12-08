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
    connect(_gateWay,&GateWay::signal_stop_recording,this,&Application::signal_stop_recording);
    
    // 连接语音消息信号，在这里处理以便访问完整的对话历史
    connect(_gateWay, &GateWay::signal_voice_message_ready, this, [this](const QString &text) {
        // 添加用户消息到历史
        addMessageToHistory("User", text);
        // 传递聊天历史以实现上下文记忆（不包含刚添加的用户消息）
        QJsonArray historyWithoutLast = _chatHistory;
        if (!historyWithoutLast.isEmpty()) {
            historyWithoutLast.removeLast();
        }
        _gateWay->sendllmMessage(text, ReqId::VOICE_LLM, historyWithoutLast);
    });
    
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
    
    // 启动时不自动加载历史对话，让用户看到问候语界面
    // 用户可以从侧边栏历史对话中手动选择加载
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
        _gateWay->resetAsrSession();  // 重置 ASR 会话，允许处理新的 offline 结果
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
    QNetworkRequest request(QUrl("http://127.0.0.1:11434/api/tags"));
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
    QNetworkRequest request(QUrl("http://127.0.0.1:50000/"));
    
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
    
    // 检查 WebSocket 是否已连接
    if (!_gateWay->wsConnected()) {
        _audioRecognizeStatus = "ASR服务未连接";
        emit audioRecognizeStatusChanged();
        return;
    }
    
    // 处理文件路径（支持 qrc 资源文件和本地文件）
    QString actualPath = filePath;
    bool isResource = false;
    if (filePath.startsWith(":/") || filePath.startsWith("qrc:/")) {
        actualPath = filePath.startsWith("qrc") ? filePath.mid(3) : filePath;
        isResource = true;
    }
    
    QFile file(actualPath);
    if (!file.open(QIODevice::ReadOnly)) {
        _audioRecognizeStatus = "文件打开失败: " + actualPath;
        emit audioRecognizeStatusChanged();
        qDebug() << "无法打开文件:" << actualPath;
        return;
    }
    
    QByteArray audioData = file.readAll();
    file.close();
    
    if (audioData.isEmpty()) {
        _audioRecognizeStatus = "文件为空";
        emit audioRecognizeStatusChanged();
        return;
    }
    
    // 解析 WAV 文件头，提取 PCM 数据
    QByteArray pcmData;
    int sampleRate = 16000;
    
    // 检查是否是 WAV 文件
    if (audioData.size() > 44 && audioData.left(4) == "RIFF" && audioData.mid(8, 4) == "WAVE") {
        // 解析 WAV 头
        // 查找 "fmt " 块
        int fmtPos = audioData.indexOf("fmt ");
        if (fmtPos != -1) {
            // 读取采样率 (偏移 fmt + 8 字节处)
            sampleRate = *reinterpret_cast<const qint32*>(audioData.constData() + fmtPos + 12);
            qDebug() << "音频采样率:" << sampleRate;
        }
        
        // 查找 "data" 块
        int dataPos = audioData.indexOf("data");
        if (dataPos != -1) {
            qint32 dataSize = *reinterpret_cast<const qint32*>(audioData.constData() + dataPos + 4);
            pcmData = audioData.mid(dataPos + 8, dataSize);
            qDebug() << "PCM 数据大小:" << pcmData.size();
        } else {
            // 没找到 data 标记，尝试跳过标准 44 字节头
            pcmData = audioData.mid(44);
        }
    } else {
        // 非 WAV 文件，假设是原始 PCM
        pcmData = audioData;
    }
    
    if (pcmData.isEmpty()) {
        _audioRecognizeStatus = "无法解析音频数据";
        emit audioRecognizeStatusChanged();
        return;
    }
    
    // 设置文件识别模式的连接来接收结果（使用 2pass-offline 信号累积结果）
    if (_fileRecognizeConn) {
        disconnect(_fileRecognizeConn);
    }
    
    // 清空累积文本
    _fileRecognizeText.clear();
    
    // 重置 ASR 会话，设置为文件识别模式
    _gateWay->resetAsrSession();
    _gateWay->setFileRecognizeMode(true);  // 启用文件识别模式，允许累积多次 2pass-offline 结果
    
    // 连接到 signal_asr_text 信号，累积所有 2pass-offline 结果
    _fileRecognizeConn = connect(_gateWay, &GateWay::signal_asr_text, this, [this](const QString &text, bool isFinal) {
        if (isFinal && !text.isEmpty()) {
            // 累积识别结果（每次 2pass-offline 都追加）
            _fileRecognizeText += text;
            qDebug() << "累积识别文本:" << _fileRecognizeText;
            
            // 实时更新到自定义样本的独立字段
            ConfigMgr::instance().setCustomVoiceText(_fileRecognizeText);
            // 如果当前使用的是自定义样本，同步应用
            if (ConfigMgr::instance().currentVoiceSample() == -1) {
                ConfigMgr::instance().applyCurrentVoiceSample();
            }
            
            // 更新状态和发送结果信号
            _audioRecognizeStatus = "识别完成";
            emit audioRecognizeStatusChanged();
            emit audioRecognizeResult(_fileRecognizeText);
        }
    });
    
    // 发送 ASR 配置（使用 2pass 模式，可以累积多个 offline 结果）
    QJsonObject jsonObj;
    jsonObj["mode"] = "2pass";  // 使用 2pass 模式
    jsonObj["wav_name"] = "file_recognize";
    jsonObj["is_speaking"] = true;
    jsonObj["wav_format"] = "pcm";
    jsonObj["audio_fs"] = sampleRate;
    jsonObj["itn"] = true;
    QJsonArray chunkSizeArray;
    chunkSizeArray << 5 << 10 << 5;
    jsonObj["chunk_size"] = chunkSizeArray;
    
    QJsonDocument doc(jsonObj);
    QString configJson = doc.toJson(QJsonDocument::Compact);
    _gateWay->wsSend(configJson);
    
    // 分块发送 PCM 数据（每块 32000 字节，约 1 秒的 16kHz 16bit 单声道音频）
    const int chunkSize = 32000;
    for (int i = 0; i < pcmData.size(); i += chunkSize) {
        QByteArray chunk = pcmData.mid(i, chunkSize);
        _gateWay->wsSendPcmData(chunk);
    }
    
    // 发送结束标记
    QJsonObject endObj;
    endObj["is_speaking"] = false;
    QJsonDocument endDoc(endObj);
    _gateWay->wsSend(endDoc.toJson(QJsonDocument::Compact));
    
    qDebug() << "音频文件识别请求已发送，等待结果...";
    
    // 延迟 3 秒后断开文件识别模式的连接并重置模式
    // 这给 ASR 足够时间返回所有 2pass-offline 结果
    QTimer::singleShot(3000, this, [this]() {
        if (_fileRecognizeConn) {
            disconnect(_fileRecognizeConn);
            _fileRecognizeConn = QMetaObject::Connection();
        }
        _gateWay->setFileRecognizeMode(false);  // 关闭文件识别模式
        qDebug() << "文件识别模式已关闭，最终识别文本:" << _fileRecognizeText;
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
