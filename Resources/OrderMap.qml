import QtQuick 2.7
import QtQuick.Window 2.15
import QtPositioning 5.15
import QtLocation 5.15
import QtQml.Models 2.15
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

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
           target: app
           function onAddRoute() {
               var newRoute = {};

               newRoute.startPosLat = app.StartLat;
               newRoute.startPosLng = app.StartLng;
               newRoute.endPosLat = app.EndLat;
               newRoute.endPosLng = app.EndLng;
               newRoute.color = app.RouteColor;
               newRoute.opacity = defOpasity;
               newRoute.isCachePath = false;

               routeListModel.append(newRoute);

               if(centrMapLat !== app.StartLat){
                   centrMapLat = app.StartLat;
               }
               if(centrMapLng !== app.StartLng){
                   centrMapLng = app.StartLng;
               }
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

               oldRoute.startPosLat = app.StartLat;
               oldRoute.startPosLng = app.StartLng;
               oldRoute.endPosLat = app.EndLat;
               oldRoute.endPosLng = app.EndLng;
               oldRoute.path = app.RoutePath;
               oldRoute.color = app.RouteColor;
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
                    app.setRouteStatus(routeModel.status);
                    if(routeModel.status === RouteModel.Ready){
                        app.setPathCache(routeModel.get(0).path);
                    }
                }
            }

            path: model.isCachePath ? app.RoutePath : routeModel.status === RouteModel.Ready ? routeModel.get(0).path : null
            line.color: model.color
            smooth:true
            line.width: 5
            opacity: model.opacity

            Component.onCompleted: {
                app.setPathCacheStatus(1); //rendering of the route cache on the map is completed
            }

            MouseArea{
                anchors.fill: parent

                acceptedButtons: Qt.LeftButton

                onClicked: {
                    if(mouse.button == Qt.LeftButton){
                        app.onUnselectRouteOnMap();
                    }
                    else if (mouse.button === Qt.LeftButton && mouse.modifiers & Qt.ShiftModifier) {
                    }

                    opacity = 1;
                    app.onSelectRouteOnMap(index);
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
               coordinate: model.isCachePath ? app.RoutePath[app.RoutePath.length - 1] : routeModel.status === RouteModel.Ready ?
                               routeModel.get(0).path[routeModel.get(0).path.length - 1] : QtPositioning.coordinate()

               sourceItem: Rectangle {
                   id: startPathMarker
                   width: 1.5 * map.zoomLevel
                   height: 1.5 * map.zoomLevel
                   radius: 180
                   border.width: 5
                   border.color: "gray"
                   color: "white"
                   opacity: model.opacity
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
               coordinate: model.isCachePath ? app.RoutePath[0] :
                           routeModel.status === RouteModel.Ready ? routeModel.get(0).path[0] : QtPositioning.coordinate()

               sourceItem: Rectangle {
                   id: startPathMarker
                   width: 1.5 * map.zoomLevel
                   height: 1.5 * map.zoomLevel
                   radius: 180
                   border.width: 5
                   border.color: "red"
                   color: "white"
                   opacity: model.opacity
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

        MapItemView{
            id: route
            model:routeListModel
            z:2
            delegate: routeDelegate
        }

        MapItemView {
            id: startPoint
            model: routeListModel
            delegate: startPointDelegate
            z:3
        }

        MapItemView {
            id: endPoint
            model: routeListModel
            delegate: endPointDelegate
            z:3
        }

        MouseArea{
            anchors.fill: parent

            acceptedButtons: Qt.LeftButton

            onClicked: {
                if(selectionRoutes.length){
                    for(var routeIndex = 0; routeIndex < selectionRoutes.length; ++routeIndex){
                        routeListModel.get(routeIndex).opacity = defOpasity;
                        app.onUnselectRouteOnMap();
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
