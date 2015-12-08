#include "CameraPage.h"
#include "MotionDetecter.h"
#include "MySettings.h"
#include "PipeLine.h"
#include "VideoSaver.h"
#include "VideoViewer.h"

#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/opencv.hpp>

CameraPage::CameraPage(QWidget *parent) :
    QWidget(parent)
{
    ui.setupUi(this);
    connect(ui.btOn, SIGNAL(toggled(bool)), this, SLOT(onTurnOn(bool)));
    onTurnOn(false);
}

void CameraPage::onTurnOn(bool on)
{
    if(!on)
    {
        _pipeLine.stop();
        ui.label->setPixmap(QPixmap(":/Images/CameraOff.png"));
        ui.btOn->setIcon(QIcon(":/Images/Red.png"));
        ui.btOn->setText("Off");
    }
    else
    {
        //"http://1:1@192.168.1.110:8888/videostream.asf"
        MySettings settings;
        _pipeLine.openDevice(0);

        MotionDetecter* motionDetecter = new MotionDetecter();
        motionDetecter->setThreshold(settings.getMinWidth(),
                                     settings.getMinHeight(),
                                     settings.getMinNumber());
        _pipeLine.addHandler(motionDetecter);

        _pipeLine.addHandler(new VideoViewer(ui.label));

        VideoSaver* videoSaver = new VideoSaver;
        videoSaver->config(settings.getStoragePath(),
                           VideoWriter::fourcc('D', 'I', 'V', 'X'),
                           settings.getFPS(),
                           _pipeLine.getFrameSize(),
                           settings.getStorageInterval());
        _pipeLine.addHandler(videoSaver);

        _pipeLine.start(1000 / MySettings().getFPS());

        ui.btOn->setIcon(QIcon(":/Images/Green.png"));
        ui.btOn->setText("On");
    }
}
