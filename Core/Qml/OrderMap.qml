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

    property double   centrMapLat: 55.7514399474066;
    property double   centrMapLng: 37.61889172533533;
    property double   defOpasity: 0.5
    property variant  selectionRoutes: [];

    Plugin{
        id: osmPlugin
        name:"osm"
    }

    ListModel{
        id: routeListModel
    }

    Connections {
           target: route_engine
           function onAddRoute() {
               var newRoute = {};

               newRoute.startPosLat = route_engine.StartLat;
               newRoute.startPosLng = route_engine.StartLng;
               newRoute.endPosLat = route_engine.EndLat;
               newRoute.endPosLng = route_engine.EndLng;
               newRoute.color = route_engine.RouteColor;
               newRoute.opacity = defOpasity;
               newRoute.isCachePath = false;

               routeListModel.append(newRoute);

               Common.setNewCenter(route_engine.StartLat, route_engine.StartLng);
           }

           function onRemoveRoute(index){
               routeListModel.remove(index);
           }

           function onRemoveAllRoutes(){
               routeListModel.clear();
           }

           function onSelectRouteOnTable(index){
               routeListModel.get(index).opacity = 1;
               selectionRoutes.push(index);
               routeListModel.get(index).z = 100;
           }

           function onUnSelectRouteOnTable(){
               routeListModel.get(selectionRoutes[selectionRoutes.length - 1]).opacity = defOpasity;
               routeListModel.get(selectionRoutes[selectionRoutes.length - 1]).z = 0;
               selectionRoutes.pop();
           }

           function onRestorRoute(){
               var oldRoute = {};

               oldRoute.startPosLat = route_engine.StartLat;
               oldRoute.startPosLng = route_engine.StartLng;
               oldRoute.endPosLat = route_engine.EndLat;
               oldRoute.endPosLng = route_engine.EndLng;
               oldRoute.path = route_engine.RoutePath;
               oldRoute.color = route_engine.RouteColor;
               oldRoute.opacity = defOpasity;
               oldRoute.isCachePath = true;

               routeListModel.append(oldRoute);
           }

           function onColorChenged(index, new_color){
               routeListModel.get(index).color = new_color;
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
                    id: routeQuery
                }

                Component.onCompleted:{
                    if(model.isCachePath === false){
                        routeQuery.addWaypoint(QtPositioning.coordinate(model.startPosLat, model.startPosLng));
                        routeQuery.addWaypoint(QtPositioning.coordinate(model.endPosLat, model.endPosLng));
                        update();
                    }
                }
                onStatusChanged:  {
                    route_engine.setRouteStatus(routeModel.status);
                    if(routeModel.status === RouteModel.Ready){
                        route_engine.setPathCache(routeModel.get(0).path);
                    }
                }
            }

            path: model.isCachePath ? route_engine.RoutePath : routeModel.status === RouteModel.Ready ? routeModel.get(0).path : null
            line.color: model.color
            smooth:true
            line.width: 5
            opacity: model.opacity

            Component.onCompleted: {
                route_engine.setPathCacheStatus(1); //rendering of the route cache on the map is completed
            }

            MouseArea{
                anchors.fill: parent

                acceptedButtons: Qt.LeftButton

                onClicked: {
                    if(mouse.button == Qt.LeftButton){
                        route_engine.onUnselectRouteOnMap();
                    }
                    else if (mouse.button === Qt.LeftButton && mouse.modifiers & Qt.ShiftModifier) {
                    }

                    opacity = 1;
                    route_engine.onSelectRouteOnMap(index);
                }
            }
        }
    }

    ListModel{
        id: warehouseListModel
    }

    Connections {
           target: warehouse_engine

           function onAddWarehouse() {
               var newWarehouse = {};

               newWarehouse.PosLat = warehouse_engine.Lat;
               newWarehouse.PosLng = warehouse_engine.Lng;

               warehouseListModel.append(newWarehouse);

               Common.setNewCenter(warehouse_engine.Lat, warehouse_engine.Lng);
           }

           function onRestorWarehouse() {
               var newWarehouse = {};

               newWarehouse.PosLat = warehouse_engine.Lat;
               newWarehouse.PosLng = warehouse_engine.Lng;

               warehouseListModel.append(newWarehouse);
           }
    }

    Component{
        id: warehouseDelegate

        MapQuickItem {
            coordinate: QtPositioning.coordinate(model.PosLat, model.PosLng)

            Component.onCompleted: {
                warehouse_engine.setWarehouseStatus(1);
            }

            anchorPoint.x: warehouseMarker.width / 2
            anchorPoint.y: warehouseMarker.height / 2

            sourceItem: Image {
               id: warehouseMarker
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
           target: destination_engine

           function onAddDestination() {
               var newDestination = {};

               newDestination.lat = destination_engine.Lat;
               newDestination.lng = destination_engine.Lng;

               destinationListModel.append(newDestination);

               //Common.setNewCenter(destination_engine.Lat, destination_engine.Lng);
            }

    }


    Component{
        id: destinationDelegate
        MapQuickItem {
            anchorPoint.x: destinationMarker.width / 4
            anchorPoint.y: destinationMarker.height

            coordinate: QtPositioning.coordinate(model.lat, model.lng);

            sourceItem: Image {
                           id: destinationMarker;
                           source: "qrc:/Destination.svg"
                           width: 50;
                           height: 50;
                       }

            Component.onCompleted: {
                destination_engine.setDestinationStatus(1);
            }
         }
    }

//    Component{
//        id: endPointDelegate

//        MapQuickItem {

//            RouteModel{
//                id: routeModel
//                autoUpdate:false
//                plugin: osmPlugin

//            }
//           anchorPoint.x: startPathMarker.width / 2
//           anchorPoint.y: startPathMarker.height / 2
//           coordinate: model.isCachePath ? route_engine.RoutePath[route_engine.RoutePath.length - 1] :
//                           routeModel.status === RouteModel.Ready ?
//                                routeModel.get(0).path[routeModel.get(0).path.length - 1] :
//                                               QtPositioning.coordinate()

//           sourceItem: Rectangle {
//               id: startPathMarker
//               width: 1.5 * map.zoomLevel
//               height: 1.5 * map.zoomLevel
//               radius: 180
//               border.width: 5
//               border.color: "gray"
//               color: "white"
//               opacity: model.opacity
//           }
//         }
//    }

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

        MapItemView{
            id: route
            model:routeListModel
            z:2
            delegate: routeDelegate
        }

        MapItemView {
            id: warehousePoint
            model: warehouseListModel
            delegate: warehouseDelegate
            z:3
        }

        MapItemView {
            id: destinationPoint
            model: destinationListModel
            delegate: destinationDelegate
            z:3
        }

        MouseArea{
            anchors.fill: parent

            acceptedButtons: Qt.LeftButton

            onClicked: {
                if(selectionRoutes.length){
                    for(var routeIndex = 0; routeIndex < selectionRoutes.length; ++routeIndex){
                        routeListModel.get(routeIndex).opacity = defOpasity;
                        route_engine.onUnselectRouteOnMap();
                    }
                    selectionRoutes.length = 0;
                }
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
