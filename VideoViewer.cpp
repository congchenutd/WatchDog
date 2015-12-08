#include "VideoViewer.h"
#include <QDateTime>
#include <QLabel>
#include <opencv2\highgui.hpp>

using namespace cv;

VideoViewer::VideoViewer(QLabel* label)
{
    _label = label;
}

void VideoViewer::handleFrame(Mat& frame)
{
    // timestamp
    QString time = QDateTime::currentDateTime().toString("MM/dd/yyyy HH:mm:ss");
    putText(frame, time.toStdString(), Point(10, 30), FONT_HERSHEY_DUPLEX, 1, Scalar(255, 255, 255));

    // show the frame
    Mat temp;
    cvtColor(frame, temp, COLOR_BGR2RGB);
    QImage image((uchar*) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
    _label->setPixmap(QPixmap::fromImage(image));
}
