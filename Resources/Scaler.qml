import QtQuick 2.7
import QtQuick.Controls 2.0

Slider {
    id: zoomBar

    property double minimumScale // Минимальный масштаб
    property double maximumScale // Максимальный масштаб
    property double currentScale // Текущий масштаб

    //Размеры слайдера и кнопок (масштабируемы):
    height: 200                                     // Все расчёты размеров, кроме "margin" и "border.width", привязаны к этому "height".
    width: Math.round(height / 20)                  // 200px - оптимально, т.к. при больших значениях интерфейс выглядит нисуразно,
                                                    // а так же .png иконки кнопок не обладают достаточным разрешением (они всего 40x40px)
    orientation: Qt.Vertical                        // Решение: установить пределы масштабирования и перевести иконки в формат .svg, но мне лень
    from: minimumScale
    value: currentScale
    to: maximumScale

    onMoved: {
        currentScale = value
        //console.log(currentScale)
    }

    handle: Rectangle {
        x: parent.leftPadding + parent.availableWidth / 2 - width / 2
        y: parent.topPadding + parent.visualPosition * (parent.availableHeight - height)
        implicitWidth: parent.width * 3
        implicitHeight: Math.round(implicitWidth / 2)
        radius: Math.round(implicitHeight / 2.5)
        color: "white"
        border.color: "lightgrey"
        border.width: 1
    }

    Button{
        id:plusButton
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.top
        anchors.topMargin: 3
        background: Rectangle {
            implicitWidth: zoomBar.width * 3
            implicitHeight: implicitWidth
            radius: Math.round(implicitWidth / 4)
            color: "white"
            border.width: 1
            border.color: "lightgrey"

            Image {
                source: "qrc:/plus.png"
                anchors.fill: parent
                anchors.centerIn: parent
            }
        }

        onClicked: {
            zoomBar.value = Math.round(zoomBar.value) + 0.5
            currentScale = zoomBar.value
            //console.log(currentScale)
        }
    }

    Button{
        id:minusButton
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.bottom
        anchors.bottomMargin: 3
        background: Rectangle {
            implicitWidth: zoomBar.width * 3
            implicitHeight: implicitWidth
            radius: Math.round(implicitWidth / 4)
            color: "white"
            border.width: 1
            border.color: "lightgrey"

            Image {
                source: "qrc:/minus.png"
                anchors.fill: parent
                anchors.centerIn: parent
            }
        }

        onClicked: {
            zoomBar.value = Math.round(zoomBar.value) - 1
            currentScale = zoomBar.value
            //console.log(currentScale)
        }
    }
}

