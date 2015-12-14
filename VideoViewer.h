#pragma once
#include "FrameHandler.h"

class QLabel;

class VideoViewer: public FrameHandler
{
public:
    VideoViewer(QLabel* label);
    void handleFrame(cv::Mat& frame, cv::Mat& previous);

private:
    QLabel* _label;
};

