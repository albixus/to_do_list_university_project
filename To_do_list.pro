#-------------------------------------------------
#
# Project created by QtCreator 2017-10-30T20:54:22
#
#-------------------------------------------------

QT       += core gui
QT += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = To_do_list
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
RC_FILE = icon.rc
# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    add.cpp \
    accept.cpp \
    stats.cpp \
    edit_priority.cpp \
    edit_date.cpp \
    edit_text.cpp

HEADERS  += mainwindow.h \
    add.h \
    accept.h \
    stats.h \
    edit_priority.h \
    edit_date.h \
    edit_text.h

FORMS    += mainwindow.ui \
    add.ui \
    accept.ui \
    stats.ui \
    edit_priority.ui \
    edit_date.ui \
    edit_text.ui

DISTFILES +=

RESOURCES += \
    resource.qrc

