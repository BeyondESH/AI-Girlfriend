#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "Test.h"
#include "audiomgr.h"
#include "WebSocketMgr.h"
#include "messagemgr.h"

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

    // AudioMgr* audioMgr=new AudioMgr;
    // WebSocketMgr* websocketMgr=new WebSocketMgr;
    // MessageMgr* messageMgr=new MessageMgr;
    // engine.rootContext()->setContextProperty("audioMgr",audioMgr);
    // engine.rootContext()->setContextProperty("websocketMgr",websocketMgr);
    // engine.loadFromModule("AIGirlfriend", "Main");

    // QObject::connect(audioMgr,&AudioMgr::signal_handlePcmData,websocketMgr,&WebSocketMgr::slot_handlePcmData);
    // QObject::connect(audioMgr,&AudioMgr::signal_endRecord,websocketMgr,&WebSocketMgr::slot_endRecord);
    // QObject::connect(websocketMgr,&WebSocketMgr::signal_handleAsrMessage,messageMgr,&MessageMgr::slots_handleAsrMessage);


    // websocketMgr->connectAsrServer(QUrl("ws://localhost:10096"));
    Test::test_jsonMgr();
    return app.exec();
}
