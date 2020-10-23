import QtQuick 2.9
import QtQuick.Controls 2.2

Rectangle {
    id: loginPages
    anchors.fill: parent
    color: "transparent"


    signal showMainWindow
    Image {
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.top: parent.top
        anchors.topMargin: 10
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
    Rectangle{
        color: "#0d2439"
        anchors.centerIn: parent
        width: 300
        height: 250

        Column{
            anchors.fill: parent
            anchors.leftMargin: 30
            anchors.rightMargin: 30
            anchors.topMargin: 15
            spacing:10
            Rectangle{
                height: 20
                width: parent.width
                color: globalSettings.transparent_color
                Text {
                    anchors.centerIn: parent
                    font.bold: true
                    font.pixelSize: parent.height/4*3
                    height: 20
                    color: "white"
                    font.family: qsTr("新宋体")
                    text: $manager.iszhongwen?qsTr("可视化中央交互系统"):qsTr("Visual Central Interaction System")
                }
            }


            Rectangle{
                width: parent.width
                height: 30
                color: "black"

                Image {
                    id: img_login_user
                    anchors.left: parent.left
                    anchors.leftMargin: 2
                    anchors.top: parent.top
                    anchors.topMargin: 2
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 2
                    width: 26
                    height: 26
                    source: "/images/login-user.png"
                }
                TextInput{
                    id:usr_text
                    height: 30
                    anchors.left: img_login_user.right
                    anchors.leftMargin: 2
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.right: parent.right
                    verticalAlignment: TextInput.AlignVCenter
                    font.pixelSize: 18
                    text: "admin"
                    color: "white"
                }

            }
            Rectangle{
                width: parent.width
                height: 30
                color: "black"
                Image {
                    id: img_login_password
                    anchors.left: parent.left
                    anchors.leftMargin: 2
                    anchors.top: parent.top
                    anchors.topMargin: 2
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 2
                    width: 26
                    height: 26
                    source: "/images/login-passsword.png"
                }
                TextInput{
                    id:pwd_text
                    anchors.left: img_login_password.right
                    anchors.leftMargin: 2
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.right: parent.right
                    color: "white"
                    verticalAlignment: TextInput.AlignVCenter
                    font.pixelSize: 18
                    height: 30
                    text: ""
                    echoMode: TextInput.Password
                }
            }

            Rectangle{  //ip输入栏a
                width: parent.width
                height: 30
                color: "black"
                Image {
                    id: img_login_ip
                    anchors.left: parent.left
                    anchors.leftMargin: 2
                    anchors.top: parent.top
                    anchors.topMargin: 2
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 2
                    width: 26
                    height: 26
                    source: "qrc:/images/ip_adress.png"
                }
                TextInput{
                    id:textinpu_ip
                    anchors.left: img_login_ip.right
                    anchors.leftMargin: 2
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.right: parent.right
                    color: "white"
                    verticalAlignment: TextInput.AlignVCenter
                    font.pixelSize: 18
                    height: 30
//                        text: $room.current_user_password

                }
                Component.onCompleted: {
                   if($manager.getIP()){
                        textinpu_ip.text=$manager.getIP()
                       //ip状态为1 说明已经有保存的ip了

                   }
                }
            }
           Rectangle{
               width: parent.width
               height: 30
//               border.color: "white"
//               border.width: 1
               color: globalSettings.transparent_color

                   RadioButton {
                       id:rb1
                       checked: true
                       anchors.verticalCenter: parent.verticalCenter
                       x:-10
                   }
                   Text {
                       anchors.verticalCenter: rb1.verticalCenter
                       anchors.left: rb1.right
                       anchors.leftMargin: 10
                       text:  $manager.iszhongwen?qsTr("单场景"):qsTr("Single")
                       color:"white"
                   }
                   RadioButton {
                       anchors.verticalCenter: rb1.verticalCenter
                       anchors.left: parent.left
                       anchors.leftMargin: parent.width/2
                       id:rb2

                   }
                   Text {
                       anchors.verticalCenter: rb2.verticalCenter
                       anchors.left: rb2.right
                       anchors.leftMargin: 10
                    color:"white"
                    text: $manager.iszhongwen? qsTr("多场景"):qsTr("Multiple")
                   }


                  }
            Button{
                width: parent.width
                height: 30
                text: $manager.iszhongwen?qsTr("登 录"):qsTr("login")
                font.family: qsTr("新宋体")
                font.pointSize: 16
                font.bold: true
                highlighted: true
                flat: false
                background: Rectangle{
                    anchors.fill: parent
                    color: globalSettings.selected_color
                }
                z: parent.z + 5
                onPressed: {
                busy_rectt.opacity=1
                }
                onReleased: {

                   if(rb1.checked){
//                      console.log(1);
                      $manager.setIP(textinpu_ip.text,usr_text.text,pwd_text.text)
                   }

                   else{
//                        console.log(2);
                      $manager.login_multiple()
                   }

                }
//                onClicked: {

//                    busy_rectt.opacity=1



////                    showMainWindow()
//                }
            }
        }
    }
    Component.onCompleted: {
        if($manager.which_os()!=1){
           close_btn.visible=false

        }
        $manager.login_error.connect(loginerror)
        $manager.login_success.connect(loginsuccess)
         $manager.login_msg.connect(loginmsg)
        $manager.ip_error.connect(iperror)
         $manager.config_error.connect(configerror)
    }
    function configerror(){
        busystop()
        var ToastCom = Qt.createComponent("MessageBox.qml")
        if (ToastCom.status === Component.Ready)
           var detaObj = ToastCom.createObject(loginPages, { "focus": true,"msg": $manager.iszhongwen?qsTr("配置文件格式错误"):qsTr("Configuration file format error") })


    }
    function iperror(){
        busystop()
        var ToastCom = Qt.createComponent("MessageBox.qml")
        if (ToastCom.status === Component.Ready)
           var detaObj = ToastCom.createObject(loginPages, { "focus": true,"msg": $manager.iszhongwen?qsTr("IP格式错误"):qsTr("IP format error") })


    }
    function loginerror(){
        busystop()
        var ToastCom = Qt.createComponent("MessageBox.qml")
        if (ToastCom.status === Component.Ready)
           var detaObj = ToastCom.createObject(loginPages, { "focus": true,"msg": $manager.iszhongwen?qsTr("登录失败"):qsTr("login error") })


    }
    function loginmsg(hint){
       if(hint==1){
           if($manager.iszhongwen){
                 busy_text.text=qsTr("开始读取配置文件..")
           }
           else{
                busy_text.text=qsTr("Start reading configuration files..")
           }

       }

    }
    function loginsuccess(){

       showMainWindow();
    }
    function busystop(){

//       busy_rectt.opacity=0
        busy_rect_ani.start()
    }
    Rectangle{
        id:busy_rectt

        color: "#292A2E"
        width: 200
        height: 100
        radius: 10
        anchors.centerIn: parent

        opacity:0
        NumberAnimation {
            id:busy_rect_ani
            target: busy_rectt
            property: "opacity"
            to:0
            duration: 500
//            easing.type: Easing.InOutQuad
        }

            BusyIndicator{
              anchors.top: parent.top
              anchors.topMargin: 10
              anchors.horizontalCenter: parent.horizontalCenter
              id:busy_indicato
              running: true

            }

            Text{
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 10
                anchors.horizontalCenter: parent.horizontalCenter
                id: busy_text
                text: $manager.iszhongwen?qsTr("登录中.."):qsTr("Login...")

                color: "white"
            }



        }
}
