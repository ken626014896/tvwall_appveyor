import QtQuick 2.9
import QtQuick.Window 2.3
import QtQuick.Controls 2.2


Rectangle {
    id: win_self_define
    color: globalSettings.transparent_color
    anchors.fill: parent

    property int mode_current_index: -1
    property string mode_current_name: ""

    signal closeSetting
    signal modeManager(var idx)

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
                    text: $manager.iszhongwen?qsTr("信号源模式管理"):qsTr("Mode Manager")
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
         id:lmLogicalMode

         ListElement {
             lg_name: qsTr("保存")
             lg_row:1
             lg_column:1
             lg_index:1
         }

         ListElement {
             lg_name: qsTr("删除")
             lg_row:2
             lg_column:2
             lg_index:2
         }

     }
     ListModel{
         id:lmLogicalMode_EN

         ListElement {
             lg_name: qsTr("保存")
             lg_row:1
             lg_column:1
             lg_index:1
         }

         ListElement {
             lg_name: qsTr("删除")
             lg_row:2
             lg_column:2
             lg_index:2
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
            }

            model: $manager.iszhongwen?lmLogicalMode:lmLogicalMode_EN
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
                        lv_logical_mode.currentIndex = index
                    }
                }
            }
        }

        Button{
            id: btnConfirm
             text: $manager.iszhongwen?qsTr("确定"):qsTr("Yes")
            width: parent.width
            highlighted: true
            height: 40
            flat: false
            background: Rectangle{
                anchors.fill: parent
                color: globalSettings.selected_color
            }

            onClicked: {
                modeManager(lv_logical_mode.currentIndex)
            }
        }
    }
}
