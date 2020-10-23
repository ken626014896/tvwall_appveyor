import QtQuick 2.9
import QtQuick.Window 2.3
import QtQuick.Controls 2.2


Rectangle {   //默认宫格选择
    color: globalSettings.transparent_color
    anchors.fill: parent

    property int current_row: -1
    property int current_column: -1
    property int current_index: -1   
    signal selectLogicalMode(var idx, var lg_row, var lg_column)
    signal closeSetting


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
                    text: $manager.iszhongwen?qsTr("小宫格设置"):qsTr("Palace Setting")
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

        }

    }


    ListModel{
        id:lmLogicalMode_zh

        ListElement {
            lg_name: qsTr("1*1")
            lg_row:1
            lg_column:1
            lg_index:1
        }

        ListElement {
            lg_name: qsTr("2*2")
            lg_row:2
            lg_column:2
            lg_index:2
        }

        ListElement {
            lg_name: qsTr("3*3")
            lg_row:3
            lg_column:3
            lg_index:3
        }
        ListElement {
            lg_name: qsTr("4*4")
            lg_row:4
            lg_column:4
            lg_index:4
        }
        ListElement {
            lg_name: qsTr("Customize")
            lg_row:5
            lg_column:5
            lg_index:5
        }
        ListElement {
            lg_name: qsTr("Refresh Window")
            lg_row:6
            lg_column:6
            lg_index:6
        }
        ListElement {
            lg_name: qsTr("Clear Window")
            lg_row:7
            lg_column:7
            lg_index:7
        }
        ListElement {
            lg_name: qsTr("Switch Language")
            lg_row:8
            lg_column:8
            lg_index:8
        }
        ListElement {
            lg_name: qsTr("Authorization")
            lg_row:9
            lg_column:9
            lg_index:9
        }
        ListElement {
                    lg_name: qsTr("LED Resolution Setting")
                    lg_row:10
                    lg_column:10
                    lg_index:10
                }
    }
    ListModel{
        id:lmLogicalMode

        ListElement {
            lg_name: qsTr("1*1")
            lg_row:1
            lg_column:1
            lg_index:1
        }

        ListElement {
            lg_name: qsTr("2*2")
            lg_row:2
            lg_column:2
            lg_index:2
        }

        ListElement {
            lg_name: qsTr("3*3")
            lg_row:3
            lg_column:3
            lg_index:3
        }
        ListElement {
            lg_name: qsTr("4*4")
            lg_row:4
            lg_column:4
            lg_index:4
        }
        ListElement {
            lg_name: qsTr("自定义")
            lg_row:5
            lg_column:5
            lg_index:5
        }
        ListElement {
            lg_name: qsTr("刷新窗口")
            lg_row:6
            lg_column:6
            lg_index:6
        }
        ListElement {
            lg_name: qsTr("清空窗口")
            lg_row:7
            lg_column:7
            lg_index:7
        }
        ListElement {
            lg_name: qsTr("切换语言")
            lg_row:8
            lg_column:8
            lg_index:8
        }
        ListElement {
            lg_name: qsTr("授权管理")
            lg_row:9
            lg_column:9
            lg_index:9
        }
        ListElement {
                  lg_name: qsTr("LED屏分辨率设置")
                  lg_row:10
                  lg_column:10
                  lg_index:10
              }
    }
    Column{
        anchors.fill: parent
        anchors.topMargin: 50
        spacing: 20
        ListView{
            id:lv_logical_mode
            width: parent.width
            height: parent.height - btnConfirm.height
            clip: true
            spacing: 10

            Component.onCompleted: {
                lv_logical_mode.currentIndex = 0;
                current_row = 1
                current_column = 1
                current_index = 1
            }

            model: $manager.iszhongwen?lmLogicalMode:lmLogicalMode_zh
            delegate: Rectangle {
                width: parent.width
                height: 30
                color: globalSettings.transparent_color

                Text {
                    anchors.centerIn: parent
                    text: lg_name
                    font.bold: true
                    font.pointSize: 20
                    color: lv_logical_mode.currentIndex===index?globalSettings.selected_font_color:globalSettings.font_color
                    wrapMode: Text.Wrap
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        current_index = lg_index
                        current_row = lg_row
                        current_column = lg_column
                        lv_logical_mode.currentIndex = index
                    }
                }
            }
        }

        Button{
            id: btnConfirm
            text: $manager.iszhongwen?qsTr("确定"):qsTr("ok")
            width: parent.width
            highlighted: true
            height: 40
            flat: false
            background: Rectangle{
                anchors.fill: parent
                color: globalSettings.selected_color
            }

            onClicked: {
                selectLogicalMode(current_index, current_row, current_column)
            }
        }
    }
}
