import QtQuick 2.0

import QtQuick.Controls 1.2
Rectangle {
    id: root
    width: 300
    height: 300

    Label {
        id: lab_name
        anchors{
            left: parent.left
            leftMargin: 30
            top: parent.top
            topMargin: 50
        }
        font.pixelSize: 16
        text: "姓名："
    }

    TextArea {
        id: lbl_name_show
        anchors{
            left:lab_name.right
            top:lab_name.top
        }
        verticalScrollBarPolicy: Qt.ScrollBarAlwaysOff
        implicitHeight: lab_name.height
        antialiasing: true
        readOnly: true
        frameVisible : false
        backgroundVisible : false
        contentItem : TextArea{
            id: lab_txt
            text:"三个程序员"
            readOnly: true
            font.pixelSize: 16
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 4
            frameVisible : false
            backgroundVisible : false
        }
        MouseArea {
            id: mouse_area
            anchors.fill: parent
            hoverEnabled: true
            acceptedButtons: Qt.MiddleButton
            onEntered: {
                tool_tip.text = lab_txt.text
                tool_tip.onVisibleStatus(true)
            }
            onExited: {
                tool_tip.text = ""
                tool_tip.onVisibleStatus(false)
            }
            onPositionChanged: {
                tool_tip.mouserHover(lbl_name_show, mouseX, mouseY)
            }
        }
        visible: true
    }

}

