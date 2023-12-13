QT       += core gui sql concurrent positioning

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets quickwidgets

CONFIG += c++17

INCLUDEPATH += \
    ui \
    Core \
    Core/AbstractModels \
    Core/ModelForQML \
    Core/Qml \
    Common \
    Resources \
    locale \


HEADERS += \
    Core/AbstractModels/AbstractCompanyItemModel.h \
    Core/AbstractModels/AbstractMapItemModel.h \
    Core/ApplicationSettings.h \
    Core/Company.h \
    Core/AbstractModels/AbstractTrafficModel.h \
    Core/ModelForQML/CarSimulatorModel.h \
    ui/CreateCompanyDialog.h \
    ui/CreateDestinationDialog.h \
    ui/CreateWarehouseDialog.h \
    ui/DestinationControlWudget.h \
    Core/ModelForQML/DestinationModel.h \
    Core/MapItemEngine.h \
    ui/OrderAddDialog.h \
    ui/OrderAddWidget.h \
    Core/OrderDB.h \
    ui/OrderTableWidget.h \
    Core/ModelForQML/RouteModel.h \
    ui/TableDelegate.h \
    Common/TextMessage.h \
    ui/WarehouseControlWidget.h \
    Core/ModelForQML/WarehouseModel.h \
    Common/common.h \
    ui/mainwindow.h

SOURCES += \
    Core/AbstractModels/AbstractCompanyItemModel.cpp \
    Core/AbstractModels/AbstractMapItemModel.cpp \
    Core/ApplicationSettings.cpp \
    Core/Company.cpp \
    Core/AbstractModels/AbstractTrafficModel.cpp \
    Core/ModelForQML/CarSimulatorModel.cpp \
    ui/CreateCompanyDialog.cpp \
    ui/CreateDestinationDialog.cpp \
    ui/CreateWarehouseDialog.cpp \
    ui/DestinationControlWudget.cpp \
    Core/ModelForQML/DestinationModel.cpp \
    Core/MapItemEngine.cpp \
    ui/OrderAddDialog.cpp \
    ui/OrderAddWidget.cpp \
    Core/OrderDB.cpp \
    ui/OrderTableWidget.cpp \
    Core/ModelForQML/RouteModel.cpp \
    ui/TableDelegate.cpp \
    ui/WarehouseControlWidget.cpp \
    Core/ModelForQML/WarehouseModel.cpp \
    Common/common.cpp \
    main.cpp \
    ui/mainwindow.cpp

FORMS += \
    Forms/MainWindow.ui \
    Forms/OrderEdit.ui \
    Forms/TruckEdit.ui \
    Forms/WarehouseEdit.ui

TRANSLATIONS += \
    locale/OctopusLogistics_ru.ts
    #locale/locale.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resources/Images.qrc \
    Core/Qml/qml.qrc \
    locale/translate.qrc

DISTFILES +=


