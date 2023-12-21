import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.14
import QtQuick.Window 2.15
import custom.framelessWindow 1.0

ApplicationWindow {
    id: mainWindow

    property var screenWidth: Screen.width
    property var screenHeight: Screen.height

    visible: true
    color: "gray"
    x: 0
    y: 0
    flags: Qt.Window | Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint

    // 顶部菜单
    Window {
        // visible: true
        // width: 200
        // height: 50
        // color: "white"
        // flags: Qt.Window | Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint
        // x: (screenWidth - mainWindow.width) / 2
        // y: 0
        // Text {
        //     anchors.verticalCenter: parent.verticalCenter
        //     anchors.horizontalCenter: parent.horizontalCenter
        //     text: "顶部菜单"
        //     color: "black"
        //     font.pixelSize: 20
        // }
        // Rectangle {
        //     anchors.fill: parent
        //     color: "transparent"
        //     border.color: "blue"
        //     border.width: 2
        // }

    }

    // 右上窗口
    Window {
        // id: redWindow
        // visible: true
        // width: 500
        // height: 250
        // title: "窗口1"
        // flags: Qt.Window | Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint
        // x: screenWidth - redWindow.width
        // y: 0
        // Rectangle {
        //     color: "red"
        //     anchors.fill: parent
        // }

    }

    // 左下窗口
    Window {
        // id: blueWindow
        // visible: true
        // width: 200
        // height: 100
        // title: "窗口2"
        // flags: Qt.Window | Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint
        // x: 0
        // y: screenHeight - blueWindow.height
        // Rectangle {
        //     color: "blue"
        //     anchors.fill: parent
        // }

    }

    //选区窗口
    Window {
        // id: draggableWindow
        // visible: true
        // color: "transparent"
        // flags: Qt.Window | Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint
        // width: 640
        // height: 360
        // x: (screenWidth - draggableWindow.width) / 2
        // y: (screenHeight - draggableWindow.height) / 2
        // Rectangle {
        //     anchors.fill: parent
        //     anchors.margins: 10
        //     border.width: 5
        //     border.color: "green"
        //     color: "transparent"
        // }
        // MouseArea {
        //     anchors.fill: parent
        //     drag.target: window
        //     drag.axis: Drag.XAndYAxis
        // }

    }

    DraggableFramelessWindow {
        id: draggableFramelessWindow

        visible: true
        width: 640
        height: 480
        minimumWidth: 480
        minimumHeight: 320
        color: "#000"
        title: qsTr("Hello World")

        Rectangle {
            width: 100
            height: 100
            anchors.centerIn: parent
            color: "red"

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onEntered: {
                    cursorShape = Qt.PointingHandCursor;
                    parent.color = "blue";
                }
                onExited: {
                    cursorShape = Qt.ArrowCursor;
                    parent.color = "red";
                }
                onPressed: parent.color = "yellow"
                onReleased: parent.color = "blue"
            }

        }

    }

}
