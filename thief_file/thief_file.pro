QT += core network
QT += gui

CONFIG += c++11

TARGET = thief_file
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    fileupload.cpp

HEADERS += \
    fileupload.h
