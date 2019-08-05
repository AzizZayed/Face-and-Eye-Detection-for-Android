#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "facefeaturedetector.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    FaceFeatureDetector detector;
    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    detector.start(engine.rootObjects().first()); //start processing

    return app.exec();
}
