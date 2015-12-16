#include "VideoViewer.h"
#include <QDateTime>
#include <QLabel>
#include <opencv2\highgui.hpp>

VideoViewer::VideoViewer(QLabel* label)
{
    _label = label;
    resize(320, 240);
    setOriginalFrameSize(cv::Size(640, 480));
}

void VideoViewer::handleFrame(cv::Mat& frame, cv::Mat& previous)
{
    // timestamp
    QString time = QDateTime::currentDateTime().toString("MM/dd/yyyy HH:mm:ss");
    cv::putText(frame, time.toStdString(), cv::Point(10, 30), cv::FONT_HERSHEY_DUPLEX, 1, cv::Scalar(255, 255, 255));

    // show the frame
    cv::Mat resized;
    cv::resize(frame, resized, _size);
    cv::cvtColor(resized, resized, cv::COLOR_BGR2RGB);
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

void VideoViewer::setOriginalFrameSize(cv::Size size) {
    _originalSize = size;
}
