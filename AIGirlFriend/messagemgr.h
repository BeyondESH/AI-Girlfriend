#ifndef MESSAGEMGR_H
#define MESSAGEMGR_H

#include <QObject>
#include <QString>
#include <QQueue>
#include <mutex>

struct Message{
    int role; // 1:用户 2:大模型
    QString text; // 文本内容
    bool isFinal; //是否是最终结果
    qint64 timestamp; //时间戳
    Message(const int &role=1,const QString &text=nullptr,const bool &isFinal=false,const qint64 &timestamp=0):role(role),text(text),isFinal(isFinal),timestamp(timestamp){
    };
};

class MessageMgr : public QObject
{
    Q_OBJECT
public:
    explicit MessageMgr(QObject *parent = nullptr);
    ~MessageMgr();

private:
    std::mutex _mutex;
    QQueue<Message> _msgQueue;
    std::atomic_bool _isInCommunication;
signals:
    void signal_sendllmMessage(const QString& text);
public slots:
    void slots_handleAsrMessage(const QString &message);
    void slot_llmResponse_finished();
};

#endif // MESSAGEMGR_H
