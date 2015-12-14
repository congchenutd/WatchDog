#include "VideoViewer.h"
#include <QDateTime>
#include <QLabel>
#include <opencv2\highgui.hpp>

using namespace cv;

VideoViewer::VideoViewer(QLabel* label)
{
    _label = label;
    resize(320, 240);
    setOriginalFrameSize(Size(640, 480));
}

void VideoViewer::handleFrame(Mat& frame, Mat& previous)
{
    // timestamp
    QString time = QDateTime::currentDateTime().toString("MM/dd/yyyy HH:mm:ss");
    putText(frame, time.toStdString(), Point(10, 30), FONT_HERSHEY_DUPLEX, 1, Scalar(255, 255, 255));

    // show the frame
    Mat resized;
    cv::resize(frame, resized, _size);
    cvtColor(resized, resized, COLOR_BGR2RGB);
    QImage image((uchar*) resized.data, resized.cols, resized.rows, resized.step, QImage::Format_RGB888);
    _label->setPixmap(QPixmap::fromImage(image));
}

void VideoViewer::resize(int width, int height)
{
    double originalAspectRatio = (double) _originalSize.width / _originalSize.height;
    double aspectRatio = (double) width / height;
    if (aspectRatio >= originalAspectRatio)
    {
        _size.height = height;
        _size.width  = height * originalAspectRatio;
    }
    else
    {
        _size.width  = width;
        _size.height = width / originalAspectRatio;
    }
}

void VideoViewer::setOriginalFrameSize(Size size) {
    _originalSize = size;
}
