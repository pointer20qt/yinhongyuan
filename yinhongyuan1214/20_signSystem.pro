#-------------------------------------------------
#
# Project created by QtCreator 2020-02-06T19:35:34
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 20_signSystem
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    person.cpp \
    login.cpp \
    regist.cpp

HEADERS  += widget.h \
    person.h \
    login.h \
    regist.h

FORMS    += widget.ui \
    person.ui \
    login.ui \
    regist.ui
