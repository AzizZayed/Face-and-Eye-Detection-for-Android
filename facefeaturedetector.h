#ifndef FACEFEATUREDETECTOR_H
#define FACEFEATUREDETECTOR_H

#include <QtCore>
#include <QtGui>
#include <QtQuick>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>

class FaceFeatureDetector : public QObject
{
    Q_OBJECT
public:
    explicit FaceFeatureDetector(QObject *parent = nullptr);
    void start(QObject *qmlObj);

public slots:
    void grab();

private slots:
    void frameReady(const QVariant &frameVariant);

private:
    QObject *qmlObject;
    QRect face = QRect(0, 0, 0, 0);
    QRect rightEye = QRect(0, 0, 0, 0);
    QRect leftEye = QRect(0, 0, 0, 0);
    cv::CascadeClassifier faceClassifier;
    cv::CascadeClassifier eyeClassifier;
};

#endif // FACEFEATUREDETECTOR_H
