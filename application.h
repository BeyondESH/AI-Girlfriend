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
private slots:

    void on_pushButtonRecord_clicked();

private:
    AudioMgr* _audioMgr;
    GateWay* _gateWay;
};

#endif // APPLICATION_H
