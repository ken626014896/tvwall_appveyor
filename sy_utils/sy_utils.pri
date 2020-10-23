!contains(INCLUDEDFIES, sy_utils.pri) {
INCLUDEDFIES += sy_utils.pri

QT += network
DEFINES += DEF_sy_UTILS


HEADERS += \
    $$PWD/sy_utils.h

SOURCES += \
    $$PWD/sy_utils.cpp

win32:LIBS += -luser32 -lws2_32 -ldbghelp \
-lkernel32 \
-lgdi32 \
-lwinspool \
-lcomdlg32 \
-ladvapi32 \
-lshell32 \
-lole32 \
-loleaut32 \
-luuid \
-lodbc32 \
-lodbccp32

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD
}
