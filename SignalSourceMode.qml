import QtQuick 2.9
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3
import QtQuick.Window 2.2
import QtQuick.Controls 2.2

Rectangle {
    color: globalSettings.transparent_color
    anchors.fill: parent
    id:sourceMode
    property int mode_current_index: -1
    property string mode_current_name: ""
    clip: true
    signal modeAdd
    signal modeMenu
    signal modeIndexChange(int indexx) //发送当前的信号模式的index给刷新窗口功能使用

    //二期
    property int current_index: -1
    property int open_index: -1
    property int isOk: 1
        Rectangle{
            id: rect_signal_mode_title
            anchors.top: parent.top
            anchors.topMargin: 10
            anchors.left: parent.left
            anchors.right: parent.right
            height: 30
            color: globalSettings.transparent_color
            clip: true
//            border.color:globalSettings.border_color
//            border.width: 1

            Text {
                    text: $manager.iszhongwen?qsTr("信号模式"):qsTr("Mode")
                    anchors.left:  parent.left
                    anchors.leftMargin: 10

                    anchors.verticalCenter: parent.verticalCenter
                    font.bold: true
//                    font.pointSize: 17
                    font.pixelSize:20
                    color: "white"
                }
                Image{
                    id: rect_signal_mode_closeAction
                    height: 20
                    width: 20
                    anchors.right: parent.right
                    anchors.rightMargin: 10
                    anchors.verticalCenter: parent.verticalCenter
                    state: "NORMAL"

                    states: [
                        State {
                            name: "NORMAL"
                            PropertyChanges { target: rect_signal_mode_closeAction; source: "/images/add-idle.png" }
                        },
                        State {
                            name: "HOVER"
                            PropertyChanges { target: rect_signal_mode_closeAction; source: "/images/add-active.png" }
                        }
                    ]

                    MouseArea{
                        anchors.fill: parent
                        hoverEnabled: true

                        onClicked: {
                            //弹出界面
                            modeAdd()
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
        Flickable{
            id:flick
            anchors.top: rect_signal_mode_title.bottom
            width: rect_signal_mode_title.width
            height: parent.height-rect_signal_mode_title.height
            clip: true
            contentHeight: lv_signal_source_mode_col3.height+rect_signal_mode_title2.height+lv_signal_source_mode_col2.height+rect_signal_mode_title2.height
            contentWidth: lv_signal_source_mode_col3.width
            flickableDirection : Flickable.VerticalFlick //只允许垂直滑动
            Column{   //D区 信号模式列表
                id:lv_signal_source_mode_col3

                spacing: 10
                Repeater{
                    id: lv_signal_source_mode3

                    model: $signal_mode
                    Component.onCompleted: {

                      $manager.index_mode_changed.connect(mode_changed)
                    }
                    function mode_changed( hint){
                        mode_current_index = hint
                        $signal_mode.set_current_index(hint)
                        mode_current_name = "xxx"

//                        $manager.select_mode_by_sync(hint,wall.width, wall.height) //同步操作
                    }
                    Rectangle {
                        id: rect_signal_mode_item3
                        anchors.left: lv_signal_source_mode_col3.left
                        anchors.leftMargin: 10
                        width: rect_signal_mode_title.width-20
                        height: 80
                        color: "#0e2439"
                        Image {
                            anchors.fill: parent
                            fillMode: Image.Stretch
                            source: dmIco
                        }


                        Rectangle{
                            anchors.top: parent.top
                            anchors.left: parent.left
                            anchors.right: parent.right
                            height: 20
                            color: $signal_mode.current_index===index?globalSettings.selected_color:"#3b4043"
                            border.color: Qt.lighter(color, 1.1)
                            opacity: 0.6
                            Text {
                                anchors.centerIn: parent
                                text: dmName
                                font.bold: true
                                font.pointSize: 12
                                color: "white"
                                wrapMode: Text.Wrap
                            }

                        }

                        MouseArea {
                            propagateComposedEvents: true
                            anchors.fill: parent
                            onPressed: {
                                mode_current_index = index
                                $signal_mode.set_current_index(index)
                                mode_current_name = dmName

                                //二期
                                modeIndexChange(index)
//                                $manager.select_mode(index, wall.width, wall.height)

                            }

                            onDoubleClicked: {
                                $manager.select_mode(index, wall.width, wall.height)
                            }
                            onPressAndHold: {
                                //弹出界面
                                modeMenu()
                            }
                        }
                    }
                }

            }
            Rectangle{
                        id: rect_signal_mode_title2
                        anchors.top: lv_signal_source_mode_col3.bottom
                        anchors.topMargin: 10

                        width: rect_signal_mode_title.width
                        height: 30
                        color: globalSettings.transparent_color
                        clip: true

                        Row{
                            anchors.fill: parent
                            spacing: 5
                            Text {

                                text: $manager.iszhongwen?qsTr("轮询模式"):qsTr("Polling")
                                width: parent.width - 20 - rect_empty2.width
                                height: 20
                                horizontalAlignment: Text.AlignHCenter
                                anchors.verticalCenter: parent.verticalCenter
                                font.bold: true
            //                    font.pointSize: 17
                                font.pixelSize:20
                                color: "white"
                            }

                            Rectangle {
                                color: globalSettings.transparent_color
                                height: 20
                                width: 20
                                id: rect_empty2
                            }
                        }
                    }
            Column{
                        id:lv_signal_source_mode_col2
                        anchors.top: rect_signal_mode_title2.bottom
                        anchors.topMargin: 10

                        ListModel{
                            id:lmPolling
                        }
                        spacing: 10
                        Repeater{
                            id: lv_signal_source_mode2

                            clip: true
                            model: lmPolling
                            function sence_changed(){
//                                console.log("clear")
                                sourceMode.current_index=-1

                                lmPolling.clear()
                                lmPolling.append($manager.get_group_polling())
                            }

                            function sence_changed2(index){
//                                console.log("clear")
                                sourceMode.current_index=-1
                                if(lmPolling.count==0){
                                    lmPolling.clear()
                                    lmPolling.append($manager.get_group_polling())
                                }

                            }
                            function status_changed(indexx){
//                               console.log(indexx);
                                sourceMode.current_index=indexx
                            }

                            Component.onCompleted: {
                                lmPolling.clear()
                                lmPolling.append($manager.get_group_polling())
                                $manager.sence_changed.connect(sence_changed)
                                $manager.sence_changed_from_polling.connect(sence_changed2)
                                $manager.polling_status_signal.connect(status_changed)



                            }

                            Rectangle {
                                id: rect_signal_mode_item2
                                anchors.left: lv_signal_source_mode_col2.left
                                anchors.leftMargin: 10
                                width: rect_signal_mode_title.width-20
                                height: 80
                                color: "#0e2439"
                                Image {
                                    id:img_polling
                                    anchors.fill: parent
                                    fillMode: Image.Stretch
                                    source:"qrc:/images/mode/img-"+index%5+".jpg"
                                }


                                Rectangle{
                                    anchors.top: parent.top
                                    anchors.left: parent.left
                                    anchors.right: parent.right
                                    height: 20
                                    color:(sourceMode.current_index===index)?globalSettings.selected_color:"#3b4043"
                                    border.color: Qt.lighter(color, 1.1)
                                    opacity: 0.6
                                    Text {
                                        anchors.centerIn: parent
                                        text: dmName
                                        font.bold: true
                                        font.pointSize: 12
                                        color: "white"
                                        wrapMode: Text.Wrap
                                    }

                                }

                                MouseArea {
                                    propagateComposedEvents: true
                                    anchors.fill: parent
                                    onClicked: {

                                        if(sourceMode.open_index===-1) //说明点击之前没有轮询启动
                                        {
//                                            sourceMode.current_index= index
                                            sourceMode.open_index=index
                                            md_polling_mode.operating_hint=1
                                            md_polling_mode.open_polling_index=index
                                            md_polling_mode.visible=true
                                            $manager.set_polling_num(sourceMode.open_index)
                                        }



                                        else if(sourceMode.open_index===index){  //停止当前轮询
                                            sourceMode.open_index=-1
//                                            sourceMode.current_index=-1
                                            md_polling_mode.operating_hint=0
                                            md_polling_mode.delete_polling_index=index
                                            md_polling_mode.visible=true
                                            $manager.set_polling_num(-1)


                                        }
                                        else{  //启动其他轮询 ，停止某个轮询
                                            md_polling_mode.delete_polling_index=sourceMode.open_index
//                                            sourceMode.current_index= index
                                            sourceMode.open_index=index
                                            md_polling_mode.operating_hint=2
                                            md_polling_mode.open_polling_index=index

                                            md_polling_mode.visible=true
                                            $manager.set_polling_num(sourceMode.open_index)
                                        }

                                    }

                                }
                            }
                        }

                    }
        }
//        Column{
//            id:lv_signal_source_mode_col
//            anchors.top: rect_signal_mode_title.bottom
//            anchors.topMargin: 10
//            anchors.left: parent.left
//            anchors.leftMargin: 10
//            anchors.rightMargin: 10
//            anchors.right: parent.right
//            spacing: 10
//            Repeater{
//                id: lv_signal_source_mode

//                clip: true

//                model: $signal_mode

//                Rectangle {
//                    id: rect_signal_mode_item
//                    width: parent.width
//                    height: 80
//                    color: "#0e2439"
//                    Image {
//                        anchors.fill: parent
//                        fillMode: Image.Stretch
//                        source: dmIco
//                    }


//                    Rectangle{
//                        anchors.top: parent.top
//                        anchors.left: parent.left
//                        anchors.right: parent.right
//                        height: 20
//                        color: $signal_mode.current_index===index?globalSettings.selected_color:"#3b4043"
//                        border.color: Qt.lighter(color, 1.1)
//                        opacity: 0.6
//                        Text {
//                            anchors.centerIn: parent
//                            text: dmName
//                            font.bold: true
//                            font.pointSize: 12
//                            color: "white"
//                            wrapMode: Text.Wrap
//                        }

//                    }

//                    MouseArea {
//                        propagateComposedEvents: true
//                        anchors.fill: parent
//                        onPressed: {
//                            mode_current_index = index
//                            $signal_mode.set_current_index(index)
//                            mode_current_name = dmName

//                            //二期
//                            modeIndexChange(index)

//                        }

//                        onDoubleClicked: {
//                            $manager.select_mode(index, wall.width, wall.height)
//                        }
//                        onPressAndHold: {
//                            //弹出界面
//                            modeMenu()
//                        }
//                    }
//                }
//            }

//        }
//        Rectangle{
//            id: rect_signal_mode_title2
//            anchors.top: lv_signal_source_mode_col.bottom
//            anchors.topMargin: 10
//            anchors.left: parent.left
//            anchors.right: parent.right
//            height: 30
//            color: globalSettings.transparent_color
//            clip: true

//            Row{
//                anchors.fill: parent
//                spacing: 5
//                Text {
//                    text: qsTr("信号模式")
//                    width: parent.width - 20 - rect_empty.width
//                    height: 20
//                    horizontalAlignment: Text.AlignRight
//                    font.bold: true
//                    font.pointSize: 17
//                    color: "white"
//                }
//                Image{
//                    id: rect_signal_mode_closeAction2
//                    height: 20
//                    width: 20
//                    state: "NORMAL"

//                    states: [
//                        State {
//                            name: "NORMAL"
//                            PropertyChanges { target: rect_signal_mode_closeAction2; source: "/images/add-idle.png" }
//                        },
//                        State {
//                            name: "HOVER"
//                            PropertyChanges { target: rect_signal_mode_closeAction2; source: "/images/add-active.png" }
//                        }
//                    ]

//                    MouseArea{
//                        anchors.fill: parent
//                        hoverEnabled: true

//                        onClicked: {
//                            //弹出界面
//                            modeAdd()
//                        }

//                        onEntered: {
//                            parent.state = "HOVER";
//                        }
//                        onExited: {
//                            parent.state = "NORMAL";
//                        }
//                    }
//                }
//                Rectangle {
//                    color: globalSettings.transparent_color
//                    height: 20
//                    width: 20
//                    id: rect_empty2
//                }
//            }
//        }
//        Column{
//            id:lv_signal_source_mode_col2
//            anchors.top: rect_signal_mode_title2.bottom
//            anchors.topMargin: 10
//            anchors.left: parent.left
//            anchors.leftMargin: 10
//            anchors.rightMargin: 10
//            anchors.right: parent.right
//            spacing: 10
//            Repeater{
//                id: lv_signal_source_mode2

//                clip: true

//                model: $signal_mode

//                Rectangle {
//                    id: rect_signal_mode_item2
//                    width: parent.width
//                    height: 80
//                    color: "#0e2439"
//                    Image {
//                        anchors.fill: parent
//                        fillMode: Image.Stretch
//                        source: dmIco
//                    }


//                    Rectangle{
//                        anchors.top: parent.top
//                        anchors.left: parent.left
//                        anchors.right: parent.right
//                        height: 20
//                        color: $signal_mode.current_index===index?globalSettings.selected_color:"#3b4043"
//                        border.color: Qt.lighter(color, 1.1)
//                        opacity: 0.6
//                        Text {
//                            anchors.centerIn: parent
//                            text: dmName
//                            font.bold: true
//                            font.pointSize: 12
//                            color: "white"
//                            wrapMode: Text.Wrap
//                        }

//                    }

//                    MouseArea {
//                        propagateComposedEvents: true
//                        anchors.fill: parent
//                        onPressed: {
//                            mode_current_index = index
//                            $signal_mode.set_current_index(index)
//                            mode_current_name = dmName

//                            //二期
//                            modeIndexChange(index)

//                        }

//                        onDoubleClicked: {
//                            $manager.select_mode(index, wall.width, wall.height)
//                        }
//                        onPressAndHold: {
//                            //弹出界面
//                            modeMenu()
//                        }
//                    }
//                }
//            }

//        }


//        ListView{
//            id: lv_signal_source_mode
//            anchors.top: rect_signal_mode_title.top
//            anchors.left: parent.left
//            anchors.right: parent.right
//            anchors.bottom: parent.bottom
//            anchors.leftMargin: 10
//            anchors.rightMargin: 10
//            anchors.topMargin: 40
//            anchors.bottomMargin: 10
//            clip: true
//            spacing: 10
//            model: $signal_mode

//            delegate:    Rectangle {
//                id: rect_signal_mode_item
//                width: parent.width
//                height: 80
//                color: "#0e2439"
//                Image {
//                    anchors.fill: parent
//                    fillMode: Image.Stretch
//                    source: dmIco
//                }


//                Rectangle{
//                    anchors.top: parent.top
//                    anchors.left: parent.left
//                    anchors.right: parent.right
//                    height: 20
//                    color: $signal_mode.current_index===index?globalSettings.selected_color:"#3b4043"
//                    border.color: Qt.lighter(color, 1.1)
//                    opacity: 0.6
//                    Text {
//                        anchors.centerIn: parent
//                        text: dmName
//                        font.bold: true
//                        font.pointSize: 12
//                        color: "white"
//                        wrapMode: Text.Wrap
//                    }

//                }

//                MouseArea {
//                    propagateComposedEvents: true
//                    anchors.fill: parent
//                    onPressed: {
//                        mode_current_index = index
//                        $signal_mode.set_current_index(index)
//                        mode_current_name = dmName

//                        //二期
//                        modeIndexChange(index)

//                    }

//                    onDoubleClicked: {
//                        $manager.select_mode(index, wall.width, wall.height)
//                    }
//                    onPressAndHold: {
//                        //弹出界面
//                        modeMenu()
//                    }
//                }
//            }
//        }



}


