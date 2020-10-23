import QtQuick 2.9
import QtQuick.Window 2.3
import QtQuick.Controls 2.2


Rectangle {
    id: win_signal_source_mode
    color: globalSettings.transparent_color
    anchors.fill: parent
    property int margin_size: 20
    property string select_image: ""

    signal closeSetting

    Rectangle{
        color: globalSettings.transparent_color
        anchors.fill: parent
//        anchors.leftMargin: 10
//        anchors.rightMargin: 10

        Rectangle{
            color: globalSettings.transparent_color
            anchors.fill: parent
//            anchors.leftMargin: margin_size * 2
//            anchors.rightMargin: margin_size * 2

            Rectangle{
                id: rect_title
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: margin_size
                anchors.rightMargin: margin_size
                height: 30
                color: globalSettings.transparent_color


                Text {
                    anchors.centerIn: parent
                    text: $manager.iszhongwen?qsTr("新增信号模式"):qsTr("Add Mode")
                    color: "gray"
                    font.pointSize: 16
                }
                Image{
                    id: add_mode_closeAction
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
                            PropertyChanges { target: add_mode_closeAction; source: "/images/close.png" }
                        },
                        State {
                            name: "HOVER"
                            PropertyChanges { target: add_mode_closeAction; source: "/images/close-active.png" }
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

            TextField{
                id: text_mode_name
                anchors.top: rect_title.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: margin_size
                anchors.rightMargin: margin_size
                height: 30
                placeholderText: $manager.iszhongwen?qsTr("请输入模式名称"):qsTr("Please input :")
                color: "white"
                background:Rectangle{
                    color: "black"
                    anchors.fill: parent
                }
            }

            Rectangle{
                color: globalSettings.transparent_color
                anchors.topMargin: 10
                anchors.top: text_mode_name.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: margin_size
                anchors.rightMargin: margin_size
                anchors.bottom: win_signal_source_mode_rect_btn.top
                GridView{
                    id: grid_mode_image
                    anchors.fill: parent
                    cellHeight: 140
                    cellWidth: 202
                    focus: true
                    clip: true

                    model: $signal_mode_image
                    delegate: Rectangle {
                        width: 200
                        height: 120
                        color: globalSettings.transparent_color
                        border.width: globalSettings.border_size *2
                        border.color: Qt.lighter(GridView.isCurrentItem ? "gold":"transparent", 1.1)

                        Component.onCompleted: {
                            if(GridView.isCurrentItem){
                                win_signal_source_mode.select_image = dmIco
                            }
                            if(win_signal_source_mode.select_image === ""){
                                win_signal_source_mode.select_image = dmIco
                            }
                        }

                        Image {
                            z: -1
                            anchors.fill: parent
                            anchors.leftMargin: globalSettings.border_size * 2
                            anchors.rightMargin: globalSettings.border_size * 2
                            anchors.topMargin: globalSettings.border_size * 2
                            anchors.bottomMargin: globalSettings.border_size * 2
                            source: dmIco
                        }
                        MouseArea{
                            anchors.fill: parent
                            onClicked: {
                                grid_mode_image.currentIndex = index
                                win_signal_source_mode.select_image = dmIco
                            }
                        }
                    }
                }

            }


            Rectangle{
                id: win_signal_source_mode_rect_btn
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                anchors.leftMargin: margin_size
                anchors.rightMargin: margin_size
                anchors.bottomMargin: 20

                height: 70
                color: globalSettings.transparent_color

                Button{
                    anchors.fill: parent
                    text: $manager.iszhongwen?qsTr("保存模式"):qsTr("Save Mode")
                    anchors.topMargin: 30
                    anchors.bottomMargin: 10
                    highlighted: true
                    flat: false
                    background: Rectangle{
                        anchors.fill: parent
                        color: globalSettings.selected_color
                    }

                    onClicked: {

                        if(text_mode_name.text===""){
                            var ToastCom = Qt.createComponent("MessageBox.qml")
                            if (ToastCom.status === Component.Ready)
                                var detaObj = ToastCom.createObject(root, {
                                                                        "focus": true,
                                                                        "msg": $manager.iszhongwen?qsTr("请输入名称"):qsTr("Please Input Something!")
                                                                    })

                        }
                        else
                        {
                            $manager.add_mode(win_signal_source_mode.select_image, text_mode_name.text);
                            closeSetting()
                        }

                    }
                }
            }
        }
    }

}
