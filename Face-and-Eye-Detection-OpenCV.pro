QT += quick core
CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        facefeaturedetector.cpp \
        main.cpp

RESOURCES += qml.qrc \
    cascades.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

######################################### INCLUDING OPENCV ###########################################
## This project is pre-configured with these kits: arm64-v8a, armeabi-v7a, minGW 64-bit, MSVC2017 64bit, UWP 64bit (all downloaded with Qt)
OPENCV_ANDROID = E:/openCV/OpenCV-4.0.1-android-sdk ##This must be changed to where YOU put OpenCV
TARGET_ARCHITECTURE = arm64-v8a ##This should also change if you are working on a different architecture like armeabi-v7a
android {
    contains(ANDROID_TARGET_ARCH, $$TARGET_ARCHITECTURE) {
        isEmpty(OPENCV_ANDROID) {
            error("Let OPENCV_ANDROID point to the opencv-android-sdk, recommended: v4.0")
        }
        INCLUDEPATH += "$$OPENCV_ANDROID/sdk/native/jni/include"
        LIBS += \
            -L"$$OPENCV_ANDROID/sdk/native/libs/$$TARGET_ARCHITECTURE" \
            -L"$$OPENCV_ANDROID/sdk/native/3rdparty/libs/$$TARGET_ARCHITECTURE" \
            -L"$$OPENCV_ANDROID/sdk/native/staticlibs/$$TARGET_ARCHITECTURE" \
            -llibtiff \
            -llibjpeg-turbo \
            -lcpufeatures \
            -lIlmImf \
            -llibprotobuf \
            -ltegra_hal \
            -llibjasper \
            -llibpng \
            -llibwebp \
            -lquirc \
            -ltbb \
            -lopencv_java4 \
            -lopencv_core \
            -lopencv_imgproc \
            -lopencv_highgui \
            -lopencv_objdetect

        ANDROID_EXTRA_LIBS += $$OPENCV_ANDROID/sdk/native/libs/$$TARGET_ARCHITECTURE/libopencv_java4.so \

    } else {
        error("Unsupported architecture: $$ANDROID_TARGET_ARCH")
    }
}

HEADERS += \
    facefeaturedetector.h
