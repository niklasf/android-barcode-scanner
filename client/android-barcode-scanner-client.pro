#-------------------------------------------------
#
# Project created by QtCreator 2013-11-20T00:01:14
#
#-------------------------------------------------

QT       += core gui network

CONFIG += static

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = android-barcode-scanner-client
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

RESOURCES += \
    resources.qrc

win32:RC_FILE = barcode.rc
win64:RC_FILE = barcode.rc

OTHER_FILES += \
    barcode.rc
