#-------------------------------------------------
#
# Project created by QtCreator 2019-08-04T22:20:56
#
#-------------------------------------------------

QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = client
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


SOURCES += main.cpp\
        widget.cpp \
    init_interface.cpp \
    login_interface.cpp \
    reg_interface.cpp \
    alonegame_interface.cpp \
    online_interface.cpp \
    battlegame_interface.cpp \
    slots_function.cpp

HEADERS  += widget.h \
    init_interface.h \
    login_interface.h \
    reg_interface.h \
    alonegame_interface.h \
    online_interface.h \
    battlegame_interface.h \
    myhead.h \
    struct_head.h

FORMS    += widget.ui \
    init_interface.ui \
    login_interface.ui \
    reg_interface.ui \
    alonegame_interface.ui \
    online_interface.ui \
    battlegame_interface.ui
