QT       += core gui sql concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets quickwidgets

CONFIG += c++17

SOURCES += \
    RouteAddDialog.cpp \
    RouteDB.cpp \
    RouteModel.cpp \
    RouteTableWidget.cpp \
    TableDelegate.cpp \
    common.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    RouteAddDialog.h \
    RouteDB.h \
    RouteModel.h \
    RouteTableWidget.h \
    TableDelegate.h \
    TextMessage.h \
    common.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    qml.qrc
