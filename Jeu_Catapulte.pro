#-------------------------------------------------
#
# Project created by QtCreator 2017-04-17T21:17:03
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Jeu_Catapulte
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    src/.cpp/main.cpp \
    src/.cpp/catapulte.cpp \
    src/.cpp/gameround.cpp \
    src/.cpp/gamesetting.cpp \
    src/.cpp/scenegl.cpp \
    src/.cpp/catapultestatus.cpp

HEADERS  += \
    src/.h/catapulte.h \
    src/.h/gameround.h \
    src/.h/gamesetting.h \
    src/.h/enum_level.h \
    src/.h/scenegl.h \
    src/.h/catapultestatus.h

FORMS    += \
    src/.ui/catapulte.ui \
    src/.ui/gamesetting.ui

INCLUDEPATH += $$(OPENCV_DIR)\..\..\include \
               $$PWD/src/.h

LIBS += -L$$(OPENCV_DIR)\lib \
    -lopencv_core2413 \
    -lopencv_highgui2413 \
    -lopencv_imgproc2413 \
    -lopencv_features2d2413 \
    -lopencv_calib3d2413 \
    -lOpengl32 -lglu32

RESOURCES += \
    res/texture_image.qrc

# RC_ICONS = /res/icon/catapulte.ico
RC_FILE = ./res/icon/icon.rc
