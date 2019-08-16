#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "facefeaturedetector.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    FaceFeatureDetector detector;

    QQmlApplicationEngine engine("qrc:/main.qml");

    detector.start(engine.rootObjects().first()); //start processing

    return app.exec();
}
