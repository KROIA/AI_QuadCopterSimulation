QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#CONFIG+=BUILD_WITH_EASY_PROFILER



include(Extern/Neural-net-2-graphics/Neural-net-2-graphics.pri)
#include(../Neural-net-2-graphics/Neural-net-2-graphics.pri)

inc = $$PWD/inc
src = $$PWD/src


INCLUDEPATH +=  $$inc

HEADERS += \
    $$inc/mainwindow.h \
    inc/AI_Controller.h \
    inc/AI_trainer.h \
    inc/Force.h \
    inc/Motor2D.h \
    inc/PID.h \
    inc/PID_Controller.h \
    inc/QuadCopter2D.h \
    inc/QuadCopterFrame2D.h \
    inc/SimulationSettings.h

SOURCES += \
    $$src/main.cpp \
    $$src/mainwindow.cpp \
    src/AI_Controller.cpp \
    src/AI_Trainer.cpp \
    src/Force.cpp \
    src/Motor2D.cpp \
    src/PID.cpp \
    src/PID_Controller.cpp \
    src/QuadCopter2D.cpp \
    src/QuadCopterFrame2D.cpp \
    src/SimulationSettings.cpp


FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
