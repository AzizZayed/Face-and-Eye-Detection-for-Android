#ifndef FACEFEATUREDETECTOR_H
#define FACEFEATUREDETECTOR_H

#include <QtQuick>
#include <opencv2/opencv.hpp>

class FaceFeatureDetector : public QObject
{
    Q_OBJECT
public:
    explicit FaceFeatureDetector(QObject *parent = nullptr);
    void start(QObject *qmlObj);
    void grab();

private slots:
    void frameReady(const QVariant &frameVariant);

private:
    QObject *qmlObject;

    cv::CascadeClassifier faceClassifier;
    cv::CascadeClassifier eyeClassifier;

    QRect face = QRect(0, 0, 0, 0);
    QRect rightEye = QRect(0, 0, 0, 0);
    QRect leftEye = QRect(0, 0, 0, 0);
};

#endif // FACEFEATUREDETECTOR_H
