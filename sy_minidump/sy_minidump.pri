!contains(INCLUDEDFIES, sy_minidump.pri) {
INCLUDEDFIES += sy_minidump.pri

HEADERS += \
    $$PWD/sy_minidump.h

SOURCES += \
    $$PWD/sy_minidump.cpp


INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD
}
