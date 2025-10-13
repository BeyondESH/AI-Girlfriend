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
    void signals_http_finished(QString data,ReqId id, ErrorCode ec);
private slots:
    void slots_http_finished(QString data,ReqId id,ErrorCode ec);
};

#endif // HTTPMGR_H
