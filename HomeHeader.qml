import QtQuick 2.9
import QtQuick.Window 2.3
import QtQuick.Controls 2.2

Rectangle {
    id: homeHeaderPages
    color: globalSettings.transparent_color
    anchors.fill: parent

    signal showSetting

    ListView {
        id: listview
        height: 60
        anchors.fill: parent
        anchors.leftMargin: 100
        anchors.rightMargin: 100
        focus: false
        orientation: ListView.Horizontal
        boundsBehavior: Flickable.StopAtBounds
        snapMode: ListView.SnapOneItem
        highlightRangeMode: ListView.StrictlyEnforceRange
        highlightMoveDuration: 250
        clip: true
        spacing: 50

        model: $room
        delegate: Rectangle {
            anchors.verticalCenter: parent.verticalCenter
            width: 150
            height: 40
            color: $room.current_index===index?globalSettings.selected_color:globalSettings.transparent_color
            border.color: $room.current_index===index?globalSettings.transparent_color:globalSettings.border_color
            border.width: $room.current_index===index?0:2

            radius: 100
            Text {
                anchors.centerIn: parent
                text: dmName
                font.bold: true
                font.pointSize: 17
                color: "white"
                wrapMode: Text.Wrap
            }
            MouseArea {
                propagateComposedEvents: true
                anchors.fill: parent
                onClicked: {
                    $room.set_current_index(index)
                }
            }
        }
    }
    Image {
        id: settingAction
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.rightMargin: 30
        anchors.topMargin: 20
        anchors.bottomMargin: 15
        width: 40
        state: "NORMAL"

        states: [
            State {
                name: "NORMAL"
                PropertyChanges { target: settingAction; source: "/images/setting-idle.png" }
            },
            State {
                name: "HOVER"
                PropertyChanges { target: settingAction; source: "/images/setting-active.png" }
            }
        ]

        MouseArea{
            anchors.fill: parent
            hoverEnabled: true
//            acceptedButtons: Qt.LeftButton | Qt.RightButton

            onClicked: {
                //弹出界面
                showSetting(); //发送信号
            }

            onEntered: {
                parent.state = "HOVER";
            }
            onExited: {
                parent.state = "NORMAL";
            }
        }
    }


}
