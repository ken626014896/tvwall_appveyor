import QtQuick 2.9
import QtQuick.Controls 2.2

Rectangle {
    color: globalSettings.transparent_color
    anchors.fill: parent
    signal controlSort(int hint)
    property bool isTotal: true
    ListModel {
                      id: model_cn
                      ListElement { text: qsTr("总控区域") }
                      ListElement { text: qsTr("环境控制") }

                  }
    ListModel {
                      id: model_zh
                      ListElement { text: qsTr("Total Control") }
                      ListElement { text: qsTr("Env Control") }

                  }
//    ComboBox {  //二期新增下拉框
//        id: comboBox
//        width: parent.width
//        height: 30
//        currentIndex: 0
//        model: $manager.iszhongwen?model_cn:model_zh
//        onCurrentIndexChanged: {

//            if(currentIndex==0){
//                comboBox.displayText=model.get(comboBox.currentIndex).text

//                  controlSort(0)
//                lv_device.visible=false
//                                        }
//            if(currentIndex==1){
//                 controlSort(1)
//                comboBox.displayText=model.get(comboBox.currentIndex).text
//                lv_device.visible=true

//                                        }

//        }

//        function funcc(){
//            console.log("switch")
//            comboBox.displayText=model.get(comboBox.currentIndex).text

//         }
//         Component.onCompleted: {

//           $manager.fix_comboBox_signal.connect(funcc)
//         }

//    }
    Rectangle{
       id:comboBox
       width: parent.width
       height: 30
         Row{
            anchors.centerIn: parent
            spacing: 10
            Image {
                 width:25
                height:25
                source:isTotal? "qrc:/images/all.png":"qrc:/images/openn.png"
                anchors.verticalCenter: parent.verticalCenter
            }
            Text {
                id:left_text
                font.pixelSize: comboBox.height/2
                font.family: "微软雅黑"
                anchors.verticalCenter: parent.verticalCenter
                text: isTotal?($manager.iszhongwen?qsTr("总控区域"):qsTr("Total Control")):($manager.iszhongwen?qsTr("环境控制"):qsTr("Env Control"))
            }
         }

         MouseArea{
           anchors.fill: parent
           onPressed: {
              left_text.font.pixelSize=left_text.font.pixelSize-1
           }
           onClicked: {
                   isTotal=!isTotal
                  if(isTotal){
                      controlSort(0)
                      lv_device.visible=false

                  }
                  else{
                      controlSort(1)
                      lv_device.visible=true
                  }


           }
           onReleased: {
               left_text.font.pixelSize=parent.height/2

           }
         }



    }
    ListView{
        id:lv_device
//        anchors.fill: parent
        anchors.top: comboBox.bottom
        width: parent.width
        height: parent.height-comboBox.height
        anchors.left: parent.left
        anchors.topMargin: 10
        anchors.leftMargin: 10
        visible: false
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        model: $device
        clip: true
        spacing: 10
        property var aaa
        Component.onCompleted: {
            lv_device.currentIndex = -1;

        }

        delegate: Rectangle {
            width: parent.width
            height: 30
            color: "transparent"
            clip: true
            Image {
                anchors.verticalCenter: parent.verticalCenter
                anchors.top: parent.top
                anchors.left: parent.left
                height: 16
                width: 30
                source: $device.current_index===index?(dmListActive===undefined?"":dmListActive):(dmListIdle===undefined?"":dmListIdle)
                id:imggg
            }
            Text {
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: imggg.right
                anchors.leftMargin: 10
                height: 12
                text: dmName
                elide: Text.ElideRight
                font.bold: true
                font.pointSize: 12
                color: $device.current_index===index?"#00bef0":globalSettings.font_color
                wrapMode: Text.Wrap
            }
            MouseArea {
                propagateComposedEvents: true
                anchors.fill: parent
                onClicked: {
                    $device.set_current_index(index)
                    lv_device.currentIndex = index
                    $manager.device_ctrl_select(index);
                }
            }
        }


    }

}
