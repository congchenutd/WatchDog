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

    //"http://1:1@192.168.1.110:8888/videostream.asf"

//    PipeLine* pipeline = new PipeLine;
//    pipeline->openDevice(0);
//    pipeline->addHandler(new MotionDetecter);
//    QLabel* label = new QLabel();
//    pipeline->addHandler(new VideoViewer(label));
//    label->resize(640, 480);
//    label->show();
//    pipeline->start();

//    return app.exec();
}
