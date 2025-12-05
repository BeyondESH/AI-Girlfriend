#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QIcon>
#include <QQmlContext>
#include "application.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/img/icon/favicon.ico"));
    app.setOrganizationName("BeyondESH");
    app.setOrganizationDomain("beyondesh.top");
    app.setApplicationName("你的女孩");

    QQmlApplicationEngine engine;

    Application application(&app);
    engine.rootContext()->setContextProperty("app", &application);

    QQuickStyle::setStyle("FluentWinUI3");
    // QQuickStyle::setStyle("Fusion");
    // QQuickStyle::setStyle("Imagine");
    // QQuickStyle::setStyle("Material");
    // QQuickStyle::setStyle("Windows");
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("AI-Girlfriend", "Main");

    return app.exec();
}
