QT += core gui network

win32:CONFIG += static

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = android-barcode-scanner-client
TEMPLATE = app

SOURCES += main.cpp \
           mainwindow.cpp

HEADERS  += mainwindow.h \
            config.h

RESOURCES += resources.qrc

win32:RC_FILE = barcode.rc
win64:RC_FILE = barcode.rc

OTHER_FILES += barcode.rc
