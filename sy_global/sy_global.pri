!contains(INCLUDEDFIES, sy_global.pri) {
INCLUDEDFIES += sy_global.pri

HEADERS += \
    $$PWD/sy_global.h

SOURCES += \
    $$PWD/sy_global.cpp


INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD
}


