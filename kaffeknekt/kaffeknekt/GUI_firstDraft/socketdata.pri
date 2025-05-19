# ─── socketdata.pri ───

CONFIG   += console c++17 thread   # legg til thread for QThread-støtte

QT       += core

# Kildene dine
SOURCES  += \
    sources/main.cpp \
    sources/websocket.cpp

# Header-filer som skal med (ingen ui-filer her)
HEADERS  += \
    sources/websocket.h

# Unix sockets + pthread
unix: LIBS += -lpthread

# Om du har nlohmann/json.hpp et annet sted (f.eks. vendor/):
#INCLUDEPATH += $$PWD/thirdparty/json/include
