#ifndef WEBSOCKETMGR_H
#define WEBSOCKETMGR_H

#include <QObject>
#include <QWebSocket>
#include <QUrl>
class WebSocketMgr : public QObject
{
    Q_OBJECT
public:
    explicit WebSocketMgr(QObject *parent = nullptr);
    ~WebSocketMgr();
    void connect(const QUrl &url);
    void connectAsrServer(const QUrl &url=QUrl("ws://localhost:10096"));
    void sendPcmData(const QByteArray &pcmData);
    void send(const QString &msg);
    Q_INVOKABLE void sendConfig();
    Q_INVOKABLE void sendConfig(const int &sampleRate);
    void stop();
    bool isConnected() const;

signals:

private:
    QWebSocket* _websocket;
    std::atomic_bool _isConnected;
public slots:
    void slot_handlePcmData(const QByteArray &pcmData);
};

#endif // WEBSOCKETMGR_H
