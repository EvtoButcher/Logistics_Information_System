import QtQuick 2.7
import QtQuick.Controls 2.0

Slider {
    id: zoomBar

    property double minimumScale // Минимальный масштаб
    property double maximumScale // Максимальный масштаб
    property double currentScale // Текущий масштаб

    //Размеры слайдера и кнопок (масштабируемы):
    height: 200                                     // Все расчёты размеров, привязаны к этому "height".
    width: height / 20                              // 200px - оптимально, т.к. при больших значениях интерфейс выглядит нисуразно

    orientation: Qt.Vertical                        // Решение: установить пределы масштабирования
    from: minimumScale
    value: currentScale
    to: maximumScale

    onMoved: {
        currentScale = value
        //console.log(currentScale)
    }

    background: Rectangle {
        anchors.centerIn: parent
        color: "white"
        border.color: "lightgrey"
        border.width: zoomBar.height / 200
        width: parent.width
        height: parent.height
        radius: width / 2
    }

    handle: Rectangle {
        anchors.horizontalCenter: parent.horizontalCenter
        y: parent.visualPosition * (parent.height -  height)
        width: parent.width * 3
        height: width / 2
        radius: width / 4
        color: "white"
        border.color: "lightgrey"
        border.width: zoomBar.height / 200
    }

    Button{
        id:plusButton
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.top
        anchors.bottomMargin: parent.height / 50
        background: Rectangle {
            implicitWidth: zoomBar.width * 3
            implicitHeight: implicitWidth
            radius: implicitWidth / 4
            color: "white"
            border.width: zoomBar.height / 200
            border.color: "lightgrey"

            Image {
                source: "qrc:/Plus.svg"
                anchors.fill: parent
                anchors.margins: parent.implicitWidth / 6
                anchors.centerIn: parent
            }
        }

        onClicked: {
            currentScale += 0.5
            //console.log(currentScale)
        }
    }

    Button{
        id:minusButton
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.bottom
        anchors.topMargin: parent.height / 50
        background: Rectangle {
            implicitWidth: zoomBar.width * 3
            implicitHeight: implicitWidth
            radius: implicitWidth / 4
            color: "white"
            border.width: zoomBar.height / 200
            border.color: "lightgrey"

            Image {
                source: "qrc:/Minus.svg"
                anchors.fill: parent
                anchors.margins: parent.implicitWidth / 6
                anchors.centerIn: parent
            }
        }

        onClicked: {
            currentScale -= 1
            //console.log(currentScale)
        }
    }
}

