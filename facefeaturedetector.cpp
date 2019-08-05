#include "facefeaturedetector.h"

/**
 * @brief FaceFeatureDetector::FaceFeatureDetector : constuctor
 * @param parent
 */
FaceFeatureDetector::FaceFeatureDetector(QObject *parent) : QObject(parent) {

    //Load face cascade classifier
    QFile xmlFace(":/haarcascade_frontalface_alt.xml");
    if (xmlFace.open(QFile::ReadOnly | QFile::Text)) {
        QTemporaryFile temp;
        if (temp.open()) {
            temp.write(xmlFace.readAll());
            temp.close();
            if (faceClassifier.load(temp.fileName().toStdString()))
                qDebug() << "Successfully loaded Face classifier!";
            else
                qDebug() << "Could not load Face classifier.";
        } else
            qDebug() << "Can't open Face temp file.";
    }

    //Load eye cascade classifier
    QFile xmlEye(":/haarcascade_eye.xml");
    if (xmlEye.open(QFile::ReadOnly | QFile::Text)) {
        QTemporaryFile temp;
        if (temp.open()) {
            temp.write(xmlEye.readAll());
            temp.close();
            if (eyeClassifier.load(temp.fileName().toStdString()))
                qDebug() << "Successfully loaded Eye classifier!";
            else
                qDebug() << "Could not load Eye classifier.";
        } else
            qDebug() << "Can't open Eye temp file.";
    }
}

/**
 * @brief FaceFeatureDetector::start : initialize the feature detector
 * @param qmlObj : to be able to interact with the QML side of the project
 */
void FaceFeatureDetector::start(QObject *qmlObj) {
    qmlObject = qmlObj;

    //Connect the QML frameReady SIGNAL to our frameReady SLOT
    connect(qmlObject, SIGNAL(frameReady(QVariant)), this, SLOT(frameReady(QVariant)));

    grab(); //Do the first grab to test for faces and eyes
}

/**
 * @brief FaceFeatureDetector::grab : call the function in QML
 */
void FaceFeatureDetector::grab() {
    QMetaObject::invokeMethod(qmlObject, "grab", Q_ARG(QVariant, face), Q_ARG(QVariant, rightEye), Q_ARG(QVariant, leftEye));
}

/**
 * @brief FaceFeatureDetector::frameReady:  extract frame from QML grab and process for faces and eyes
 * @param frameVariant
 */
void FaceFeatureDetector::frameReady(const QVariant &frameVariant) {
    //extract image from grab
    QQuickItemGrabResult *grab_result = qvariant_cast<QQuickItemGrabResult*>(frameVariant); //Cast from QVariant
    QImage frame = grab_result->image(); //Get the QImage
    grab_result->deleteLater(); //Release QQuickItemGrabResult

    //convert QImage to cv::Mat
    cv::Mat img(frame.height(), frame.width(), CV_8UC4, (void *)frame.constBits(), frame.bytesPerLine());
    cv::cvtColor(img, img, cv::COLOR_BGR2GRAY);

    face = QRect(0, 0, 0, 0);
    rightEye = QRect(0, 0, 0, 0);
    leftEye = QRect(0, 0, 0, 0);

    //detecting faces and drawing:
    std::vector<cv::Rect> cvfaces, cvfaceeyes;
    faceClassifier.detectMultiScale(img, cvfaces); //magic

    //process faces and eyes
    if (cvfaces.size() >= 1) {
        cv::Rect cvface = cvfaces[0];
        face = QRect(cvface.x, cvface.y, cvface.width, cvface.height);
        cv::Mat faceImg = img(cvface);
        eyeClassifier.detectMultiScale(faceImg, cvfaceeyes);

        if (cvfaceeyes.size() >= 2) {
            std::sort(cvfaceeyes.begin(), cvfaceeyes.end(), [](const cv::Rect& a, const cv::Rect& b) {
                return (a.y < b.y);
            });

            cv::Rect eye1 = cvfaceeyes[0];
            cv::Rect eye2 = cvfaceeyes[1];
            if (eye1.x < eye2.x){
                leftEye = QRect(eye1.x + cvface.x, eye1.y + cvface.y, eye1.width, eye1.height);
                rightEye = QRect(eye2.x + cvface.x, eye2.y + cvface.y, eye2.width, eye2.height);
            } else {
                leftEye = QRect(eye2.x + cvface.x, eye2.y + cvface.y, eye2.width, eye2.height);
                rightEye = QRect(eye1.x + cvface.x, eye1.y + cvface.y, eye1.width, eye1.height);
            }
        }
    }

    grab(); //Do the next frame grab
}
