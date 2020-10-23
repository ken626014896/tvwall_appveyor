import QtQuick 2.0
import QtQuick.Controls 2.2
Rectangle {
    //总控区域关闭电源提示框
    id: root

    width: 200
    height: 150
    anchors.centerIn: parent
    property var  num
    property string selected_color: "#00c1ef" //被选中的对象的背景色
    property var msg
    signal numSig(int hint)
    Rectangle {
        color: "transparent"
        anchors.fill: parent
        anchors.topMargin: 15
        Column {
            anchors.fill: parent
            spacing: 10
            anchors.horizontalCenter: parent.horizontalCenter
            Text {
                height: 30
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("提示")
                font.pointSize: 16
                color: "gray"
            }
            Text {
                height: 40
                anchors.horizontalCenter: parent.horizontalCenter
                text: msg
                font.bold: true
                font.pointSize: 14
            }
            Row {
                anchors.horizontalCenter: parent.horizontalCenter
                height: 30
                spacing: 10
                Rectangle {
                    width: 60
                    height: 25
                    color: root.selected_color
                    Button {
                        anchors.fill: parent
                        text: qsTr("取消")
                        highlighted: true
                        flat: false
                        background: Rectangle {
                            anchors.fill: parent
                            color: root.selected_color
                        }
                        onClicked: {
                            numSig(5)
                        }
                    }
                }
                Rectangle {
                    width: 60
                    height: 25
                    color: "red"
                    Button {
                        anchors.fill: parent
                        text: qsTr("确认")
                        highlighted: true
                        flat: false
                        background: Rectangle {
                            anchors.fill: parent
                            color: "red"
                        }
                        onClicked: {
                            if(num===1){
                                $manager.control_all_power_device(1)
                                numSig(1)
                            }
                            else if(num===2){
                                $manager.control_all_power_device(0)
                                numSig(2)

                            }
                            else if(num===3){
                                 $manager.control_all_screen_device(1)
                                 numSig(3)
                            }
                            else{
                                $manager.control_all_screen_device(0)
                                numSig(4)

                            }

                        }
                    }
                }
            }
        }
    }
}
