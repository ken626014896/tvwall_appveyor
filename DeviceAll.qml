﻿import QtQuick 2.0
import QtGraphicalEffects 1.12
import QtQuick.Window 2.3
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.1
Rectangle {  //总控区域  功能1.用于关闭多个开关  功能2 控制色相头  功能3 。。。。
    color: "#292A2E"
//    color: "#2B313F"
    anchors.fill: parent
    id:root_device_all
    property color btnColor: "#009688"
    property int topmargin : 30


//    property bool top_flag: false  //方向标记
//    property bool bottom_flag: false
//    property bool left_flag: false
//    property bool right_flag: false
//    property bool increase_flag: false
//    property bool decrease_flag: false

    property var detaObjMsg
    property var powerisclick: -1
    property var screenisclick: -1
    signal simulation(int hint)
    signal showSetting
    function msgTip() {


            var ToastCom = Qt.createComponent("MessageBox.qml")
            if (ToastCom.status === Component.Ready)
                var detaObj = ToastCom.createObject(homePages, {
                                                        "focus": true,
                                                      "msg": $manager.iszhongwen?qsTr("没有设备!"):qsTr("No Device！")
                                                    })
    }
    function msgSend(a,b){ //a为文本，b为标志号

        simulation(1)
        var ToastCom = Qt.createComponent("TippBox.qml")
        if (ToastCom.status === Component.Ready)
            detaObjMsg = ToastCom.createObject(homePages, {
                                                    "focus": true,
                                                    "msg": a,
                                                    "num":b

                                                })
        detaObjMsg.numSig.connect(doSomething)


    }
    function doSomething(num){

        if(num===1){
             root_device_all.powerisclick=1
            detaObjMsg.destroy()
        }
        else if(num===2){
             root_device_all.powerisclick=2
            detaObjMsg.destroy()

        }
        else if(num===3){
            root_device_all.screenisclick=1
            detaObjMsg.destroy()
        }
        else if(num===4){
           root_device_all.screenisclick=2
            detaObjMsg.destroy()

        }
        else if(num===5){
           detaObjMsg.destroy()

        }

       simulation(2)


    }
    Column{
       anchors.fill: parent

       Rectangle{
          height: parent.height/4
          width: parent.width
          color: globalSettings.transparent_color

          Rectangle{  //场景区域
             id:scene_rect
             height: parent.height
             width: parent.width/2
             color: globalSettings.transparent_color
             anchors.left: parent.left
//             border.color: "white"
//             border.width: 1
             Rectangle{
                id:scene_text_rect
                height: parent.height-10
                width: parent.width/6
                radius: 5
                anchors.left: parent.left
                anchors.leftMargin: 10
                color: "#7EFF79"
                anchors.verticalCenter: parent.verticalCenter

                Text {
                    anchors.centerIn: parent
                    text: $manager.iszhongwen?qsTr("场景应用"):qsTr("Scene")
                    font.pixelSize:  parent.height/3
                    font.bold: true
                }

             }

             ListView {
                 id: listview
                 anchors.verticalCenter: parent.verticalCenter
                 anchors.left: scene_text_rect.right
                 anchors.leftMargin: 20
                 anchors.right: parent.right
                 height: parent.height-10
                 width: parent.width/6*5


                 anchors.rightMargin: 10
                 focus: false
                 orientation: ListView.Horizontal
                 boundsBehavior: Flickable.StopAtBounds
                 snapMode: ListView.SnapOneItem
                 highlightRangeMode: ListView.StrictlyEnforceRange
                 highlightMoveDuration: 250
                 clip: true
                 spacing: 10

                 model: $room
                 delegate: Rectangle {
                     anchors.verticalCenter: parent.verticalCenter
                     width: 100
                     height: listview.height
                     color: $room.current_index===index?globalSettings.selected_color:globalSettings.transparent_color
                     border.color: $room.current_index===index?globalSettings.transparent_color:globalSettings.border_color
                     border.width: $room.current_index===index?0:2

                     radius: 20
                     Text {
                         anchors.centerIn: parent
                         text: dmName
                         font.bold: true
                         font.pointSize: 10
                         color: "white"
                         wrapMode: Text.Wrap
                     }
                     MouseArea {
                         propagateComposedEvents: true
                         anchors.fill: parent
                         onClicked: {
                             $room.set_current_index(index)
                         }
                     }
                 }
             }


          }

          Rectangle{  // 宫格变化
              id:grid_rect
              height: parent.height
              width: parent.width/2
              color: globalSettings.transparent_color
              anchors.right: parent.right
//              border.color: "white"
//              border.width: 1

              Rectangle{
                 id:grid_text_rect
                 height: parent.height-10
                 width: parent.width/6
                 radius: 5
                 anchors.left: parent.left
                 anchors.leftMargin: 10
                 color: "#7EFF79"
                 anchors.verticalCenter: parent.verticalCenter

                 Text {
                     anchors.centerIn: parent
                     text: $manager.iszhongwen?qsTr("画面分割"):qsTr("Cut up")
                     font.pixelSize:  parent.height/3
                     font.bold: true
                 }

              }

              Rectangle{
                  id:grid_rect01
//                  border.color: "white"
//                  border.width: 1
                  color: globalSettings.transparent_color
                  anchors.verticalCenter: parent.verticalCenter
                  anchors.left: grid_text_rect.right
                  anchors.leftMargin: 10
                  anchors.rightMargin: 10
                  anchors.right: parent.right
                  height: parent.height/5*3
                  width: parent.width/6*5


                  Row{
                    id:grid_row
                    spacing: (grid_rect01.width-split1.width*6)/5

                             Image {
                                 id: split1
                                 source: "qrc:/images/group/split1.png"
                                 width: grid_rect01.height
                                 height:width
                                 MouseArea{
                                    anchors.fill: parent
                                    onClicked: {
                                        $manager.logical_wall_column = 1;
                                        $manager.logical_wall_row = 1;
//                                        $manager.save_conf();

                                    }

                                 }
                             }

                             Image {
                                 id: split4
                                 source: "qrc:/images/group/split4.png"

                                 width: grid_rect01.height
                                 height:width
                                 MouseArea{
                                    anchors.fill: parent
                                    onClicked: {
                                        $manager.logical_wall_column =2;
                                        $manager.logical_wall_row = 2;
//                                        console.log("2*2")
//                                        $manager.save_conf();

                                    }

                                 }
                             }
                             Image {
                                 id: split9
                                 source: "qrc:/images/group/split9.png"

                                 width: grid_rect01.height
                                 height:width
                                 MouseArea{
                                    anchors.fill: parent
                                    onClicked: {
                                        $manager.logical_wall_column =3;
                                        $manager.logical_wall_row = 3;
//                                        $manager.save_conf();

                                    }

                                 }
                             }
                             Image {
                                 id: split16
                                 source: "qrc:/images/group/split16.png"

                                 width: grid_rect01.height
                                 height:width
                                 MouseArea{
                                    anchors.fill: parent
                                    onClicked: {
                                        $manager.logical_wall_column =4;
                                        $manager.logical_wall_row = 4;
//                                        $manager.save_conf();

                                    }

                                 }
                             }
                             Image {
                                 id: split25
                                 source: "qrc:/images/group/split25.png"

                                 width: grid_rect01.height
                                 height:width

                                 MouseArea{
                                    anchors.fill: parent
                                    onClicked: {
                                        $manager.logical_wall_column =5;
                                        $manager.logical_wall_row = 5;
                                        $manager.save_conf();

                                    }

                                 }
                             }

                             Image {
                                 id: settingAction
                                 width: grid_rect01.height
                                 height:width
                                 state: "NORMAL"

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
                                         //弹出界面

                                         showSetting(); //发送信号
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


       }

       Rectangle{
          height: parent.height/4*3
          width: parent.width
          color: globalSettings.transparent_color

          Row{
            spacing: 0
            anchors.fill: parent
            Rectangle{   //电源控制
               width: parent.width/3
               height: parent.height
               color:globalSettings.transparent_color
               border.color:"grey"
               border.width: 1
               clip: true
               Column{
                 anchors.fill: parent

                 Rectangle{
                     height: parent.height/5
                     width: parent.width
                     color: "#7BC9B1"
                     border.color: "grey"
                     border.width: 1
                     Text {

                         text: $manager.iszhongwen?qsTr("窗口控制"):qsTr("Window Control")

                         anchors.centerIn: parent
                         font.bold: true
                         color: "black"
                         font.pixelSize: parent.height/3*2
                     }

                 }
                 Rectangle{
                    id:power_and_screen
                    color:globalSettings.transparent_color
                    height: parent.height/5*4
                    width: parent.width

                    Column{
                       visible: false
                       anchors.fill: parent
                       Row{
                         visible: false
                         width: parent.width
                         height: parent.height/2

                         Rectangle{
                             height: parent.height
                             width: parent.width/2
                             color: globalSettings.transparent_color
   //                          border.color: "white"
   //                          border.width: 1
                             Image {
                                 anchors.verticalCenter: parent.verticalCenter
                                 anchors.horizontalCenter: parent.horizontalCenter
                                 anchors.horizontalCenterOffset: -20
                                 id: power_img
                                 height: parent.height-10
                                 width: height
                                 source: root_device_all.powerisclick==1?"qrc:/images/all_ctl/powerpress.png":"qrc:/images/all_ctl/power.png"
                                 MouseArea{
                                 id:power_btn
                                 anchors.fill: parent
                                 onClicked: {

                                     if(root_device_all.powerisclick==1){

                                     }
                                     else if($manager.get_num_of_device(5)>0){
//                                        root_device_all.powerisclick=1
//                                        $manager.control_all_power_device(1)
//                                        close_power_mode.isOpen=true
//                                        close_power_mode.visible=true

                                         msgSend($manager.iszhongwen?qsTr("确认开启电源？"):qsTr("Open Power?"),1)

                                    }
                                    else
                                        msgTip();



                                 }
                                 }
                             }

                             Text {
                                 anchors.verticalCenter: power_img.verticalCenter
                                 anchors.left: power_img.right
                                 anchors.leftMargin: 5
                                 color: "white"
                                 font.bold: true
                                 font.pixelSize: parent.height/4
                                 text: $manager.iszhongwen?qsTr("电源开"):qsTr("Open Power")
                             }

                         }
                         Rectangle{
                             height: parent.height
                             width: parent.width/2
                             color: globalSettings.transparent_color
   //                          border.color: "white"
   //                          border.width: 1
                             Image {
                                 anchors.verticalCenter: parent.verticalCenter
                                 anchors.horizontalCenter: parent.horizontalCenter
                                 anchors.horizontalCenterOffset: -20
                                 id: colse_img
                                 height: parent.height-10
                                 width: height
                                 source: root_device_all.powerisclick==2?"qrc:/images/all_ctl/powerpress.png":"qrc:/images/all_ctl/power.png"
                                 MouseArea{
                                 id:colse_btn
                                 anchors.fill: parent
                                 onClicked: {

                                     if(root_device_all.powerisclick==2){

                                     }
                                     else if($manager.get_num_of_device(5)>0)
                                     {
//                                         root_device_all.powerisclick=2
//                                         $manager.control_all_power_device(0)
//                                         close_power_mode.isOpen=false
//                                         close_power_mode.visible=true
                                         msgSend($manager.iszhongwen?qsTr("确认关闭电源？"):qsTr("Close Power?"),2)


                                      }

                                     else
                                         msgTip();
                                 }

                                 }

                             }

                             Text {
                                 anchors.verticalCenter: colse_img.verticalCenter
                                 anchors.left: colse_img.right
                                 anchors.leftMargin: 5
                                 color: "white"
                                 font.bold: true
                                 font.pixelSize: parent.height/4
                                 text: $manager.iszhongwen?qsTr("电源关"):qsTr("Close Power")
                             }

                         }

                       }

                       Row{
                           visible: false
                           width: parent.width
                           height: parent.height/2

                           Rectangle{
                               height: parent.height
                               width: parent.width/2
                               color: globalSettings.transparent_color
                               Image {
                                   anchors.verticalCenter: parent.verticalCenter
                                   anchors.horizontalCenter: parent.horizontalCenter
                                   anchors.horizontalCenterOffset: -20
                                   id: screen_img
                                   height: parent.height-10
                                   width: height
                                   source: root_device_all.screenisclick==1?"qrc:/images/all_ctl/screenpress.png":"qrc:/images/all_ctl/screen.png"
                                   MouseArea{
                                   id:screen_btn
                                   anchors.fill: parent
                                   onClicked: {
                                      if(root_device_all.screenisclick==1){
                                      }
                                      else if($manager.get_num_of_device(4)>0){
//                                          close_screen_mode.isOpen=true
//                                          close_screen_mode.visible=true
                                          msgSend($manager.iszhongwen?qsTr("确认开启屏幕？"):qsTr("Open Screen?"),3)

                                      }
                                      else
                                          msgTip();

                                   }
                                   }
                               }

                               Text {
                                   anchors.verticalCenter: screen_img.verticalCenter
                                   anchors.left: screen_img.right
                                   anchors.leftMargin: 5
                                   color: "white"
                                   font.bold: true
                                   font.pixelSize: parent.height/4
                                   text: $manager.iszhongwen?qsTr("屏幕开"):qsTr("Open Screen")
                               }

                           }
                           Rectangle{
                               height: parent.height
                               width: parent.width/2
                               color: globalSettings.transparent_color
     //                          border.color: "white"
     //                          border.width: 1
                               Image {
                                   anchors.verticalCenter: parent.verticalCenter
                                   anchors.horizontalCenter: parent.horizontalCenter
                                   anchors.horizontalCenterOffset: -20
                                   id: screen_colse_img
                                   height: parent.height-10
                                   width: height
                                   source: root_device_all.screenisclick==2?"qrc:/images/all_ctl/screenpress.png":"qrc:/images/all_ctl/screen.png"
                                   MouseArea{
                                   id:screen_colse_btn
                                   anchors.fill: parent
                                   onClicked: {

                                       if(root_device_all.screenisclick==2){
                                       }
                                       else if($manager.get_num_of_device(4)>0)
                                       {
//                                           close_screen_mode.isOpen=false
//                                           close_screen_mode.visible=true
                                           msgSend($manager.iszhongwen?qsTr("确认关闭屏幕？"):qsTr("Close Screen?"),4)

                                        }

                                       else
                                           msgTip();
                                   }

                                   }

                               }

                               Text {
                                   anchors.verticalCenter: screen_colse_img.verticalCenter
                                   anchors.left: screen_colse_img.right
                                   anchors.leftMargin: 5
                                   color: "white"
                                   font.bold: true
                                   font.pixelSize: parent.height/4
                                   text: $manager.iszhongwen?qsTr("屏幕关"):qsTr("Close Screen")
                               }

                           }

                         }
                    }

                    Row{
                       anchors.centerIn: parent
                       spacing: fresh_setting.width
                       Image {
                           id: fresh_setting
                           source:  fresh_mousearea.pressed?"qrc:/images/all_ctl/ID10_press.png":"qrc:/images/all_ctl/ID9.png"
                           width:power_and_screen.width/4
                           height:power_and_screen.height/3

                           Text {
                               anchors.centerIn: parent
                               color: "white"
                               text: $manager.iszhongwen?qsTr("刷新窗口"):qsTr("Refresh Window")
                               font.pixelSize: parent.height/3
                               font.bold: true
                           }
                           MouseArea{
                              id:fresh_mousearea
                              anchors.fill: parent
                              onClicked: {
                               $manager.fresh_all_signal_in_mode();

                              }

                           }
                       }
                       Image {
                           id: empty_setting
                           source:  empty_mousearea.pressed?"qrc:/images/all_ctl/ID10_press.png":"qrc:/images/all_ctl/ID9.png"
                           width:power_and_screen.width/4
                           height:power_and_screen.height/3
                           Text {
                               anchors.centerIn: parent
                               color: "white"
                               text: $manager.iszhongwen?qsTr("清空窗口"):qsTr("Clear Window")
                               font.pixelSize: parent.height/3
                               font.bold: true
                           }
                           MouseArea{
                              id: empty_mousearea
                              anchors.fill: parent
                              onClicked: {
                               $manager.delete_all_signal_in_mode()

                              }

                           }
                       }

                    }

                 }



               }

            }
            Rectangle{   //云台控制
               width: parent.width/3
               height: parent.height
               color:globalSettings.transparent_color
               border.color: "grey"
               border.width: 1
               Column{
                 anchors.fill: parent
                 Rectangle{  //标题
                     height: parent.height/5
                     width: parent.width
                     color: "#7BC9B1"
                     border.color: "grey"
                     border.width: 1
                     Text {

                         text: $manager.iszhongwen?qsTr("云台控制"):qsTr("PTZ Control")
                         anchors.centerIn: parent
                         font.bold: true
                         color: "black"
                         font.pixelSize: parent.height/3*2
                     }

                 }
                 Rectangle{
                     height: parent.height/5*4
                     width: parent.width
                     color:globalSettings.transparent_color
                     Rectangle{
                         anchors.left: parent.left
                         anchors.top: parent.top
                         anchors.bottom: parent.bottom
                         anchors.right: add_img.left
                         color:globalSettings.transparent_color

                         Rectangle{   //中心圆点
                            id:center_rect
                            height: parent.height/3
                            width: height
                            color: "grey"
                            anchors.centerIn: parent
                            radius: width/2

                         }
                         Image {
                             width: center_rect.width*1.5
                             height: parent.height/3
                             anchors.horizontalCenter: center_rect.horizontalCenter
                             anchors.bottom: center_rect.top
                             source: topbtn.pressed?"qrc:/images/all_ctl/toppress.png":"qrc:/images/all_ctl/top.png"
                             Text {
                                 anchors.centerIn: parent
                                 font.pixelSize:parent.height/4
                                 color: "white"
                                 text: $manager.iszhongwen?qsTr("上"):qsTr("UP")
                             }
                             MouseArea{
                                 id:topbtn
                               anchors.fill: parent
                               onPressed: {
                                   $manager.ptz_control(1,0)

                               }
                               onReleased: {
                                   $manager.ptz_control(1,1)

                               }
                             }
                         }
                         Image {
                             width: center_rect.width*1.5
                             height: parent.height/3
                             anchors.horizontalCenter: center_rect.horizontalCenter
                             anchors.top: center_rect.bottom
                             source: bottombtn.pressed?"qrc:/images/all_ctl/bottompress.png":"qrc:/images/all_ctl/bottom.png"
                             Text {
                                 anchors.centerIn: parent
                                 font.pixelSize:parent.height/4
                                 color: "white"
                                 text: $manager.iszhongwen?qsTr("下"):qsTr("DOWN")
                             }
                             MouseArea{
                                 id:bottombtn
                               anchors.fill: parent
                               onPressed: {
                                   $manager.ptz_control(2,0)

                               }
                               onReleased: {
                                   $manager.ptz_control(2,1)

                               }
                             }
                         }

                         Image {
                             width: parent.height/3
                             height:center_rect.width*1.5
                             anchors.verticalCenter:  center_rect.verticalCenter
                             anchors.right: center_rect.left
                             source: leftbtn.pressed?"qrc:/images/all_ctl/leftpress.png":"qrc:/images/all_ctl/left.png"
                             Text {
                                 anchors.centerIn: parent
                                 font.pixelSize:parent.width/4
                                 color: "white"
                                 text: $manager.iszhongwen?qsTr("左"):qsTr("LEFT")
                             }
                             MouseArea{
                               id:leftbtn
                               anchors.fill: parent
                               onPressed: {
                                   $manager.ptz_control(3,0)

                               }
                               onReleased: {
                                   $manager.ptz_control(3,1)

                               }
                             }
                         }
                         Image {
                             width: parent.height/3
                             height:center_rect.width*1.5
                             anchors.verticalCenter:  center_rect.verticalCenter
                             anchors.left: center_rect.right
                             source: rightbtn.pressed?"qrc:/images/all_ctl/rightpress.png":"qrc:/images/all_ctl/right.png"
                             Text {
                                 anchors.centerIn: parent
                                 font.pixelSize:parent.width/4
                                 color: "white"
                                 text: $manager.iszhongwen?qsTr("右"):qsTr("RIGHT")
                             }
                             MouseArea{
                               id:rightbtn
                               anchors.fill: parent
                               onPressed: {
                                   $manager.ptz_control(4,0)

                               }
                               onReleased: {
                                   $manager.ptz_control(4,1)

                               }
                             }
                         }



                     }
                     Text {
                         id: add_text
                         text: $manager.iszhongwen?qsTr("变焦+"):qsTr("Zoom+")
                         color: "white"
                         font.pixelSize: 12
                         anchors.right: parent.right
                         anchors.top: parent.top
                         anchors.topMargin: 20
                         anchors.rightMargin: 10
                     }

                     Text {
                         id: dec_text
                         text: $manager.iszhongwen?qsTr("变焦-"):qsTr("Zoom-")
                         color: "white"
                         font.pixelSize: 12
                         anchors.right: parent.right
                         anchors.bottom: parent.bottom
                         anchors.bottomMargin: 20
                         anchors.rightMargin: 10
                     }
                     Image{
                         id: add_img
                         anchors.right: add_text.left
                         anchors.rightMargin: 10
                         anchors.verticalCenter: add_text.verticalCenter
                         source: add_mousearea.pressed?"qrc:/images/all_ctl/addpress.png":"qrc:/images/all_ctl/add.png"
                         width:parent.height/3
                         height:width
                         MouseArea{
                             id:add_mousearea
                             anchors.fill: parent
                             onPressed: {

                                 $manager.ptz_control(5,0)
                             }
                             onReleased: {

                                 $manager.ptz_control(5,1)
                             }
                         }


                     }

                     Image{
                         id: dec_img
                         anchors.right: dec_text.left
                         anchors.rightMargin: 10
                         anchors.verticalCenter: dec_text.verticalCenter
                         source: dec_mousearea.pressed?"qrc:/images/all_ctl/decpress.png":"qrc:/images/all_ctl/dec.png"
                         width:parent.height/3
                         height:width
                         MouseArea{
                             id:dec_mousearea
                             anchors.fill: parent
                             onPressed: {

                                 $manager.ptz_control(6,0)
                             }
                             onReleased: {

                                 $manager.ptz_control(6,1)
                             }
                         }


                     }

                 }
                 }

            }
            Rectangle{ //字幕底图
               width: parent.width/3
               height: parent.height
               color: globalSettings.transparent_color
               border.color: "grey"
               border.width: 1
               Column{
                 anchors.fill: parent
                 Rectangle{
                     height: parent.height/5
                     width: parent.width
                     color: "#7BC9B1"
                     border.color: "grey"
                     border.width: 1
                     Text {

                         text: $manager.iszhongwen?qsTr("字幕底图切换"):qsTr("Subtitle Basemap Switching")
                         anchors.centerIn: parent
                         font.bold: true
                         color: "black"
                         font.pixelSize: parent.height/3*2
                     }

                 }
                 GridLayout {
                    height: parent.height/5*4
                    width: parent.width
                    rows: 2
                    columns: 2

                    Image {
                        id: base_img_setting
                        source: base_img_mousearea.pressed?"qrc:/images/all_ctl/ID10_press.png":"qrc:/images/all_ctl/ID9.png"
                        Layout.minimumWidth:parent.width/4
                        Layout.minimumHeight:parent.height/3
                        Layout.maximumWidth:parent.width/4
                        Layout.maximumHeight:parent.height/3
                        Layout.alignment:Qt.AlignVCenter|Qt.AlignHCenter
                        Text {
                            anchors.centerIn: parent
                            color: "white"
                            text: $manager.iszhongwen?qsTr("底图设置"):qsTr("Set Basemap")
                            font.pixelSize: parent.height/3
                            font.bold: true
                        }
                        MouseArea{
                           id:base_img_mousearea
                           anchors.fill: parent
                           onClicked: {
                            $manager.open_basemap_setting()

                           }

                        }

                    }
                    Image {
                        id: curtain_setting
                        source: curtain_mousearea.pressed?"qrc:/images/all_ctl/ID10_press.png":"qrc:/images/all_ctl/ID9.png"
                        Layout.minimumWidth:parent.width/4
                        Layout.minimumHeight:parent.height/3
                        Layout.maximumWidth:parent.width/4
                        Layout.maximumHeight:parent.height/3
                        Layout.alignment:Qt.AlignVCenter|Qt.AlignHCenter
                        Text {
                            anchors.centerIn: parent
                            color: "white"
                            text: $manager.iszhongwen?qsTr("字幕设置"):qsTr("Set Subtitle")
                            font.pixelSize: parent.height/3
                            font.bold: true
                        }
                        MouseArea{
                           id:curtain_mousearea
                           anchors.fill: parent
                           onClicked: {
                            $manager.open_caption_setting();

                           }

                        }
                    }
//                    Image {
//                        id: fresh_setting
//                        visible: false
//                        source:  fresh_mousearea.pressed?"qrc:/images/all_ctl/ID10_press.png":"qrc:/images/all_ctl/ID9.png"
//                        Layout.minimumWidth:parent.width/4
//                        Layout.minimumHeight:parent.height/3
//                        Layout.maximumWidth:parent.width/4
//                        Layout.maximumHeight:parent.height/3
//                        Layout.alignment:Qt.AlignVCenter|Qt.AlignHCenter
//                        Text {
//                            anchors.centerIn: parent
//                            color: "white"
//                            text: $manager.iszhongwen?qsTr("刷新窗口"):qsTr("Refresh Window")
//                            font.pixelSize: parent.height/3
//                            font.bold: true
//                        }
//                        MouseArea{
//                           id:fresh_mousearea
//                           anchors.fill: parent
//                           onClicked: {
//                            $manager.fresh_all_signal_in_mode();

//                           }

//                        }
//                    }
//                    Image {
//                        id: empty_setting
//                        visible: false
//                        source:  empty_mousearea.pressed?"qrc:/images/all_ctl/ID10_press.png":"qrc:/images/all_ctl/ID9.png"
//                        Layout.minimumWidth:parent.width/4
//                        Layout.minimumHeight:parent.height/3
//                        Layout.maximumWidth:parent.width/4
//                        Layout.maximumHeight:parent.height/3
//                        Layout.alignment:Qt.AlignVCenter|Qt.AlignHCenter
//                        Text {
//                            anchors.centerIn: parent
//                            color: "white"
//                            text: $manager.iszhongwen?qsTr("清空窗口"):qsTr("Clear Window")
//                            font.pixelSize: parent.height/3
//                            font.bold: true
//                        }
//                        MouseArea{
//                           id: empty_mousearea
//                           anchors.fill: parent
//                           onClicked: {
//                            $manager.delete_all_signal_in_mode()

//                           }

//                        }
//                    }


                 }
               }


            }


          }

       }


    }

//    Row {
//          visible: false
//          spacing: 2
//          anchors.fill: parent
//          Rectangle {   //屏幕和电源

//             color: globalSettings.transparent_color
//              width: parent.width/3;
//              height: parent.height
//              Rectangle {

//                  color: globalSettings.transparent_color
//                  anchors.centerIn: parent
//                  width: parent.width/1.3;
//                  height: parent.height

//                  property color btnColor: "#009688"

//                  Rectangle {  //屏幕开关
//                      id:rect_screen_open
//                      width:rect_screen_close.width
//                      height: 40
//                      anchors.left: parent.left
//                      anchors.top: parent.top
//                      anchors.topMargin: topmargin
//                      color: "#292A2E"
//                      radius: 20
//                      Text {
//                          id: text_screen_open
//                          anchors.centerIn: parent
//                          text: qsTr("屏幕开")
//                          color: "white"
//                      }
//                      MouseArea {
//                          id: btn_screen_open
//                          anchors.fill: parent
//                          onPressed: {
//                            shadow_screen_open.visible=false
//                            $manager.control_all_screen_device(1) //打开所有的屏幕， 1代表开启 0代表关闭

//                          }

//                          onReleased: {
//                            shadow_screen_open.visible=true
//                          }
//                      }

//                  }
//                  DropShadow {
//                          id:shadow_screen_open
//                         anchors.fill: rect_screen_open
//                         horizontalOffset: 3
//                         verticalOffset: 3
//                         radius: 8.0
//                         samples: 16
//                         color: "#80000000"
//                         source: rect_screen_open
//                     }
//                  Rectangle {  //屏幕开关
//                      id:rect_screen_close
//                      width:text_screen_close.width+20
//                      height: 40
//                      anchors.right: parent.right
//                      anchors.top: parent.top
//                      anchors.topMargin: topmargin
//                      color: "#292A2E"
//                      radius: 20

//                      Text {
//                          id: text_screen_close
//                          anchors.centerIn: parent
//                          text: qsTr("屏幕关")
//                          color: "white"
//                      }
//                      MouseArea {
//                          id: btn_screen_close
//                          anchors.fill: parent
//                          onPressed: {
//                            shadow_screen_close.visible=false
//                            $manager.control_all_screen_device(0) //打开所有的屏幕， 1代表开启 0代表关闭

//                          }

//                          onReleased: {
//                            shadow_screen_close.visible=true
//                          }
//                      }
//                  }
//                  DropShadow {
//                          id:shadow_screen_close
//                         anchors.fill: rect_screen_close
//                         horizontalOffset: 3
//                         verticalOffset: 3
//                         radius: 8.0
//                         samples: 16
//                         color: "#80000000"
//                         source: rect_screen_close
//                     }
//                  Rectangle {  //电源开
//                      id:rect_power_open
//                      width:rect_screen_close.width
//                      height: 40
//                      anchors.left: parent.left
//                      anchors.bottom: parent.bottom
//                      anchors.bottomMargin: topmargin
//                      color: "#292A2E"
//                      radius: 20
//                      Text {
//                          id: text_power_open
//                          anchors.centerIn: parent
//                          text: qsTr("电源开")
//                          color:"white"
//                      }
//                      MouseArea {
//                          id: btn_power_open
//                          anchors.fill: parent
//                          onPressed: {
//                            shadow_power_open.visible=false
//                            $manager.control_all_power_device(1) //打开所有的屏幕， 1代表开启 0代表关闭

//                          }

//                          onReleased: {
//                            shadow_power_open.visible=true
//                          }

//                      }
//                  }
//                  DropShadow {
//                          id:shadow_power_open
//                         anchors.fill: rect_power_open
//                         horizontalOffset: 3
//                         verticalOffset: 3
//                         radius: 8.0
//                         samples: 16
//                         color: "#80000000"
//                         source: rect_power_open
//                     }
//                  Rectangle {  //电源关
//                      id:rect_power_close
//                      width:rect_screen_close.width
//                      height: 40
//                      anchors.right: parent.right
//                      anchors.bottom: parent.bottom
//                      anchors.bottomMargin: topmargin
//                      color: "#292A2E"
//                      radius: 20
//                      Text {
//                          id: text_power_close
//                          anchors.centerIn: parent
//                          text: qsTr("电源关")
//                          color: "white"
//                      }
//                      MouseArea {
//                          id: btn_power_close
//                          anchors.fill: parent
//                          onPressed: {
//                            shadow_power_close.visible=false
//                            $manager.control_all_power_device(0) //关闭所有的电源， 1代表开启 0代表关闭

//                          }

//                          onReleased: {
//                            shadow_power_close.visible=true
//                          }
//                      }
//                  }
//                  DropShadow {
//                          id:shadow_power_close
//                         anchors.fill: rect_power_close
//                         horizontalOffset: 3
//                         verticalOffset: 3
//                         radius: 4
//                         samples: 16
//                         color: "#80000000"
//                         source: rect_power_close
//                     }




//                  }
//              Component.onCompleted: {
//              //没有设备，则无法点击按钮
//              var screen_num=$manager.get_num_of_device(4)
//              var power_num=$manager.get_num_of_device(5)
////              console.log(screen_num);
//              if(screen_num<=0)
//              {
//                  btn_screen_close.enabled=false
//                  btn_screen_open.enabled=false
//              }
//              if(power_num<=0)
//              {
//                  btn_power_close.enabled=false
//                  btn_power_open.enabled=false
//              }



//              }
//              }



//          Rectangle {   //云台控制

//             color: globalSettings.transparent_color
//              width: parent.width/3
//                              border.color:globalSettings.border_color
//                              border.width: 2
//              height: parent.height
//              Rectangle {

//                  color: globalSettings.transparent_color
//                  anchors.centerIn: parent
//                  width: parent.width/1.3;
//                  height: parent.height

//                  Text {
//                      id: header_text
//                      anchors.horizontalCenter:  parent.horizontalCenter

//                      anchors.left: parent.left
//                      font.bold: true
//                      font.pointSize: 10
//                      color:"white"
//                      enabled: false
//                      text: qsTr("云台控制")
//                  }
//                  Rectangle{
//                     width: 60
//                     height: width
//                     id:centel_temp
//                     color: globalSettings.transparent_color
//                     radius: height/2
//                     anchors.centerIn: parent
//                     Image {
//                         id: name
//                         source: "qrc:/images/all_ctl/center.png"
//                         anchors.fill: parent
//                     }

//                  }
//                  Image {  //向上
//                      id: image_top
//                      anchors.horizontalCenter:  parent.horizontalCenter
//                      anchors.bottom: centel_temp.top
//                      anchors.bottomMargin: 10
//                      width: 70
//                      height: 50
//                      source: top_flag?"qrc:/images/all_ctl/btnup.png":"qrc:/images/all_ctl/btnup_normal.png"
////                      sourceSize: Qt.size(80, 50)
////                      smooth: true

//////                      visible: false
////                      source: btn_image_top.pressed?"qrc:/images/all_ctl/btnup.png":"qrc:/images/all_ctl/btnup_normal.png"
//                      MouseArea{
//                          id:btn_image_top
//                          anchors.fill: parent
//                          onPressed: {
////                              console.log(1)
//                              top_flag=true
//                              $manager.ptz_control(1,0)   //开始
//                          }
//                          onReleased: {
//                              top_flag=false

//                              $manager.ptz_control(1,1)   //停止
//                          }
//                      }
//                  }
////                  DropShadow {
////                         id:shadow_image_top
////                         anchors.fill: image_top
////                         horizontalOffset: 3
////                         verticalOffset: 3
////                         radius: 8.0
////                         samples: 16
////                         color: "#80000000"
////                         source: image_top
////                     }
////                  DropShadow {
////                         id:shadow_image_top2
////                         anchors.fill: image_top
////                         horizontalOffset: -3
//////                         verticalOffset: -3

////                         radius: 8.0
////                         samples: 16
////                         color: "#80000000"
////                         source: image_top
////                     }
//                  Image {  //向下
//                      id: image_bottom
//                      anchors.horizontalCenter:  parent.horizontalCenter
//                      anchors.top: centel_temp.bottom
//                      anchors.topMargin:10
//                      width: 70
//                      height: 50
//                      source: bottom_flag?"qrc:/images/all_ctl/btndown.png":"qrc:/images/all_ctl/btndown_normal.png"
//                      MouseArea{
//                          anchors.fill: parent
//                          onPressed: {
//                             bottom_flag=true
//                              $manager.ptz_control(2,0)
//                          }
//                          onReleased: {
//                             bottom_flag=false
//                              $manager.ptz_control(2,1)
//                          }
//                      }
//                  }
//                  Image {  //向左
//                      id: image_left
//                      anchors.verticalCenter:   parent.verticalCenter
//                      anchors.right: centel_temp.left
//                      anchors.rightMargin:20
//                      width:50
//                      height:70
//                      source: left_flag?"qrc:/images/all_ctl/btnleft.png":"qrc:/images/all_ctl/btnleft_normal.png"
//                      MouseArea{
//                          anchors.fill: parent
//                          onPressed: {
//                             left_flag=true
//                              $manager.ptz_control(3,0)
//                          }
//                          onReleased: {
//                             left_flag=false
//                              $manager.ptz_control(3,1)
//                          }
//                      }
//                  }
//                  Image {  //向右
//                      id: image_right
//                      anchors.verticalCenter:   parent.verticalCenter
//                      anchors.left: centel_temp.right
//                      anchors.leftMargin: 20
//                      width: 50
//                      height: 70
//                      source: right_flag?"qrc:/images/all_ctl/btnright.png":"qrc:/images/all_ctl/btnright_normal.png"
//                      MouseArea{
//                          anchors.fill: parent
//                          onPressed: {
//                             right_flag=true
//                              $manager.ptz_control(4,0)
//                          }
//                          onReleased: {
//                             right_flag=false
//                              $manager.ptz_control(4,1)
//                          }
//                      }
//                  }
//                  Image {  //缩小
//                      id: image_in
//                      anchors.left: parent.left
//                      anchors.leftMargin: topmargin
//                      anchors.top: image_left.bottom

//                      width: 50
//                      height: 50
//                      source: decrease_flag?"qrc:/images/all_ctl/in-active.png":"qrc:/images/all_ctl/in-idle.png"
//                      MouseArea{
//                          anchors.fill: parent
//                          onPressed: {
//                             decrease_flag=true
//                              $manager.ptz_control(6,0)
//                          }
//                          onReleased: {
//                             decrease_flag=false
//                              $manager.ptz_control(6,1)
//                          }
//                      }
//                  }
//                  Image {  //放大
//                      id: image_out
//                      anchors.right: parent.right
//                      anchors.rightMargin: topmargin
//                      anchors.top: image_right.bottom

//                      width: 50
//                      height: 50
//                      source: increase_flag?"qrc:/images/all_ctl/out-active.png":"qrc:/images/all_ctl/out-idle.png"
//                      MouseArea{
//                          anchors.fill: parent
//                          onPressed: {
//                             increase_flag=true
//                              $manager.ptz_control(5,0)
//                          }
//                          onReleased: {
//                             increase_flag=false
//                              $manager.ptz_control(5,1)
//                          }
//                      }
//                  }

//                  }

//              }

//          Rectangle {   //字幕和底图和循环

//             color: globalSettings.transparent_color
//              width: parent.width/3;
//              height: parent.height
//              Rectangle {

//                  color: globalSettings.transparent_color
//                  anchors.centerIn: parent
//                  width: parent.width/1.3;
//                  height: parent.height

//                  property color btnColor: "#009688"

//                  Rectangle {  //字幕设置
//                      id:rect_caption

//                      width:text_caption.width+20
//                      height: 40
//                      anchors.left: parent.left
//                      anchors.top: parent.top
//                      anchors.topMargin: topmargin
//                      color: "#292A2E"
//                      radius: 20
//                      Text {
//                          id: text_caption
//                          anchors.centerIn: parent
//                          text: qsTr("字幕设置")
//                          color: "white"
//                      }
//                      MouseArea {
//                          id: btn_caption
//                          anchors.fill: parent
//                          onPressed: {
//                           shadow_rect_caption.visible=false
//                           $manager.open_caption_setting();
//                          }

//                          onReleased: {
//                            shadow_rect_caption.visible=true
//                          }

//                      }
//                  }
//                  DropShadow {
//                          id:shadow_rect_caption
//                         anchors.fill: rect_caption
//                         horizontalOffset: 3
//                         verticalOffset: 3
//                         radius: 8.0
//                         samples: 16
//                         color: "#80000000"
//                         source: rect_caption
//                     }

//                  Rectangle {  // 底图设置
//                      id:rect_bsemap
//                      width:rect_caption.width
//                      height: 40
//                      anchors.right: parent.right
//                      anchors.top: parent.top
//                      anchors.topMargin: topmargin
//                      color: "#292A2E"
//                      radius: 20
//                      Text {
//                          id: text_bsemap
//                          anchors.centerIn: parent
//                          text: qsTr("底图设置")
//                          color: "white"
//                      }
//                      MouseArea {
//                          id: btn_bsemap
//                          anchors.fill: parent
//                          onPressed: {
//                           shadow_rect_bsemap.visible=false
//                            $manager.open_basemap_setting()
//                          }

//                          onReleased: {
//                            shadow_rect_bsemap.visible=true
//                          }

//                      }
//                  }
//                  DropShadow {
//                          id:shadow_rect_bsemap
//                         anchors.fill: rect_bsemap
//                         horizontalOffset: 3
//                         verticalOffset: 3
//                         radius: 8.0
//                         samples: 16
//                         color: "#80000000"
//                         source: rect_bsemap
//                     }

////                  Rectangle {  //循环开
////                      id:rect_cycle_open
////                      width:parent.width/3
////                      height: 40
////                      anchors.left: parent.left
////                      anchors.bottom: parent.bottom
////                      anchors.bottomMargin: topmargin
////                      color: cycle_open_flag?globalSettings.selected_color:globalSettings.transparent_color
////                      border.color:cycle_open_flag?globalSettings.transparent_color:globalSettings.border_color
////                      border.width: cycle_open_flag?0:2
////                      radius: 20
//////                      color: btnArea.pressed ? "#00bef0":globalSettings.font_color
////                      Text {
////                          id: text_cycle_open
////                          anchors.centerIn: parent
////                          text: qsTr("循环开启")
////                          color:"white"
////                      }
////                      MouseArea {
////                          id: btn_cycle_open
////                          anchors.fill: parent

////                          onClicked: {
////                              if(!cycle_open_flag){

////                                  cycle_open_flag=true
////                                  cycle_close_flag=false

////                              }



////                          }
////                      }
////                  }

////                  Rectangle {  //循环停止
////                      id:rect_cycle_close
////                      width:parent.width/3
////                      height: 40
////                      anchors.right: parent.right
////                      anchors.bottom: parent.bottom
////                      anchors.bottomMargin: topmargin
////                      color: cycle_close_flag?globalSettings.selected_color:globalSettings.transparent_color
////                      border.color:cycle_close_flag?globalSettings.transparent_color:globalSettings.border_color
////                      border.width: cycle_close_flag?0:2
////                      radius: 20
//////                      color: btnArea.pressed ? "#00bef0":globalSettings.font_color
////                      Text {
////                          id: text_cycle_close
////                          anchors.centerIn: parent
////                          text: qsTr("循环停止")
////                          color: "white"
////                      }
////                      MouseArea {
////                          id: btn_cycle_close
////                          anchors.fill: parent

////                          onClicked: {

////                              if(!cycle_close_flag){
////                                  cycle_open_flag=false
////                                  cycle_close_flag=true

////                              }



////                          }
////                      }
////                  }




//                  }

//              }

//      }

}

