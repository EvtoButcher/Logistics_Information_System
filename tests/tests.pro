QT += testlib sql positioning qml widgets

CONFIG += c++17 console testcase
CONFIG -= app_bundle

TEMPLATE = app
TARGET = tst_core

INCLUDEPATH += \
    ../Core \
    ../Core/AbstractModels \
    ../Core/ModelForQML \
    ../Common

HEADERS += \
    ../Core/ApplicationSettings.h \
    ../Core/OrderDB.h \
    ../Core/AbstractModels/AbstractMapItemModel.h \
    ../Core/AbstractModels/AbstractTrafficModel.h \
    ../Core/ModelForQML/RouteModel.h \
    ../Core/ModelForQML/CarSimulatorModel.h

SOURCES += \
    tst_core.cpp \
    ../Common/common.cpp \
    ../Core/ApplicationSettings.cpp \
    ../Core/OrderDB.cpp \
    ../Core/AbstractModels/AbstractMapItemModel.cpp \
    ../Core/AbstractModels/AbstractTrafficModel.cpp \
    ../Core/ModelForQML/RouteModel.cpp \
    ../Core/ModelForQML/CarSimulatorModel.cpp
