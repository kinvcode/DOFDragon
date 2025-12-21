QT       += core gui

RC_ICONS = assets/images/icons/abc.ico

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

OPENCV_PATH = D:/CodeRepos/3th/opencv/           #opencv目录

INCLUDEPATH +=  $${OPENCV_PATH}/include \
                $$PWD/include \
                D:/CodeRepos/CPP/vcpkg/installed/x64-windows-static/include \

CONFIG(debug, debug|release) {
    LIBS += $${OPENCV_PATH}/x64/vc16/lib/opencv_core460d.lib \
            $${OPENCV_PATH}/x64/vc16/lib/opencv_imgproc460d.lib \
            $${OPENCV_PATH}/x64/vc16/lib/opencv_imgcodecs460d.lib
}

# Release 模式下的配置
CONFIG(release, debug|release) {
    LIBS += $${OPENCV_PATH}/x64/vc16/lib/opencv_core460.lib \
            $${OPENCV_PATH}/x64/vc16/lib/opencv_imgproc460.lib \
            $${OPENCV_PATH}/x64/vc16/lib/opencv_imgcodecs460.lib
}

LIBS += $$PWD/library/msdk/msdkLib.lib \
        $$PWD/library/msdk/hid.lib \
        $$PWD/library/vmp/VMProtectSDK64.lib \
        $$PWD/library/openssl/libssl.lib \
        $$PWD/library/openssl/libcrypto.lib \

SOURCES += \
    driver/driverload.cpp \
    driver/mgapi.cpp \
    global.cpp \
    helper/rand.cpp \
    logviewer.cpp \
    main.cpp \
    mainwindow.cpp \
    mem.cpp \
    noticewidget.cpp \
    process/process.cpp

HEADERS += \
    baseAddress.h \
    dataStruct.h \
    driver/driverload.h \
    driver/inc.h \
    driver/mgapi.h \
    global.h \
    helper/rand.h \
    logviewer.h \
    mainwindow.h \
    mem.h \
    noticewidget.h \
    process/process.h \
    vmp/VMProtectSDK.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc
