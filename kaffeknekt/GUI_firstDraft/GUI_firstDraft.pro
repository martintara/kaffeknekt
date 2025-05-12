QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    coffeeinstructionsdialog.cpp \
    graphdialog.cpp \
    graphview.cpp \
    infodetaildialog.cpp \
    main.cpp \
    mainwindow.cpp \
    onedaydialog.cpp \
    onemonthdialog.cpp \
    oneweekdialog.cpp \
    optionsdialog.cpp \
    sensoranalyticsdialog.cpp \
    settingsdialog.cpp \
    warningdialog.cpp \
    websocketclient.cpp

HEADERS += \
    coffeeinstructionsdialog.h \
    graphdialog.h \
    graphview.h \
    infodetaildialog.h \
    mainwindow.h \
    onedaydialog.h \
    onemonthdialog.h \
    oneweekdialog.h \
    optionsdialog.h \
    sensoranalyticsdialog.h \
    settingsdialog.h \
    warningdialog.h \
    websocketclient.h

FORMS += \
    coffeinstructionsdialog.ui \
    graphdialog.ui \
    infodetaildialog.ui \
    mainwindow.ui \
    onedaydialog.ui \
    onemonthdialog.ui \
    oneweekdialog.ui \
    optionsdialog.ui \
    sensoranalyticsdialog.ui \
    settingsdialog.ui \
    warningdialog.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc \
    # resources.qrc

DISTFILES += \
    ../../../../kaffemaskina.webp \
    socketdata.pri
