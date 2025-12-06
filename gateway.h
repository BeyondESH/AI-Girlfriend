#ifndef GATEWAY_H
#define GATEWAY_H

#include <QObject>
#include <QWebSocket>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QTimer>

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
public:
    explicit GateWay(QObject *parent = nullptr);
    ~GateWay();
    void wsConnect(const QUrl &url);
    void wsConnectAsrServer(const QUrl &url=QUrl("ws://localhost:10096"));
    void wsSendPcmData(const QByteArray &pcmData);
    void wsSend(const QString &msg);
    void wsSendAsrConfig();
    void get(const QUrl & url,ReqId id);
    void post(const QUrl &url,QByteArray &data,ReqId id);
    void post(const QUrl &url,QHttpMultiPart *multiPart,ReqId id);
    void sendllmMessage(const QString& text,ReqId id);
    void sendttsMessage(const QString& text);
signals:
    void signal_connectAsrWS();
    void signal_tts_finished(const QByteArray &data);
    void signal_receive_llm(const QString &context);
    void signal_asr_text(const QString &text, bool isFinal);
public slots:
    void slot_handlePcmData(const QByteArray &pcmData);
    void handle_http_finished(QByteArray data,ReqId id,ErrorCode ec);

    void slot_endAsrRecord();
private:
    QWebSocket* _websocket;
    QNetworkAccessManager* _networkAccessMgr;
    std::atomic_bool _wsConnected;
    QTimer* _reconnectTimer;
};

#endif // GATEWAY_H

