TEMPLATE = app
TARGET = chatterboxd
DEPENDPATH += .
INCLUDEPATH += .

#CONFIG += console

QT += network widgets

HEADERS += ChatterBoxServer.h
SOURCES += ChatterBoxServer.cpp main.cpp
