#-------------------------------------------------
#
# Project created by QtCreator 2019-01-19T22:02:34
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = glshadow
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    oglwidget.cpp \
    graphicsitem.cpp \
    graphicsorigin.cpp \
    graphicslines.cpp \
    graphicscube.cpp \
    stlloader.cpp \
    graphicstriangles.cpp \
    openglvertex.cpp \
    graphicsbillboard.cpp \
    graphicsshadow.cpp

HEADERS  += mainwindow.h \
    oglwidget.h \
    graphicsitem.h \
    graphicsorigin.h \
    graphicslines.h \
    graphicscube.h \
    stlloader.h \
    graphicstriangles.h \
    facet.h \
    openglvertex.h \
    graphicsbillboard.h \
    materialproperty.h \
    lightproperty.h \
    graphicsshadow.h

#win32:LIBS += -lOpengl32 \
#                -lglu32 \
#                -lglut
win32:LIBS += -lOpengl32
FORMS    += mainwindow.ui

RESOURCES += \
    file.qrc

#DISTFILES += QT_OPENGL_ES_2

DISTFILES +=
