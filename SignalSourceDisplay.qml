import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2


import "js/utils.js" as Utils
import videoitem 1.0


Rectangle {  //C区
    color: globalSettings.transparent_color

    anchors.fill: parent
    clip: true

    Component.onCompleted: {
        $manager.sence_changed.connect(fix_row_colum)
        $manager.display_audio_msg.connect(display_audio_msg)
    }
    function fix_row_colum(){ //修复读取配置文件时,小宫格位置错误

        $manager.logical_wall_column=$manager.logical_wall_column
        $manager.logical_wall_row=$manager.logical_wall_column

    }
    function display_audio_msg(hint,msg){
        if(hint){
            var ToastCom=Qt.createComponent("MessageBox.qml")
            if(ToastCom.status===Component.Ready)
              var detaObj=ToastCom.createObject(homePages, {"focus": true,"msg":  $manager.iszhongwen?(qsTr("同步打开音频:")+msg):(qsTr("Synchronize Open Audio:")+msg)});

        }else{

            var ToastCom=Qt.createComponent("MessageBox.qml")
            if(ToastCom.status===Component.Ready)
              var detaObj=ToastCom.createObject(homePages, {"focus": true,"msg": $manager.iszhongwen?(qsTr("同步关闭音频:")+msg):(qsTr("Synchronize Close Audio:")+msg)});
        }

    }
    Rectangle{
        anchors.fill: parent
        id:c_rect
        color: globalSettings.transparent_color
        clip: true


        Repeater{
            id:signalRepeater
            model: $signal_source_display
            SignalSourceDisplayItem{
                id: signal_source_display


            }


            delegate: signal_source_display
        }

     }

    Grid{
        anchors.fill: parent
        columns: $room.phy_wall_column
        rows: $room.phy_wall_row
        z: -1
//        Component.onCompleted: {
//          console.log( $room.phy_wall_column,$room.phy_wall_row)
//        }
        Repeater{
            model: $room.phy_wall_column * $room.phy_wall_row
            Rectangle{
//                    color: "transparent"
                width: parent.width / $room.phy_wall_column
                height: parent.height / $room.phy_wall_row
                color: "#001F3A"//"transparent"
                opacity: 0.7
                border.width: globalSettings.border_size
                border.color: "#667988"//border_color//Qt.lighter(border_color, 1.1)
                Grid{
                    anchors.fill: parent
                    columns: $manager.logical_wall_column
                    rows: $manager.logical_wall_row
                    Repeater{
                        model: $manager.logical_wall_column * $manager.logical_wall_row
                        Rectangle{
                            color: "transparent"
                            width: parent.width / $manager.logical_wall_column
                            height: parent.height / $manager.logical_wall_row
                            z: -2
                            Canvas{
                                anchors.fill: parent
                                onPaint: {
                                    var ctx = getContext("2d")
                                    var r_width = width;
                                    var r_height = height;

                                    ctx.lineWidth = globalSettings.border_size
                                    ctx.strokeStyle = "#7f7f7f"//border_color

                                    ctx.beginPath();
//                                        console.log("width,height", parent.x, parent.y, parent.width, parent.height);
                                    dottedLinTo(ctx, 0, 0, parent.width, 0, 5);
                                    dottedLinTo(ctx, 0, 0, 0, parent.height, 5);
                                    dottedLinTo(ctx, parent.width, parent.height, parent.width, 0, 5);
                                    dottedLinTo(ctx, parent.width, parent.height, 0, parent.height, 5);

                                    ctx.stroke();
                                }
                            }
                        }
                    }
                }

                /* 绘制虚线：
                                * context 绘制画布环境对象
                                * sx 开始位置 - X坐标
                                * sy 开始位置 - Y坐标
                                * ex 结束位置 - X坐标
                                * ey 结束位置 - Y坐标
                                * length 虚线的单位长度
                                * */
                function dottedLinTo(context,sx,sy,ex,ey,length){
                    var w = ex - sx; //获取线条的水平宽度
                    var h = ey - sy; //获取线条的垂直高度
                    var l = Math.sqrt(w*w + h*h); // 获取线条的长度
                    var index = Math.floor(l/length); //获取虚线的个数
//                    context.beginPath();
                    for(var i=0;i<index/2;i++){
                        if(i%2==0){
                            context.moveTo(sx+w/index*i*2,sy+h/index*i*2);
                        }else{
                            context.lineTo(sx+w/index*i*2,sy+h/index*i*2);
                        }
                    }
//                    context.stroke();
                }
            }
        }


    }
    DropArea{
        anchors.fill: parent
        z: -1

        onEntered: {
            drag.source.isclicked = false
            drag.accepted = true;
            destArea.width = drag.source.width;//parseInt(drag.getDataAsString("width"));
            destArea.height = drag.source.height;//parseInt(drag.getDataAsString("height"));
            followArea.width = drag.source.width;//parseInt(drag.getDataAsString("width"));
            followArea.height = drag.source.height;//parseInt(drag.getDataAsString("height"));

            if(drag.supportedActions == Qt.CopyAction){
                followArea.dragSignalSourceIndex = parseInt(drag.getDataAsString("index"));
                followArea.dragSignalSourceName = (drag.getDataAsString("text"));
                followArea.dragSignalSourceVideoId = drag.getDataAsString("videoid");
                followArea.visible = true;
//                    console.log("onEntered, formats - ", drag.source, drag.formats, " action - ", drag.action, followArea.width, followArea.height, drag.getDataAsString("index"), drag.getDataAsString("text"));
            }
            else{
//                    console.log("onEntered, formats - ", drag.source.width, drag.source.height, drag.formats, " action - ", drag.action, followArea.width, followArea.height);
                followArea.visible = false;
            }

            destArea.visible = false;
        }

        onPositionChanged: {
            {
                drag.accepted = true;
                if(drag.supportedActions == Qt.CopyAction){
                    followArea.x = drag.x-(followArea.width/2) ;
                    followArea.y = drag.y-(followArea.height/2) ;
                }
                else{
//                        drag.source.x = drag.x;
//                        drag.source.y = drag.y;
//                        $manager.move_device_position(parseInt(drag.getDataAsString("index")), drag.x, drag.y, parent.width, parent.height);
                }
//                    Utils.calc_dest_area(drag.x, drag.y, followArea.width, followArea.height, parent.width, parent.height);
//                    calc_dest_area(drag.x, drag.y, followArea.width, followArea.height, parent.width, parent.height);
            }
        }

        onDropped: {
//                console.log("onDropped-----------");
            if(drop.supportedActions == Qt.CopyAction){
//                    $manager.add_device_display(parseInt(drop.getDataAsString("index")), destArea.width, destArea.height, destArea.x, destArea.y, parent.width, parent.height);
//                console.log(followArea.width)
//                console.log(wall.width)
                $manager.add_device_display((drop.getDataAsString("videoid")), followArea.width, followArea.height, followArea.x, followArea.y, wall.width, wall.height);
//                    drop.source.mouseAreaDragTarget = null
            }/*else{
//                    drop.source.x = destArea.x;
//                    drop.source.y = destArea.y;
//                    drop.source.width = destArea.width;
//                    drop.source.height = destArea.height;
//                    console.log("move:", drop.getDataAsString("index"), destArea.x, destArea.y, destArea.width, destArea.height);
//                    $manager.update_device_position(parseInt(drop.getDataAsString("index")), destArea.width, destArea.height, destArea.x, destArea.y, parent.width, parent.height);
                $manager.update_device_position(parseInt(drop.getDataAsString("index")), drop.x, drop.y, parent.width, parent.height);
            }*/
            drop.acceptProposedAction();
            drop.accepted = true;
            destArea.visible = false;
            followArea.visible = false;
            followArea.dragSignalSourceIndex = -1;
            followArea.dragSignalSourceName = "";
            followArea.dragSignalSourceVideoId = "";
//                $signal_source_display.data_changed_slot(parseInt(drop.getDataAsString("index")));
//                console.log("move out:", drop.getDataAsString("index"), destArea.x, destArea.y, destArea.width, destArea.height);
        }
        onExited: {
            destArea.visible = false;
            followArea.visible = false;
            followArea.dragSignalSourceIndex = -1;
            followArea.dragSignalSourceName = "";
            followArea.dragSignalSourceVideoId = "";
        }


        Rectangle {
            id: followArea;
            z: 2;
            property alias dragSignalSourceIndex: vi_video.signal_source_index
            property alias dragSignalSourceName:_txt_camera_name.text
            property alias dragSignalSourceVideoId: vi_video.video_id
//            radius: 10
//            clip: true
            width: 110;
            height: 80;
            color: globalSettings.transparent_color
            border.width: globalSettings.border_size;
            border.color: Qt.lighter(globalSettings.border_color, 1.1);

            visible: false;

            function set_image_follow(id, img){
                if(visible){
                    vi_video.set_image(id, img);
                }
            }
            onVisibleChanged: {
                if(!visible){
                    vi_video.clear_image();
                }
            }

            VideoItem{
                id: vi_video
                anchors.fill: parent
                signal_source_index: -1
                model: $room
                video_id: ""
                Component.onCompleted: {
//                        $manager.image_incoming_signal.connect(followArea.set_image_follow);
                }
            }

            Rectangle{
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                height: 25
                color: "#3b4043"
                border.color: Qt.lighter(color, 1.1)
                opacity: 0.6
                Text {
                    id:_txt_camera_name
                    anchors.centerIn: parent
                    font.bold: true
                    font.pointSize: 12
                    color: "white"
                    wrapMode: Text.Wrap
                }
            }
        }
        Rectangle {
            id: destArea
            color: globalSettings.transparent_color
            visible: false
            width: 0
            height: 0
            border.width: globalSettings.border_size * 2;
            border.color: Qt.lighter("gold", 1.1);

            property int row: -1
            property int column: -1
            property int row_num: -1
            property int column_num: -1
        }
    }


    Rectangle{   //C区信号菜单栏模块   二期增加的
        id: win_signal_menu
        visible: false
        width: signal_menu.width
        height: signal_menu.height

//        flags: Qt.FramelessWindowHint
        property int mouse_start_x: 0  //菜单栏的弹出位置
        property int mouse_start_y: 0

        property int mouse_start_x_by_volume: 0  //音量栏的弹出位置
        property int mouse_start_y_by_volume: 0

        property var ipc_name
        property int a_volume: 0

        property int delete_signal_index: 0
        signal positionChange();
        //二期新增
        property  var  index
        //
        x: mouse_start_x;
        y: mouse_start_y;
//        onFocusChanged: {
//          console.log("focus change")
//        }

        Menu{
            id: signal_menu
            width: signal_menu_item_1.width
            onClosed: {
              win_signal_menu.visible=false
            }
            MenuItem{
                id: signal_menu_item_0
                anchors.left: parent.left
                anchors.leftMargin: 6
                text: $manager.iszhongwen?qsTr("关闭"):qsTr("Close")
                height: 24

                onTriggered:{

                      $manager.remove_device_display(win_signal_menu.delete_signal_index)
                      win_signal_menu.visible=false

                }
            }

            MenuItem{
                id: signal_menu_item_1
                anchors.left: parent.left
                anchors.leftMargin: 6
                text: $manager.iszhongwen?qsTr("缩放到子屏"):qsTr("Zoom Subscreen")
                height: 24

                onTriggered:{


                    Utils.calc_dest_area_new(signalRepeater.itemAt(win_signal_menu.index).x,signalRepeater.itemAt(win_signal_menu.index).y,signalRepeater.itemAt(win_signal_menu.index).width,signalRepeater.itemAt(win_signal_menu.index).height,wall.width,wall.height)
                    $manager.update_device_position(win_signal_menu.index,destArea.width,destArea.height, destArea.x, destArea.y, wall.width, wall.height);

                    signalRepeater.itemAt(win_signal_menu.index).width = destArea.width
                    signalRepeater.itemAt(win_signal_menu.index).height = destArea.height
                    signalRepeater.itemAt(win_signal_menu.index).x = destArea.x
                    signalRepeater.itemAt(win_signal_menu.index).y = destArea.y
                    signalRepeater.itemAt(win_signal_menu.index).signalState=1


                    win_signal_menu.visible=false

                }
            }
            MenuItem{
                id: signal_menu_item_2
                anchors.left: parent.left
                anchors.leftMargin: 6
                text:  $manager.iszhongwen?qsTr("全屏"):qsTr("Full Screen")
                height: 24

                onTriggered:{
                    Utils.full_screen(wall.width, wall.height)
                    $manager.update_device_position(win_signal_menu.index,destArea.width,destArea.height, destArea.x, destArea.y, wall.width, wall.height);

                    signalRepeater.itemAt(win_signal_menu.index).width = destArea.width
                    signalRepeater.itemAt(win_signal_menu.index).height = destArea.height
                    signalRepeater.itemAt(win_signal_menu.index).x = destArea.x
                    signalRepeater.itemAt(win_signal_menu.index).y = destArea.y
                    signalRepeater.itemAt(win_signal_menu.index).signalState=1
                    win_signal_menu.visible=false
                }
            }
            MenuItem{
                id: signal_menu_item_3
                anchors.left: parent.left
                anchors.leftMargin: 6
                text:  $manager.iszhongwen?qsTr("打开音频"):qsTr("Open Audio")
                height: 24

                onTriggered:{
                    win_signal_menu.visible=false
                    $manager.set_audio_param(0,true)
                    var ToastCom=Qt.createComponent("MessageBox.qml")
                    if(ToastCom.status===Component.Ready)
                        var detaObj=ToastCom.createObject(homePages, {"focus": true,"msg": $manager.iszhongwen?(qsTr("打开音频:")+win_signal_menu.ipc_name):(qsTr("Open Audio:")+win_signal_menu.ipc_name)});

                }
            }
            MenuItem{
                id: signal_menu_item_4
                anchors.left: parent.left
                anchors.leftMargin: 6
                text: $manager.iszhongwen?qsTr("关闭音频"):qsTr("Close Audio")
                height: 24

                onTriggered:{
                    win_signal_menu.visible=false
                     $manager.set_audio_param(0,false)
                    var ToastCom=Qt.createComponent("MessageBox.qml")
                    if(ToastCom.status===Component.Ready)
                        var detaObj=ToastCom.createObject(homePages, {"focus": true,"msg":$manager.iszhongwen?( qsTr("关闭音频:")+win_signal_menu.ipc_name):( qsTr("Close Audio:")+win_signal_menu.ipc_name)});
                }
            }
            MenuItem{
                id: signal_menu_item_5
                anchors.left: parent.left
                anchors.leftMargin: 6
                text: $manager.iszhongwen?qsTr("设置音量"):qsTr("Set Volume")
                height: 24

                onTriggered:{
                    win_signal_menu.visible=false
                    vol_rect.visible=true



                }
            }

        }
    }
    Rectangle{
        id:vol_rect
        visible:false
        x:win_signal_menu.mouse_start_x_by_volume-width/2
        y:win_signal_menu.mouse_start_y_by_volume-height/2
        width: volControl.width+20
        height: volControl.height+20
        color: "#292A2E"
        radius: 10
        property  int hint: 0
        Slider{

                             id: volControl

                             from:0
                             value: win_signal_menu.a_volume
                             to:100
                             anchors.centerIn: parent
                             stepSize:1
                             background: Rectangle {
                                 x: volControl.leftPadding
                                 y: volControl.topPadding + volControl.availableHeight / 2 - height / 2
                                 implicitWidth: 120
                                 implicitHeight: 4
                                 width: volControl.availableWidth
                                 height: implicitHeight
                                 radius: 2
                                 color: "#454646"

                                 Rectangle {
                                     width: volControl.visualPosition * parent.width
                                     height: parent.height
                                     color: globalSettings.selected_color
                                     radius: 2
                                 }

                             }
                             MouseArea{
                                  propagateComposedEvents: true  //记得要传递事件到下层去
                                  anchors.fill: parent
                                  hoverEnabled: true
                                  id:sliderArea
                                  onPressAndHold: {
                                     mouse.accepted = false
                                  }
                                  onClicked: {
                                    mouse.accepted = false
                                  }
                                  onPressed: {
                                    mouse.accepted = false
                                  }

                             }
                             handle: Rectangle {
                                 x: volControl.leftPadding + volControl.visualPosition * (volControl.availableWidth - width)
                                 y: volControl.topPadding + volControl.availableHeight / 2 - height / 2
                                 implicitWidth: 18
                                 implicitHeight: 18
                                 radius: 9
                                 visible:sliderArea.containsMouse? true:false
                                 color: volControl.pressed ? "#f0f0f0" : "#f6f6f6"
                                 border.color: "#bdbebf"
                                 Rectangle{
                                    anchors.centerIn: parent
                                    width: 6
                                    height: 6
                                    radius: 3
                                    color:globalSettings.selected_color
                                 }
                             }
                             onMoved: {

                             }
                             onPositionChanged: {
//                               console.log(value)
                             }

                         }

        MouseArea{
          anchors.fill: parent
          hoverEnabled: true //默认是false
          propagateComposedEvents: true
          onPressAndHold: {
             mouse.accepted = false
          }
          onClicked: {
            mouse.accepted = false
          }
          onPressed: {
            mouse.accepted = false
          }
          onEntered: {
             vol_rect.hint=1
        }
          onExited: {
              if(vol_rect.hint==1){
                   vol_rect.visible=false
              }
              vol_rect.hint=0

              $manager.set_signal_source_in_wall_volume(0,volControl.value)
         }

        }

    }



}
