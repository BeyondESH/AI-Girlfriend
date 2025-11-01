#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
// #include "Test.h"
#include "audiomgr.h"
#include "WebSocketMgr.h"
#include  <thread>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    AudioMgr* audioMgr=new AudioMgr;
    engine.loadFromModule("AIGirlfriend", "Main");
    engine.rootContext()->setContextProperty("audioMgr",audioMgr);
    WebSocketMgr* websocketMgr=new WebSocketMgr;
    engine.rootContext()->setContextProperty("websocketMgr",websocketMgr);

    QObject::connect(audioMgr,&AudioMgr::signal_handlePcmData,websocketMgr,&WebSocketMgr::slot_handlePcmData);

    int sampleRate=audioMgr->sampleRate();
    websocketMgr->connectAsrServer(QUrl("ws://localhost:10096"));

    return app.exec();
}
