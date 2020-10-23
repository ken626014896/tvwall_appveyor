import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2


import videoitem 1.0

Rectangle {  //B区
    color: globalSettings.transparent_color
    anchors.fill: parent


    Rectangle{
        id: rect_signal_source_title
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.right: parent.right
        height: 30
        color: globalSettings.transparent_color
        clip: true

        Text {
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            text: $manager.iszhongwen?qsTr("信号源"):qsTr("Signal")
            height: 20
            font.bold: true
//            font.pointSize: 17
            font.pixelSize:20
            color: "white"
        }
    }
    TextField{
        id: text_mode_name
        anchors.top: rect_signal_source_title.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 10
        anchors.rightMargin:10
        height: 30
        font.pointSize: 7
        placeholderText: $manager.iszhongwen?qsTr("请输入"):qsTr("please input")
        color: "white"
        background:Rectangle{
            color: "#292A2E"
            anchors.fill: parent

            Rectangle{
                height: parent.height
                anchors.right: parent.right
                width: 30
                color: "transparent"
                Image {
                    id: name
                    anchors.fill: parent
                    source: text_colse.pressed?"qrc:/images/group/search-active.png":"qrc:/images/group/search-idel.png"
                }
                MouseArea{
                 id:text_colse
                 anchors.fill: parent
                 onClicked: {


                    if(text_mode_name.text!=null&&text_mode_name.text!="")
                   {
                    var num=$manager.find_signal(text_mode_name.text)
                    var add=0
                  if(num[0]!=0&&num[1]!=0){
                        if(num[0]==1)
                            add=20*num[0]
                        else if(num[0]>1)
                            add=20+(num[0]-1)*40

                        if(num[1]!=0)
                        {
    //                     console.log(add + (num[1]-1) * 100)
                         lv_signal_source.contentY=add + (num[1]-1) * 100
                        }
                        if(lv_signal_source.contentY!=add + (num[1]-1) * 100){
                            lv_signal_source.contentY=add + (num[1]-1) * 100
                        }

                    }

                 }
                 }

                }

            }
        }
        onTextEdited: {
            //搜索前要展开列表
            var list=lv_signal_source.contentItem.children

//                     console.log("count: " + list.length);
                   for ( var i in list) {
                     if(i!=1)
                     {

//                               console.log("list[ " +i + " ] objectName = " + list[i].list_signal_source_a)
//                               console.log("list[ " +i + " ] num = " + list[i].num)
//                               console.log("list[ " +i + " ] width = " + list[i].width)
//                               console.log("list[ " +i + " ] height = " + list[i].height)
                         var item=lv_signal_source.contentItem.children[j]
                         var v = false;
                         var num=list[i].num

                         for(var j=1; j < list[i].column_group_a.children.length -1; j++){
                             if(!list[i].column_group_a.children[j].visible)
                                list[i].column_group_a.children[j].visible = true
                             else
                                break
                             v = list[i].column_group_a.children[j].visible;
                         }
                         if(v){
                             console.log("open group")
                             list[i].list_signal_source_a.height = 20 + num * 100;
                         }


                     }

                   }

        }


    }
    ListView{
            id:lv_signal_source
            anchors.top:text_mode_name.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.topMargin: 10
            anchors.bottomMargin: 10
            clip: true
            spacing: 20
    //        highlightRangeMode: ListView.StrictlyEnforceRange
    //        boundsBehavior: Flickable.StopAtBounds
    //        snapMode: ListView.SnapOneItem
    //        highlightMoveDuration: 250
    //            interactive: true

            function sence_changed(){
                lmSignalSource.clear()
                lmSignalSource.append($manager.get_group_childs())

            }
            function sence_changed2() {
                if(lmSignalSource.count==0){      //页面生成而数据尚没到达时调用这个方法
                    lmSignalSource.clear()
                    lmSignalSource.append($manager.get_group_childs())
                }

            }

            Component.onCompleted: {

                lmSignalSource.clear()
                lmSignalSource.append($manager.get_group_childs())
                $manager.sence_changed.connect(sence_changed)
                $manager.sence_changed_from_siganl_source.connect(sence_changed2)
//               console.log(lmSignalSource.count)

            }

            ListModel{
                id:lmSignalSource
            }

            model: lmSignalSource//$signal_source


            delegate: com_signal_source_item
        }

        Component{
            id:com_signal_source_item
            Rectangle{
                objectName: "aaa"
                id: list_signal_source
                property alias sourceIndex: video.signal_source_index
                property alias sourceName: txt_signal_source_name.text
        //        color: "#292A2E"//"#0e2439"
                property alias column_group_a: column_group
                property alias  list_signal_source_a: list_signal_source
                property var  num: dmCount
                width: parent===null?80:parent.width
                height: dmDeviceType === 7?(20 + dmCount * 100):80
                color: globalSettings.transparent_color
                z: -1
                property var mouseOld;   //鼠标按下时的坐标
                property var mouseNew;   //鼠标移动时的坐标

                Rectangle{
                    id: rect_signal_source_group
                    visible: dmDeviceType === 7?true:false
                    anchors.fill: parent
    //                width: parent.height
                    color: "transparent"//$signal_source.current_index===dmIndex?selected_color:"#001F3A"

                    Column{
                        id: column_group
                        anchors.fill: parent
                        spacing: 20
                        Rectangle{
                            width: parent.width
                            height: 20
                            color: "transparent"
                            Rectangle{
                                anchors.leftMargin: 15
                                anchors.rightMargin: 10
                                anchors.fill: parent
                                color: $signal_source.current_index===dmIndex?globalSettings.selected_color:"#3b4043"
                                Row{
                                    anchors.fill: parent
                                    spacing: 2
                                    Image {
                                        width: 20
                                        height: 20
                                        source: (column_group.children.length > 2 ?
                                                     (column_group.children[1].visible ?
                                                          dmGroupImageActive : dmGroupImageIdle) : dmGroupImageIdle)
                                    }
                                    Text {
                                        text: dmName
                                        font.bold: true
                                        font.pointSize: 12
                                        color: globalSettings.font_color
                                        wrapMode: Text.Wrap

                                    }
                                }
                            }
                            MouseArea{
                                anchors.fill: parent
                                onClicked: {
                                    $signal_source.set_current_index(dmIndex)
                                    var v = false;
                                    for(var i=1; i < column_group.children.length -1; i++){
                                        column_group.children[i].visible = !column_group.children[i].visible
                                        v = column_group.children[i].visible;
                                    }
                                    if(v){
                                        list_signal_source.height = 20 + dmCount * 100;
                                    }
                                    else{
                                        list_signal_source.height = 30;
                                    }
                                }
                            }
                        }
                        Repeater{
                            model: dmGroupChilds
                            delegate: com_signal_source_item
                        }
                    }

                }

                Rectangle {
                    id: r_camera
                    anchors.fill: parent
    //                width: parent.width
    //                height: 80
                    anchors.leftMargin: 10
                    anchors.rightMargin: 10
                    visible: dmDeviceType ===7 ?false:true
                    color: "#292A2E"
                    //是否点击
                    property bool isclicked: false;
    //                property alias mouseAreaDragTarget: dragArea.drag.target
        //            function set_map(idx, map){
        //                video.set_map(idx, map);
        //            }

                    function set_image(id, img){
                        if(visible){
                            video.set_image(id, img);
                        }
                    }

                    VideoItem{
                        id: video
                        anchors.fill: parent
                        signal_source_index: dmIndex
                        model: $room
                        video_id: dmVideoId
                        Component.onCompleted: {
    //                        $manager.image_incoming_signal.connect(set_image);
                        }
                    }


                    Drag.active: dragArea.drag.active
                    Drag.supportedActions: Qt.CopyAction
                    Drag.dragType: Drag.Automatic
                    Drag.mimeData: {"index": index, "width": width, "height": height, "text": txt_signal_source_name.text, "videoid": dmVideoId}

                    Rectangle{
                        id:s_text_rect
                        anchors.top: parent.top
                        anchors.left: parent.left
                        anchors.right: parent.right
                        height: 25
                        color: $signal_source.current_index===dmIndex?globalSettings.selected_color:"#3b4043"
                        border.color: Qt.lighter(color, 1.1)
                        opacity: 0.6
                        clip: true
                        Text {
                            id:txt_signal_source_name
                            anchors.centerIn:txt_signal_source_name.contentWidth<s_text_rect.width? parent:null
                            text: dmName
                            font.bold: true
//                            font.pointSize: 12
                            font.pixelSize: 15
                            color: "white"
//                            anchors.horizontalCenter:  txt_signal_source_name.contentWidth>s_text_rect.width?parent.horizontalCenter:null
                            anchors.verticalCenter:  txt_signal_source_name.contentWidth>s_text_rect.width?parent.verticalCenter:parent.verticalCenter
                            wrapMode: Text.NoWrap
                            elide: Text.ElideRight
//                            clip: true


//                            NumberAnimation {
//                                            id:animText
//                                            target: txt_signal_source_name
//                                            property: "x"
//                                            duration: 15000
//                                            to:(txt_signal_source_name.text.length - s_text_rect.width )
//                                            running: false
//                                            easing.type: Easing.OutCubic
//                                        }

                        }
//                        MyText{
//                            id:txt_signal_source_name
//                            anchors.centerIn: parent
//                            text: dmName
//                            font.bold: true
//                            font.pointSize: 12
//                            color: "white"
//                            horizontalAlignment: Text.AlignHCenter//(1)
//                            verticalAlignment: Text.AlignVCenter//(2)
//                            width: parent.width

//                        }

                    }
                    MouseArea {
                        id: dragArea
                        anchors.fill: parent
    //                    drag.target: r_camera
    //                    propagateComposedEvents: true
                        pressAndHoldInterval: 300

                        onPressed: {
                            $signal_source.set_current_index(dmIndex)
                            $signal_source.set_current_id(dmVideoId)
    //                        console.log(dmVideoId);
//                            animText.start()
                            drag.target = null
                        }

                        onPressAndHold: {
                            drag.target = r_camera
                        }
                        onDoubleClicked: {
                            $manager.add_device_display(dmVideoId, 80, 80, 0, 0, wall.width, wall.height);
                        }

                    }
                }
            }


        }
}
