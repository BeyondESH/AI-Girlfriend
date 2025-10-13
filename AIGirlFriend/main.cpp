#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "Test.h"

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
    engine.loadFromModule("AIGirlfriend", "Main");
    Test::test_post_chat();
    return app.exec();
}
