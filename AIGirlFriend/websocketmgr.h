#ifndef WEBSOCKETMGR_H
#define WEBSOCKETMGR_H

#include <QObject>
#include <QWebSocket>
#include <QUrl>
class WebSocketMgr : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isConnected READ isConnected NOTIFY isConnectedChanged FINAL)
public:
    explicit WebSocketMgr(QObject *parent = nullptr);
    ~WebSocketMgr();
    void connect(const QUrl &url);
    void connectAsrServer(const QUrl &url=QUrl("ws://localhost:10096"));
    void sendPcmData(const QByteArray &pcmData);
    void send(const QString &msg);
    void sendEndData();
    Q_INVOKABLE void sendConfig();
    Q_INVOKABLE void sendConfig(const int &sampleRate);
    void stop();
    bool isConnected() const;

signals:
    void isConnectedChanged();
    void signal_handleAsrMessage(const QString &message);
private:
    QWebSocket* _websocket;
    bool _isConnected;
public slots:
    void slot_handlePcmData(const QByteArray &pcmData);
    void slot_endRecord();
};

#endif // WEBSOCKETMGR_H
