QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets quickwidgets

CONFIG += c++17

SOURCES += \
    RouteControlWidget.cpp \
    RouteDB.cpp \
    RouteModel.cpp \
    RouteTableWidget.cpp \
    TableDelegate.cpp \
    common.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    RouteControlWidget.h \
    RouteDB.h \
    RouteModel.h \
    RouteTableWidget.h \
    TableDelegate.h \
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
