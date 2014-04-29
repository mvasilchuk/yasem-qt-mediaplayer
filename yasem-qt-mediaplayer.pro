#-------------------------------------------------
#
# Project created by QtCreator 2014-02-06T12:20:01
#
#-------------------------------------------------

QT       += multimedia multimediawidgets opengl

QT       -= gui

CONFIG += C++11

TARGET = QtMediaPlayer
TEMPLATE = lib
CONFIG += plugin

DEFINES += QTMEDIAPLAYER_LIBRARY

INCLUDEPATH += ../yasem-core \
    ../vlc-qt/build/prefix/include/

SOURCES += qtmediaplayer.cpp \
    qtcustomvideowidget.cpp

HEADERS += qtmediaplayer.h\
        qtmediaplayer_global.h \
        qtcustomvideowidget.h \
        ../yasem-core/mediasignalsender.h

#LIBS        += -L/home/max/Dropbox/projects/yasem/vlc-qt/build/prefix/lib/ -lvlc-qt -lvlc-qt-widgets

unix {
    target.path = /usr/lib
    INSTALLS += target
}

OTHER_FILES += \
    metadata.json

include(../common.pri)
DESTDIR = $$DEFAULT_PLUGIN_DIR

