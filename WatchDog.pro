QT       += core gui widgets

TARGET = WatchDog
TEMPLATE = app

INCLUDEPATH += C:\\opencv\\build\\include
LIBS += -LC:\\opencv\\build\\x64\\vc12\\lib \
	-lopencv_ts300 \
	-lopencv_world300

win32 {
	RC_FILE = Resources.rc
}

SOURCES +=\
        MainWindow.cpp \
    SettingsPage.cpp \
    Main.cpp \
    FrameHandler.cpp \
    MotionDetecter.cpp \
    PipeLine.cpp \
    VideoSaver.cpp \
    VideoViewer.cpp \
    MySettings.cpp \
    CameraPage.cpp \
    ReviewPage.cpp \
    CameraPagelet.cpp \
    CameraConfigDlg.cpp \
    CameraView.cpp \
    Camera.cpp

HEADERS  += MainWindow.h \
    SettingsPage.h \
    FrameHandler.h \
    MotionDetecter.h \
    PipeLine.h \
    VideoSaver.h \
    VideoViewer.h \
    MySettings.h \
    CameraPage.h \
    ReviewPage.h \
    CameraPagelet.h \
    CameraConfigDlg.h \
    CameraView.h \
    Camera.h

FORMS    += MainWindow.ui \
    SettingsPage.ui \
    CameraPage.ui \
    ReviewPage.ui \
    CameraPagelet.ui \
    CameraConfigDlg.ui \
    CameraView.ui

RESOURCES += \
    Resources.qrc

QMAKE_LFLAGS += /ignore:4819

DISTFILES += \
    Resources.rc
