import QtQuick 2.7
import QtQuick.Window 2.15
import QtPositioning 5.15
import QtLocation 5.15
import QtQml.Models 2.15
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

Item {
    width: 10
    height: 250

    property double minimumScale  // Минимальный масштаб
    property double maximumScale // Максимальный масштаб

    property double currentScale // Текущий масштаб

    Rectangle {
        id: zoomBar
        width: 10
        height: 200
        color:"lightgray"
        border.color: "gray"
        border.width: 2
        z:1

        //anchors.leftMargin: 15

        Rectangle{
            id:minusButton
            width: parent.width + 20
            height:30
            color:"lightgray"
            border.color: "gray"
            border.width: 2
            radius: 5
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.top
            z:2

            Text {
                text: qsTr("-")
                anchors.centerIn: parent
                color: "gray"
                font.pixelSize: 30
            }

            MouseArea{
                anchors.fill: parent
                acceptedButtons: Qt.LeftButton
                onClicked: {
                    //console.log(currentScale);
                    currentScale -= 0.5;
                }
            }
        }

        Rectangle {
            id: zoomHandle
            width: parent.width + 20
            height: 15
            color: "gray"
            border.color: "lightgray"
            border.width: 2
            anchors.horizontalCenter: parent.horizontalCenter
            y:currentScale/((maximumScale - minimumScale) / parent.height)

            MouseArea {
                id: zoomArea
                anchors.fill: parent
                drag.target: zoomHandle

                onPositionChanged: {
                    // Ограничиваем перемещение ползунка в пределах zoomBar
                    if (zoomHandle.x < 0) {
                        zoomHandle.x = 0;
                    } else if (zoomHandle.x + zoomHandle.width > zoomBar.width) {
                        zoomHandle.x = zoomBar.width - zoomHandle.width;
                    }
                    if (zoomHandle.y < 0) {
                        zoomHandle.y = 0;
                    } else if (zoomHandle.y + zoomHandle.height > zoomBar.height) {
                        zoomHandle.y = zoomBar.height - zoomHandle.height;
                    }

                    // Обновляем текущий масштаб в зависимости от положения ползунка
                    currentScale = (zoomHandle.y / parent.height) * (maximumScale - minimumScale) / parent.height;
                    //console.log(currentScale);
                }
            }
        }

        Rectangle{
            id:plusButton
            width: parent.width + 20
            height:30
            color:"lightgray"
            border.color: "gray"
            border.width: 2
            radius: 5
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.bottom
            z:2

            Text {
                text: qsTr("+")
                anchors.centerIn: parent
                color: "gray"
                font.pixelSize: 30
            }

            MouseArea{
                anchors.fill: parent
                acceptedButtons: Qt.LeftButton
                onClicked: {
                    console.log(currentScale);
                    currentScale += 0.5;
                }
            }
        }
    }
}
