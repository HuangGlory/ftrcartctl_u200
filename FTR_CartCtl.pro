QT -= gui
QT += serialport network

CONFIG += c++11 console
CONFIG -= app_bundle


TARGET = ftrCartCtl_V0.0.23.00.U200@20210608

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    BLDC_SpeedCtl.cpp \
    CreateMap.cpp \
    Pipe_Operate.cpp \
    RC_StateCtl.cpp \
    SB_StateCtl.cpp \
    VTK_StateCtl.cpp \
    VTP_StateCtl.cpp \
    cartstatesetting.cpp \
    clearpipe_thread.cpp \
    ftr_ctr3speedctl.cpp \
    imu.cpp \
    kalman.cpp \
    licensecheck.cpp \
    main.cpp \
    mapinfo.cpp \
    motorctrlinfo.cpp \
    rc_thread.cpp \
    readinputpipe_thread.cpp \
    readvtkpipe_thread.cpp \
    readvtppipe_thread.cpp \
    sb_thread.cpp \
    stationattribute.cpp \
    tcp_server.cpp \
    tcp_server_private.cpp \
    uwb_aoa.cpp \
    vtk_thread.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

LIBS += -lwiringPi

HEADERS += \
    PlatformParameter.h \
    cartstatesetting.h \
    clearpipe_thread.h \
    ftr_ctr3speedctl.h \
    imu.h \
    kalman.h \
    licensecheck.h \
    mapinfo.h \
    motorctrlinfo.h \
    raspi-config.h \
    rc_thread.h \
    readinputpipe_thread.h \
    readvtkpipe_thread.h \
    readvtppipe_thread.h \
    sb_thread.h \
    stationattribute.h \
    streamlitApp.h \
    tcp_server.h \
    tcp_server_private.h \
    updateScript.h \
    uwb_aoa.h \
    vtk_thread.h \
    wifiConf.h

DISTFILES +=

RESOURCES += \
    file.qrc
