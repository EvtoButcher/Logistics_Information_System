import QtQuick 2.7
import QtQuick.Window 2.15
import QtPositioning 5.15
import QtLocation 5.15
import QtQml.Models 2.15
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3


Rectangle {

    ListModel{
        id: routeListModel
    }

    Component {
        id: routeDelegate

        MapPolyline{

            RouteModel{
                id: routeModel
                autoUpdate:false
                plugin: Plugin{name:"osm"}
                query: RouteQuery{
                    id : routeQuery
                }

                Component.onCompleted:{
                    //console.log(startPosLat)

                    routeQuery.addWaypoint(QtPositioning.coordinate(startPosLat, startPosLng));
                    routeQuery.addWaypoint(QtPositioning.coordinate(endPosLat, endPosLng));
                    update();
                }
            }

            path: routeModel.status === RouteModel.Ready ? routeModel.get(0).path : null
            line.color: "green"
            smooth:true
            line.width: 5
            opacity: 0.8


            MouseArea{
                anchors.fill: parent

                acceptedButtons: Qt.LeftButton

                onClicked: {
                    //console.log("right click")
                    routeListModel.remove(index);
                }
            }
        }
    }

    Connections {
           target: app
           onAdd_route_:{
               var newRoute = {};
               newRoute.startPosLat = app.StartLat;
               newRoute.startPosLng = app.StartLng;
               newRoute.endPosLat = app.EndLat;
               newRoute.endPosLng = app.EndLng;
               routeListModel.append(newRoute);

           }
       }

    Map{
        id: map
        anchors.fill:  parent
        plugin: Plugin{name: "mapboxgl"}
        center: QtPositioning.coordinate(55.908961, 37.391218)
        zoomLevel: 12.5


        MapItemView{
            model:routeListModel
            delegate: routeDelegate
        }



//        MapItemView {
//                   model:routeDelegate. routeModel.status == RouteModel.Ready ? routeModel.get(0).path : null
//                   delegate: MapQuickItem {
//                       anchorPoint.x: pathMarker.width / 2
//                       anchorPoint.y: pathMarker.height / 2
//                       coordinate: modelData

//                       sourceItem: Rectangle {//MapPolyline
//                           id: pathMarker
//                           width: 4
//                           height: 4
//                           radius: 180
//                           border.width: 1
//                           border.color: "black"
//                           color: "white"
//                       }
//                   }
//        }

//        MapItemView {
//                   model: routeModel.status == RouteModel.Ready ? routeModel.get(0).path : null
//                   delegate: MapQuickItem {
//                       anchorPoint.x: startPathMarker.width / 2
//                       anchorPoint.y: startPathMarker.height / 2
//                       coordinate: routeModel.get(0).path[0]

//                       sourceItem: Rectangle {
//                           id: startPathMarker
//                           width: 2 * map.zoomLevel
//                           height: 2 * map.zoomLevel
//                           radius: 180
//                           border.width: 1
//                           border.color: "black"
//                           color: "red"
//                       }
//                   }
//        }

//        MapItemView {
//                   model: routeModel.status == RouteModel.Ready ? routeModel.get(0).path : null
//                   delegate: MapQuickItem {
//                       anchorPoint.x: endPathMarker.width / 2
//                       anchorPoint.y: endPathMarker.height / 2
//                       coordinate: routeModel.get(0).path[routeModel.get(0).path.length-1]

//                       sourceItem: Rectangle {
//                           id: endPathMarker
//                           width: 2 * map.zoomLevel
//                           height: 2 * map.zoomLevel
//                           radius: 180
//                           border.width: 1
//                           border.color: "black"
//                           color: "blue"
//                       }
//                   }
//        }
    }

}
