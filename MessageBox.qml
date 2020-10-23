import QtQuick 2.0

Rectangle{
    id:root
    width: name.width<100?100:name.width+100
    height: 50
    color: "#292A2E"
    radius: 10
    property var msg
    y:0
    x:parent.width/2-width/2
    opacity: 0
    signal anistop
    Component.onCompleted: {
        anima.start();

    }
    Text {
        id: name
        text: msg
        anchors.centerIn: parent
        color: "white"
    }
    Timer{
        id:timer
        interval: 500;

        repeat: false

        onTriggered:{
           anima2.start()

        }


    }

    ParallelAnimation{
        id:anima



        NumberAnimation {
            target: root
            property: "y"
            to:50
            duration: 600
//            easing.type: Easing.InOutQuad
        }

        NumberAnimation {
            target: root
            property: "opacity"
            to:1
            duration: 600
//            easing.type: Easing.InOutQuad


        }
        onStopped: {
           timer.start()

        }






    }


    NumberAnimation {
        id:anima2
        target: root
        property: "opacity"
        to:0
        duration: 500

        onStopped: {
          anistop()

        }

//        easing.type: Easing.InOutQuad
    }





}

