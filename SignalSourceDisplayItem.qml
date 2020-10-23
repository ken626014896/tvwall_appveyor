import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2

import "js/utils.js" as Utils

import videoitem 1.0

Component{
    id:signal_source_display_item


    Rectangle{
        property alias signal_x: display_signal_source.x
        property alias signal_y: display_signal_source.y
        //二期新增信号缩放状态
        property var parent_width:  parent.width
        property var parent_height:  parent.height
        property alias signalState:display_signal_source.signalState;

        property var ddddmWinId: dmWinId
        width: dmDisplayWidth * parent.width
        height: dmDisplayHeight * parent.height
        x: dmDisplayX * parent.width
        y: dmDisplayY * parent.height
        color: "#292A2E"//"#0e2439"//"transparent"
        z: $signal_source_display.current_index===index?1:0

//        width: dmDisplayWidth * parent.width - 1
//        height: dmDisplayHeight * parent.height - 1
//        x: dmDisplayX * parent.width
//        y: dmDisplayY * parent.height

//        color: "transparent"
//        Behavior on width {
//          NumberAnimation{
//             duration: 100
//          }

//        }
//        Behavior on height {
//          NumberAnimation{
//             duration: 100
//          }

//        }
//        Behavior on x {
//          NumberAnimation{
//             duration: 100
//          }

//        }
//        Behavior on y {
//          NumberAnimation{
//             duration: 100
//          }

//        }


        id: display_signal_source
        property int step: 5;   //鼠标的检测区域尺寸

        property var mouseOld;   //鼠标按下时的坐标
        property var mouseNew;   //鼠标移动时的坐标
        property  int old_x:0
        property  int old_y:0
        property  int old_width:0
        property  int old_height:0
        property  int init_width
        property  int init_height

        //是否点击
        property bool isclicked: false;
        //鼠标状态
        property int mouseState: 0;
        ////二期新增信号缩放状态
        property int signalState:0;


//        Drag.active: dragArea.drag.active
//        Drag.hotSpot.x: 10
//        Drag.hotSpot.y: 10
//        Drag.supportedActions: Qt.MoveAction
//        Drag.dragType: Drag.Automatic
//        Drag.mimeData: {"index": index, "width": width, "height": height, "text": dmName, "videoid": dmVideoId}

        Rectangle {
            id: r_camera
            anchors.fill: parent
            color: "transparent"
            function set_map(idx, map){
                video.set_map(idx, map);
            }

            function set_image(id, img){
                video.set_image(id, img);
            }

            VideoItem{
                id: video
                anchors.fill: parent
                signal_source_index: dmIndex
                model: $room
                video_id: dmVideoId
                Component.onCompleted: {
//                    $manager.image_incoming_signal.connect(set_image);
                }
            }


            Rectangle{
                id:sig_title
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                height: 25
                color: $signal_source_display.current_index===index?globalSettings.selected_color:"#3b4043"
                border.color: Qt.lighter(color, 1.1)
                opacity: 0.6

                Text {
                    id:txt_signal_source_name
                    anchors.centerIn: parent
                    text: dmName
                    font.bold: true
                    font.pointSize: 8
                    color: "white"
                    wrapMode: Text.Wrap
                }
            }
//            Image {  //二期新增  音频指示图标
//                id: audio_img
//                source: $signal_source_display.current_index===index?"qrc:/images/6/music-active.png":""
//                width: 25
//                height: sig_title.height
//                anchors.top: sig_title.bottom
//                anchors.topMargin: 5
//                anchors.rightMargin: 5
//                anchors.right: parent.right
//            }
        }

        PinchArea{
            id: pinch_area
            anchors.fill: parent
            pinch.minimumScale: 0.01
            pinch.maximumScale: 10
            property bool pinch_start: false

            onPinchStarted: {
                $signal_source_display.current_index = index

                pinch.accepted = true;
                pinch_area.pinch_start = true;

            }
            onPinchUpdated: {
//                console.log("ss")
                if(pinch_area.pinch_start){
                    dragArea.enabled=false
                    var s = display_signal_source.scale;
                    s -= pinch.previousScale - pinch.scale;

                    var old_width = display_signal_source.width;
                    var old_height = display_signal_source.height;
                    var current_width = old_width * s;
                    var current_height = old_height * s;

                    if(current_height > 50 && current_width > 50 ){
                        display_signal_source.width = current_width;
                        display_signal_source.height = current_height;
                        display_signal_source.x += (old_width - current_width)/2;
                        display_signal_source.y += (old_width - current_width)/2;
                    }
                }
            }

            onPinchFinished: {

                if(pinch_area.pinch_start){
                    pinch_area.pinch_start = false;
                    $manager.update_device_position(index, display_signal_source.width, display_signal_source.height, display_signal_source.x, display_signal_source.y, wall.width, wall.height);
                    //更新窗体缩放状态
//                     console.log("ss")
                    if(!dragArea.istwoclicked){
//                                   console.log("istwoclicked")
                      display_signal_source.signalState=0
                      dragArea.istwoclicked=!dragArea.istwoclicked
                    }
                    else{
                      dragArea.istwoclicked=!dragArea.istwoclicked
                    }

                }
                 dragArea.enabled=true
            }

            MouseArea {
                id: dragArea;
                anchors.fill: parent
    //            drag.target: display_signal_source
                propagateComposedEvents: true
    //            drag.axis: Drag.XAndYAxis
    //            Drag.dragType: Drag.Internal
                property bool is_moving: false
                property bool istwoclicked: false
                hoverEnabled: display_signal_source.focus;
                acceptedButtons: Qt.LeftButton | Qt.RightButton // 激活右键（别落下这个）
                onPressed:{
//                    console.log(display_signal_source.width)
                  if (mouse.button === Qt.LeftButton) {
//                     console.log("sss")
                    $signal_source_display.set_current_index(index)
                    $signal_source.set_current_id(dmVideoId)
                    display_signal_source.focus=true;  //?
                    display_signal_source.isclicked=true;   //?
                    display_signal_source.mouseOld=display_signal_source.mapToItem(display_signal_source.parent, mouseX, mouseY);  //?
                    win_signal_menu.visible = false
                    vol_rect.visible=false
                     }
    //                mouse.accepted=false;
                }
                onPressAndHold: {
                    if(!is_moving){
                    $signal_source_display.set_current_index(index)
                    $signal_source.set_current_id(dmVideoId)
                    display_signal_source.focus=true;
//                    display_signal_source.isclicked=true;
                    display_signal_source.mouseOld=display_signal_source.mapToItem(display_signal_source.parent, mouseX, mouseY);
                    win_signal_menu.mouse_start_x = display_signal_source.x+display_signal_source.width
                    win_signal_menu.mouse_start_y = display_signal_source.y
                    win_signal_menu.mouse_start_x_by_volume=display_signal_source.x+display_signal_source.width/2
                    win_signal_menu.mouse_start_y_by_volume=display_signal_source.y+display_signal_source.height/2
                    win_signal_menu.a_volume=dmVolume
                    win_signal_menu.ipc_name=dmName
                    win_signal_menu.delete_signal_index =index
                    win_signal_menu.visible = true
                    //二期新增 保存位置参数
                   //保存旧的位置参数
                    if(display_signal_source.signalState==0){  //已处于缩放状态就不保存旧参数
                        display_signal_source.old_x=display_signal_source.x
                        display_signal_source.old_y=display_signal_source.y
//                             console.log(display_signal_source.widths)
                        display_signal_source.old_width=display_signal_source.width
                        display_signal_source.old_height=display_signal_source.height
                    }

                    //缩放到小屏需要的参数
                    win_signal_menu.index=index
                    signal_menu.open();
                    }
                }
                 onClicked: {  //二期新增。右键呼出菜单栏

                     if (mouse.button === Qt.RightButton) {
//                         console.log(qsTr("二期新增。右键呼出菜单栏"))
                         $signal_source_display.set_current_index(index)
                         $signal_source.set_current_id(dmVideoId)
                         display_signal_source.focus=true;
//                         display_signal_source.isclicked=true;
                         display_signal_source.mouseOld=display_signal_source.mapToItem(display_signal_source.parent, mouseX, mouseY);
                         win_signal_menu.mouse_start_x = display_signal_source.x+display_signal_source.width
                         win_signal_menu.mouse_start_y = display_signal_source.y
                         win_signal_menu.mouse_start_x_by_volume=display_signal_source.x+display_signal_source.width/2
                         win_signal_menu.mouse_start_y_by_volume=display_signal_source.y+display_signal_source.height/2
                         win_signal_menu.a_volume=dmVolume
                         win_signal_menu.ipc_name=dmName
//                         console.log(index,dmVolume)
                         win_signal_menu.delete_signal_index =index
                         win_signal_menu.visible = true
                         //二期新增 保存位置参数
                        //保存旧的位置参数
                         if(display_signal_source.signalState==0){
                             display_signal_source.old_x=display_signal_source.x
                             display_signal_source.old_y=display_signal_source.y
//                             console.log(display_signal_source.widths)
                             display_signal_source.old_width=display_signal_source.width
                             display_signal_source.old_height=display_signal_source.height
                         }

                         //缩放到小屏需要的参数
                         win_signal_menu.index=index
                         signal_menu.open();
                     }

                 }

                onDoubleClicked: {
                     dragArea.istwoclicked=true
                     if (mouse.button === Qt.LeftButton&& display_signal_source.signalState==1) {
                      //二期新代码 还原位置
                      $manager.update_device_position(index, display_signal_source.old_width, display_signal_source.old_height, display_signal_source.old_x, display_signal_source.old_y, wall.width, wall.height);
                      display_signal_source.width = display_signal_source.old_width
                      display_signal_source.height = display_signal_source.old_height
                      display_signal_source.x = display_signal_source.old_x
                      display_signal_source.y = display_signal_source.old_y
                      //更新状态
                      display_signal_source.signalState=0
//                       console.log("small")

                    }
                    else if (mouse.button === Qt.LeftButton&& (display_signal_source.signalState==0)){
//                         console.log("big")
//                         console.log(display_signal_source.width)
                         display_signal_source.old_x=display_signal_source.x
                         display_signal_source.old_y=display_signal_source.y
                         display_signal_source.old_width=display_signal_source.width
                         display_signal_source.old_height=display_signal_source.height

                         Utils.calc_dest_area(display_signal_source.x, display_signal_source.y, display_signal_source.width, display_signal_source.height, wall.width, wall.height)
                         $manager.update_device_position(index,destArea.width,destArea.height, destArea.x, destArea.y, wall.width, wall.height);


                         display_signal_source.width = destArea.width
                         display_signal_source.height = destArea.height
                         display_signal_source.x = destArea.x
                         display_signal_source.y = destArea.y
                         display_signal_source.signalState=1
//                         console.log(display_signal_source.width)

                     }
                }

                onPositionChanged: {

                    if(display_signal_source.isclicked)
                    {

                        is_moving = true;
                        display_signal_source.mouseNew=display_signal_source.mapToItem(display_signal_source.parent, mouseX, mouseY);
//                        console.log("onPositionChanged", display_signal_source.isclicked, display_signal_source.mouseState, display_signal_source.x, display_signal_source.y, display_signal_source.mouseNew.x-display_signal_source.mouseOld.x, display_signal_source.mouseNew.y-display_signal_source.mouseOld.y)

                        switch(display_signal_source.mouseState) //判断鼠标当前状态，0代表，在无焦点的情况下，直接点击就可以拖动。
                        {
                        case 0:
                        case 5:
                            display_signal_source.x = display_signal_source.x+display_signal_source.mouseNew.x-display_signal_source.mouseOld.x;
                            display_signal_source.y = display_signal_source.y+display_signal_source.mouseNew.y-display_signal_source.mouseOld.y;
                            break;

                        case 1:
                            display_signal_source.width=display_signal_source.width-display_signal_source.mouseNew.x+display_signal_source.mouseOld.x;
                            display_signal_source.height=display_signal_source.height-display_signal_source.mouseNew.y+display_signal_source.mouseOld.y;
                            if(display_signal_source.width>25)
                            display_signal_source.x=display_signal_source.x+display_signal_source.mouseNew.x-display_signal_source.mouseOld.x;
                            if(display_signal_source.height>25)
                            display_signal_source.y=display_signal_source.y+display_signal_source.mouseNew.y-display_signal_source.mouseOld.y;
                            break;

                        case 2:
                            display_signal_source.width=display_signal_source.width-display_signal_source.mouseNew.x+display_signal_source.mouseOld.x;
                            if(display_signal_source.width>25)
                            display_signal_source.x=display_signal_source.x+display_signal_source.mouseNew.x-display_signal_source.mouseOld.x;
                            break;

                        case 3:
                            display_signal_source.width = display_signal_source.width - (display_signal_source.mouseNew.x - display_signal_source.mouseOld.x);
                            display_signal_source.height = display_signal_source.height + (display_signal_source.mouseNew.y-display_signal_source.mouseOld.y);
                            if(display_signal_source.width>25)
                            display_signal_source.x = display_signal_source.x + (display_signal_source.mouseNew.x - display_signal_source.mouseOld.x);

                            break;

                        case 4:
                            display_signal_source.y = display_signal_source.y + (display_signal_source.mouseNew.y - display_signal_source.mouseOld.y);
                            display_signal_source.height = display_signal_source.height - (display_signal_source.mouseNew.y - display_signal_source.mouseOld.y);
                            break;

                        case 6:
                            display_signal_source.height = display_signal_source.height + (display_signal_source.mouseNew.y-display_signal_source.mouseOld.y) ;
                            break;

                        case 7:
                            display_signal_source.height=display_signal_source.height-display_signal_source.mouseNew.y+display_signal_source.mouseOld.y;
                            display_signal_source.width=display_signal_source.width+display_signal_source.mouseNew.x-display_signal_source.mouseOld.x;
                            if(display_signal_source.height>25)
                            display_signal_source.y=display_signal_source.y+display_signal_source.mouseNew.y-display_signal_source.mouseOld.y;
                            break;

                        case 8:
                            display_signal_source.width = display_signal_source.width + (display_signal_source.mouseNew.x-display_signal_source.mouseOld.x);
                            break;

                        case 9:
                            display_signal_source.width = display_signal_source.width + (display_signal_source.mouseNew.x-display_signal_source.mouseOld.x);
                            display_signal_source.height = display_signal_source.height + (display_signal_source.mouseNew.y-display_signal_source.mouseOld.y);
                            break;
                        default:

                        }
                        //这里的两个if是限制display_signal_source的最小尺寸，防止缩小到看不见。
                        if(display_signal_source.width<=25)
                            display_signal_source.width=25;

                        if(display_signal_source.height<=25)
                            display_signal_source.height=25;

                        display_signal_source.mouseOld = display_signal_source.mouseNew;
                    }
                    else
                    {
                        if(mouseX<display_signal_source.step&&mouseX>=0)
                        {
                            if(0<=mouseY&&mouseY<display_signal_source.step){
                                display_signal_source.mouseState=1;
                                dragArea.cursorShape= Qt.SizeFDiagCursor;
                            }
                            else if((display_signal_source.height-display_signal_source.step)<mouseY&&mouseY<=display_signal_source.height){
                                display_signal_source.mouseState=3;
                                dragArea.cursorShape= Qt.SizeBDiagCursor;
                            }
                            else if(display_signal_source.step<=mouseY&&mouseY<=display_signal_source.height-display_signal_source.step){
                                display_signal_source.mouseState=2;
                                dragArea.cursorShape= Qt.SizeHorCursor;
                            }
                        }
                        else if(display_signal_source.width-display_signal_source.step<mouseX&&mouseX<=display_signal_source.width)
                        {
                            if(0<=mouseY&&mouseY<display_signal_source.step){
                                display_signal_source.mouseState=7;
                                dragArea.cursorShape= Qt.SizeBDiagCursor;
                            }
                            else if((display_signal_source.height-display_signal_source.step)<mouseY&&mouseY<=display_signal_source.height){
                                display_signal_source.mouseState=9;
                                dragArea.cursorShape= Qt.SizeFDiagCursor;
                            }
                            else if(display_signal_source.step<=mouseY&&mouseY<=display_signal_source.height-display_signal_source.step){
                                display_signal_source.mouseState=8;

                                dragArea.cursorShape= Qt.SizeHorCursor;
                            }
                        }
                        else if(display_signal_source.width-display_signal_source.step>=mouseX&&mouseX>=display_signal_source.step)
                        {
                            if(0<=mouseY&&mouseY<display_signal_source.step){
                                display_signal_source.mouseState=4;
                                dragArea.cursorShape= Qt.SizeVerCursor;
                            }
                            else if((display_signal_source.height-display_signal_source.step)<mouseY&&mouseY<=display_signal_source.height){
                                display_signal_source.mouseState=6;
                                dragArea.cursorShape= Qt.SizeVerCursor;
                            }
                            else if(display_signal_source.step<=mouseY&&mouseY<=display_signal_source.height-display_signal_source.step){
                                display_signal_source.mouseState=5;
                                dragArea.cursorShape=Qt.ArrowCursor;
                            }
                        }
                    }

                }

                onReleased: {


                   if (mouse.button === Qt.LeftButton) {
//                      console.log(display_signal_source.mouseNew.x+","+display_signal_source.mouseNew.y)
    //                console.log("onReleased", display_signal_source.isclicked)
                    display_signal_source.isclicked=false;
                    mouse.accepted=false;
                    if(is_moving){

                            if(display_signal_source.old_x!=display_signal_source.x||display_signal_source.old_y!=display_signal_source.y||display_signal_source.old_height!=display_signal_source.height||display_signal_source.old_width!=display_signal_source.width)
                            {
                                console.log("move")
                                $manager.update_device_position(index, display_signal_source.width, display_signal_source.height, display_signal_source.x, display_signal_source.y, wall.width, wall.height);
                                //更新状态
                                display_signal_source.signalState=0

                           }

                    }
                    is_moving = false;
                    dragArea.cursorShape=Qt.ArrowCursor;
                    display_signal_source.mouseState=0;
                     }
                }
                onFocusChanged: {

                    if(!display_signal_source.focus)
                    {
                        display_signal_source.isclicked=false;
                        dragArea.cursorShape=Qt.ArrowCursor;
                        display_signal_source.mouseState=0;
                        is_moving = false;
                    }
                }

            }
        }
      Component.onCompleted: {
          //记录刚生成是信号的位置参数
          //保存旧的位置参数

          $manager.test_signal22.connect(func2222)
          display_signal_source.old_x=display_signal_source.x
          display_signal_source.old_y=display_signal_source.y
          display_signal_source.old_width=display_signal_source.width
          display_signal_source.old_height=display_signal_source.height

//          console.log(dmDisplayWidth)
//          $manager.rellay_get_device_display.connect(rellay_get_device_display_slot)

      }
      function rellay_get_device_display_slot(){  //有延时，所以等真的获取完C区信号后 记录旧的位置大小信息

//          display_signal_source.old_width= dmDisplayWidth *$manager.manager_get_wall_width()
//          display_signal_source.old_height=dmDisplayHeight * $manager.manager_get_wall_height()
//          display_signal_source.old_x= dmDisplayX * $manager.manager_get_wall_width()
//          display_signal_source.old_y=dmDisplayY *$manager.manager_get_wall_height()

    }

      function func2222(x2,y2,width2,height2 ,win_id){
      //解决出现的同步不成功问题  数据层面改变了但是视图层面可能没改， 所以
      //最后是数据层面和视图层面一起改保险

//      console.log(win_id,ddddmWinId)
         if(win_id===ddddmWinId)
         {

             display_signal_source.x=x2*wall.width*1.0
             display_signal_source.y=y2*wall.height*1.0
             display_signal_source.width=width2*wall.width*1.0
             display_signal_source.height=height2*wall.height*1.0


         }




      }

    }
}
