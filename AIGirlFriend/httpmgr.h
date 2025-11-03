#ifndef HTTPMGR_H
#define HTTPMGR_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkReply>
#include "global.h"

class HttpMgr : public QObject
{
    Q_OBJECT
public:
    explicit HttpMgr(QObject *parent = nullptr);
    void get(const QUrl & url,ReqId id);
    void post(const QUrl &url,ReqId id, QByteArray &data);
private:
    QNetworkAccessManager * _networkAccessMgr;
signals:
    void signal_http_finished(QString data,ReqId id, ErrorCode ec);
    void signal_llmResponse_finished();
public slots:
    void slot_http_finished(QString data,ReqId id,ErrorCode ec);
    void slot_signal_sendllmMessage(const QString& text);
};

#endif // HTTPMGR_H
