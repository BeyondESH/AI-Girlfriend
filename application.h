#ifndef APPLICATION_H
#define APPLICATION_H

#include <QObject>
#include <QJsonArray>
#include <QJsonObject>
#include "audiomgr.h"
#include "gateway.h"

class Application : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QJsonArray chatHistory READ chatHistory NOTIFY chatHistoryChanged)
    Q_PROPERTY(QJsonArray conversationList READ conversationList NOTIFY conversationListChanged)
    Q_PROPERTY(QString currentConversationId READ currentConversationId WRITE setCurrentConversationId NOTIFY currentConversationIdChanged)
    Q_PROPERTY(QString audioRecognizeStatus READ audioRecognizeStatus NOTIFY audioRecognizeStatusChanged)
    Q_PROPERTY(bool ollamaOnline READ ollamaOnline NOTIFY ollamaStatusChanged)
    Q_PROPERTY(bool asrOnline READ asrOnline NOTIFY asrStatusChanged)
    Q_PROPERTY(bool ttsOnline READ ttsOnline NOTIFY ttsStatusChanged)
    
public:
    explicit Application(QObject *parent = nullptr);
    ~Application();
    void sendAsrConfig();
    Q_INVOKABLE void sendChatMessage(QString text);
    Q_INVOKABLE void startVoiceRecord();
    Q_INVOKABLE void stopVoiceRecord();
    Q_INVOKABLE void toggleVoiceRecord();
    
    // 历史对话管理
    Q_INVOKABLE void createNewConversation();
    Q_INVOKABLE void loadConversation(const QString &conversationId);
    Q_INVOKABLE void deleteConversation(const QString &conversationId);
    Q_INVOKABLE void addMessageToHistory(const QString &role, const QString &content);
    Q_INVOKABLE void clearCurrentHistory();
    Q_INVOKABLE QJsonArray getConversationList();
    
    // 服务器状态检测
    Q_INVOKABLE void checkOllamaStatus();
    Q_INVOKABLE void checkAsrStatus();
    Q_INVOKABLE void checkTtsStatus();
    
    // 音频文件识别（用于语音克隆样本）
    Q_INVOKABLE void recognizeAudioFile(const QString &filePath);
    QString audioRecognizeStatus() const { return _audioRecognizeStatus; };
    
    QJsonArray chatHistory() const;
    QJsonArray conversationList() const;
    QString currentConversationId() const;
    void setCurrentConversationId(const QString &id);

    bool ollamaOnline() const { return _ollamaOnline; }
    bool asrOnline() const { return _asrOnline; }
    bool ttsOnline() const { return _ttsOnline; }
    
signals:
    void signal_receive_llm(const QString &content);
    void signal_asr_text(const QString &text, bool isFinal);
    void signal_tts_playback_finished();  // TTS播放完成信号
    void signal_stop_recording();  // 通知停止录音
    void chatHistoryChanged();
    void conversationListChanged();
    void currentConversationIdChanged();
    void ollamaStatusChanged(bool online, const QString &info);
    void asrStatusChanged(bool online, const QString &info);
    void ttsStatusChanged(bool online, const QString &info);
    void audioRecognizeStatusChanged();
    void audioRecognizeResult(const QString &text);
    
private slots:
    void on_pushButtonRecord_clicked();
    
private:
    AudioMgr* _audioMgr;
    GateWay* _gateWay;
    
    // 历史对话
    QJsonArray _chatHistory;
    QJsonArray _conversationList;
    QString _currentConversationId;
    QString _historyDir;
    QString _audioRecognizeStatus = "就绪";
    QString _fileRecognizeText;  // 累积的文件识别文本

    bool _ollamaOnline = false;
    bool _asrOnline = false;
    bool _ttsOnline = false;
    
    // 文件识别连接
    QMetaObject::Connection _fileRecognizeConn;
    
    void saveCurrentConversation();
    void loadConversationList();
    void saveConversationList();
    QString generateConversationId();
};

#endif // APPLICATION_H
