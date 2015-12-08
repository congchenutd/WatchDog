#pragma once
#include <opencv2/opencv.hpp>
#include <QString>
#include "FrameHandler.h"

using namespace cv;

class VideoSaver: public FrameHandler
{
public:
    void config(const QString& path, int fourcc, int fps, Size frameSize, int interval);
    void handleFrame(Mat& frame);

private:
    QString generateFileName();

private:
	VideoWriter _videoWriter;
    int         _frameCount;
    QString     _path;
    QString     _fileName;
    int         _fourcc;
    int         _fps;
    Size        _frameSize;
    int         _interval;
};

