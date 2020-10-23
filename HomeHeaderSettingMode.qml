import QtQuick 2.9
import QtQuick.Window 2.3
import QtQuick.Controls 2.2


Rectangle { //宫格自定义
    id: win_self_define
    color: globalSettings.transparent_color
    anchors.fill: parent

    signal selfLogicalMode(var lg_row, var lg_column)
    signal closeSetting


    property int line_1: 2
    property int line_2: 2

    ListModel{
        id:lmNums

        ListElement{
            num: 1
        }
        ListElement{
            num: 2
        }
        ListElement{
            num: 3
        }
        ListElement{
            num: 4
        }
        ListElement{
            num: 5
        }
        ListElement{
            num: 6
        }
        ListElement{
            num: 7
        }
        ListElement{
            num: 8
        }
        ListElement{
            num: 9
        }
        ListElement{
            num: 10
        }
    }

    Rectangle{
        color: globalSettings.transparent_color
        anchors.fill: parent
        anchors.leftMargin: 10
        anchors.rightMargin: 10

        Rectangle{
            color: globalSettings.transparent_color
            anchors.fill: parent
            anchors.leftMargin: globalSettings.margin_size * 2
            anchors.rightMargin: globalSettings.margin_size * 2

            Rectangle{
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
                    text: $manager.iszhongwen?qsTr("小宫格自定义"):qsTr("Palace Custom")
                    color: "gray"
                    font.pointSize: 16
                }
                Image{
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
                            PropertyChanges { target: win_self_define_closeAction; source: "/images/close.png" }
                        },
                        State {
                            name: "HOVER"
                            PropertyChanges { target: win_self_define_closeAction; source: "/images/close-active.png" }
                        }
                    ]

                    MouseArea{
                        anchors.fill: parent
                        hoverEnabled: true

                        onClicked: {
                           closeSetting()
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

            Column{
                anchors.top: rect_win_self_define_title.bottom
                anchors.topMargin: 30
                anchors.leftMargin: 20
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: win_self_define_rect_btn.top
                spacing: 20

                Row{
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 20
                    Repeater{
                        model: lmNums
                        Rectangle{
                            radius: 50
                            width: 30
                            height: 30
                            color: (win_self_define.line_1 - 1) === index?globalSettings.selected_color:"gray"
                            Text {
                                anchors.centerIn: parent
                                font.bold: true
                                font.pointSize: 17
                                text: num
                                MouseArea{
                                    anchors.fill: parent
                                    onClicked: {
                                        win_self_define.line_1 = num
                                    }
                                }
                            }
                        }
                    }
                }

                Row{
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 20
                    Repeater{
                        model: lmNums
                        Rectangle{
                            radius: 50
                            width: 30
                            height: 30
                            color: (win_self_define.line_2 - 1) === index?globalSettings.selected_color:"gray"
                            Text {
                                anchors.centerIn: parent
                                font.bold: true
                                font.pointSize: 17
                                text: num
                                MouseArea{
                                    anchors.fill: parent
                                    onClicked: {
                                        win_self_define.line_2 = num
                                    }
                                }
                            }
                        }
                    }
                }
                Text {
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.bold: true
                    font.pointSize: 20
                    color: "white"
                    text: win_self_define.line_1 + " x " + win_self_define.line_2
                }
            }

            Rectangle{
                id: win_self_define_rect_btn
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                anchors.leftMargin: globalSettings.margin_size
                anchors.rightMargin: globalSettings.margin_size
                anchors.bottomMargin: 20

                height: 70
                color: "transparent"

                Button{
                    anchors.fill: parent
                    text: $manager.iszhongwen?qsTr("保 存"):qsTr("save")
                    anchors.topMargin: 30
                    anchors.bottomMargin: 10
                    highlighted: true
                    flat: false
                    background: Rectangle{
                        anchors.fill: parent
                        color: globalSettings.selected_color
                    }

                    onClicked: {
                        if(win_self_define.line_1 !== 0 &&
                                win_self_define.line_2 !== 0){
                            selfLogicalMode(win_self_define.line_1, win_self_define.line_2)
                        }
                    }
                }
            }

        }

    }
}
