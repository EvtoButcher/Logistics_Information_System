import QtQuick 2.7
import QtQuick.Window 2.15
import QtPositioning 5.15
import QtLocation 5.15
import QtQml.Models 2.15
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

import "commonQML.js" as Common

Rectangle {
    id: mapBox
    width: 700
    height: 700

    property double   centrMapLat: 55.7514399474066;
    property double   centrMapLng: 37.61889172533533;

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
               var newWarehouse = {};

               newWarehouse.lat = warehouseModel.Lat;
               newWarehouse.lng = warehouseModel.Lng;

               warehouseListModel.append(newWarehouse);

               Common.setNewCenter(warehouseModel.Lat, warehouseModel.Lng);
            }
    }


    Component{
        id: warehouseDelegate

        MapQuickItem {
               coordinate: QtPositioning.coordinate(model.lat, model.lng);

               sourceItem: Image {
                               source: "qrc:/WarehouseDepart.svg"
                               width: 50;
                               height: 50;
                           }
         }
    }


    ListModel{
        id: destinationListModel
    }

    Connections {
           target: destinationModel

           function onAddDestination() {
               var newDestination = {};

               newDestination.lat = destinationModel.Lat;
               newDestination.lng = destinationModel.Lng;

               destinationListModel.append(newDestination);

               Common.setNewCenter(destinationModel.Lat, destinationModel.Lng);
            }
    }


    Component{
        id: destinationDelegate
        MapQuickItem {
            //anchorPoint.x: destinationMarker.width / 2
            anchorPoint.y: destinationMarker.height / 2
            coordinate: QtPositioning.coordinate(model.lat, model.lng);
            sourceItem: Image {
                           id: destinationMarker;
                           source: "qrc:/Destination.svg"
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

        MapItemView {
            id: destination
            model: destinationListModel
            delegate: destinationDelegate
            //z:3
        }

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

