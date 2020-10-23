QT += quick qml network serialport

android{
QT += androidextras
}
CONFIG += c++11
SYSTEM_NAME = gscontrol-pc

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

RC_ICONS = tvwall.ico

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += SY_APP_TYPE="\\\"gscontrol-pc\\\""


# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    device_ctrl_com_base.cpp \
    device_ctrl_tcp_type_7.cpp \
    device_ctrl_tcp_type_8.cpp \
    device_ctrl_udp_base.cpp \
        main.cpp \
    acc_conf_info.cpp \
    cmd_info.cpp \
    data_info.cpp \
    data_model.cpp \
    device_ctrl_base.cpp \
    device_ctrl_com.cpp \
    device_ctrl_tcp.cpp \
    device_ctrl_tcp_base.cpp \
    device_ctrl_tcp_type_0.cpp \
    device_ctrl_tcp_type_1.cpp \
    device_ctrl_tcp_type_2.cpp \
    device_ctrl_tcp_type_5.cpp \
    device_ctrl_tcp_type_5_static.cpp \
    device_ctrl_tcp_type_6.cpp \
    device_info.cpp \
    device_thread.cpp \
    device_worker.cpp \
    room_check.cpp \
    room_com.cpp \
    room_databuff.cpp \
    room_info.cpp \
    room_manager.cpp \
    room_signal_sync_thread.cpp \
    room_signal_sync_worker.cpp \
    room_thread.cpp \
    room_worker.cpp \
    secret_key.cpp \
    signal_resource_info.cpp \
    signal_resource_thread.cpp \
    signal_resource_worker.cpp \
    video_item.cpp

RESOURCES += qml.qrc \
    translator.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


msvc:QMAKE_CXXFLAGS += -source-charset:utf-8

win32{
    LIBS += -lbcrypt -lws2_32 -lsecur32
}



include($$PWD/sy_project_base/sy_project_base.pri)
include($$PWD/sy_logger/sy_logger.pri)
include($$PWD/sy_utils/sy_utils.pri)
include($$PWD/sy_global/sy_global.pri)
include($$PWD/sy_minidump/sy_minidump.pri)

HEADERS += \
    acc_conf_info.h \
    cmd_info.h \
    data_info.h \
    data_model.h \
    device_ctrl_base.h \
    device_ctrl_com.h \
    device_ctrl_com_base.h \
    device_ctrl_tcp.h \
    device_ctrl_tcp_base.h \
    device_ctrl_tcp_type_0.h \
    device_ctrl_tcp_type_1.h \
    device_ctrl_tcp_type_2.h \
    device_ctrl_tcp_type_5.h \
    device_ctrl_tcp_type_5_static.h \
    device_ctrl_tcp_type_6.h \
    device_ctrl_tcp_type_7.h \
    device_ctrl_tcp_type_8.h \
    device_ctrl_udp_base.h \
    device_info.h \
    device_thread.h \
    device_worker.h \
    room_check.h \
    room_com.h \
    room_databuff.h \
    room_info.h \
    room_manager.h \
    room_signal_sync_thread.h \
    room_signal_sync_worker.h \
    room_thread.h \
    room_worker.h \
    secret_key.h \
    signal_resource_info.h \
    signal_resource_thread.h \
    signal_resource_worker.h \
    video_item.h


UI_DIR=./UI

#TRANSLATIONS += \
#os_language_English.ts \
#   os_language_ChineseSimpilifed.ts \


#lupdate_only{
#    SOURCES +=
#*.qml\

#}

DISTFILES += \
    android/AndroidManifest.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew \
    android/gradlew.bat \
    android/res/values/libs.xml

contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
    ANDROID_PACKAGE_SOURCE_DIR = \
        $$PWD/android
}



