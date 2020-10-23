!contains(INCLUDEDFIES, sy_logger.pri) {
INCLUDEDFIES += sy_logger.pri

include($$PWD/../sy_global/sy_global.pri)
include($$PWD/../sy_utils/sy_utils.pri)

HEADERS += \
    $$PWD/sy_logger.h

SOURCES += \
    $$PWD/sy_logger.cpp

DEPENDPATH += $$PWD
INCLUDEPATH += $$PWD

include($$PWD/log4qt/log4qt.pri)
}
