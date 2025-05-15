QT += widgets serialport

CONFIG += c++17
TEMPLATE = app

SOURCES += \
    main.cpp \
    clock_time.cpp

HEADERS += \
    clock_time.h

FORMS += \
    clock_time.ui

win32:CONFIG += console
macx {
    QMAKE_INFO_PLIST = Info.plist
    QMAKE_BUNDLE = 1
}

VERSION = 0.1
