#ifndef MESSAGEMGR_H
#define MESSAGEMGR_H

#include <QObject>
#include <QString>

struct Message{
    int role; // 1:用户 2:大模型
    QString text; // 文本内容
    bool isFinal; //是否是最终结果
    qint64 timestamp; //时间戳
    Message(const int &role=1,const bool &isFinal=true,const qint64 &timestamp=0):role(role),isFinal(isFinal),timestamp(timestamp){
    };
};

class MessageMgr : public QObject
{
    Q_OBJECT
public:
    explicit MessageMgr(QObject *parent = nullptr);
    ~MessageMgr();
signals:

public slots:
    void slots_handleAsrMessage(const QString &message);
};

#endif // MESSAGEMGR_H
