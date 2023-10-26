QT       += core gui sql concurrent positioning

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets quickwidgets

CONFIG += c++17

HEADERS += \
    Headers/ApplicationSettings.h \
    Headers/Company.h \
    Headers/CreateCompanyDialog.h \
    Headers/CreateWarehouseDialog.h \
    Headers/MapItemEngine.h \
    Headers/OrderAddDialog.h \
    Headers/OrderAddWidget.h \
    Headers/OrderDB.h \
    Headers/OrderTableWidget.h \
    Headers/RouteModel.h \
    Headers/TableDelegate.h \
    Headers/TextMessage.h \
    Headers/WarehouseControlWidget.h \
    Headers/WarehouseModel.h \
    Headers/common.h \
    Headers/mainwindow.h

SOURCES += \
    Sources/ApplicationSettings.cpp \
    Sources/Company.cpp \
    Sources/CreateCompanyDialog.cpp \
    Sources/CreateWarehouseDialog.cpp \
    Sources/MapItemEngine.cpp \
    Sources/OrderAddDialog.cpp \
    Sources/OrderAddWidget.cpp \
    Sources/OrderDB.cpp \
    Sources/OrderTableWidget.cpp \
    Sources/RouteModel.cpp \
    Sources/TableDelegate.cpp \
    Sources/WarehouseControlWidget.cpp \
    Sources/WarehouseModel.cpp \
    Sources/common.cpp \
    Sources/main.cpp \
    Sources/mainwindow.cpp

FORMS += \
    Forms/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resources/Images.qrc \
    Resources/qml.qrc

