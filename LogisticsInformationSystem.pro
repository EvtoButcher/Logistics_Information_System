QT       += core gui sql concurrent positioning

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets quickwidgets

CONFIG += c++17

INCLUDEPATH += \
    ui \
    Core \
    Core/ModelForQML \
    Core/Qml \
    Common \
    Resources \


HEADERS += \
    Core/ApplicationSettings.h \
    Core/Company.h \
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
    Core/ApplicationSettings.cpp \
    Core/Company.cpp \
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
    Forms/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resources/Images.qrc \
    Core/Qml/qml.qrc

