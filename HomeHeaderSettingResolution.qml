import QtQuick 2.0
import QtQuick.Window 2.3
import QtQuick.Controls 2.2

Rectangle {
    //分辨率自定义
    id: win_self_define
    color: globalSettings.transparent_color
    anchors.fill: parent

    signal closeSetting

    function setResolution(msg) {

        res_text.text = msg
        var msg_arr=msg.split("*")
        width_text.text=msg_arr[0]
        height_text.text=msg_arr[1]
    }

    Component.onCompleted: {
        $manager.get_resolution.connect(setResolution)
    }
    Rectangle {
        color: globalSettings.transparent_color
        anchors.fill: parent
        anchors.leftMargin: 10
        anchors.rightMargin: 10

        Rectangle {
            color: globalSettings.transparent_color
            anchors.fill: parent
            anchors.leftMargin: globalSettings.margin_size * 2
            anchors.rightMargin: globalSettings.margin_size * 2

            Rectangle {
                id: rect_win_self_define_title
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: globalSettings.margin_size
                anchors.rightMargin: globalSettings.margin_size
                height: 30
                color: globalSettings.transparent_color

                Text {
                    anchors.centerIn: parent
                    text: $manager.iszhongwen ? qsTr("分辨率设置") : qsTr(
                                                    "Resolution Setting")
                    color: "gray"
                    font.pointSize: 16
                }
                Image {
                    id: win_self_define_closeAction
                    anchors.top: parent.top
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    anchors.rightMargin: 5
                    anchors.topMargin: 5
                    anchors.bottomMargin: 5
                    width: 20
                    state: "NORMAL"

                    states: [
                        State {
                            name: "NORMAL"
                            PropertyChanges {
                                target: win_self_define_closeAction
                                source: "/images/close.png"
                            }
                        },
                        State {
                            name: "HOVER"
                            PropertyChanges {
                                target: win_self_define_closeAction
                                source: "/images/close-active.png"
                            }
                        }
                    ]

                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true

                        onClicked: {
                            closeSetting()
                        }
                        onEntered: {
                            parent.state = "HOVER"
                        }
                        onExited: {
                            parent.state = "NORMAL"
                        }
                    }
                }
            }
            Column {
                id: msg_col
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: rect_win_self_define_title.bottom
                anchors.topMargin: 50
                spacing: 30

                Text {

                    color: "white"
                    font.pointSize: 20
                    text: $manager.iszhongwen ? qsTr("当前分辨率：") : qsTr(
                                                    "Resolution：")
                    anchors.horizontalCenter: parent.horizontalCenter
                }
                Text {
                    id: res_text
                    color: "white"
                    font.pointSize: 20

                    anchors.horizontalCenter: parent.horizontalCenter
                }

                Row {
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 10
                    Rectangle {
                        width: 100
                        height: 30
                        color: "white"
                        anchors.verticalCenter: parent.verticalCenter
                        TextInput {
                            id: width_text
                            anchors.fill: parent
                            verticalAlignment: TextInput.AlignVCenter
                            font.pixelSize: 18
                            text: "1920"
                            color: "black"
                        }
                    }
                    Text {
                        color: "white"
                        font.pointSize: 20
                        text: "*"
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    Rectangle {
                        width: 100
                        height: 30
                        color: "white"
                        anchors.verticalCenter: parent.verticalCenter

                        TextInput {
                            id: height_text
                            anchors.fill: parent
                            verticalAlignment: TextInput.AlignVCenter
                            font.pixelSize: 18
                            text: "1020"
                            color: "black"
                        }
                    }
                }

                Rectangle {
                    width: 100
                    height: 30
                    color: globalSettings.selected_color
                    anchors.horizontalCenter: parent.horizontalCenter
                    Text {

                        text: $manager.iszhongwen ? qsTr("保存") : qsTr("save")
                        font.pixelSize: parent.height / 2
                        anchors.centerIn: parent
                        color: "white"
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {

                           $manager.set_wall_width_height(width_text.text,height_text.text)

                        }
                    }
                }
            }
        }
    }
}
