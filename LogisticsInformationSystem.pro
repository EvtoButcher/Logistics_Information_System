QT       += core gui sql concurrent positioning

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets quickwidgets

CONFIG += c++17

SOURCES += \
    OrderAddDialog.cpp \
    OrderAddWidget.cpp \
    OrderDB.cpp \
    OrderTableWidget.cpp \
    RouteModel.cpp \
    TableDelegate.cpp \
    common.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    OrderAddDialog.h \
    OrderAddWidget.h \
    OrderDB.h \
    OrderTableWidget.h \
    RouteModel.h \
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
