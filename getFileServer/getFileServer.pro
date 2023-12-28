QT += core network
QT += gui

CONFIG += c++11

TARGET = getFileServer
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    widget.cpp

HEADERS += \
    widget.h
