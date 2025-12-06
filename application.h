#ifndef APPLICATION_H
#define APPLICATION_H

#include <QObject>
#include "audiomgr.h"
#include "gateway.h"

class Application : public QObject
{
    Q_OBJECT
public:
    explicit Application(QObject *parent = nullptr);
    ~Application();
    void sendAsrConfig();
    Q_INVOKABLE void sendChatMessage(QString text);
    Q_INVOKABLE void startVoiceRecord();
    Q_INVOKABLE void stopVoiceRecord();
    Q_INVOKABLE void toggleVoiceRecord();
signals:
    void signal_receive_llm(const QString &content);
    void signal_asr_text(const QString &text, bool isFinal);
private slots:
    void on_pushButtonRecord_clicked();
private:
    AudioMgr* _audioMgr;
    GateWay* _gateWay;
};

#endif // APPLICATION_H
