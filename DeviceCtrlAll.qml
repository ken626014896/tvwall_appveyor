import QtQuick 2.0
import QtQuick.Controls 2.5
Rectangle {
    id: rectangle  //总控区域
    color: globalSettings.transparent_color
    anchors.fill: parent
    MouseArea{
      anchors.fill: parent
      onClicked: {

         console.log("ss")
      }

    }
    }
//    Row{
//        anchors.fill: parent
//        Rectangle{
//           id:allControl1
//           height: parent.height
//           width: parent.width/3
//           color: "transparent"

//        }
//        Rectangle{
//           id:allControl2
//           height: parent.height
//           width: parent.width/3
//           color: "transparent"

//        }
//        Rectangle{
//           id:allControl3
//           height: parent.height
//           width: parent.width/3
//           color: "transparent"

//        }

//    }



}



/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}D{i:1;anchors_height:50;anchors_width:50}
}
 ##^##*/
