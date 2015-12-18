QT       += core gui widgets printsupport multimediawidgets sql

TARGET = WatchDog
TEMPLATE = app

INCLUDEPATH += C:\\opencv\\build\\include

debug {
LIBS += -LC:\\opencv\\build\\x64\\vc12\\lib \
	-lopencv_ts300d \
	-lopencv_world300d
}

release {
LIBS += -LC:\\opencv\\build\\x64\\vc12\\lib \
	-lopencv_ts300 \
	-lopencv_world300
}


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
    Camera.cpp \
    TimeLineWidget.cpp \
    VideoPlayer.cpp \
    MotionManager.cpp \
    VideoWidget.cpp

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
    Camera.h \
    TimeLineWidget.h \
    VideoPlayer.h \
    MotionManager.h \
    VideoWidget.h

FORMS    += MainWindow.ui \
    SettingsPage.ui \
    CameraPage.ui \
    ReviewPage.ui \
    CameraPagelet.ui \
    CameraConfigDlg.ui \
    VideoPlayer.ui

RESOURCES += \
    Resources.qrc

QMAKE_LFLAGS += /ignore:4819

DISTFILES += \
    Resources.rc
