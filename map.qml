import QtQuick 2.7
import QtQuick.Window 2.15
import QtPositioning 5.15
import QtLocation 5.15
import QtQml.Models 2.15
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

Rectangle {
    id: mapBox

    property double centrMapLat: 55.7514399474066;
    property double centrMapLng: 37.61889172533533;

    Plugin{
        id: osmPlugin
        name:"osm"
    }


    ListModel{
        id: routeListModel
    }

    Connections {
           target: app
           function onAdd_route_() {
               var newRoute = {};
               newRoute.startPosLat = app.StartLat;
               newRoute.startPosLng = app.StartLng;
               newRoute.endPosLat = app.EndLat;
               newRoute.endPosLng = app.EndLng;
               newRoute.color = app.RouteColor;

               routeListModel.append(newRoute);

               if(centrMapLat !== app.StartLat){
                   centrMapLat = app.StartLat;
               }
               if(centrMapLng !== app.StartLng){
                   centrMapLng = app.StartLng;
               }
           }
       }


    Component {
        id: routeDelegate

        MapPolyline{

            RouteModel{
                id: routeModel
                autoUpdate:false
                plugin: osmPlugin
                query: RouteQuery{
                    id : routeQuery
                }

                Component.onCompleted:{
                    routeQuery.addWaypoint(QtPositioning.coordinate(model.startPosLat, model.startPosLng));
                    routeQuery.addWaypoint(QtPositioning.coordinate(model.endPosLat, model.endPosLng));
                    update();
                    console.log("start " + model.startPosLat + " " + model.startPosLng);
                }
            }

            path: routeModel.status === RouteModel.Ready ? routeModel.get(0).path : null
            line.color: color
            smooth:true
            line.width: 5
            opacity: 0.8

            MouseArea{
                anchors.fill: parent

                acceptedButtons: Qt.LeftButton

                onClicked: {
                    if(routeListModel.count > 0){
                        routeListModel.remove(index);
                    }
                }
            }
        }
    }

    Component{
        id: endPointDelegate

        MapQuickItem {

            RouteModel{
                id: routeModel
                autoUpdate:false
                plugin: osmPlugin

            }

               anchorPoint.x: startPathMarker.width / 2
               anchorPoint.y: startPathMarker.height / 2
               coordinate: routeModel.status === RouteModel.Ready ?
                               routeModel.get(0).path[routeModel.get(0).path.length - 1] : QtPositioning.coordinate()

               sourceItem: Rectangle {
                   id: startPathMarker
                   width: 1.5 * map.zoomLevel
                   height: 1.5 * map.zoomLevel
                   radius: 180
                   border.width: 5
                   border.color: "gray"
                   color: "white"
               }
         }
    }

    Component{
        id: startPointDelegate

        MapQuickItem {

            RouteModel{
               id: routeModel
               autoUpdate:false
               plugin: osmPlugin
            }

               anchorPoint.x: startPathMarker.width / 2
               anchorPoint.y: startPathMarker.height / 2
               coordinate: routeModel.status === RouteModel.Ready ? routeModel.get(0).path[0] : QtPositioning.coordinate()

               sourceItem: Rectangle {
                   id: startPathMarker
                   width: 1.5 * map.zoomLevel
                   height: 1.5 * map.zoomLevel
                   radius: 180
                   border.width: 5
                   border.color: "red"
                   color: "white"
               }
         }
    }

    Map {
        id: map
        anchors.fill:  parent
        plugin: Plugin{name: "mapboxgl"}
        center: QtPositioning.coordinate(centrMapLat, centrMapLng)
        zoomLevel: zoomBar.currentScale

        onZoomLevelChanged: {
            zoomBar.currentScale = map.zoomLevel;
        }

        MapItemView{
            id: route
            model:routeListModel
            delegate: routeDelegate

            Component.onCompleted: {
                console.log("route completed");
            }
        }

        MapItemView {
            id: startPoint
           model: routeListModel
           delegate: startPointDelegate
        }

        MapItemView {
            id: endPoint
           model: routeListModel
           delegate: endPointDelegate
        }
        Component.onCompleted: {
            console.log("map completed");
        }

    }

    Scaler {
        id:zoomBar
        minimumScale: map.minimumZoomLevel
        maximumScale: map.maximumZoomLevel
        currentScale: 10
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.leftMargin: 15
        anchors.topMargin: (parent.height / 2) - (height / 2)
        z:1
    }

}
