#pragma once
#include "FrameHandler.h"

class QLabel;

class VideoViewer: public FrameHandler
{
public:
    VideoViewer(QLabel* label);
    void handleFrame(cv::Mat& frame, cv::Mat& previous);
    void resize(int width, int height);
    void setOriginalFrameSize(cv::Size size);

private:
    QLabel*     _label;
    cv::Size    _size;
    cv::Size    _originalSize;
};

