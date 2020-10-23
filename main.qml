import QtQuick 2.9
import QtQuick.Controls 2.2
import Qt.labs.settings 1.0
import QtQuick.Window 2.2

import QtQuick.Controls 1.2 as QC

ApplicationWindow {
    id: root
    visible: true
    //    width: 1200
    //    height:800
    width: 800
    height: 600

    //    minimumWidth:800
    //    minimumHeight: 600
    flags: Qt.CustomizeWindowHint | Qt.Window
    //flags: Qt.FramelessWindowHint | Qt.Window
    title: $manager.iszhongwen ? qsTr("电视墙管理") : qsTr("tvwall")
    property bool isFullScreen: false
    property int o_w: width
    property int o_H: height
    property int o_x: x
    property int o_y: y

    Settings {
        // 全局变量
        id: globalSettings
        property string userName
        property string password
        property string email
        property bool remeberPassword: true
        property bool autoLoad: true
        readonly property int maxLenthUserName: 100
        readonly property int maxLenthPassword: 20
        property int margin_size: 2
        property string selected_color: "#00c1ef"
        property string transparent_color: "transparent"
        property string border_color: "#667988"
        property string bottom_color: "#292A2E"
        property int border_size: 1
        property int left_width: 200
        property int right_width: 200
        property int head_height: 60
        property int bottom_height: 180
        property string font_color: "#a9b3bb"
        property string selected_font_color: "#00c1ef"
    }

    MouseArea {
        //为窗口添加鼠标事件
        id: mouse_win
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton //只处理鼠标左键
        enabled: $manager.which_os() == 1 ? true : false
        property point clickPos: "0,0"
        onPressed: {
            //接收鼠标按下事件
            clickPos = Qt.point(mouse.x, mouse.y)
        }
        onPositionChanged: {
            //鼠标按下后改变位置
            //鼠标偏移量
            var delta = Qt.point(mouse.x - clickPos.x, mouse.y - clickPos.y)

            //如果mainwindow继承自QWidget,用setPos
            root.setX(root.x + delta.x)
            root.setY(root.y + delta.y)
        }
    }
    Login {
        id: loginPage
        anchors.fill: parent
        visible: false
        z: parent.z + 3
    }

    Home {
        id: homePage
        anchors.fill: parent
        visible: false
        z: parent.z + 4
    }

    HomeHeaderSetting {
        // 宫格设置页面
        id: settingPage
        anchors.bottomMargin: 50
        anchors.fill: parent
        visible: false
        z: parent.z + 5
    }

    HomeHeaderSettingMode {
        //自定义宫格设置页面
        id: settingModePage
        anchors.bottomMargin: 50
        anchors.fill: parent
        visible: false
        z: parent.z + 6
    }
    HomeHeaderSettingResolution {
        id: settingModePageResolution
        anchors.bottomMargin: 50
        anchors.fill: parent
        visible: false
        z: parent.z + 6
    }
    SignalSourceModeAdd {
        //增加信号模式窗口
        id: signal_source_mode_add_pages
        anchors.fill: parent
        visible: false
        z: parent.z + 7
    }

    SignalSourceModeMenu {
        //信号模式菜单窗口
        id: signal_source_mode_menu_pages
        anchors.fill: parent
        visible: false
        anchors.bottomMargin: 50
        z: parent.z + 8
    }

    SignalSourceModeDeleteConfirm {
        //信号模式确认删除窗口
        id: signal_source_mode_delete_confirm_pages
        anchors.fill: parent
        visible: false
        anchors.bottomMargin: 50
        z: parent.z + 9
    }

    Component.onCompleted: {
        loginPage.visible = true

        $key.send_msg.connect(key_func)
        $key.error_msg.connect(error_func)
        $key.success_msg.connect(suc_func)
        $key.try_success_msg.connect(try_suc_func)
        $key.activation_msg.connect(activation_func)
        $key.hide_try_btn.connect(hide_func)
        $key.send_msg_again.connect(again_func)
    }
    function again_func(key) {

        //过期后重新激活
        loginPage.visible = false
        homePage.visible = false
        tipsPages.visible = false
        numPages.visible = false
        try_btn.visible = false
        numDisplayPages.visible = true
        lbl_name_show.text = key
        var ToastCom = Qt.createComponent("MessageBox.qml")
        if (ToastCom.status === Component.Ready)
            var detaObj = ToastCom.createObject(root, {
                                                    "focus": true,
                                                    "msg": $manager.iszhongwen ? qsTr("密匙过期，请联系供应商重新获取密匙") : qsTr("Key expired, please contact supplier to get key again")
                                                })
    }
    function hide_func(hint) {

        //隐藏试用按钮
        if (hint === 1)
            try_btn.visible = false
        else if (hint === 2)
            jump_btn.visible = false
    }
    function activation_func(msg) {

        //激活界面没有试用按钮
        loginPage.visible = false
        homePage.visible = false
        tipsPages.visible = false
        numPages.visible = false
        try_btn.visible = false
        numDisplayPages.visible = true
        lbl_name_show.text = msg
    }
    function key_func(tempmsg) {
        if (temp) {
            loginPage.visible = false
            homePage.visible = false
            tipsPages.visible = false
            numPages.visible = false
            numDisplayPages.visible = true
            lbl_name_show.text = msg
            var ToastCom = Qt.createComponent("MessageBox.qml")
            if (ToastCom.status === Component.Ready)
                var detaObj = ToastCom.createObject(root, {
                                                        "focus": true,
                                                        "msg": $manager.iszhongwen ? qsTr("请联系供应商获取密匙") : qsTr("Contact supplier for key")
                                                    })
        } else if (!temp) {
            console.log($manager.iszhongwen)
            loginPage.visible = false
            homePage.visible = false
            tipsPages.visible = true
            numDisplayPages.visible = false
            if (msg === "1") {
                if ($manager.iszhongwen)
                    tipsText.text = qsTr("config.ini文件错误,请联系供应商!")
                else
                    tipsText.text = qsTr(
                                "config.ini file error, please contact the vendor!")
            } else if (msg === "2") {
                if ($manager.iszhongwen)
                    tipsText.text = qsTr("密匙文件错误已损坏,请联系供应商获取新的密钥!")
                else
                    tipsText.text = qsTr(
                                "The key file error is corrupted, please contact the vendor for a new key!")
            } else {
                if ($manager.iszhongwen)
                    tipsText.text = qsTr("密匙文件错误,请联系供应商获取新的密钥!")
                else
                    tipsText.text = qsTr(
                                "The key file is wrong, please contact the vendor for a new key!")
            }
        }
    }

    function error_func(msg) {
        var ToastCom = Qt.createComponent("MessageBox.qml")
        if (ToastCom.status === Component.Ready)
            var detaObj = ToastCom.createObject(root, {
                                                    "focus": true,
                                                    "msg": msg
                                                })
    }
    function suc_func() {
        var ToastCom = Qt.createComponent("MessageBox.qml")
        if (ToastCom.status === Component.Ready)
            var detaObj = ToastCom.createObject(root, {
                                                    "focus": true,
                                                    "msg": $manager.iszhongwen ? qsTr("授权成功") : qsTr("Authorization succeeded")
                                                })
        loginPage.visible = true
        numPages.visible = false
    }
    function try_suc_func() {
        //30天试用成功
        var ToastCom = Qt.createComponent("MessageBox.qml")
        if (ToastCom.status === Component.Ready)
            var detaObj = ToastCom.createObject(root, {
                                                    "focus": true,
                                                    "msg": $manager.iszhongwen ? qsTr("30天试用开始") : qsTr("30-day trial begins")
                                                })
        loginPage.visible = true
        numDisplayPages.visible = false
    }
    Rectangle {
        //序列号生成框
        id: numDisplayPages
        width: 550
        height: 230
        anchors.centerIn: parent
        visible: false
        Rectangle {
            color: "#0d2439"
            anchors.fill: parent
            Image {
                //通过授权管理进入时才出现
                width: 15
                height: 15
                id: close_btn_1
                anchors.top: parent.top
                anchors.right: parent.right
                anchors.rightMargin: 10
                anchors.topMargin: 10
                visible: false
                source: "/images/app-close.png"
                MouseArea {
                    anchors.fill: parent
                    onClicked: {

                        tipsPages.visible = false
                        numPages.visible = false
                        try_btn.visible = false
                        numDisplayPages.visible = false
                        homePage.visible = true
                        close_btn_1.visible = false
                        close_btn_2.visible = false
                    }
                }
            }
            Text {
                id: numDisText
                text: $manager.iszhongwen ? qsTr("请把生成的序列号发送给供应商以获取密匙") : qsTr(
                                                "Please send the generated serial number to the vendor for the key")
                anchors.top: parent.top
                anchors.topMargin: 50
                anchors.horizontalCenter: parent.horizontalCenter
                color: "white"
                font.bold: true
                font.pixelSize: 15
            }

            Rectangle {
                id: ser_rect
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: 50
                anchors.rightMargin: 50
                anchors.top: numDisText.bottom
                anchors.topMargin: 20
                height: 60
                QC.TextArea {
                    id: lbl_name_show
                    anchors.fill: ser_rect
                    verticalScrollBarPolicy: Qt.ScrollBarAlwaysOff
                    implicitHeight: ser_rect.height
                    antialiasing: true
                    readOnly: true
                    font.pixelSize: parent.height / 3
                    wrapMode: TextEdit.WrapAnywhere
                    //                        frameVisible : false
                    //                        backgroundVisible : false
                }

                //                QC.TextArea {
                //                    id: lbl_name_show
                //                    text: "ssssss"
                //                    anchors.fill: parent
                ////                    readOnly: true
                //                    selectByMouse: true

                //                }
            }

            Button {
                width: parent.width / 4
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 20
                anchors.left: parent.left
                anchors.leftMargin: 50
                height: 30
                text: $manager.iszhongwen ? qsTr("输入密匙") : qsTr("Enter Key")
                font.family: qsTr("新宋体")
                font.pixelSize: height / 2
                font.bold: true
                highlighted: true
                flat: false
                background: Rectangle {
                    anchors.fill: parent
                    color: globalSettings.selected_color
                }

                onClicked: {
                    numPages.visible = true
                    numDisplayPages.visible = false
                }
            }
            Button {
                id: try_btn
                width: parent.width / 4
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 20
                anchors.horizontalCenter: parent.horizontalCenter
                height: 30
                text: $manager.iszhongwen ? qsTr("30天试用") : qsTr("30-day trial")
                font.family: qsTr("新宋体")
                font.pixelSize: height / 2
                font.bold: true
                highlighted: true
                flat: false
                background: Rectangle {
                    anchors.fill: parent
                    color: globalSettings.selected_color
                }

                onClicked: {
                    $key.try_30_days()
                }
            }
            Button {
                width: parent.width / 4
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 20
                anchors.right: parent.right
                anchors.rightMargin: 50
                height: 30
                text: $manager.iszhongwen ? qsTr("退 出") : qsTr("Exit")
                font.family: qsTr("新宋体")
                font.pixelSize: height / 2
                font.bold: true
                highlighted: true
                flat: false
                background: Rectangle {
                    anchors.fill: parent
                    color: globalSettings.selected_color
                }

                onClicked: {
                    Qt.quit()
                }
            }
        }
    }
    Rectangle {
        //密匙输入框
        id: numPages
        width: 500
        height: 230
        anchors.centerIn: parent
        visible: false
        Rectangle {
            color: "#0d2439"
            anchors.fill: parent
            Image {
                //通过授权管理进入时才出现
                width: 15
                height: 15
                id: close_btn_2
                anchors.top: parent.top
                anchors.right: parent.right
                anchors.rightMargin: 10
                anchors.topMargin: 10
                visible: false
                source: "/images/app-close.png"
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        tipsPages.visible = false
                        numPages.visible = false
                        try_btn.visible = false
                        numDisplayPages.visible = false
                        homePage.visible = true
                        close_btn_1.visible = false
                        close_btn_2.visible = false
                    }
                }
            }
            Text {
                id: numText
                text: $manager.iszhongwen ? qsTr("请输入密匙") : qsTr(
                                                "Please enter key")
                anchors.top: parent.top
                anchors.topMargin: 50
                anchors.horizontalCenter: parent.horizontalCenter
                color: "white"
                font.bold: true
                font.pixelSize: 15
            }
            TextField {
                id: numInput
                anchors.top: numText.bottom
                anchors.topMargin: 20
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: 50
                anchors.rightMargin: 50
                height: 30
                placeholderText: $manager.iszhongwen ? qsTr("请输入") : qsTr(
                                                           "Please input")
                color: "black"
                background: Rectangle {
                    color: "white"
                    anchors.fill: parent
                }
            }

            Button {
                width: numInput.width / 2 - 20
                anchors.top: numInput.bottom
                anchors.topMargin: 20
                anchors.left: parent.left
                anchors.leftMargin: 50
                height: 30
                text: $manager.iszhongwen ? qsTr("确 定") : qsTr("ok")
                font.family: qsTr("新宋体")
                font.pixelSize: height / 2
                font.bold: true
                highlighted: true
                flat: false
                background: Rectangle {
                    anchors.fill: parent
                    color: globalSettings.selected_color
                }

                onClicked: {

                    if (numInput.text == "" || numInput.text.indexOf(
                                "-") === -1) {
                        var ToastCom = Qt.createComponent("MessageBox.qml")
                        if (ToastCom.status === Component.Ready)
                            var detaObj = ToastCom.createObject(root, {
                                                                    "focus": true,
                                                                    "msg": $manager.iszhongwen ? qsTr("请输入正确的序列号") : qsTr("Please enter the correct serial number")
                                                                })
                        return
                    }
                    $key.set_num(numInput.text)
                }
            }
            Button {
                width: numInput.width / 2 - 20
                anchors.top: numInput.bottom
                anchors.topMargin: 20
                anchors.right: parent.right
                anchors.rightMargin: 50
                height: 30
                text: $manager.iszhongwen ? qsTr("退 出") : qsTr("Exit")
                font.family: qsTr("新宋体")
                font.pixelSize: height / 2
                font.bold: true
                highlighted: true
                flat: false
                background: Rectangle {
                    anchors.fill: parent
                    color: globalSettings.selected_color
                }

                onClicked: {
                    Qt.quit()
                }
            }
        }
    }
    Rectangle {
        //授权提示框
        visible: false
        id: tipsPages
        width: 300
        height: 230
        anchors.centerIn: parent
        Rectangle {
            color: "#0d2439"
            anchors.fill: parent
            Rectangle {
                height: 20
                width: parent.width
                color: globalSettings.transparent_color
                Text {
                    anchors.centerIn: parent
                    font.bold: true
                    font.pointSize: 15
                    height: 20
                    color: "white"
                    font.family: qsTr("新宋体")
                    text: $manager.iszhongwen ? qsTr("提示") : qsTr("Tips")
                }
            }
            Text {

                width: parent.width - 20
                id: tipsText
                color: "white"
                anchors.centerIn: parent
                text: qsTr("错误")
                wrapMode: Text.WordWrap
            }

            Button {
                width: parent.width / 3
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 20
                anchors.right: parent.right
                anchors.rightMargin: 20
                height: 30
                text: $manager.iszhongwen ? qsTr("退 出") : qsTr("Exit")
                font.family: qsTr("新宋体")
                font.pixelSize: height / 2
                font.bold: true
                highlighted: true
                flat: false
                background: Rectangle {
                    anchors.fill: parent
                    color: globalSettings.selected_color
                }
                z: parent.z + 5
                onClicked: {
                    Qt.quit()
                }
            }
            Button {
                id: jump_btn
                width: parent.width / 3
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 20
                anchors.left: parent.left
                anchors.leftMargin: 20
                height: 30
                text: $manager.iszhongwen ? qsTr("授 权") : qsTr("Authorization")
                font.family: qsTr("新宋体")
                font.pixelSize: height / 2
                font.bold: true
                highlighted: true
                flat: false
                background: Rectangle {
                    anchors.fill: parent
                    color: globalSettings.selected_color
                }
                z: parent.z + 5
                onClicked: {
                    //                         $manager.get_current_dir();
                    $key.activation()
                }
            }
        }
    }

    Connections {
        //登录框的信号连接
        target: loginPage
        onShowMainWindow: {
            loginPage.visible = false
            homePage.visible = true
            //            busy_rect.visible=true
        }
    }
    Connections {
        //改变布局
        target: settingPage
        onSelectLogicalMode: {

            settingPage.visible = false
            if (idx == 5) {
                settingModePage.visible = true
            } else if (idx == 6) {

                //刷新窗口
                $manager.fresh_all_signal_in_mode()
                homePage.visible = true
            } else if (idx == 7) {
                //清空窗口
                $manager.delete_all_signal_in_mode()
                homePage.visible = true
            } else if (idx == 8) {
                //中英切换
                $manager.language_switch()
                homePage.visible = true
            } else if (idx == 9) {
                //授权管理   此时的授权是可以返回主页的
                close_btn_1.visible = true
                close_btn_2.visible = true
                $key.activation()
            } else if (idx == 10) {
                //授权管理   此时的授权是可以返回主页的
                settingModePageResolution.visible = true
            } else {
                $manager.logical_wall_column = lg_column
                $manager.logical_wall_row = lg_row
                //                $manager.save_conf();
                homePage.visible = true
            }
        }

        onCloseSetting: {
            settingPage.visible = false
            homePage.visible = true
        }
    }

    Connections {
        //通过自定义改变布局
        target: settingModePage
        onSelfLogicalMode: {
            settingModePage.visible = false
            $manager.logical_wall_column = lg_column
            $manager.logical_wall_row = lg_row
            //            $manager.save_conf();
            homePage.visible = true
        }

        onCloseSetting: {
            settingModePage.visible = false
            homePage.visible = true
        }
    }

    Connections {
        //打开各种额外页面
        target: homePage
        onShowSettingbyHome: {
            //打开布局设置页面
            homePage.visible = false
            settingPage.visible = true
        }

        onModeAddbyHome: {
            //增加信号模式页面
            $manager.read_mode_images()
            homePage.visible = false
            signal_source_mode_add_pages.visible = true
        }

        onModeMenubyHome: {
            //D区信号模式菜单页面
            homePage.visible = false
            signal_source_mode_menu_pages.mode_current_index = idx
            signal_source_mode_menu_pages.mode_current_name = name
            signal_source_mode_menu_pages.visible = true
        }
        //        //二期
        //        onModeIndexChangebyHome:{
        //           signal_source_mode_menu_pages.mode_current_index = indexx
        //        }
        onWindowhint: {

            if (dd) {

                mouse_win.enabled = false
            } else {
                mouse_win.enabled = true
            }
        }
    }

    Connections {
        target: signal_source_mode_add_pages

        onCloseSetting: {
            signal_source_mode_add_pages.visible = false
            homePage.visible = true
        }
    }
    Connections {
        target: settingModePageResolution

        onCloseSetting: {
            settingModePageResolution.visible = false
            homePage.visible = true
        }
    }
    Connections {
        target: signal_source_mode_menu_pages

        onModeManager: {
            //            console.log("onModeManager", idx, signal_source_mode_menu_pages.mode_current_index, signal_source_mode_menu_pages.mode_current_name)
            signal_source_mode_menu_pages.visible = false
            if (idx == 0) {
                // 保存当前信号模式
                $manager.update_mode(
                            signal_source_mode_menu_pages.mode_current_index)
                homePage.visible = true
            } else {
                //弹出删除确认框
                signal_source_mode_delete_confirm_pages.delete_mode_name
                        = signal_source_mode_menu_pages.mode_current_name
                signal_source_mode_delete_confirm_pages.delete_index
                        = signal_source_mode_menu_pages.mode_current_index
                signal_source_mode_delete_confirm_pages.visible = true
            }
        }

        onCloseSetting: {
            signal_source_mode_menu_pages.visible = false
            homePage.visible = true
        }
    }

    Connections {
        target: signal_source_mode_delete_confirm_pages

        onCloseSetting: {
            signal_source_mode_delete_confirm_pages.visible = false
            homePage.visible = true
        }
    }

    background: Image {
        anchors.fill: parent
        source: $manager.background_image
        z: parent.z - 3
    }
}
