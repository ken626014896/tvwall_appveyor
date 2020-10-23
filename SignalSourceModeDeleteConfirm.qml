import QtQuick 2.9
import QtQuick.Window 2.3
import QtQuick.Controls 2.2


Rectangle {
    id: md_delete_signal_mode
    color: globalSettings.transparent_color
    anchors.fill: parent

    property int delete_index: -1
    property string delete_mode_name: ""

    signal closeSetting

    Rectangle{
        color: globalSettings.transparent_color
        anchors.fill: parent
        anchors.topMargin: 15
        Column{
            anchors.fill: parent
            spacing: 10
            anchors.horizontalCenter: parent.horizontalCenter
            Text {
                height: 30
                anchors.horizontalCenter: parent.horizontalCenter
                text: $manager.iszhongwen?qsTr("删除信号模式"):qsTr("Delete Mode")
                font.pointSize: 16
                color: "gray"
            }
            Text {
                height: 40
                anchors.horizontalCenter: parent.horizontalCenter
                text: $manager.iszhongwen?(qsTr("确认删除 \"") + md_delete_signal_mode.delete_mode_name + "\" ?"):(qsTr("Delete \"") + md_delete_signal_mode.delete_mode_name + "\" ?")
                font.bold: true
                font.pointSize: 16

            }
            Row{
                anchors.horizontalCenter: parent.horizontalCenter
                height: 30
                spacing: 10
                Rectangle{
                    width: 60
                    height: 25
                    color: globalSettings.selected_color
                    Button{
                        anchors.fill: parent
                        text: $manager.iszhongwen?qsTr("取消"):qsTr("no")
                        highlighted: true
                        flat: false
                        background: Rectangle{
                            anchors.fill: parent
                            color: globalSettings.selected_color
                        }
                        onClicked:{
                            closeSetting()
                        }
                    }
                }
                Rectangle{
                    width: 60
                    height: 25
                    color: "red"
                    Button{
                        anchors.fill: parent
                        text: $manager.iszhongwen?qsTr("确认"):qsTr("yes")
                        highlighted: true
                        flat: false
                        background: Rectangle{
                            anchors.fill: parent
                            color: "red"
                        }
                        onClicked:{
                            $manager.remove_mode(md_delete_signal_mode.delete_index)
                            md_delete_signal_mode.delete_index = -1
                            closeSetting()
                        }
                    }
                }
            }
        }
    }


}
