#ifndef GATEWAY_H
#define GATEWAY_H

#include <QObject>
#include <QWebSocket>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QTimer>
#include <QJsonArray>

enum ReqId{
    SEND_ASR,
    SEDN_TTS,
    CHAT_LLM,
    VOICE_LLM
};

enum ErrorCode{
    ERROR_NETWORK,
    SUCCESS,
};

class GateWay : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool wsConnected READ wsConnected NOTIFY wsConnectedChanged)
public:
    explicit GateWay(QObject *parent = nullptr);
    ~GateWay();
    bool wsConnected() const { return _wsConnected; }
    void wsConnect(const QUrl &url);
    void wsConnectAsrServer(const QUrl &url=QUrl("ws://127.0.0.1:10096"));
    void wsSendPcmData(const QByteArray &pcmData);
    void wsSend(const QString &msg);
    void wsSendAsrConfig();
    void get(const QUrl & url,ReqId id);
    void post(const QUrl &url,QByteArray &data,ReqId id);
    void post(const QUrl &url,QHttpMultiPart *multiPart,ReqId id);
    void sendllmMessage(const QString& text, ReqId id, const QJsonArray& chatHistory = QJsonArray());
    void sendttsMessage(const QString& text);
    void resetAsrSession();  // 重置 ASR 会话状态
    void setFileRecognizeMode(bool isFileRecognize);  // 设置文件识别模式
signals:
    void signal_connectAsrWS();
    void signal_tts_finished(const QByteArray &data);
    void signal_receive_llm(const QString &context);
    void signal_asr_text(const QString &text, bool isFinal);
    void signal_file_recognize_result(const QString &text);  // 文件识别结果（独立信号）
    void signal_stop_recording();  // 通知停止录音
    void signal_voice_message_ready(const QString &text);  // 语音消息准备好，需要发送LLM
    void wsConnectedChanged();
public slots:
    void slot_handlePcmData(const QByteArray &pcmData);
    void handle_http_finished(QByteArray data,ReqId id,ErrorCode ec);

    void slot_endAsrRecord();
private:
    QWebSocket* _websocket;
    QNetworkAccessManager* _networkAccessMgr;
    std::atomic_bool _wsConnected;
    QTimer* _reconnectTimer;
    std::atomic_bool _offlineProcessed;  // 标记是否已处理过 offline 结果
    std::atomic_bool _isFileRecognizing;  // 是否处于文件识别模式
};

#endif // GATEWAY_H

