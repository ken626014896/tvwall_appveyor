import QtQuick 2.9
import QtQuick.Controls 2.3
import QtGraphicalEffects 1.12

import QtQuick.Controls 1.4 as Controls
import Qt.labs.calendar 1.0
Rectangle {
    id: rect_device_ctrl
    color: globalSettings.transparent_color
    anchors.fill: parent
    property string selected_color: "#00c1ef"
    property string font_color: "#a9b3bb"
    property string selected_font_color: "#00c1ef"
    ListView{
        id:device_ctrl
        anchors.fill: parent
        anchors.leftMargin: 10
        anchors.rightMargin: 5
        focus: false
        orientation: ListView.Horizontal
        boundsBehavior: Flickable.StopAtBounds
        snapMode: ListView.SnapOneItem
        highlightRangeMode: ListView.StrictlyEnforceRange
        highlightMoveDuration: 250
        model: $device_ctrl
        clip: true
        spacing: 50


        delegate: Rectangle {
            id: rect_device_ctrl_item
            property int device_ctrl_remote_panel_width: 26
            property int device_ctrl_remote_panel_height: 70
            property int current_operator: 0
            property int current_operator_bottom: 0
            property int parent_index: index

            property bool sync_msg: false
            width: dmDeviceType != 7?(dmDeviceType === 6? (rect_device_ctrl.width > 650?rect_device_ctrl.width:750) : 220):650
            height: parent.height
            color: globalSettings.transparent_color
//            border.color: "white"
//            border.width: 1
            Component.onCompleted: {
//                console.log("comp");
                lmMusicButtons.clear();
                lmMusicButtons.append(dmMusicButtons)

                lmMusicSlides.clear();
                lmMusicSlides.append(dmMusicSlides);

                $manager.mute_status_changed_signal.connect(set_volume_mute);
                $manager.volume_value_signal.connect(set_volume_val);
//                $manager.volume_value_signal.connect(set_volume_val2);
            }

            ListModel{
                id:lmMusicButtons
            }
            ListModel{
                id:lmMusicSlides
            }

            function set_volume_mute(id, index, status){
                if(id !== dmVideoId){
                       console.log("no set_volume_mute", index, status);
                    return;
                }
                console.log("set_volume_mute", index, status);
                var item=lv_mute.contentItem.children[index]
                item.music_mule.select_mule = status;
            }

            function set_volume_val(id, index, val, y){
                if(id !== dmVideoId){
                    return;
                }
                rect_device_ctrl_item.sync_msg=true
                console.log("set_volume_val", index, val, y);
                var item=lv_mute.contentItem.children[index]
                lv_mute.contentItem.children[index].music_db = val;
                item.music_slide.y = item.music_slide_bkg.height-y*item.music_slide_bkg.height-item.music_slide.height/2;

                rect_device_ctrl_item.sync_msg=false
            }

            Rectangle {
                id: device_ctrl_item_header
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                height: 30
                visible: dmDeviceType !== 6
//                border.color:globalSettings.border_color
//                border.width: 2
                color: "transparent"
                Text {
                    id: device_ctrl_item_header_text
                    anchors.centerIn: parent
                    font.bold: true
                    font.pointSize: 12
                    color: font_color
                    enabled: false
                    text: dmName
                }
            }
            Rectangle {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: device_ctrl_item_header.bottom
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 2
                visible: dmVisualCtrl
                enabled: dmVisualCtrl
                color: "transparent"
                Row{
                    id: device_ctrl_row_center
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.bottom: rect_device_ctrl_air_bottom.top
                    spacing: 10
                    Image {
                        id: image_left
                        anchors.verticalCenter: parent.verticalCenter
                        width: 50
                        height: 50
                        source: dmVisualCtrlLeft?(rect_device_ctrl_item.current_operator===1?dmImageLeftActive:dmImageLeftIdle):""
                        MouseArea{
                            anchors.fill: parent
                            onClicked: {
                                if(dmVisualCtrlLeft){
                                    if(dmDeviceType === 2 ||
                                            dmDeviceType === 3 ||
                                            dmDeviceStatus){
                                        $manager.device_ctrl(index, "btnLeft");
                                        rect_device_ctrl_item.current_operator = 1;
                                    }
                                }
                            }
                        }
                    }
                    Image {
                        id: btn_center
                        width: dmVisualCtrlLeft?100:106
                        height: dmVisualCtrlLeft?100:45
                        visible: dmVisualCtrlCenterButton
                        enabled: dmVisualCtrlCenterButton
                        anchors.verticalCenter: parent.verticalCenter
                        source: dmVisualCtrlCenterButton?(dmDeviceStatus?dmImageCenterActive:dmImageCenterIdle):""
                        MouseArea{
                            anchors.fill: parent
                            onClicked: {

                                $device_ctrl.set_device_status(index);
    //                            rect_device_ctrl_item.current_operator = -1;
                                if(dmDeviceStatus){
                                    $manager.device_ctrl(index, "btnCenterActive");
                                }
                                else{
                                    $manager.device_ctrl(index, "btnCenterIdle");
                                }

                            }
                        }
                    }
                    Rectangle {
                        width: dmVisualCtrlCenter?100:0
                        height: dmVisualCtrlCenter?100:0
                        visible: dmVisualCtrlCenter
                        enabled: dmVisualCtrlCenter
                        anchors.verticalCenter: parent.verticalCenter
                        color: "transparent"
                        Canvas{
                            id: canvas_temperature
                            anchors.fill: parent
                            Text {
                                id: text_temperature_dispay
                                anchors.centerIn: parent
                                font.bold: true
                                font.pointSize: 38
                                color: dmDeviceStatus?selected_color:"gray"
                                text: dmCtrlProgress
                            }
                            onPaint: {
                                var startX = parent.width / 2;
                                var startY = parent.height / 2;
                                var stopX = parent.width / 2;
                                var stopY = parent.height / 2;
                                var ctx = getContext("2d")
                                ctx.lineWidth = 5
                                ctx.strokeStyle = dmDeviceStatus?selected_color:"gray"
                                ctx.beginPath()
                                ctx.arc(startX, startY, parent.height / 2 - 5, 0, 360, false)
                                ctx.closePath()
                                ctx.stroke()
                                //                            console.log("canvas onpaint")
                            }
                        }
                    }
                    Image {
                        id:image_right
                        anchors.verticalCenter: parent.verticalCenter
                        width: 50
                        height: 50
                        source: rect_device_ctrl_item.current_operator===2?dmImageRightActive:dmImageRightIdle
                        visible: dmVisualCtrlRight
                        enabled: dmVisualCtrlRight
                        MouseArea{
                            anchors.fill: parent
                            onClicked: {
                                if(device_ctrl_row_center.enabled){
                                    if(dmDeviceType === 2 ||
                                            dmDeviceType === 3 ||
                                            dmDeviceStatus){

                                        $manager.device_ctrl(index, "btnRight");
                                        rect_device_ctrl_item.current_operator = 2;
                                    }
                                }
                            }
                        }
                    }
                }

                Row{  //0,1 即空调和灯光的底部区域
                    id: rect_device_ctrl_air_bottom
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: dmVisualCtrlBottom?50:0
                    visible: dmVisualCtrlBottom
                    enabled: dmVisualCtrlBottom
                    spacing: 35

                    Image {
                        width: 50
                        height: 50
    //                    visible: dmVisualBottomLeft
                        source: dmVisualBottomLeft?(rect_device_ctrl_item.current_operator_bottom===3?dmImageBottomLeftActive:dmImageBottomLeftIdle):""
                        MouseArea{
                            anchors.fill: parent
                            onClicked: {
                                if(dmDeviceStatus){
                                    $manager.device_ctrl(index, "btnBottomLeft");
                                    rect_device_ctrl_item.current_operator_bottom = 3;
                                }
                            }
                        }
                    }
                    Image {
                        width: 50
                        height: 50
                        visible: dmVisualBottomCenter
                        source: dmDeviceStatus?dmImageBottomCenterActive:dmImageBottomCenterIdle
                        MouseArea{
                            anchors.fill: parent
                            onClicked: {
                                $manager.device_ctrl(index, dmDeviceStatus?"btnBottomCenterIdle":"btnBottomCenterActive");
                                $device_ctrl.set_device_status(index);
                                rect_device_ctrl_item.current_operator = -1;
                                rect_device_ctrl_item.current_operator_bottom = -1;
                                canvas_temperature.requestPaint(); //改变数值
                            }
                        }
                    }
                    Image {
                        width: 50
                        height: 50
                        visible: dmVisualBottomRight
                        source: rect_device_ctrl_item.current_operator_bottom===4?dmImageBottomRightActive:dmImageBottomRightIdle
                        MouseArea{
                            anchors.fill: parent
                            onClicked: {
                                if(dmDeviceStatus){
                                    $manager.device_ctrl(index, "btnBottomRight");
                                    rect_device_ctrl_item.current_operator_bottom = 4;
                                }
                            }
                        }
                    }
                }
            }

            Rectangle{  //音响区域
                anchors.fill: parent
                visible: dmDeviceType === 6
                color: "transparent"
                Row{
                    anchors.fill: parent
                    spacing: 20
                    Rectangle{   //音效设置
                        width: 200
                        height: parent.height
                        color: "transparent"
                        Rectangle {
                            anchors.top: parent.top
                            anchors.left: parent.left
                            anchors.right: parent.right
                            height: 30
                            color: "transparent"
                            Text {
                                anchors.centerIn: parent
                                font.bold: true
                                font.pointSize: 12
                                color: font_color
                                enabled: false
                                text: dmName
                            }
                        }
                        GridView{
                            id: gv_music
                            anchors.topMargin: 30
                            anchors.fill: parent
                            clip: true
                            cellHeight: 35
                            cellWidth: 100
                            model: lmMusicButtons

                            Component.onCompleted: {
                                gv_music.currentIndex = -1;
                            }

                            delegate: Image {
                                width: 90
                                height: 30
                                source: dmMusicIco

                                Text {
                                    anchors.centerIn: parent
                                    font.bold: true
                                    font.pointSize: 12
                                    color: gv_music.currentIndex === index?selected_color:"black"
                                    text: dmMusicName
                                }
                                MouseArea{
                                    anchors.fill: parent
                                    onClicked: {
                                        gv_music.currentIndex = index;
                                        $manager.device_ctrl(rect_device_ctrl_item.parent_index, "setEffect", index + 1);
                                    }
                                }
                            }
                        }
                    }
                    Rectangle{ //音量设置
                        width: parent.width-200
                        height: parent.height
                        color: "transparent"
                        ListView{
                            id: lv_mute
                            anchors.fill: parent
                            focus: false
                            orientation: ListView.Horizontal
                            boundsBehavior: Flickable.StopAtBounds
                            snapMode: ListView.SnapOneItem
                            highlightRangeMode: ListView.StrictlyEnforceRange
                            highlightMoveDuration: 250
                            clip: true
                            spacing: 10

                            model:lmMusicSlides

                            delegate: Rectangle {
                                id: lv_mute_ctrl
                                property int music_db: dmVol
                                property int old_music_db: dmVol
                                property bool mouse_click: false
                                property bool mouse_click2: false

                                property int old_slide_x: img_music_slide.x
                                property int old_slide_y: img_music_slide.y
                                //二期新增
//                                property alias music_text:music_db_text.text
                                property alias music_slide_bkg:img_music_slide_bkg
                                property alias music_slide:img_music_slide
                                property alias music_mule:btn_music_mule
                                width: 80
                                height: parent.height
                                color: "transparent"
                                Column{
                                    anchors.fill: parent
                                    spacing: 10
                                    Rectangle{
                                        width: 40
                                        height: 20
                                        color: "transparent"
                                        anchors.horizontalCenter: parent.horizontalCenter
                                        Row{
                                            anchors.fill: parent
                                            spacing: 2
                                            Text {
                                                width: 20
                                                height: 20
                                                font.bold: true
                                                font.pointSize: 12
                                                horizontalAlignment: Text.AlignRight
                                                color: font_color
                                                text: dmMusicName

                                            }
                                            Text {
                                                id:music_db_text
                                                width: 20
                                                height: 20
                                                font.bold: true
                                                font.pointSize: 12
                                                horizontalAlignment: Text.AlignLeft
                                                color: globalSettings.selected_color
                                                text: music_db + "db"    //音量值
                                            }
                                        }
                                    }
                                    Image {  // 音量条
                                        id: img_music_slide_bkg
                                        anchors.horizontalCenter: parent.horizontalCenter
                                        width: 40
                                        height: parent.height - 80
                                        source: dmMusicIco
                                        Image {
                                            z:2
                                            id: img_music_slide
                                            x: parent.width/2 - img_music_slide.width/2
                                            y: parent.height/2 -img_music_slide.height/2
                                            width: 16
                                            height: 22
                                            smooth: true
                                            source: dmMusicSlide

                                            Component.onCompleted: {
    //                                            console.log("Component.onCompleted", y, dmVol, dmMusicDbMin, dmMusicDbMax, img_music_slide_bkg.height);
                                                if(dmVol >= dmMusicDbMin && dmVol <= dmMusicDbMax){
                                                    img_music_slide.y = (img_music_slide_bkg.height - (dmVol - dmMusicDbMin) * img_music_slide_bkg.height / (dmMusicDbMax - dmMusicDbMin)) - img_music_slide.height/2;
    //                                                console.log("Component.onCompleted --", y, dmVol, dmMusicDbMin, dmMusicDbMax, img_music_slide_bkg.height);
                                                }
                                            }
//                                            MouseArea{
//                                              anchors.fill: parent
//                                              propagateComposedEvents: true

//                                              onPressed: {
//                                                  console.log("onPressed")
//                                                  mouse_click2=false

//                                                  mouse.accepted = true//传递到此为止？


//                                              }
//                                              onReleased: {
//                                                  console.log("onReleased")

//                                                  mouse_click2=false

//                                                  mouse.accepted = false//传递到此为止？


//                                              }
//                                            }
                                            onYChanged: {
//                                                console.log("onYChanged:", y, dmMusicDbMax, dmMusicDbMin)
                                                if(!rect_device_ctrl_item.sync_msg)
                                                music_db = dmMusicDbMin + (img_music_slide_bkg.height - (img_music_slide.y + img_music_slide.height / 2)) * (dmMusicDbMax - dmMusicDbMin) / img_music_slide_bkg.height
//                                                console.log(img_music_slide_bkg.height,img_music_slide.y)
//                                                if(mouse_click2){
//                                                    console.log("onYChanged setVol")
//                                                    $manager.device_ctrl(rect_device_ctrl_item.parent_index, "setVol", index + 1, music_db);


//                                                }
                                            }
                                        }
                                        MouseArea{
                                            anchors.fill: parent
                                            drag.target: img_music_slide
                                            drag.axis: Drag.YAxis
                                            drag.maximumY: parent.height-img_music_slide.height/2
                                            drag.minimumY: -img_music_slide.height/2

                                            onClicked: {
//                                                console.log("slide click", parent.x, parent.y);
                                                if(mouseY > img_music_slide.y + img_music_slide.height / 2){

                                                    img_music_slide.y += dmMusicDbStep;
                                                    $manager.device_ctrl(rect_device_ctrl_item.parent_index, "turnVolLow", index + 1);
                                                }
                                                else if(mouseY < img_music_slide.y  + img_music_slide.height / 2){
                                                    img_music_slide.y -= dmMusicDbStep;
                                                    $manager.device_ctrl(rect_device_ctrl_item.parent_index, "turnVolHigh", index + 1);
                                                }

                                            }
                                            onPressed: {
                                                old_slide_x = img_music_slide.x;
                                                old_slide_y = img_music_slide.y;
                                                mouse_click = true;
                                                old_music_db=music_db


                                            }

                                            onPositionChanged: {
                                                if(mouse_click){

                                                    music_db = dmMusicDbMin + (img_music_slide_bkg.height - (img_music_slide.y + img_music_slide.height / 2)) * (dmMusicDbMax - dmMusicDbMin) / img_music_slide_bkg.height;

                                                    $manager.device_ctrl(rect_device_ctrl_item.parent_index, "setVol", index + 1, music_db);
                                                    music_db_text.text=music_db+"db"
                                                }
                                            }

                                            onReleased: {
                                                if(mouse_click && (old_slide_x !== img_music_slide.x || old_slide_y !== img_music_slide.y)){
                                                    music_db = dmMusicDbMin + (img_music_slide_bkg.height - (img_music_slide.y + img_music_slide.height / 2)) * (dmMusicDbMax - dmMusicDbMin) / img_music_slide_bkg.height;
                                                    $manager.device_ctrl(rect_device_ctrl_item.parent_index, "setVol", index + 1, music_db);
                                                    music_db_text.text=music_db+"db"
//                                                    if(music_db>old_music_db){
//                                                        console.log("增加音量",music_db-old_music_db)
//                                                        for(var i=0;i<music_db-old_music_db;i++)
//                                                            $manager.device_ctrl(rect_device_ctrl_item.parent_index, "turnVolHigh", index + 1);


//                                                    }
//                                                    else{

//                                                        console.log("降低音量",old_music_db-music_db)
//                                                        for(var i=0;i<old_music_db-music_db;i++)
//                                                            $manager.device_ctrl(rect_device_ctrl_item.parent_index, "turnVolLow", index + 1);

//                                                    }
                                                }
                                                mouse_click = false;
                                            }
                                        }
                                    }
                                    Image {  //静音按钮
                                        id: btn_music_mule
                                        anchors.horizontalCenter: parent.horizontalCenter
                                        property bool select_mule: dmStatus
                                        width: 40
                                        height: 40
                                        source: select_mule?dmMusicImageActive:dmMusicImageIdle
                                        MouseArea{
                                            anchors.fill: parent
                                            onClicked: {
                                                parent.select_mule = !parent.select_mule;
                                                $manager.device_ctrl(rect_device_ctrl_item.parent_index, parent.select_mule?"muteActive":"muteIdle", index + 1);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }


            Rectangle{  //会议摄像机
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: device_ctrl_item_header.bottom
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 2
                visible: dmDeviceType == 9
                enabled: dmDeviceType == 9
                id:audio_rect
                property var operate
                property var action
                property var num: 1
                color: "transparent"
//                border.color:"white"
//                border.width: 1
//                    Timer {
//                         id:audio_timer
//                         interval: 200
//                         repeat: true
//                         onTriggered: {
////                           console.log(audio_rect.operate,audio_rect.action)
//                            $manager.hdmi_control(audio_rect.operate,audio_rect.action)
//                         }
//                     }
                    Image {
                        id:image_top_7
                        width: parent.width/3
                        height: parent.height/5
                        anchors.bottom: center_rect.top
                        anchors.horizontalCenter: parent.horizontalCenter
                        source: image_top_area.containsPress?"qrc:/images/all_ctl/toppress.png":"qrc:/images/all_ctl/top.png"

                        MouseArea{
                            id:image_top_area
                            anchors.fill: parent                       
                            onPressed: {
                                $manager.device_ctrl(index,1,0,0)    //0代表开始


                            }
                            onReleased: {
                                $manager.device_ctrl(index,1,1,0)    //0代表开始


                            }
                        }
                    }
                     Image {
                         id:image_left_7
                         width: parent.height/5
                         height:parent.width/3
                         source: image_left_area.containsPress?"qrc:/images/all_ctl/leftpress.png":"qrc:/images/all_ctl/left.png"
                         anchors.verticalCenter: parent.verticalCenter
                         anchors.right: center_rect.left
                         MouseArea{
                             id:image_left_area
                             anchors.fill: parent
                             onPressed: {
                                 $manager.device_ctrl(index,3,0,0)    //0代表开始

                             }
                             onReleased: {
                                 $manager.device_ctrl(index,3,1,0)    //0代表开始

                             }
                         }
                     }
                     Rectangle{
                        id:center_rect
                       width: parent.width/4
                       height: width
                       radius: height/2
                       anchors.centerIn: parent
                       color: "grey"

                       Text {
                         anchors.centerIn: parent
                         color: "white"
                         font.pixelSize: parent.height/5
                         text: $manager.iszhongwen?(qsTr("预置点:")+audio_rect.num):(qsTr("Preset:")+audio_rect.num)
                              }
                     }

                     Image {
                         id:image_right_7
                         width: parent.height/5
                         height:parent.width/3
                         source: image_right_area.containsPress?"qrc:/images/all_ctl/rightpress.png":"qrc:/images/all_ctl/right.png"
                         anchors.verticalCenter: parent.verticalCenter
                         anchors.left: center_rect.right
                         MouseArea{
                             id:image_right_area
                             anchors.fill: parent
                             onPressed: {
                                 $manager.device_ctrl(index,4,0,0)    //0代表开始



                             }
                             onReleased: {
                                 $manager.device_ctrl(index,4,1,0)    //0代表开始

                             }
                         }
                     }
                     Image {
                             id:image_bottom_7
                             width: parent.width/3
                             height: parent.height/5
                             anchors.top: center_rect.bottom
                             anchors.horizontalCenter: parent.horizontalCenter
                             source: image_bottom_area.containsPress?"qrc:/images/all_ctl/bottompress.png":"qrc:/images/all_ctl/bottom.png"

                             MouseArea{
                                 id:image_bottom_area
                                 anchors.fill: parent
                                 onPressed: {
                                     $manager.device_ctrl(index,2,0,0)    //0代表开始

                                 }
                                 onReleased: {
                                     $manager.device_ctrl(index,2,1,0)    //0代表开始

                                 }
                             }
                         }
                     Image {
                             id:image_in_7
                             width: 50
                             height: 50
                             source: image_in_area.containsPress?"qrc:/images/all_ctl/decpress.png":"qrc:/images/all_ctl/dec.png"
                             anchors.left: parent.left
                             anchors.bottom: parent.bottom
                             MouseArea{
                                 id:image_in_area
                                 anchors.fill: parent
                                 onPressed: {

                                     if(audio_rect.num-1>=1){
                                         audio_rect.num=audio_rect.num-1
                                         $manager.device_ctrl(index,5,0,audio_rect.num)    //0代表开始
                                     }


                                 }
                                 onReleased: {
                                      $manager.device_ctrl(index,5,1,audio_rect.num)    //0代表开始


                                 }
                             }
                         }

                     Image {
                         id:image_out_7
                         width: 50
                         height: 50
                         source: image_out_area.containsPress?"qrc:/images/all_ctl/addpress.png":"qrc:/images/all_ctl/add.png"
                         anchors.right: parent.right
                         anchors.bottom: parent.bottom
                         MouseArea{
                             id:image_out_area
                             anchors.fill: parent
                             onPressed: {

                                 if(audio_rect.num+1<=6){

                                     audio_rect.num=audio_rect.num+1
                                     $manager.device_ctrl(index,6,0,audio_rect.num)    //0代表开始
                                 }


                             }
                             onReleased: {
                                  $manager.device_ctrl(index,6,1,audio_rect.num)    //0代表开始


                             }
                         }
                     }


            }
            Rectangle{  //播放遥控器
                id:control_rect
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: device_ctrl_item_header.bottom
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 2
                visible: dmDeviceType == 7
                enabled: dmDeviceType == 7
                color: "transparent"
                //                border.color:"white"
                //                border.width: 1
                property bool isStop: true
                Rectangle{  //按键方向区域
                    id:direct_rect
                    height: parent.height
                    width: 220
                    color: "transparent"
                    Image {
                        id:image_top_8
                        width: parent.width/3
                        height: parent.height/5
                        anchors.bottom: center_rect8.top
                        anchors.horizontalCenter: parent.horizontalCenter
                        source: image_top_area_8.containsPress?"qrc:/images/all_ctl/toppress.png":"qrc:/images/all_ctl/top.png"

                        MouseArea{
                            id:image_top_area_8
                            anchors.fill: parent
                            onClicked: {
                                $manager.device_ctrl(index, "btnTop");
                            }
                        }
                    }
                    Image {
                        id:image_left_8
                        width: parent.height/5
                        height:parent.width/3
                        source: image_left_area_8.containsPress?"qrc:/images/all_ctl/leftpress.png":"qrc:/images/all_ctl/left.png"
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.right: center_rect8.left
                        MouseArea{
                            id:image_left_area_8
                            anchors.fill: parent
                            onClicked: {
                                $manager.device_ctrl(index, "btnLeft");
                            }
                        }
                    }
                    //                     Image {
                    //                         id:image_begain_8
                    //                         width: 50
                    //                         height: 50
                    //                         source: control_rect.isBegain?"qrc:/images/8/center-active.png":"qrc:/images/8/center-idle.png"
                    //                         anchors.centerIn: parent

                    //                         MouseArea{
                    //                             id:image_begain_area_8
                    //                             anchors.fill: parent
                    //                             onClicked: {
                    //                                control_rect.isBegain=!control_rect.isBegain
                    //                                if(control_rect.isBegain)
                    //                                 $manager.device_ctrl(index, "btnCenterActive");
                    //                                else
                    //                                 $manager.device_ctrl(index, "btnCenterIdle");
                    //                             }
                    //                         }
                    //                     }

                    Rectangle{
                        id:center_rect8
                        width: parent.width/4
                        height: width
                        radius: height/2
                        anchors.centerIn: parent
                        color: "grey"

                        Image {
                            anchors.centerIn: parent
                            width: parent.width/3*2
                            height: width
                            source: control_rect.isStop?"qrc:/images/8/start.png":"qrc:/images/8/stop.png"

                            MouseArea{
                                id:btn_stop_8
                                anchors.fill: parent
                                onClicked: {
                                    if(control_rect.isStop)
                                        $manager.device_ctrl(index, "btnPlay")

                                    else
                                        $manager.device_ctrl(index, "btnStop")

                                    control_rect.isStop=!control_rect.isStop


                                }
                            }
                        }

                    }
                    Image {
                        id:image_right_8
                        width: parent.height/5
                        height:parent.width/3
                        source: image_right_area_8.containsPress?"qrc:/images/all_ctl/rightpress.png":"qrc:/images/all_ctl/right.png"
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: center_rect8.right
                        MouseArea{
                            id:image_right_area_8
                            anchors.fill: parent
                            onClicked: {
                                $manager.device_ctrl(index, "btnRight");
                            }
                        }
                    }

                    Image {
                        id:image_bottom_8
                        width: parent.width/3
                        height: parent.height/5
                        anchors.top: center_rect8.bottom
                        anchors.horizontalCenter: parent.horizontalCenter
                        source: image_bottom_area_8.containsPress?"qrc:/images/all_ctl/bottompress.png":"qrc:/images/all_ctl/bottom.png"

                        MouseArea{
                            id:image_bottom_area_8
                            anchors.fill: parent
                            onClicked: {
                                $manager.device_ctrl(index, "btnBottom");
                            }
                        }
                    }


                    Image {
                        id:image_forward_8
                        width: parent.width/4
                        height: width
                        source: image_forward_area_8.containsPress?"qrc:/images/8/forwardpress.png":"qrc:/images/8/forward.png"
                        anchors.right: parent.right
                        anchors.verticalCenter: parent.verticalCenter
                        MouseArea{
                            id:image_forward_area_8
                            anchors.fill: parent
                            onClicked: {
                                $manager.device_ctrl(index, "btnForward");
                            }
                        }
                    }
                    Image {
                        id:image_backward_8
                        width: parent.width/4
                        height: width
                        source: image_backward_area_8.containsPress?"qrc:/images/8/backwardpress.png":"qrc:/images/8/backward.png"
                        anchors.left: parent.left
                        anchors.verticalCenter: parent.verticalCenter
                        MouseArea{
                            id:image_backward_area_8
                            anchors.fill: parent
                            onClicked: {
                                $manager.device_ctrl(index, "btnRewind");
                            }
                        }
                    }

                }

                Rectangle{
                    id:btn_rect
                    anchors.right: parent.right
                    anchors.left:direct_rect.right
                    height: parent.height
                    color: "transparent"
                    property int dev_index: index
                    ListModel {
                        id: btn_lsitModel

                        ListElement {
                            LText: "上一页"
                            LCmd: "btnPrevious"
                        }
                        ListElement {
                            LText: "下一页"
                            LCmd: "btnNext"
                        }
                        ListElement {
                            LText: "音量加"
                            LCmd: "btnVolumeUp"
                        }
                        ListElement {
                            LText: "音量减"
                            LCmd: "btnVolumeDown"
                        }
                        ListElement {
                            LText: "静音"
                            LCmd: "btnMuteIdle"
                        }
                        ListElement {
                            LText: "取消静音"
                            LCmd: "btnMuteActive"
                        }
                        ListElement {
                            LText: "开机"
                            LCmd: "btnActive"
                        }
                        ListElement {
                            LText: "关机"
                            LCmd: "btnIdle"
                        }
                    }
                    Grid { //功能键grid
                        width: parent.width/2
                        height: parent.height
                        id:some_btn_grid
                        columns: 2
                        spacing:0
                        rows:4
                        Repeater {
                            model: btn_lsitModel
                            Rectangle {
                                width: some_btn_grid.width/2; height: parent.height/4
                                border.width: 1
                                color: "transparent"
                                Image {
                                    anchors.centerIn: parent
                                    width: parent.width-10
                                    height: parent.height-10
                                    source: btn_area.pressed?"qrc:/images/all_ctl/ID10_press.png":"qrc:/images/all_ctl/ID9.png"
                                    Text {

                                        text: qsTr(LText)
                                        anchors.centerIn: parent
                                        font.pixelSize: parent.height/2
                                        color: "white"

                                    }
                                }
                                MouseArea{
                                    id:btn_area
                                    anchors.fill: parent
                                    onClicked: {

                                        $manager.device_ctrl(btn_rect.dev_index, LCmd);
                                    }
                                }
                            }
                        }
                    }

                    Grid { //数字键

                        height: parent.height
                        anchors.left: some_btn_grid.right
                        anchors.right: parent.right
                        anchors.rightMargin: 10
                        id:num_grid
                        columns: 3
                        spacing:0
                        rows:4
                        Repeater {
                            model: 9
                            Rectangle {
                                width: num_grid.width/3; height: parent.height/4
                                border.width: 1
                                color: "transparent"
                                Image {
                                    anchors.centerIn: parent
                                    width: parent.width-10
                                    height: parent.height-10
                                    source: num_area.pressed?"qrc:/images/all_ctl/ID10_press.png":"qrc:/images/all_ctl/ID9.png"
                                    Text {
                                        id:num_text
                                        text: index+1
                                        anchors.centerIn: parent
                                        font.pixelSize: parent.height/2
                                        color: "white"

                                    }
                                }
                                MouseArea{
                                    id:num_area
                                    anchors.fill: parent
                                    onClicked: {
                                        $manager.device_ctrl(btn_rect.dev_index, "btnNum"+(index+1));

                                    }
                                }
                            }
                        }
                        Rectangle {
                            width: num_grid.width/3; height: parent.height/4
                            border.width: 1
                            color: "transparent"
                            Image {
                                anchors.centerIn: parent
                                width: parent.width-10
                                height: parent.height-10
                                source: num0_area.pressed?"qrc:/images/all_ctl/ID10_press.png":"qrc:/images/all_ctl/ID9.png"
                                Text {
                                    id:num0_text
                                    text: "0"
                                    anchors.centerIn: parent
                                    font.pixelSize: parent.height/2
                                    color: "white"

                                }
                            }
                            MouseArea{
                                id:num0_area
                                anchors.fill: parent
                                onClicked: {

                                    $manager.device_ctrl(btn_rect.dev_index, "btnNum0");

                                }
                            }
                        }



                    }
                }

            }
            Rectangle{  //其他设备
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: device_ctrl_item_header.bottom
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 2
                visible: dmDeviceType == 8
                enabled: dmDeviceType == 8
                id:other_rect
                property var action:false
                color: "transparent"

                Image {
                    id:other_img
                    width: parent.width/2
                    height: parent.height/3
                    anchors.centerIn: parent
                    source: "qrc:/images/other_btn.png"

                    MouseArea{
                        id:other_img_area
                        anchors.fill: parent
                        onClicked: {

                            $device_ctrl.set_device_status(index);
                          if(dmDeviceStatus){
                              rect_Glow.visible=true
                              $manager.device_ctrl(index, "btnActive");
                          }
                          else{
                              rect_Glow.visible=false
                              $manager.device_ctrl(index, "btnIdle");
                          }
                        }

                    }
                }
                Glow{
                          source: other_img
                          anchors.fill: other_img
                          visible: dmDeviceStatus

                          radius:15
                          samples: 17
                          spread: 0.3

                          id:rect_Glow
                          color: "#203c6b"
                      }


            }
        }

        Tumbler {
            id: tumbler
            x: 133
            y: 147
            model: 10
        }

    }

}
