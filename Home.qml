import QtQuick 2.9
import QtQuick.Controls 1.4
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Window 2.2
import QtGraphicalEffects 1.0

Rectangle {
    id: homePages
    color: globalSettings.transparent_color
    anchors.fill: parent

    signal showSettingbyHome
    signal modeAddbyHome
    signal modeMenubyHome(int idx, string name)
    signal windowhint(bool dd) //窗口状态，满屏时不可以移动窗口
    //    signal modeIndexChangebyHome(int indexx) //发送当前的信号模式的index给刷新窗口功能使用
    property int wall_width: wall.width
    property int wall_height: wall.height
    property int wall_x: wall.x
    property int wall_y: wall.y + rect_head.height

    property bool isDisplay: false

    property real height_mm;
    property real width_mm;
    property real hypotenuse;
    property var whichDecvice: "win"

    MouseArea{   //模拟模态的触屏区域

       id:modal_mousearea
       anchors.fill: parent
       enabled: false


    }
//    Keys.onPressed: {
//              switch(event.key)
//              {
//              case Qt.Key_Escape:
//              {

//                 console.log(root.old_win_w)

//              }
//                  break;
//              default:
//                  return;
//              }

//    }
    function get_in(){  //计算屏幕尺寸并判断设备种类

      height_mm= Screen.desktopAvailableHeight/Screen.pixelDensity.toFixed(2)
      width_mm=Screen.desktopAvailableWidth/Screen.pixelDensity.toFixed(2)
      height_mm=height_mm*0.0393701  //转为英寸
      width_mm=width_mm*0.0393701
      hypotenuse= Math.sqrt(height_mm*height_mm+width_mm*width_mm)

      if($manager.which_os()==1)
          whichDecvice="win"


      else{
        if(hypotenuse.toFixed(2)<7)
            whichDecvice="phone"

        else
            whichDecvice="pad"

      }

      console.log(whichDecvice)
    }

    Row {
        //最小化和关闭按钮
        id:menu_row
        anchors.topMargin: 10
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.rightMargin: 25
        width: 35
        height: 12
        spacing: 5
        z:2
        Image {
            width: 15
            height: 15

            source: "/images/min.png"
            MouseArea {
                anchors.fill: parent
                onClicked: {

                    root.visibility = Window.Minimized
                }
            }
        }

        Image {
            width: 15
            height: 15
            id:full_btn
            source:root.isFullScreen?"/images/re.png": "/images/full.png"
            MouseArea {
                anchors.fill: parent
                onClicked: {
//                    $manager.free_memory()

                    if(root.isFullScreen){
                           windowhint(false)
                           root.setX( root.o_x)
                           root.setY( root.o_y)
                           root.setWidth(root.o_w)
                           root.setHeight(root.o_H)
                           root.isFullScreen=!root.isFullScreen
                    }
                    else{

                          windowhint(true)
                          root.o_H=root.height
                          root.o_w=root.width
                          root.o_x=root.x
                          root.o_y=root.y
                          root.setX(0)
                          root.setY(0)
                          root.setWidth(Screen.width+1)
                          root.setHeight(Screen.height+1)
                          root.isFullScreen=!root.isFullScreen

                    }

                }
            }
        }
        Image {
            width: 15
            height: 15
            id:close_btn
            source: "/images/app-close.png"
            MouseArea {
                anchors.fill: parent
                onClicked: {
//                    $manager.free_memory()
                    Qt.quit()
                }
            }
        }
        Component.onCompleted: {
           if($manager.which_os()!=1){
              menu_row.visible=false

           }

        }
    }

    SplitView {
        anchors.fill: parent
        orientation: Qt.Vertical
        resizing: false

        Rectangle {
            //A区
            id: rect_head
            width: parent.width
            height: globalSettings.head_height
            Layout.minimumHeight: globalSettings.head_height
            Layout.maximumHeight: globalSettings.head_height
            color: globalSettings.transparent_color
            border.width: globalSettings.border_size
            border.color: Qt.lighter(globalSettings.border_color, 1.1)
            Image {
                anchors.left: parent.left
                anchors.leftMargin: 1
                anchors.verticalCenter: parent.verticalCenter
                height: parent.height/2
                width: height*4
                z:2
//                source: "qrc:/images/logo.png"

            }
            Image {
                width: parent.width
                anchors.top: parent.top
                anchors.topMargin: close_btn.height
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                source: "qrc:/images/logo/0914.png"

                Text {
                    id: name
                    text: $manager.iszhongwen?qsTr("可视化中央交互系统"):qsTr("Visual Central Interaction System")
                    font.family: "微软雅黑"
                    font.pixelSize: parent.height/2
                    font.bold: true
                    anchors.centerIn: parent
                    color: "white"


                }
            }
//            HomeHeader {
//                id: homeHeaderPages
//                anchors.fill: parent
//            }
//            Connections {
//                target: homeHeaderPages
//                onShowSetting: {
//                    showSettingbyHome()
//                }
//            }
            Image {  //使用设备为手机时出现
                id: settingAction
                width: parent.height/2
                height:width
                state: "NORMAL"
                z:1
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                visible: whichDecvice=="phone"?true:false

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

//                         console.log(hypotenuse.toFixed(2))
//                        var ToastCom = Qt.createComponent("MessageBox.qml")
//                        if (ToastCom.status === Component.Ready)
//                            var detaObj = ToastCom.createObject(homePages, {
//                                                                    "focus": true,
//                                                                    "msg": hypotenuse.toFixed(2)
//                                                                })

                        //弹出界
                        if(!isDisplay){
                            rect_ctrl_mobile.y=homePages.height-rect_ctrl_mobile.height

                        }
                        else{
                            rect_ctrl_mobile.y=homePages.height

                        }

                        isDisplay=!isDisplay

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
        Rectangle {
            //B,C,D区
            id: rect_work
            width: parent.width
            Layout.fillHeight: true
//            Layout.minimumHeight: 200
            color: globalSettings.transparent_color

            SplitView {
                anchors.fill: parent
                orientation: Qt.Horizontal
                resizing: false

                Rectangle {
                    //B区
                    width: 150
                    height: parent.height
                    Layout.minimumWidth:150
//                    Layout.maximumWidth: globalSettings.left_width
                    color: globalSettings.transparent_color
                    border.width: globalSettings.border_size
                    border.color: Qt.lighter(globalSettings.border_color, 1.1)
                    SignalSourceList {
                        anchors.fill: parent
                    }
                }
                Rectangle {
                    //C区
                    id: wall
                    height: parent.height
                    Layout.fillWidth: true
                    Layout.minimumWidth: 200
                    color: globalSettings.transparent_color
                    border.width: globalSettings.border_size
                    border.color: Qt.lighter(globalSettings.border_color, 1.1)

                    SignalSourceDisplay {
                        anchors.fill: parent
                    }
                    Component.onCompleted: {

                        $manager.manager_set_wall_height(wall.height)
                        $manager.manager_set_wall_width(wall.width)
                    }
                }
                Rectangle {
                    //D区  信号模式区
                    width: 150
                    height: parent.height
                    Layout.minimumWidth: 150
//                    Layout.maximumWidth: globalSettings.right_width
                    color: globalSettings.transparent_color
                    border.width: globalSettings.border_size
                    border.color: Qt.lighter(globalSettings.border_color, 1.1)
                    clip: true
                    SignalSourceMode {
                        id: signal_source_mode_pages
                        anchors.fill: parent
                    }

                    Connections {
                        target: signal_source_mode_pages
                        onModeAdd: {
                            modeAddbyHome()
                        }
                        onModeMenu: {
                            modeMenubyHome(
                                        signal_source_mode_pages.mode_current_index,
                                        signal_source_mode_pages.mode_current_name)
                        }
                        //二期
//                        onModeIndexChange:{
//                            modeIndexChangebyHome(indexx)

//                        }
                    }


                }
            }
        }
        Rectangle {
            //E,F区
            id: rect_ctrl
            width: parent.width
            height: globalSettings.bottom_height
            Layout.minimumHeight: globalSettings.bottom_height
            Layout.maximumHeight: globalSettings.bottom_height
            color: globalSettings.bottom_color
            border.width: globalSettings.border_size
            border.color: Qt.lighter(globalSettings.border_color, 1.1)
            visible: whichDecvice=="phone"?false:true
            SplitView {
                anchors.fill: parent
                orientation: Qt.Horizontal
                resizing: false

                Rectangle {
                    //E区
//                    width: globalSettings.left_width
                    width: 150
                    height: parent.height
                    Layout.minimumWidth: 150
//                    Layout.maximumWidth: globalSettings.left_width
                    color: globalSettings.transparent_color
                    border.width: globalSettings.border_size
                    border.color: Qt.lighter(globalSettings.border_color, 1.1)
                    clip: true
                    DeviceList {
                        id: deviceList
                        anchors.fill: parent
                    }
                }
                Rectangle {
                    //F区
                    height: parent.height
                    Layout.fillWidth: true
                    Layout.minimumWidth: 200
                    color: globalSettings.transparent_color
                    border.width: globalSettings.border_size
                    border.color: Qt.lighter(globalSettings.border_color, 1.1)

                    DeviceCtrl {
                        id: deviceCtrl
                        anchors.fill: parent
                        visible: false
                    }
                    DeviceAll {
                        //总控区域
                        id: allDeviceCtrl
                        anchors.fill: parent
                        visible: true
                    }
                }
                Connections {
                    //下拉框的信号连接
                    target: deviceList
                    onControlSort: {
                        if (hint === 1) {
                            deviceCtrl.visible = true
                            allDeviceCtrl.visible = false
                        } else {
                            deviceCtrl.visible = false
                            allDeviceCtrl.visible = true
                        }
                    }
                }
                Connections {
                    //下拉框的信号连接
                    target: allDeviceCtrl
                    onShowSetting: {
                        showSettingbyHome()

                    }
//                    onSimulation: {
//                        if(hint===1){

//                           modal_mousearea.enabled=true
//                        }
//                        else{
//                          modal_mousearea.enabled=false
//                        }

//                    }
                }
            }
        }

    }


    //设备是手机的时候，E,F区域的布局改变
    Rectangle {
        y:parent.height
        //E,F区
        id: rect_ctrl_mobile
        width: parent.width
        height: globalSettings.bottom_height
        color: globalSettings.bottom_color
        border.width: globalSettings.border_size
        border.color: Qt.lighter(globalSettings.border_color, 1.1)
        visible: whichDecvice=="phone"?true:false

        Behavior on y {
                 NumberAnimation{
                    duration:500
                 }

               }

        SplitView {
            anchors.fill: parent
            orientation: Qt.Horizontal
            resizing: false

            Rectangle {
                //E区
//                    width: globalSettings.left_width
                width: 150
                height: parent.height
                Layout.minimumWidth: 150
//                    Layout.maximumWidth: globalSettings.left_width
                color: globalSettings.transparent_color
                border.width: globalSettings.border_size
                border.color: Qt.lighter(globalSettings.border_color, 1.1)
                clip: true
                DeviceList {
                    id: deviceList_mobile
                    anchors.fill: parent
                }
            }
            Rectangle {
                //F区
                height: parent.height
                Layout.fillWidth: true
                Layout.minimumWidth: 200
                color: globalSettings.transparent_color
                border.width: globalSettings.border_size
                border.color: Qt.lighter(globalSettings.border_color, 1.1)

                DeviceCtrl {
                    id: deviceCtrl_mobile
                    anchors.fill: parent
                    visible: false
                }
                DeviceAll {
                    //总控区域
                    id: allDeviceCtrl_mobile
                    anchors.fill: parent
                    visible: true
                }
            }
            Connections {
                //下拉框的信号连接
                target: deviceList_mobile
                onControlSort: {
                    if (hint === 1) {
                        deviceCtrl_mobile.visible = true
                        allDeviceCtrl_mobile.visible = false
                    } else {
                        deviceCtrl_mobile.visible = false
                        allDeviceCtrl_mobile.visible = true
                    }
                }
            }
            Connections {

                target: allDeviceCtrl_mobile
                onShowSetting: {

                    showSettingbyHome()


                }
//                onSimulation: {


//                }
            }
        }
    }




    Rectangle {

        // 轮询模式提示框
        id: md_polling_mode
        visible: false
        width: 200
        height: 150

        signal md_polling_mode_result(int hint)
        property int delete_polling_index: -1
        property int open_polling_index: -1
        property int operating_hint: -1
        property string selected_color: "#00c1ef" //被选中的对象的背景色
        anchors.centerIn: parent
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
                    text: md_polling_mode.operating_hint >= 1 ? ($manager.iszhongwen?qsTr("启动轮询"):qsTr("Start Polling") ): ($manager.iszhongwen?qsTr(
                                                                    "停止轮询"):qsTr("Stop Polling"))
                    font.pointSize: 16
                    color: "gray"
                }
                Text {
                    height: 40
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: md_polling_mode.operating_hint >= 1 ? ($manager.iszhongwen?qsTr("确认启动？"):qsTr("Open?")): ($manager.iszhongwen?qsTr(
                                                                    "确认停止？"):qsTr("Stop?"))
                    font.bold: true
                    font.pointSize: 16
                }
                Row {
                    anchors.horizontalCenter: parent.horizontalCenter
                    height: 30
                    spacing: 10
                    Rectangle {
                        width: 60
                        height: 25
                        color: md_polling_mode.selected_color
                        Button {
                            anchors.fill: parent
                            text: $manager.iszhongwen?qsTr("取消"):qsTr("cancel")
                            highlighted: true
                            flat: false
                            background: Rectangle {
                                anchors.fill: parent
                                color: md_polling_mode.selected_color
                            }
                            onClicked: {
                                md_polling_mode.visible = false
                                //                                signal_source_mode_pages.current_index=-1
                                //                                md_polling_mode_result(0)
                                if (md_polling_mode.operating_hint === 1) {
                                    //仅仅开启某个轮询
                                    signal_source_mode_pages.current_index = -1
                                    signal_source_mode_pages.open_index = -1
                                } else if (md_polling_mode.operating_hint === 0) {
                                    //仅仅停止某个轮询
                                    signal_source_mode_pages.current_index
                                            = md_polling_mode.delete_polling_index
                                    signal_source_mode_pages.open_index
                                            = md_polling_mode.delete_polling_index
                                } else if (md_polling_mode.operating_hint === 2) {

                                    signal_source_mode_pages.current_index
                                            = md_polling_mode.delete_polling_index
                                    signal_source_mode_pages.open_index
                                            = md_polling_mode.delete_polling_index
                                }
                            }
                        }
                    }
                    Rectangle {
                        width: 60
                        height: 25
                        color: "red"
                        Button {
                            anchors.fill: parent
                            text: $manager.iszhongwen?qsTr("确认"):qsTr("ok")
                            highlighted: true
                            flat: false
                            background: Rectangle {
                                anchors.fill: parent
                                color: "red"
                            }
                            onClicked: {

                                md_polling_mode.visible = false
                                if (md_polling_mode.operating_hint === 1) {

                                    //仅仅开启某个轮询
                                    $manager.set_polling(
                                                md_polling_mode.open_polling_index,
                                                -1)
                                    //                                    console.log("open"+md_polling_mode.open_polling_index)
                                } else if (md_polling_mode.operating_hint === 0) {
                                    //仅仅停止某个轮询
                                    $manager.set_polling(
                                                -1,
                                                md_polling_mode.delete_polling_index)
                                    //                                    console.log("close"+md_polling_mode.delete_polling_index)
                                } else if (md_polling_mode.operating_hint === 2) {

                                    $manager.set_polling(
                                                md_polling_mode.open_polling_index,
                                                md_polling_mode.delete_polling_index)
                                    //                                      console.log("open"+md_polling_mode.open_polling_index+",close"+md_polling_mode.delete_polling_index)
                                }
                            }
                        }
                    }
                }
            }
        }
    }


    Rectangle{
        id:busy_rect
        visible: false
        color: "#292A2E"
        width: 200
        height: 100
        radius: 10
        anchors.centerIn: parent
        z:parent.z + 1
        Column{
            anchors.centerIn: parent
            BusyIndicator{

              id:busy_indicato
              running: true

            }

            Text{
                id: busy_text
                text: $manager.iszhongwen?qsTr("加载中.."):qsTr("Loading..")

                color: "white"
            }


            }
        }
    Rectangle{
        id:reconnect_rect
        visible: false
        color: "#292A2E"
        width: 200
        height: 100
        radius: 10
        anchors.centerIn: parent
        z:parent.z + 1
        Column{
            anchors.centerIn: parent
            BusyIndicator{

              id:reconnect_indicato
              running: true

            }

            Text{
                id: reconnect_text
                text: $manager.iszhongwen?qsTr("重连中.."):qsTr("Reconnecting..")

                color: "white"
            }


            }
        }
    Component.onCompleted: {
        get_in()
        $manager.scene_type.connect(sceneTypeTip)
        $manager.disconnect_signal.connect(disconnectFunc)
        $manager.reconnect_signal.connect(reconnectFunc)
    }
    function disconnectFunc(){

          reconnect_rect.visible=true
   }
    function reconnectFunc(){

          reconnect_rect.visible=false
   }
    function sceneTypeTip(type) {
        busy_rect.visible=false
        if (type === 1) {

            var ToastCom = Qt.createComponent("MessageBox.qml")
            if (ToastCom.status === Component.Ready)
                var detaObj = ToastCom.createObject(homePages, {
                                                        "focus": true,
                                                        "msg": $manager.iszhongwen?qsTr("单场景"):qsTr("Single Scene")
                                                    })
        } else {

            var ToastCom = Qt.createComponent("MessageBox.qml")
            if (ToastCom.status === Component.Ready)
                var detaObj = ToastCom.createObject(homePages, {
                                                        "focus": true,
                                                        "msg": $manager.iszhongwen?qsTr("多场景"):qsTr("Multiple Scenes")
                                                    })
        }
    }
}
