#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "threadmgr.h"
#include "test.h"
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
    engine.loadFromModule("AIGirlFriend", "Main");

    Test::test_threadMgr();
    return app.exec();
}
