#pragma once
#include <opencv2/opencv.hpp>
#include <QString>
#include "FrameHandler.h"

class VideoSaver: public FrameHandler
{
public:
    void config(const QString& path, int fourcc, int fps, cv::Size frameSize, int interval);
    void handleFrame(cv::Mat& frame, cv::Mat& previous);

private:
    cv::VideoWriter _videoWriter;
    int             _frameCount;
    QString         _path;
    QString         _fileName;
    int             _fourcc;
    int             _fps;
    cv::Size        _frameSize;
    int             _interval;
};

