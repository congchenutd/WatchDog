#include "MainWindow.h"
#include "MotionDetecter.h"
#include "PipeLine.h"
#include "VideoSaver.h"
#include "VideoViewer.h"
#include <QApplication>

#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/opencv.hpp>

//#include <QJsonObject>
//#include <QSettings>

using namespace cv;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return app.exec();

//    PipeLine* pipeline = new PipeLine;
//    pipeline->openDevice("http://1:1@192.168.1.5:8888/videostream.asf");
//    QLabel* label = new QLabel();
//    pipeline->addHandler(new VideoViewer(label));
//    label->resize(640, 480);
//    label->show();
//    pipeline->start();

//    return app.exec();

//    VideoCapture cap("http://192.168.1.9/axis-cgi/mjpg/video.cgi.mjpg");
////    VideoCapture cap("rtsp://admin:w0r3o3i9@192.168.1.9:554/cam/realmonitor?channel=1&subtype=1");
//    if (cap.isOpened())
//        return 0;
//    return 1;
}
