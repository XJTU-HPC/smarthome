QT       += core gui network mqtt sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

QMAKE_CXXFLAGS += -fopenmp
QMAKE_LFLAGS += -fopenmp -s -ldl -lpthread
LIBS += -fopenmp -lgomp
INCLUDEPATH += /home/zyh/smarthome/opencv/include/opencv4 \
               /home/zyh/smarthome/ncnn/include/ncnn \
               /home/zyh/libgpiod/include

LIBS += /home/zyh/smarthome/opencv/lib/libopencv_*.so \
        /home/zyh/smarthome/ncnn/lib/libncnn.a \
        /opt/qt5.15.8/lib/libQt5Test.so.5.15.8 \
        /home/zyh/libgpiod/lib/libgpiod.so

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
    TArcface.cpp \
    TBlur.cpp \
    TLive.cpp \
    TMtCNN.cpp \
    TRetina.cpp \
    TWarp.cpp \
    chatitemdelegate.cpp \
    erniellm.cpp \
    facerecthread.cpp \
    collentdatathread.cpp \
    login.cpp \
    main.cpp \
    mainwindow.cpp \
    my_lineedit.cpp \
    objdetectthread.cpp \
    readpe15thread.cpp \
    register.cpp \
    set_wifi.cpp \
    speechrecthread.cpp \
    syszuxpinyin.cpp

HEADERS += \
    TArcface.h \
    TBlur.h \
    TLive.h \
    TMtCNN.h \
    TRetina.h \
    TWarp.h \
    chatitemdelegate.h \
    erniellm.h \
    facerecthread.h \
    collentdatathread.h \
    login.h \
    mainwindow.h \
    my_lineedit.h \
    objdetectthread.h \
    readpe15thread.h \
    register.h \
    set_wifi.h \
    speechrecthread.h \
    syszuxpinyin.h

FORMS += \
    login.ui \
    mainwindow.ui \
    register.ui \
    set_wifi.ui \
    syszuxpinyin.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /home/user/tanbaby/smarthome
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icon.qrc \
    images/images.qrc \
    pic.qrc \
    qtr.qrc
