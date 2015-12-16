#include "VideoSaver.h"
#include <QDateTime>
#include <QObject>

void VideoSaver::config(const QString& path, int fourcc, int fps, cv::Size frameSize, int interval)
{
    _path       = path;
    _fileName   = generateFileName();
    _fourcc     = fourcc;
    _fps        = fps;
    _frameSize  = frameSize;
    _interval   = interval;
    _frameCount = 0;
    _videoWriter.open(_fileName.toStdString(), _fourcc, _fps, _frameSize);
}

void VideoSaver::handleFrame(cv::Mat& frame, cv::Mat& previous)
{
    _videoWriter << frame;
    if(++_frameCount > _interval * 60 * _fps)
        config(_path, _fourcc, _fps, _frameSize, _interval);
}

QString VideoSaver::generateFileName() {
    return QObject::tr("%1/%2.avi")
            .arg(_path)
            .arg(QDateTime::currentDateTime().toMSecsSinceEpoch());
}
