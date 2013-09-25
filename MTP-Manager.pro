#-------------------------------------------------
#
# Project created by QtCreator 2010-09-12T15:01:54
#
#-------------------------------------------------

QT       += core gui declarative
TARGET = MTP-Manager
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    MTPhandler.cpp \
    treeitem.cpp \
    treemodel.cpp \
    devfilesview.cpp \
    mtputils/pathutils.cpp

HEADERS  += mainwindow.h \
    MTPhandler.h \
    treeitem.h \
    treemodel.h \
    devfilesview.h \
    mtputils/pathutils.h

FORMS    += \
    mainwindow.ui
LIBS += -lmtp -ltag
