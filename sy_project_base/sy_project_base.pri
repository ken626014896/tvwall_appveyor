!contains(INCLUDEDFIES, sy_project_base.pri) {
INCLUDEDFIES += sy_project_base.pri
#CONFIG += console

    CONFIG(debug, debug|release) {
        BUILD_TYPE = debug
    } else {
        BUILD_TYPE = release
#        DEFINES += QT_NO_WARNING_OUTPUT\
#                          QT_NO_DEBUG_OUTPUT
    }

    win32 {

    ## Windows common build here

    !contains(QMAKE_TARGET.arch, x86_64) {
        message("x86 build")

        ## Windows x86 (32bit) specific build here
        DESTDIR = $$PWD/../../../rel/$$BUILD_TYPE/$$SYSTEM_NAME/bin
    } else {
        message("x86_64 build")

        ## Windows x64 (64bit) specific build here
        DESTDIR = $$PWD/../../../rel/x64/$$BUILD_TYPE/$$SYSTEM_NAME/bin
    }

    QMAKE_LFLAGS_RELEASE += /MAP
    QMAKE_CFLAGS_RELEASE += /Zi
    QMAKE_LFLAGS_RELEASE += /debug /opt:ref
    }
}

