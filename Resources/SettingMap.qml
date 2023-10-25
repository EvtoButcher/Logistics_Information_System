import QtQuick 2.7
import QtQuick.Window 2.15
import QtPositioning 5.15
import QtLocation 5.15
import QtQml.Models 2.15
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

Rectangle {
    id: mapBox
    width: 600
    height: 610

    property double   centrMapLat: 55.7514399474066;
    property double   centrMapLng: 37.61889172533533;
//    property double   defOpasity: 0.5
//    property variant  selectionRoutes: [];

    Plugin{
        id: osmPlugin
        name:"osm"
    }

    ListModel{
        id: warehouseListModel
    }

    Connections {
           target: warehouseModel

           function onAddWarehouse() {
               var newRoute = {};

               newRoute.lat = warehouseModel.Lat;
               newRoute.lng = warehouseModel.Lng;

               warehouseListModel.append(newRoute);

               if(centrMapLat !== warehouseModel.Lat){
                   centrMapLat = warehouseModel.Lat;
               }
               if(centrMapLng !== warehouseModel.Lng){
                   centrMapLng = warehouseModel.Lng;
               }
            }
    }


    Component{
        id: warehouseDelegate

        MapQuickItem {

               //anchorPoint.x: startPathMarker.width / 2
               //anchorPoint.y: startPathMarker.height / 2
               coordinate: QtPositioning.coordinate(model.lat, model.lng);

               sourceItem: Image {
                               source: "qrc:/WarehouseDepart.svg"
                               width: 50;
                               height: 50;
                           }
         }
    }


    Map {
        id: map
        anchors.fill:  parent
        z:1
        plugin: Plugin{name: "mapboxgl"}
        center: QtPositioning.coordinate(centrMapLat, centrMapLng)
        zoomLevel: zoomBar.currentScale

        onZoomLevelChanged: {
            zoomBar.currentScale = map.zoomLevel;
        }

        MapItemView {
            id: warehouse
            model: warehouseListModel
            delegate: warehouseDelegate
            //z:3
        }

//        MapItemView {
//            id: endPoint
//            model: routeListModel
//            delegate: endPointDelegate
//            z:3
//        }

        MouseArea{
            anchors.fill: parent

            acceptedButtons: Qt.LeftButton

            onClicked: {

            }
        }
    }

    Scaler {
        id:zoomBar
        minimumScale: map.minimumZoomLevel
        maximumScale: map.maximumZoomLevel
        currentScale: 10
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.rightMargin: 14
        anchors.topMargin: (parent.height / 2) - (height / 2)
        z:1
    }

}

