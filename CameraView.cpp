#include "CameraView.h"
#include "MotionDetecter.h"
#include "MySettings.h"
#include "PipeLine.h"
#include "VideoSaver.h"
#include "VideoViewer.h"

#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/opencv.hpp>

CameraView::CameraView(const QString& url, QWidget* parent) :
    QWidget(parent)
{
    ui.setupUi(this);
    _url            = url;
    _motionDetector = new MotionDetector();
    _videoViewer    = new VideoViewer(ui.label);
    _videoSaver     = new VideoSaver;
    _pipeLine.addHandler(_motionDetector);
    _pipeLine.addHandler(_videoViewer);
//    _pipeLine.addHandler(_videoSaver);

    turnOn(false);
}

void CameraView::turnOn(bool on)
{
    if(!on)
    {
        _pipeLine.stop();
        ui.label->hide();
    }
    else
    {
        ui.label->show();
        //"http://1:1@192.168.1.110:8888/videostream.asf"
        MySettings settings;
//        if (_url.isEmpty())
            _pipeLine.openDevice(0);
//        else
//            _pipeLine.openDevice(settings.getUrl().toStdString());

        _motionDetector->setThreshold(settings.getMinWidth(),
                                      settings.getMinHeight(),
                                      settings.getMinNumber());

        if (!settings.getStoragePath().isEmpty())
            _videoSaver->config(settings.getStoragePath(),
                                VideoWriter::fourcc('D', 'I', 'V', 'X'),
                                settings.getFPS(),
                                _pipeLine.getFrameSize(),
                                settings.getStorageInterval());

        _pipeLine.start(MySettings().getFPS());
    }
}
