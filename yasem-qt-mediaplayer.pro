#-------------------------------------------------
#
# Project created by QtCreator 2014-02-06T12:20:01
#
#-------------------------------------------------

VERSION = 0.1.0
TARGET = yasem-qt-mediaplayer
TEMPLATE = lib

include($${top_srcdir}/common.pri)

QT       += multimedia multimediawidgets opengl
QT       -= gui

DEFINES += QTMEDIAPLAYER_LIBRARY

SOURCES += \
    qtcustomvideowidget.cpp \
    qtplayerplugin.cpp \
    qtmediaplayerobject.cpp

HEADERS +=\
    qtmediaplayer_global.h \
    qtcustomvideowidget.h \
    qtplayerplugin.h \
    qtmediaplayerobject.h

win32: {
HEADERS +=\
    $${SDK_DIR}/mediaplayer.h
}

#LIBS        += -L/home/max/Dropbox/projects/yasem/vlc-qt/build/prefix/lib/ -lvlc-qt -lvlc-qt-widgets

unix {
    target.path = /usr/lib
    INSTALLS += target
}

OTHER_FILES += \
    metadata.json

