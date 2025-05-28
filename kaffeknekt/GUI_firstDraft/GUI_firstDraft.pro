QT       += core gui widgets
QT       += core gui
QT       += serialport

QT += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    coffeeinstructionsdialog.cpp \
    datafetcher.cpp \
    graphdialog.cpp \
    graphview.cpp \
    infodetaildialog.cpp \
    klokke.cpp \
    main.cpp \
    mainwindow.cpp \
    onedaydialog.cpp \
    onemonthdialog.cpp \
    oneweekdialog.cpp \
    optionsdialog.cpp \
    sensoranalyticsdialog.cpp \
    settingsdialog.cpp \
    statistics.cpp \
    warningdialog.cpp \
    websocketclient.cpp

HEADERS += \
    coffeeinstructionsdialog.h \
    datafetcher.h \
    graphdialog.h \
    graphview.h \
    infodetaildialog.h \
    klokke.h \
    mainwindow.h \
    onedaydialog.h \
    onemonthdialog.h \
    oneweekdialog.h \
    optionsdialog.h \
    sensoranalyticsdialog.h \
    settingsdialog.h \
    statistics.h \
    warningdialog.h \
    websocketclient.h

FORMS += \
    coffeinstructionsdialog.ui \
    graphdialog.ui \
    infodetaildialog.ui \
    klokke.ui \
    mainwindow.ui \
    onedaydialog.ui \
    onemonthdialog.ui \
    oneweekdialog.ui \
    optionsdialog.ui \
    sensoranalyticsdialog.ui \
    settingsdialog.ui \
    statistics.ui \
    warningdialog.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
    # resources.qrc

DISTFILES += \
    ../../../../kaffemaskina.webp \
    Bilder_ikoner/SpyBot.png \
    Bilder_ikoner/battery-charging.svg \
    Bilder_ikoner/beans.jpg \
    Bilder_ikoner/beans1.png \
    Bilder_ikoner/calendar.svg \
    Bilder_ikoner/clock.svg \
    Bilder_ikoner/coffee.svg \
    Bilder_ikoner/droplet.svg \
    Bilder_ikoner/espresso.jpg \
    Bilder_ikoner/espresso1.jpg \
    Bilder_ikoner/home.svg \
    Bilder_ikoner/info.svg \
    Bilder_ikoner/kaffeknekt_logo.png \
    Bilder_ikoner/kaffemaskina.webp \
    Bilder_ikoner/menu.svg \
    Bilder_ikoner/pie-chart.svg \
    Bilder_ikoner/qr_codeInstagram.png \
    Bilder_ikoner/qr_codeWebsite.png \
    Bilder_ikoner/semcon_logo.webp \
    Bilder_ikoner/settings.svg \
    Bilder_ikoner/thermometer.svg \
    Bilder_ikoner/Activity_Diagram.drawio.png \
    Bilder_ikoner/Electrical_Diagram.drawio.png \
    Bilder_ikoner/Sequence_Diagram.drawio.png

    socketdata.pri
