import QtQuick 2.12
import QtQuick.Window 2.12
import QtMultimedia 5.12
import QtQuick.Controls 2.12

Window {
    visible: true
    color: "white"
    width: 1080 //change for your screen
    height: 2280 //change for your screen

    property int w: 240 // camera resolution width
    property int h: 320 // camera resolution height
    property rect face: Qt.rect(0, 0, 10, 10)
    property rect reye: Qt.rect(0, 0, 10, 10)
    property rect leye: Qt.rect(0, 0, 10, 10)
    property int th: 5
    property real sc: 4.5 // scale at which we render

    signal frameReady(var frame)

    Camera
    {
        id: camera
        captureMode: Camera.CaptureViewfinder
        position: Camera.FrontFace

        // camera is rotated FOR MY DEVICE (Huawei p20). It may or may not be for yours.
        // Testing is required to verify this. Switch 'w' and 'h' to see if it works.
        viewfinder.resolution: Qt.size(h, w)

        focus {
            focusMode: Camera.FocusContinuous
        }

    }

    VideoOutput
    {
        id: videoOutput
        source: camera

        // camera is rotated FOR MY DEVICE (Huawei p20). It may or may not be for yours.
        // Testing is required to verify this. try -180, -90, 0, 90, 180...etc to see what works
        orientation: -90


        visible: true
        width: w * sc
        height: h * sc
    }

    // Boundaries for face
    Rectangle {
        id: rectFace
        x: face.x * sc
        y: face.y * sc
        //        radius: (face.width + face.height) * sc / 4.0 //this is to make it a circle if wanted
        width: face.width * sc
        height: face.height * sc
        color: "transparent"
        border.color: "red"
        border.width: th
    }

    // Boundaries for right eye
    Rectangle {
        id: rectRightEye
        x: reye.x * sc
        y: reye.y * sc
        radius: (reye.width + reye.height) * sc / 4.0 //this is to make it a circle if wanted
        width: reye.width * sc
        height: reye.height * sc
        color: "transparent"
        border.color: "blue"
        border.width: th
    }

    // Boundaries for left eye
    Rectangle {
        id: rectLeftEye
        x: leye.x * sc
        y: leye.y * sc
        radius: (leye.width + leye.height) * sc / 4.0 //this is to make it a circle if wanted
        width: leye.width * sc
        height: leye.height * sc
        color: "transparent"
        border.color: "blue"
        border.width: th
    }

    //Text for printing coordinates of face and eyes
    Text {
        id: faceCoordText
        y: videoOutput.height + 5
        x: 15
        text: "Face: " + face.x + ", " + face.y + ", " + face.width + "x" + face.height
        color: "black"
    }
    Text {
        id: leftEyeCoordText
        y: faceCoordText.y + faceCoordText.contentHeight
        x: 15
        text: "Left Eye: " + leye.x + ", " + leye.y + ", " + leye.width + "x" + leye.height
        color: "black"
    }
    Text {
        id: rightEyeCoordText
        y: leftEyeCoordText.y + leftEyeCoordText.contentHeight
        x: 15
        text: "Right Eye: " + reye.x + ", " + reye.y + ", " + reye.width + "x" + reye.height
        color: "black"
    }

    //button to toggle visibility of the viewfinder
    Button {
        id: visibilityButton
        width: 500
        height: 150

        text: "Toggle Visibility"

        x: parent.width / 2 - width / 2
        y: parent.height - height - 5

        onClicked: {
            videoOutput.visible = !videoOutput.visible
        }
    }

    //function called to send a frame to C++
    function grab(fc, re, le)
    {
        face = fc
        reye = re
        leye = le

        videoOutput.grabToImage(function(frame) {
            frameReady(frame) //Emit frameReady SIGNAL
        }, Qt.size(w, h))
    }
}

