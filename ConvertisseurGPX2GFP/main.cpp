#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "converter.h"
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    Converter converter;

    engine.rootContext()->setContextProperty("converter", &converter);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("ConvertisseurGPX2GFP", "Main");

    return app.exec();
}
